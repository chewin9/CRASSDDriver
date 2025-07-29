#include "shell.h"
#include<string>
#include<fstream>
#include<iostream>

std::string Shell::read(std::string command) {
	//parser
	// 
	// SSD 수행해줘
	// 
	// 
	// file 읽어
	std::ifstream ssdOutputFp("ssd_output.txt");
	std::string outputData = "";

	if (!ssdOutputFp.is_open()) {
		std::cout << "open fail\n";
	}
	if (!std::getline(ssdOutputFp, outputData)) return "";
	ssdOutputFp.close();

	return outputData;

}
