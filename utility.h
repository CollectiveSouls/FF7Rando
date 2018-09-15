/*
 * utility.h
 */
 
#ifndef UTILITY_H_
#define UTILITY_H_

//
// std namespace libs
#include <sstream>
#include <string>
//
// boost namespace libs
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

/*
 * binary/hex data conversion handlers
 */
namespace convert {
	std::string int2hex(int int_inputData) {
		std::stringstream sstr_outputData;
		sstr_outputData << std::setfill('0') << std::setw(2) << std::hex << int_inputData;
		
		return sstr_outputData.str();
	}
	
	std::string hex2bin(std::string const& s) {
		assert(s.length() % 2 == 0);

		std::string sOut;
		sOut.reserve(s.length()/2);

		std::string extract;
		for (std::string::const_iterator pos = s.begin(); pos<s.end(); pos += 2) {
			extract.assign(pos, pos+2);
			sOut.push_back(std::stoi(extract, nullptr, 16));
		}
		return sOut;
	}

	std::string bin2hex(std::string& bytes) {
		std::stringstream bin(bytes);
		unsigned char x;
		std::stringstream hex;

		// convert file's binary to hex and fix up the data
		while (bin >> std::noskipws >> x) {
			hex << std::setfill('0') << std::setw(2) << std::hex << static_cast<unsigned int>(x);
		}
		
		return hex.str();
	}


	std::string BytesToHexFS(std::ifstream& bytes) {
		unsigned char x;
		std::stringstream hex;

		// convert file's binary to hex and fix up the data
		while (bytes >> std::noskipws >> x) {
			hex << std::setfill('0') << std::setw(2) << std::hex << static_cast<unsigned int>(x);
		}
		
		return hex.str();
	}

	// swap endian-ness of a hex string
	std::string endianSwapS(std::string& input) {
		std::string output;
		std::string temp;
		
		for(unsigned int i = 0; i < (input.length() / 2); i++) {
			output.insert(0, input.substr(i*2, 2));
		}
		
		return output;
	}
}

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

#endif /* UTILITY_H_ */
