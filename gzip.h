/*
 * gzip.h
 */

#ifndef __GZIP_H__
#define __GZIP_H__

// std libs
#include <sstream>
// boost libs
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
// custom libs
#include <utility.h>

class Gzip {
public:
	static std::string compress(const std::string& data)
	{
		namespace bio = boost::iostreams;

		std::stringstream compressed;
		std::stringstream origin(data);

		bio::filtering_streambuf<bio::input> out;
		out.push(bio::gzip_compressor(bio::gzip_params(bio::gzip::best_compression)));
		out.push(origin);
		bio::copy(out, compressed);

		return compressed.str();
	}

	std::string decompress(std::string& inputData)	{
		std::stringstream compressed;
		std::stringstream decompressed;
		
		compressed << hex2bin(inputData);
		
		boost::iostreams::filtering_istream in;
        in.push(boost::iostreams::gzip_decompressor());
        in.push(compressed);
		boost::iostreams::copy(in, decompressed);

		return decompressed.str();
	}
};

#endif // __GZIP_H__
