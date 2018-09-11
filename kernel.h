/*
 * kernel.h
 */
#ifndef KERNEL_H_
#define KERNEL_H_

// std libs
#include <iostream>		// std::cout
#include <fstream>		// std::fstream, std::ifstream, std::ofstream
#include <iomanip>		
#include <sstream>		// std::stringstream
#include <string>		// std::string
#include <vector>		// std::vector
#include <cstdlib>      // std::rand, std::srand
#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
// custom libs
#include <utility.h>

class Kernel {
	public:
		// Kernel::methods
		Kernel();
		Kernel(std::string str_target);
		void randomize();
		std::string present(std::string str_target);
	private:
		// Kernel::variables
		std::string str_target;
		std::string str_rawData;
		std::vector<DataFile> arr_unpackedData;
		std::vector<DataFile> arr_randoDataFile;
		// Kernel::methods
		std::vector<DataFile> unpackFile(std::string& str_inputData);
		std::vector<DataFile> separateData(std::string& str_inputData);
    std::vector<ItemRecord> structItems(std::vector<std::string> inputData, std::vector<std::string> nameData, std::vector<std::string> descData);
		void randomizeItems();
		void randomizeWeapons(std::string& str_originData);
		void randomizeArmors(std::string& str_originData);
		void randomizeAccessories(std::string& str_originData);
		void randomizeMateria(std::string& str_originData);
		// old and needs refactoring
		// void reassemble();
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
	randomizeItems();
	randomizeWeapons(arr_unpackedData[5].content);
	randomizeArmors(arr_unpackedData[6].content);
	randomizeAccessories(arr_unpackedData[7].content);
	randomizeMateria(arr_unpackedData[8].content);
} // end Kernel::randomize()


/*
// Begin Private Methods
*/

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
	
	std::string unpackedData = fftext::unlzs(adjustedData);

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
		arr_outputData.push_back({cmpSize, rawSize, index, {content}});
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

std::vector<ItemRecord> Kernel::structItems(std::vector<std::string> inputData, std::vector<std::string> nameData, std::vector<std::string> descData) {
  std::vector<ItemRecord> outputData;
	ItemRecord itemData;
	unsigned int tracker = 0;

	for(unsigned int i = 0; i < inputData.size(); i++) {
		for(unsigned int j = 0; j < inputData[i].size(); j+=56) {
			tracker = 0;
			itemData.Data["Unknown1"] = inputData[i].substr(j,16);
			tracker += 16;
			itemData.Data["CameraMovementId"] = inputData[i].substr(j+tracker,4);
			tracker += 4;
			itemData.Data["MenuRestriction"] = inputData[i].substr(j+tracker,4);
			tracker += 4;
			itemData.Data["TargetingFlag"] = inputData[i].substr(j+tracker,2);
			tracker += 2;
			itemData.Data["AttackEffectId"] = inputData[i].substr(j+tracker,2);
			tracker += 2;
			itemData.Data["DamageCalculation"] = inputData[i].substr(j+tracker,2);
			tracker += 2;
			itemData.Data["PowerForDamageCalculation"] = inputData[i].substr(j+tracker,2);
			tracker += 2;
			itemData.Data["ConditionSubmenu"] = inputData[i].substr(j+tracker,2);
			tracker += 2;
			itemData.Data["StatusEffectChange"] = inputData[i].substr(j+tracker,2);
			tracker += 2;
			itemData.Data["AdditionalEffectsModifier"] = inputData[i].substr(j+tracker,2);
			tracker += 2;
			itemData.Data["StatusEffectMask"] = inputData[i].substr(j+tracker,8);
			tracker += 8;
			itemData.Data["ElementMask"] = inputData[i].substr(j+tracker,4);
			tracker += 4;
			itemData.Data["SpecialAttackFlags"] = inputData[i].substr(j+tracker,4);
			itemData.Name = "";
			itemData.Desc = "";
		}
    
		outputData.push_back(itemData);
	}
  
  // add in the name and description data for the items
  for(unsigned int i = 0; i < nameData.size(); i++) {
    outputData[i].Name = nameData[i];
    outputData[i].Desc = descData[i];
  }

	return outputData;
}

void Kernel::randomizeItems() {
  // TODO: add arguments provided by Qt interface
	const unsigned short ROW_SIZE = 56;
  std::vector<std::string> itemsName = fftext::unpack(arr_unpackedData[19].content);
  std::vector<std::string> itemsDesc = fftext::unpack(arr_unpackedData[11].content);
  std::vector<std::string> itemsData;
  std::vector<ItemRecord> itemsMerged;
	
  //
  // populate working data
  for(unsigned int i = 0; i < arr_unpackedData[4].content.size(); i+=ROW_SIZE) {
    itemsData.push_back(arr_unpackedData[4].content.substr(i, ROW_SIZE));
  }
  
  //
  // merge data struct
  itemsMerged = structItems(itemsData, itemsName, itemsDesc);
  
  //
  // remove dummy items
  unsigned int ix = 0;
  while(ix < itemsMerged.size()) {
    if(itemsMerged[ix].Name == "FF") {
      itemsMerged.erase(itemsMerged.begin()+ix);
    }
    else {
      ix++;
    }
  }

  //
  // announce item count
  if(true) {
    std::cout << itemsMerged.size() << " Kernel::Items included successfully!" << std::endl;
	}

  //
  // quick randomization
  std::random_shuffle(itemsMerged.begin(), itemsMerged.end() );
  
  //
  // merged struct testing
  if(false) {
    for(unsigned int i = 0; i < itemsMerged.size(); i++) {
      std::cout << fftext::decode(itemsMerged[i].Name) << " | " 
            << fftext::decode(itemsMerged[i].Desc) << "\n"
            << itemsMerged[i].Data["Unknown1"] << " | "
            << itemsMerged[i].Data["CameraMovementId"] << " | "
            << itemsMerged[i].Data["MenuRestriction"] << " | "
            << itemsMerged[i].Data["TargetingFlag"] << " | " 
            << itemsMerged[i].Data["AttackEffectId"] << " | "
            << itemsMerged[i].Data["DamageCalculation"] << " | "
            << itemsMerged[i].Data["PowerForDamageCalculation"] << " | "
            << itemsMerged[i].Data["ConditionSubmenu"] << " | "
            << itemsMerged[i].Data["StatusEffectChange"] << " | "
            << itemsMerged[i].Data["AdditionalEffectsModifier"] << " | "
            << itemsMerged[i].Data["StatusEffectMask"] << " | "
            << itemsMerged[i].Data["ElementMask"] << " | "
            << itemsMerged[i].Data["SpecialAttackFlags"] << std::endl;
    }
  }
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
	
	std::cout << "\t" << tempContainer.size() << " weapons' data included" << std::endl;
	
	std::random_shuffle(tempContainer.begin(), tempContainer.end() );
	
	for(unsigned short i = 0; i < tempContainer.size(); i++) {
		str_outputData.append(tempContainer[i]);
	}
	
	arr_randoDataFile[5].content = str_outputData;
	
	//std::cout << arr_unpackedData[11].content << std::endl;

	tempContainer = fftext::unpack(arr_unpackedData[12].content);
	std::cout << "\t" << tempContainer.size() << " weapon descriptions included" << std::endl;

	tempContainer = fftext::unpack(arr_unpackedData[20].content);
	std::cout << "\t" << tempContainer.size() << " weapon names included" << std::endl;

	std::cout << "Kernel::Weapons randomized successfully!" << std::endl;
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
	std::cout << "\t" << tempContainer.size() << " armors' data included" << std::endl;
	
	std::random_shuffle(tempContainer.begin(), tempContainer.end() );
	
	for(unsigned short i = 0; i < tempContainer.size(); i++) {
		str_outputData.append(tempContainer[i]);
	}
	
	arr_randoDataFile[6].content = str_outputData;

	tempContainer = fftext::unpack(arr_unpackedData[13].content);
	std::cout << "\t" << tempContainer.size() << " armor descriptions included" << std::endl;

	tempContainer = fftext::unpack(arr_unpackedData[21].content);
	std::cout << "\t" << tempContainer.size() << " armor names included" << std::endl;

	std::cout << "Kernel::Armors randomized successfully!" << std::endl;
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
	std::cout << "\t" << tempContainer.size() << " accessories' data included" << std::endl;
		
	std::random_shuffle(tempContainer.begin(), tempContainer.end() );
	
	for(unsigned short i = 0; i < tempContainer.size(); i++) {
		str_outputData.append(tempContainer[i]);
	}
	
	arr_randoDataFile[7].content = str_outputData;

	tempContainer = fftext::unpack(arr_unpackedData[14].content);
	std::cout << "\t" << tempContainer.size() << " accessory descriptions included" << std::endl;

	tempContainer = fftext::unpack(arr_unpackedData[22].content);
	std::cout << "\t" << tempContainer.size() << " accessory names included" << std::endl;

	std::cout << "Kernel::Accessories randomized successfully!" << std::endl;
}

void Kernel::randomizeMateria(std::string& str_originData) {
	std::vector<std::string> tempContainer;
	unsigned short position = 0;
	const unsigned short ROW_SIZE = 40	;
	std::string str_outputData;
		
	while(position < str_originData.size() ) {
		tempContainer.push_back(str_originData.substr(position, ROW_SIZE) );
		position += ROW_SIZE;
	}
  
	std::cout << "\t" << tempContainer.size() << " materia's data included" << std::endl;
	
	std::random_shuffle(tempContainer.begin(), tempContainer.end() );
	
	for(unsigned short i = 0; i < tempContainer.size(); i++) {
		str_outputData.append(tempContainer[i]);
	}
	
	arr_randoDataFile[8].content = str_outputData;

	tempContainer = fftext::unpack(arr_unpackedData[15].content);
 	std::cout << "\t" << tempContainer.size() << " materia descriptions included" << std::endl;
  
 	tempContainer = fftext::unpack(arr_unpackedData[23].content);
	std::cout << "\t" << tempContainer.size() << " materia names included" << std::endl;

//	std::string recombined;
//	for(unsigned int i = 0; i < tempContainer.size(); i++) {
//		std::cout << tempContainer[i] << std::endl;
//		fftext::decode(tempContainer[i]);
//		recombined.append(tempContainer[i]);
//	}
//	std::string tempString = fftext::relzs(recombined);
//	std::cout << tempString << std::endl;

	std::cout << "Kernel::Materia randomized successfully!" << std::endl;
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
*/

#endif /* KERNEL_H_ */
