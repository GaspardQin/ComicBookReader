#pragma once
#include "comic_book_reader_contract.h"

class ImageProcess:public ImageProcessInterface {
	bool AutoAdjustImage(cv::Mat& input_image, cv::Mat& output_image, int image_type_flag, int height, int width) {
		// image_type_flags = 0 for image whose the type is text
		// image_type_flags = 1 for image whose the type is graphics
		// output_image's height in pixels
		// output_image's width in pixels
		
		if (image_type_flag == 0) AutoAdjustImageText(input_image, output_image, height, width);
		else {
			if (image_type_flag == 1) AutoAdjustImageGraphic(input_image, output_image, height, width);
			else {
				return false;
			}
		}
		return true;
	}

	
	bool AutoAdjustImageText(cv::Mat& input_image, cv::Mat& output_image, int height, int width)
	{
		cv::Mat temp_image;
		/****Transfer color image into gray image****/
		cvtColor(input_image, temp_image, CV_RGB2GRAY);


		/****smooth the image****/
		//could be slow by using GaussianBlur, could be changed to other linear filter.
		cv::GaussianBlur(temp_image, temp_image, cv::Size(3, 3), 0, 0);

		
		
		/****Auto Threhold divise into white(paper) and black(text)****/
		cv::adaptiveThreshold(temp_image, temp_image, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 10, 0);
		//if slow, change ADAPTIVE_THRESH_GAUSSIAN_C into CV_ADAPTIVE_THRESH_MEAN_C
		//Could use GUI to change blocksize.

		/****scale to the propre height and width****/
		cv::resize(temp_image, output_image, cv::Size(width, height));

		return true;
	}
	bool AutoAdjustImageGraphic(cv::Mat& input_image, cv::Mat& output_image, int height, int width) 
	{
		
		cv::Mat temp_image;
		
		/****smooth the image****/
		//could be slow by using GaussianBlur, could be changed to other linear filter.
		cv::GaussianBlur(input_image, temp_image, cv::Size(3, 3), 0, 0);
		
		/****autojustment by equilize Hist****/
		std::vector<cv::Mat> channels;
		split(input_image, channels); 		//Split input_image into 3 channels RGB
		for(int i = 0; i < 3; i++){
			cv::equalizeHist(input_image, channels[i]);
		}
		cv::merge(channels, temp_image);
		

		/****autojustment by Laplace filter (Increasing local contrast)****/
		cv::Mat kernel = (cv::Mat_<int>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
		cv::filter2D(temp_image, temp_image, temp_image.depth(), kernel);

		/****scale to the propre height and width****/
		cv::resize(temp_image, output_image, cv::Size(width,height));
		return true;
	}
	bool virtual QuickPreview(cv::Mat& input_image, cv::Mat& output_image, int image_type_flag, int height, int width) = 0;
	// used for quickly flipping of pages
	// show the image with low resolution quickly

	bool virtual AutoAdjustForTwoImages(cv::Mat& input_image1, cv::Mat& input_image2, cv::Mat& output_image, int image_type_flag1, int image_type_flag2, int height, int width) = 0;
	//used for showing 2 images in the same screen
	// output_image is a whole image which contains 2 parts

	bool virtual getImage(int num, cv::Mat& output_image) = 0;
	//first step: check if the page exists in cache, if not, processing the images
};

