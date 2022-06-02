cxx := g++
includes := ./include
libs := -lws2_32

all:
	$(cxx) src/main.cpp -I $(includes) $(libs) -o tello

.PHONY: clean

clean:
	rm tello
