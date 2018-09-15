/*
 * gzip.h
 */
 
#ifndef GZIP_H_
#define GZIP_H_

//
// std namespace libs
#include <sstream>
//
// boost namespace libs
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

/*
 * gzip compression handlers
 */
namespace gzip {
	std::string compress(const std::string& data) {
		std::stringstream compressed;
		std::stringstream origin(data);

		boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
		out.push(boost::iostreams::gzip_compressor(boost::iostreams::gzip_params(boost::iostreams::gzip::best_compression)));
		out.push(origin);
		boost::iostreams::copy(out, compressed);

		return compressed.str();
	}

	std::string decompress(std::string& inputData)	{
		std::stringstream compressed;
		std::stringstream decompressed;
		
		compressed << convert::hex2bin(inputData);
		
		boost::iostreams::filtering_istream in;
		in.push(boost::iostreams::gzip_decompressor());
		in.push(compressed);
		boost::iostreams::copy(in, decompressed);

		return decompressed.str();
	}
}

#endif /* GZIP_H_ */
