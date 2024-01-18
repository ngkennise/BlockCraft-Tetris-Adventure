// Header file for Gamemap

#pragma once
#  define GL_SILENCE_DEPRECATION
#  include <GL/glew.h>

#include <iostream>
#include <string>
#include <vector>

#include "Block.h"

const int BOTTOM_ROWS   = 1;
const int GAMEOVER_LINE = 4;

class GameMap
{
 public:
  int                  w, h;
  int                  block_size;

  vector<vector<char>> game_map;
  vector<GLfloat>      vertexes;
  vector<GLfloat>      colors;
  vector<GLfloat>      textures;

  void                 init_game_map();
  int                  screen_w();
  int                  screen_h();
  int                  map_w();
  int                  map_h();

  vector<GLfloat>      position(int x_idx, int y_idx);
  vector<GLfloat>      texCoords(int blockType);
  Block                block_from_index_and_char(int x, int y, char c);
  vector<GLfloat>      vertexes_of_map(vector<vector<char>>& m);
  vector<GLfloat>      colors_of_map(vector<vector<char>>& m);
  vector<GLfloat>      texCoords_of_map(vector<vector<char>>& m);

  bool                 is_collision(int x_idx, int y_idx, int x_vec, int y_vec);
  vector<int>          deletable_rows();
  void                 delete_rows(vector<int> deletable_rows);
  
  GameMap(int block_size);
};
