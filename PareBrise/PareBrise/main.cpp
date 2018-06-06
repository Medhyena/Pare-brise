#include<opencv2/opencv.hpp>

#include<iostream>
#include<string>
#include<cmath>
#include<ctime>
#include<conio.h>

///////////////////////////////////////////////////////////////////////////////////////////////////

int netteteCalcul(const cv::Mat &img)
{
	int compteur = 0;
	for (int i = 0; i < img.rows / 3; i++)
	{
		for (int j = 0; j < img.cols / 3; j++)
		{
			cv::Vec3b color = img.at<cv::Vec3b>(cv::Point(j, i));
			if (color[0] == 255 && color[1] == 255 && color[2] == 255)
				compteur++;
		}
	}
	return compteur;
}

void comparaisonNettete(int numImg)
{
	cv::Mat imgFog;
	cv::Mat imgNousExp;
	cv::Mat imgNousLog;
	cv::Mat imgNousLog2;
	cv::Mat imgUnfog;

	imgFog = cv::imread("./Images/ImagesOriginales/imagefog" + std::to_string(numImg) + ".png");

	if (imgFog.empty()) {
		std::cout << "error: image fog, incorrect name\n\n";
		_getch();
		return;
	}

	imgNousExp = cv::imread("./Images/ImagesNotreMethode/imgDebrumeeNotreMethodeExp" + std::to_string(numImg) + ".png");

	if (imgNousExp.empty()) {
		std::cout << "error: image nous exp, incorrect name\n\n";
		_getch();
		return;
	}

	imgNousLog = cv::imread("./Images/ImagesNotreMethode/imgDebrumeeNotreMethodeLog" + std::to_string(numImg) + ".png");

	if (imgNousLog.empty()) {
		std::cout << "error: image nous exp, incorrect name\n\n";
		_getch();
		return;
	}

	imgUnfog = cv::imread("./Images/ImagesDEFADE/imageunfog" + std::to_string(numImg) + ".png");

	if (imgUnfog.empty()) {
		std::cout << "error: image unfog, incorrect name\n\n";
		_getch();
		return;
	}

	cv::cvtColor(imgFog, imgFog, CV_BGR2GRAY);
	cv::cvtColor(imgUnfog, imgUnfog, CV_BGR2GRAY);
	cv::cvtColor(imgNousExp, imgNousExp, CV_BGR2GRAY);
	cv::cvtColor(imgNousLog, imgNousLog, CV_BGR2GRAY);
	cv::cvtColor(imgNousLog2, imgNousLog2, CV_BGR2GRAY);

	blur(imgFog, imgFog, cv::Size(3, 3));
	blur(imgUnfog, imgUnfog, cv::Size(3, 3));
	blur(imgNousExp, imgNousExp, cv::Size(3, 3));
	blur(imgNousLog, imgNousLog, cv::Size(3, 3));
	blur(imgNousLog2, imgNousLog2, cv::Size(3, 3));

	cv::Canny(imgFog, imgFog, 20, 60, 3);
	cv::Canny(imgUnfog, imgUnfog, 20, 60, 3);
	cv::Canny(imgNousExp, imgNousExp, 20, 60, 3);
	cv::Canny(imgNousLog, imgNousLog, 20, 60, 3);
	cv::Canny(imgNousLog2, imgNousLog2, 20, 60, 3);
	
	imgFog.convertTo(imgFog, CV_8U);
	imgUnfog.convertTo(imgUnfog, CV_8U);
	imgNousExp.convertTo(imgNousExp, CV_8U);
	imgNousLog.convertTo(imgNousLog, CV_8U);
	imgNousLog2.convertTo(imgNousLog2, CV_8U);

	double tableaunettete[5];

	tableaunettete[0] = netteteCalcul(imgFog);
	tableaunettete[1] = netteteCalcul(imgNousExp);
	tableaunettete[2] = netteteCalcul(imgNousLog);
	tableaunettete[3] = netteteCalcul(imgNousLog2);
	tableaunettete[4] = netteteCalcul(imgUnfog);

	tableaunettete[1] /= tableaunettete[0];
	tableaunettete[2] /= tableaunettete[0];
	tableaunettete[3] /= tableaunettete[0];
	tableaunettete[4] /= tableaunettete[0];
	tableaunettete[0] /= tableaunettete[0];


	std::cout << "Indice de nettete:\n";
	std::cout << "Image de base brumee:\t" << tableaunettete[0] << std::endl;
	std::cout << "Image debrumee exp:\t" << tableaunettete[1] << std::endl;
	std::cout << "Image debrumee log:\t" << tableaunettete[2] << std::endl;
	std::cout << "Image debrumee log2:\t" << tableaunettete[3] << std::endl;
	std::cout << "Image debrumee DEFADE:\t" << tableaunettete[4] << std::endl;

	_getch();
}

void bruitArtificielPoivreSel(cv::Mat &imgABruiter)
{
	srand(time(NULL));
	int randomBruit;
	bool randomColor;
	for (int i = 0; i < imgABruiter.rows; i++)
	{
		for (int j = 0; j < imgABruiter.cols; j++)
		{
			randomBruit = rand() % 1000;
			randomColor = rand() % 2;
			if (randomBruit < 1)
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

void Diffentre2Img(int numimg)
{
	cv::Mat imgFog;
	cv::Mat imgUnfog;
	cv::Mat imgDiff;
	cv::Mat imgDiffGray;

	imgFog = cv::imread("./Images/ImagesOriginales/imagefog" + std::to_string(numimg) + ".png");

	if (imgFog.empty()) {
		std::cout << "error: image fog, incorrect name\n\n";
		_getch();
		return;
	}

	imgUnfog = cv::imread("./Images/ImagesDEFADE/imageunfog" + std::to_string(numimg) + ".png");

	if (imgUnfog.empty()) {
		std::cout << "error: image unfog, incorrect name\n\n";
		_getch();
		return;
	}

	cv::absdiff(imgFog, imgUnfog, imgDiff);

	cv::imshow("imgDiff", imgDiff);

	cv::cvtColor(imgDiff, imgDiffGray, CV_BGR2GRAY);

	cv::imshow("imgDiffGray", imgDiffGray);

	cv::imwrite("./imgDiffGray" + std::to_string(numimg) + ".png", imgDiffGray);

	cv::waitKey(0);
}

void CannyTest(int numimg)
{
	cv::Mat imgFog;
	cv::Mat imgUnfog;
	cv::Mat imgNousExp;
	cv::Mat imgNousLog;

	imgFog = cv::imread("./Images/ImagesOriginales/imagefog" + std::to_string(numimg) + ".png");

	if (imgFog.empty()) {
		std::cout << "error: image fog, incorrect name\n\n";
		_getch();
		return;
	}

	imgUnfog = cv::imread("./Images/ImagesDEFADE/imageunfog" + std::to_string(numimg) + ".png");

	if (imgUnfog.empty()) {
		std::cout << "error: image unfog, incorrect name\n\n";
		_getch();
		return;
	}

	imgNousExp = cv::imread("./Images/ImagesNotreMethode/imgDebrumeeNotreMethodeExp" + std::to_string(numimg) + ".png");

	if (imgNousExp.empty()) {
		std::cout << "error: image nous, incorrect name\n\n";
		_getch();
		return;
	}

	imgNousLog = cv::imread("./Images/ImagesNotreMethode/imgDebrumeeNotreMethodeLog" + std::to_string(numimg) + ".png");

	if (imgNousLog.empty()) {
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
	cv::cvtColor(imgNousExp, imgNousExp, CV_BGR2GRAY);
	cv::cvtColor(imgNousLog, imgNousLog, CV_BGR2GRAY);

	blur(imgFog, imgFog, cv::Size(3, 3));
	blur(imgUnfog, imgUnfog, cv::Size(3, 3));
	blur(imgNousExp, imgNousExp, cv::Size(3, 3));
	blur(imgNousLog, imgNousLog, cv::Size(3, 3));

	cv::Canny(imgFog, imgFog, 20, 60, 3);
	cv::Canny(imgUnfog, imgUnfog, 20, 60, 3);
	cv::Canny(imgNousExp, imgNousExp, 20, 60, 3);
	cv::Canny(imgNousLog, imgNousLog, 20, 60, 3);

	imgFog.convertTo(imgFog, CV_8U);
	imgUnfog.convertTo(imgUnfog, CV_8U);
	imgNousExp.convertTo(imgNousExp, CV_8U);
	imgNousLog.convertTo(imgNousLog, CV_8U);

	cv::imshow("image fog", imgFog);
	cv::imshow("image unfog", imgUnfog);
	cv::imshow("image nous exp", imgNousExp);
	cv::imshow("image nous log", imgNousExp);
	
	cv::imwrite("./Images/ImagesContours/imagecontourfog" + std::to_string(numimg) + ".png", imgFog);
	cv::imwrite("./Images/ImagesContours/imagecontourunfog" + std::to_string(numimg) + ".png", imgUnfog);
	cv::imwrite("./Images/ImagesContours/imagecontournousexp" + std::to_string(numimg) + ".png", imgNousExp);
	cv::imwrite("./Images/ImagesContours/imagecontournouslog" + std::to_string(numimg) + ".png", imgNousLog);

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
	uchar luminosite = (uchar) (pourcent * 255) / 3; // Meilleurs résultats quand on divise par trois
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

void DebrumageParContraste(int numimgfog)
{
	cv::Mat imgBrumee = cv::imread("./Images/ImagesOriginales/imagefog" + std::to_string(numimgfog) + ".png");

	//bruitArtificielPoivreSel(imgBrumee);

	cv::Mat imgBrumee2 = imgBrumee.clone();
	cv::Mat imgOrig = imgBrumee.clone();

	cv::Vec3b color;

	for (int i = 0; i < imgBrumee.rows; i++)
	{
		double pourcent1 = exp(((double)-i) / (imgBrumee.rows - 1));
		double pourcent2 = log(exp(1) - (double)i / (double)imgBrumee.rows);
		for (int j = 0; j < imgBrumee.cols; j++)
		{
			color = imgBrumee.at<cv::Vec3b>(cv::Point(j, i));
			DiminuerLuminositePixel(color, pourcent1);
			AugmenterContrastePixel(color, pourcent1);
			imgBrumee.at<cv::Vec3b>(cv::Point(j, i)) = color;
			color = imgBrumee2.at<cv::Vec3b>(cv::Point(j, i));
			DiminuerLuminositePixel(color, pourcent2);
			AugmenterContrastePixel(color, pourcent2);
			imgBrumee2.at<cv::Vec3b>(cv::Point(j, i)) = color;
		}
	}
	cv::imshow("ET HOP", imgBrumee);
	cv::imwrite("./Images/ImagesNotreMethode/imgDebrumeeNotreMethodeExp" + std::to_string(numimgfog) + ".png", imgBrumee);
	cv::imshow("EN FAIT, NON!", imgBrumee2);
	cv::imwrite("./Images/ImagesNotreMethode/imgDebrumeeNotreMethodeLog" + std::to_string(numimgfog) + ".png", imgBrumee2);
	cv::imshow("BAH MANGE TES MORTS", imgOrig);
	cv::waitKey(0);
}

void DebrumageParContrasteVideo(std::string nomVidBrumee)
{
	cv::VideoCapture capture(nomVidBrumee);
	cv::Size S = cv::Size((int)capture.get(CV_CAP_PROP_FRAME_WIDTH) * 2,    // Taille de la vidéo * 2 en horizontal à cause de la concaténation
		(int)capture.get(CV_CAP_PROP_FRAME_HEIGHT));
	int fourcc = CV_FOURCC('F', 'M', 'P', '4');
	cv::VideoWriter output("concat720log.mp4", fourcc, capture.get(CV_CAP_PROP_FPS), S, true);
	cv::Mat imgBrumee;
	cv::Mat imgCopy;
	cv::Mat imgShow;

	for (; ; )
	{
		capture >> imgBrumee;
		if (imgBrumee.empty())
			break;
		imgCopy = imgBrumee.clone();
		for (int i = 0; i < imgBrumee.rows; i++)
		{
			double pourcent = exp(((double)-i) / (imgBrumee.rows - 1));
			double pourcent3 = log(exp(1) - (double)i / (((double)imgBrumee.rows * 3.0 / 4.0) - 1));
			for (int j = 0; j < imgBrumee.cols; j++)
			{
				cv::Vec3b color = imgBrumee.at<cv::Vec3b>(cv::Point(j, i));
				DiminuerLuminositePixel(color, pourcent3);
				AugmenterContrastePixel(color, pourcent3);
				imgBrumee.at<cv::Vec3b>(cv::Point(j, i)) = color;
			}
		}
		cv::hconcat(imgCopy, imgBrumee, imgShow);
		output << imgShow;
		//cv::imshow("Rendu vidéo", imgShow);
		//cv::imwrite("img720ptrainbrume.png", imgCopy);
		//cv::imwrite("img720ptraindebrume.png", imgBrumee);
		//cv::waitKey(1); // waits to display frame
	}
	capture.release();
}

int main()
{
	//AlphaBlendingImg("dashcam.jpg", "imageblend1.png", "alpha1.png");
	//DebrumageParContrasteVideo("test720p.mp4");
	for (int i = 1; i < 8; i++)
	{
		DebrumageParContraste(i);
		CannyTest(i);
	}
	//comparaisonNettete(4);

	return(0);
}