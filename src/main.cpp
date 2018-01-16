#include "../include/image_process.h"

void testImageProcess() {
	std::string path1 = "../TestSamples/image.jpg";
	ImageProcess image_process;
	cv::Mat output_img1;
	image_process.setImageFakePath(path1);
	image_process.getImage(0, 1, output_img1);
	cv::namedWindow("graphe_test", cv::WINDOW_NORMAL);
	cv::imshow("graphe_test",output_img1);
	//cv::waitKey();

	std::string path2 = "../TestSamples/text.jpg";
	cv::Mat output_img2;
	image_process.setImageFakePath(path2);
	image_process.getImage(0, 0, output_img2);
	cv::imshow("text_test", output_img2);
	cv::waitKey();
}

int main() {
	testImageProcess();
	return 0;
}