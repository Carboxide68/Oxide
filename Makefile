
objFiles := $(wildcard Oxide/build/*.o)

app:
	g++ -o app Sandbox/main.cpp -g $(objFiles) -I Oxide/src/ -I Oxide/vendor/ -LOxide/vendor/lib -lGL -lGLU -lGLEW -lglfw3 -lrt -lm -ldl -lX11 -lpthread -lxcb -lXau -lXdmcp