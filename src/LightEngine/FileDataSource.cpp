#include "FileDataSource.h"
#include "DataAsset.h"

using namespace std::filesystem;

FileDataSource::FileDataSource(const std::string& path) : m_path(path)
{
	if (exists(m_path))
	{
		m_lastWriteTime = last_write_time(m_path);
	}
}

bool FileDataSource::HasChanged()
{
	if (exists(m_path) == false)
		return false;

	file_time_type currentTime = last_write_time(m_path);
	
	if (currentTime == m_lastWriteTime)
		return false;

	m_lastWriteTime = currentTime;
	return true;
}

bool FileDataSource::Load(DataAsset* asset)
{
	if (asset == nullptr)
		return false;

	asset->Reload();
	return true;
}
