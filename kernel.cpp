/*
 * kernel.cpp
 *
 *  Created on: May 24, 2018
 *      Author: KernelPanix
 */

#include <iostream>
#include <vector>

class Kernel {
	string fileLocation = "";

	vector <string> unpackFiles() {
		vector <string> returnVector = {"This needs to be completed."};
		int fileCount = 0;
		int rowSize = 0;
		vector <int> dataPoints = {};

		// process kernel subfiles
		for(int i = 0; i < fileCount; i++) {
			switch(i) {
				case 0:
					rowSize = 8;
					dataPoints = {1,1,2,2,2};
					processFile(rowSize, dataPoints);
					break;
				case 1:
					rowSize = 28;
					dataPoints = {1,1,1,1,2,2,2,2,1,1,1,1,1,1,1,1,4,2,2};
					processFile(rowSize, dataPoints);
					break;
				case 2:
					rowSize = 0;
					processFile(rowSize, dataPoints);
					break;
				case 3:
					rowSize = 0;
					processFile(rowSize, dataPoints);
					break;
				case 4:
					rowSize = 28;
					dataPoints = {8,2,2,1,1,1,1,1,1,1,1,4,2};
					processFile(rowSize, dataPoints);
					break;
				case 5:
					rowSize = 44;
					dataPoints = {1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2};
					processFile(rowSize, dataPoints);
					break;
				case 6:
					rowSize = 36;
					dataPoints = {1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,2,2,2,1,1,1,1,1,1,1,1,2,2};
					processFile(rowSize, dataPoints);
					break;
				case 7:
					rowSize = 16;
					dataPoints = {1,1,1,1,1,1,2,4,2,2};
					processFile(rowSize, dataPoints);
					break;
				case 8:
					rowSize = 20;
					dataPoints = {1,1,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1};
					processFile(rowSize, dataPoints);
					break;
			}
		}

		return returnVector;
	}

	string repackFiles() {
		string returnString = "This needs to be completed.";

		return returnString;
	}

	void randomizeData(vector <string> flag) {
	}

	vector <string> processFile(int rowSize, vector <int> dataPoints) {
		vector <string> returnVector = {};
		return returnVector;
	}
};
