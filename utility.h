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
// custom libs
#include <gzip.h>

/*
 * Conversion handlers
 */

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


/*
 * Compression handlers
 */
std::string gzipCompress(const std::string& data) {
	namespace bio = boost::iostreams;

	std::stringstream compressed;
	std::stringstream origin(data);

	bio::filtering_streambuf<bio::input> out;
	out.push(bio::gzip_compressor(bio::gzip_params(bio::gzip::best_compression)));
	out.push(origin);
	bio::copy(out, compressed);

	return compressed.str();
}

std::string gzipDecompress(std::string& inputData)	{
	std::stringstream compressed;
	std::stringstream decompressed;
	
	compressed << hex2bin(inputData);
	
	boost::iostreams::filtering_istream in;
	in.push(boost::iostreams::gzip_decompressor());
	in.push(compressed);
	boost::iostreams::copy(in, decompressed);

	return decompressed.str();
}

/*
 * File handlers
 */

std::vector<std::string> fileSplitter(std::string& fileData, std::vector<unsigned int>& dataSizes, unsigned short rowSize = 0) {
	std::vector<std::string> fileRows;
	// splitting happens here
	for(unsigned short i = 0; i < (fileData.length() / rowSize); i++) {
		fileRows.push_back(fileData.substr(i*rowSize, rowSize));
	}
	
	for(unsigned short i = 0; i < fileRows.size(); i++) {
		std::cout << fileRows[i] << std::endl;
	}
	
	return fileRows;
}

std::vector<std::vector<std::string> > prepareFile() {
	std::vector<std::vector<std::string> > outputData;
	return outputData;
}

std::vector<std::string> split2DataMems(std::string& inputData, std::vector<int> dataPoints) {
	std::vector<std::string> outputData;
	
	for(unsigned int i = 0; i < inputData.size(); i+=dataPoints[i]*2) {
		outputData.push_back(inputData.substr(i, dataPoints[i]*2) );
	}
	
	return outputData;
}

std::vector<std::vector<std::string> > split2WorkingData(std::string inputData, unsigned int rowSize, std::vector<int> dataPoints) {
	std::vector<std::vector<std::string> > outputData;
	std::vector<std::string> tempValue;
	std::string targetRow;

	for(unsigned int i = 0; i < inputData.size(); i+=rowSize) {
		targetRow = inputData.substr(i, rowSize);
		tempValue = split2DataMems(targetRow, dataPoints);
		outputData.push_back(tempValue);
	}
	
	return outputData;
}

std::string ungzipString(std::string target) {
	std::string outputString;
	outputString = gzipDecompress(target);
	
	return outputString;

} // end Kernel::ungzip()

// splits the kernel files into vectors and ungzips the result
std::vector<std::string> separateData(std::string& str_inputData) {
	std::vector<std::string> outputData;
	unsigned short cmpSize = 0;
	unsigned short position = 0;
	std::string tempStr;

	while(position < str_inputData.length()) {
		tempStr = str_inputData.substr(position, 4);
		tempStr = endianSwapS(tempStr);
		cmpSize = stoi(tempStr, nullptr, 16) *2;
		position = position + 12;
		
		tempStr = str_inputData.substr(position, cmpSize);
		position = position + cmpSize;
		
		// needed for the 2 byte eof footer
		if(position == str_inputData.length() - 4) {
			position += 4;
		}
		
		outputData.push_back(tempStr.c_str());
	}

	return outputData;
} // end Kernel::separate()

std::vector<std::string> unpackFile(std::string& str_inputData) {
	std::vector<std::string> outputData;
	std::vector<std::string> data_files = separateData(str_inputData);

	// std::cout << data_files[0] << std::endl;
	
	std::string tempString;
	
	for(unsigned short i; i < data_files.size(); i++) {
		tempString = ungzipString(data_files[i]);
		outputData.push_back(bin2hex(tempString) );
		
		// dump data
		// std::cout << tempString << std::endl;
		// OR
		// std::cout << outputData[i] << std::endl;
	}
	// dump specific file
	// std::cout << outputData[0] << std::endl;
	return outputData;
} // end Kernel::unpack()

std::vector<std::string> processFFText(std::string& inputData) {
	std::vector<std::string> outputData;
	std::string tempStorage;
	std::string dataWindow;

	for(unsigned int i = 0; i < inputData.size(); i+=2) {
		dataWindow = inputData.substr(i,2);
		if(inputData.substr(i,2) != "ff") {
			tempStorage.append(dataWindow);
		}
		else {
			tempStorage.append(dataWindow);
			outputData.push_back(tempStorage);
			tempStorage.clear();
		}
	}

	return outputData;
}

// Al Bhed text mode encoder
std::string albhedSwap(const std::string& inputStr) {
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

#endif /* UTILITY_H_ */
