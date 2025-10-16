#ifndef CONTACTMAP_CALCULATION_DCD_PARSER_HPP
#define CONTACTMAP_CALCULATION_DCD_PARSER_HPP

#include "coordinate.hpp"
#include "utility.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>
#include <array>

class DCDParser {

	public:
		DCDParser()  = default;
		~DCDParser() = default;

		std::vector<Coordinate> read(const std::string& filename);
		Coordinate              read(const std::string& filename, int frame);
		bool            has_unitcell() const { return has_unitcell_; }

	private:
		std::string read_block(std::ifstream& ifs);
		Coordinate read_xyz(std::ifstream& ifs, const int atom_num);
		void skip_xyz(std::ifstream& ifs);

		int read_frame_num(const std::string& first_block);
		int read_atom_num(const std::string& third_block);
		bool read_unitcell_flag(const std::string& first_block);
		std::vector<float> read_coordinates(const std::string& block, const int atom_num) const;
		std::array<float, 3> read_pbc_box(const std::string& block) const;

	private:
		const int frame_num_index_ = 4;
		const int unit_cell_index_ = 44;
		const int atom_num_index_  = 0;
		bool has_unitcell_ = false;

};

std::vector<Coordinate> DCDParser::read(const std::string& filename)
{
	std::ifstream ifs(filename);
	if (!ifs.is_open())
		throw std::runtime_error("[error] File or directory not found: " + filename);

	// 1st block
	const std::string& first_block = read_block(ifs);
	const int frame_num = read_frame_num(first_block);
	has_unitcell_ = read_unitcell_flag(first_block);
	// 2nd block (not used)
	read_block(ifs);
	// 3rd block
	const std::string& third_block = read_block(ifs);
	const int atom_num  = read_atom_num(third_block);

	std::vector<Coordinate> traj;

	for (std::size_t iframe = 0; iframe < frame_num; ++iframe)
	{
		traj.push_back(read_xyz(ifs, atom_num));
	}
	ifs.close();

	return traj;
}

Coordinate DCDParser::read(const std::string& filename, int frame)
{
	std::ifstream ifs(filename);
	if (!ifs.is_open())
		throw std::runtime_error("[error] File or directory not found: " + filename);
	// 1st block
	const std::string& first_block = read_block(ifs);
	const int frame_num = read_frame_num(first_block);
	has_unitcell_ = read_unitcell_flag(first_block);
	if (frame >= frame_num)
		throw std::runtime_error(
			"[error] Frame number " + std::to_string(frame) + " is out of range");
	// 2nd block (not used)
	read_block(ifs);
	// 3rd block
	const std::string& third_block = read_block(ifs);
	const int atom_num  = read_atom_num(third_block);

	if (frame < 0) frame = frame_num;

	for (std::size_t iframe = 0; iframe < frame - 1; ++iframe) skip_xyz(ifs);

	Coordinate coord = read_xyz(ifs, atom_num);
	ifs.close();
	return coord;
}

std::string DCDParser::read_block(std::ifstream& ifs)
{
	std::int32_t block_size;
	std::vector<char> buffer;

	constexpr int int32_size = sizeof(int32_t);
	ifs.read(reinterpret_cast<char*>(&block_size), int32_size);
	buffer.resize(block_size);
	ifs.read(buffer.data(), block_size);

	std::int32_t check_block_size;
	ifs.read(reinterpret_cast<char*>(&check_block_size), int32_size);

	if (block_size != check_block_size)
		throw std::runtime_error(
			"[error] Inconsistent block size:"
				+ std::to_string(block_size)
				+ " != " + std::to_string(check_block_size));
	return std::string(buffer.begin(), buffer.end());
}

void DCDParser::skip_xyz(std::ifstream& ifs)
{
	if (has_unitcell_) read_block(ifs);
	for (std::size_t idim = 0; idim < 3; ++idim) read_block(ifs);
}

Coordinate DCDParser::read_xyz(std::ifstream& ifs, const int atom_num)
{
	std::array<float, 3> pbc_box = {0.0, 0.0, 0.0};
	if (has_unitcell_) pbc_box = read_pbc_box(read_block(ifs));
	const std::string& x_block = read_block(ifs);
	const std::string& y_block = read_block(ifs);
	const std::string& z_block = read_block(ifs);
	std::array<std::vector<float>, 3> xyz = {
		read_coordinates(x_block, atom_num),
		read_coordinates(y_block, atom_num),
		read_coordinates(z_block, atom_num)};
	if (has_unitcell_) return Coordinate(xyz, pbc_box);
	else return Coordinate(xyz);
}

int DCDParser::read_frame_num(const std::string& first_block)
{
	return Utility::read_binary_as<int>(&first_block.at(frame_num_index_));
}

int DCDParser::read_atom_num(const std::string& third_block)
{
	return Utility::read_binary_as<int>(&third_block.at(atom_num_index_));
}

std::vector<float> DCDParser::read_coordinates(const std::string& block, const int atom_num) const
{
	std::vector<float> coordinates(atom_num);
	constexpr int float_size = sizeof(float);
	for (std::size_t iatom = 0; iatom < atom_num; ++iatom)
	{
		std::size_t pos_in_block = iatom * float_size;
		coordinates[iatom] = Utility::read_binary_as<float>(&block.at(pos_in_block));
	}
	return coordinates;
}

bool DCDParser::read_unitcell_flag(const std::string& first_block)
{
	return static_cast<bool>(Utility::read_binary_as<int>(&first_block.at(unit_cell_index_)));
}

std::array<float, 3> DCDParser::read_pbc_box(const std::string& block) const
{
	constexpr int double_size = sizeof(double);
	const float x = static_cast<float>(Utility::read_binary_as<double>(&block.at(0 * double_size)));
	const float y = static_cast<float>(Utility::read_binary_as<double>(&block.at(2 * double_size)));
	const float z = static_cast<float>(Utility::read_binary_as<double>(&block.at(5 * double_size)));
	return std::array<float, 3>{x, y, z};
}

#endif // CONTACTMAP_CALCULATION_DCD_PARSER_HPP
