#include <opencv2/core.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;

int main () {
	cv::Mat inputFrame, frame, back, foregroundMask;
	cv::VideoCapture cap;
	cap.open(0);

	Ptr<BackgroundSubtractor> bg = createBackgroundSubtractorMOG2(); 

	vector<vector<Point>> contours;
	namedWindow ("Frame");

	for (;;){
		cap >> inputFrame;
		if (inputFrame.empty()) {
		    cout << "Finished reading: empty frame" << endl;
		    break;
		}
		const Size scaledSize(640, 640 * inputFrame.rows / inputFrame.cols);
		resize(inputFrame, frame, scaledSize, 0, 0, INTER_LINEAR);

		bg->apply(frame, foregroundMask, -1);

		cv::erode(foregroundMask, foregroundMask, cv::Mat(), Point(-1, -1), 5);
		cv::dilate(foregroundMask, foregroundMask, cv::Mat (), Point(-1, -1), 5);

		findContours(foregroundMask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
		drawContours(frame, contours, -1, Scalar (255, 255, 255), 1);

		int a=0;
		vector<Rect> boundRect( contours.size() );
		for(unsigned int i = 0; i < contours.size(); i++ ) {
			boundRect[i] = boundingRect( contours[i] );
		}

		for(unsigned int i = 0; i< contours.size(); i++ ) {
			if(boundRect[i].width>=40 || boundRect[i].height>=40) {
				a=a+(boundRect[i].height)*(boundRect[i].width);
			}
			if((a>=int(frame.rows)*int(frame.cols)/3)) {
				putText(frame,"Tampering",Point(5,30),FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,255),2);
				std::cout<<"\a";
			}
		}

		imshow ("Frame", frame);

		char c = (char)waitKey(10);
		if( c == 27 )
			break;
	}
	return 1;
}
