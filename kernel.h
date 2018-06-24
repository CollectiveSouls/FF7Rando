/*
 * kernel.h
 */

#ifndef KERNEL_H_
#define KERNEL_H_

// std libs
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
// custom libs
#include <gzip.h>
#include <utility.h>

class Kernel {
	public:
		Kernel();
		Kernel(std::string str_target);
		void randomize(std::vector<bool> flags);
		std::string present(std::string str_target);
	private:
		// variables
		std::string str_target;
		std::string kernel_data_raw;
		std::string str_fileData;
		std::vector<std::string> kernel_data_files;
		// methods
		std::vector<std::pair<int, std::string>> materiaExclusions(std::vector<std::string> arr);
		std::vector<std::string> separate(std::string str_inputData);
		std::string ungzip(std::string target);
		std::vector<std::string> unpack(std::string str_inputData);

}; // end class Kernel

Kernel::Kernel(std::string str_target) {
	std::ifstream ifs_fileData(str_target, std::ios::binary);
	str_fileData = BytesToHexFS(ifs_fileData);

	// TODO: Move unpacking into here and make it a private method

	// self-test
	std::cout << present(str_target) << std::endl;
	
	// Unpack and decompress data files
	unpack(str_target);
	
	// dumps raw kernel file hex
	// std::cout << str_fileData << std::endl;
} // end Kernel::Kernel()

// main randomization function
void Kernel::randomize(std::vector<bool> flags) {
	// TODO: Translate everything over from python.
} // end Kernel::randomize()

std::string Kernel::present(std::string str_target) {
	std::string returnString = "\nKernel is present and targeting: " + str_target;
	return returnString;
} // end Kernel::present()


/*
// Begin Private Methods
*/

// splits the kernel files into vectors and ungzips the result
std::vector<std::string> Kernel::separate(std::string str_inputData) {
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

std::string Kernel::ungzip(std::string target) {
	std::string outputString;
	
	Gzip compressedFile;
	outputString = compressedFile.decompress(target);
	
	return outputString;

} // end Kernel::ungzip()

std::vector<std::string> Kernel::unpack(std::string str_inputData) {
	std::vector<std::string> outputData;
	kernel_data_files = separate(str_fileData);

	// std::cout << kernel_data_files[0] << std::endl;
	
	std::string tempString;
	for(unsigned short i; i < kernel_data_files.size(); i++) {
		tempString = ungzip(kernel_data_files[i]);
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

// exclude dummied and master materias
std::vector<std::pair<int, std::string>> Kernel::materiaExclusions(std::vector<std::string> arr) {
	std::vector<std::pair<int,std::string>> dataExclusions;
	std::string tempStr;
	
	for (unsigned short i = 0; i < (arr.size() / 2); i++) {
		tempStr = arr[i];
		if ( arr[i].substr(0,16) == "ffffffffffffffff" ) {
			dataExclusions.push_back(std::make_pair(i, tempStr));
			//dataExclusions.push_back(tempStr);
		}
	}
	
	return dataExclusions;
} // end Kernel::materiaExclusions()

#endif /* KERNEL_H_ */
