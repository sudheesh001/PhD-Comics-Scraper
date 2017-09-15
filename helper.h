#include <string>
#include <sstream>
#include <vector>
#include <iterator>

// This is a common helper function that helps split a given string by a seperator and sends back a vector<string> stream.
std::vector<std::string> split(const std::string &text, char sep) {
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos) {
	tokens.push_back(text.substr(start, end - start));
	start = end + 1;
	}
	tokens.push_back(text.substr(start));
	return tokens;
}

// This is a common function that removes duplicates from the vector<string> and returns vector<string>
template <typename T>
void remove_duplicates(std::vector<T>& vec) {
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}
