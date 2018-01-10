#pragma once
#include <string>
#include <map>
#include <opencv2\opencv.hpp>
class ArchiveFormatInterface {
	//the interface for different libararies of archive files

};

class ArchiveInterface
{
// class for archiving file, unzip the image files
// Use OpenCV, format for image: "cv::Mat"
public:
	ArchiveInterface() ;
	~ArchiveInterface() ;

	bool virtual loadArchivedFiles(std::string file_path) = 0;
	// Use 7z function
	//load to private element: archive_loaded.

	bool virtual loadOneImage(int num, cv::Mat &a_image) = 0;

	// Use OpenCV function
	//load one image from archive_loaded




private:
	ArchiveFormatInterface archive_loaded;
};


class ImageProcessInterface
{
public:
	ImageProcessInterface();
	~ImageProcessInterface();


	bool virtual AutoAdjustImage(cv::Mat& input_image, cv::Mat& output_image, int image_type_flag, int height, int width) = 0;
	// image_type_flags = 0 for image whose the type is text
	// image_type_flags = 1 for image whose the type is graphics
	// output_image's height in pixels
	// output_image's width in pixels

	bool virtual QuickPreview(cv::Mat& input_image, cv::Mat& output_image, int image_type_flag, int height, int width) = 0;
	// used for quickly flipping of pages
	// show the image with low resolution quickly

	bool virtual AutoAdjustForTwoImages(cv::Mat& input_image1, cv::Mat& input_image2, cv::Mat& output_image, int image_type_flag1, int image_type_flag2, int height, int width) = 0;
	//used for showing 2 images in the same screen
	// output_image is a whole image which contains 2 parts

	bool virtual getImage(int num, cv::Mat& output_image) = 0;
	//first step: check if the page exists in cache, if not, processing the images

private:
	std::map<int, cv::Mat> image_processed_cache;
};