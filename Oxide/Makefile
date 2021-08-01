buildDir = build/
libDir = lib/
includes = src/ vendor/
flags = -std=c++2a -Wall -Wno-unused-function
Cflags = -Wall -Wno-unused-function -O2 -march=native

includes := $(addprefix -I ,$(includes))
additionalFiles =
dirs := $(dir $(shell find src/ -name "*.cpp" -o -name "*.c"))
objFiles := $(patsubst %.cpp, %.o, $(addprefix $(buildDir), $(notdir $(foreach dir, $(dirs), $(wildcard $(dir)*.cpp))))) $(patsubst %.c, %.o, $(addprefix $(buildDir), $(notdir $(foreach dir, $(dirs), $(wildcard $(dir)*.c)))))

CC = g++
VPATH = %.cpp $(dirs)

debug: flags := $(flags) -g -DDEBUG
debug: $(objFiles)

release: flags := $(flags) -O2 -march=native
release: $(objFiles)

profiling: flags := $(flags) -O2 -march=native -DTRACY_ENABLE
profiling: $(objFiles)


precompile.cc: flags := $(flags) -E
precompile.cc: $(objFiles)

lib: $(libDir)liboxide.a 

$(buildDir)%.o: %.cpp
	$(CC) $(flags) $(includes) -c $< -o $@


$(buildDir)%.o: %.c
	gcc $(Cflags) $(includes) -c $< -o $@

$(libDir)liboxide.a: $(objFiles)
	ar rvs $(libDir)liboxide.a $(objFiles)

clean:
	rm -f build/* || true
