#include "DataManager.h"
#include "DataSource.h"
#include "DataAsset.h"

DataManager& DataManager::Get()
{
	static DataManager Instance;
	return Instance;
}

void DataManager::Register(DataAsset* dataAsset, DataSource* dataSource)
{
	m_entries.push_back({ dataAsset, dataSource });
}

void DataManager::Update()
{
	for (Entry& entry : m_entries)
	{
		if (entry.source->HasChanged())
		{
			entry.source->Load(entry.asset);
		}
	}
}
