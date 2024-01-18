#include <iostream>
#include <string>
#include <vector>

#include "Block.h"
#include "GameMap.h"

using namespace std;

const vector<vector<char>> DEFAULT_GAME_MAP = {
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.'},
        {'#', '#', '#', '#', '#', '.', '.', '.', '.', '.', '.', '#', '#', '#', '#', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
};

int GameMap::screen_w()
{
    return block_size * w;
}

int GameMap::screen_h()
{
    return block_size * h;
}

int GameMap::map_w()
{
    return game_map[0].size();
}
int GameMap::map_h()
{
    return game_map.size();
}

vector<GLfloat> GameMap::position(int x_idx, int y_idx)
{
    GLfloat x = -1.0f + 2.0f / w * x_idx;
    GLfloat y = 1.0f - 2.0f / h * y_idx;
    return vector<GLfloat> { x, y };
}

vector<GLfloat> GameMap::texCoords(int blockType) {

    return vector<GLfloat> {
            0.0f, 0.0f, // Bottom-left
            1.0f, 0.0f, // Bottom-right
            1.0f, 1.0f, // Top-right
            0.0f, 1.0f  // Top-left
    };
}

void GameMap::init_game_map()
{
    game_map = DEFAULT_GAME_MAP;
}

GameMap::GameMap(int _block_size)
        : w(DEFAULT_GAME_MAP[0].size()), h(DEFAULT_GAME_MAP.size()), block_size(_block_size)
{
    init_game_map();
}

Block GameMap::block_from_index_and_char(int x, int y, char c)
{
    GLfloat w_rate = 2.0f / w;
    GLfloat h_rate = 2.0f / h;
    switch (c) {
        case '#':
        case '0': {
            Block block(w_rate, h_rate, position(x, y), 0);
            return block;
        }
        case '1': {
            Block block(w_rate, h_rate, position(x, y), 1);
            return block;
        }
        case '2': {
            Block block(w_rate, h_rate, position(x, y), 2);
            return block;
        }
        case '3': {
            Block block(w_rate, h_rate, position(x, y), 3);
            return block;
        }
        case '4': {
            Block block(w_rate, h_rate, position(x, y), 4);
            return block;
        }
        case '5': {
            Block block(w_rate, h_rate, position(x, y), 5);
            return block;
        }
        default: {
            Block block(w_rate, h_rate, position(-100, -100), 0);
            return block;
        }
    }
}

vector<GLfloat> GameMap::vertexes_of_map(vector<vector<char>>& m)
{
    vector<GLfloat> vertexes;
    for (int y = 0; y < m.size(); y++) {
        for (int x = 0; x < m[0].size(); x++) {
            if (m[y][x] != '.') {
                Block b = block_from_index_and_char(x, y, m[y][x]);
                vertexes.insert(vertexes.end(), b.positions.begin(), b.positions.end());
            }
        }
    }
    return vertexes;
}

vector<GLfloat> GameMap::colors_of_map(vector<vector<char>>& m)
{
    vector<GLfloat> colors;
    for (int y = 0; y < m.size(); y++) {
        for (int x = 0; x < m[0].size(); x++) {
            if (m[y][x] != '.') {
                Block b = block_from_index_and_char(x, y, m[y][x]);
                colors.insert(colors.end(), b.colors.begin(), b.colors.end());
            }
        }
    }
    return colors;
}

vector<GLfloat> GameMap::texCoords_of_map(vector<vector<char>>& m) {
    vector<GLfloat> texCoords;
    for (int y = 0; y < m.size(); y++) {
        for (int x = 0; x < m[0].size(); x++) {
            if (m[y][x] != '.') {
                // Bottom-left vertex
                texCoords.push_back(0.0f);
                texCoords.push_back(0.0f);

                // Bottom-right vertex
                texCoords.push_back(1.0f);
                texCoords.push_back(0.0f);

                // Top-right vertex
                texCoords.push_back(1.0f);
                texCoords.push_back(1.0f);

                // Top-left vertex
                texCoords.push_back(0.0f);
                texCoords.push_back(1.0f);
            }
        }
    }
    return texCoords;
}

bool GameMap::is_collision(int x_idx, int y_idx, int x_vec, int y_vec)
{
    x_idx += x_vec;
    y_idx += y_vec;
    if (game_map[y_idx][x_idx] == '.') return false;
    return true;
}

vector<int> GameMap::deletable_rows()
{
    vector<int> _deletable_rows;
    for (int y_idx = 0; y_idx < h - BOTTOM_ROWS; y_idx++) {
        bool deletable = true;
        for (int x_idx = 0; x_idx < w; x_idx++) {
            if (game_map[y_idx][x_idx] == '.') {
                deletable = false;
            }
        }
        if (deletable) {
            _deletable_rows.push_back(y_idx);
        }
    }
    return _deletable_rows;
}

void GameMap::delete_rows(vector<int> deletable_rows)
{
    vector<vector<char>> tmp_map = game_map;
    for (int idx : deletable_rows) {
        tmp_map.erase(tmp_map.begin() + idx);
        tmp_map.insert(
                tmp_map.begin() + GAMEOVER_LINE + 1, DEFAULT_GAME_MAP.begin() + GAMEOVER_LINE + 1,
                DEFAULT_GAME_MAP.begin() + GAMEOVER_LINE + 2);
    }
    game_map = tmp_map;
}