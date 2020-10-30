buildDir = build/
libDir = lib/
includes = src/ vendor/
flags = -std=c++2a -O2 -Wall -Wno-unused-function

includes := $(addprefix -I ,$(includes))

dirs := $(dir $(shell find src/ -name "*.cpp"))
objFiles := $(patsubst %.cpp, %.o, $(addprefix $(buildDir), $(notdir $(foreach dir, $(dirs), $(wildcard $(dir)*.cpp)))))

CC = g++
VPATH = %.cpp $(dirs)

$(buildDir)%.o: %.cpp
	$(CC) $(flags) $(includes) -c $< -o $@

$(libDir)oxide.a: $(objFiles)
	ar rvs $(libDir)oxide.a $(objFiles)

clean:
	rm -f build/* || true

build: $(objFiles)
lib: $(libDir)oxide.a 