links = -lGL -lGLU -lGLEW -lglfw3 -lrt -lm -ldl -lX11 -lpthread -lxcb -lXau -lXdmcp -lassimp
objFiles := $(wildcard Oxide/build/*.o)

app: FORCE
	g++ -o app Sandbox/main.cpp $(objFiles) -g -I Oxide/src/ -I Oxide/vendor/ -LOxide/vendor/lib $(links) -std=c++2a

precompile: FORCE
	g++ -o precompiled.cc Sandbox/main.cpp -I Oxide/src/ -I Oxide/vendor/ -E -std=c++2a

FORCE:
