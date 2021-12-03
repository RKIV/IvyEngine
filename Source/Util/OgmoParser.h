#pragma once
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <Engine/Leaf/Leaf.h>
#include <Engine/Leaf/LSphere.h>

using json = nlohmann::json;

class OgmoParser
{
public:
	struct OgmoParserConfig
	{
		// Maps from layer to a vector or leaf CDOs representing the tiles to be generated
		std::unordered_map<std::string, std::vector<LeafClass>> LayerTileToLeafMap;
	};
public:
	static Leaf* ParseOgmoLevel(const char* LevelPath, OgmoParserConfig* LeafMap)
	{
		json j_level;

		std::ifstream f_level(LevelPath);
		f_level >> j_level;
		f_level.close();

		Leaf* LevelRoot = new LSphere();
		for (auto& layer : j_level["layers"])
		{
			std::string s = layer["name"];
			Leaf* LayerRoot = LevelRoot->AddSubLeaf<Leaf>(s.c_str());
			int height = layer["gridCellsY"];
			int width = layer["gridCellsX"];
			
			int top_left_x = -width;
			int top_left_y = height;

			if (LeafMap->LayerTileToLeafMap.contains(s))
			{
				for (int r = 0; r < height; ++r)
				{
					for (int c = 0; c < width; ++c)
					{
						int idx = layer["data2D"][r][c];
						if (idx != -1 && idx < LeafMap->LayerTileToLeafMap[s].size())
						{
							char idx_s[10];
							sprintf_s(idx_s, 10, "Tile%d-%d", r, c);
							Leaf* Tile = LayerRoot->AddSubLeaf(LeafMap->LayerTileToLeafMap[s][idx], idx_s);

							Tile->position.x = (float)(top_left_x + c*2 + 1);
							Tile->position.y = (float)(top_left_y - r*2 - 1);
						}
					}
				}
			}
		}

		return LevelRoot;
	}
};