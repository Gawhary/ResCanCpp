#include "stdafx.h"
#include "ResCan.h"
#include <iomanip>

using namespace cv;
using namespace std;

ResCan::ResCan(void)
{
	codes.push_back(RGB( 20,	20,		20 ));
	codes.push_back(RGB( 71,	53,		38 )); // brown
	codes.push_back(RGB( 105,	31,		40 )); // red
	codes.push_back(RGB( 160,	90,		50 )); // orange
	codes.push_back(RGB( 157,	123,	39 )); // yellow
	codes.push_back(RGB( 41,	70,		46 )); // green
	codes.push_back(RGB( 40,	73,		86 )); // blue
	codes.push_back(RGB( 75,	55,		75 )); // violet
	codes.push_back(RGB( 73,	65,		62 )); // gray
	codes.push_back(RGB( 200,	200,	200 ));		// white

	codesview.push_back(RGB( 0,		0,		0 )); // black
	codesview.push_back(RGB( 139,	69,		19 )); // brown
	codesview.push_back(RGB( 255,	0,		0 )); // red
	codesview.push_back(RGB( 255,	128,	0 )); // orange
	codesview.push_back(RGB( 255,	255,	0 )); // yellow
	codesview.push_back(RGB( 0,		255,	0 )); // green
	codesview.push_back(RGB( 0,		0,		255 )); // blue
	codesview.push_back(RGB( 200,	0,		255 )); // violet
	codesview.push_back(RGB( 128,	128,	128 )); // gray
	codesview.push_back(RGB( 255,	255,	255 ));		// white
	
	codename.push_back("black"); // black
	codename.push_back("brown"); // brown
	codename.push_back("red"); // red
	codename.push_back("orange"); // orange
	codename.push_back("yellow"); // yellow
	codename.push_back("green"); // green
	codename.push_back("blue"); // blue
	codename.push_back("violet"); // violet
	codename.push_back("gray"); // gray
	codename.push_back("white");// white

	contfound = 0;


}


ResCan::~ResCan(void)
{
}

cv::string ResCan::doMagic(Mat image){
		
	//! determine working rectangle
	cv::Rect workingRect(50, 50, image.cols - 100, image.rows - 100);
	Mat small = image(workingRect);
	cv::string valuestring;
	Scalar mean;
	try{
	mean = cv::mean(small);
	}catch(...){
		return valuestring;
	}
		
	// Scalar lastcolor = new Scalar(0,0,0,0);  ! commented in original ResCan java code
	vector<int> nums(small.cols, 0);
	vector<double> avgy(small.cols, 0);
	// double colors[][] = new double[small.cols()][4];  ! commented in original ResCan java code
	for (int x = 0; x < small.cols; x += 1) {
			
		int num = 0;
		Vec3b *res = 0;
		for (int y = 0; y < small.rows; y += 1) {
				res = small.ptr<Vec3b>(y, x);
				if (res->val[0] > mean[0] * 0.5 && res->val[1] > mean[1] * 0.5 && res->val[2] > mean[2] * 0.5) {
				//small.at<Vec3b>(y,x)=0; ! commented in original ResCan java code
			}
			else {
				num++;
				avgy[x] += y;
			}
		}
		if (num > 2) {
			nums[x] = num;
			avgy[x] /= num;
		}
	}
		
	double avgnum = 0;
	int avgnumnum = 0;
	for (int x = 0; x < small.cols; x += 1) {
		if (nums[x] > 0) {
			avgnum += nums[x];
			avgnumnum++;
		}
	}
	int minx = -1;
	int maxx = 0;
	if (avgnumnum > 0) {
		avgnum /= avgnumnum * 1.0;
			
		for (int x = 0; x < small.cols; x ++) {
			if (nums[x] > avgnum + 15) {
				if (minx < 0) {
					minx = x;
				}
				maxx = x;
					
			}
		}
	}
	int miny = 0;
	int maxy = 0;
	if (minx >= 0) {
		miny = (int) avgy[minx];
		maxy = (int) avgy[maxx];
			
			
		int minx2 = minx - 20;
		if (minx2 < 0) {
			minx2 = 0;
		}
		if (minx2 > (signed)avgy.size() - 1) {
			minx2 = avgy.size() - 1;
		}
		int maxx2 = maxx + 20;
		if (maxx2 < 0) {
			maxx2 = 0;
		}
		if (maxx2 >(signed)avgy.size() - 1) {
			maxx2 = avgy.size() - 1;
		}
		double a = (avgy[maxx2] - avgy[minx2]) / ((maxx2) - (minx2));
			
		miny = (int) (a * (minx - minx2) + avgy[minx2]);
		maxy = (int) (a * (maxx - maxx2) + avgy[maxx2]);
			
		// Mat tiny = small.submat(y1, y2, minx, maxx);  ! commented in original ResCan java code
			
		int w = maxx - minx;

			
		Vec3b *col1, *col2, *col3, *col4;
		if (w > 10 && a < 0.7 && a > -0.7) {			
			int x, y, x2, row, col;
			int oy, ox;
			Vec3b res;
			double a2;
			Vec3b blue(255,0,0);

			for (x = 0; x < w; x++) {
				y = (int)(a * x) + miny;
				//if( y>40 && y<small.rows()-40)  ! commented in original ResCan java code
				//{
				a2 = -1.0 / a;
						
				res = 0;
						
				x2 = x + minx;
						
				oy = 15;
				ox = (int)(oy / a2);

				do //! for break when accessing out of range
				{
					row = y + oy;
					col = x2 + ox;

					col1 = NULL;
					//! fast range check
					if ((unsigned)(row-0) >= (small.rows) ||
						(unsigned)(col-0) >= (small.cols))
						break;
						
					col1 = small.ptr<Vec3b>(row,col);


					oy = -15;
					ox = (int)(oy / a2);

					row = y + oy;
					col = x2 + ox;


					col2 = NULL;
					//! fast range check
					if ((unsigned)(row) >= (small.rows) ||
						(unsigned)(col) >= (small.cols))
						break;
					col2 = small.ptr<Vec3b>(row, col);

					oy = 25;
					ox = (int)(oy / a2);

					row = y + oy;
					col =  x2 + ox;

					col3 = NULL;
					//! fast range check
					if ((unsigned)(row) >= (small.rows) ||
						(unsigned)(col) >= (small.cols))
						break;
					col3 = small.ptr<Vec3b>(row,col);

					oy = -25;
					ox = (int)(oy / a2);
					row = y + oy;
					col = x2 + ox;
					col4 = NULL;
					//! fast range check
					if ((unsigned)(row) >= (small.rows) ||
						(unsigned)(col) >= (small.cols))
						break;
					col4 = small.ptr<Vec3b>(row, col );

					res[0] = (col1->val[0] + col2->val[0] + col3->val[0] + col4->val[0]) / 4;
					res[1] = (col1->val[1] + col2->val[1] + col3->val[1] + col4->val[1]) / 4;
					res[2] = (col1->val[2] + col2->val[2] + col3->val[2] + col4->val[2]) / 4;
					image.at<Vec3b>(0, x) = res;	
				}
				while (false); // do
						
			}
			Mat result;
			//! copy scalled effective region to result  
			cv::resize(image(Rect(0, 0, w, 1)), result, Size(image.cols, 50), 0, 0,
					cv::INTER_NEAREST);
				
			//! detect colors and calculate value from result image segment
			//! and draw on image
			valuestring = detect(image, result);

			//! draw (scaled) working segmet on top of the image 
			result.copyTo(image(Rect(0, 0, result.cols, result.rows)));

		}
				
		//! draw mean red line
		cv::line(small, Point(minx, miny), Point(maxx, maxy), Scalar(0, 0, 255), 3);

	}
	//! draw rectangle on working area
	cv::rectangle(image, workingRect, cv::Scalar(0, 255, 0));
		
	return valuestring;
	
}


cv::string ResCan::detect(Mat image, Mat res) {
	cv::string valuestring = ""; //! return 
	
	//! calculate resistor background color
	int bgWidth = res.cols /16;
	cv::Scalar bgScalar;

	//! Gawhary: calculate bg color, get avarage of some columns from begginig 
	cv::Rect bgRec(res.cols - bgWidth, 0, bgWidth, res.rows);
	bgScalar = mean(res(bgRec));
	

	////! calculate bg color, get avarage of some pixels from begginig 
	//unsigned int x, i;
	//for (x = res.cols - bgWidth; x < res.cols; x++) {
	//	Vec3b val = res.at<Vec3b>(0, x);
	//	for (i = 0; i < 3; i++) {
	//		bgScalar[i] += (float)val[i] / bgWidth;
	//	}
	//}

	Vec3b bg = Scalar2Vec3b(bgScalar);

	vector<double> bgdists(res.cols, 0);
	double avgdist = 0;
	for (int x = 0; x < res.cols - 5; x++) {
		bgdists[x] = coldist2(&bg, res.ptr<Vec3b>(0, x) ) / 100.0;
		avgdist += bgdists[x] / res.cols;

		//! Gawhary: the following line generates noise
		//cv::line(image, Point(x * image.cols / res.cols, bgdists[x]), Point((x + 1) * image.cols
		//	/ res.cols, bgdists[x]), Scalar(0, 255, 255));
	}
	//! draw the horixontal thin red line
	cv::line(image, Point(0, (int)avgdist), Point(image.cols, (int)avgdist), Scalar(0, 0, 255));



	vector<int> coldet(res.cols, 0);
	for (int x = 0; x < res.cols; x++) {
		if (bgdists[x] > avgdist * 1.0) {
			double min = 100000;
			int minc = -1;
			for (unsigned int c = 0; c < codes.size(); c++) {
				double dist = coldist2(res.ptr<Vec3b>(0, x), &codes[c]);
				if (dist < min) {
					min = dist;
					minc = c;
				}
			}
			coldet[x] = minc;
			if (minc >= 0) {

				//cv::rectangle(image, Point(x * image.cols / res.cols, 50),
				//	Point((x + 1) * image.cols / res.cols, 100), Vec3b2Scalar(codes[minc]), -1);

				// draw predicted colors on top, next to scaled region of interest
				cv::rectangle(image, Point(x * image.cols / res.cols, 50),
					Point((x + 1) * image.cols / res.cols, 100), Vec3b2Scalar( codesview[minc]), -1);

			}

		}
		else {
			coldet[x] = -1;
		}
	}

	int numconti = 0;
	int numcodes = 0;
	vector<int> sumcodes(res.cols, 0);
	vector<int> result(3, 0);
	bool found = false;
	for (int x = 0; x < res.cols; x++) {
		if (coldet[x] == -1 && numconti > 20) { // 20 continuous at least
			// System.out.println("contiend");
			vector<int> sumc(codes.size(), 0);
			for (int i = 0; i < numconti - 20; i++) {
				sumc[sumcodes[i]]++;
			}


			int maxnum = 0;
			int code = -1;
			for (unsigned int i = 0; i < codes.size(); i++) {
				if (sumc[i] > maxnum) {
					maxnum = sumc[i];
					code = i;
				}

			}
			if (code != -1) {

				// darstellen

				
				//cv::line(image, Point(x - 10, 0), Point(x - 10, 100), Scalar(0, 255, 0));

				//! draw final detected colors with text on bottom
				cv::rectangle(image, Point(numcodes * image.cols / 4, image.rows - 50), Point(
					(numcodes + 1) * image.cols / 4, image.rows), Scalar(codesview[code]), -1);
				cv::putText(image, codename[code], Point(numcodes * image.cols / 4, image.rows - 10),
					cv::FONT_HERSHEY_TRIPLEX, 1.0, Scalar(200, 200, 200));

				result[numcodes] = code;
				numcodes++;
				if (numcodes >= 3) {
					found = true;
					break;
				}
			}
			numconti = 0;
			
			std::fill(sumcodes.begin(), sumcodes.end(), 0); //! reset sumcodes
		}
		else if (coldet[x] >= 0) {
			if (numconti > 10)
			{
				sumcodes[numconti - 10] = coldet[x]; // don't obey the left
				// border
			}

			//! green vertical lines on top
			//if (numconti == 10) {
			//	cv::line(image, Point(x, 0), Point(x, 100), Scalar(0, 255, 0));
			//}

			numconti++;
		}
		else {
			numconti = 0;
		}

	}


	if (found) {
		if (lastresult.empty()) {
			lastresult =  vector<int>(result);
		}
		if (result[0] == lastresult[0] && result[1] == lastresult[1] && result[2] == lastresult[2]) {
			contfound++;
			//! report result after several continous result
			if (contfound > 5) { // Gawhary: changed from 10 in original java code

				double resistance = calcvalue(result);
				if (resistance > 0){ 
					//! calculate unit
					String unit = " Ohm";
					if (resistance >= 1000.0) {
						resistance /= 1000;
						unit = "k";
					}
					if (resistance >= 1000.0) {
						resistance /= 1000;
						unit = "meg";
					}
					//! compose result string
					stringstream ss;
					ss << setprecision(2) << resistance << " " << unit;
					valuestring = ss.str();
				}
				else 
					valuestring = ""; // Gawhary: to prevent changing last valid result to 0

				contfound = 0;
			}
		}
		else {
			contfound -= 1;
		}

		lastresult = vector<int>(result);
	}
	else {
		contfound -= 2;
		valuestring = "";
	}
	if (contfound < 0) {
		contfound = 0;
	}
	return valuestring;

}




