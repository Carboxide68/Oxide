links = -lz -lGL -lGLU -lGLEW -lglfw3 -lrt -lm -ldl -lX11 -lXdmcp -lIrrXML -lassimp -lpthread -lxcb -lXau
objFiles := $(wildcard Oxide/build/*.o)

app: FORCE
	g++ -o app Sandbox/main.cpp $(objFiles) -g -I Oxide/src/ -I Oxide/vendor/ -LOxide/vendor/lib -L/usr/local/lib $(links) -std=c++2a

profiling: FORCE
	g++ -o app Sandbox/main.cpp Oxide/vendor/tracy/TracyClient.cpp $(objFiles) -DTRACY_ENABLE -I Oxide/src/ -I Oxide/vendor/ -LOxide/vendor/lib -L/usr/local/lib $(links) -std=c++2a -O2 -march=native

release: FORCE
	g++ -o app Sandbox/main.cpp $(objFiles) -I Oxide/src/ -I Oxide/vendor/ -LOxide/vendor/lib -L/usr/local/lib $(links) -std=c++2a -O2 -march=native

precompile: FORCE
	g++ -o precompiled.cc Sandbox/main.cpp -I Oxide/src/ -I Oxide/vendor/ -E -std=c++2a

FORCE:
