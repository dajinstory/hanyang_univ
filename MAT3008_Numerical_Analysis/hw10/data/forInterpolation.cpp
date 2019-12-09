//file I/O and resize with interpolation
#include <opencv2/opencv.hpp>
#include <iostream>

//보간하는 부분 변경 가능
void BilinearInterpolation(cv::Mat &src, cv::Mat &dst);
cv::Mat bi_dst;

int main()
{
	cv::Mat src;
	src = cv::imread("***your path***", 1);  //절대 경로 혹은 상대 경로(0은 gray scale)
	if (src.empty())
	{
		std::cout << "Cannot find an image" << std::endl;
		return -1;
	}
	cv::imshow("SourceImage", src);
	cv::waitKey(0);

	//이미지를 2배 키웠을 때의 Bilinear interpolation
	int height = src.rows;
	int width = src.cols;
	int scale_rate = 2;
	bi_dst = cv::Mat(height, width, src.type(), cv::Scalar(0)); //결과물을 담을 Mat

	//bilinearInterpolation
	BilinearInterpolation(src, bi_dst);
	cv::imshow("BIImage", bi_dst);
	cv::waitKey(0);

	return 0;
}

void BilinearInterpolation(cv::Mat &src, cv::Mat &dst)
{
	///////////////////////////////////////////////
	//bilinear interpolation
	

	///////////////////////////////////////////////
}
