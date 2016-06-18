#pragma once

#include <cstdint>
#include <cstddef>
#include <stdexcept>
#include <istream>
#include <ostream>

#include <boost/serialization/strong_typedef.hpp>

BOOST_STRONG_TYPEDEF(uint16_t, Magic);

struct NotAMZFileException : public std::runtime_error {
	NotAMZFileException();
};

struct UnexpectedEOS : public std::runtime_error {
	UnexpectedEOS();
	
	UnexpectedEOS(std::string location);
};

struct MZHeader {
	Magic magic;
	uint16_t bytes_in_last_block;
	uint16_t blocks_in_file;
	uint16_t num_relocs;
	uint16_t header_paragraphs;
	uint16_t min_extra_paragraphs;
	uint16_t max_extra_paragraphs;
	uint16_t ss;
	uint16_t sp;
	uint16_t checksum;
	uint16_t ip;
	uint16_t cs;
	uint16_t reloc_table_offset;
	uint16_t overlay_number;
};

uint16_t parse_uint16_t(std::istream& is, std::string name = "");

MZHeader parse_mz_file(std::istream& is);

void print_mz_header(MZHeader const& header);

std::ostream& operator<<(std::ostream& os, MZHeader const& header);

