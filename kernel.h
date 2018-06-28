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
#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
// custom libs
#include <utility.h>

class Kernel {
	public:
		Kernel();
		Kernel(std::string str_target);
		void randomize();
		std::string present(std::string str_target);
	private:
		// variables
		std::string str_target;
		std::string str_rawData;
		std::vector<DataFile> arr_unpackedData;
		std::vector<DataFile> arr_randoDataFile;
		// methods
		std::vector<DataFile> unpackFile(std::string& str_inputData);
		std::vector<DataFile> separateData(std::string& str_inputData);
		void randomizeItems(std::string& str_originData);
		void randomizeWeapons(std::string& str_originData);
		void randomizeArmors(std::string& str_originData);
		void randomizeAccessories(std::string& str_originData);
		void randomizeMateria(std::string& str_originData);
		// old and needs refactoring
		// void disassemble();
		// void reassemble();
		// std::vector<std::pair<unsigned int, std::string> > materiaExclusions(std::vector<std::string> arr);
}; // end class Kernel

Kernel::Kernel(std::string str_target) {
	// self-test
	std::cout << present(str_target) << std::endl;
	
	// Unpack and decompress data files
	std::ifstream ifs_rawData(str_target, std::ios::binary);
	str_rawData = convert::BytesToHexFS(ifs_rawData);

	arr_unpackedData = unpackFile(str_rawData);
	arr_randoDataFile = arr_unpackedData; // seeding the indicies
	
	randomize();
} // end Kernel::Kernel()

std::string Kernel::present(std::string str_target) {
	std::string returnString = "\nKernel is present and targeting: " + str_target;
	return returnString;
} // end Kernel::present()
// main randomization function

void Kernel::randomize() {
	randomizeItems(arr_unpackedData[4].content);
	randomizeWeapons(arr_unpackedData[5].content);
	randomizeArmors(arr_unpackedData[6].content);
	randomizeAccessories(arr_unpackedData[7].content);
	randomizeMateria(arr_unpackedData[8].content);
} // end Kernel::randomize()

/*
// Begin Private Methods
*/
std::vector<DataFile> Kernel::separateData(std::string& str_inputData) {
	std::vector<DataFile> arr_outputData;
	unsigned short cmpSize = 0;
	unsigned short rawSize = 0;
	unsigned short index = 0;
	std::string content;
	unsigned short position = 0;
	std::string tempStr;

	for(unsigned short i = 0; position < str_inputData.length(); i++) {
		// get compressed data size
		tempStr = str_inputData.substr(position, 4);
		tempStr = convert::endianSwapS(tempStr);
		cmpSize = stoi(tempStr, nullptr, 16) *2;
		position = position + 4;
		// get raw data size
		tempStr = str_inputData.substr(position, 4);
		tempStr = convert::endianSwapS(tempStr);
		rawSize = stoi(tempStr, nullptr, 16) *2;
		position = position + 4;
		// get index
		tempStr = str_inputData.substr(position, 4);
		tempStr = convert::endianSwapS(tempStr);
		index = stoi(tempStr, nullptr, 16) *2;
		position = position + 4;
		// get data
		content = str_inputData.substr(position, cmpSize);
		position += cmpSize;
		
		// needed for the 2 byte eof footer
		if(position == str_inputData.length() - 4) {
			position += 4;
		}
		arr_outputData.push_back({cmpSize, rawSize, index, content});
	}

	return arr_outputData;
} // end Kernel::separateData()

std::vector<DataFile> Kernel::unpackFile(std::string& str_inputData) {
	std::vector<DataFile> arr_outputData;
	std::vector<DataFile> arr_rawFiles = separateData(str_inputData);
	std::string dcmpString;
	std::string convString;
	
	for(unsigned short i; i < arr_rawFiles.size(); i++) {
		dcmpString = gzip::decompress(arr_rawFiles[i].content);
		convString = convert::bin2hex(dcmpString);
		arr_outputData.push_back({arr_rawFiles[i].cmpSize, arr_rawFiles[i].rawSize, arr_rawFiles[i].index, convString} );
	}
	return arr_outputData;
} // end Kernel::unpackFile()

void Kernel::randomizeItems(std::string& str_originData) {
	std::vector<std::string> tempContainer;
	unsigned short position = 0;
	const unsigned short ROW_SIZE = 56;
	std::string str_outputData;
	
	while(position < str_originData.size() ) {
		tempContainer.push_back(str_originData.substr(position, ROW_SIZE) );
		position += ROW_SIZE;
	}
	
	std::random_shuffle(tempContainer.begin(), tempContainer.end() );
	
	for(unsigned short i = 0; i < tempContainer.size(); i++) {
		str_outputData.append(tempContainer[i]);
	}
	
	arr_randoDataFile[4].content = str_outputData;
	std::cout << "\tKernel::Items randomized successfully!" << std::endl;
}

void Kernel::randomizeWeapons(std::string& str_originData) {
	std::vector<std::string> tempContainer;
	unsigned short position = 0;
	const unsigned short ROW_SIZE = 88;
	std::string str_outputData;
	
	while(position < str_originData.size() ) {
		tempContainer.push_back(str_originData.substr(position, ROW_SIZE) );
		position += ROW_SIZE;
	}
	
	std::random_shuffle(tempContainer.begin(), tempContainer.end() );
	
	for(unsigned short i = 0; i < tempContainer.size(); i++) {
		str_outputData.append(tempContainer[i]);
	}
	
	arr_randoDataFile[5].content = str_outputData;
	std::cout << "\tKernel::Weapons randomized successfully!" << std::endl;
}

void Kernel::randomizeArmors(std::string& str_originData) {
	std::vector<std::string> tempContainer;
	unsigned short position = 0;
	const unsigned short ROW_SIZE = 72;
	std::string str_outputData;
	
	while(position < str_originData.size() ) {
		tempContainer.push_back(str_originData.substr(position, ROW_SIZE) );
		position += ROW_SIZE;
	}
	
	std::random_shuffle(tempContainer.begin(), tempContainer.end() );
	
	for(unsigned short i = 0; i < tempContainer.size(); i++) {
		str_outputData.append(tempContainer[i]);
	}
	
	arr_randoDataFile[6].content = str_outputData;
	std::cout << "\tKernel::Armors randomized successfully!" << std::endl;
}

void Kernel::randomizeAccessories(std::string& str_originData) {
	std::vector<std::string> tempContainer;
	unsigned short position = 0;
	const unsigned short ROW_SIZE = 32;
	std::string str_outputData;
	
	while(position < str_originData.size() ) {
		tempContainer.push_back(str_originData.substr(position, ROW_SIZE) );
		position += ROW_SIZE;
	}
	
	std::random_shuffle(tempContainer.begin(), tempContainer.end() );
	
	for(unsigned short i = 0; i < tempContainer.size(); i++) {
		str_outputData.append(tempContainer[i]);
	}
	
	arr_randoDataFile[7].content = str_outputData;
	std::cout << "\tKernel::Accessories randomized successfully!" << std::endl;
}

void Kernel::randomizeMateria(std::string& str_originData) {
	std::vector<std::string> tempContainer;
	unsigned short position = 0;
	const unsigned short ROW_SIZE = 72;
	std::string str_outputData;
	
	while(position < str_originData.size() ) {
		tempContainer.push_back(str_originData.substr(position, ROW_SIZE) );
		position += ROW_SIZE;
	}
	
	std::random_shuffle(tempContainer.begin(), tempContainer.end() );
	
	for(unsigned short i = 0; i < tempContainer.size(); i++) {
		str_outputData.append(tempContainer[i]);
	}
	
	arr_randoDataFile[8].content = str_outputData;
	std::cout << "\tKernel::Materia randomized successfully!" << std::endl;
}


/*
void Kernel::reassemble() {
	for(int i = arr_ffTexts.size(); i >= 0; i--) {
		std::cout << i + 9 << std::endl;
	}
	for(int i = arr_dsmFileDataHex.size()-1; i >= 0; i--) {
		std::cout << i << std::endl;
	}
}
 
void Kernel::disassemble() {
	// TODO: split individual files and data structs
	unsigned int rowSize;
	std::vector<unsigned int> dataPoints;
	unsigned int tempValue;
	
	std::cout << "DSM began successfully" << std::endl;

	for(unsigned int i = 0; i < arr_unpackedFileDataHex.size(); i++) {
		if(i == 0) {				// Command Data
			rowSize = 8;
			dataPoints = {1,1,2,2,2};
			arr_dsmFileDataHex.push_back(fhandler::split2WorkingData(arr_unpackedFileDataHex[i], rowSize, dataPoints) );
		} else if (i == 1) {		// Attack Data
			rowSize = 28;
			dataPoints = {1,1,1,1,2,2,2,2,1,1,1,1,1,1,1,1,4,2,2};
			arr_dsmFileDataHex.push_back(fhandler::split2WorkingData(arr_unpackedFileDataHex[i], rowSize, dataPoints));
		} else if (i == 2) {		// Battle & Growth Data
			rowSize = 0;
			dataPoints = {56,56,56,56,56,56,56,56,56,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,1508,540,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,64,56};
			arr_dsmFileDataHex.push_back(fhandler::split2WorkingData(arr_unpackedFileDataHex[i], rowSize, dataPoints));
		} else if (i == 3) {		// Initialization Data
			rowSize = 2876;
			dataPoints = {2876};
			arr_dsmFileDataHex.push_back(fhandler::split2WorkingData(arr_unpackedFileDataHex[i], rowSize, dataPoints));
		} else if (i == 4) {		// Item Data
			rowSize = 28;
			dataPoints = {8,2,2,1,1,1,1,1,1,1,1,4,2};
			arr_dsmFileDataHex.push_back(fhandler::split2WorkingData(arr_unpackedFileDataHex[i], rowSize, dataPoints));
		} else if (i == 5) {		// Weapon Data
			rowSize = 44;
			dataPoints = {1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2};
			arr_dsmFileDataHex.push_back(fhandler::split2WorkingData(arr_unpackedFileDataHex[i], rowSize, dataPoints));
		} else if (i == 6) {		// Armor Data
			rowSize = 36;
			dataPoints = {1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,2,2,2,1,1,1,1,1,1,1,1,2,2};
			arr_dsmFileDataHex.push_back(fhandler::split2WorkingData(arr_unpackedFileDataHex[i], rowSize, dataPoints));
		} else if (i == 7) {		// Accessory Data
			rowSize = 16;
			dataPoints = {1,1,1,1,1,1,2,4,2,2};
			arr_dsmFileDataHex.push_back(fhandler::split2WorkingData(arr_unpackedFileDataHex[i], rowSize, dataPoints));
		} else if (i == 8) {		// Materia Data
			rowSize = 20;
			dataPoints = {1,1,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1};
			arr_dsmFileDataHex.push_back(fhandler::split2WorkingData(arr_unpackedFileDataHex[i], rowSize, dataPoints));
		} else if (i >= 9) {		// FFText Files
			rowSize = 0;
			arr_ffTexts = fhandler::processFFText(arr_unpackedFileDataHex[i]);
			//std::cout << arr_ffTexts[2] << std::endl;
			std::cout << "FFtext 9_" << i - 9 << " Completed Successfully!\n\n";
		}
		// std::cout << "DSM completed successfully " << i << " times." << std::endl;
	}
}

// exclude dummied and master materias
std::vector<std::pair<unsigned int, std::string>> Kernel::materiaExclusions(std::vector<std::string> arr) {
	std::vector<std::pair<unsigned int,std::string>> dataExclusions;
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
*/

#endif /* KERNEL_H_ */
