#pragma once
#include <vector>

class DataSource;
class DataAsset;

class DataManager
{
	struct Entry
	{
		DataAsset* asset;
		DataSource* source;
	};

	std::vector<Entry> m_entries;

public:
	static DataManager& Get();

	void Register(DataAsset* dataAsset, DataSource* dataSource);

	void Update();
};

