/*
 * utility.h
 */
 
#ifndef UTILITY_H_
#define UTILITY_H_

// std libs
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <cassert>
#include <bitset>
// boost libs
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

struct ItemData {
	std::string content;
	std::string name;
	std::string desc;
};

struct DataFile {
	int cmpSize;
	int rawSize;
	int index;
	std::string content;
};

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
		
		compressed << convert::hex2bin(inputData);
		
		boost::iostreams::filtering_istream in;
		in.push(boost::iostreams::gzip_decompressor());
		in.push(compressed);
		boost::iostreams::copy(in, decompressed);

		return decompressed.str();
	}
}

/*
 * fftext file handlers
 */
namespace fftext {
	// returns the offset value in chars
	unsigned int uint_findPosition(unsigned int uint_initOffset, std::string& str_targetData, std::string& str_dataWindow) {
		unsigned int uint_distance = str_dataWindow.size() * 2;
		unsigned int uint_offset = str_targetData.size() - uint_distance - uint_initOffset;
		unsigned int uint_dataWindowSize = str_dataWindow.size();
		std::string str_targetSubstring;
		
		while(uint_offset < 128) {
			str_targetSubstring = str_targetData.substr(uint_offset, uint_dataWindowSize);
			
			if(str_dataWindow == str_targetSubstring) {
				return uint_offset - str_dataWindow.size();
			} else {
				uint_offset += 2;
			}
		}
		return 0;
	}

	// time to do some heavy lifting
	std::string relzs(std::string& str_inputData) {
		unsigned int int_count = 0;
		unsigned int int_offset = 0;
		std::string str_outputData;
		unsigned int uint_position;
		std::string str_workingData;
		unsigned int uint_workingDataSize = str_inputData.size();
		std::string str_dataWindow;
		unsigned int uint_dataWindowStart;
		unsigned int uint_dataWindowSize = 10;
		// temporary variables for conversion work
		std::string str_tempBin;
		unsigned int int_tempInt;
		
		while(uint_dataWindowSize > 3) {
			// initialize values
			uint_dataWindowStart = uint_workingDataSize - uint_dataWindowSize;
			str_dataWindow = str_workingData.substr(uint_dataWindowStart, uint_dataWindowSize);
			
			uint_position = uint_findPosition(int_offset, str_workingData, str_dataWindow);

			if(uint_position != 0) {
				str_outputData.insert(0, str_workingData.substr(uint_workingDataSize - 2, 2) );
				str_workingData.erase(uint_workingDataSize - 2, 2);
			} else {
				str_tempBin.insert(0,std::bitset<2>((uint_dataWindowSize - 4) / 2).to_string() );
				str_tempBin.insert(2,std::bitset<6>(uint_position).to_string() );
				int_tempInt = stoi(str_tempBin, nullptr, 2);
				str_outputData.insert(0, "F9" + convert::int2hex(int_tempInt) );
				str_workingData.erase(uint_workingDataSize - uint_dataWindowSize, uint_dataWindowSize);
			}
			
			if(int_count == str_inputData.size() ) {
				uint_dataWindowSize--;
			}
		}
		return str_outputData;
	}
	
	std::string unlzs(std::string& str_inputData) {
		std::string str_outputData;
		std::string dataWindow;
		std::string str_lzsPointer;
		int int_lzsPointer;
		unsigned short bytes2Read;
		short lzsOffset;
		
		for(unsigned int i = 0; i < str_inputData.size(); i+=2) {
			// !TODO: Add support for the battle message files which include text replacement flag bytes.
			if(str_inputData.substr(i,2) == "f9") {
					str_lzsPointer = str_inputData.substr(i+2,2);
					int_lzsPointer = stoi(str_lzsPointer, nullptr, 16);
					str_lzsPointer = std::bitset<8>(int_lzsPointer).to_string();
					bytes2Read = stoi(str_lzsPointer.substr(0,2), nullptr, 2)* 2 + 4;
					lzsOffset = stoi(str_lzsPointer.substr(2,6), nullptr, 2) + 1;

					dataWindow = str_inputData.substr(i-(lzsOffset*2), bytes2Read*2);
					str_outputData.append(dataWindow);
					i += 2;
			} else {
				dataWindow = str_inputData.substr(i,2);
				str_outputData.append(dataWindow);
			}
		}
		return str_outputData;
	}

	// fully unpacks fftext documents passed to it.
	std::vector<std::string> unpack(std::string& inputData) {
		std::vector<std::string> outputData;
		std::string tempStorage;
		std::string adjustedData;
		std::string dataWindow;

		// calculate header for exclusion
		std::string rawHeaderSize = inputData.substr(0,4);
		std::string swappedHeader = convert::endianSwapS(rawHeaderSize);
		unsigned int headerSize = stoi(swappedHeader, nullptr, 16) * 2;
		
		adjustedData = inputData.substr(headerSize,inputData.size()-headerSize);
		
		std::string unpackedData = unlzs(adjustedData);

		for(unsigned int i = 0; i < unpackedData.size(); i+=2) {
			dataWindow = unpackedData.substr(i,2);
			if(unpackedData.substr(i,2) == "ff") {
				if(unpackedData.substr(i,4) == "ffff") {
					dataWindow = unpackedData.substr(i,4);
					tempStorage.append(dataWindow);
					outputData.push_back(tempStorage);
					tempStorage.clear();
					i += 2;
				} else {
					dataWindow = unpackedData.substr(i,2);
					tempStorage.append(dataWindow);
					outputData.push_back(tempStorage);
					tempStorage.clear();
				}
			} else if(unpackedData.substr(i,2) == "") {
				outputData.push_back(tempStorage.substr(0,tempStorage.size()-2));
				break;
			} else {
				dataWindow = unpackedData.substr(i,2);
				tempStorage.append(dataWindow);
			}
		}
		return outputData;
	}
	
	// converts FFText to readable ASCII text
	void decode(std::string& str_inputData) {
		std::string ffText = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ ÄÅÇÉÑÖÜáàâäãåçéèêëíìîïñóòôöõúùûü♥°¢£↔→♪ßα  ´¨≠ÆØ∞±≤≥¥µ∂ΣΠπ⌡ªºΩæø¿¡¬√ƒ≈∆«»… ÀÃÕŒœ–—“”‘’÷◊ÿŸ⁄ ‹›ﬁﬂ■‧‚„‰ÂÊÁËÈÍÎÏÌÓÔ ÒÚÛÙıˆ˜¯˘˙˚¸˝˛ˇ       ";
		std::string str_outputData;
		unsigned int getChar;
		for(unsigned int i = 0; i < str_inputData.size(); i+=2) {
			if(str_inputData.substr(i,2) == "ff") {
				break;
			} else {
				getChar = stoi(str_inputData.substr(i,2), nullptr, 16);
				str_outputData.append(ffText.substr(getChar,1) );
			}
		}
		std::cout << str_outputData << std::endl;
	}
	
	// changes text in unpacked fftext strings to Al Bhed cipher encoding
	std::string albhedEncode(const std::string& inputStr) {
		unsigned int inputNum = std::stoi(inputStr, nullptr, 16);
		unsigned int outputNum;
		std::stringstream outputHex;
		
		switch (inputNum) {
			case 34:					// A
				outputNum = 58;			// Y
				break;
			case 35:					// B
				outputNum = 49;			// P
				break;
			case 36:					// C
				outputNum = 45;			// L
				break;
			case 37:					// D
				outputNum = 53;			// T
				break;
			case 38:					// E
				outputNum = 34;			// A
				break;
			case 39:					// F
				outputNum = 55;			// W
				break;
			case 40:					// G
				outputNum = 44;			// K
				break;
			case 41:					// H
				outputNum = 51;			// R
				break;
			case 42:					// I
				outputNum = 38;			// E
				break;
			case 43:					// J
				outputNum = 59;			// Z
				break;
			case 44:					// K
				outputNum = 40;			// G
				break;
			case 45:					// L
				outputNum = 46;			// M
				break;
			case 46:					// M
				outputNum = 52;			// S
				break;
			case 47:					// N
				outputNum = 35;			// H
				break;
			case 48:					// O
				outputNum = 54;			// U
				break;
			case 49:					// P
				outputNum = 35;			// B
				break;
			case 50:					// Q
				outputNum = 57;			// X
				break;
			case 51:					// R
				outputNum = 47;			// N
				break;
			case 52:					// S
				outputNum = 36;			// C
				break;
			case 53:					// T
				outputNum = 37;			// D
				break;
			case 54:					// U
				outputNum = 42;			// I
				break;
			case 55:					// V
				outputNum = 43;			// J
				break;
			case 56:					// W
				outputNum = 39;			// F
				break;
			case 57:					// X
				outputNum = 50;			// Q
				break;
			case 58:					// Y
				outputNum = 48;			// O
				break;
			case 59:					// Z
				outputNum = 56;			// W
				break;
			case 66:					// a
				outputNum = 90;			// y
				break;
			case 67:					// b
				outputNum = 81;			// p
				break;
			case 68:					// c
				outputNum = 77;			// l
				break;
			case 69:					// d
				outputNum = 85;			// t
				break;
			case 70:					// e
				outputNum = 66;			// a
				break;
			case 71:					// f
				outputNum = 87;			// v
				break;
			case 72:					// g
				outputNum = 76;			// k
				break;
			case 73:					// h
				outputNum = 83;			// r
				break;
			case 74:					// i
				outputNum = 70;			// e
				break;
			case 75:					// j
				outputNum = 91;			// z
				break;
			case 76:					// k
				outputNum = 72;			// g
				break;
			case 77:					// l
				outputNum = 78;			// m
				break;
			case 78:					// m
				outputNum = 84;			// s
				break;
			case 79:					// n
				outputNum = 73;			// h
				break;
			case 80:					// o
				outputNum = 86;			// u
				break;
			case 81:					// p
				outputNum = 67;			// b
				break;
			case 82:					// q
				outputNum = 89;			// x
				break;
			case 83:					// r
				outputNum = 79;			// n
				break;
			case 84:					// s
				outputNum = 68;			// c
				break;
			case 85:					// t
				outputNum = 69;			// d
				break;
			case 86:					// u
				outputNum = 74;			// i
				break;
			case 87:					// v
				outputNum = 75;			// j
				break;
			case 88:					// w
				outputNum = 71;			// f
				break;
			case 89:					// x
				outputNum = 82;			// q
				break;
			case 90:					// y
				outputNum = 80;			// o
				break;
			case 91:					// z
				outputNum = 88;			// w
				break;
			default:
				outputNum = inputNum;
		}
		
		outputHex << std::setfill('0') << std::setw(2) << std::hex << outputNum;
		
		return outputHex.str();
	}
}
#endif /* UTILITY_H_ */
