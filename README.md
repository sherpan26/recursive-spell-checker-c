# Recursive Spell Checker in C

A spell checker written in C that scans a single text file or recursively traverses a directory,
reporting misspellings using a dictionary file.

## Features
- **Recursive directory traversal** (walks subdirectories)
- **Dictionary-backed lookup** (binary search for fast checks)
- **Punctuation-aware tokenization** (handles leading/trailing punctuation)
- **Capitalization rules** (accepts `hello`, `Hello`, `HELLO`; rejects mixed-case like `HeLlO`)
- **Positioned error output** (reports line/column + the misspelled token)


## Run
```bash
./spchk <path-to-file-or-directory> english.dict
```

## Examples:
```bash
./spchk test1 english.dict
./spchk ./some_folder english.dict
```
## Output
```bash
Each misspelling is reported with its line, column, and word (format depends on implementation).
```

## Project Files
```bash
spchk.c — main program + recursive traversal

dictionary.c/.h — dictionary loading + lookup + case rules

spchk.h — spell-check function declarations

spchk2.c — alternate/extended implementation (optional)

english.dict — dictionary (one word per line)

test1 — sample input

Makefile — build automation
```
