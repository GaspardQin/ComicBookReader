#include "../include/image_process.h"
bool ImageProcess::autoAdjustImage(cv::Mat &input_image, cv::Mat &output_image, int image_type_flag) {
    // image_type_flags = 0 for image whose the type is text
    // image_type_flags = 1 for image whose the type is graphics
    // image_type_flags = 2 for image whose the type is raw
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

bool ImageProcess::autoAdjustImageText(cv::Mat &input_image, cv::Mat &output_image)
{
    cv::Mat temp_image;
    /****Transfer color image into gray image****/
    cv::cvtColor(input_image, temp_image, CV_RGB2GRAY);


    /****smooth the image****/
    //could be slow by using GaussianBlur, could be changed to other linear filter.
    cv::GaussianBlur(temp_image, temp_image, cv::Size(5,5), 0, 0);
    //smooth path is included in adaptiveThreshold

    cv::equalizeHist(temp_image, temp_image);


    /****Auto Threhold divise into white(paper) and black(text)****/
    cv::adaptiveThreshold(temp_image, output_image, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 21, 10);
    //int blocksize can only be impaire
    //if slow, change ADAPTIVE_THRESH_GAUSSIAN_C into ADAPTIVE_THRESH_MEAN_C
    //Could use GUI to change blocksize.


    cv::cvtColor(output_image, output_image, CV_GRAY2RGB);

    return true;
}

bool ImageProcess::autoAdjustImageGraphic(cv::Mat &input_image, cv::Mat &output_image)
{

    cv::Mat temp_image;

    /****smooth the image****/
    //could be slow by using GaussianBlur, could be changed to other linear filter.
    cv::GaussianBlur(input_image, temp_image, cv::Size(3, 3), 0, 0);

    /****autojustment by equilize Hist****/
    /*
        std::vector<cv::Mat> channels;
        split(input_image, channels); 		//Split input_image into 3 channels RGB

        //equilize Hist and convert bgr to rgb
        cv::Mat channel_temp;
        cv::equalizeHist(channels[0], channel_temp);
        cv::equalizeHist(channels[2], channels[0]);
        channels[2] = channel_temp;
        cv::equalizeHist(channels[1], channels[1]);

        cv::merge(channels, temp_image);
        */

    /****autojustment by Laplace filter (Increasing local contrast)****/
    cv::Mat kernel = (cv::Mat_<int>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    cv::filter2D(temp_image, temp_image, temp_image.depth(), kernel);
    cv::cvtColor(temp_image, temp_image, CV_BGR2RGB);
    output_image = temp_image;
    return true;
}

bool ImageProcess::loadArchive(std::string path) {
    if(!archive_reader.loadArchivedFiles(path))
        return false;
    page_num_total = archive_reader.getPageNumTotal();
    is_loaded = true;
    return true;
}

bool ImageProcess::getImage(int num, int image_type_flag, cv::Mat &output_image)
{

    cv::Mat input_image;
    if(!archive_reader.loadOneImage(num, input_image)) {
        std::cout<<"Cannot load Image of page "<<num<<std::endl;
        return false;
    }
    if (image_type_flag == 2) { //RAW
        cv::cvtColor(input_image, output_image, CV_BGR2RGB);
        return true;
    }
    if (!autoAdjustImage(input_image, output_image, image_type_flag)){
        std::cout<<"Cannot process the image "<<std::endl;
    }

    return true;

}

int ImageProcess::getPageNumTotal() {
    if(is_loaded==true)
        return page_num_total;
    else return 1;
}
