// Header file for Blocks

#pragma once
#  define GL_SILENCE_DEPRECATION
#  include <GL/glew.h>

#include <vector>

#include "Block.h"

enum class BlocksID { I, L, T, Z, O };

struct BlocksType {
  BlocksID             id;
  vector<int>          center;
  vector<vector<char>> blocks_map;
};

class Blocks
{
 public:
  int        x;  
  int        y;  

  BlocksType blocks_type;
  Blocks(int _x, int _y, int blocks_types_idx);

  void                 fall();
  void                 go_right();
  void                 go_left();
  void                 rotate(vector<vector<char>> rotated_map);
  
  vector<vector<char>> rotated_map(int vec);
  vector<vector<int>>  block_indexes();
};
