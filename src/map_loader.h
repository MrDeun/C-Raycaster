#ifndef MAP_LOADER
#define MAP_LOADER

#include <cstddef>
#include <fstream>
#include <string>
#include <vector>

#include "Option.h"

enum MAP_CODE {
  FLOOR = 0,
  BLUE_WALL = 1,
};

class Map {
private:
  const unsigned int _width;
  const unsigned int _height;
  MAP_CODE **_matrix;

public:
  ~Map();
  MAP_CODE* getWallCode(int x, int y) {
    if (x >= _width || y >= _height) {
      return nullptr;
    }
    return new MAP_CODE(_matrix[y][x]);
  }
  Map():_width(0),_height(0),_matrix(nullptr){}
  Map(const std::vector<std::string>& map_data, const unsigned int width,
      const unsigned int height);
  Map(const Map& _map) = default;
};

Map LoadMap(std::string path);

#endif