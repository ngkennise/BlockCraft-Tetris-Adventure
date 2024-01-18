// C++ Standard Template Library (STL)
#include <iostream>
#include <vector>

// Importing header files
#include "Block.h"

// Block types and colors
vector<BlockType> block_types {
  BlockType {  BlockID::HARD, vector<GLfloat> { 0.3f, 0.3f, 0.3f }, false},
  BlockType {   BlockID::RED, vector<GLfloat> { 1.0f, 0.2f, 0.07f },  true},
  BlockType { BlockID::GREEN, vector<GLfloat> { 0.45f, 0.8f, 0.23f },  true},
  BlockType {  BlockID::BLUE, vector<GLfloat> { 0.0f, 0.25f, 0.68f },  true},
  BlockType {BlockID::ORANGE, vector<GLfloat> { 0.99f, 0.5f, 0.12f },  true},
  BlockType {BlockID::YELLOW, vector<GLfloat> { 1.0f, 0.83f, 0.0f },  true},
};

// Constructor
Block::Block(GLfloat _w, GLfloat _h, vector<GLfloat> _posi, int block_types_idx)
    : w(_w), h(_h), block_type(block_types[block_types_idx]), position(_posi)
{
  GLfloat x = position[0];
  GLfloat y = position[1];
  positions = { x, y, 0.0f, x + w, y, 0.0f, x + w, y - h, 0.0f, x, y - h, 0.0f };
  GLfloat r = block_type.color[0];
  GLfloat g = block_type.color[1];
  GLfloat b = block_type.color[2];
  colors    = {
    r, g, b, r, g, b, r, g, b, r, g, b,
  };
}
