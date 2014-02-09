#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"

#include <sstream>
#include <iostream>
#include <vector>
#include <math.h>

using namespace cv;
using namespace std;
int main(int, char**)
{
    Mat frame;
    //frame = imread("two_dots.jpg");
    VideoCapture cam(0);
    cam >> frame;
    if (!frame.data) {
	cout << "Error image not found" << endl;
	return 1;
    }
    vector<Vec3f> old_circles(2, Vec3f(0, 0, 0));
    namedWindow("edges",1);
    int num_frames = 1;
    for (;;) {
	cam >> frame;
	Mat HSVFrame;
	cvtColor(frame, HSVFrame, CV_BGR2HSV);
	Mat PurpleFiltered;
	inRange(HSVFrame, Scalar(100, 10, 20), Scalar(160, 255, 200), PurpleFiltered);
	GaussianBlur(PurpleFiltered, PurpleFiltered, Size(9, 9), 2, 2);
	// PurpleFiltered is now a bitmask of purple in the original image
	vector<Vec3f> circles;
	vector<Vec3f> two_circles(2);
	HoughCircles(PurpleFiltered, circles, CV_HOUGH_GRADIENT, 3, PurpleFiltered.rows / 4, 500, 50, 20, PurpleFiltered.rows);
	int j = 0;
	for (size_t i = 0; i < circles.size(); i++) {
	    Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
	    int radius = cvRound(circles[i][2]);
	    if (radius > 20 && radius < 50) {
		// cout << "Circle radius: " << radius << endl;
		
		if (j > 1) break;
		two_circles[j] = circles[i];
		j++;
	    }
	}
	if (two_circles[0][1] > two_circles[1][1]) {
	    Vec3f swap = two_circles[1];
	    two_circles[1] = two_circles[0];
	    two_circles[0] = swap;
	}
	const int DIST = 10;
	if (norm(two_circles[0] - old_circles[0]) < DIST && norm(two_circles[1] - old_circles[1]) < DIST) {
	    for (size_t i = 0; i < two_circles.size(); i++) {
		continue;
	    }
	}
	const float alpha = 0.3;
	for (size_t i = 0; i < two_circles.size(); i++) {
	    old_circles[i] = (alpha * two_circles[i]) + (1.0 - alpha) * old_circles[i];
	    Point center(cvRound(old_circles[i][0]), cvRound(old_circles[i][1]));
	    int radius = cvRound(old_circles[i][2]);
	    circle(frame, center, 3, Scalar(0, 255, 0), -1, 8, 0);
	    circle(frame, center, radius, Scalar(0, 0, 255), 3, 8, 0);
	}
	num_frames++;
	Vec3f rod = old_circles[1] - old_circles[0];
	double angle = -atan2(rod[0], rod[1]) * 180.0 / 3.14159265;
	cout << "Angle: " << angle << endl;
	Mat res;
	// bitwise_and(frame, frame, res, PurpleFiltered);
	imshow("edges", frame);
	if(waitKey(1) > 0) {
	    break;
	}
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
