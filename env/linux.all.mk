CC := g++
CFLAGS := $(CFLAGS) -fpic -lX11

LIB_DIRS := \
	/usr/local/lib

INCLUDE_DIRS := \
	/usr/local/include

BUILD_FLAGS := \
	$(BUILD_FLAGS) \
	-pthread  \
	-lX11

LINK_LIBRARIES := \
	$(LINK_LIBRARIES) \
	stdc++fs \
	X11

PRODUCTION_LINUX_ICON := sfml

PRODUCTION_LINUX_APP_NAME := SFML Boilerplate
PRODUCTION_LINUX_APP_COMMENT := My SFML Game
