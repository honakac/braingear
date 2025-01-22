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

#include <iostream>
#include <fstream>
#include "include/Brainfuck.h"

std::string readFile(char *filepath)
{
	std::ifstream file(filepath);
	if (!file.is_open()) {
		fprintf(stderr, "Error opening file: %s\n", filepath);
		std::exit(1);
	}

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
	}

	Brainfuck bf(readFile(argv[1]));
	bf.compile(std::string(argv[1]) + ".c");
	return 0;
}
