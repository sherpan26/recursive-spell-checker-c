SPELL CHECKER (spchk.c)
__________________________________________________
We implemented a spell checker program, named spchk.c designed to identify and 
report misspelled words in text files. 


FEATURES
1. Trailing Punctuation: It ignores punctuation at the end of words to focus on the actual spelling.
Our program is designed to process words while ignoring those trailing punctuation marks. This includes periods, commas, semicolons, question marks,numbers, exclamation marks, and ellipses at the end of a word are disregarded. 
Similarly, leading single and double quotes, as well as opening brackets (parentheses, square brackets, and curly braces), are also omitted. This makes sure that the actual word is take out not including any surrounding punctuation.
So for the following examples below displays how the code should work:
apple.     -> apple (ignores the . at the end of the word )
"apple     -> apple (ignores the " at the front of the word)
Rutgers?   -> Rutgers  (ignores the ? at end of the word)
NSA...     -> NSA  (ignores the trailing ... at the end of the word)
aBc;       -> aBc  (ignores ; at the end of the word)

In these cases, the program will identify and process the words apple, Rutgers, NSA, and aBc correctly, regardless of the trailing punctuation

2. Hyphenated Words: Our Program correctly identifies hyphenated words if all parts are spelled correctly.

Hyphenated words are two or more smaller words separated by hyphens (-). Thus, we need both the component words to be spelled correctly in order it to be a hyphenated word.

Our program treats hyphenated words with special case. A hyphenated word is considered to be any term that includes one or more hyphens, connecting two or more component words. For example, "well-being" is a hyphenated word consisting of "well" and "being."

To be seen as correctly spelled in our program each component word within a hyphenated word must be correct according to the dictionary. 
So our idea for the program was to split the hyphenated words at the hyphens and verify each word separately if it is in the dictionary. 

Example Test Cases:
well-being   -> both 'well' and 'being' must be correct 
mother-in-law   -> 'mother', 'in', and 'law' must all be correct
long-term   -> 'long' and 'term' must be correct

In all of these cases, when the hyphens are read in the program, it treats the word following after it separately from the word before. Thus, it then checks if each individual word is in the dictionary and if all words are in the dictionary then it is a hyphanted word. 

3. Capitalization: Our program includes three variations of a word: if a word is all lowercase, initial is capital, and word is all uppercase.

3 variations: 
_______________

Regular case (all lowercase)
Initial capital (only the first letter is uppercase)
All capitals (all uppercase)
If the dictionary has the word "hello," it recognizes "hello," "Hello," and "HELLO" as correct. 
However, mixed case variations such as "HeLlO" would be marked as incorrect.

Two capital letters or more: 
__________________________________________________

We need to note for this program that for words that naturally contain capital letters, such as "MacDonald," 
then our spell checker requires an exact match for the capitalization pattern which is very important for these types of words.
So, if the dictionary lists "MacDonald," only "MacDonald" and "MACDONALD" are correct, and "Macdonald" or "macdonald" are not correct and variations of these.
__________________________________________________


FEATURES
__________________________________________________
DATA STRUCTURES IMPLEMENTED 

1. Binary Search: To efficiently find words in any dictionary, we used binary search algorithm. This makes the program efficient when dealing with small and larger dictionaries, so since the size of any dictionary is not specified 
we decided it was best to use binary search since it was so good at finding words in a large dictionary. The Binary Search operates on sorted array and divides search in half over and over untill it finds the word. The binary_search function we have takes a word as input and returns 1 if the word is in the dictionary otherwise it returns a 0. 
- It was crucial for the dictionary to be loaded into an array to make the program simplier when using binary search. 

Process_file
__________________________________________________
File Processing: The process_file function we have shows how the files are read and processed character by character. This method allows us to identify words based on separators (trailing punctuation, hyphens, and etc) and to accurately report line and column tracking for words that might be spelled incorrectly. 


Word Identification: As characters are read by process_file, they are appended to a buffer until a non-word character is encountered. This supports the identifying of words, including those with hyphens, and makes sure that words at the end of lines are correctly handled. 


Error Reporting: When identifying a potentially misspelled word (a word not found in the dictionary), the module reports the filename, line number, column number, and the word itself. This feedback is important for allowing us to locate and review if program runs as it is supposed to. This was so important is checking what part of the code was working
and what parts were not through using different dictionaries and test files and getting these error reports. 


Recursive Directory traversal
__________________________________________________
-Directory Opening: The process_directory function is how we handled directory traversal as requested in write-up directions. When successfully opening a directory, the function recursively traverses over each entry within the directory and checks the spelling of all files whose names end with “.txt”, but ignoring any files or directories whose names begin with “.”.

When our tool finds an item in a folder, it uses  stat, a function, to get information about that item. Stat tells us whether it is a file, a folder, or something else. Once stat gives us this information, it stores it in st.st_mode

The st.st_mode has codes that tell us exactly what the item is. Our tool looks at these codes to find out if the item is a directory. If the codes match the pattern for a directory, our tool knows it's found a folder.

If the item is  a folder, our tool calls itself again, but different this time instead for the contents of this newly found folder. We imagined this scenario like being pirates, "Okay, we've got treasure box; let's open this one too and check everything inside." It keeps doing this, going deeper into each folder it finds, checking all the way down.
