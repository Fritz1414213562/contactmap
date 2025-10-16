#ifndef CONTACT_MAP_WRITE_BINARY_HPP
#define CONTACT_MAP_WRITE_BINARY_HPP
#include <fstream>

template<typename T>
void write_binary_as(T& value, std::ofstream& fout)
{
	fout.write(reinterpret_cast<char*>(&value), sizeof(T));
}

#endif // CONTACT_MAP_WRITE_BINARY_HPP
