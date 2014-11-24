all: main.cpp DbReader.hpp PatternStore.hpp Pattern.hpp
	g++ -o main main.cpp

clean:
	\rm main
