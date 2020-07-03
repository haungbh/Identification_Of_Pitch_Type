#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <cstring>
#include <sstream>
#include <fstream>

using namespace cv;
using namespace std;

int main()
{
	int i=1;
	
	
	namedWindow("Display window", WINDOW_AUTOSIZE);
	while(1)
	{
		stringstream ss;
		string str="/home/domino/Desktop/Untitled Folder 3/";
		ss<<i;
        str=ss.str();
		str+=".jpg";
		Mat picture=imread(str.c_str(),CV_LOAD_IMAGE_GRAYSCALE);
		 
		imshow("Display window", picture);
		waitKey(500);
		i++;
		cout<<i<<endl;
		if(i>=10)
			i=1;
	}
}