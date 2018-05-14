// CannyStill.cpp

#include<opencv2/opencv.hpp>

#include<iostream>
#include<conio.h>           // may have to modify this line if not using Windows

///////////////////////////////////////////////////////////////////////////////////////////////////

void exemple()
{

	cv::Mat imgOriginal;        // input image
	cv::Mat imgGrayscale;       // grayscale of input image
	cv::Mat imgBlurred;         // intermediate blured image
	cv::Mat imgCanny;           // Canny edge image

	imgOriginal = cv::imread("image.png");          // open image

	if (imgOriginal.empty()) {                                  // if unable to open image
		std::cout << "error: image not read from file\n\n";     // show error message on command line
		_getch();                                               // may have to modify this line if not using Windows
		return;                                              // and exit program
	}

	cv::cvtColor(imgOriginal, imgGrayscale, CV_BGR2GRAY);       // convert to grayscale

	cv::GaussianBlur(imgGrayscale,          // input image
		imgBlurred,                         // output image
		cv::Size(5, 5),                     // smoothing window width and height in pixels
		1.5);                               // sigma value, determines how much the image will be blurred

	cv::Canny(imgBlurred,           // input image
		imgCanny,                   // output image
		82,                         // low threshold
		164);                       // high threshold

									// declare windows
	cv::namedWindow("imgOriginal", CV_WINDOW_AUTOSIZE);     // note: you can use CV_WINDOW_NORMAL which allows resizing the window
	cv::namedWindow("imgCanny", CV_WINDOW_AUTOSIZE);        // or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
															// CV_WINDOW_AUTOSIZE is the default
	cv::imshow("imgOriginal", imgOriginal);     // show windows
	cv::imshow("imgCanny", imgCanny);

	cv::waitKey(0);                 // hold windows open until user presses a key
}

void diffentre2img()
{
	cv::Mat imgFog;
	cv::Mat imgUnfog;
	cv::Mat imgDiff;
	cv::Mat imgDiffGray;

	imgFog = cv::imread("imagefog3.png");

	if (imgFog.empty()) {
		std::cout << "error: image fog, incorrect name\n\n";
		_getch();
		return;
	}

	imgUnfog = cv::imread("imageunfog3.png");

	if (imgUnfog.empty()) {
		std::cout << "error: image unfog, incorrect name\n\n";
		_getch();
		return;
	}

	cv::absdiff(imgFog, imgUnfog, imgDiff);

	cv::imshow("imgDiff", imgDiff);

	cv::cvtColor(imgDiff, imgDiffGray, CV_BGR2GRAY);

	cv::imshow("imgDiffGray", imgDiffGray);

	cv::imwrite("./test.png", imgDiffGray);

	cv::waitKey(0);
}

void alphablendingimg()
{
	// Read the images
	cv::Mat foreground = cv::imread("dashcam.jpg");
	cv::Mat background = cv::imread("imageblend1.png");
	cv::Mat alpha = cv::imread("alpha1.png");

	// Convert Mat to float data type
	foreground.convertTo(foreground, CV_32FC3);
	background.convertTo(background, CV_32FC3);

	// Normalize the alpha mask to keep intensity between 0 and 1
	alpha.convertTo(alpha, CV_32FC3, 1.0 / 255); // 

												 // Storage for output image
	cv::Mat ouImage = cv::Mat::zeros(foreground.size(), foreground.type());

	// Multiply the foreground with the alpha matte
	cv::multiply(alpha, foreground, foreground);

	// Multiply the background with ( 1 - alpha )
	cv::multiply(cv::Scalar::all(1.0) - alpha, background, background);

	// Add the masked foreground and background.
	cv::add(foreground, background, ouImage);

	// Display image
	cv::imshow("alpha blended image", ouImage / 255);

	cv::imwrite("./alphablended.png", ouImage);

	cv::waitKey(0);
}

int main() {

	alphablendingimg();

	return(0);
}

