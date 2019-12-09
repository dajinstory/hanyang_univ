//file I/O and resize with interpolation
#include <opencv2/opencv.hpp>
#include <iostream>

//�����ϴ� �κ� ���� ����
void BilinearInterpolation(cv::Mat &src, cv::Mat &dst);
cv::Mat bi_dst;

int main()
{
	cv::Mat src;
	src = cv::imread("***your path***", 1);  //���� ��� Ȥ�� ��� ���(0�� gray scale)
	if (src.empty())
	{
		std::cout << "Cannot find an image" << std::endl;
		return -1;
	}
	cv::imshow("SourceImage", src);
	cv::waitKey(0);

	//�̹����� 2�� Ű���� ���� Bilinear interpolation
	int height = src.rows;
	int width = src.cols;
	int scale_rate = 2;
	bi_dst = cv::Mat(height, width, src.type(), cv::Scalar(0)); //������� ���� Mat

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
