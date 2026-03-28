#pragma once

class DataAsset;

class DataSource
{
public:
	virtual bool HasChanged() = 0;
	virtual bool Load(DataAsset* dataAsset) = 0;
};

