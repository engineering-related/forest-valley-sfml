cmake_minimum_required(VERSION 3.1)
project(forestvalley)


add_executable(
	forestvalley 
	MapGenerator.cpp
	MapGenerator.h
	NoiseSim.cpp
	PerlinNoise.cpp
	PerlinNoise.h
	Slider.hpp
	simMouse.hpp
	utils.hpp
	)

#find_library(
#	lsfml-graphics
#	lsfml-window
#	lsfml-system
#)


target_link_libraries(lsfml-graphics lsfml-window lsfml-system)


