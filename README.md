Compiling & Running
===================
Simply run `make` to compile and then run `shell`. 

Important Notes
===============
All spaces in commands, variable assignments and profile file must be escaped
by backslash `\` or put inside quotes. 

e.g. `cd /Volumes/Macintosh\ HD` or `cd "/Volumes/Macintosh HD"`

Because backslash is an escape character, user must type `\\` to enter a 
single backslash. 

Files
=====
- HashTable.h and HashTable.c
    Defines and implements a hash table that maps string values to string values
    (used to store variables in Shell)
- Helpers.h and Helpers.c
    Contains helper functions for modifying and parsing strings
- Shell.h and Shell.c
    Defines a `Shell` struct that encapsulates an emulated shell and
    implements all shell emulation functions, most importantly `Shell_PromptUser()`
    which contains the main loop of the program that repeatedly prompts user for commands
- main.c
    small entry function that initializes Shell and hands off to `Shell_PromptUser()`