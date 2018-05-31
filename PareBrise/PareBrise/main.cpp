#include<opencv2/opencv.hpp>

#include<iostream>
#include<string>
#include<cmath>
#include<ctime>
#include<conio.h>

///////////////////////////////////////////////////////////////////////////////////////////////////

void bruitArtificielPoivreSel(cv::Mat &imgABruiter)
{
	srand(time(NULL));
	int randomBruit;
	bool randomColor;
	for (int i = 0; i < imgABruiter.rows; i++)
	{
		for (int j = 0; j < imgABruiter.cols; j++)
		{
			randomBruit = rand() % 100;
			randomColor = rand();
			if (randomBruit < 5)
			{
				cv::Vec3b color = imgABruiter.at<cv::Vec3b>(cv::Point(j, i));
				if (randomColor)
				{
					color[0] = 0;
					color[1] = 0;
					color[2] = 0;
				}
				else
				{
					color[0] = 255;
					color[1] = 255;
					color[2] = 255;
				}
				imgABruiter.at<cv::Vec3b>(cv::Point(j, i)) = color;
			}
		}
	}

}

void Diffentre2Img()
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

void CannyTest()
{
	cv::Mat imgFog;
	cv::Mat imgUnfog;
	cv::Mat imgNous;

	imgFog = cv::imread("imagefog6.png");

	if (imgFog.empty()) {
		std::cout << "error: image fog, incorrect name\n\n";
		_getch();
		return;
	}

	imgUnfog = cv::imread("imageunfog6.png");

	if (imgUnfog.empty()) {
		std::cout << "error: image unfog, incorrect name\n\n";
		_getch();
		return;
	}

	imgNous = cv::imread("imgDebrumeeNotreMethode63.png");

	if (imgNous.empty()) {
		std::cout << "error: image nous, incorrect name\n\n";
		_getch();
		return;
	}
	/*
	bruitArtificielPoivreSel(imgFog);
	bruitArtificielPoivreSel(imgUnfog);
	bruitArtificielPoivreSel(imgNous);
	*/
	cv::cvtColor(imgFog, imgFog, CV_BGR2GRAY);
	cv::cvtColor(imgUnfog, imgUnfog, CV_BGR2GRAY);
	cv::cvtColor(imgNous, imgNous, CV_BGR2GRAY);

	blur(imgFog, imgFog, cv::Size(3, 3));
	blur(imgUnfog, imgUnfog, cv::Size(3, 3));
	blur(imgNous, imgNous, cv::Size(3, 3));

	cv::Canny(imgFog, imgFog, 20, 60, 3);
	cv::Canny(imgUnfog, imgUnfog, 20, 60, 3);
	cv::Canny(imgNous, imgNous, 20, 60, 3);

	imgFog.convertTo(imgFog, CV_8U);
	imgUnfog.convertTo(imgUnfog, CV_8U);
	imgNous.convertTo(imgNous, CV_8U);

	cv::imshow("image fog", imgFog);
	cv::imshow("image unfog", imgUnfog);
	cv::imshow("image nous", imgNous);
	/*
	cv::imwrite("./imagecontourfog6.png", imgFog);
	cv::imwrite("./imagecontourunfog6.png", imgUnfog);
	cv::imwrite("./imagecontournous63.png", imgNous);
	*/
	//cv::imwrite("./test.png", imgDiffGray);

	cv::waitKey(0);
}

void AlphaBlendingImg(std::string foregroundname, std::string backgroundname, std::string alphaname)
{
	cv::Mat foreground = cv::imread(foregroundname);
	cv::Mat background = cv::imread(backgroundname);
	cv::Mat alpha = cv::imread(alphaname);

	foreground.convertTo(foreground, CV_32FC3);
	background.convertTo(background, CV_32FC3);

	alpha.convertTo(alpha, CV_32FC3, 1.0 / 255);

	cv::Mat ouImage = cv::Mat::zeros(foreground.size(), foreground.type());

	cv::multiply(alpha, foreground, foreground);

	cv::multiply(cv::Scalar::all(1.0) - alpha, background, background);

	cv::add(foreground, background, ouImage);

	cv::imshow("alpha blended image", ouImage / 255);

	cv::imwrite("./alphablended.png", ouImage);

	cv::waitKey(0);
}

void AlphaBlendingVideo(std::string nomVidBlend, std::string nomVidBackground, std::string nomVidAlpha)
{
	cv::VideoCapture captureBlend(nomVidBlend);
	cv::VideoCapture captureBackground(nomVidBackground);
	cv::VideoCapture captureAlpha(nomVidAlpha);
	cv::Mat foreground;
	cv::Mat background;
	cv::Mat alpha;

	cv::namedWindow("w", 1);
	for (; ; )
	{
		captureBlend >> foreground;
		captureBackground >> background;
		captureAlpha >> alpha;

		if (foreground.empty())
			break;
		if (background.empty())
			break;
		if (alpha.empty())
			break;

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

		cv::waitKey(1);
	}
	captureBlend.release();
	captureBackground.release();
	captureAlpha.release();
}

void DiminuerLuminositePixel(cv::Vec3b &color, double pourcent)
{
	uchar luminosite = (uchar) (pourcent * 255) / 2; // Meilleurs résultats quand on divise par deux
	// Red
	int tmp = color[0] - luminosite;
	if (tmp > 255) color[0] = 255;
	else if (tmp < 0) color[0] = 0;
	else color[0] = (uchar) tmp;
	// Green
	tmp = color[1] - luminosite;
	if (tmp > 255) color[1] = 255;
	else if (tmp < 0) color[1] = 0;
	else color[1] = (uchar) tmp;
	// Blue
	tmp = color[2] - luminosite;
	if (tmp > 255) color[2] = 255;
	else if (tmp < 0) color[2] = 0;
	else color[2] = (uchar) tmp;
}

void AugmenterContrastePixel(cv::Vec3b &color, double pourcent)
{
	uchar contraste = (uchar)(pourcent * 255);
	// Red
	double tmp = color[0] + (double) (contraste - 128) / 255.0 * (color[0] - 127);
	if (tmp > 255) color[0] = 255;
	else if (tmp < 0) color[0] = 0;
	else color[0] = (uchar)tmp;
	// Green
	tmp = color[1] + (double)(contraste - 128) / 255.0 * (color[1] - 127);
	if (tmp > 255) color[1] = 255;
	else if (tmp < 0) color[1] = 0;
	else color[1] = (uchar)tmp;
	// Blue
	tmp = color[2] + (double)(contraste - 128) / 255.0 * (color[2] - 127);
	if (tmp > 255) color[2] = 255;
	else if (tmp < 0) color[2] = 0;
	else color[2] = (uchar)tmp;
}

void DebrumageParContraste(std::string nomImgBrumee)
{
	cv::Mat imgBrumee = cv::imread(nomImgBrumee);

	bruitArtificielPoivreSel(imgBrumee);

	cv::Mat imgOrig = imgBrumee.clone();

	for (int i = 0; i < imgBrumee.rows; i++)
	{
		double pourcent = 1.0 - (double)i / (imgBrumee.rows - 1);
		double pourcent2 = exp(((double)-i) / (imgBrumee.rows - 1));
		double pourcent3 = log(exp(1) - (double)i / (((double)imgBrumee.rows * 3.0 / 4.0) - 1));
		for (int j = 0; j < imgBrumee.cols; j++)
		{
			cv::Vec3b color = imgBrumee.at<cv::Vec3b>(cv::Point(j, i));
			DiminuerLuminositePixel(color, pourcent3);
			AugmenterContrastePixel(color, pourcent3);
			imgBrumee.at<cv::Vec3b>(cv::Point(j, i)) = color;
		}
	}
	cv::imshow("ET HOP", imgBrumee);
	cv::imwrite("imgDebrumeeNotreMethode63.png", imgBrumee);
	cv::imshow("BAH MANGE TES MORTS", imgOrig);
	cv::waitKey(0);
}

void DebrumageParContrasteVideo(std::string nomVidBrumee)
{
	cv::VideoCapture capture(nomVidBrumee);
	cv::Mat imgBrumee;
	cv::Mat imgCopy;
	cv::Mat imgShow;

	cv::namedWindow("w", 1);
	for (; ; )
	{
		capture >> imgBrumee;
		if (imgBrumee.empty())
			break;
		imgCopy = imgBrumee.clone();
		for (int i = 0; i < imgBrumee.rows; i++)
		{
			double pourcent = exp(((double)-i) / (imgBrumee.rows - 1));
			for (int j = 0; j < imgBrumee.cols; j++)
			{
				cv::Vec3b color = imgBrumee.at<cv::Vec3b>(cv::Point(j, i));
				DiminuerLuminositePixel(color, pourcent);
				AugmenterContrastePixel(color, pourcent);
				imgBrumee.at<cv::Vec3b>(cv::Point(j, i)) = color;
			}
		}
		cv::hconcat(imgCopy, imgBrumee, imgShow);
		cv::imshow("Rendu vidéo", imgShow);
		//cv::imwrite("img720ptrainbrume.png", imgCopy);
		//cv::imwrite("img720ptraindebrume.png", imgBrumee);
		cv::waitKey(1); // waits to display frame
	}
	capture.release();
}

int main()
{
	//AlphaBlendingImg("dashcam.jpg", "imageblend1.png", "alpha1.png");
	//DebrumageParContraste("imagefog6.png");
	//DebrumageParContrasteVideo("test480p.mp4");
	CannyTest();

	return(0);
}