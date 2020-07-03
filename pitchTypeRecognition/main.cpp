#include <stdio.h>
#include <string.h>
#include "pitchTypeRecognition.h"

float ball_vectorX[20];
float ball_vectorY[20];
int speed;

struct data fastball,curveball,sinker,slider,splitter,changeup;
struct dataPerPitch input;

int main()
{	
	
	FILE * fp;
	printf("Please type video filename.\n");
	char * video_name;
	char * video_name_withPath;
	video_name = (char*)malloc(sizeof(30));
	video_name_withPath = (char*)malloc(sizeof(30));
	strcpy(video_name_withPath,"./Video/");
    scanf("%s",video_name);
	strcat(video_name_withPath,video_name);

	int ans_get_picture=get_picture(video_name_withPath);
	if(ans_get_picture==-1)
    	cout <<"Get frames fail."<<endl;
	else
    	cout<<"Get "<<ans_get_picture<<" frames from video."<<endl;

    cut_img(ans_get_picture);
	binarization(ans_get_picture);
	get_location(ans_get_picture);
	

	fp = fopen("result.txt","r");

	int i;
	for(i=0;i<=19;i++)
	{
		ball_vectorX[i] = 0;
		ball_vectorY[i] = 0;
	}
	i=0;
	
	while(fscanf(fp,"%f %f\n",&ball_vectorX[i],&ball_vectorY[i])!=-1)
	{
		//printf("%d %d\n",ball_vectorX[i],ball_vectorY[i]);
		i++;
	}
	vectorComplete(ball_vectorX,ball_vectorY);
	printf("Avg : %f %f\n",input.xAvg,input.yAvg);
	if(!(input.xAvg<100 && input.xAvg>-100 && input.yAvg<100 && input.yAvg >-100))
	{
		printf("Acceleration out of range!\n");
		exit(0);
	}
	input.speed = readSpeed(video_name);
	if(input.speed == -1)
	{
		printf("Can not read speed\n");
		exit(0);
	}
	//printf("Please type speed\n");
	//cin>>input.speed;
	printf("Speed: %d\n\n",input.speed);
	
	fastball = loadData("./pitchTypeData/fastball_coordinate.txt");
	curveball = loadData("./pitchTypeData/curveball_coordinate.txt");
	sinker = loadData("./pitchTypeData/sinker_coordinate.txt");
	slider = loadData("./pitchTypeData/slider_coordinate.txt");
	splitter = loadData("./pitchTypeData/splitter_coordinate.txt");
	changeup = loadData("./pitchTypeData/changeup_coordinate.txt");
	fastball = computeAcceleration(fastball);
	curveball = computeAcceleration(curveball);
	sinker = computeAcceleration(sinker);
	slider = computeAcceleration(slider);
	splitter = computeAcceleration(splitter);
	changeup = computeAcceleration(changeup);

	srand((unsigned)time(NULL));
	printf("K-means (K=3):\n");
	printf("[C]:Curveball  [L]:Slider  [P]:Splitter  [H]:Changeup  [S]:Sinker [F]:Fastball\n");
	Kmeans_Algorithm("111111",3);
	drawAllAndCenters(3);
	printf("\n\nK-means (K=2):\n");
	Kmeans_Algorithm("010100",2);
	drawAllAndCenters(2);
	pitchTypeRecognition(video_name);
	
	free(video_name);
	free(video_name_withPath);
    return 0;
}
