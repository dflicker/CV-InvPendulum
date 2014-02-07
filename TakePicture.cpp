#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
	cout << "Invalid number of args" << endl;
	cout << "Usage: " << argv[0] << "path_to_output_file" << endl;
	return 1;
    }
    Mat frame;
    VideoCapture cam(0);
    cam >> frame;
    if (!frame.data) {
	cout << "Error: Image not found" << endl;
	return 1;
    }
    namedWindow("display", WINDOW_AUTOSIZE);
    for (;;) {
	cam >> frame;
	imshow("display", frame);
	imwrite(argv[1], frame);
	if (waitKey(1)>0) {
	    break;
	}
    }
    return 0;
}
