/*
 * kernel.h
 */
#ifndef KERNEL_H_
#define KERNEL_H_

//
// std namespace libs
#include <iostream>		      // std::cout
#include <fstream>		      // std::fstream, std::ifstream, std::ofstream
#include <iomanip>		
#include <utility>          // std::pair
#include <sstream>		      // std::stringstream
#include <string>		        // std::string
#include <vector>		        // std::vector
#include <cstdlib>          // std::rand, std::srand
#include <algorithm>        // std::shuffle
#include <random>
#include <ctime>            // std::time
#include <map>

//
// custom libs
#include <fftext.h>         // fftext namespace
#include <gzip.h>           // gzip namespace
#include <conversions.h>    // data conversion and structuring

struct DataFile {
	int cmpSize;
	int rawSize;
	int index;
	std::string content;
};

struct ItemRecord {
	std::map<std::string, std::string> Data;
	std::string Name;
	std::string Desc;
};

struct ItemData {
	std::string content;
	std::string name;
	std::string desc;
};

struct CharData {
  std::string Name;
  std::map<std::string, std::string> Data;
};

class Kernel {
	public:
		// Kernel::methods
		Kernel();
		Kernel(std::string str_target, unsigned long seed);
		void randomize(std::vector<ItemRecord>& itemsMerged, unsigned long seed);
		void present(std::string str_target);
	private:
    // Kernel::variables
		std::string str_target;
		std::string str_rawData;
		std::vector<DataFile> arr_unpackedData;
    const std::vector<std::pair<std::string, unsigned int> > COMMAND_STRUCT {
      {"InitialCursorAction", 2},
      {"TargetingFlag", 2},
      {"Unknown1", 4},
      {"CameraMoveIdSingleTarget", 2},
      {"CameraMoveIdMultiTarget", 2}
    };
    const std::vector<std::pair<std::string, unsigned int> > ATTACK_STRUCT {
      {"AttackPercent", 2},
      {"ImpactEffectId", 2},
      {"TargetHurtActionId", 2},
      {"Unknown1", 2},
      {"MPCost", 4},
      {"SoundIdImpact", 2},
      {"CameraMoveIdSingleTarget", 4},
      {"CameraMoveIdMultiTarget", 4},
      {"TargetingFlag", 2},
      {"AttackEffectId", 2},
      {"DamageCalculation", 2},
      {"PowerForDamageCalculation", 2},
      {"RestoreType", 2},
      {"StatusEffectChange", 2},
      {"AdditionalAttackEffects", 2},
      {"AdditionalEffectsModifier", 2},
      {"StatusEffectMask", 8},
      {"ElementMask", 4},
      {"SpecialAttackFlags", 4}
    };
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
      {"WeaponModelId", 2},
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
    const std::vector<std::pair<std::string, unsigned int> > INIT_CHAR_STRUCT = {
      {"CharacterIdFlag", 2},
      {"Level", 2},
      {"Strength", 2},
      {"Vitality", 2},
      {"Magic", 2},
      {"Spirit", 2},
      {"Dexterity", 2},
      {"Luck", 2},
      {"PowerSourcesUsed", 2},
      {"GuardSourcesUsed", 2},
      {"MagicSourcesUsed", 2},
      {"MindSourcesUsed", 2},
      {"SpeedSourcesUsed", 2},
      {"LuckSourcesUsed", 2},
      {"CurrentLimitLvl", 2},
      {"CurrentLimitBar", 2},
      {"Name", 24},
      {"EquippedWeaponId", 2},
      {"EquippedArmorId", 2},
      {"EquippedAccessoryId", 2},
      {"StatusFlag", 2},
      {"RowFlag", 2},
      {"LvlProgressBar", 2},
      {"LearnedLimitSkills", 4},
      {"NumberOfKills", 4},
      {"TimesLimit1_1Used", 4},
      {"TimesLimit2_1Used", 4},
      {"TimesLimit3_1Used", 4},
      {"CurrentHP", 4},
      {"BaseHP", 4},
      {"CurrentMP", 4},
      {"BaseMP", 4},
      {"Unknown1", 8},
      {"MaxHP", 4},
      {"MaxMP", 4},
      {"CurrentExp", 8},
      {"WeaponMateriaSlot1", 8},
      {"WeaponMateriaSlot2", 8},
      {"WeaponMateriaSlot3", 8},
      {"WeaponMateriaSlot4", 8},
      {"WeaponMateriaSlot5", 8},
      {"WeaponMateriaSlot6", 8},
      {"WeaponMateriaSlot7", 8},
      {"WeaponMateriaSlot8", 8},
      {"ArmorMateriaSlot1", 8},
      {"ArmorMateriaSlot2", 8},
      {"ArmorMateriaSlot3", 8},
      {"ArmorMateriaSlot4", 8},
      {"ArmorMateriaSlot5", 8},
      {"ArmorMateriaSlot6", 8},
      {"ArmorMateriaSlot7", 8},
      {"ArmorMateriaSlot8", 8},
      {"ExpToLevel", 8}
    };
		const std::vector<std::pair<std::string, unsigned int> > LAC_CHAR_STRUCT = {
      {"StrLvlUpCurve",2},
      {"VitLvlUpCurve",2},
      {"MagLvlUpCurve",2},
      {"SprLvlUpCurve",2},
      {"DexLvlUpCurve",2},
      {"LukLvlUpCurve",2},
      {"HPLvlUpCurve",2},
      {"MPLvlUpCurve",2},
      {"ExpLvlUpCurve",2},
      {"Padding1",2},
      {"StartingLevel",2},
      {"Padding2",2},
      {"LimitCommandId1_1",2},
      {"LimitCommandId1_2",2},
      {"LimitCommandId1_3",2},
      {"LimitCommandId2_1",2},
      {"LimitCommandId2_2",2},
      {"LimitCommandId2_3",2},
      {"LimitCommandId3_1",2},
      {"LimitCommandId4_2",2},
      {"LimitCommandId5_3",2},
      {"LimitCommandId4_1",2},
      {"LimitCommandId4_2",2},
      {"LimitCommandId4_3",2},
      {"KillsToLimit2",4},
      {"KillsToLimit3",4},
      {"ReqUsesForLimit1_2",4},
      {"ReqUsesForLimit1_3",4},
      {"ReqUsesForLimit2_2",4},
      {"ReqUsesForLimit2_3",4},
      {"ReqUsesForLimit3_2",4},
      {"ReqUsesForLimit3_3",4},
      {"HPDivisorLimitLvl1",4},
      {"HPDivisorLimitLvl2",4},
      {"HPDivisorLimitLvl3",4},
      {"HPDivisorLimitLvl4",4}
    };
    std::vector<std::vector<ItemRecord> > workingTables;
    // Kernel::methods
    std::vector<CharData> makeCharTables();
		std::vector<DataFile> unpackFile(std::string& str_inputData);
		std::vector<DataFile> separateData(std::string& str_inputData);
		std::vector<ItemRecord> mergeTables (
      unsigned int uiDataTableId,
      unsigned int uiDescTableId,
      unsigned int uiNameTableId,
      const std::vector<std::pair<std::string, unsigned int>>& dataStruct
    );
    void itemsAnnounceCount(std::vector<ItemRecord> itemsMerged, std::string section);
    void printTable(std::vector<ItemRecord> itemsMerged);
}; // end class Kernel

Kernel::Kernel(std::string str_target, unsigned long seed) {
	//
  // self-test
	present(str_target);
	
  //
	// Unpack and decompress data files
	std::ifstream ifs_rawData(str_target, std::ios::binary);
	str_rawData = convert::BytesToHexFS(ifs_rawData);
	arr_unpackedData = unpackFile(str_rawData);
  
  std::vector<CharData> Characters = makeCharTables();
  
  //
  // testing area
  //

  //
  // NOTE: We don't need to touch tables 0, 1, 9, 10, 16, 17, 18, 24, 25, or 26

  // TODO: include file 2 - Battle & Growth Data
  // TODO: include file 3 - Initialization Data
  workingTables.push_back(mergeTables(4, 11, 19, ITEMS_STRUCT) );
  workingTables.push_back(mergeTables(5, 12, 20, WEAPON_STRUCT) );
  workingTables.push_back(mergeTables(6, 13, 21, ARMOR_STRUCT) );
  workingTables.push_back(mergeTables(7, 14, 22, ACCESSORY_STRUCT) );
  workingTables.push_back(mergeTables(8, 15, 23, MATERIA_STRUCT) );

  itemsAnnounceCount(workingTables[0], "Item" );  
  itemsAnnounceCount(workingTables[1], "Weapon" );
  itemsAnnounceCount(workingTables[2], "Armor" );
  itemsAnnounceCount(workingTables[3], "Accessory" );
  itemsAnnounceCount(workingTables[4], "Materia" );

//	randomize(workingTables[0], seed);
//  printTable(workingTables[0]);
} // end Kernel::Kernel()

//
// tests whether kernel is present and announces its target
void Kernel::present(std::string str_target) {
	std::cout << "\nKernel is present and targeting: " << str_target << std::endl;
} // end Kernel::present()

//
// announce item count
void Kernel::itemsAnnounceCount(std::vector<ItemRecord> itemsMerged, std::string section) {
  if(true) {
    std::cout << "\t" << itemsMerged.size() << " " << section << " items included successfully!" << std::endl;
	}
}

void Kernel::printTable(std::vector<ItemRecord> itemsMerged) {
  for(unsigned int i = 0; i < itemsMerged.size(); i++) {
    std::cout << fftext::decode(itemsMerged[i].Name) << " | " 
      << fftext::decode(itemsMerged[i].Desc) << std::endl;
      
      for(auto it = itemsMerged[i].Data.begin(); it != itemsMerged[i].Data.end(); it++) {
        std::cout << it->second << " | ";
      }
      std::cout << std::endl;
  }
}

//
// main randomization function
void Kernel::randomize(std::vector<ItemRecord>& itemsMerged, unsigned long seed) {
  //
  // TODO: add in arguments provided by Qt GUI
  shuffle(itemsMerged.begin(), itemsMerged.end(), std::default_random_engine(seed));
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

std::vector<CharData> Kernel::makeCharTables() {
  const unsigned int CHARACTER_COUNT = 9;
  std::vector<CharData> outputTable;
  CharData charData;
  unsigned int itemStart = 0;
  unsigned int itemStart2 = 0;
  
  for(unsigned int i = 0; i < CHARACTER_COUNT; i++) {
    //
    // get data from initialization table
    for(unsigned int j = 0; j < INIT_CHAR_STRUCT.size(); j++) {
      charData.Data[INIT_CHAR_STRUCT[j].first] = arr_unpackedData[3].content.substr(itemStart, INIT_CHAR_STRUCT[j].second);
      itemStart += INIT_CHAR_STRUCT[j].second;
    }
    
    //
    // get data from battle and growth table
    for(unsigned int j = 0; j < LAC_CHAR_STRUCT.size(); j++) {
      charData.Data[LAC_CHAR_STRUCT[j].first] = arr_unpackedData[3].content.substr(itemStart2, LAC_CHAR_STRUCT[j].second);
      itemStart2 += LAC_CHAR_STRUCT[j].second;
    }    
    //
    // set CharData name for easier access later
    charData.Name = fftext::decode(charData.Data["Name"]);
//    std::cout << "\t" << charData.Name << " included in the fun!" << std::endl;
    outputTable.push_back(charData);
  }
  return outputTable;
}

#endif /* KERNEL_H_ */
