#ifndef CONTACTMAP_CALCULATION_HPP
#define CONTACTMAP_CALCULATION_HPP
#include "coordinate.hpp"
#include <eigen3/Eigen/Core>
#include <array>
#include <cmath>


class ContactMapCalculator
{
	public:
		static void run(
			const Coordinate& snap, const std::vector<int>& group1,
			const std::vector<int>& group2, const float cutoff,
			const int mask, const bool is_periodic, Eigen::MatrixXf& contactmap);
	
	private:
		static std::array<float, 3> adjust_pbc(const std::array<float, 3>& pos, const std::array<float, 3>& box);
		static float calculate_shift_number(const float& r, const float& L);
};

void ContactMapCalculator::run(
	const Coordinate& snap, const std::vector<int>& group1,
	const std::vector<int>& group2, const float cutoff,
	const int mask, const bool is_periodic, Eigen::MatrixXf& contactmap)
{
	const float cutoff_sqr = cutoff * cutoff;
	for (std::size_t idx = 0; idx < group1.size(); ++idx)
	{
		const int& group1_id = group1[idx];
		std::array<float, 3> ri = snap.atom_of(group1_id);
		if (is_periodic) { ri = adjust_pbc(ri, snap.pbc_box()); }
		for (std::size_t jdx = 0; jdx < group2.size(); ++jdx)
		{
			const int& group2_id = group2[jdx];
			if (std::abs(group1_id - group2_id) < mask) continue;
			std::array<float, 3> rj = snap.atom_of(group2_id);
			if (is_periodic) { rj = adjust_pbc(rj, snap.pbc_box()); }
			float d_sqr = 0.0;
			for (std::size_t idim = 0; idim < 3; ++idim)
				d_sqr += (ri[idim] - rj[idim]) * (ri[idim] - rj[idim]);
			if (d_sqr < cutoff_sqr) ++contactmap(idx, jdx);
		}
	}
}

std::array<float, 3> ContactMapCalculator::adjust_pbc(const std::array<float, 3>& pos, const std::array<float, 3>& box)
{
	const float x_shift_num = calculate_shift_number(pos[0], box[0]);
	const float y_shift_num = calculate_shift_number(pos[1], box[1]);
	const float z_shift_num = calculate_shift_number(pos[2], box[2]);
	return {pos[0] - x_shift_num * box[0], pos[1] - y_shift_num * box[1], pos[2] - z_shift_num * box[2]};
}

float ContactMapCalculator::calculate_shift_number(const float& r, const float& L)
{
	if (r < 0.0) return calculate_shift_number(r + L, L) - 1.0;
	else if (r > L) return calculate_shift_number(r - L, L) + 1.0;
	else return 0.0;
}

#endif // CONTACTMAP_CALCULATION_HPP
