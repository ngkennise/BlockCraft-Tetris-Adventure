// C++ Standard Template Library (STL)
#include <iostream>
#include <map>
#include <random>
#include <vector>

// Importing header files
#include "Block.h"
#include "Blocks.h"
#include "Game.h"

// Constructor
Game::Game(vector<int>& _inputs)
    : inputs(_inputs)
    , now_blocks(random_blocks(6, 0))
    , next_blocks(random_blocks(0, 0))
    , gamemap(GameMap(BLOCK_SIZE))
    , falling_time(system_clock::now())
{}

// Method to check if game is over
bool Game::checkGameOver() {
    return isGameOver;
}

// Update time to make the block fall periodically
float Game::time_after_fall()
{
  system_clock::time_point now = system_clock::now();
  return duration_cast<milliseconds>(now - falling_time).count();
}

// Update the game 
// This includes block fall periodically and user inputs
// User inputs include right, left, rotate and fall
void Game::update()
{
  if (inputs[SDL_SCANCODE_LEFT] > 0 && inputs[SDL_SCANCODE_RIGHT] > 0) {
    inputs[SDL_SCANCODE_LEFT] = 0;
    inputs[SDL_SCANCODE_RIGHT] = 0;
  } else if (inputs[SDL_SCANCODE_LEFT] > 0) {
    inputs[SDL_SCANCODE_LEFT]          = 0;
    vector<vector<int>> indexes = now_blocks.block_indexes();
    if (! is_collision_blocks(indexes, -1, 0)) {
      now_blocks.go_left();
    }
  } else if (inputs[SDL_SCANCODE_RIGHT] > 0) {
    inputs[SDL_SCANCODE_RIGHT]          = 0;
    vector<vector<int>> indexes = now_blocks.block_indexes();
    if (! is_collision_blocks(indexes, 1, 0)) {
      now_blocks.go_right();
    }
  }
  if (time_after_fall() < FALL_INTERVAL) {
    if (inputs[SDL_SCANCODE_DOWN] > 0) {
      inputs[SDL_SCANCODE_DOWN]          = 0;
      vector<vector<int>> indexes = now_blocks.block_indexes();
      if (! is_collision_blocks(indexes, 0, 1)) {
        now_blocks.fall();
      }
    }
  } else {
    fall();
    return;
  }
  // Determine clockwise or anticlockwise

  if (inputs[SDL_SCANCODE_R] > 0 && inputs[SDL_SCANCODE_E] > 0) {
    inputs[SDL_SCANCODE_R] = 0;
    inputs[SDL_SCANCODE_E] = 0;
  } else if (inputs[SDL_SCANCODE_R] > 0 || inputs[SDL_SCANCODE_E] > 0) {
    int                  vec         = inputs[SDL_SCANCODE_R] > 0 ? -1 : 1;
    inputs[SDL_SCANCODE_R]               = 0;
    inputs[SDL_SCANCODE_E]               = 0;
    vector<vector<char>> rotated_map = now_blocks.rotated_map(vec);
    if (! is_collision_rotated_blocks(rotated_map)) {
      now_blocks.rotate(rotated_map);
    }
  }
}

// Method to delete the line when it is filled
void Game::delete_block_lines()
{
  vector<int> deletable_rows = gamemap.deletable_rows();
  int         row_num        = deletable_rows.size();
  if (row_num == 0) return;
  // cout << row_num << "deleted!" << endl;
  gamemap.delete_rows(deletable_rows);
}

// Method to make the block fall, place it and check for collision and deletion
void Game::fall()
{
  inputs[SDL_SCANCODE_DOWN]          = 0;
  vector<vector<int>> indexes = now_blocks.block_indexes();
  if (is_collision_blocks(indexes, 0, 1)) {
    fix_blocks(indexes);
    next_blocks_to_now_blocks();
    delete_block_lines();
  } else {
    now_blocks.fall();
  }
  falling_time = system_clock::now();
}

// Method to check and stop block if collision occurs when rotated
bool Game::is_collision_rotated_blocks(vector<vector<char>> rotated_map)
{
  for (int y_idx = 0; y_idx < rotated_map.size(); y_idx++) {
    for (int x_idx = 0; x_idx < rotated_map[0].size(); x_idx++) {
      if (rotated_map[y_idx][x_idx] != '.') {
        if (gamemap.is_collision(x_idx + now_blocks.x, y_idx + now_blocks.y, 0, 0)) return true;
      }
    }
  }
  return false;
}

// Method to check and stop block if collision occurs
bool Game::is_collision_blocks(vector<vector<int>> indexes, int x_vec, int y_vec)
{
  for (vector<int> xy : indexes) {
    if (gamemap.is_collision(xy[0], xy[1], x_vec, y_vec)) return true;
  }
  return false;
}

// Method to place the blocks
void Game::fix_blocks(vector<vector<int>> indexes)
{
  for (int i = 0; i < indexes.size(); i++) {
    int  x_idx = indexes[i][0];
    int  y_idx = indexes[i][1];
    char c     = now_blocks.blocks_type.blocks_map[y_idx - now_blocks.y][x_idx - now_blocks.x];
    gamemap.game_map[y_idx][x_idx] = c;
  }
}

// Method to covert the next block (predicted block) to now block(current block)
// and generate a new next block 
void Game::next_blocks_to_now_blocks()
{

  next_blocks.x = 6;
  now_blocks    = next_blocks;
  next_blocks   = random_blocks(0, 0);

  if (!canPlaceBlockAtStartingPosition(now_blocks)) {
    isGameOver = true;
    return;
  }
}

// Method to check if block can be placed at start position 
// i.e checks if it is filled to the top to end the game
bool Game::canPlaceBlockAtStartingPosition(Blocks block) {
    vector<vector<int>> indexes = block.block_indexes();

    for (const auto& index : indexes) {
        int x = index[0];
        int y = index[1];
        if (x >= gamemap.map_w() || y < 0 || y >= gamemap.map_h()-1) {
            return false; 
        }
        if (gamemap.game_map[y][x] != '.') {
            return false;
        }
    }

    // All positions are free, block can be placed
    return true;
}

// Random block generation from the 5 blocks that have been created
Blocks Game::random_blocks(int x, int y)
{
  random_device rnd;
  return Blocks(x, y, rnd() % 5);
}

// Method to update blocks in the map
vector<vector<char>> Game::merged_map()
{
  vector<vector<char>>  tmp_map         = gamemap.game_map;
  vector<vector<char>>& now_blocks_map  = now_blocks.blocks_type.blocks_map;
  vector<vector<char>>& next_blocks_map = next_blocks.blocks_type.blocks_map;
  int                   x, y;
  for (y = 0; y < now_blocks_map.size(); y++) {
    for (x = 0; x < now_blocks_map[0].size(); x++) {
      if (now_blocks_map[y][x] != '.') {
        tmp_map[y + now_blocks.y][x + now_blocks.x] = now_blocks_map[y][x];
      }
    }
  }
  for (y = 0; y < next_blocks_map.size(); y++) {
    for (x = 0; x < next_blocks_map[0].size(); x++) {
      if (next_blocks_map[y][x] != '.') {
        tmp_map[y + next_blocks.y][x + next_blocks.x] = next_blocks_map[y][x];
      }
    }
  }
  return tmp_map;
}

// Method to get the vertices
vector<GLfloat> Game::vertexes(vector<vector<char>>& merged_map)
{
  return gamemap.vertexes_of_map(merged_map);
}

// Method to get the colors
vector<GLfloat> Game::colors(vector<vector<char>>& merged_map)
{
  return gamemap.colors_of_map(merged_map);
}

vector<GLfloat> Game::textures(vector<vector<char>>& merged_map)
{
    vector<GLfloat> textureCoords;
    int numBlockTypes = 5; // for 5 different block types

    for (int i = 0; i < merged_map.size(); ++i) {
        for (int j = 0; j < merged_map[i].size(); ++j) {
            char blockType = merged_map[i][j];

            // checking to see if the cell empty
            if (blockType != '.') {
                int typeIndex = blockType - '1';

                float blockTextureWidth = 1.0f / numBlockTypes;

                float uStart = typeIndex * blockTextureWidth;
                float uEnd = uStart + blockTextureWidth;

                float vStart = 0.0f;
                float vEnd = 1.0f;

                // Bottom-left vertex
                textureCoords.push_back(uStart);
                textureCoords.push_back(vEnd);

                // Bottom-right vertex
                textureCoords.push_back(uEnd);
                textureCoords.push_back(vEnd);

                // Top-right vertex
                textureCoords.push_back(uEnd);
                textureCoords.push_back(vStart);

                // Top-left vertex
                textureCoords.push_back(uStart);
                textureCoords.push_back(vStart);
            }
        }
    }
    return textureCoords;
}

void Game::display_map_in_terminal(vector<vector<char>>& merged_map)
{
    for (int y = 0; y < merged_map.size(); y++) {
        for (int x = 0; x < merged_map[0].size(); x++) {
            cout << merged_map[y][x] << " ";
        }
        cout << endl;
    }
    cout << endl;
}
