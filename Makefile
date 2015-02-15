# build an executable named index from index.c
all: index

index: invertIndexing.cpp
	g++ --std=c++11 -lpthread -o index invertIndexing.cpp

clean:
	$(RM) index
