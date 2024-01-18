#  define GL_SILENCE_DEPRECATION

#  include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>
#include <vector>

#include "../include/Game.h"
#include "../include/shader.h"

using namespace std;

vector<int> inputs(512, 0);
int loadTexture(const char* FilePath);
bool isWireFrame = false;
GLenum gPolygonMode = GL_LINE;

SDL_Renderer* renderer;
int attempt = 0;
SDL_Texture* welcomeTexture;
SDL_Rect welcomeRect;
SDL_Event e;
bool quit = false;

int loadTexture(const char* FilePath){

    SDL_Surface* image = IMG_Load(FilePath);
    if (image == NULL){
        std::cout<<("Unable to load texture?: Error: %s", SDL_GetError());
        return -1;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    GLenum format;
    if (image->format->BytesPerPixel == 4) {
        format = GL_RGBA;
    } else {
        format = GL_RGB;
    }

    //Texture info passed from sdl surface
    glTexImage2D(GL_TEXTURE_2D,
                 0,              // Mipmap Level
                 format,         // Internal Format
                 image->w,
                 image->h,
                 0,              // Texture Border
                 format,         // Format
                 GL_UNSIGNED_BYTE,
                 image->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //Clear memory
    SDL_FreeSurface(image);
    return texture;//Returns the ID in opengl
}

void WelcomeScreen(){
    // std::cout << "Welcome Screen Started" << std::endl;
    bool gameStarted = false;
    while (!quit && !gameStarted) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.scancode) {
                    case SDL_SCANCODE_S:
                        // std::cout << "Starting game from welcome screen" << std::endl;
                        gameStarted = true;
                        break;
                    case SDL_SCANCODE_ESCAPE:
                    case SDL_SCANCODE_Q:
                        // std::cout << "Quitting from welcome screen" << std::endl;
                        quit = true;
                        break;
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, welcomeTexture, NULL, &welcomeRect);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(welcomeTexture);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    // std::cout << "Exiting Welcome Screen" << std::endl;
}

int main(int argc, char *argv[]) {
    while (!quit){

        SDL_Window *window = NULL;
        GLuint vao;
        GLuint vertex_vbo;
        GLuint color_vbo;
        GLuint texture_vbo;
        Game game(inputs);
        GLfloat screen_w = game.gamemap.screen_w();
        GLfloat screen_h = game.gamemap.screen_h();
        SDL_GLContext m_openGLContext;

        // const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
        SDL_Event e;

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
            exit(EXIT_FAILURE);
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        // We want to request a double buffer for smooth updating.
        // SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        //Create window
        window = SDL_CreateWindow("Tetris",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  screen_w,
                                  screen_h,
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        // Check if Window did not create.
        if (window == NULL) {
            std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
            exit(EXIT_FAILURE);
        }

        // Create context and check if it got created
        m_openGLContext = SDL_GL_CreateContext(window);
        if (m_openGLContext == NULL) {
            std::cerr << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
            exit(EXIT_FAILURE);
        }

        SDL_GL_MakeCurrent(window, m_openGLContext);
        SDL_GL_SetSwapInterval(1);

        // GLEW init 
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            cerr << "Can't initialize GLEW" << endl;
            return 1;
        }

        // SDL_image init
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            std::cerr << "SDL_image could not initialize! SDL_image Error" << std::endl;
            exit(EXIT_FAILURE);
        }

        // Create Renderer and check
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << "\n";
            exit(EXIT_FAILURE);
        }

        // Files to display based on attempt number
        const char* chosenFile;
        const char* filePath = "media/welcome.png";
        const char* filePath2 = "media/restart.png";
        if (attempt == 0){
            chosenFile = filePath;}
        else{
            chosenFile = filePath2;
        }

        // Surface for start screen
        SDL_Surface* welcomeSurface = IMG_Load(chosenFile);
        if (!welcomeSurface) {
            std::cerr << "Unable to load image! SDL_image Error" << std::endl;
            exit(EXIT_FAILURE);
        }
        
        // Creating Texture from Surface
        welcomeTexture = SDL_CreateTextureFromSurface(renderer, welcomeSurface);
        if (!welcomeTexture) {
            std::cerr << "Unable to create texture from surface! SDL Error: " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(welcomeSurface);

        // Dimensions of screen
        welcomeRect.x = 0;
        welcomeRect.y = 0;
        welcomeRect.w = screen_w;
        welcomeRect.h = screen_h;

        WelcomeScreen();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        GLuint blockTexture = loadTexture("include/blocks.png"); //loading texture file
        GLint shader = makeShader();

        // VAO and VBO's 
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vertex_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);

        glGenBuffers(1, &color_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, color_vbo);

        glGenBuffers(1, &texture_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);

        // Game Loop
        while (!quit && !game.checkGameOver()) {
            // Poll Inputs
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                        case SDL_SCANCODE_Q:
                            quit = true;
                            break;
                        case SDL_SCANCODE_LEFT:
                            inputs[SDL_SCANCODE_LEFT]++;
                            break;
                        case SDL_SCANCODE_RIGHT:
                            inputs[SDL_SCANCODE_RIGHT]++;
                            break;
                        case SDL_SCANCODE_DOWN:
                            inputs[SDL_SCANCODE_DOWN]++;
                            break;
                        case SDL_SCANCODE_R:
                            inputs[SDL_SCANCODE_R]++;
                            break;
                        case SDL_SCANCODE_E:
                            inputs[SDL_SCANCODE_E]++;
                            break;
                        case SDL_SCANCODE_W:
                            isWireFrame = !isWireFrame;
                            if (isWireFrame) {
                                gPolygonMode = GL_LINE;
                            } else {
                                gPolygonMode = GL_FILL;
                            }
                            break;
                    }
                }
            }

            // Game Updation

            game.update();

            glClearColor(0.69f,0.9f,0.93f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            vector<vector<char>> merged_map = game.merged_map();
            vector<GLfloat> vertexes = game.vertexes(merged_map);
            vector<GLfloat> colors = game.colors(merged_map);
            vector<GLfloat> textures = game.textures(merged_map);
            game.display_map_in_terminal(merged_map);

            glUseProgram(shader);
            glUniform1i(glGetUniformLocation(shader, "blockTextures"), 0); // setting the texture uniform
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, blockTexture); // binding the texture

            glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
            glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(GLfloat), vertexes.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
            glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
            glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(GLfloat), textures.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(2);

            // Wireframe mode
            if (isWireFrame) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            for (int i = 0; i < vertexes.size() - 4; i += 4) {
//            cout << "Block " << i / 4 << " Position: (" << vertexes[i] << ", " << vertexes[i + 1] << ")" << endl;
//            cout << "Block " << i / 4 << " Color: (" << colors[i] << ", " << colors[i + 1] << ", " << colors[i + 2] << ")" << endl;
//            cout << "Block " << i / 4 << " Texture Coord: (" << textures[i + 2] << ", " << textures[i + 3] << ")" << endl;

                glDrawArrays(GL_TRIANGLE_FAN, i, 4);
            }

            SDL_GL_SwapWindow(window);
        }

        //Clean Up

        glDeleteBuffers(1, &vertex_vbo);
        glDeleteBuffers(1, &color_vbo);
        glDeleteBuffers(1, &texture_vbo);

        SDL_GL_DeleteContext(m_openGLContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        attempt += 1;
    }
    return 0;
}
