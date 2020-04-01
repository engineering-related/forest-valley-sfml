OBJS = MapGenerator.o NoiseSim.o PerlinNoise.o
CXX = g++
CXXFLAGS = -std=c++17
LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio 
GAMENAME = forestvalley

game: $(OBJS)
	$(CXX) $(OBJS) -o $(GAMENAME) $(LDLIBS)

#$(OBJS): MapGenerator.h PerlinNoise.h Slider.hpp simMouse.hpp utils.hpp

MapGenerator.o: MapGenerator.h utils.hpp
NoiseSim.o: simMouse.hpp utils.hpp
PerlinNoise.o: PerlinNoise.h utils.hpp


clean: 
	$(RM) $(GAMENAME) $(OBJS)
