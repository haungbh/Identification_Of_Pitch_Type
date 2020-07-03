all: final

final:final.cpp
	g++ `pkg-config --cflags opencv` final.cpp `pkg-config --libs opencv` pitch_recognition_with_Kmeans.cpp main.cpp -o final

clean:
	rm final
