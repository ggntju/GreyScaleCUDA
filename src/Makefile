EXECUTABLE_NAME=RGB2GREY
CPP=g++
NVCC=nvcc
ARCH=sm_30
INC=-I/usr/local/cuda/include/ -I/usr/local/include/opencv4
NVCCFLAGS=-Wall -Wextra -Werror -Wshadow -Ofast -fomit-frame-pointer -march=native -funroll-all-loops -fpeel-loops -ftracer -ftree-vectorize
CPPFLAGS=-O0 -g3 -Wall -c -fmessage-length=0 -std=c++17
LIBS=-L/usr/local/cuda/lib64 -L/usr/local/lib -lcudart -lopencv_core -lopencv_imgproc -lopencv_features2d -lopencv_highgui -lopencv_imgcodecs
CPPSOURCES=$(wildcard *.cpp)
CUSOURCES=$(wildcard *.cu)

OBJECTS=$(CPPSOURCES:.cpp=.o) $(CUSOURCES:.cu=.o)

all: $(CPPSOURCES) $(CUSOURCES) $(EXECUTABLE_NAME)

$(EXECUTABLE_NAME) : $(OBJECTS)
	$(CPP) $(CPPFLAGS) $(OBJECTS) -o $@ $(LIBS)

%.o:%.cpp
	$(CPP) -c $(INC) $(CPPFLAGS) $< -o $@

%.o:%.cu
	$(NVCC) --use_fast_math -arch=$(ARCH) -O3 -ccbin $(CC) -std=c++11 -c $(INC) -Xcompiler "$(NVCCFLAGS)" $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE_NAME)