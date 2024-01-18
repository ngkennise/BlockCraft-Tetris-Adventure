// C++ Standard Template Library (STL)
#include <iostream>
#include <vector>

// Importing header files
#include "Block.h"
#include "Blocks.h"

// Block types (geometry)
vector<BlocksType> blocks_types {
  BlocksType {BlocksID::I, vector<int> { 2, 2 },
              vector<vector<char>> {
              { '.', '.', '.', '.', '.' },
              { '.', '.', '.', '.', '.' },
              { '1', '1', '1', '1', '.' },
              { '.', '.', '.', '.', '.' },
              { '.', '.', '.', '.', '.' },
              }},
  BlocksType {BlocksID::L, vector<int> { 1, 1 },
              vector<vector<char>> {
              { '.', '2', '.', '.' },
              { '.', '2', '.', '.' },
              { '.', '2', '2', '.' },
              { '.', '.', '.', '.' },
              }},
  BlocksType {BlocksID::T, vector<int> { 1, 1 },
              vector<vector<char>> {
              { '.', '3', '.', '.' },
              { '.', '3', '3', '.' },
              { '.', '3', '.', '.' },
              { '.', '.', '.', '.' },
              }},
  BlocksType {BlocksID::Z, vector<int> { 1, 1 },
              vector<vector<char>> {
              { '.', '4', '.', '.' },
              { '.', '4', '4', '.' },
              { '.', '.', '4', '.' },
              { '.', '.', '.', '.' },
              }},
  BlocksType {BlocksID::O, vector<int> { 1, 1 },
              vector<vector<char>> {
              { '.', '5', '5', '.' },
              { '.', '5', '5', '.' },
              { '.', '.', '.', '.' },
              { '.', '.', '.', '.' },
              }},
};

// Constructor
Blocks::Blocks(int _x, int _y, int blocks_types_idx)
    : x(_x), y(_y), blocks_type(blocks_types[blocks_types_idx])
{}

// Methods to modify the position of the block

void Blocks::fall()
{
  y++;
}

void Blocks::go_left()
{
  x--;
}

void Blocks::go_right()
{
  x++;
}

void Blocks::rotate(vector<vector<char>> rotated_map)
{
  blocks_type.blocks_map = rotated_map;
}

// Method to calculate and update the index after rotation
vector<vector<char>> Blocks::rotated_map(int vec)
{
  vector<int>           center = blocks_type.center;
  vector<vector<char>>& m      = blocks_type.blocks_map;
  int                   h      = m.size();
  int                   w      = m[0].size();
  vector<vector<char>>  tmp_map(h, vector<char>(w, '.'));
  for (int y_idx = 0; y_idx < h; y_idx++) {
    for (int x_idx = 0; x_idx < w; x_idx++) {
      if (m[y_idx][x_idx] != '.') {
        int _x, _y, _x2, _y2;
        _y = y_idx - center[1];  
        _x = x_idx - center[0];  
        if (vec > 0) {
          _y2 = -_x;
          _x2 = _y;
        } else {
          _y2 = _x;
          _x2 = -_y;
        }
        tmp_map[_y2 + center[1]][_x2 + center[0]] = m[y_idx][x_idx];
      }
    }
  }
  return tmp_map;
}

// Method to update block index after any operation or periodic fall
vector<vector<int>> Blocks::block_indexes()
{
  vector<vector<char>>& m = blocks_type.blocks_map;
  int                   h = m.size();
  int                   w = m[0].size();
  vector<vector<int>>   indexes;
  for (int y_idx = 0; y_idx < h; y_idx++) {
    for (int x_idx = 0; x_idx < w; x_idx++) {
      if (m[y_idx][x_idx] != '.') {
        indexes.push_back(vector<int> { x_idx + x, y_idx + y });
      }
    }
  }
  return indexes;
}
