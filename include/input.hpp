#ifndef CONTACTMAP_INPUT_OPTION
#define CONTACTMAP_INPUT_OPTION

#include<boost/program_options.hpp>
#include<iostream>
#include<string>

namespace boost_po = boost::program_options;

class Input {
	public:
		Input(int argc, char *argv[]);
		std::vector<std::string> input_name()  const { return ifname; }
		std::string output_name() const { return ofname; }
		std::string group1_name() const { return group1;  }
		std::string group2_name() const { return group2; }
		int         mask_size()   const { return mask; }
		int         start_index() const { return istart; }
		int         end_index()   const { return iend; }
		int			skip_size()   const { return iskip; }
		float       cutoff()      const { return cutoff_length; }

	private:
		std::vector<std::string> ifname;
		std::string ofname;
		std::string group1;
		std::string group2;
		float       cutoff_length;
		int         istart;
		int         iend;
		int         iskip;
		int         mask;
};

Input::Input(int argc, char *argv[]) {
	boost_po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("ifname, i", boost_po::value<std::vector<std::string>>()->multitoken(),
		 "path to an input trajectory list")
		("group1, 1", boost_po::value<std::string>(),
		 "path to file which specifies indices of contact group 1")
		("group2, 2", boost_po::value<std::string>(),
		 "path to file which specifies indices of contact group 2")
		("ofname, o", boost_po::value<std::string>(), "path to output")
		("cutoff, c", boost_po::value<float>()->default_value(10.0),
		 "cutoff length to assume that two beads contact each other")
		("start,  s", boost_po::value<int>()->default_value(0),
		 "index to start calculation")
		("end,    e", boost_po::value<int>()->default_value(-1),
		 "index to end calculation")
		("skip,   k", boost_po::value<int>()->default_value(1),
		 "frame number to skip calculation")
		("mask,   m", boost_po::value<int>()->default_value(5),
		 "number of neighbor beads to ignore contacts");
	
	boost_po::variables_map vm;
	try {
		boost_po::store(boost_po::parse_command_line(argc, argv, desc), vm);
	} catch(const boost_po::error_with_option_name& e) {
		std::cerr << e.what() << std::endl;
		std::exit(1);
	}
	boost_po::notify(vm);
	if (vm.count("help") || !vm.count("ifname") || !vm.count("ofname")
						 || !vm.count("group1") || !vm.count("group2")) {
		std::cerr << desc << std::endl;
		std::exit(1);
	}
	this->ifname = vm["ifname"].as<std::vector<std::string>>();
	this->ofname = vm["ofname"].as<std::string>();
	this->group1 = vm["group1"].as<std::string>();
	this->group2 = vm["group2"].as<std::string>();
	this->cutoff_length = vm["cutoff"].as<float>();
	this->mask   = vm["mask"].as<int>();
	this->istart = vm["start"].as<int>();
	this->iend   = vm["end"].as<int>();
	this->iskip  = vm["skip"].as<int>();
}


#endif // CONTACTMAP_INPUT_OPTION
