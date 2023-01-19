PROJECT(tetris)
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

ADD_EXECUTABLE(tetris main.cpp
                      AppController.cpp
                      Screen.cpp
                      MenuScreen.cpp
                      Painter.cpp
                      Text.cpp
                      Field.cpp
                      Piece.cpp
                      Singer.cpp
                      Player.cpp)

INCLUDE_DIRECTORIES(SDL2.framework/Headers
                    SDL2_ttf.framework/Headers)

INCLUDE_DIRECTORIES(/opt/homebrew/Cellar/sdl2/2.26.1/include/SDL2
                    /opt/homebrew/Cellar/sdl2_ttf/2.20.1/include/SDL2)

FIND_LIBRARY(SDL2_LIBRARY SDL2)
FIND_LIBRARY(SDL2TTF_LIBRARY SDL2_ttf)

TARGET_LINK_LIBRARIES(tetris ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY})

