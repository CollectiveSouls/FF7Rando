/*
 * kernel.h
 */
#ifndef KERNEL_H_
#define KERNEL_H_

// std libs
#include <iostream>		  // std::cout
#include <fstream>		  // std::fstream, std::ifstream, std::ofstream
#include <iomanip>		
#include <utility>      // std::pair
#include <sstream>		  // std::stringstream
#include <string>		    // std::string
#include <vector>		    // std::vector
#include <cstdlib>      // std::rand, std::srand
#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
// custom libs
#include <utility.h>    // data conversion and structuring

class Kernel {
	public:
		// Kernel::methods
		Kernel();
		Kernel(std::string str_target);
		void randomize(std::vector<ItemRecord> itemsMerged);
		std::string present(std::string str_target);
	private:
    // Kernel::variables
		std::string str_target;
		std::string str_rawData;
		std::vector<DataFile> arr_unpackedData;
		std::vector<DataFile> arr_randoDataFile;
    const std::vector<std::pair<std::string, unsigned int> > ITEMS_STRUCT {
      {"Unknown1", 16},
      {"CameraMovementId", 4},
      {"MenuRestriction", 4},
      {"TargetingFlag", 2},
      {"AttackEffectId", 2},
      {"DamageCalculation", 2},
      {"PowerForDamageCalculation", 2},
      {"ConditionSubmenu", 2},
      {"StatusEffectChange", 2},
      {"AdditionalAttackEffects",2},
      {"AdditionalEffectsModifier", 2},
      {"StatusEffectMask", 8},
      {"ElementMask", 4},
      {"SpecialAttackFlags", 4}
    };
    const std::vector<std::pair<std::string, unsigned int> > WEAPON_STRUCT = {
      {"TargetingFlag", 2},
      {"AttackEffectId", 2},
      {"DamageCalculation", 2},
      {"Unknown1", 2},
      {"PowerForDamageCalculation", 2},
      {"StatusAttack", 2},
      {"MateriaGrowthModifier", 2},
      {"CriticalHitPercent", 2},
      {"WeaponHitPercent", 2},
      {"ModelAlignment", 2},
      {"HighSoundId", 2},
      {"CameraMovementId", 4},
      {"CharacterMask", 4},
      {"ElementMask", 4},
      {"Unknown2", 4},
      {"StatId1", 2},
      {"StatId2", 2},
      {"StatId3", 2},
      {"StatId4", 2},
      {"Stat1Increase", 2},
      {"Stat2Increase", 2},
      {"Stat3Increase", 2},
      {"Stat4Increase", 2},
      {"MateriaSlot1", 2},
      {"MateriaSlot2", 2},
      {"MateriaSlot3", 2},
      {"MateriaSlot4", 2},
      {"MateriaSlot5", 2},
      {"MateriaSlot6", 2},
      {"MateriaSlot7", 2},
      {"MateriaSlot8", 2},
      {"SoundIdHitNormal", 2},
      {"SoundIdHitCritital", 2},
      {"SoundIdHitMissed", 2},
      {"SoundIdImpact", 2},
      {"SpecialAttackFlags", 4},
      {"MenuRestriction", 4}
    };
    const std::vector<std::pair<std::string, unsigned int> > ARMOR_STRUCT = {
      {"Unknown1", 2},
      {"ElementAffinity", 2},
      {"Defense", 2},
      {"MDefense", 2},
      {"DefensePercent", 2},
      {"MDefensePercent", 2},
      {"StatusDefenseId", 2},
      {"Unknown2", 4},
      {"MateriaSlot1", 2},
      {"MateriaSlot2", 2},
      {"MateriaSlot3", 2},
      {"MateriaSlot4", 2},
      {"MateriaSlot5", 2},
      {"MateriaSlot6", 2},
      {"MateriaSlot7", 2},
      {"MateriaSlot8", 2},
      {"MateriaGrowthModifier", 2},
      {"CharacterMask", 4},
      {"ElementMask", 4},
      {"Unknown3", 4},
      {"StatId1", 2},
      {"StatId2", 2},
      {"StatId3", 2},
      {"StatId4", 2},
      {"Stat1Increase", 2},
      {"Stat2Increase", 2},
      {"Stat3Increase", 2},
      {"Stat4Increase", 2},
      {"MenuRestriction", 4},
      {"Unknown4", 4}
    };
    const std::vector<std::pair<std::string, unsigned int> > ACCESSORY_STRUCT = {
      {"StatId1", 2},
      {"StatId2", 2},
      {"Stat1Increase", 2},
      {"Stat2Increase", 2},
      {"ElementAffinity", 2},
      {"SpecialEffect", 2},
      {"ElementMask", 4},
      {"StatusMask", 8},
      {"CharacterMask", 4},
      {"MenuRestriction", 4}
    };
    const std::vector<std::pair<std::string, unsigned int> > MATERIA_STRUCT = {
      {"APReq1", 4},
      {"APReq2", 4},
      {"APReq3", 4},
      {"APReq4", 4},
      {"EffectValue", 2},
      {"StatusMask", 6},
      {"ElementID", 2},
      {"MateriaType", 2},
      {"StrModifier", 2},
      {"VitModifier", 2},
      {"MagModifier", 2},
      {"SprModifier", 2},
      {"DexModifier", 2},
      {"LukModifier", 2}
    };
		// Kernel::methods
		std::vector<DataFile> unpackFile(std::string& str_inputData);
		std::vector<DataFile> separateData(std::string& str_inputData);
		std::vector<ItemRecord> mergeTables (
      unsigned int uiDataTableId,
      unsigned int uiDescTableId,
      unsigned int uiNameTableId,
      const std::vector<std::pair<std::string, unsigned int>>& dataStruct
    );
}; // end class Kernel

Kernel::Kernel(std::string str_target) {
	// self-test
	std::cout << present(str_target) << std::endl;
	
	// Unpack and decompress data files
	std::ifstream ifs_rawData(str_target, std::ios::binary);
	str_rawData = convert::BytesToHexFS(ifs_rawData);

	arr_unpackedData = unpackFile(str_rawData);
	arr_randoDataFile = arr_unpackedData; // seeding the indicies
  
  //
  // NOTE: likely going to be called individually, this is mainly set-up for mass testing purposes
	randomize(mergeTables(4, 11, 19, ITEMS_STRUCT) );
	randomize(mergeTables(5, 12, 20, WEAPON_STRUCT) );
	randomize(mergeTables(6, 13, 21, ARMOR_STRUCT) );
	randomize(mergeTables(7, 14, 22, ACCESSORY_STRUCT) );
	randomize(mergeTables(8, 15, 23, MATERIA_STRUCT) );
} // end Kernel::Kernel()

std::string Kernel::present(std::string str_target) {
	std::string returnString = "\nKernel is present and targeting: " + str_target;
	return returnString;
} // end Kernel::present()
// main randomization function

void Kernel::randomize(std::vector<ItemRecord> itemsMerged) {
  //
  // TODO: add arguments provided by Qt interface

  //
  // announce item count
  if(true) {
    std::cout << itemsMerged.size() << " items included successfully!" << std::endl;
	}

  //
  // quick randomization
  //  std::random_shuffle(itemsMerged.begin(), itemsMerged.end() );
  
  //
  // merged struct testing
  if(false) {
    for(unsigned int i = 0; i < itemsMerged.size(); i++) {
      std::cout << fftext::decode(itemsMerged[i].Name) << " | " 
        << fftext::decode(itemsMerged[i].Desc) << std::endl;
        
        for(auto it = itemsMerged[i].Data.begin(); it != itemsMerged[i].Data.end(); it++) {
          std::cout << it->second << " | ";
        }
        std::cout << std::endl;
    }
  }
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
		//
    // get compressed data size
		tempStr = str_inputData.substr(position, 4);
		tempStr = convert::endianSwapS(tempStr);
		cmpSize = stoi(tempStr, nullptr, 16) *2;
		position = position + 4;
		
    //
    // get raw data size
		tempStr = str_inputData.substr(position, 4);
		tempStr = convert::endianSwapS(tempStr);
		rawSize = stoi(tempStr, nullptr, 16) *2;
		position = position + 4;
		
    //
    // get index
		tempStr = str_inputData.substr(position, 4);
		tempStr = convert::endianSwapS(tempStr);
		index = stoi(tempStr, nullptr, 16) *2;
		position = position + 4;
		
    //
    // get data
		content = str_inputData.substr(position, cmpSize);
		position += cmpSize;
		
    //
		// trims the 2 byte eof footer
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

std::vector<ItemRecord> Kernel::mergeTables (
	unsigned int uiDataTableId,
	unsigned int uiDescTableId,
	unsigned int uiNameTableId,
	const std::vector<std::pair<std::string, unsigned int>>& dataStruct
) {
	//
	// prepare fftext files
	std::vector<std::string> nameData = fftext::unpack(arr_unpackedData[uiNameTableId].content);
	std::vector<std::string> descData = fftext::unpack(arr_unpackedData[uiDescTableId].content);
	std::vector<std::string> itemsData;
	std::vector<ItemRecord> itemsMerged;
	ItemRecord mergedItemData;
	unsigned int rowSize = 0;
	unsigned int itemStart = 0;

  //
  // generate row size
  for(unsigned int i = 0; i < dataStruct.size(); i++) {
     rowSize += dataStruct[i].second;
  }

	//
	// populate working data
	for(unsigned int i = 0; i < arr_unpackedData[uiDataTableId].content.size(); i+=rowSize) {
		itemsData.push_back(arr_unpackedData[uiDataTableId].content.substr(i, rowSize));
	}

  for(unsigned int i = 0; i < itemsData.size(); i++) {
		//
		// populate dataStruct.Data
		itemStart = 0;    
		for(unsigned int j = 0; j < dataStruct.size(); j++) {
 			mergedItemData.Data[dataStruct[j].first] = itemsData[i].substr(itemStart,dataStruct[j].second);
			itemStart = itemStart + dataStruct[j].second;
		}
		mergedItemData.Name = nameData[i];
		mergedItemData.Desc = descData[i];
		itemsMerged.push_back(mergedItemData);
	}
	
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
  
	return itemsMerged;
} //end mergeTables()

#endif /* KERNEL_H_ */
