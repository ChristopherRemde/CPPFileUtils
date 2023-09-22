#pragma once
#include "FileUtils.h";


class Test
{
private:
	void Log(std::string message);
	void Compare(bool actual, bool should, std::string testname);
	void Compare(int actual, int should, std::string testname);
	void Compare(std::string actual, std::string should, std::string testname);
	void Compare(char* resultIs, char* resultExpected, std::string testname);
	void Compare(std::filesystem::path resultIs, std::filesystem::path resultExpected, std::string testname);
	void LogResult(std::string resultIs, std::string resultShould, std::string testname, bool success);
	void TestFolderBasics(std::filesystem::path testPath);
	void TestFileBasics(std::filesystem::path testPath);
	void TestDiscovery(std::filesystem::path testPath);
	void TestConversions(std::filesystem::path testPath);

public:
	int RunAllTests(std::filesystem::path);
};


