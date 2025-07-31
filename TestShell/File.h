#pragma once
#include <string>

class IFile {
public:
	virtual std::string ReadOutputFile(const std::string& filename) = 0;
};

class File : public IFile{
public:
	std::string ReadOutputFile(const std::string& filename);
};