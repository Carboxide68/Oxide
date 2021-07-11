buildDir = build/
libDir = lib/
includes = src/ vendor/
flags = -std=c++2a -Wall -Wno-unused-function

includes := $(addprefix -I ,$(includes))

dirs := $(dir $(shell find src/ -name "*.cpp"))
objFiles := $(patsubst %.cpp, %.o, $(addprefix $(buildDir), $(notdir $(foreach dir, $(dirs), $(wildcard $(dir)*.cpp)))))

CC = g++
VPATH = %.cpp $(dirs)

debug: flags := $(flags) -g
debug: $(objFiles)

release: flags := $(flags) -O2
release: $(objFiles)

precompile.cc: flags := $(flags) -E
precompile.cc: $(objFiles)

lib: $(libDir)liboxide.a 

$(buildDir)%.o: %.cpp
	$(CC) $(flags) $(includes) -c $< -o $@

$(libDir)liboxide.a: $(objFiles)
	ar rvs $(libDir)liboxide.a $(objFiles)

clean:
	rm -f build/* || true
