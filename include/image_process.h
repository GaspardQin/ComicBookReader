#pragma once
#include "comic_book_reader_contract.h"
class ImageProcess:public ImageProcessInterface {
public:
	bool autoAdjustImage(cv::Mat& input_image, cv::Mat& output_image, int image_type_flag) {
		// image_type_flags = 0 for image whose the type is text
		// image_type_flags = 1 for image whose the type is graphics
		// output_image's height in pixels
		// output_image's width in pixels
		
		if (image_type_flag == 0) autoAdjustImageText(input_image, output_image);
		else {
			if (image_type_flag == 1) autoAdjustImageGraphic(input_image, output_image);
			else {
				return false;
			}
		}
		return true;
	}

	
	bool autoAdjustImageText(cv::Mat& input_image, cv::Mat& output_image)
	{
		cv::Mat temp_image;
		/****Transfer color image into gray image****/
		cvtColor(input_image, temp_image, CV_RGB2GRAY);


		/****smooth the image****/
		//could be slow by using GaussianBlur, could be changed to other linear filter.
		cv::GaussianBlur(temp_image, temp_image, cv::Size(3, 3), 0, 0);

		
		
		/****Auto Threhold divise into white(paper) and black(text)****/
		cv::adaptiveThreshold(temp_image, output_image, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 5, 0);
		//int blocksize can only be impaire
		//if slow, change ADAPTIVE_THRESH_GAUSSIAN_C into CV_ADAPTIVE_THRESH_MEAN_C
		//Could use GUI to change blocksize.

		return true;
	}
	bool autoAdjustImageGraphic(cv::Mat& input_image, cv::Mat& output_image) 
	{
		
		cv::Mat temp_image;
		
		/****smooth the image****/
		//could be slow by using GaussianBlur, could be changed to other linear filter.
		cv::GaussianBlur(input_image, temp_image, cv::Size(3, 3), 0, 0);
		
		/****autojustment by equilize Hist****/
		std::vector<cv::Mat> channels;
		split(input_image, channels); 		//Split input_image into 3 channels RGB
		for(int i = 0; i < 3; i++){
			cv::equalizeHist(channels[i], channels[i]);
		}
		cv::merge(channels, temp_image);
		

		/****autojustment by Laplace filter (Increasing local contrast)****/
		cv::Mat kernel = (cv::Mat_<int>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
		cv::filter2D(temp_image, temp_image, temp_image.depth(), kernel);

		output_image = temp_image;
		return true;
	}
	bool virtual quickPreview(cv::Mat& input_image, cv::Mat& output_image) 
	{
		// used for quickly flipping of pages
		// show the image with low resolution quickly
		cv::resize(input_image, output_image, cv::Size(input_image.cols / 4, input_image.rows / 4), 0, 0, cv::INTER_LINEAR);
		
		//Do not process
		return true;
	}


	bool virtual autoAdjustForTwoImages(cv::Mat& input_image1, cv::Mat& input_image2, cv::Mat& output_image, int image_type_flag1, int image_type_flag2, int height, int width)
	{
		//used for showing 2 images in the same screen
		// output_image is a whole image which contains 2 parts



		return true;
	}





	cv::Mat getImageFake(int num, std::string path) {
		cv::Mat test_img = cv::imread(testPath);
		return test_img;

	}

	void setImageFakePath(std::string path) {
		testPath = path;
	}

	bool getImage(int num, int image_type_flag, cv::Mat& output_image)
	{
		///////////////////////////////////
		// need to be replaced by archivedFunctions
		cv::Mat input_image = getImageFake(num,"This function should be replaced!!!");
		///////////////////////////////////


		//first step: check if the page exists in cache, if not, processing the images

		std::map<int, std::pair<int, cv::Mat>>::iterator it;
		it = image_processed_cache.find(num);
		if (it != image_processed_cache.end()) {
			if (it->second.first == image_type_flag) {
				//has been cached before
				output_image = it->second.second;
				return true;
			}
		}


		//if it has not been cached before
		assert(autoAdjustImage(input_image, output_image, image_type_flag));

		return true;
		
	}

private:
	std::string testPath;//Need to be deleted!!!!!!!!!!!!
	std::map<int, std::pair<int,cv::Mat>> image_processed_cache;
};

