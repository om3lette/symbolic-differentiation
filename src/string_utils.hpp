#include <algorithm>
#include <string>

// Returns a copy of a given string in lowercase
static std::string to_lower(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}
