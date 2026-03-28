#include "TiledUtils.h"
#include <filesystem>

std::string TiledUtils::ResolveTiledPath(const std::string& tmxPath, const std::string& relative)
{
    std::filesystem::path mapPath = std::filesystem::path(tmxPath).parent_path();
    std::filesystem::path full = mapPath / relative;

    full = std::filesystem::weakly_canonical(full);

    return full.string();
}

std::string TiledUtils::ResolveTileGID(const std::string& tmxFilePath, int gid)
{
    XMLDocument doc;
    if (doc.LoadFile(tmxFilePath.c_str()) != XML_SUCCESS)
        return "";

    XMLElement* map = doc.FirstChildElement("map");
    if (!map) return "";

    std::vector<TilesetInfo> tilesets;
    for (XMLElement* ts = map->FirstChildElement("tileset"); ts; ts = ts->NextSiblingElement("tileset"))
    {
        const char* source = ts->Attribute("source");
        if (!source) continue;
        int firstgid = ts->IntAttribute("firstgid");
        tilesets.push_back({ firstgid, source });
    }

    TilesetInfo* tsFound = nullptr;
    for (size_t i = 0; i < tilesets.size(); i++)
    {
        int nextFirstGid = (i + 1 < tilesets.size()) ? tilesets[i + 1].firstgid : INT_MAX;
        if (gid >= tilesets[i].firstgid && gid < nextFirstGid)
        {
            tsFound = &tilesets[i];
            break;
        }
    }
    if (!tsFound) return "";

    std::filesystem::path tsxPath = std::filesystem::path(tmxFilePath).parent_path() / tsFound->source;
    XMLDocument tsxDoc;
    if (tsxDoc.LoadFile(tsxPath.string().c_str()) != XML_SUCCESS)
        return "";

    int localId = gid - tsFound->firstgid;
    XMLElement* tsx = tsxDoc.FirstChildElement("tileset");
    if (!tsx) return "";

    for (XMLElement* tile = tsx->FirstChildElement("tile"); tile; tile = tile->NextSiblingElement("tile"))
    {
        int id = tile->IntAttribute("id");
        if (id == localId)
        {
            XMLElement* image = tile->FirstChildElement("image");
            if (image)
            {
                const char* imgSrc = image->Attribute("source");
                if (imgSrc)
                    return (tsxPath.parent_path() / imgSrc).string();
            }
        }
    }

    return "";
}