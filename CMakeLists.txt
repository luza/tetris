CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

ADD_EXECUTABLE(main main.cpp
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

SET(CMAKE_LIBRARY_PATH ".")
FIND_LIBRARY(SDL2_LIBRARY SDL2)
FIND_LIBRARY(SDL2TTF_LIBRARY SDL2_ttf)

TARGET_LINK_LIBRARIES(main ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY})

