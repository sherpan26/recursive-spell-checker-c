//spchk.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "dictionary_loader.h"  // load
#include <fcntl.h> // for open, O_RDONLY


int errorFound = 0; // 0 means no error, 1 means error found

// get if WORD is UPPER
int isUpperCase(char word[]) {
    for (int i = 0; word[i] != '\0'; i++) {
        if (!isupper(word[i])) {
            return 0; // Not all characters are uppercase
        }
    }
    return 1; // ALL are uppercase
}

//CONVERT TO LOWER
void toLowercase(char str[]) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}


int is_correct_word(char word[], int index) {
    if (strcmp(word, dictionary[index]) == 0) { // Check for normal match
        return 1; // Found it, go next
    } else if(isUpperCase(word)){   // if word is ALL upper case YES
        return 1; // Match found for all uppercase word
    } else if (isupper(word[0])) { // Check if first character is uppercase
        char word_initial_cap[50];
        strcpy(word_initial_cap, word);
        word_initial_cap[0] = tolower(word_initial_cap[0]); // Convert first character to lowercase
        if (strcmp(dictionary[index], word_initial_cap) == 0) {
            return 1; // Match found for initial capitalization
        }
    }
    return 0; // Not found
}

int processMatches(char *word, int *indices, int count) {
    int anyCorrectWordFound = 0; // Flag to indicate if any correct word is found

    for (int i = 0; i < count; i++) {
        if (is_correct_word(word, indices[i])) {
            anyCorrectWordFound = 1; // A correct word was found
            //break; // Exit the loop early as we only need one match
            return 1;
        }
    }

    // Here, you can decide what to do if any correct word was found
    if (!anyCorrectWordFound) {
        return 0; //none
    }
    return 0;
}


int search(char word[]) { // come from buffit
    char lowercaseWord[50]; // Assign
    char lowercaseDictWord[50]; // Assign
    int indices[1000]; // Array to store indices of matches
    int count = 0; // Count of matches

    strcpy(lowercaseWord, word);
    toLowercase(lowercaseWord);

    for (int i = 0; i < dictionarySize; i++) {
        strcpy(lowercaseDictWord, dictionary[i]);
        toLowercase(lowercaseDictWord);
        if (strcmp(lowercaseWord, lowercaseDictWord) == 0) {
            // If it's a match, check further if it's correct
            if (is_correct_word(word, i)) {
                indices[count++] = i; // Store the index of the match
            }
        }
    }
    int a = processMatches(word, indices, count); //get the indexes word to there

    return a; // Return if found
}

void callitBuf(char buffer[200], int line, int column, const char* filePAth) {
    char final[200]; // Store the processed word(s)
    int indexF = 0; // Index for final array
    int len = strlen(buffer); // Length of the input buffer
    
  // get line col len and word
  //  printf("Line: %d, Column: %d, Length: %d, Word: %s\n", line, column, len, buffer);

    // Find the first alphabetic character

    int start = 0;
    while (start < len && !isalpha(buffer[start])) {
        start++;
    }

    // Find the last alphabetic character
    int end = len - 1;
    while (end >= 0 && !isalpha(buffer[end])) {
        end--;
    }

    // Copy characters from start to end, including only alphabetic characters or separating words by spaces instead of dashes
    for (int i = start; i <= end; i++) {
        if (isalpha(buffer[i])) {
            final[indexF++] = buffer[i]; // Copy alphabetic characters directly
        } else if (buffer[i] == '-' && i > start && i < end && isalpha(buffer[i - 1]) && isalpha(buffer[i + 1])) {
            // Replace '-' with a space if it's between two alphabetic characters
            final[indexF++] = ' ';
        }
    }

    // Null-terminate the final string
    final[indexF] = '\0';
    //int finalLength = strlen(final); // after getting the word call other functio



   // printf("HERE: %s, Length: %d\n", final, finalLength);
    // find in the array DW about caps
  //  trimWhitespace(final);
  
    int a=search(final); 

    if(a ==0){//printf("Line: %d, Column: %d, Length: %d, Word: %s\n", line, column, len, buffer);
        printf("%s (%d,%d): %s \n",filePAth, line, column, final);
        errorFound=1;
    }

}



void processLine(char* line, int lineNumber, const char* filePATH) {
    const char delimiters[] = " \t\n-"; // Include '-' as a delimiter
    int column = 1; // Initialize column count
    char *cursor = line; // Cursor to track the current position in the line

    while (*cursor) {
        if (strchr(delimiters, *cursor)) {
            if (*cursor == '-' && isalpha(*(cursor-1)) && isalpha(*(cursor+1))) {
            } else {
                column++; 
            }
        } else {
            char buffer[200]; // Buffer to hold the word
            int bufferIndex = 0;
            while (*cursor && !strchr(delimiters, *cursor)) {
                buffer[bufferIndex++] = *cursor++; // Copy characters 
            }
            buffer[bufferIndex] = '\0'; // Null-terminate 
            if (bufferIndex > 0) { // Check if the buffer contains a word
                callitBuf(buffer, lineNumber, column, filePATH);
            }
            column += bufferIndex; 
            continue; 
        }
        cursor++; // Move to the next 
    }
}
void checkSpellingInFile(const char* filePath) {
    int fd = open(filePath, O_RDONLY); // Use open here
    if (fd < 0) { // Check for error
        fprintf(stderr, "Error opening file: %s\n", filePath);
        errorFound=1;
        return;
    }

    char buffer[4096]; // Buffer to store chunks of file data
    char lineBuffer[4096]; // Buffer to assemble a complete line
    ssize_t bytesRead;
    int lineBufferIndex = 0;
    int lineNumber = 1;

    while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        for (int i = 0; i < bytesRead; ++i) {
            if (buffer[i] == '\n' || buffer[i] == '\0') { // End of line or file
                lineBuffer[lineBufferIndex] = '\0'; // Null-terminate the line
                if (lineBufferIndex > 0) { // Ignore empty lines
                    processLine(lineBuffer, lineNumber, filePath);
                }
                lineNumber++;
                lineBufferIndex = 0; // Reset for the next line
            } else if (lineBufferIndex < sizeof(lineBuffer) - 1) {
                lineBuffer[lineBufferIndex++] = buffer[i]; // Add char to line
            }
        }
    }

    // Process any remaining content if the file doesn't end with a newline
    if (lineBufferIndex > 0) {
        lineBuffer[lineBufferIndex] = '\0';
        processLine(lineBuffer, lineNumber, filePath);
    }

    if (bytesRead < 0) {
        fprintf(stderr, "Error reading file: %s\n", filePath);
        errorFound=1;
    }

    close(fd); // Close the file descriptor
}
        


// go thru each file!
void checkSpellingInDirectory(const char *basePath) {
    char path[10000]; //path

    struct dirent *dp; //pointer dp -> type dirent. 
    DIR *dir = opendir(basePath); // opens the base location

    if (!dir) { // not found
        printf("\n Cannot open directory '%s'\n", basePath);
        errorFound=1;
        return; // fail
    }
    //printf("\nOpened directory BASE: '%s'\n\n", basePath); // the first thing cs211test itself


    while ((dp = readdir(dir)) != NULL) { // reads directory untill END 


        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) { // not current directory or parent
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name); // new path 
           // printf("Processing RN: '%s'\n", path); // process it


            struct stat statbuf;    // info stat
            if (stat(path, &statbuf) != 0) {    // non-zero, it indicates an error.
                printf("Cannot access the stats of '%s'\n", path);
                errorFound=1;
                continue;
            }

            // Check if it's a directory itself
            if (S_ISDIR(statbuf.st_mode)) { // if yes
            //    printf("'%s' is a directory\n", path); // PRINT

                //if it's a directory and doesn't start with '.'
                if (dp->d_name[0] != '.') { // IF an curret directory OR parent
                  //  printf("Entering directory '%s'\n", path);
                    checkSpellingInDirectory(path);

                } else {
                 //   printf("Directory '%s' is ignored because it starts with '.'\n", path);
                }


            } else if (S_ISREG(statbuf.st_mode)) {
                // Check if it's a .txt file and doesn't start with '.'
                if (strstr(dp->d_name, ".txt") && dp->d_name[0] != '.') {
                //    printf("Found .txt file '%s', checking spelling...\n", path);   // RUN HERE ONLY
                    checkSpellingInFile(path);
                    // This is where you would check the spelling
                    // For demonstration, just print the file's name

                } else {// if not opening file "." or / any


                 //   printf("File '%s' is ignored because it does not end with '.txt' or starts with '.'\n", path);
                }
            }
        }
    }
  //  printf("Closing directory '%s'\n", basePath);
    closedir(dir);
}


int fileExists(const char* path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

// Modified main function
int main(int argc, char* argv[]) {
     // load it now
    //if(size<1){printf("Nothing Found in Dictionary");}

    if (argc < 3) { // fail if less
        fprintf(stderr, "Usage: %s <directory_or_file_path>\n", argv[0]);
        errorFound=1;
        return EXIT_FAILURE;
    }
    loadDictionary(argc[1]);

    char filePath[1024];
    struct stat path_stat;

    for (int i = 2; i < argc; i++) {
        stat(argv[i], &path_stat);

        if (S_ISREG(path_stat.st_mode)) {
            checkSpellingInFile(argv[i]);
        } else if (!fileExists(argv[i]) && !S_ISDIR(path_stat.st_mode)) {
            snprintf(filePath, sizeof(filePath), "%s.txt", argv[i]);
            if (fileExists(filePath)) {
                checkSpellingInFile(filePath);
            } else {
                fprintf(stderr, "Error: '%s' is not a valid file or directory\n", argv[i]);
                errorFound=1;
            }
        } else if (S_ISDIR(path_stat.st_mode)) {
 //           printf("Starting the spelling check in directory: %s\n", argv[i]);
            checkSpellingInDirectory(argv[i]);
 //           printf("Completed spelling check for directory: %s\n", argv[i]);
        } else {
            fprintf(stderr, "Error: '%s' is not a valid file or directory\n", argv[i]);
            errorFound=1;
        }
    }
    if (errorFound) {
        return EXIT_FAILURE; // Exit with failure if any errors were found
    } else {
        return EXIT_SUCCESS; // Exit successfully if no errors were found
    }
  //  printf("Completed all spelling checks.\n");
    return 0;
}

