#include "StringHelper.h"

std::wstring StringHelper::StringToWide(std::string str)
{
	return std::wstring(str.begin(), str.end());
}

std::string StringHelper::GetDirectoryFromPath(const std::string& filePath)
{
	size_t off1 = filePath.find_last_of('\\');
	size_t off2 = filePath.find_last_of('/');
	if (off1 == std::string::npos && off2 == std::string::npos)
		return "";

	if (off1 == std::string::npos)
		return filePath.substr(0, off2);

	if (off2 == std::string::npos)
		return filePath.substr(0, off1);

	// If both exists, no need to use the greater offset
	return filePath.substr(0, (off1 > off2 ? off1 : off2));
}

std::string StringHelper::GetFileExtension(const std::string& fileName)
{
	size_t off = fileName.find_last_of('.');
	if (off == std::string::npos)
		return{};

	return std::string(fileName.substr(off + 1));
}
