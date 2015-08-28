// ResCanCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ResCan.h"

#include <excpt.h>
#include <iostream>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"

#include <windows.h>

#include"HiResTimer.h"

using namespace std;
using namespace cv;



int main(int argc, char* argv[])
{

	cv::Mat frame;
	cv::Mat org;
	ResCan resCan;
	cv::string valuestring;

	if (argc == 2){
		org = cv::imread(argv[1]);
		if (org.data){	
			resCan.showResult(org, "original", false);
			for (int i = 0; i < 10; ++i){
				cv::flip(org, frame, -1);
				resCan.doMagic(frame);
			}
			resCan.showResult(frame);
			cout << "Press any key to continue";
			return 0;
		}
	}
	//try{
		cv::VideoCapture camera;
		camera.open(0); // Useless
		if (!camera.isOpened()) {
			cout << ("Camera Error");
			return -1;
		}
		else {
			cout << ("Camera OK?");
		}

		
		//camera.retrieve(frame);
		//org = frame.clone();

		CHiResTimer timer;
		timer.Init();
		int msec;


		while (camera.isOpened()) {
			 //cout << ("Frame Grabbed");
			 camera.retrieve(frame);
			 if(!frame.data)
				 continue;
			 //cout << ("Frame Decoded");
			cv::flip(frame, frame, -1);

			/*
			 * No difference camera.release();
			 */

			// System.out.println("Captured Frame Width " + frame.width());

			 // copy captured frame
			 //frame.copyTo(org); // ror debugging
//			 cv::imshow("cam", frame);						// Show our image inside it.
			 //if (waitKey(1) >= 0)
			 {											 // Wait for a keystroke in the window
				 //try{
				 timer.GetElapsedMilliSeconds();

				 //for(int i = 0; i < 10; ++i)
					valuestring = resCan.doMagic(frame);
					
				 msec = timer.GetElapsedMilliSeconds();
				 if(msec > 0)
					cout << "FPS: " << 1000/msec;

				 //}catch(...)
				 //{
					////resCan.doMagic(org); // for debugging
					// continue;
				 //}
					
				cout << (valuestring) << endl;
				 resCan.showResult(frame, "result", false);
			 }

			/*
			 * try { Thread.sleep(10); } catch (InterruptedException e) { //
			 * TODO Auto-generated catch block e.printStackTrace(); }
			 */
		}
	//}
	//catch (const cv::Exception& ex)
	//{
	//	std::cout << "Error: " << ex.what() << std::endl;
	//}
	//catch (...){
	//	std::cout << "Error: Unhandled Exception";
	//}
	return 0;
}

