#include <iostream>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <cstring>
#include <sstream>
#include <fstream>
#include <stdio.h>
#define MAX_PITCH_AMOUNT 10			//the max amount of the pitches of every pitch type
#define MAX_VECTOR_AMOUNT 30		//the max amount of the pictures of every single pitch
#define MAX_DATA_AMOUNT 140      //max amount of the datas
#define GROUP 6                      //K groups
#define DIM 2                    //dimension
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define PURPLE  "\x1B[35m"
#define NONE  "\x1B[0m"


using namespace cv;
using namespace std;

struct data
{
	int pitchAmount;
	int speed[MAX_PITCH_AMOUNT];		//10 pitches for default
	int vectorAmount[MAX_PITCH_AMOUNT];	//10 pitches for default
	int x[MAX_PITCH_AMOUNT][MAX_VECTOR_AMOUNT];		//10 pitches for default
	int y[MAX_PITCH_AMOUNT][MAX_VECTOR_AMOUNT]; 		//10 pitches for default
	int xAcceleration[MAX_PITCH_AMOUNT][MAX_VECTOR_AMOUNT-2];
	int yAcceleration[MAX_PITCH_AMOUNT][MAX_VECTOR_AMOUNT-2];
	int xAvg[MAX_PITCH_AMOUNT];
	int yAvg[MAX_PITCH_AMOUNT];
};

extern struct data fastball,curveball,sinker,slider,splitter,changeup;

struct dataPerPitch
{
	int speed;
	int vectorAmount;
	float x[MAX_VECTOR_AMOUNT];
	float y[MAX_VECTOR_AMOUNT];
	float xAcceleration[MAX_VECTOR_AMOUNT-2];
	float yAcceleration[MAX_VECTOR_AMOUNT-2];
	float xAvg;
	float yAvg;
};

extern struct dataPerPitch input;

void determine(string str,int size);
int get_picture(string s);
void cut_img(int size);
void binarization(int size);
void get_location(int size);
void pitchTypeRecognition(char * video_name);
void getInputFromFile();
void vectorComplete(float x[20],float y[20]);
struct data loadData(const char * filename);
struct data computeAcceleration(struct data thedata);
void Kmeans_Algorithm(const char * pitch,int K);
void drawAllAndCenters(int K);
int readSpeed(char * name);

