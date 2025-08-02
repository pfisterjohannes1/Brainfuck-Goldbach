
# Brainfuck Goldbach

## About

The goal is to have a brainfuck program that only halts when the
 goldbach conjecture is wrong.
And to make the brainfuck code as short as possible.

We did this by creating a C program with a goldbach algorithm first, then replacing all ifs,
 variables, = ... till we only use a single array with a single index pointer (not a c pointer)
 and only while - loops
This C program will be converted run through a C-Preprocessor and then converted to brainfuck with
 a script.

Why using this approch:
 - Easier to change to a different variable layout
 - Easier to test something new
 - Having an equivalent C code you can debug with a C debugger.


## Files

Created Files:
  - .gitignore        -
  - Makefile          Makefile to start conversation to Brainfuck and compile C file
  - Readme.md         -
  - convert.py        Python 3 Script to convert reduced C code to Brainfuck
  - goldbach.c        C-Implementation, very close to Brainfuck
  - interpret.py      Brainfuck interpreter
  - testif            Notes on different ways how we could implement a if

Generated files:
  - debug\_out\_line.txt        Brainfuck code next to the corresponding c expression
  - debug\_out\_expression.txt  Brainfuck code next to the corresponding c Line
  - goldbach\_simple.c          striped goldbach.c run through a preprocessor, used in convert.py
  - jumpcount.txt               List/Matrix how often we change from one variable to a other. For optimation
  - native                      Compiled C code with gcc
  - out.bf                      Generated Brainfuck code

## Requirements


The Brainfuck environment needs:
- Unlimited cellsize
- About 20 or cells
- Access to negative cell offsets (Current version does not use it)
The script interpret.py should provide that.

For converting and compiling:
- make
- Python 3 interpreter
- Bash
- gcc


## Generation

Just use the make command

Preprocessor flags:
This flags should be 0 for the normal code, set them to 1 for the following alternative
 behaviour.
TESTHALT: Don't test only/all even numbers but all numbers of 4+3\*n, (4,7,10,13...). The program
 ahould halt at 37. To test if algorithm really works.
PRINT: Print what number we test and how many ways we found to represent it as a sum of 2 primes.
 Please note that the number outputed is raw and the Brainfuck interpreter should convert it to
 ASCII. interpret.py does that.


## Licence

WTFPL: Do what you want.


