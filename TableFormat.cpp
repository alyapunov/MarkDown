/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2021, Aleksandr Lyapunov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

size_t
str_utf8_length(std::string_view str)
{
	size_t res = 0;
	for (char c : str)
		res += (c & 0xc0) != 0x80;
	return res;
}

void
str_trim(std::string& str, const std::string& whitespace = " \t")
{
	size_t first = str.find_first_not_of(whitespace);
	if (first == str.npos)
		return str.clear();
	size_t last = str.find_last_not_of(whitespace) + 1;
	std::copy(str.begin() + first, str.begin() + last, str.begin());
	str.resize(last - first);
}

void
str_trim(std::string_view& str, std::string_view whitespace = " \t")
{
	size_t first = str.find_first_not_of(whitespace);
	if (first == str.npos) {
		str = {};
		return;
	}
	size_t last = str.find_last_not_of(whitespace) + 1;
	str = str.substr(first, last - first);
}

void
str_extend(std::string& str, size_t size)
{
	str.reserve(size);
	for (size_t i = str_utf8_length(str); i < size; i++)
		str.push_back(' ');
}

void
normalize(std::vector<std::vector<std::string>>& arr)
{
	if (arr.empty())
		return;
	for (size_t i = 0; i < arr[0].size(); i++) {
		size_t max_len = 2;
		for (auto& row : arr) {
			if (i >= row.size())
				continue;
			str_trim(row[i]);
			max_len = std::max(max_len, str_utf8_length(row[i]));
		}
		for (auto& row : arr) {
			if (i >= row.size())
				continue;
			str_extend(row[i], max_len);
		}
	}
}

void
transpose(std::vector<std::vector<std::string>>& arr)
{
	size_t a = arr.size();
	size_t b = 0;
	for (const auto& row : arr)
		b = std::max(b, row.size());
	size_t c = std::max(a, b);
	arr.resize(c);
	for (auto& row : arr)
		row.resize(c);
	for (size_t i = 0; i < c; i++) {
		for (size_t j = i + 1; j < c; j++)
			std::swap(arr[i][j], arr[j][i]);
	}
	for (auto& row : arr)
		row.resize(a);
	arr.resize(b);
}

std::vector<std::vector<std::string>>
parse(std::string_view str)
{
	std::vector<std::vector<std::string>> res;
	size_t line_no = 0;
	while (!str.empty()) {
		size_t pos = str.find_first_of('\n');
		std::string_view line = pos == str.npos ? str : str.substr(0, pos);
		str = str.substr(pos == str.npos ? str.size() : pos + 1);
		str_trim(line);
		if (line.empty())
			continue;
		if (line.front() != '|')
			return {};
		line = line.substr(1);
		if (++line_no != 2)
			res.emplace_back();
		while (!line.empty()) {
			pos = 0;
			bool escaped = false;
			while (true) {
				if (pos == line.size())
					return {};
				if (line_no == 2 && line[pos] != '|' &&
				    line[pos] != '-' && line[pos] != ' ' &&
				    line[pos] != '\t')
					return {};
				if (escaped)
					escaped = false;
				else if (line[pos] == '\\')
					escaped = true;
				else if (line[pos] == '|')
					break;
				++pos;
			}
			if (line_no != 2)
				res.back().emplace_back(line.substr(0, pos));
			line = line.substr(pos + 1);
		}
	}
	return res;
}

std::string
generate(const std::vector<std::vector<std::string>>& arr)
{
	if (arr.empty())
		return {};
	std::string res;

	for (size_t i = 0; i < arr.size(); i++) {
		res += "|";
		for (size_t j = 0; j < arr[i].size(); j++) {
			res += " ";
			res += arr[i][j];
			res += " |";
		}
		res += "\n";
		if (i == 0) {
			res += "|";
			for (size_t j = 0; j < arr[i].size(); j++) {
				res += " ";
				size_t len = str_utf8_length(arr[i][j]);
				for (size_t k = 0; k <  len; k++)
					res += "-";
				res += " |";
			}
			res += "\n";
		}
	}
	return res;
}

int main()
{
	std::string str,line;
	while (std::getline(std::cin, line))
	{
		str += line;
		if (!std::cin.eof())
			str += '\n';
	}

	auto table = parse(str);
	if (table.empty()) {
		std::cout << str;
		return -1;
	}

#ifdef MD_TRANSPOSE
	transpose(table);
#endif
	normalize(table);
	std::cout << generate(table);
	return 0;
}
