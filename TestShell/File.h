#pragma once
#include <string>
#include <vector>

class IFile {
public:
	virtual std::vector<std::string> ReadScriptFile(const std::string& filename) = 0;
	virtual std::string ReadOutputFile(const std::string& filename) = 0;
};

class File : public IFile{
public:
	std::string ReadOutputFile(const std::string& filename);
	std::vector<std::string> ReadScriptFile(const std::string& filename);
};