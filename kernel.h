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
#include <utility.h>

class Kernel {
	public:
		Kernel();
		Kernel(std::string str_target);
		void randomize(std::vector<bool> flags);
		std::string present(std::string str_target);
	private:
		// variables
		std::string str_rawFileDataHex;
		std::vector<std::string> arr_unpackedFileDataHex;
		std::vector<std::string> kernel_data_files;
		std::vector<std::vector<std::string>> arr_dsmFileDataHex;
		// methods
		std::string str_target;
		std::vector<std::vector<std::string>> disassemble(std::vector<std::string> inputData);
		std::vector<std::pair<int, std::string>> materiaExclusions(std::vector<std::string> arr);
}; // end class Kernel

Kernel::Kernel(std::string str_target) {
	std::ifstream ifs_fileData(str_target, std::ios::binary);
	str_rawFileDataHex = BytesToHexFS(ifs_fileData);
	std::vector<std::string> arr_unpackedFileDataHex;
	std::vector<std::vector<std::string>> arr_dsmFileDataHex;

	// dumps raw kernel file hex
	// std::cout << str_rawFileDataHex << std::endl;

	// self-test
	std::cout << present(str_target) << std::endl;
	
	// Unpack and decompress data files
	arr_unpackedFileDataHex = unpackFile(str_rawFileDataHex);
	
	std::cout << arr_unpackedFileDataHex[0] << std::endl;
	
	// arr_dsmFileDataHex = finalDsm(arr_unpackedFileDataHex);
	
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

std::vector<std::vector<std::string>> Kernel::disassemble(std::vector<std::string> inputData) {
	std::vector<std::vector<std::string>> outputData;
	// TODO: split individual files and data structs
	/*
	unsigned short rowSize;
	std::vector<unsigned short> dataPoints;

	for(int i = 0; i < arr_unpackedFileDataHex.size(); i++) {
		switch(i) {
			case 0:		// Command Data
				rowSize = 8;
				dataPoints = {1,1,2,2,2};
				secondSplit(arr_unpackedFileDataHex[i], rowSize, dataPoints);
				break;
			case 1:		// Attack Data
				rowSize = 28;
				dataPoints = {1,1,1,1,2,2,2,2,1,1,1,1,1,1,1,1,4,2,2};
				secondSplit(arr_unpackedFileDataHex[i], rowSize, dataPoints);
				break;
			case 2:		// Battle & Growth Data
				rowSize = 0;
				dataPoints = {56,56,56,56,56,56,56,56,56,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,1508,540,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,64,56};
				secondSplit(arr_unpackedFileDataHex[i], rowSize, dataPoints);
				break;
			case 3:		// Initialization Data
				rowSize = 0;
				dataPoints = {}; // This is a SERIOUSLY insane file.
				// Disabled for now, because, again, this is a SERIOUSLY insanely large file.
				// secondSplit(arr_unpackedFileDataHex[i], rowSize, dataPoints);
				break;
			case 4:		// Item Data
				rowSize = 28;
				dataPoints = {8,2,2,1,1,1,1,1,1,1,1,4,2};
				secondSplit(arr_unpackedFileDataHex[i], rowSize, dataPoints);
				break;
			case 5:		// Weapon Data
				rowSize = 44;
				dataPoints = {1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2};
				secondSplit(arr_unpackedFileDataHex[i], rowSize, dataPoints);
				break;
			case 6:		// Armor Data
				rowSize = 36;
				dataPoints = {1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,2,2,2,1,1,1,1,1,1,1,1,2,2};
				secondSplit(arr_unpackedFileDataHex[i], rowSize, dataPoints);
				break;
			case 7:		// Accessory Data
				rowSize = 16;
				dataPoints = {1,1,1,1,1,1,2,4,2,2};
				secondSplit(arr_unpackedFileDataHex[i], rowSize, dataPoints);
				break;
			case 8:		// Materia Data
				rowSize = 20;
				dataPoints = {1,1,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1};
				secondSplit(arr_unpackedFileDataHex[i], rowSize, dataPoints);
				break;
			case >= 9 && <= 27:	// FFText Files
				rowSize = 0;
				processFFText(arr_unpackedFileDataHex[i]);
			default:
				break;
		}
	}*/
	
	return outputData;
}

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
