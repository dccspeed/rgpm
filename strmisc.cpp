#include "strmisc.h"

#include <algorithm>
#include <ctype.h>
#include <iostream>

std::string& toLower(std::string& s)
{
	std::transform(s.begin(), s.end(), s.begin(), (int(*)(int))tolower);
	return s;
}

std::string& toUpper(std::string& s)
{
	std::transform(s.begin(), s.end(), s.begin(), (int(*)(int))toupper);
	return s;
}


bool startsWith(const std::string& data, const std::string& start )
{
	if (start.size() > data.size()) { return false; }

	return std::equal(start.begin(), start.end(),data.begin());
	
}

bool endsWith(const std::string& u, const std::string& end )
{
	if (end.size() > u.size()) { return false; }

	return std::equal(end.rbegin(), end.rend(),u.rbegin());
}


std::string strip(const std::string _s)
{
	std::string::size_type start;
	std::string::size_type end;

	std::string s = _s; 

	start = s.find_first_not_of(WHITESPACE);
	if (start == std::string::npos) {
		// String made of spaces...
		s.clear();
		return s;
	}
	s = s.substr(start);

	// take space out of the end of the string
	end = s.find_last_not_of(WHITESPACE);
	if (end == std::string::npos) {
		s.clear();
		return s;
	}
	s = s.substr(0,end +1);
	return s;
}

std::string unichr(unsigned int uv)
{
	const unsigned int UTF_MAX  = 6;	/* They can be bigger, but we just don't care
			    	 *  about those bastards
			    	 */

	char output[UTF_MAX +1] = {0,0,0,0,0,0,0}; /* Strings will
						    * be null-termiinated
						    * no matter what
						    */
	char* d = output;

	if (uv < 0x80) {
		*d++ = uv;
		return std::string(output);
	}
	if (uv < 0x800) {
		*d++ = (( uv >>  6)         | 0xc0);
		*d++ = (( uv        & 0x3f) | 0x80);
		return std::string(output);
	}
	if (uv < 0x10000) {
		*d++ = (( uv >> 12)         | 0xe0);
		*d++ = (((uv >>  6) & 0x3f) | 0x80);
		*d++ = (( uv        & 0x3f) | 0x80);
		return std::string(output);
	}
	if (uv < 0x200000) {
		*d++ = (( uv >> 18)         | 0xf0);
		*d++ = (((uv >> 12) & 0x3f) | 0x80);
		*d++ = (((uv >>  6) & 0x3f) | 0x80);
		*d++ = (( uv        & 0x3f) | 0x80);
		return std::string(output);
	}
	if (uv < 0x4000000) {
		*d++ = (( uv >> 24)         | 0xf8);
		*d++ = (((uv >> 18) & 0x3f) | 0x80);
		*d++ = (((uv >> 12) & 0x3f) | 0x80);
		*d++ = (((uv >>  6) & 0x3f) | 0x80);
		*d++ = (( uv        & 0x3f) | 0x80);
		return std::string(output);
	}
	if (uv < 0x80000000) {
		*d++ = (( uv >> 30)         | 0xfc);
		*d++ = (((uv >> 24) & 0x3f) | 0x80);
		*d++ = (((uv >> 18) & 0x3f) | 0x80);
		*d++ = (((uv >> 12) & 0x3f) | 0x80);
		*d++ = (((uv >>  6) & 0x3f) | 0x80);
		*d++ = (( uv        & 0x3f) | 0x80);
		return std::string(output);
	}

	// Corner case - returning codepoint 0x7fffffff representation
	// in UTF-8
        return std::string("\xfd\xbf\xbf\xbf\xbf\xbf");

}

std::vector<std::string>
split (const std::string &inString, const std::string &separator,
		int maxsplit)
{
	std::vector<std::string> returnVector;
    if (inString.empty()) { 
        //std::cout << "string to split is empty!\n";
    	return returnVector;
    }
	
    
    std::string::size_type start = 0;
	std::string::size_type end = 0;
	int count = maxsplit;

	while ( ((end=inString.find (separator, start)) != std::string::npos) &&
			(maxsplit ? count-- : true) )
	{
		returnVector.push_back (inString.substr (start, end-start));
		start = end+separator.size();
	}

	returnVector.push_back (inString.substr (start));

	return returnVector;

} 



