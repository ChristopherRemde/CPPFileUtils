#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>


// File System Utilities. These are helper functions that sit ontop of the Filesystem library included since C++ 17.
// These functions try to make file handling with C++ easier, and implement many functions used everyday in apps that rely on much file-processing.

class FileUtils
{
public:
    // Folder basics
    static bool FolderExists(std::filesystem::path path);
    static bool CreateNewFolder(std::filesystem::path path);
    static bool DeleteFolder(std::filesystem::path path);
    static bool RenameFolder(std::filesystem::path path, std::filesystem::path newPath);
    static bool MoveFolder(std::filesystem::path from, std::filesystem::path to);
    static bool CopyFolder(std::filesystem::path src, std::filesystem::path dest);

    // File basics
    static bool FileExists(std::filesystem::path path);
    static bool DeleteFile(std::filesystem::path path);
    static bool RenameFile(std::filesystem::path file, std::filesystem::path renamedFile);
    static bool MoveFile(std::filesystem::path from, std::filesystem::path to);
    static bool CopyFile(std::filesystem::path src, std::filesystem::path dest);

    //File IO
    static bool WriteTextFile(std::filesystem::path path, std::string text);
    static bool WriteBinaryFile(std::filesystem::path path, char* bytes, int size);
    static char* ReadBinaryFile(std::filesystem::path path);
    static std::string ReadTextFile(std::filesystem::path path);

    // File/Folder discovery
    static std::vector<std::filesystem::path> GetFilesByExtension(std::filesystem::path path, std::string extension);
    static std::vector<std::filesystem::path> GetFilesByName(std::filesystem::path path, std::string filenameContains);
    static std::vector<std::filesystem::path> GetFoldersByName(std::filesystem::path path, std::string foldernameContains);
    static std::vector<std::filesystem::path> SortPathsByNumericValue(std::vector<std::filesystem::path> paths, bool ascending);

    // Path conversion
    static std::string GetFilename(std::filesystem::path pathToFile);
    static std::string GetFileExtension(std::filesystem::path pathToFile);
    static std::string GetFilenameWithExtension(std::filesystem::path pathToFile);
    static std::string GetFolderName(std::filesystem::path pathToFolder);
    static std::filesystem::path GetParentFolder(std::filesystem::path pathToFolder);
    static int GetIntFromFilename(std::string fileName);


   
};


/// <summary>
/// Does a folder exist?
/// </summary>
/// <param name="path">The path to the folder</param>
/// <returns>Returns true if the folder exists, false if the folder could not be found</returns>
bool FileUtils::FolderExists(std::filesystem::path path)
{
    try
    {
        if (!std::filesystem::exists(path))
            return false;
    }

    catch (...)
    {
        return false;
    }

    return true;
}

/// <summary>
/// Creates a new folder at the given path. If the to be created folder sits inside one or multiple non-existing parent folders, they will also be created
/// </summary>
/// <param name="path">The desired directoy path where the folder should be created.</param>
/// <returns>Returns true if the folder has been created or already exists, false when the folder could not be created</returns>
bool FileUtils::CreateNewFolder(std::filesystem::path path)
{
    if (FolderExists(path))
        return true;

    else
    {
        try
        {
            if (!std::filesystem::create_directories(path))
                return false;
        }

        catch (...)
        {
            return false;
        }
    }

    return true;
}

/// <summary>
/// Removes the folder and recursively all the content inside of it
/// </summary>
/// <param name="path">The path to the folder</param>
/// <returns>True when the folder has been deleted, false if it could not be deleted, or an error occured</returns>
bool FileUtils::DeleteFolder(std::filesystem::path path)
{
    if (!FolderExists(path))
        return true; //Debatable, but we assume that the non-existence of the dir was the intented action of the user, not the deletion itself

    else
    {
        try
        {
            if (std::filesystem::remove_all(path) < 1)
                return false; //When the path exists but nothing was deleted, indicates error
        }

        catch (...)
        {
            return false;
        }
    }

    return true;
}


/// <summary>
/// Rename a folder
/// </summary>
/// <param name="path">The path the folder with it's current name</param>
/// <param name="newPath">The path to the folder with its new name</param>
/// <returns>True when the folder was successfully renamed, false if an error occured during renaming</returns>
bool FileUtils::RenameFolder(std::filesystem::path path, std::filesystem::path newPath)
{
    if (!FolderExists(path) || FolderExists(newPath))
        return false;

    if (FolderExists(newPath))
        return false;

    try
    {
        std::filesystem::rename(path, newPath);
    }

    catch (...)
    {
        return false;
    }

    return true;
}

/// <summary>
/// Moves the directoy and it's contents to a new location. The parent path of the new folder has to already exist
/// </summary>
/// <param name="from">The path to the folder which will be moved</param>
/// <param name="to">The path to the folder which will contain the moved folder</param>
/// <returns>Returns true if the move has been succesfull, false if the folder could not be moved </returns>
bool FileUtils::MoveFolder(std::filesystem::path from, std::filesystem::path to)
{
    to /= GetFolderName(from);

    if (!FolderExists(from) || FolderExists(to))
        return false;

    try
    {
        std::filesystem::rename(from, to);
    }

    catch (...)
    {
        return false;
    }

    return true;
}


/// <summary>
/// Copies the folder and all of its contents to a new location.
/// </summary>
/// <param name="src">The current path of the folder</param>
/// <param name="dest">The desired location of the duplicated folder, including its own folder name</param>
/// <returns>Returns true when folder could be copied, false if an error has occured, or destination folder already exists</returns>
bool FileUtils::CopyFolder(std::filesystem::path source, std::filesystem::path destination)
{
    if (!FolderExists(source) || FolderExists(destination))
        return false;

    try
    {
        std::filesystem::copy(source, destination, std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
    }

    catch (...)
    {
        return false;
    }

    return true;
}

/// <summary>
/// Does a file exist?
/// </summary>
/// <param name="path">The path to the file</param>
/// <returns>Returns true if the file exists, false if the file could not be found</returns>
bool FileUtils::FileExists(std::filesystem::path path)
{
    try
    {
        if (!std::filesystem::exists(path))
            return false;
    }

    catch (...)
    {
        return false;
    }

    return true;
}


/// <summary>
/// Removes the file
/// </summary>
/// <param name="path">The path to the file</param>
/// <returns>True when the file has been deleted, false if it could not be deleted, or an error occured</returns>
bool FileUtils::DeleteFile(std::filesystem::path path)
{
    if (!FileExists(path))
        return true; //Debatable, but we assume that the non-existence of the file was the intented action of the user, not the deletion itself

    else
    {
        try
        {
            if (!std::filesystem::remove(path))
                return false;
        }

        catch (...)
        {
            return false;
        }
    }

    return true;
}


/// <summary>
/// Rename a file
/// </summary>
/// <param name="path">The path the file</param>
/// <param name="newName">The new name of the file, including it's file extension</param>
/// <returns>True when the file was successfully renamed, false if an error occured during renaming</returns>
bool FileUtils::RenameFile(std::filesystem::path file, std::filesystem::path renamedFile)
{

    if (!FileExists(file) || FileExists(renamedFile))
        return false;

    try
    {
        std::filesystem::rename(file, renamedFile);
    }

    catch (...)
    {
        return false;
    }

    return true;
}

/// <summary>
/// Moves the file. The parent directory of the new file location has to already exist
/// </summary>
/// <param name="from">The current path to the file</param>
/// <param name="to">The new path to the file, including it's own file name and extension</param>
/// <returns>Returns true if the move has been succesfull, false if the file could not be moved </returns>
bool FileUtils::MoveFile(std::filesystem::path from, std::filesystem::path to)
{
    if (!FileExists(from) || FileExists(to))
        return false;

    try
    {
        std::filesystem::rename(from, to);
    }

    catch (...)
    {
        return false;
    }

    return true;
}

/// <summary>
/// Copies the file to a new location.
/// </summary>
/// <param name="src">The current path of the file</param>
/// <param name="dest">The desired location of the duplicated file, including its own file name and extension</param>
/// <returns>Returns true when files could be copied, false if an error has occured, or destination already exists</returns>
bool FileUtils::CopyFile(std::filesystem::path src, std::filesystem::path dest)
{
    if (!FileExists(src) || FileExists(dest))
        return false;

    try
    {
        std::filesystem::copy(src, dest, std::filesystem::copy_options::overwrite_existing);
    }

    catch (...)
    {
        return false;
    }

    return true;
}

/// <summary>
/// Write a string to a file. If file already exists, it'll be overridden.
/// </summary>
/// <param name="path">The path to the folder where the file should be created</param>
/// <param name="filename">The filename including it's extension</param>
/// <param name="text">The content to write to the file</param>
/// <returns>True if text was successfully written, false if an error occured</returns>
bool FileUtils::WriteTextFile(std::filesystem::path path, std::string text)
{
    if (!FolderExists(path.parent_path()))
        return false;

    std::ofstream file(path.c_str());
    if (file.is_open())
    {
        file << text.c_str();
        file.close();
        return !file.fail();
    }

    else
        return false;
}

/// <summary>
/// Reads the whole contents of a text file.
/// </summary>
/// <param name="path">The path to the text file</param>
/// <returns>Returns the file contents, if file could not be read, an empty string will be returned</returns>
std::string FileUtils::ReadTextFile(std::filesystem::path path)
{
    if (!FileExists(path))
        return std::string();

    std::ifstream file(path.c_str(), std::ios::ate);
    if (file.is_open())
    {
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::string text(size, ' ');
        file.read(&text[0], size);
        file.close();

        if (file.fail())
            return std::string();

        else
            return text;
    }

    else
        return std::string();
}


/// <summary>
/// Write the contents of a bytes buffer to a file
/// </summary>
/// <param name="path">The path to the folder where the file should be saved</param>
/// <param name="filename">The filename, including its extension</param>
/// <param name="bytes">A pointer to a char array containg the byte buffer</param>
/// <param name="size">The size of the char array</param>
/// <returns>True when file could be written, false when an error has occured</returns>
bool FileUtils::WriteBinaryFile(std::filesystem::path path, char* bytes, int size)
{
    if (!FolderExists(path.parent_path()))
        return false;

    std::ofstream file(path.c_str(), std::ios::binary);
    if (file.is_open())
    {
        file.write(bytes, size);
        file.close();
        return !file.fail();
    }

    else
        return false;
}


/// <summary>
/// Reads all the contents of a binary file into a byte buffer
/// </summary>
/// <param name="path">The path to the file</param>
/// <returns>The pointer to the read byte buffer, if an error has occured a nullpointer will be returned. 
/// Don't forget to delete the buffer when you're done using it. </returns>
char* FileUtils::ReadBinaryFile(std::filesystem::path path)
{
    if (!FileExists(path))
        return nullptr;

    std::ifstream file(path.c_str(), std::ios::binary | std::ios::ate);
    if (file.is_open())
    {
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        char* buffer = new char[size];
        file.read(buffer, size);
        file.close();

        if (file.fail())
        {
            delete[] buffer;
            return nullptr;
        }

        else
            return buffer;
    }

    else
        return nullptr;
}


/// <summary>
/// Get all files inside of folder with a certain file extension
/// </summary>
/// <param name="path">The path to the folder in which to search</param>
/// <param name="extension">The file extension, including the dot</param>
/// <returns>A unsorted list of paths to the files matching the extension. List is empty if no files could be found</returns>
std::vector<std::filesystem::path> FileUtils::GetFilesByExtension(std::filesystem::path path, std::string extension)
{
    std::vector<std::filesystem::path> files;

    if (!FolderExists(path))
        return files;

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.path().extension().string() != extension)
        {
            files.push_back(entry.path());
        }
    }

    return files;
}

/// <summary>
/// Get all files inside of a directory which names contain the search string. The search string can be only a part of the full file name
/// </summary>
/// <param name="path">The path to the folder which contains the files to search</param>
/// <param name="filenameContains">The search string which should be contained in the file name</param>
/// <returns>A unsorted list of paths to the files matching the extension. List is empty if no files could be found</returns>
std::vector<std::filesystem::path> FileUtils::GetFilesByName(std::filesystem::path path, std::string filenameContains)
{
    std::vector<std::filesystem::path> files;

    if (!FolderExists(path))
        return files;

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (!std::filesystem::is_directory(entry.status()))
        {
            if (entry.path().string().find(filenameContains) != std::string::npos)
            {
                files.push_back(entry.path());
            }
        }
    }

    return files;
}

/// <summary>
/// Get all sub-folders inside of a folder which contain or match the search string
/// </summary>
/// <param name="path">The folder in which to search for the sub-folders</param>
/// <param name="foldernameContains">The search string which the desired folder names match or contain</param>
/// <returns>A unsorted list of paths to the folders matching the extension. List is empty if no folders could be found</returns>
std::vector<std::filesystem::path> FileUtils::GetFoldersByName(std::filesystem::path path, std::string foldernameContains)
{
    std::vector<std::filesystem::path> files;

    if (!FolderExists(path))
        return files;

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (std::filesystem::is_directory(entry.status()))
        {
            if (entry.path().string().find(foldernameContains) != std::string::npos)
            {
                files.push_back(entry.path());
            }
        }
    }

    return files;
}

std::vector<std::filesystem::path> FileUtils::SortPathsByNumericValue(std::vector<std::filesystem::path> paths, bool ascending)
{
    //ToDo
    throw std::runtime_error("Function not implemented");
    return std::vector<std::filesystem::path>();
}

/// <summary>
/// Gets the filename without extension from a path
/// </summary>
/// <param name="pathToFile">The path to the file</param>
/// <returns>The filename</returns>
std::string FileUtils::GetFilename(std::filesystem::path pathToFile)
{
    return pathToFile.stem().string();
}

/// <summary>
/// Get the file extension
/// </summary>
/// <param name="pathToFile">Path to the file</param>
/// <returns>The extensions, including the dot (".jpg")</returns>
std::string FileUtils::GetFileExtension(std::filesystem::path pathToFile)
{
    return pathToFile.extension().string();
}


/// <summary>
/// Get the filename including its extension
/// </summary>
/// <param name="pathToFile">Path to the file</param>
/// <returns>Filename with extension</returns>
std::string FileUtils::GetFilenameWithExtension(std::filesystem::path pathToFile)
{
    return pathToFile.filename().string();
}

/// <summary>
/// Get the name of the folder which the path points to
/// </summary>
/// <param name="pathToFolder">The path to a folder</param>
/// <returns>The name of the folder</returns>
std::string FileUtils::GetFolderName(std::filesystem::path pathToFolder)
{
    if (std::filesystem::is_directory(pathToFolder))
        return pathToFolder.filename().string();
    else
        return pathToFolder.parent_path().filename().string();
}

/// <summary>
/// Get the parent folder of the given folder or file
/// </summary>
/// <param name="path">The path to a file or folder</param>
/// <returns>The parent folder of the given folder/file</returns>
std::filesystem::path FileUtils::GetParentFolder(std::filesystem::path path)
{
    if (std::filesystem::is_directory(path))
        return path.parent_path();

    else
        return path.parent_path();
}

/// <summary>
/// If the filename contains a numeric value, the value will be returned as int.
/// The number can be anywhere in the filename, but can only be a positive integer.
/// If there are multiple distinct numbers in a file, this function will return
/// the numbers concatted (14name99,jpg -> 1499) 
/// </summary>
/// <param name="filename">The filename, can be with or without extension</param>
/// <returns>The value as a positive integer, -1 if there is no number inside of the file name</returns>
int FileUtils::GetIntFromFilename(std::string filename)
{
    std::string intAsString;

    if (filename.empty())
        return -1;

    try
    {
        intAsString = std::regex_replace(filename, std::regex("[^0-9]*([0-9]+).*"), std::string("$1"));
    }

    catch (...)
    {
        return -1;
    }

    if (intAsString.empty() || intAsString.size() < 1)
        return -1;

    return std::stoi(intAsString);
}




