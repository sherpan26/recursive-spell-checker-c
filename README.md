# recursive-spell-checker-c
# Recursive Spell Checker in C

This project implements a robust spell checker in C that scans individual text files or recursively traverses directories to check spelling using a custom dictionary file.

## 📁 Project Structure

- `spchk.c`: Main spell-checker program with recursive directory traversal.
- `dictionary.c` / `dictionary.h`: Loads and stores dictionary words; handles case variation rules.
- `spchk.h`: Header for spell-checking functions.
- `spchk2.c`: Alternate or extended implementation with advanced case handling.
- `english.dict`: Plain text dictionary file (one word per line).
- `test1`: Sample test input file.
- `Makefile`: Automates compilation of the project.

## ✨ Features

### ✅ Punctuation Handling
- Ignores trailing and leading punctuation (e.g., `"apple."` → `"apple"`).
- Accepts valid hyphenated words (e.g., `mother-in-law`, `well-being`).

### 🔠 Capitalization Rules
- Accepts:
  - All lowercase: `hello`
  - First-letter uppercase: `Hello`
  - All uppercase: `HELLO`
- Rejects:
  - Mixed-case like `HeLlO`
  - Incorrectly capitalized words (e.g., `macdonald` if dictionary has `MacDonald`)

### 🔍 Spell-Checking Logic
- Binary search for fast word lookup.
- Tokenization logic to isolate words line-by-line and track:
  - **Line number**
  - **Column number**
  - **Exact word text**

### 📂 Recursive Directory Traversal
- Recursively descends into subdirectories.
- Skips hidden files and directories.
- Only checks `.txt` files.

### 📢 Error Reporting
Outputs misspelled words in the format:
