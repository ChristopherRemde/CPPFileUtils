#include "Test.h"
#include <stdexcept>

void Test::Log(std::string message)
{
	std::cout << message << std::endl;
}


void Test::Compare(bool resultIs, bool resultExpected, std::string testname)
{
		LogResult(std::to_string(resultIs), std::to_string(resultExpected), testname, resultIs == resultExpected);
}

void Test::Compare(int resultIs, int resultExpected, std::string testname)
{
	LogResult(std::to_string(resultIs), std::to_string(resultExpected), testname, resultIs == resultExpected);
}

void Test::Compare(std::string resultIs, std::string resultExpected, std::string testname)
{
	LogResult(resultIs, resultExpected, testname, resultIs == resultExpected);
}

void Test::Compare(char* resultIs, char* resultExpected, std::string testname)
{
	LogResult("Char*", "Char*", testname, *resultIs == *resultExpected);
}

void Test::Compare(std::filesystem::path resultIs, std::filesystem::path resultExpected, std::string testname)
{
	LogResult(resultIs.string(), resultExpected.string(), testname, resultIs == resultExpected);
}

void Test::LogResult(std::string resultIs, std::string resultexpected, std::string testname, bool success)
{
	if (success)
		Log(testname + "				OK");
	else
	{
		Log(testname + ": Result is: " + resultIs + ",  expected result: " + resultexpected + "		FAIL");
		Log("Stopping tests, " + testname + " was not successfull");
		throw std::runtime_error("Test failed!");
	}

}

void Test::TestFolderBasics(std::filesystem::path testPath)
{
	testPath /= "TestFolderContainer";
	if (!FileUtils::CreateNewFolder(testPath))
		Compare(false, true, "SetupTestFolder");

	std::string folderName = "folderTest";
	std::string renamedFolderName = "renamedFolderTest";
	std::filesystem::path folderTestPath = testPath / folderName;
	std::filesystem::path renamedTestPath = testPath / renamedFolderName;
	std::filesystem::path movedTestPath = testPath / folderName / renamedFolderName;


	try
	{
		Compare(FileUtils::CreateNewFolder(folderTestPath), true, "CreateNewFolder");
		Compare(FileUtils::FolderExists(folderTestPath), true, "FolderExists");
		Compare(FileUtils::RenameFolder(folderTestPath, renamedTestPath), true, "RenameFolder");
		Compare(FileUtils::CopyFolder(renamedTestPath, folderTestPath), true, "CopyFolder");
		Compare(FileUtils::MoveFolder(renamedTestPath, folderTestPath), true, "MoveFolder");
		Compare(FileUtils::DeleteFolder(folderTestPath), true, "DeleteFolder");

	}

	catch (...)
	{
		FileUtils::DeleteFolder(testPath);
		return;
	}

	FileUtils::DeleteFolder(testPath);

	Log("All Basic folder operation tests successfull");
	Log(" ");
}

void Test::TestFileBasics(std::filesystem::path testPath)
{
	testPath /= "TestFileContainer";
	std::string textFileName = "fileTest.txt";
	std::string binaryFileName = "fileTest.bin";
	std::string renamedFileName = "Renamed.txt";
	std::string copiedFileName = "Copied.txt";
	std::string movedFoldername = "MovedTest";

	std::filesystem::path movedFolderPath = testPath / movedFoldername;
	if (!FileUtils::CreateNewFolder(testPath))
		Compare(false, true, "SetupTestFolder");
	if(!FileUtils::CreateNewFolder(movedFolderPath))
		Compare(false, true, "SetupMoveTestFolder");

	std::filesystem::path textFileTestPath = testPath / textFileName;
	std::filesystem::path binaryFileTestPath = testPath / binaryFileName;
	std::filesystem::path renamedFilePath = testPath / renamedFileName;
	std::filesystem::path movedFilePath = movedFolderPath / textFileName;
	std::filesystem::path copiedFilePath = testPath / copiedFileName;


	try
	{
		Compare(FileUtils::WriteTextFile(textFileTestPath, "Test"), true, "WriteFile");
		Compare(FileUtils::FileExists(textFileTestPath), true, "FileExists");
		Compare(FileUtils::RenameFile(textFileTestPath, renamedFilePath), true, "RenameFile");
		Compare(FileUtils::MoveFile(renamedFilePath, movedFilePath), true, "MoveFile");
		Compare(FileUtils::CopyFile(movedFilePath, copiedFilePath), true, "CopyFile");
		Compare(FileUtils::DeleteFile(movedFilePath), true, "DeleteFile");


		Compare(FileUtils::WriteTextFile(textFileTestPath, "Test"), true, "WriteTextFile");
		Compare(FileUtils::ReadTextFile(textFileTestPath), "Test", "ReadTextFile");

		const int size = 5;
		char byteBuffer[size] = { 0,1,2,3,4 };
		Compare(FileUtils::WriteBinaryFile(binaryFileTestPath, byteBuffer, size), true, "WriteBinaryFile");

		char* readBuffer = FileUtils::ReadBinaryFile(binaryFileTestPath);
		Compare(readBuffer, byteBuffer, "ReadBinaryFile");
		delete[] readBuffer;

	}

	catch (...)
	{
		FileUtils::DeleteFolder(testPath);
		return;
	}

	FileUtils::DeleteFolder(testPath);
	Log("All Basic file operation tests successfull");
	Log(" ");
}

void Test::TestDiscovery(std::filesystem::path testPath)
{
	testPath /= "TestFileContainer";
	if (!FileUtils::CreateNewFolder(testPath))
		Compare(false, true, "SetupTestFolder");

	int count = 10;

	for (size_t i = 0; i < count; i++)
	{
		FileUtils::CreateNewFolder(testPath / ("test" + std::to_string(i)));
		FileUtils::WriteTextFile(testPath / ("test" + std::to_string(i) + ".txt"), "Test");
	}

	try
	{
		Compare(FileUtils::GetFilesByExtension(testPath, ".txt").size(), 10, "GetFilesByExtension");
		Compare(FileUtils::GetFilesByName(testPath, "test").size(), 10, "GetFilesByExtension");
		Compare(FileUtils::GetFoldersByName(testPath, "test").size(), 10, "GetFoldersByName");
	}

	catch (...)
	{
		FileUtils::DeleteFolder(testPath);
		return;
	}

	FileUtils::DeleteFolder(testPath);
	Log("All file/folder discovery operation tests successfull");
	Log(" ");

}

void Test::TestConversions(std::filesystem::path testPath)
{
	std::string testFolderName = "ConversionTest";
	std::string testChildFolderName = "ChildFolder";
	std::string testFileName = "TestFile";
	std::string testFileExt = ".txt";
	std::string testFileNameWithExt = testFileName + testFileExt;
	testPath /= testFolderName;
	std::filesystem::path testFilePath = testPath / testFileNameWithExt;
	std::filesystem::path testChildFolderPath = testPath / testChildFolderName;

	if(!FileUtils::CreateNewFolder(testPath))
		Compare(false, true, "SetupTestFolder");
	if (!FileUtils::WriteTextFile(testFilePath, "test"))
		Compare(false, true, "SetupTestFile");
	if (!FileUtils::CreateNewFolder(testChildFolderPath))
		Compare(false, true, "SetupTestChildFolder");

	std::string testFilePathNumber1 = "TestFile12345678.txt";
	std::string testFilePathNumber2 = "12345678TestFile1.txt";
	std::string testFilePathNumber3 = "Tes12345678tFile.txt";
	std::string testFilePathNumber4 = "TestFile_12345678.txt";
	std::string testFilePathNumber5 = "12345678.txt";

	try
	{
		Compare(FileUtils::GetFilename(testFilePath), testFileName, "GetFileName");
		Compare(FileUtils::GetFileExtension(testFilePath), testFileExt, "GetFileExtension");
		Compare(FileUtils::GetFilenameWithExtension(testFilePath), testFileNameWithExt, "GetFileNameAndExtension");
		
		Compare(FileUtils::GetFolderName(testFilePath), testFolderName, "GetFoldernameFromFilePath");
		Compare(FileUtils::GetFolderName(testChildFolderPath), testChildFolderName, "GetFoldernameFromFolderPath");
		
		Compare(FileUtils::GetParentFolder(testFilePath), testPath, "GetParentFolderFromFile");
		Compare(FileUtils::GetParentFolder(testChildFolderPath), testPath, "GetParentFolderFromFolder");

		Compare(FileUtils::GetIntFromFilename(testFilePathNumber1), 12345678, "GetIntFromFilename1");
		Compare(FileUtils::GetIntFromFilename(testFilePathNumber2), 12345678, "GetIntFromFilename2");
		Compare(FileUtils::GetIntFromFilename(testFilePathNumber3), 12345678, "GetIntFromFilename3");
		Compare(FileUtils::GetIntFromFilename(testFilePathNumber4), 12345678, "GetIntFromFilename4");
		Compare(FileUtils::GetIntFromFilename(testFilePathNumber5), 12345678, "GetIntFromFilename5");
	}

	catch (...)
	{
		FileUtils::DeleteFolder(testPath);
		return;
	}

	FileUtils::DeleteFolder(testPath);
	Log("All file/folder conversion operation tests successfull");
	Log(" ");
}

int Test::RunAllTests(std::filesystem::path testPath)
{
	TestFolderBasics(testPath);
	TestFileBasics(testPath);
	TestDiscovery(testPath);
	TestConversions(testPath);
	Log("\r \r ");
	Log("All tests successfull");
	return 0;
}


int main()
{
	Test test;
	test.RunAllTests("C:\\FileUtilTest\\");
}
