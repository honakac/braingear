<p align="center">
<img src="https://img.shields.io/github/contributors/honakac/braingear.svg">
<img src="https://img.shields.io/github/forks/honakac/braingear.svg">
<img src="https://img.shields.io/github/stars/honakac/braingear.svg">
<img src="https://img.shields.io/github/issues/honakac/braingear.svg">
<br/>

# braingear
braingear is a compiler for the Brainfuck programming language that generates C code from .bf files.

## Features
* Converts Brainfuck code into C code.
* Simple command-line interface.
* Generates C files with the same name as the input file, adding a `.c` postfix.
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
To generate C code from a Brainfuck file, use:

```bash
braingear <input file>
```

### For example:

Input file hello.bf:

```bf
++++++++++[>+++++++>++++++++++>+++<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.
```

Command:

```
braingear hello.bf
```

Compile and run:

```bash
gcc -O2 -march=native -pipe hello.bf.c -o hello
./hello
```

Output:

```
Hello World!
```

## License
This project is licensed under the GNU General Public License v3.0: https://www.gnu.org/licenses/gpl-3.0.html

## Contact
If you have any questions or suggestions, feel free to reach out via GitHub: https://github.com/honakac/braingear