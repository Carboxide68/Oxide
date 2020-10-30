
app:
	g++ -static -o app Sandbox/main.cpp -LOxide/lib -LOxide/vendor/lib -loxide -lglfw3 -lGLEW -I Oxide/src/ -I vendor/