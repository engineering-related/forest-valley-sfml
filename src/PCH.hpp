#ifndef PRECOMPILED_HEADER_HPP
#define PRECOMPILED_HEADER_HPP

#ifndef _DEBUG
	#ifndef NDEBUG
		#define NDEBUG
	#endif
#endif // _DEBUG

// SFML
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

// Raspberry Pi
#ifdef SFML_SYSTEM_LINUX
	#ifdef __arm__
		#define SFML_SYSTEM_PI
	#endif
#endif // SFML SYSTEM_LINUX

// Typical stdafx.h
#include <algorithm>
#include <cstdio>
#include <deque>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <climits>

// Additional C/C++ libs
#include <atomic>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <exception>
#include <functional>
#include <iomanip>
#include <limits>
#include <mutex>
#include <random>
#include <sstream>
#include <thread>
#include <type_traits>
#include <typeinfo>

// Windows
#ifdef _WIN32
	#ifndef UNICODE
		#define UNICODE
	#endif

	#ifndef _UNICODE
		#define _UNICODE
	#endif

	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif // _WIN32

// Utils
#include "Utility/FileSystem.hpp"
#include "Utility/Types.hpp"
#include "Utility/Functions.hpp"
#include "Utility/Textures.hpp"

// Macros
#define UNUSED(x) (void)(x)
#define PI 3.14159265359

//Game
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define MULTIPLIER 60
#define FRAMERATELIMIT 144
#define CAP 30
#define SCALE sf::Vector2f(1.f, 1.f)
#define TILE_SIZE sf::Vector2i(32, 32)

#endif // PRECOMPILED_HEADER_HPP
