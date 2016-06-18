module mzheader;

import std.exception;
import std.format;
import std.stdio;
import std.string;

class NotAMZFileException : Exception {
	this() {
		super("This file is not an MZ file", __FILE__, __LINE__);
	}
}

class UnexpectedEOS : Exception {
	this() {
		super("Unexpected end of stream", __FILE__, __LINE__);
	}
	
	this(string location) {
		super(format("%s: %s", "Unexpected end of stream after", location), __FILE__, __LINE__);
	}
}

struct Magic {
	ushort magic;
	alias magic this;
}

struct MZHeader {
	Magic magic;
	ushort bytes_in_last_block;
	ushort blocks_in_file;
	ushort num_relocs;
	ushort header_paragraphs;
	ushort min_extra_paragraphs;
	ushort max_extra_paragraphs;
	ushort ss;
	ushort sp;
	ushort checksum;
	ushort ip;
	ushort cs;
	ushort reloc_table_offset;
	ushort overlay_number;
};

T parse(T:ushort)(File file) {
	auto buffer = file.rawRead(new char[2]);
	return (buffer[1] << 8) | buffer[0];
}

T parse(T:Magic)(File file) {
	ushort s = parse!ushort(file);
	return Magic(s);
}

MZHeader parse_mz_header(File f) {

	MZHeader header;
	
	foreach(member; __traits(allMembers, MZHeader)) {
		alias MT = typeof(__traits(getMember, header, member)); 
		__traits(getMember, header, member) = parse!MT(f);
	}
	
	return header;
}

string to_header_line(string name, Magic magic) {
	char[2] magic_str;
	magic_str[0] = cast(char) magic;
	magic_str[1] = cast(char) (magic >> 8);
	return format("%s: %04 4x (%s)", name, magic, magic_str);
}

string to_header_line(string name, ushort value) {
	return format("%s: %04 4x", name, value);
}

void output(File f, MZHeader header) {
	foreach(member; __traits(allMembers, MZHeader)) {
		f.writeln(to_header_line(member, __traits(getMember, header, member)));
	}
}


