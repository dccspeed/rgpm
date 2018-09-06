#ifndef STRMISC_H
#define STRMISC_H

#include <string>
#include <vector>
#include <sstream>

const char _WHITESPACE[] = " \t\n\x0b\x0c\r\0";
const std::string WHITESPACE(_WHITESPACE,sizeof(_WHITESPACE));
const char SEPARATOR[]=",";

std::string& toLower(std::string& s);
std::string& toUpper(std::string& s);
std::string strip(const std::string s);
bool startsWith(const std::string&, const std::string&);
bool endsWith(const std::string&, const std::string&);
std::string unichr(unsigned int uv);
std::vector<std::string>
split (const std::string &inString, const std::string &separator,
		int maxsplit = 0);

#endif // __STRMISC_H
