module mzheader;

import std.exception;
import std.format;
import std.stdio;
import std.string;

class NotAMZFileException : Exception {
	this() {
		super("This file is not an MZ file");
	}
}

class UnexpectedEOS : Exception {
	this() {
		super("Unexpected end of stream");
	}
	
	this(string location) {
		super(format("%s: %s", "Unexpected end of stream after", location));
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

T parse(T:ushort)(File file, string name) {
	enforce(!file.error, new UnexpectedEOS());
	
	char[2] inbuffer;
	auto buffer = file.rawRead(inbuffer);
	
	enforce(!file.error && buffer.length == 2, new UnexpectedEOS(name));
	
	return (buffer[1] << 8) | buffer[0];
}

T parse(T:Magic)(File file, string name) {
	ushort s = parse!ushort(file, name);
	return Magic(s);
}

MZHeader parse_mz_header(File f) {

	MZHeader header;
	
	foreach(member; __traits(allMembers, MZHeader)) {
		alias MT = typeof(__traits(getMember, header, member)); 
		__traits(getMember, header, member) = parse!MT (f, member);
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


