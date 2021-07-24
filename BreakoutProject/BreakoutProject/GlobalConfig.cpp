#include "GlobalConfig.h"
#include "Windows.h"

std::string GlobalConfig::GetPathToData()
{
	const std::string assetFolder = "\\Assets\\";
	CHAR szFileName[MAX_PATH];
	GetModuleFileNameA(NULL, szFileName, MAX_PATH);
	std::string path = szFileName;
	path = path.substr(0, path.rfind("\\", std::string::npos));
	DWORD attributes = GetFileAttributesA(std::string(path + assetFolder).c_str());
	
	if (attributes != INVALID_FILE_ATTRIBUTES && attributes & FILE_ATTRIBUTE_DIRECTORY)
		return path + assetFolder;

	path = path.substr(0, path.rfind("\\", std::string::npos));
	path = path.substr(0, path.rfind("\\", std::string::npos));
	attributes = GetFileAttributesA(std::string(path + assetFolder).c_str());

	if (attributes != INVALID_FILE_ATTRIBUTES && attributes & FILE_ATTRIBUTE_DIRECTORY)
		return path + assetFolder;
	
	return path;
}
