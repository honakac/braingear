<p align="center">
<img src="https://img.shields.io/github/contributors/honakac/braingear.svg">
<img src="https://img.shields.io/github/forks/honakac/braingear.svg">
<img src="https://img.shields.io/github/stars/honakac/braingear.svg">
<img src="https://img.shields.io/github/issues/honakac/braingear.svg">
<br/>

# braingear
**braingear** is a Brainfuck interpreter and compiler written in C. <br> It's designed to be simple, portable, and easy to use.

## Features
* Converts Brainfuck code into C code.
* Interprets the Brainfuck file
* Simple command-line interface.
* Easy compilation and installation using make.
## Installation
To install, run the following commands:

```bash
git clone https://github.com/honakac/braingear.git
cd braingear
make
```

Ensure you have a C compiler (e.g., gcc) and the make utility installed.

## Usage
To run a Brainfuck file, use:

```bash
braingear run <input file>
```

To generate C code from a Brainfuck file, use:

```bash
braingear compile <input file> <output file>
```

### For example (Compile program 'Hello, world'):

> [!NOTE]
> Examples are provided in the 'examples' directory.

Input file hello.bf:

```bf
++++++++++[>+++++++>++++++++++>+++<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.
```

Command:

```
braingear compile hello.bf hello.c
```

Compile and run:

```bash
cc -O2 -march=native -pipe hello.c -o hello
./hello
```

Output:

```
Hello World!
```

## See also

[rustyfuck](https://github.com/OctoBanon-Main/rustyfuck) - A Brainfuck interpreter written in Rust by OctoBanon

## License
This project is licensed under the GNU General Public License v3.0: https://www.gnu.org/licenses/gpl-3.0.html

## Contact
If you have any questions or suggestions, feel free to reach out via GitHub: https://github.com/honakac/braingear
