#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<iostream>
using namespace std;
using namespace cv;
VideoCapture cap(0);
Mat img, imgHSV;
vector<vector<int>> newpoints;

vector<vector<int>> myColors
{ {54,33,0,99,117,189} //green
,{72,97,170,169,255,255} };//blue
vector<Scalar>mycolorsvalue{ {0,255,0},{255,0,0} };
Point getcontours(Mat imgdil)
{
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;

	findContours(imgdil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	/*drawContours(img, contours, -1, Scalar(0, 0, 0), 5);*/
	vector<vector<Point>>conpoly(contours.size());
	vector<Rect>boundrect(contours.size());
	Point mypoint(0, 0);
	int area;

	for (int i = 0; i < contours.size(); i++)
	{
		area = contourArea(contours[i]);
		cout << area << endl;
		if (area > 1000)
		{
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conpoly[i], 0.02 * peri, true);
			cout << conpoly[i].size() << endl;
			boundrect[i] = boundingRect(conpoly[i]);
			mypoint.x = boundrect[i].x + boundrect[i].width / 2;
			mypoint.y = boundrect[i].y;
			
			drawContours(img, conpoly, i, Scalar(255, 0, 255), 2);
			rectangle(img, boundrect[i].tl(), boundrect[i].br(), Scalar(0, 255, 0), 5);
		}


	}

	return mypoint;


}




vector<vector<int>> findcolor(Mat img)
{
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	for (int i = 0; i < myColors.size(); i++)
	{
		Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
		Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
		Mat mask;
		inRange(imgHSV, lower, upper, mask);
		/*imshow(to_string(i), mask);*/

		Point mypoint = getcontours(mask);
		if (mypoint.x != 0 && mypoint.y != 0)
		{
			newpoints.push_back({mypoint.x, mypoint.y, i});

		}

	}
	return newpoints;
}

void drawoncanvas(vector<vector<int>> newpoints, vector<Scalar>mycolorsvalue)
{
	for (int i = 0; i < newpoints.size(); i++)
	{
		circle(img, Point(newpoints[i][0], newpoints[i][1]), 8 , mycolorsvalue[newpoints[i][2]], FILLED);
	}
}
int main()
{
	while (true)
	{
		cap.read(img);
		newpoints=findcolor(img);
		drawoncanvas(newpoints, mycolorsvalue);
		resize(img, img, Size(), 1.8, 1.7);
		imshow("window",img);
			waitKey(1);
	}
}