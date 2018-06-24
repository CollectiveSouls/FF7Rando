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

#endif /* UTILITY_H_ */
