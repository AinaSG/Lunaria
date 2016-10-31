set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -I/usr/include/freetype2 -L/usr/local/lib -lfreetype")
cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0015 NEW)

#execute_process(COMMAND ln -s ../Lunaria/shaders)
#execute_process(COMMAND ln -s ../Lunaria/images)
#execute_process(COMMAND ln -s ../Lunaria/levels)
#execute_process(COMMAND ln -s ../Lunaria/fonts)

execute_process(COMMAND ln -s ../shaders)
execute_process(COMMAND ln -s ../images)
execute_process(COMMAND ln -s ../levels)
execute_process(COMMAND ln -s ../fonts)

set(appName Bubble)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(SOIL REQUIRED)
find_package(Freetype REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLUT_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${SOIL_INCLUDE_DIRS})
include_directories(${FREETYPE_INCLUDE_DIRS})

link_directories(${OPENGL_LIBRARY_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})
link_directories(${SOIL_LIBRARY_DIRS})

add_executable(${appName} Player.h Player.cpp Text.h Text.cpp TexturedQuad.h TexturedQuad.cpp AnimKeyframes.h Character.cpp Character.h MedicineItem.h SwordItem.h BoneItem.h ShipItem.h GlueItem.h DrillItem.h BlockItem.h BlockItem.cpp EmptyItem.h EmptyItem.cpp Item.h Item.cpp Simplex.h Simplex.cpp Sprite.h Sprite.cpp Input.h Input.cpp ResourceManager.cpp ResourceManager.h TileMap.h TileMap.cpp Texture.h Texture.cpp Scene.h Scene.cpp InfoScene.h InfoScene.cpp MenuScene.h MenuScene.cpp GameScene.h GameScene.cpp Shader.h Shader.cpp ShaderProgram.h ShaderProgram.cpp Enemy.h Enemy.cpp RockEnemy.h RockEnemy.cpp Game.h Game.cpp main.cpp)

target_link_libraries(${appName} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${SOIL_LIBRARIES} ${FREETYPE_LIBRARIES}) # ${GLM_LIBRARIES}
