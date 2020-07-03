#include<stdio.h> 
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>               //for bool
#include<time.h>                 
#include<math.h>
#include"pitchTypeRecognition.h"





struct data loadData(const char * filename)
{
	//pitchNum stores the data read from the file
	//pitchCounter means the name says
	int pitchNum,pitchCounter=0;
	//temp structure
	struct data temp;
	//i for the for loop
	int i;
	
	char * str = (char*)malloc(sizeof(char)*10);
	FILE * file = fopen(filename,"r");
	if(file == NULL)
	{
		printf("Fopen failed!\n");
		sleep(1);
		exit(0);
	}
	while(1)		//pitch1, pitch2, pitch3,........ 
	{
		//vectorNum stores the amount of the pitch vectors
		int vectorNum; 
		fscanf(file,"%s",str);
		if(strcmp(str,"end")==0)
		{
			temp.pitchAmount = pitchCounter;
			return temp;
		}
		fscanf(file," %d",&pitchNum);
		pitchCounter++;
		if((strcmp(str,"pitch")!=0)||(pitchNum!=pitchCounter))
		{
			printf("The format of coordinate file is wrong!\n");
			sleep(1);
			exit(0);
		}
		fscanf(file,"%d",&temp.speed[pitchCounter-1]);
		fscanf(file,"%d",&vectorNum);
		temp.vectorAmount[pitchCounter-1] = vectorNum;
		for(i=0;i<vectorNum;i++)
		{
			fscanf(file,"%d %d",&temp.x[pitchCounter-1][i],&temp.y[pitchCounter-1][i]);
		}
	}
	fclose(file);
	free(str);
} 

void showData(struct data thedata)
{
	int i;
	for(i=0;i<thedata.pitchAmount;i++)
	{
		printf("Pitch %d\n",i+1);
		printf("Speed:%d\n",thedata.speed[i]);
		printf("%d\n",thedata.vectorAmount[i]);
		int x;
		for(x=0;x<thedata.vectorAmount[i];x++)
		{
			printf("%d %d\n",thedata.x[i][x],thedata.y[i][x]);
		}
		for(x=0;x<thedata.vectorAmount[i]-2;x++)
		{
			printf("%d %d\n",thedata.xAcceleration[i][x],thedata.yAcceleration[i][x]);
		}
	}
}

struct data computeAcceleration(struct data thedata)
{
	int xVelocity[MAX_VECTOR_AMOUNT-1];
	int yVelocity[MAX_VECTOR_AMOUNT-1];
	int i;
	for(i=0;i<thedata.pitchAmount;i++)
	{
		int x;
		for(x=0;x<thedata.vectorAmount[i]-1;x++)
		{
			xVelocity[x] = thedata.x[i][x+1]-thedata.x[i][x];
			yVelocity[x] = thedata.y[i][x+1]-thedata.y[i][x];
		}
		for(x=0;x<thedata.vectorAmount[i]-2;x++)
		{
			thedata.xAcceleration[i][x] = xVelocity[x+1]-xVelocity[x];
			thedata.yAcceleration[i][x] = yVelocity[x+1]-yVelocity[x];
		}
		int xAvg=0,yAvg=0;
		for(x=0;x<thedata.vectorAmount[i]-2;x++)	//calculate the average of the accelerations
		{
			xAvg = xAvg + thedata.xAcceleration[i][x];
			yAvg = yAvg + thedata.yAcceleration[i][x];
		}
		thedata.xAvg[i] = (int)(xAvg*10/(thedata.vectorAmount[i]-2));	//*10 for making the distribution more clear
		thedata.yAvg[i] = (int)(yAvg*10/(thedata.vectorAmount[i]-2));
	}
	return thedata;
}

void getInput()
{
	int i,xVelocity[MAX_VECTOR_AMOUNT-1],yVelocity[MAX_VECTOR_AMOUNT-1],xAvg=0,yAvg=0;
	printf("Please key input data\n");
	printf("What is the speed of the pitch?\n");
	scanf("%d",&input.speed);
	printf("What is the amount of vectors?\n");
	scanf("%d",&input.vectorAmount);
	printf("Please key the vectors of the pitch (x y)\n");
	for(i=0;i<input.vectorAmount;i++)
	{
		printf("Vector %d\n",i+1);
		scanf("%f %f",&input.x[i],&input.y[i]);
	}
	
	for(i=0;i<input.vectorAmount-1;i++)
	{
		xVelocity[i] = input.x[i+1]-input.x[i];
		yVelocity[i] = input.y[i+1]-input.y[i];
	}
	for(i=0;i<input.vectorAmount-2;i++)
	{
		input.xAcceleration[i] = xVelocity[i+1] - xVelocity[i];
		input.yAcceleration[i] = yVelocity[i+1] - yVelocity[i];
	}
	for(i=0;i<input.vectorAmount-2;i++)
	{
		xAvg = xAvg + input.xAcceleration[i];
		yAvg = yAvg + input.yAcceleration[i];
	}
	input.xAvg = (int)(xAvg*10/(input.vectorAmount-2));
	input.yAvg = (int)(yAvg*10/(input.vectorAmount-2));
}
void getInputFromFile()
{
	FILE * inputFile;
	int i,xVelocity[MAX_VECTOR_AMOUNT-1],yVelocity[MAX_VECTOR_AMOUNT-1],xAvg=0,yAvg=0;
	inputFile = fopen("input.txt","r");
	if(inputFile == NULL)
	{
		printf("Fopen input file failed!\n");
		sleep(1);
		exit(0);
	}
	fscanf(inputFile,"%d",&input.speed);
	fscanf(inputFile,"%d",&input.vectorAmount);
	for(i=0;i<input.vectorAmount;i++)
	{
		fscanf(inputFile,"%f %f",&input.x[i],&input.y[i]);
	}
	for(i=0;i<input.vectorAmount-1;i++)
	{
		xVelocity[i] = input.x[i+1]-input.x[i];
		yVelocity[i] = input.y[i+1]-input.y[i];
	}
	for(i=0;i<input.vectorAmount-2;i++)
	{
		input.xAcceleration[i] = xVelocity[i+1] - xVelocity[i];
		input.yAcceleration[i] = yVelocity[i+1] - yVelocity[i];
	}
	for(i=0;i<input.vectorAmount-2;i++)
	{
		xAvg = xAvg + input.xAcceleration[i];
		yAvg = yAvg + input.yAcceleration[i];
	}
	input.xAvg = (int)(xAvg*10/(input.vectorAmount-2));
	input.yAvg = (int)(yAvg*10/(input.vectorAmount-2));
	fclose(inputFile);
}

void draw(struct data thedata)
{
	int matrix[79][79];
	int i;
	for(i=0;i<79;i++)			//initialize
	{
		int x;
		for(x=0;x<79;x++)
		{
			matrix[i][x] = 0;
		}
	}
	for(i=0;i<thedata.pitchAmount;i++)
	{
		matrix[thedata.xAvg[i]+40][thedata.yAvg[i]+40]++;
	}
	for(i=0;i<79;i++)		//printing
	{
		int x;
		for(x=0;x<79;x++)
		{
			if(matrix[x][i]!=0)
				printf("%d",matrix[x][i]);
			else if((i==40)||(x==40))		//for x=0 and y=0
				printf("+"); 
			else if(matrix[x][i]==0)
				printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}


//===========================Kmeans==================================

int Vectors[MAX_DATA_AMOUNT][DIM]={0};  //every average acceleration point
int centers[GROUP][DIM]={0};                //coordinate of the centers
int table[MAX_DATA_AMOUNT]={0};         //record each point is in which group of the centers
int cluster[GROUP]={0};                     //how many elements are in each group of the centers


int LoadVector(const char * ball)               //load xAvg,yAvg to one array
{
	int i,j;
	int sum=0;
	
	if(strlen(ball)!=6||!strcmp(ball,"000000"))
	{
		printf("error");
		return 0;
	}
	if(ball[0]=='1')
	{	
		for(i=0;i<fastball.pitchAmount;i++)       //fastballs
     	{
   			Vectors[i][0]=fastball.xAvg[i];
			Vectors[i][1]=fastball.yAvg[i];
     	}
      	sum+=fastball.pitchAmount;
	}
	if(ball[1]=='1')
	{
		for(i=i;i<sum+curveball.pitchAmount;i++)    //curveballs
    	{
   			Vectors[i][0]=curveball.xAvg[i-sum];
   			Vectors[i][1]=curveball.yAvg[i-sum];
    	}
    	sum+=curveball.pitchAmount;
	}
	if(ball[2]=='1')
	{
		for(i=i;i<sum+sinker.pitchAmount;i++)        //sinkers
    	{
   			Vectors[i][0]=sinker.xAvg[i-sum];
   			Vectors[i][1]=sinker.yAvg[i-sum];
    	}
    	sum+=sinker.pitchAmount;
	}
	if(ball[3]=='1')
	{
		for(i=i;i<sum+slider.pitchAmount;i++)    //sliders
    	{
   			Vectors[i][0]=slider.xAvg[i-sum];
   			Vectors[i][1]=slider.yAvg[i-sum];
    	}
    	sum+=slider.pitchAmount;
	}
	if(ball[4]=='1')
	{
		for(i=i;i<sum+splitter.pitchAmount;i++)   //splitters
    	{
   			Vectors[i][0]=splitter.xAvg[i-sum];
			Vectors[i][1]=splitter.yAvg[i-sum];
    	}
    	sum+=splitter.pitchAmount; 
	}
	if(ball[5]=='1')
	{
		for(i=i;i<sum+changeup.pitchAmount;i++)           //all in
    	{
   			Vectors[i][0]=changeup.xAvg[i-sum];
   			Vectors[i][1]=changeup.yAvg[i-sum];
      	}
    	sum+=changeup.pitchAmount;
	}
   	return sum;
}


int CalculateDistance(int *target,int *center)           //calculate the distance between two 2-dimension vectors
{
	int dis=0;
	int i;
	for(i=0;i<DIM;i++)
	{
		dis+=(target[i]-center[i])*(target[i]-center[i]);         
	}
	return dis;
}
void GetRandomPoint(int Total_Amount,int K)   //generate K random points for the initial centers
{
	int i,j,k,l;   //for loop
	int point,dis,dis_other,rank=0;         
	int test,temp=0;
	int Random[GROUP]={0,0,0,0,0,0};       //record random select points
	for(i=0;i<K;i++)
	{
		rank=dis=dis_other=temp=0;         
		point=rand()%Total_Amount;
		for(j=0;j<i;j++)            //check if the point is already been chosen
		{
			if(Random[j]==point)     //chosen before
			break;
		}
		if(j==i)                   //haven't been chosen yet
		{
			if(j==0)               //first point
			   Random[i]=point;    //store
    		else
			{
	         	for(l=0;l<i;l++)
	         	{
	         		rank=0;
	         		dis=CalculateDistance(Vectors[point],Vectors[Random[l]]);      //distance between new center and old center
	        		for(k=0;k<Total_Amount;k++)                                    
	            	{
	            		dis_other=0;
		   	        	dis_other=CalculateDistance(Vectors[k],Vectors[Random[l]]); //distance beteen other non-center points to new center 
						if(dis_other<dis)                                           //new center to old center is further than other non-center point
			   	    	{
		   		        	rank++;
			   	    	}
	            	}
	            	test=rand()%Total_Amount;
	            	if(test<(Total_Amount/2))
	            		test=Total_Amount/2;
	            	if(test%Total_Amount>=rank)                                   //delete the new center which is not further enough to the old center
	            	{
	            		temp++;
	            		break;
	            	}
	         	}
	         	if(temp>0)                                                      //bad choice 
	       	    	i--;                                                         //choose again
	   			else                                                            //good choice
	   	    	{
	     	    	Random[i]=point;                                             //store
	   	      	}
    		} 
		}
    	else
        	i--;         //choose again
	}
	for(i=0;i<K;i++)                                                        //store exact data to center array
	{
		centers[i][0]=Vectors[Random[i]][0];
		centers[i][1]=Vectors[Random[i]][1];
	}
}

bool NewCenter(int Total_Amount,int K)                                         //update centers
{
	int i,j,k,cluster_sum[GROUP][DIM];
	int new_center[GROUP][DIM];
	bool change=false;
	memset(cluster_sum, 0, sizeof(cluster_sum));
	for(i=0;i<Total_Amount;i++)
	{
		cluster_sum[table[i]][0]+=Vectors[i][0];
		cluster_sum[table[i]][1]+=Vectors[i][1];
	}
	for(j=0;j<K;j++)
	{
		new_center[j][0]=cluster_sum[j][0]/cluster[j];                   //sum of every element in each group divided by amount
		new_center[j][1]=cluster_sum[j][1]/cluster[j];
		for(k=0;k<DIM;k++)
		{
			if(centers[j][k]!=new_center[j][k])                           //if change
				change=true;
         centers[j][k]=new_center[j][k];
		}
	}
	return change; 
}

void Kmeans(int Total_Amount,int K)
{
	int i,j;   //for loop
	int TempDistance=0,min_dis=0,times=0;
	bool New=true;
	do
	{
		memset(cluster, 0, sizeof(cluster));
		for(i=0;i<Total_Amount;i++)
		{
			for(j=0;j<K;j++)
			{
				TempDistance=CalculateDistance(Vectors[i],centers[j]);        //calculate the distance between every point to every centers
				if(j==0)
				{
					min_dis=TempDistance;                                     //first point
					table[i]=0;                                               //set to group 0 for defaut
				}	
				if(TempDistance<min_dis)                                     //every point set to the group of the nearest center
				{
					min_dis=TempDistance;
					table[i]=j;
				}	
		   	}
			cluster[table[i]]++;                                           //amount of elements in each group
		}
	   	New=NewCenter(Total_Amount,K);                                     //new centers are not eqaul to old ones
	   	times++;                                                         //run 10 times at most   
	}while(times<10000&&New);                                              //stop if center remains the same or run 10 times
}

void Show_Centers(int K)                                                   //display
{
	int i,j; //for loop
	for(i=0;i<K;i++)
	{
		printf("Center%d:",i);
		for(j=0;j<DIM;j++)
		{
			printf("%d ",centers[i][j]);
		}
		printf("\n");
	}
}

void Clear_Data()
{
	memset(Vectors, 0, sizeof(Vectors[0][0] * MAX_DATA_AMOUNT * DIM));  
	memset(centers, 0, sizeof(centers[0][0] * GROUP * DIM));                
	memset(table, 0, sizeof(table));
	memset(cluster, 0, sizeof(cluster));
}

void Kmeans_Algorithm(const char * pitch,int K)	//pitch-> fastball,curveball,sinker,slider,splitter,changeup
{
	int sum=0;                      //amount of all the data
	Clear_Data();               
	sum=LoadVector(pitch);
	GetRandomPoint(sum,K);
  	Kmeans(sum,K);
  	
  	Show_Centers(K);
}
//===========================End======================================
void drawAllAndCenters(int K)
{
	char matrix[79][79];
	int i;
	
	for(i=0;i<79;i++)			//initialize
	{
		int x;
		for(x=0;x<79;x++)
		{
			matrix[i][x] = '0';
		}
	}
	
	for(i=0;i<fastball.pitchAmount;i++)
	{
		matrix[fastball.xAvg[i]+40][40-fastball.yAvg[i]] = 'F';
	}
	for(i=0;i<curveball.pitchAmount;i++)
	{
		matrix[curveball.xAvg[i]+40][40-curveball.yAvg[i]] = 'C';
	}
	for(i=0;i<sinker.pitchAmount;i++)
	{
		matrix[sinker.xAvg[i]+40][40-sinker.yAvg[i]] = 'S';
	}
	for(i=0;i<slider.pitchAmount;i++)
	{
		matrix[slider.xAvg[i]+40][40-slider.yAvg[i]] = 'L';
	}
	for(i=0;i<splitter.pitchAmount;i++)
	{
		matrix[splitter.xAvg[i]+40][40-splitter.yAvg[i]] = 'P';
	}
	for(i=0;i<changeup.pitchAmount;i++)
	{
		matrix[changeup.xAvg[i]+40][40-changeup.yAvg[i]] = 'H';
	}
	for(i=0;i<K;i++)                                         //mark centers               
	{
		matrix[centers[i][0]+40][40-centers[i][1]] = 'X';	
	}
	matrix[(int)input.xAvg+40][40-(int)input.yAvg] = 'N';
	for(i=0;i<49;i++)		//printing
	{
		int x;
		for(x=0;x<79;x++)
		{
			if(matrix[x][i]=='X')
				printf(RED "*" NONE);
			else if(matrix[x][i]=='N')
				printf(PURPLE "%c" NONE,matrix[x][i]);
			else if(matrix[x][i]!='0')
				printf(GREEN "%c" NONE,matrix[x][i]);
			else if((i==40)||(x==40))		//for x=0 and y=0
				printf("+"); 
			else if(matrix[x][i]=='0')
				printf(" ");
		}
		printf("\n");
	}
}

void pitchTypeRecognition(char * video_name)
{
	//3 GROUP center -> (-8,22) (4,5) (4,13)
	printf("The pitch type of %s is ",video_name);
	float min_dis;
	int group,speedGroup;
	min_dis = sqrt(((input.xAvg+8)*(input.xAvg+8))+((input.yAvg-22)*(input.yAvg-22)));
	group = 1;
	if(sqrt(((input.xAvg-4)*(input.xAvg-4))+((input.yAvg-5)*(input.yAvg-5)))<min_dis)
	{
		min_dis = sqrt(((input.xAvg-4)*(input.xAvg-4))+((input.yAvg-5)*(input.yAvg-5)));
		group = 2;
	}
	if(sqrt(((input.xAvg-4)*(input.xAvg-4))+((input.yAvg-13)*(input.yAvg-13)))<min_dis)
	{
		min_dis = sqrt(((input.xAvg-4)*(input.xAvg-4))+((input.yAvg-13)*(input.yAvg-13)));
		group = 3;
	}
	if(group==1)	//curveball -> (-8,27)   slider->(-7,16)
	{
		min_dis = sqrt(((input.xAvg+8)*(input.xAvg+8))+((input.yAvg-27)*(input.yAvg-27)));
		if(sqrt(((input.xAvg+7)*(input.xAvg+7))+((input.yAvg-16)*(input.yAvg-16)))<min_dis)
			printf(PURPLE "Slider!\n" NONE);
		else
			printf(PURPLE "Curveball!\n" NONE);
	}
	else if(group==2)
	{
		printf(PURPLE "Fastball!\n" NONE);
	}
	else if(group==3)	//changeup->82.5  splitter->84.5  sinker->92
	{
		min_dis = sqrt((input.speed-82.5)*(input.speed-82.5));
		speedGroup = 1;
		if(sqrt((input.speed-84.5)*(input.speed-84.5))<min_dis)
		{
			min_dis = sqrt((input.speed-84.5)*(input.speed-84.5));
			speedGroup = 2;
		}
		if(sqrt((input.speed-92)*(input.speed-92))<min_dis)
		{
			min_dis = sqrt((input.speed-92)*(input.speed-92));
			speedGroup = 3;
		}
		if(speedGroup==1)
			printf(PURPLE "Changeup!\n" NONE);
		else if(speedGroup==2)
			printf(PURPLE "Splitter!\n" NONE);
		else if(speedGroup==3)
			printf(PURPLE "Sinker!\n" NONE);
	}
}

int readSpeed(char * name)
{
	char * str;
	int Speed;
	str = (char*)malloc(sizeof(15));
	FILE * fp = fopen("./Video/speed.txt","r");
	while(1)
	{
		if(fscanf(fp,"%s %d\n",str,&Speed) == EOF)
			return -1;
		strcat(str,".mp4");
		if(strcmp(name,str)==0)
		{
			return Speed;
		}
	}
	
	free(str);
}



