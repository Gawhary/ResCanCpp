#pragma once


#include <excpt.h>
#include <iostream>
#include <vector>
#include <string>

#include "opencv/highgui.h"

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"

class ResCan
{

	//! fill Vec3b BGR color in RGB order for convenient
	inline cv::Vec3b RGB(uchar r, uchar g, uchar b){
		return cv::Vec3b(b, g, r);
	}

public:
	ResCan(void); 
	~ResCan(void);

	//! process frame and return resistor value(with unit) as string 
	/*!
	  \param image captured image
	  \return detected value or empty string
	*/
	cv::string doMagic(cv::Mat image);



	
	//! shows processing image on openCV window
	//! used only in console application
	inline void showResult(cv::Mat img, const char* title = "result", bool waitForKey = true){
	//static bool firstCall = true;
	//if (firstCall){
		cv::namedWindow(title, CV_WINDOW_NORMAL);	// Create a window for display.
		cv::resizeWindow(title, 500, 500);
	//	firstCall = false;
	//}
	cv::imshow(title, img);						// Show our image inside it.
	cv::waitKey(waitForKey? 0:1);					// Wait for a keystroke in the window

}
	

	
private:

	//! helper function	
	inline double coldist2(const cv::Vec3b* c1, const cv::Vec3b* c2) {
		return cv::pow((float)c1->val[0] - c2->val[0], 2)
			+ cv::pow((float)c1->val[1] - c2->val[1], 2)
			+ cv::pow((float)c1->val[2] - c2->val[2], 2);
	}

	//! converts Vec3b to Scalar
	inline cv::Scalar Vec3b2Scalar(const cv::Vec3b& v){
		return cv::Scalar(v[0], v[1], v[2]);
	}
	//! converts Scalar to Vec3b
	inline cv::Vec3b Scalar2Vec3b(const cv::Scalar& v){
		return cv::Vec3b((uchar)v[0], (uchar)v[1], (uchar)v[2]);
	}

	//! helper function	
	//! calculates resistor value given the three lines values  
	inline double calcvalue(const std::vector<int> rings){

	double result = rings[0] * 10 + rings[1];
	result *= cv::pow(10.0, rings[2]);

	return result;
}
	
	//! detects risistor value from risestor major segment image.
	/*!
	  \param image The whole working image to draw result on it.
	  \param res The effctive image segment used in detection.
	  \return detected value string or empty string in case of failure.
	*/
	cv::string detect(cv::Mat image, cv::Mat res);
	
private:
	
	int	contfound; //! number of continous valid frames generates the same (current) value
	std::vector<int> lastresult; //! previous detection result

	std::vector<cv::Vec3b>	codes; //! list of color codes
	std::vector<cv::Vec3b>	codesview; //! list of color codes
	std::vector<cv::string>	codename;//! list of color names




};

