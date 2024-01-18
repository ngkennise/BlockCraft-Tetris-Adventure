#  define GL_SILENCE_DEPRECATION
#  include <vector>
#  include <GL/glew.h>

#if defined(LINUX) || defined(MINGW)
#include <SDL2/SDL.h>
#else // This works for Mac
#include <SDL2/SDL.h>
#endif

#include <chrono>
#include <iostream>
#include <map>
#include <vector>

#include "Block.h"
#include "Blocks.h"
#include "GameMap.h"
using namespace std;
using namespace std::chrono;

const int   BLOCK_SIZE    = 25;
const float FALL_INTERVAL = 200;

class Game
{
public:
    vector<int>& inputs;

    Blocks       now_blocks;
    Blocks       next_blocks;

    GameMap      gamemap;

    Blocks       random_blocks(int x, int y);

    Game(vector<int>& _inputs);

    vector<vector<char>> merged_map();
    vector<GLfloat>      vertexes(vector<vector<char>>& merged_map);
    vector<GLfloat>      colors(vector<vector<char>>& merged_map);
    vector<GLfloat>      textures(vector<vector<char>>& merged_map);

    float                time_after_fall();
    void                 update();
    void                 delete_block_lines();
    void                 fall();
    void                 display_map_in_terminal(vector<vector<char>>& merged_map);
    bool                 is_collision_blocks(vector<vector<int>> indexes, int x_vec, int y_vec);
    bool                 is_collision_rotated_blocks(vector<vector<char>> rotated_map);
    void                 fix_blocks(vector<vector<int>> indexes);
    void                 next_blocks_to_now_blocks();

    bool                 checkGameOver();
    bool                 canPlaceBlockAtStartingPosition(Blocks next_blocks);
    bool                 isGameOver = false;

private:
    system_clock::time_point falling_time;
};