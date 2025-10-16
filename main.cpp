#include "dcd_parser.hpp"
#include "coordinate.hpp"
#include <eigen3/Eigen/Core>
#include <parse_indices.hpp>
#include <contactmap_calculation.hpp>
#include <write_binary.hpp>
#include <boost/timer/progress_display.hpp>
#include <input.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>

int main(int argc, char *argv[])
{

	Input input(argc, argv);
	const std::vector<int>& group1 = parse_indices_in(input.group1_name());
	const std::vector<int>& group2 = parse_indices_in(input.group2_name());
	const std::vector<std::string>& fnames = input.input_name();
	const float& cutoff = input.cutoff();
	const int& mask_size = input.mask_size();
	const int& skip_size = input.skip_size();
	const int& istart    = input.start_index();
	const int& iend      = input.end_index();
	DCDParser traj_reader = DCDParser();
	Eigen::MatrixXf contactmap = Eigen::MatrixXf::Zero(group1.size(), group2.size());

	float total_frame = 0.0;
	for (const auto& fname : fnames)
	{
		const std::vector<Coordinate>& traj = traj_reader.read(fname);
		const bool is_periodic = traj_reader.has_unitcell();
		const int& iend_tmp = (iend < 0) ? traj.size() + iend + 1 : iend;
		boost::timer::progress_display progress(iend_tmp - istart);

		for (int idx = istart; idx < iend_tmp; idx += skip_size)
		{
			const auto& coordinate = traj[idx];
			ContactMapCalculator::run(coordinate, group1, group2, cutoff, mask_size, is_periodic, contactmap);
			++total_frame;
			++progress;
		}
	}
	contactmap /= total_frame;

	std::ofstream ofs(input.output_name());
	int nrow = group1.size();
	int ncol = group2.size();
	int block_size = sizeof(int) * 2;
	// matrix shape
	write_binary_as<int>(block_size, ofs);
	write_binary_as<int>(nrow, ofs);
	write_binary_as<int>(ncol, ofs);
	write_binary_as<int>(block_size, ofs);
	// matrix body
	block_size = sizeof(float) * nrow * ncol;
	write_binary_as<int>(block_size, ofs);
	for (std::size_t idx = 0; idx < group1.size(); ++idx)
		for (std::size_t jdx = 0; jdx < group2.size(); ++jdx)
			write_binary_as<float>(contactmap(idx, jdx), ofs);
	write_binary_as<int>(block_size, ofs);
	ofs.close();

	return 0;
}
