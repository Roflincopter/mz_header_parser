#include "mz_header.hpp"
#include "index_list.hpp"

#include <boost/fusion/adapted/struct.hpp>
#include <boost/fusion/adapted/struct/detail/extension.hpp>

#include <vector>
#include <iomanip>

UnexpectedEOS::UnexpectedEOS()
: std::runtime_error("Unexpected end of stream.")
{}

UnexpectedEOS::UnexpectedEOS(std::__cxx11::string location)
: std::runtime_error("Unexpected end of stream after " + location)
{}

NotAMZFileException::NotAMZFileException()
: std::runtime_error("This stream does not contain a valid MZ executable")
{}

BOOST_FUSION_ADAPT_STRUCT(
	MZHeader,
	(Magic, magic)
	(uint16_t, bytes_in_last_block)
	(uint16_t, blocks_in_file)
	(uint16_t, num_relocs)
	(uint16_t, header_paragraphs)
	(uint16_t, min_extra_paragraphs)
	(uint16_t, max_extra_paragraphs)
	(uint16_t, ss)
	(uint16_t, sp)
	(uint16_t, checksum)
	(uint16_t, ip)
	(uint16_t, cs)
	(uint16_t, reloc_table_offset)
	(uint16_t, overlay_number)
)

template<typename T>
T parse(std::istream& is, std::string name);

template<>
uint16_t parse<uint16_t>(std::istream& is, std::string name) {
	if(!is) {
		throw UnexpectedEOS();
	}
	char data[2];
	is.read(data, 2);
	if(!is) {
		throw UnexpectedEOS(name);
	}
	
	return (data[1] << 8) | data[0];
}

template<>
Magic parse<Magic>(std::istream& is, std::string name) {
	if(!is) {
		throw UnexpectedEOS();
	}
	char data[2];
	is.read(data, 2);
	if(!is) {
		throw UnexpectedEOS(name);
	}
	
	return static_cast<Magic>((data[1] << 8) | data[0]);
}

template <int... Indices>
MZHeader parse_mz_file_impl(std::istream& is, indices<Indices...>) {
	
	return {parse<typename std::decay<typename boost::fusion::result_of::at_c<MZHeader, Indices>::type>::type>(is, boost::fusion::extension::struct_member_name<MZHeader, Indices>::call())...};
	
}

MZHeader parse_mz_file(std::istream& is) {
	
	typedef build_indices<boost::fusion::result_of::size<MZHeader>::value>::type indices;
	
	return parse_mz_file_impl(is, indices{});
}

std::ostream& output_impl(std::ostream& os, MZHeader const&, indices<>) {
	return os;
}

template<typename T>
std::ostream& output(std::ostream& os, T rh) {
	return os << std::hex << std::setw(sizeof(T) * 2) << std::setfill('0') << rh << std::endl;
}

std::string to_string(Magic magic) {
	char* c = reinterpret_cast<char*>(&magic);
	return std::string(c, sizeof(Magic));
}

template <>
std::ostream& output<Magic>(std::ostream& os, Magic m) {
	return os << std::hex << std::setw(4) << std::setfill('0') << m << " (" << to_string(m) << ")" << std::endl;
}

template <int I, int... Indices>
std::ostream& output_impl(std::ostream& os, const MZHeader& header, indices<I, Indices...>) {
	os << boost::fusion::extension::struct_member_name<MZHeader, I>::call() << ": ";
	output(os, boost::fusion::at_c<I>(header));
	return output_impl(os, header, indices<Indices...>{});
}

std::ostream& operator<<(std::ostream& os, MZHeader const& header)
{
	typedef build_indices<boost::fusion::result_of::size<MZHeader>::value>::type indices;
	
	return output_impl(os, header, indices{});
}
