
import mzheader;

import std.stdio;
import std.getopt;
import std.file;
import std.format;

int main(string[] args) {

	bool help;
	string exe;
    
	auto helpInformation = getopt(
		args,
		"exe|e", &exe,
	);
	
	if (helpInformation.helpWanted)
	{
		defaultGetoptPrinter("Info: ", helpInformation.options);
	}

	if(!exe.exists()) {
		stderr.writeln(format("%s %s", exe, " does not exist"));
		defaultGetoptPrinter("Info: ", helpInformation.options);
		return -1;
	}
	if(exe.isDir()) {
		stderr.writeln(format("%s %s", exe, " is a directory"));
		defaultGetoptPrinter("Info: ", helpInformation.options);
		return -1;
	}
	
	auto f = File(exe, "r");
	auto header = parse_mz_header(f);
	output(stdout, header);
	
	return 0;
}
