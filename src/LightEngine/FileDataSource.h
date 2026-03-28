#pragma once
#include "DataSource.h"
#include <string>
#include <filesystem>

class FileDataSource : public DataSource
{
	std::string m_path;
	std::filesystem::file_time_type m_lastWriteTime;

public:
	FileDataSource(const std::string& path);

	bool HasChanged() override;
	bool Load(DataAsset* asset) override;

	std::string GetPath() { return m_path; }
};

