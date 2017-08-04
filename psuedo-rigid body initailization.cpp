//Written by  Kyle Hounslow 2013

// modified by: Ahmad Kaifi, Hassan Althobaiti

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
//, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
//and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//IN THE SOFTWARE.

#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include<fstream>
#include "Object.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <time.h>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <stdlib.h>
#include <cmath>
#include <windows.h>

#define h cout << "bug " << endl;
const int kmax = 10;
using namespace std;
using namespace cv;
 using std::acos; // 反餘弦函數
 using std::sqrt; // 平方根函數
 
 int angle10,angle20,angle30;
 int sLorR1,sLorR2,sLorR3;
 double D0_value,D1_value,D2_value;

 int atangle10,atangle20,atangle30 ; 
  int angle100,angle200,angle300;
 int sLorR100,sLorR200,sLorR300;

 int angle1,angle2,angle3;
 int LorR1,LorR2,LorR3;
 double M_PI=3.1415926;

int n_boards = 0; //Will be set by input list
int board_dt = 90; //Wait 90 frames per chessboard view
int board_w;
int board_h;
//initial min and max HSV filter values.
//these will be changed using trackbars
int H_MIN = 120;
int H_MAX = 179;
int S_MIN = 138;
int S_MAX = 255;
int V_MIN = 146;
int V_MAX = 255;

int H_MIN2 = 36;
int H_MAX2 = 90;
int S_MIN2 = 112;
int S_MAX2 = 255;
int V_MIN2 = 46;
int V_MAX2 = 255;

int H_MIN3 =98;
int H_MAX3 = 104;
int S_MIN3= 43;
int S_MAX3 = 255;
int V_MIN3= 170;
int V_MAX3 = 255;  //new blue

int H_MIN4 =26;
int H_MAX4 = 34;
int S_MIN4= 92;
int S_MAX4 = 255;
int V_MIN4= 46;
int V_MAX4 = 255; //new2 yellow


int H_MIN5 =125;
int H_MAX5 = 155;
int S_MIN5= 43;
int S_MAX5 = 133;
int V_MIN5= 200;
int V_MAX5 = 255;  //new3 purple

int H_MIN6 =11;
int H_MAX6 = 57;
int S_MIN6= 14;
int S_MAX6 = 89;
int V_MIN6= 182;
int V_MAX6 = 255;  //new4 white

//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "red";
const string windowName3 = "After Morphological Operations";
const string windowName4 = "green";
const string windowName6 = "blue";//new
const string windowName7 = "yellow";//new2
const string windowName8 = "purple";//new3
const string windowName9 = "white";//new4
const string trackbarWindowName = "Trackbars";
const string trackbarWindowName2= "trackbars2";
const string trackbarWindowName3= "trackbars3";//new
const string trackbarWindowName4= "trackbars4";//new2
const string trackbarWindowName5= "trackbars5";//new3
const string trackbarWindowName6= "trackbars6";//new4


//The following for canny edge detec
Mat dst, detected_edges;
Mat src, src_gray;
int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;

unsigned framCnt=0;
ofstream fout("output.txt");
ofstream rout;

ofstream fcar0;
ofstream fcar2;
ofstream fcar3;

ofstream icar0;
ofstream icar2;
ofstream icar3;


char* window_name = "Edge Map";

void on_trackbar( int, void* )
{//This function gets called whenever a
	// trackbar position is changed

}

string intToString(int number){

	std::stringstream ss;
	ss << number;
	return ss.str();
}


void createTrackbars(){
	//create window for trackbars
	namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf( TrackbarName, "H_MIN", H_MIN);
	sprintf( TrackbarName, "H_MAX", H_MAX);
	sprintf( TrackbarName, "S_MIN", S_MIN);
	sprintf( TrackbarName, "S_MAX", S_MAX);
	sprintf( TrackbarName, "V_MIN", V_MIN);
	sprintf( TrackbarName, "V_MAX", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH),
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->
	createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
	createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
	createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
	createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
	createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
	createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );
}
void createTrackbars2(){
	//create window for trackbars
	namedWindow(trackbarWindowName2,0);
	//create memory to store trackbar name on window
	char TrackbarName2[50];
	sprintf( TrackbarName2, "H_MIN", H_MIN2);
	sprintf( TrackbarName2, "H_MAX", H_MAX2);
	sprintf( TrackbarName2, "S_MIN", S_MIN2);
	sprintf( TrackbarName2, "S_MAX", S_MAX2);
	sprintf( TrackbarName2, "V_MIN", V_MIN2);
	sprintf( TrackbarName2, "V_MAX", V_MAX2);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH),
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->
	createTrackbar( "H_MIN", trackbarWindowName2, &H_MIN2, H_MAX2, on_trackbar );
	createTrackbar( "H_MAX", trackbarWindowName2, &H_MAX2, H_MAX2, on_trackbar );
	createTrackbar( "S_MIN", trackbarWindowName2, &S_MIN2, S_MAX2, on_trackbar );
	createTrackbar( "S_MAX", trackbarWindowName2, &S_MAX2, S_MAX2, on_trackbar );
	createTrackbar( "V_MIN", trackbarWindowName2, &V_MIN2, V_MAX2, on_trackbar );
	createTrackbar( "V_MAX", trackbarWindowName2, &V_MAX2, V_MAX2, on_trackbar );
}
void createTrackbars3(){
	//create window for trackbars
	namedWindow(trackbarWindowName3,0);
	//create memory to store trackbar name on window
	char TrackbarName3[50];
	sprintf( TrackbarName3, "H_MIN", H_MIN3);
	sprintf( TrackbarName3, "H_MAX", H_MAX3);
	sprintf( TrackbarName3, "S_MIN", S_MIN3);
	sprintf( TrackbarName3, "S_MAX", S_MAX3);
	sprintf( TrackbarName3, "V_MIN", V_MIN3);
	sprintf( TrackbarName3, "V_MAX", V_MAX3);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH),
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->
	createTrackbar( "H_MIN", trackbarWindowName3, &H_MIN3, H_MAX3, on_trackbar );
	createTrackbar( "H_MAX", trackbarWindowName3, &H_MAX3, H_MAX3, on_trackbar );
	createTrackbar( "S_MIN", trackbarWindowName3, &S_MIN3, S_MAX3, on_trackbar );
	createTrackbar( "S_MAX", trackbarWindowName3, &S_MAX3, S_MAX3, on_trackbar );
	createTrackbar( "V_MIN", trackbarWindowName3, &V_MIN3, V_MAX3, on_trackbar );
	createTrackbar( "V_MAX", trackbarWindowName3, &V_MAX3, V_MAX3, on_trackbar );
}//new
void createTrackbars4(){
	//create window for trackbars
	namedWindow(trackbarWindowName4,0);
	//create memory to store trackbar name on window
	char TrackbarName4[50];
	sprintf( TrackbarName4, "H_MIN", H_MIN4);
	sprintf( TrackbarName4, "H_MAX", H_MAX4);
	sprintf( TrackbarName4, "S_MIN", S_MIN4);
	sprintf( TrackbarName4, "S_MAX", S_MAX4);
	sprintf( TrackbarName4, "V_MIN", V_MIN4);
	sprintf( TrackbarName4, "V_MAX", V_MAX4);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH),
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->
	createTrackbar( "H_MIN", trackbarWindowName4, &H_MIN4, H_MAX4, on_trackbar );
	createTrackbar( "H_MAX", trackbarWindowName4, &H_MAX4, H_MAX4, on_trackbar );
	createTrackbar( "S_MIN", trackbarWindowName4, &S_MIN4, S_MAX4, on_trackbar );
	createTrackbar( "S_MAX", trackbarWindowName4, &S_MAX4, S_MAX4, on_trackbar );
	createTrackbar( "V_MIN", trackbarWindowName4, &V_MIN4, V_MAX4, on_trackbar );
	createTrackbar( "V_MAX", trackbarWindowName4, &V_MAX4, V_MAX4, on_trackbar );
}//new2
void createTrackbars5(){
	//create window for trackbars
	namedWindow(trackbarWindowName5,0);
	//create memory to store trackbar name on window
	char TrackbarName5[50];
	sprintf( TrackbarName5, "H_MIN", H_MIN5);
	sprintf( TrackbarName5, "H_MAX", H_MAX5);
	sprintf( TrackbarName5, "S_MIN", S_MIN5);
	sprintf( TrackbarName5, "S_MAX", S_MAX5);
	sprintf( TrackbarName5, "V_MIN", V_MIN5);
	sprintf( TrackbarName5, "V_MAX", V_MAX5);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH),
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->
	createTrackbar( "H_MIN", trackbarWindowName5, &H_MIN5, H_MAX5, on_trackbar );
	createTrackbar( "H_MAX", trackbarWindowName5, &H_MAX5, H_MAX5, on_trackbar );
	createTrackbar( "S_MIN", trackbarWindowName5, &S_MIN5, S_MAX5, on_trackbar );
	createTrackbar( "S_MAX", trackbarWindowName5, &S_MAX5, S_MAX5, on_trackbar );
	createTrackbar( "V_MIN", trackbarWindowName5, &V_MIN5, V_MAX5, on_trackbar );
	createTrackbar( "V_MAX", trackbarWindowName5, &V_MAX5, V_MAX5, on_trackbar );
}//new3
void createTrackbars6(){
	//create window for trackbars
	namedWindow(trackbarWindowName6,0);
	//create memory to store trackbar name on window
	char TrackbarName6[50];
	sprintf( TrackbarName6, "H_MIN", H_MIN6);
	sprintf( TrackbarName6, "H_MAX", H_MAX6);
	sprintf( TrackbarName6, "S_MIN", S_MIN6);
	sprintf( TrackbarName6, "S_MAX", S_MAX6);
	sprintf( TrackbarName6, "V_MIN", V_MIN6);
	sprintf( TrackbarName6, "V_MAX", V_MAX6);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH),
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->
	createTrackbar( "H_MIN", trackbarWindowName6, &H_MIN6, H_MAX6, on_trackbar );
	createTrackbar( "H_MAX", trackbarWindowName6, &H_MAX6, H_MAX6, on_trackbar );
	createTrackbar( "S_MIN", trackbarWindowName6, &S_MIN6, S_MAX6, on_trackbar );
	createTrackbar( "S_MAX", trackbarWindowName6, &S_MAX6, S_MAX6, on_trackbar );
	createTrackbar( "V_MIN", trackbarWindowName6, &V_MIN6, V_MAX6, on_trackbar );
	createTrackbar( "V_MAX", trackbarWindowName6, &V_MAX6, V_MAX6, on_trackbar );
}//new4


void morphOps(Mat &thresh){

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle
	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);


	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);
}

double SSE (vector <int> pointsvec, Mat centers)
{
	 int k = centers.rows;
	 double sum = 0;
	 for(int i = 0; i < pointsvec.size()/2;i++)
	 {
		double x,y;
		double mini = DBL_MAX;
		y = pointsvec[2*i];
		x = pointsvec[2*i + 1];
		for(int j = 0; j < k ; j++)
		{
			float cenX = centers.at<float>(j,0);
			float cenY = centers.at<float>(j,1) ;
			double temp = pow(x - cenX, 2) + pow(y - cenY, 2);
			if(temp < mini)
				mini = temp;
		}
		sum += mini;
	 }

	 cout << "SSE k = " << k << " : " << sum << endl;
	 return sum;
}

Mat trackFilteredObject(Mat threshold,Mat HSV, Mat &cameraFeed)
{
	Mat temp;
	threshold.copyTo(temp);
	vector <int> pointsvec ;//宣告一個vector儲存pointspos
	//cout<<threshold.rows<<","<<threshold.cols<<endl;
	Mat label;
	
	for(int i = 0; i <threshold.rows; i++){
		//const unsigned int* row = threshold.ptr<unsigned int>(i);
		for(int j = 0; j < threshold.cols; j++)
		{
			if(threshold.at<uchar>(i, j) == 255)
			{
				pointsvec.push_back(i);
				pointsvec.push_back(j);

			}
		}
	}
	//for(int i = 0; i < pointsvec.size(); i++)
	//{
	//if(i % 2 == 0)
	//	cout << "(" << pointsvec[i];
	//else
	//	cout << "," << pointsvec[i] << ")\n";
	//}//測試存取ｍａｔ程式碼
	
	int matsize = pointsvec.size()/2;
	Mat points(matsize,1,CV_32FC2);//將vector存入points
	for(int i=0; i<matsize; i++)
		for(int j=0; j<2; j++){

   points.at<Vec2f>(i)[0] = pointsvec[i*2+1];

   points.at<Vec2f>(i)[1] = pointsvec[i*2];

  }
		
	vector <Mat> centerss;//宣告一個矩陣儲存各k值得centers
	vector <double> SSEs;//各k值sses
	vector <double> SSEslopes;//各k值sse斜率
	vector <double> slopedivides;//各k值sse/sse
	centerss.reserve(kmax);
	SSEs.reserve(kmax);
	SSEslopes.reserve(kmax);
	slopedivides.reserve(kmax);
	double SSEdivide_max = DBL_MIN;
	double testdou;
	int asso_k = 1;
	for(int i = 0; i < kmax;i++)
	{
		Mat temp(i+1, 1, threshold.type());
		centerss.push_back(temp);
	}
	
	for(int j = 0; j < kmax; j++)//kmeans
	{
	kmeans(points, j+1,label,
              TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 20, 1.0),
              2, KMEANS_PP_CENTERS, centerss[j]);
	SSEs.push_back(SSE(pointsvec,centerss[j]));	
	
	}

	
	if(SSEs[0] < 900000)
	{
		cout<<"centers"<<centerss[asso_k -1]<<endl;
	cout << "k = " << asso_k << endl;

	for(int i = 0; i < asso_k;i++)
	{
		float x,y;
		x = centerss[asso_k -1].at<float>(i,0);
		y = centerss[asso_k -1].at<float>(i,1);
		Point centerspoint;
		centerspoint.x = x;
		centerspoint.y = y;
		circle(cameraFeed,centerspoint,10,(0,255,0));
	}

	return centerss[asso_k -1];
	}

	for(int k=0;k<kmax-1;k++)
	{
		SSEslopes.push_back(SSEs[k]-SSEs[k+1]);
		cout<<"k="<<k<<"slope"<<SSEslopes[k]<<endl;
	}
	for(int i=0;i<kmax-2;i++)
	{
		slopedivides.push_back(SSEslopes[i]/SSEslopes[i+1]);
		if(slopedivides[i] > SSEdivide_max)
		{
		SSEdivide_max = slopedivides[i];
		asso_k = i + 2;
		}
		cout<<"slopedivides"<<slopedivides[i]<<endl;
	}



	//cout<<"points"<<points<<endl;

	cout<<"centers"<<centerss[asso_k -1]<<endl;
	cout << "k = " << asso_k << endl;

	for(int i = 0; i < asso_k;i++)
	{
		float x,y;
		x = centerss[asso_k -1].at<float>(i,0);
		y = centerss[asso_k -1].at<float>(i,1);
		Point centerspoint;
		centerspoint.x = x;
		centerspoint.y = y;
		circle(cameraFeed,centerspoint,10,(0,255,0));
		}
	return centerss[asso_k -1];
}

Mat trackFilteredObject2(Mat threshold,Mat HSV, Mat &cameraFeed)
{
	Mat temp;
	threshold.copyTo(temp);
	vector <int> pointsvec ;
	//cout<<threshold.rows<<","<<threshold.cols<<endl;
	Mat label;
	
	for(int i = 0; i <threshold.rows; i++){
		//const unsigned int* row = threshold.ptr<unsigned int>(i);
		for(int j = 0; j < threshold.cols; j++)
		{
			if(threshold.at<uchar>(i, j) == 255)
			{
				pointsvec.push_back(i);
				pointsvec.push_back(j);

			}
		}
	}
	//for(int i = 0; i < pointsvec.size(); i++)
	//{
	//if(i % 2 == 0)
	//	cout << "(" << pointsvec[i];
	//else
	//	cout << "," << pointsvec[i] << ")\n";
	//}//測試存取ｍａｔ程式碼
	
	int matsize = pointsvec.size()/2;
	Mat points(matsize,1,CV_32FC2);
	for(int i=0; i<matsize; i++)
		for(int j=0; j<2; j++){

   points.at<Vec2f>(i)[0] = pointsvec[i*2+1];

   points.at<Vec2f>(i)[1] = pointsvec[i*2];

  }
		
	vector <Mat> centerss;
	vector <double> SSEs;
	vector <double> SSEslopes;
	vector <double> slopedivides;
	centerss.reserve(kmax);
	SSEs.reserve(kmax);
	SSEslopes.reserve(kmax);
	slopedivides.reserve(kmax);
	double SSEdivide_max = DBL_MIN;
	double testdou;
	int asso_k = 1;
	for(int i = 0; i < kmax;i++)
	{
		Mat temp(i+1, 1, threshold.type());
		centerss.push_back(temp);
	}
	
	for(int j = 0; j < kmax; j++)
	{
	kmeans(points, j+1,label,
              TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 20, 1.0),
              2, KMEANS_PP_CENTERS, centerss[j]);
	SSEs.push_back(SSE(pointsvec,centerss[j]));	
	
	}

		if(SSEs[0] < 900000)
	{
	cout<<"centers"<<centerss[asso_k -1]<<endl;
	cout << "k = " << asso_k << endl;

	for(int i = 0; i < asso_k;i++)
	{
		float x,y;
		x = centerss[asso_k -1].at<float>(i,0);
		y = centerss[asso_k -1].at<float>(i,1);
		Point centerspoint;
		centerspoint.x = x;
		centerspoint.y = y;
		circle(cameraFeed,centerspoint,10,(0,255,0));
	}

	return centerss[asso_k -1];
	}


	for(int k=0;k<kmax-1;k++)
	{
		SSEslopes.push_back(SSEs[k]-SSEs[k+1]);
		cout<<"k="<<k<<"slope"<<SSEslopes[k]<<endl;
	}
	for(int i=0;i<kmax-2;i++)
	{
		slopedivides.push_back(SSEslopes[i]/SSEslopes[i+1]);
		if(slopedivides[i] > SSEdivide_max)
		{
		SSEdivide_max = slopedivides[i];
		asso_k = i + 2;
		}
	}



	//cout<<"points"<<points<<endl;

	cout<<"centers"<<centerss[asso_k -1]<<endl;
	cout << "k = " << asso_k << endl;

	for(int i = 0; i < asso_k;i++)
	{
		float x,y;
		x = centerss[asso_k -1].at<float>(i,0);
		y = centerss[asso_k -1].at<float>(i,1);
		Point centerspoint;
		centerspoint.x = x;
		centerspoint.y = y;
		circle(cameraFeed,centerspoint,10,(0,255,0));
	}
		return centerss[asso_k -1];
}

Mat trackFilteredObject3(Mat threshold,Mat HSV, Mat &cameraFeed)
{
	Mat temp;
	threshold.copyTo(temp);
	vector <int> pointsvec ;
	//cout<<threshold.rows<<","<<threshold.cols<<endl;
	Mat label;
	
	for(int i = 0; i <threshold.rows; i++){
		//const unsigned int* row = threshold.ptr<unsigned int>(i);
		for(int j = 0; j < threshold.cols; j++)
		{
			if(threshold.at<uchar>(i, j) == 255)
			{
				pointsvec.push_back(i);
				pointsvec.push_back(j);

			}
		}
	}
	//for(int i = 0; i < pointsvec.size(); i++)
	//{
	//if(i % 2 == 0)
	//	cout << "(" << pointsvec[i];
	//else
	//	cout << "," << pointsvec[i] << ")\n";
	//}//測試存取ｍａｔ程式碼
	
	int matsize = pointsvec.size()/2;
	Mat points(matsize,1,CV_32FC2);
	for(int i=0; i<matsize; i++)
		for(int j=0; j<2; j++){

   points.at<Vec2f>(i)[0] = pointsvec[i*2+1];

   points.at<Vec2f>(i)[1] = pointsvec[i*2];

  }
		
	vector <Mat> centerss;
	vector <double> SSEs;
	vector <double> SSEslopes;
	vector <double> slopedivides;
	centerss.reserve(kmax);
	SSEs.reserve(kmax);
	SSEslopes.reserve(kmax);
	slopedivides.reserve(kmax);
	double SSEdivide_max = DBL_MIN;
	double testdou;
	int asso_k = 1;
	for(int i = 0; i < kmax;i++)
	{
		Mat temp(i+1, 1, threshold.type());
		centerss.push_back(temp);
	}
	
	for(int j = 0; j < kmax; j++)
	{
	kmeans(points, j+1,label,
              TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 20, 1.0),
              2, KMEANS_PP_CENTERS, centerss[j]);
	SSEs.push_back(SSE(pointsvec,centerss[j]));	
	
	}

	if(SSEs[0] < 900000)
	{
		cout<<"centers"<<centerss[asso_k -1]<<endl;
	cout << "k = " << asso_k << endl;

	for(int i = 0; i < asso_k;i++)
	{
		float x,y;
		x = centerss[asso_k -1].at<float>(i,0);
		y = centerss[asso_k -1].at<float>(i,1);
		Point centerspoint;
		centerspoint.x = x;
		centerspoint.y = y;
		circle(cameraFeed,centerspoint,10,(0,255,0));
	}

	return centerss[asso_k -1];
	}

	for(int k=0;k<kmax-1;k++)
	{
		SSEslopes.push_back(SSEs[k]-SSEs[k+1]);
		cout<<"k="<<k<<"slope"<<SSEslopes[k]<<endl;
	}
	for(int i=0;i<kmax-2;i++)
	{
		slopedivides.push_back(SSEslopes[i]/SSEslopes[i+1]);
		if(slopedivides[i] > SSEdivide_max)
		{
		SSEdivide_max = slopedivides[i];
		asso_k = i + 2;
		}
	}



	//cout<<"points"<<points<<endl;

	cout<<"centers"<<centerss[asso_k -1]<<endl;
	cout << "k = " << asso_k << endl;

	for(int i = 0; i < asso_k;i++)
	{
		float x,y;
		x = centerss[asso_k -1].at<float>(i,0);
		y = centerss[asso_k -1].at<float>(i,1);
		Point centerspoint;
		centerspoint.x = x;
		centerspoint.y = y;
		circle(cameraFeed,centerspoint,10,(0,255,0));
	}

		return centerss[asso_k -1];
}//new

Mat trackFilteredObject4(Mat threshold,Mat HSV, Mat &cameraFeed)
{
	Mat temp;
	threshold.copyTo(temp);
	vector <int> pointsvec ;
	//cout<<threshold.rows<<","<<threshold.cols<<endl;
	Mat label;
	
	for(int i = 0; i <threshold.rows; i++){
		//const unsigned int* row = threshold.ptr<unsigned int>(i);
		for(int j = 0; j < threshold.cols; j++)
		{
			if(threshold.at<uchar>(i, j) == 255)
			{
				pointsvec.push_back(i);
				pointsvec.push_back(j);

			}
		}
	}
	//for(int i = 0; i < pointsvec.size(); i++)
	//{
	//if(i % 2 == 0)
	//	cout << "(" << pointsvec[i];
	//else
	//	cout << "," << pointsvec[i] << ")\n";
	//}//測試存取ｍａｔ程式碼
	
	int matsize = pointsvec.size()/2;
	Mat points(matsize,1,CV_32FC2);
	for(int i=0; i<matsize; i++)
		for(int j=0; j<2; j++){

   points.at<Vec2f>(i)[0] = pointsvec[i*2+1];

   points.at<Vec2f>(i)[1] = pointsvec[i*2];

  }
		
	vector <Mat> centerss;
	vector <double> SSEs;
	vector <double> SSEslopes;
	vector <double> slopedivides;
	centerss.reserve(kmax);
	SSEs.reserve(kmax);
	SSEslopes.reserve(kmax);
	slopedivides.reserve(kmax);
	double SSEdivide_max = DBL_MIN;
	double testdou;
	int asso_k = 1;
	for(int i = 0; i < kmax;i++)
	{
		Mat temp(i+1, 1, threshold.type());
		centerss.push_back(temp);
	}
	
	for(int j = 0; j < kmax; j++)
	{
	kmeans(points, j+1,label,
              TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 20, 1.0),
              2, KMEANS_PP_CENTERS, centerss[j]);
	SSEs.push_back(SSE(pointsvec,centerss[j]));	
	
	}

	if(SSEs[0] < 900000)
	{
		cout<<"centers"<<centerss[asso_k -1]<<endl;
	cout << "k = " << asso_k << endl;

	for(int i = 0; i < asso_k;i++)
	{
		float x,y;
		x = centerss[asso_k -1].at<float>(i,0);
		y = centerss[asso_k -1].at<float>(i,1);
		Point centerspoint;
		centerspoint.x = x;
		centerspoint.y = y;
		circle(cameraFeed,centerspoint,10,(0,255,0));
	}

	return centerss[asso_k -1];
	}

	for(int k=0;k<kmax-1;k++)
	{
		SSEslopes.push_back(SSEs[k]-SSEs[k+1]);
		cout<<"k="<<k<<"slope"<<SSEslopes[k]<<endl;
	}
	for(int i=0;i<kmax-2;i++)
	{
		slopedivides.push_back(SSEslopes[i]/SSEslopes[i+1]);
		if(slopedivides[i] > SSEdivide_max)
		{
		SSEdivide_max = slopedivides[i];
		asso_k = i + 2;
		}
	}



	//cout<<"points"<<points<<endl;

	cout<<"centers"<<centerss[asso_k -1]<<endl;
	cout << "k = " << asso_k << endl;

	for(int i = 0; i < asso_k;i++)
	{
		float x,y;
		x = centerss[asso_k -1].at<float>(i,0);
		y = centerss[asso_k -1].at<float>(i,1);
		Point centerspoint;
		centerspoint.x = x;
		centerspoint.y = y;
		circle(cameraFeed,centerspoint,10,(0,255,0));
	}
		return centerss[asso_k -1];
}//new2

Mat trackFilteredObject5(Mat threshold,Mat HSV, Mat &cameraFeed)
{
	Mat temp;
	threshold.copyTo(temp);
	vector <int> pointsvec ;
	//cout<<threshold.rows<<","<<threshold.cols<<endl;
	Mat label;
	
	for(int i = 0; i <threshold.rows; i++){
		//const unsigned int* row = threshold.ptr<unsigned int>(i);
		for(int j = 0; j < threshold.cols; j++)
		{
			if(threshold.at<uchar>(i, j) == 255)
			{
				pointsvec.push_back(i);
				pointsvec.push_back(j);

			}
		}
	}
	//for(int i = 0; i < pointsvec.size(); i++)
	//{
	//if(i % 2 == 0)
	//	cout << "(" << pointsvec[i];
	//else
	//	cout << "," << pointsvec[i] << ")\n";
	//}//測試存取ｍａｔ程式碼
	
	int matsize = pointsvec.size()/2;
	Mat points(matsize,1,CV_32FC2);
	for(int i=0; i<matsize; i++)
		for(int j=0; j<2; j++){

   points.at<Vec2f>(i)[0] = pointsvec[i*2+1];

   points.at<Vec2f>(i)[1] = pointsvec[i*2];

  }
		
	vector <Mat> centerss;
	vector <double> SSEs;
	vector <double> SSEslopes;
	vector <double> slopedivides;
	centerss.reserve(kmax);
	SSEs.reserve(kmax);
	SSEslopes.reserve(kmax);
	slopedivides.reserve(kmax);
	double SSEdivide_max = DBL_MIN;
	double testdou;
	int asso_k = 1;
	for(int i = 0; i < kmax;i++)
	{
		Mat temp(i+1, 1, threshold.type());
		centerss.push_back(temp);
	}
	
	for(int j = 0; j < kmax; j++)
	{
	kmeans(points, j+1,label,
              TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 20, 1.0),
              2, KMEANS_PP_CENTERS, centerss[j]);
	SSEs.push_back(SSE(pointsvec,centerss[j]));	
	
	}

	if(SSEs[0] < 900000)
	{
		cout<<"centers"<<centerss[asso_k -1]<<endl;
	cout << "k = " << asso_k << endl;

	for(int i = 0; i < asso_k;i++)
	{
		float x,y;
		x = centerss[asso_k -1].at<float>(i,0);
		y = centerss[asso_k -1].at<float>(i,1);
		Point centerspoint;
		centerspoint.x = x;
		centerspoint.y = y;
		circle(cameraFeed,centerspoint,10,(0,255,0));
	}

	return centerss[asso_k -1];
	}

	for(int k=0;k<kmax-1;k++)
	{
		SSEslopes.push_back(SSEs[k]-SSEs[k+1]);
		cout<<"k="<<k<<"slope"<<SSEslopes[k]<<endl;
	}
	for(int i=0;i<kmax-2;i++)
	{
		slopedivides.push_back(SSEslopes[i]/SSEslopes[i+1]);
		if(slopedivides[i] > SSEdivide_max)
		{
		SSEdivide_max = slopedivides[i];
		asso_k = i + 2;
		}
	}



	//cout<<"points"<<points<<endl;

	cout<<"centers"<<centerss[asso_k -1]<<endl;
	cout << "k = " << asso_k << endl;

	for(int i = 0; i < asso_k;i++)
	{
		float x,y;
		x = centerss[asso_k -1].at<float>(i,0);
		y = centerss[asso_k -1].at<float>(i,1);
		Point centerspoint;
		centerspoint.x = x;
		centerspoint.y = y;
		circle(cameraFeed,centerspoint,10,(0,255,0));
	}
		return centerss[asso_k -1];
}//new3

Mat trackFilteredObject6(Mat threshold,Mat HSV, Mat &cameraFeed)
{
	Mat temp;
	threshold.copyTo(temp);
	vector <int> pointsvec ;
	//cout<<threshold.rows<<","<<threshold.cols<<endl;
	Mat label;
	
	for(int i = 0; i <threshold.rows; i++){
		//const unsigned int* row = threshold.ptr<unsigned int>(i);
		for(int j = 0; j < threshold.cols; j++)
		{
			if(threshold.at<uchar>(i, j) == 255)
			{
				pointsvec.push_back(i);
				pointsvec.push_back(j);

			}
		}
	}
	//for(int i = 0; i < pointsvec.size(); i++)
	//{
	//if(i % 2 == 0)
	//	cout << "(" << pointsvec[i];
	//else
	//	cout << "," << pointsvec[i] << ")\n";
	//}//測試存取ｍａｔ程式碼
	
	int matsize = pointsvec.size()/2;
	Mat points(matsize,1,CV_32FC2);
	for(int i=0; i<matsize; i++)
		for(int j=0; j<2; j++){

   points.at<Vec2f>(i)[0] = pointsvec[i*2+1];

   points.at<Vec2f>(i)[1] = pointsvec[i*2];

  }
		
	vector <Mat> centerss;
	vector <double> SSEs;
	vector <double> SSEslopes;
	vector <double> slopedivides;
	centerss.reserve(kmax);
	SSEs.reserve(kmax);
	SSEslopes.reserve(kmax);
	slopedivides.reserve(kmax);
	double SSEdivide_max = DBL_MIN;
	double testdou;
	int asso_k = 1;
	for(int i = 0; i < kmax;i++)
	{
		Mat temp(i+1, 1, threshold.type());
		centerss.push_back(temp);
	}
	
	for(int j = 0; j < kmax; j++)
	{
	kmeans(points, j+1,label,
              TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 20, 1.0),
              2, KMEANS_PP_CENTERS, centerss[j]);
	SSEs.push_back(SSE(pointsvec,centerss[j]));	
	
	}

	if(SSEs[0] < 900000)
	{
	cout<<"centers"<<centerss[asso_k -1]<<endl;
	cout << "k = " << asso_k << endl;

	for(int i = 0; i < asso_k;i++)
	{
		float x,y;
		x = centerss[asso_k -1].at<float>(i,0);
		y = centerss[asso_k -1].at<float>(i,1);
		Point centerspoint;
		centerspoint.x = x;
		centerspoint.y = y;
		circle(cameraFeed,centerspoint,10,(0,255,0));
	}

	return centerss[asso_k -1];
	}

	for(int k=0;k<kmax-1;k++)
	{
		SSEslopes.push_back(SSEs[k]-SSEs[k+1]);
		cout<<"k="<<k<<"slope"<<SSEslopes[k]<<endl;
	}
	for(int i=0;i<kmax-2;i++)
	{
		slopedivides.push_back(SSEslopes[i]/SSEslopes[i+1]);
		if(slopedivides[i] > SSEdivide_max)
		{
		SSEdivide_max = slopedivides[i];
		asso_k = i + 2;
		}
	}



	//cout<<"points"<<points<<endl;

	cout<<"centers"<<centerss[asso_k -1]<<endl;
	cout << "k = " << asso_k << endl;

	for(int i = 0; i < asso_k;i++)
	{
		float x,y;
		x = centerss[asso_k -1].at<float>(i,0);
		y = centerss[asso_k -1].at<float>(i,1);
		Point centerspoint;
		centerspoint.x = x;
		centerspoint.y = y;
		circle(cameraFeed,centerspoint,10,(0,255,0));
	}
		return centerss[asso_k -1];
}//new4


int main(int argc, char* argv[])
{
	//if we would like to calibrate our filter values, set to true.
	bool calibrationMode = true;

	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	Mat threshold;
	Mat HSV;

	
	Mat threshold2;
	Mat HSV2;
	
	Mat threshold3;
	Mat HSV3;//new

	Mat threshold4;
	Mat HSV4;//new2

	Mat threshold5;
	Mat HSV5;//new3

	Mat threshold6;
	Mat HSV6;//new4	


	if(calibrationMode){
		//create slider bars for HSV filtering
		//createTrackbars();
		//createTrackbars2();
		//createTrackbars3();//new
		//createTrackbars4();//new2
		//createTrackbars5();//new3
		//createTrackbars6();//new4
	}
	
	CvCapture* capture;// = cvCreateCameraCapture( 0 );
  //assert( capture );
  board_w  = 7;
  board_h  = 4;
  n_boards = 10;
  board_dt = 20;
  
  

  char camera[] = "http://root:hellokitty@10.0.0.100/axis-cgi/mjpg/video.cgi?resolution=640x480&req_fps=30&.mjpg";
  int board_n  = board_w * board_h;// 格子數=寬*高
  int board_nx  = (board_w-1) * (board_h-1);// 內部角個數=(寬-1)*(高-1)
  CvSize board_sz = cvSize( board_w-1, board_h-1 );// 內部row和column的角個數
  capture = cvCaptureFromFile(camera);
  if(!capture) {
    printf("\nCouldn't open the camera\n");
    system("pause");
    return -1;
  }

  //ALLOCATE STORAGE
  CvMat* image_points      = cvCreateMat(n_boards*board_n,2,CV_32FC1);
  CvMat* object_points     = cvCreateMat(n_boards*board_n,3,CV_32FC1);
  CvMat* point_counts      = cvCreateMat(n_boards,1,CV_32SC1);
  CvMat* intrinsic_matrix  = cvCreateMat(3,3,CV_32FC1);
  CvMat* distortion_coeffs = cvCreateMat(4,1,CV_32FC1);
 
  CvPoint2D32f* corners = new CvPoint2D32f[ board_n ];
  int corner_count;
  int successes = 0;
  int step, frame = 0;
  
  IplImage *image = cvQueryFrame( capture );
  IplImage *gray_image = cvCreateImage(cvGetSize(image),8,1);//subpixel
  // CAPTURE CORNER VIEWS LOOP UNTIL WE?E GOT n_boards
  // SUCCESSFUL CAPTURES (ALL CORNERS ON THE BOARD ARE FOUND)
  int countx=0;// 擷取次數
  
 
  // EXAMPLE OF LOADING THESE MATRICES BACK IN:
  CvMat *intrinsic = (CvMat*)cvLoad("Intrinsics.xml");
  CvMat *distortion = (CvMat*)cvLoad("Distortion.xml");
 
  // Build the undistort map which we will use for all
  // subsequent frames.
  IplImage* mapx = cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1 );
  IplImage* mapy = cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1 );
  cvInitUndistortMap(
    intrinsic,
    distortion,
    mapx,
    mapy
  );
  // Just run the camera to the screen, now showing the raw and
  // the undistorted image.
   IplImage *r = cvCreateImage(cvGetSize(image),8,1);//subpixel
   IplImage *g = cvCreateImage(cvGetSize(image),8,1);//subpixel
   IplImage *b = cvCreateImage(cvGetSize(image),8,1);//subpixel
 
 
  int iLastX1 = -1; 
  int iLastY1 = -1;
  int iLastX2 = -1; 
  int iLastY2 = -1;
  int iLastX3 = -1; 
  int iLastY3 = -1;

   

  while(1) {

	  	framCnt++;
			
       if(framCnt>3){

	   framCnt = 0;
           
			
			imwrite("C:\\Users\\User\\Documents\\Visual Studio 2010\\Projects\\test\\images\\start.jpg", cameraFeed);

			//break; 
			//system("pause");
			
		}
	 
	

       cvSplit(image, r,g,b, NULL);
       cvRemap( r, r, mapx, mapy ); // Undistort image
       cvRemap( g, g, mapx, mapy ); // Undistort image
       cvRemap( b, b, mapx, mapy ); // Undistort image
       cvMerge(r,g,b, NULL, image);
       cvShowImage("Undistort image", image);     // Show corrected image
    //Handle pause/unpause and ESC
 
    
  
		//store image to matrix
		cv::Mat cameraFeed = cv::cvarrToMat(image);

		src = cameraFeed;


	
  		if( !src.data )
  		{ return -1; }

		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		cvtColor(cameraFeed,HSV2,COLOR_BGR2HSV);
		cvtColor(cameraFeed,HSV3,COLOR_BGR2HSV);//new
		cvtColor(cameraFeed,HSV4,COLOR_BGR2HSV);//new2
		cvtColor(cameraFeed,HSV5,COLOR_BGR2HSV);//new3
		cvtColor(cameraFeed,HSV6,COLOR_BGR2HSV);//new4
		if(calibrationMode==true){

		//need to find the appropriate color range values
		// calibrationMode must be false

		//if in calibration mode, we track objects based on the HSV slider values.
			cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
			inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);//二值化
			morphOps(threshold);//型態處理
			imshow(windowName2,threshold);

			ofstream hode;
			hode.open("hode.txt");
			hode<<threshold<<endl;
			

			cvtColor(cameraFeed,HSV2,COLOR_BGR2HSV);
			inRange(HSV2,Scalar(H_MIN2,S_MIN2,V_MIN2),Scalar(H_MAX2,S_MAX2,V_MAX2),threshold2);
			morphOps(threshold2);
			imshow(windowName4,threshold2);

			cvtColor(cameraFeed,HSV3,COLOR_BGR2HSV);
			inRange(HSV3,Scalar(H_MIN3,S_MIN3,V_MIN3),Scalar(H_MAX3,S_MAX3,V_MAX3),threshold3);
			morphOps(threshold3);
			//imshow(windowName6,threshold3);//new

			cvtColor(cameraFeed,HSV4,COLOR_BGR2HSV);
			inRange(HSV4,Scalar(H_MIN4,S_MIN4,V_MIN4),Scalar(H_MAX4,S_MAX4,V_MAX4),threshold4);
			morphOps(threshold4);
			//imshow(windowName7,threshold4);//new2
			
			cvtColor(cameraFeed,HSV5,COLOR_BGR2HSV);
			inRange(HSV5,Scalar(H_MIN5,S_MIN5,V_MIN5),Scalar(H_MAX5,S_MAX5,V_MAX5),threshold5);
			morphOps(threshold5);
			//imshow(windowName8,threshold5);//new3

			cvtColor(cameraFeed,HSV6,COLOR_BGR2HSV);
			inRange(HSV6,Scalar(H_MIN6,S_MIN6,V_MIN6),Scalar(H_MAX6,S_MAX6,V_MAX6),threshold6);
			morphOps(threshold6);
			//imshow(windowName9,threshold6);//new4
      
  		

		//the folowing for canny edge detec
			/// Create a matrix of the same type and size as src (for dst)
	  		dst.create( src.size(), src.type() );
	  		/// Convert the image to grayscale
	  		cvtColor( src, src_gray, CV_BGR2GRAY );
	  		/// Create a window
	  		namedWindow( window_name, CV_WINDOW_AUTOSIZE );
	  		/// Create a Trackbar for user to enter threshold
	  		createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold);
	  		/// Show the image

			Mat centers_red;
			Mat centers_green;
			Mat centers_blue;
			Mat centers_yellow;
			Mat centers_purple;
			Mat centers_white;

			centers_red =  trackFilteredObject(threshold,HSV,cameraFeed);
			centers_green=trackFilteredObject2(threshold2,HSV2,cameraFeed);
		    centers_blue=trackFilteredObject3(threshold3,HSV3,cameraFeed);//new
			centers_yellow=trackFilteredObject4(threshold4,HSV4,cameraFeed);//new2
			centers_purple=trackFilteredObject5(threshold5,HSV5,cameraFeed);//new3
			centers_white=trackFilteredObject6(threshold6,HSV6,cameraFeed);//new4

			//cout<<"redcenter"<<centers_red<<"size"<<centers_red.rows<<endl;
            

		float rx,ry,gx,gy;
		for(int i = 0; i < centers_red.rows;i++)
	    {
		float rxtemp,rytemp,gxtemp,gytemp;
		rxtemp = centers_red.at<float>(i,0);
		rytemp = centers_red.at<float>(i,1);
		for(int j=0;j<centers_green.rows;j++){
		gxtemp = centers_green.at<float>(j,0);
		gytemp = centers_green.at<float>(j,1);
		if(pow(rxtemp - gxtemp,2)+pow(rytemp-gytemp,2) <1500 && pow(rxtemp - gxtemp,2)+pow(rytemp-gytemp,2) > 40)
		{
			rx = rxtemp;
			ry = rytemp;
			gx = gxtemp;
			gy = gytemp;
			break;
		}
		}
	   } 
		
		float bx,by,yx,yy;
		for(int i = 0; i < centers_blue.rows;i++)
	    {
		float bxtemp,bytemp,yxtemp,yytemp;
		bxtemp = centers_blue.at<float>(i,0);
		bytemp = centers_blue.at<float>(i,1);
		for(int j=0;j<centers_yellow.rows;j++){
		yxtemp = centers_yellow.at<float>(j,0);
		yytemp = centers_yellow.at<float>(j,1);
		if(pow(bxtemp - yxtemp,2)+pow(bytemp-yytemp,2) < 1500 && pow(bxtemp - yxtemp,2)+pow(bytemp-yytemp,2) > 40)
		{
			bx = bxtemp;
			by = bytemp;
			yx = yxtemp;
			yy = yytemp;
			break;
		}
		}
	   } 

		float px,py,wx,wy;
		for(int i = 0; i < centers_purple.rows;i++)
	    {
		float pxtemp,pytemp,wxtemp,wytemp;
		pxtemp = centers_purple.at<float>(i,0);
		pytemp = centers_purple.at<float>(i,1);
		for(int j=0;j<centers_white.rows;j++){
		wxtemp =centers_white.at<float>(j,0);
		wytemp = centers_white.at<float>(j,1);
		if(pow(pxtemp - wxtemp,2)+pow(pytemp-wytemp,2) < 1500 && pow(pxtemp - wxtemp,2)+pow(pytemp-wytemp,2) > 40)
		{
			px = pxtemp;
			py = pytemp;
			wx = wxtemp;
			wy = wytemp;
			break;
		}
		}
	   } 
		
		    cout<<"car0向量為"<<rx-gx<<","<<ry-gy<<endl;
			cout<<"car1向量為"<<bx-yx<<","<<by-yy<<endl;
			cout<<"car2向量為"<<px-wx<<","<<py-wy<<endl;

		
		
			cout<<"car0姿態向量為"<<(rx-gx)/sqrt(pow(rx- gx,2)+pow(ry-gy,2))<<","<<(ry-gy)/sqrt(pow(rx- gx,2)+pow(ry-gy,2))<<endl;
			cout<<"car1姿態向量為"<<(bx-yx)/sqrt(pow(bx- yx,2)+pow(by-yy,2))<<","<<(by-yy)/sqrt(pow(bx- yx,2)+pow(by-yy,2))<<endl;
			cout<<"car2姿態向量為"<<(px-wx)/sqrt(pow(px- wx,2)+pow(py-wy,2))<<","<<(py-wy)/sqrt(pow(px- wx,2)+pow(py-wy,2))<<endl;

imshow("result image",cameraFeed);

}
		 image = cvQueryFrame( capture );   //防止校正發散的指令

		waitKey(0);
	}}
