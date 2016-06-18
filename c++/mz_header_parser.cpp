
#include "mz_header.hpp"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <iostream>

int main(int argc, char* argv[]) {
	boost::program_options::options_description description;
	description.add_options()
		("help,h", "produces this help message")
		("exe,e", boost::program_options::value<std::string>(), "The MZ executable to parse the header for.")
	;
	
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, description), vm);
	boost::program_options::notify(vm);
	
	if(vm.count("help")) {
		std::cout << description << std::endl;
		return 0;
	}
	
	boost::filesystem::path file_path;
	if(vm.count("exe")) {
		std::string exe_file = vm["exe"].as<std::string>();
		
		if(boost::filesystem::exists(exe_file)) {
			if(!boost::filesystem::is_directory(exe_file)) {
				file_path = exe_file;
			} else {
				std::cerr << exe_file << " is a folder" << std::endl;
				std::cerr << std::endl;
				std::cerr << description << std::endl;
				return -1;
			}
		} else {
			std::cerr << "file: " << exe_file << " does not exist" << std::endl;
			std::cerr << std::endl;
			std::cerr << description << std::endl;
			return -1;
		}
	} else {
		std::cerr << "Option \"exe_file\" is required";
		std::cerr << std::endl;
		std::cerr << description << std::endl;
		return -1;
	}
	
	std::ifstream file(file_path.string());
	auto x = parse_mz_file(file);
	std::cout << x << std::endl;
	
	return 0;
}