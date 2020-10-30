buildDir = build/
libDir = lib/
includes = src/ vendor/
flags = -std=c++2a -g -Wall -Wno-unused-function

includes := $(addprefix -I ,$(includes))

dirs := $(dir $(shell find src/ -name "*.cpp"))
objFiles := $(patsubst %.cpp, %.o, $(addprefix $(buildDir), $(notdir $(foreach dir, $(dirs), $(wildcard $(dir)*.cpp)))))

CC = g++
VPATH = %.cpp $(dirs)

$(buildDir)%.o: %.cpp
	$(CC) $(flags) $(includes) -c $< -o $@

$(libDir)liboxide.a: $(objFiles)
	ar rvs $(libDir)liboxide.a $(objFiles)

clean:
	rm -f build/* || true

build: $(objFiles)
lib: $(libDir)liboxide.a 