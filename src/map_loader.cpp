#include "map_loader.h"
#include <cstddef>
Map::Map(const std::vector<std::string> &map_data, const unsigned int width,
         const unsigned int height)
    : _width(width), _height(height) {
  _matrix = new MAP_CODE *[_height];
  for (int i = 0; i < _height; i++) {
    _matrix[i] = new MAP_CODE[_width];
  }
}

Map::~Map() {
  if (_matrix == nullptr) {
    return;
  }
  for (size_t i = 0; i < _height; i++) {
    if (_matrix != nullptr)
      delete[] _matrix[i];
  }
  delete _matrix;
}

Map LoadMap(const std::string &path) {
  std::ifstream file;
  file.open(path);

  std::vector<std::string> map_data = {};
  std::string temp;
  unsigned int width = 0, height = 0;
  while (std::getline(file, temp)) {
    if (temp.size() > width)
      width = temp.size();
    map_data.push_back(temp);
    height++;
  }
}
