ALL = pipline pipesquare sequential
BUILD = build/
CPPFLAGS = -std=c++11 -lpthreads

PIPELINE = pipeline/
PIPESQUARE = pipesquare/
SEQUENTIAL = sequential/

.PHONY : ALL clean build 

.SECONDEXPANSION:
sequential : $$(BUILD)sequential.o $$(BUILD)encoder.o $$(BUILD)types.o $$(BUILD)input.o $$(BUILD)output.o 
	g++ $(CPPFLAGS) -o testut $^

.SECONDEXPANSION:
build/sequential.o : $$(SEQUENTIAL)sequential.cpp $$(BUILD)encoder.o $$(BUILD)input.o $$(BUILD)output.o
	g++ $(CPPFLAGS) -c $< -o $@

.SECONDEXPANSION:
build/encoder.so : encoder.hpp $$(BUILD)types.o
	g++ $(CPPFLAGS) -c $< -o $@

.SECONDEXPANSION:
build/types.so : types.hpp 
	g++ $(CPPFLAGS) -c $< -o $@

.SECONDEXPANSION:
build/input.so : input.hpp $$(BUILD)stb_image.o $$(BUILD)types.o 
	g++ $(CPPFLAGS) -c $< -o $@

.SECONDEXPANSION:
build/output.so : output.hpp $$(BUILD)types.o
	g++ $(CPPFLAGS) -c $< -o $@

.SECONDEXPANSION:
build/stb_image.so : stb_image.h
	gcc -c $< -o $@

clean :
	-rm $(BUILD)*.o 

