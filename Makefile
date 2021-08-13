links = -lz -lGL -lGLU -lGLEW -lglfw3 -lrt -lm -ldl -lX11 -lXdmcp -lIrrXML -lassimp -lpthread -lxcb -lXau

app: libdebug FORCE
	sleep 1s
	g++ -o app Sandbox/main.cpp $(wildcard Oxide/build/*.o) -g -I Oxide/src/ -I Oxide/vendor/ -LOxide/vendor/lib -L/usr/local/lib $(links) -std=c++2a

profiling: libprofiling FORCE
	sleep 1s
	g++ -o app Sandbox/main.cpp $(wildcard Oxide/build/*.o) -DTRACY_ENABLE -I Oxide/src/ -I Oxide/vendor/ -LOxide/vendor/lib -L/usr/local/lib $(links) -std=c++2a -O2 -march=native

release: librelease FORCE
	sleep 1s
	g++ -o app Sandbox/main.cpp $(wildcard Oxide/build/*.o) -I Oxide/src/ -I Oxide/vendor/ -LOxide/vendor/lib -L/usr/local/lib $(links) -std=c++2a -O2 -march=native

libdebug:
	$(MAKE) -C Oxide/ debug

libprofiling:
	$(MAKE) -C Oxide/ profiling

librelease:
	$(MAKE) -C Oxide/ release

precompile: FORCE
	g++ -o precompiled.cc Sandbox/main.cpp -I Oxide/src/ -I Oxide/vendor/ -E -std=c++2a

clean: FORCE
	rm -f Oxide/build/* || true

FORCE:
