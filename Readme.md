
# Brainfuck Goldbach

## About the Project

The goal of this project is to create a Brainfuck program that halts **only** if the Goldbach Conjecture is false. Additionally, the Brainfuck code should be as short and minimal as possible.

To achieve this, we first wrote a C program implementing the Goldbach algorithm. Step by step, we replaced all `if` statements, variables, and assignments (`=`) until only a single array with one index pointer (not a C pointer) and `while` loops remained. This reduced C code is then passed through a C preprocessor and converted to Brainfuck using a Python script.

### Why this approach?

- Easy to change variable layout
- Easy to test new ideas
- Having equivalent C code allows for debugging with a C debugger

---

## Files

### Created Files

- `.gitignore`
- `Makefile` – Compiles the C code and converts it to Brainfuck
- `Readme.md` – This documentation
- `convert.py` – Python 3 script to convert reduced C code to Brainfuck
- `goldbach.c` – C implementation, very close to Brainfuck
- `interpret.py` – Brainfuck interpreter
- `testif` – Notes on different ways to implement `if` logic in Brainfuck

### Generated Files

- `debug_out_line.txt` – Brainfuck code with the corresponding C lines
- `debug_out_expression.txt` – Brainfuck code with corresponding C expressions
- `goldbach_simple.c` – Preprocessed version of `goldbach.c`, used by `convert.py`
- `jumpcount.txt` – Matrix showing how often variables are switched (helpful for optimization)
- `native` – Compiled C executable using `gcc`
- `out.bf` – Generated Brainfuck code

---

## Requirements

### Brainfuck Environment

- Unlimited cell size
- Around 20 or more memory cells
- Support for negative cell offsets (not used in current version)

The provided `interpret.py` interpreter satisfies these requirements.

### Tools Needed for Building

- `make`
- Python 3
- Bash
- `gcc`

---

## Code Generation

Simply run:

```sh
make
```

### Preprocessor Flags

These flags can be set to `1` to enable alternative behavior. By default, they should be set to `0`.

- `TESTHALT`: Instead of testing only even numbers, this flag enables testing of numbers in the form `4 + 3n` (i.e., 4, 7, 10, 13, ...).
  This is useful for verifying that the algorithm functions correctly. When this flag is enabled, the program should halt at 37.

- `PRINT`: Enables output of the currently tested number and the number of valid Goldbach pairings (i.e., how many ways it can be written as the sum of two primes).
  Note: The output is in raw format, not as ASCII. You need a Brainfuck interpreter that can handle this. The provided `interpret.py` script handles this correctly.

---

## License

**WTFPL** – Do What the Fuck You Want to Public License
Do whatever you want.

