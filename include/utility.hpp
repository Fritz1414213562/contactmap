#ifndef CONTACTMAP_CALCULATION_UTILITY_HPP
#define CONTACTMAP_CALCULATION_UTILITY_HPP
#include <random>
#include <cstring>
#include <array>
#include <vector>
#include <string>

namespace Utility
{
template<typename T>
T read_binary_as(const char *str)
{
	T result;
	std::memcpy(std::addressof(result), str, sizeof(T));
	return result;
}

template<typename T>
T inner_product(const std::array<T, 3>& lhs, const std::array<T, 3>& rhs)
{
	return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
}

template<typename T>
std::array<T, 3> cross_product(const std::array<T, 3>& lhs, const std::array<T, 3>& rhs)
{
	return std::array<T, 3>({
		lhs[1] * rhs[2] - lhs[2] * rhs[1],
		lhs[2] * rhs[0] - lhs[0] * rhs[2],
		lhs[0] * rhs[1] - lhs[1] * rhs[0],
	});
}

bool startswith(const std::string& s, const std::string& prefix)
{
	if (s.size() < prefix.size()) return false;
	return std::equal(std::begin(prefix), std::end(prefix), std::begin(s));
}

std::vector<std::string> split_string(const std::string& s, const char delimiter)
{
	std::vector<std::string> retval;
	std::string buffer;
	for (const char& chr : s)
	{
		if ((chr == delimiter) && (buffer.empty())) continue;
		else if ((chr == delimiter) && (!buffer.empty()))
		{
			retval.push_back(buffer);
			buffer.clear();
		}
		else buffer.push_back(chr);
	}
	if (!buffer.empty()) retval.push_back(buffer);
	return retval;
}

}


#endif // CONTACTMAP_CALCULATION_UTILITY_HPP
