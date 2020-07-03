#include <iostream>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <cstring>
#include <sstream>
#include <fstream>
#include "pitchTypeRecognition.h"

using namespace cv;
using namespace std;



int get_picture(string s)
{
    Mat frame;
    VideoCapture cap(s.c_str());

    if(!cap.isOpened())
        return -1;
    else
    {
        int i=1;
        while(1)
        {
            int x=cap.read(frame);
            if(x==0)
            {
                i--;
                break;
            }
            stringstream ss;
            ss<<i;
            string str=ss.str();
            str+=".jpg";

            imwrite (str.c_str(),frame);
            i++;
        }
        return i;
    }
}

void cut_img(int size)
{  
      
    int i;
    for(i=1;i<=size;i++)
    {
        stringstream ss;
        ss<<i;
        string str=ss.str();
        str+=".jpg";

        Mat picture=imread(str.c_str(),CV_LOAD_IMAGE_GRAYSCALE);
        Mat dst;
    	Rect rect(500,100,400,400);  
    	picture(rect).copyTo(dst); 

        string qw=ss.str()+".jpg";
        imwrite(qw.c_str(),dst);
    }
    
} 

void binarization(int size)
{
    int i;
    for(i=1;i<=size;i++)
    {
        stringstream ss;
        ss<<i;
        string str=ss.str();
        str+=".jpg";

        Mat picture=imread(str.c_str(),CV_LOAD_IMAGE_GRAYSCALE);
        Mat dst;
        threshold(picture,dst,170,255,THRESH_BINARY);
        string qw=ss.str()+"b.jpg";
        imwrite (qw.c_str(),dst);
    }
}


void get_location(int size)
{
    fstream file1;
    file1.open("data.txt",ios::out);

    int i,j,k;
	Mat roiImg[7];
	Mat src;
	for(j=6;j<=12;j++)
	{
		stringstream ss2;
        ss2<<j;
        string str2=ss2.str();
        string path2="/Users/YuRong/Desktop/pitchTypeRecognition/base/"+str2+".bmp";
		roiImg[j-6] = imread(path2.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	}
    for(i=1;i<=size;i++)
    {
        
        stringstream ss1;
        ss1<<i;
        string str1=ss1.str()+"b.jpg";
		src = imread(str1.c_str(), CV_LOAD_IMAGE_GRAYSCALE);

        for(j=6;j<=12;j++)
        {
            stringstream ss2;
            ss2<<j;
            string str2=ss2.str();
            //string path2="/home/domino/Desktop/pitchTypeRecognition/base/"+str2+".bmp";


            Mat displayImg = src.clone();
            Mat result;
            result.create(src.rows-roiImg[j-6].rows+1, src.cols-roiImg[j-6].cols+1, CV_32FC1);

            matchTemplate(src, roiImg[j-6], result, CV_TM_SQDIFF);
            double minVal; 
            Point minLoc;
            minMaxLoc(result, &minVal, 0, &minLoc, 0);

            rectangle(displayImg, minLoc, Point(minLoc.x+roiImg[j-6].cols , minLoc.y+roiImg[j-6].rows),Scalar(128,128,128),2);
            string x;
            string name=ss1.str()+"_"+ss2.str()+".bmp";
            x+=name;
            imwrite (x.c_str(),displayImg);

            file1<<minLoc.x<<" "<<minLoc.y<<endl;
        }

        file1<<endl;
    }
	
    file1.close();
    determine("data.txt",size);
}

void determine(string str,int size)
{
    fstream file1,file2;
    
    file1.open(str.c_str(),ios::in);    //the data of ball's path
    file2.open("result.txt",ios::out);
    

    int re_x[250],re_y[250];  //store the correct location of the i-th picture
    char s[20]; //read string

    int check_first=0;
    int i,j,k;
    int y_down=0,x_change=0;
    int pre=0;
    int con=0;


    for(i=1;i<=size;i++)
    {
        int x[7],y[7];  //store every sample's location of the i-th picture
        int eye[7];
        int check_all_fault=0;
        //----file1---------
        for(j=0;j<=6;j++)
        {
            file1.getline(s,20);
            char *test=strtok(s," ");
            x[j]=atoi(test);
            test=strtok(NULL," ");
            y[j]=atoi(test);
        }
        file1.getline(s,20);    //read space rows


        int check_sample[7];
        //cout<<"---------------------------------------"<<endl;
        //cout<<"p:"<<i<<endl;
        for(j=0;j<=6;j++)
        {
            if(check_first==0)	  //determine the data is the first picture
            {

                    check_sample[j]=1;
            }
            else
            {
                if((x[j]>=re_x[pre]-20*(i-pre) && x[j]<=re_x[pre]+20*(i-pre)) && (y[j]>=re_y[pre]-20*(i-pre) && y[j]<=re_y[pre]+20*(i-pre)))
                    check_sample[j]=1;
                else
                    check_sample[j]=0;
				//cout<<j<<"b: "<< check_sample[j]<<"\tx:"<<x[j]<<"\ty:"<<x[j]<<endl;

                if(check_sample[j]==1)
                {
                    //-----determine y if non-regular-------
                    if(y_down==0)
                    	check_sample[j]=1;
                    else if(y_down==1&&y[j]+5<re_y[pre])
                    	check_sample[j]=0;

                    //-----determine x if non-regular-------

                    if(x[j]<re_x[pre]&&x_change==2)
                        check_sample[j]=0;
                    else if(x[j]>re_x[pre]&&x_change==-2)
                        check_sample[j]=0;
                }

            }
            //cout<<j<<": "<< check_sample[j]<<endl;
        }

        for(j=0;j<=6;j++)
        {
            if(check_sample[j]==1)
            {
                for(k=0;k<=6;k++)
                {
                    if(k==j||check_sample[k]==0)
                        continue;
                    else if((x[j]+10>=x[k]&&x[j]-10<=x[k])&&(y[j]+10>=y[k]&&y[j]-10<=y[k]))
                        check_sample[j]++;
                }
            }
        }

        int max=0,ans=-1;
        for(j=0;j<=6;j++)
        {
            if(check_sample[j]>max)
            {
                max=check_sample[j];
                ans=j;
            }
        }
        //cout<<ans<<endl;
        if(check_sample[ans]>1&&ans!=-1)
        {

            re_x[i]=x[ans];
            re_y[i]=y[ans];
            
            if(check_first>=1)
            {
                if(re_y[i]<re_y[pre])
                y_down=1;
            	

                if(x_change==0)
                {
                    if(re_x[i]<re_x[pre])
                        x_change=-1;
                    else if(re_x[i]>re_x[pre])
                        x_change=1;
                }
                else if(x_change==1&&re_x[i]<re_x[pre])
                    x_change=-2;
                else if(x_change==-1&&re_x[i]>re_x[pre])
                    x_change=2;
            }

            check_first++;
            pre=i;
        }
        else
        {
            re_x[i]=0;
            re_y[i]=0;
        }
    }
    for(i=1;i<=size;i++)
    {
    	if(re_x[i]!=0&&re_y[i]!=0)
    		file2<<re_x[i]<<" "<<re_y[i]<<endl;
    	else
    		file2<<"0 0"<<endl;

    	if(i==con)
    	cout<<"----------------"<<endl;

    	cout<<i<<" : "<<re_x[i]<<" "<<re_y[i]<<endl;
    }
	cout<<endl<<endl;

    file1.close();
    file2.close();
}

void vectorComplete(float x[20],float y[20])
{
	int i=0,k;
	int nullStart,nullEnd;
	float velocityX[19];
	float velocityY[19];
	float accX,accY;
	float disX,disY,time;

	for(i=0;i<=19;i++)
	{
		//printf("%f %f\n",x[i],y[i]);
		velocityX[i]=0;
		velocityY[i]=0;
		if(x[i]==0 || x[i+1]==0)
			continue;
		velocityX[i] = x[i+1] - x[i];
		velocityY[i] = y[i+1] - y[i];
		
	}
	
	for(i=0;i<=19;i++)
	{
		if(x[i]==0 && y[i]==0)
		{
			nullStart = i;
			
			if(nullStart>=2)
			{
				for(k=i+1;k<=19;k++)
				{
					if(x[k]!=0 && y[k]!=0)
					{
						nullEnd = k;
						break;
					}
					if(k==19)		//null recovering complete
					{
						goto l1;
						//return;
					}
				}
				time = nullEnd -nullStart+1;
				disX = x[nullEnd] - x[nullStart-1];
				disY = y[nullEnd] - y[nullStart-1];
				//printf("dis:%f %f  %d %d\n",disX,disY,nullStart,nullEnd);
				accX = 2*( disX-(velocityX[i-2]*time ) ) / (time*time) ;
				accY = 2*( disY-(velocityY[i-2]*time ) ) / (time*time) ;
				//printf("acc %f  %f\n",accX,accY);
				for(k=nullStart;k<nullEnd;k++)
				{
					velocityX[k-1] = velocityX[k-2] + accX;
					velocityY[k-1] = velocityY[k-2] + accY; 
					x[k] = x[k-1] + velocityX[k-1];
					y[k] = y[k-1] + velocityY[k-1];
					if(x[k+1]!=0 && y[k+1]!=0)
					{
						velocityX[k] = x[k+1] - x[k];
						velocityY[k] = y[k+1] - y[k];
					}
					//printf("fix  %f  %f  %d\n",x[k],y[k],k);
				}
			}
			else if(nullStart==1)
			{
				for(k=i+1;k<=19;k++)
				{
					if(velocityX[k]!=0 && velocityY[k]!=0)
					{
						nullEnd = k;
						break;
					}
					if(k==19)		//null recovering complete
					{
						goto l1;
						//return;
					}
				}
				time = nullEnd -nullStart+1;
				disX = x[nullStart-1] - x[nullEnd];
				disY = y[nullStart-1] - y[nullEnd];
				accX = 2*( disX-(-(velocityX[nullEnd]*time) ) ) / (time*time) ;
				accY = 2*( disY-(-(velocityY[nullEnd]*time) ) ) / (time*time) ;
				for(k=nullEnd-1;k>=nullStart;k--)
				{
					velocityX[k] = -((-velocityX[k+1]) + accX);
					velocityY[k] = -((-velocityY[k+1]) + accY); 
					x[k] = x[k+1] - velocityX[k];
					y[k] = y[k+1] - velocityY[k];
					if(x[k-1]!=0 && y[k-1]!=0)
					{
						velocityX[k-1] = x[k] - x[k-1];
						velocityY[k-1] = y[k] - y[k-1];
					}
				}
			}

		}
		//i = nullEnd;
	}
l1:;
	int count=0;
	int xVelocity[MAX_VECTOR_AMOUNT-1],yVelocity[MAX_VECTOR_AMOUNT-1],xAvg=0,yAvg=0;
	for(i=0;i<=19;i++)
	{
		if(x[i]!=0 && y[i]!=0)
		{
			input.x[i] = x[i];
			input.y[i] = y[i];
			count++;
		}
		//printf("%.2f %.2f\n",x[i],y[i]);
	}
	input.vectorAmount = count;
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
