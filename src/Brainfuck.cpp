/*
 * BrainGear - A Brainfuck compiler that generates C code, implemented in C++.
 * Copyright (C) 2025 Andrey Stekolnikov <honakac@yandex.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "include/Brainfuck.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <queue>

Brainfuck::Brainfuck(const std::string& code)
{
	this->exec = code;
}

static inline void writeFile(std::ofstream &file, std::string line)
{
	file << line << std::endl;
}

static inline size_t calculate_repeat(const char c, std::string::iterator &iter, std::string::iterator end)
{
	size_t repeat = 0;
	for (; *iter == c && iter != end; iter++)
		repeat++;
	iter--;
	return repeat;
}

void Brainfuck::compile(const std::string& output)
{
	std::ofstream file(output);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening output file");
	}
	
	writeFile(file, "#include <stdio.h>");
	writeFile(file, "#include <math.h>");
	writeFile(file, "int main() {");
	writeFile(file, "char mem[65536];");
	writeFile(file, "unsigned short ptr = 0;");

	for (auto iter = this->exec.begin(); iter != this->exec.end(); ++iter) {
		switch (*iter) {
            case '>': writeFile(file, "ptr += " + std::to_string(calculate_repeat('>', iter, this->exec.end())) + ";");  break;
            case '<': writeFile(file, "ptr -= " + std::to_string(calculate_repeat('<', iter, this->exec.end())) + ";");  break;
            case '+': writeFile(file, "mem[ptr] += " + std::to_string(calculate_repeat('+', iter, this->exec.end())) + ";");  break;
            case '-': writeFile(file, "mem[ptr] -= " + std::to_string(calculate_repeat('-', iter, this->exec.end())) + ";");  break;
            case ',': writeFile(file, "mem[ptr] = getchar();"); break;
            case '.': writeFile(file, "putchar(mem[ptr]);"); break;
            case '[': writeFile(file, "while (mem[ptr] != 0) {"); break;
            case ']': writeFile(file, "}"); break;
            default: break;
        }
	}
	writeFile(file, "}");
}