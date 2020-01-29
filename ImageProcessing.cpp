#include "ImageProcessing.h"



cv::Mat getImageOfHistogram(const cv::Mat& image) {
	if (image.type() == CV_8UC3)
	{
		// Divided into three channels
		std::vector<cv::Mat> channels;
		cv::split(image, channels);

		// Set the number of bin
		int histBinNum = 255;

		// Set value range
		float range[] = { 0, 255 };
		const float* histRange = { range };

		bool uniform = true;
		bool accumulate = false;

		// Declare hist array of three channels
		cv::Mat redHist, greenHist, blueHist;

		// Calculate the histogram
		cv::calcHist(&channels[0], 1, 0, cv::Mat(), blueHist, 1, &histBinNum, &histRange, uniform, accumulate);
		cv::calcHist(&channels[1], 1, 0, cv::Mat(), greenHist, 1, &histBinNum, &histRange, uniform, accumulate);
		cv::calcHist(&channels[2], 1, 0, cv::Mat(), redHist, 1, &histBinNum, &histRange, uniform, accumulate);

		// get histogram size
		int histSize = redHist.rows;

		cv::Mat histImage(histSize * 2, histSize * 3, CV_8UC3, cv::Scalar(255, 255, 255));

		// set highest point at 90% of nbins (i.e. image height)
		int hpt = static_cast<int>(0.9 * histSize);

		double maxVal[3];
		double minVal[3];

		// Get min and max bin values
		cv::minMaxLoc(redHist, &minVal[0], &maxVal[0], 0, 0);
		cv::minMaxLoc(greenHist, &minVal[1], &maxVal[1], 0, 0);
		cv::minMaxLoc(blueHist, &minVal[2], &maxVal[2], 0, 0);

		// Draw the histogram
		for (int i = 1; i < histSize; i++)
		{
			float leftBinVal = redHist.at<float>(i - 1);
			float rightBinVal = redHist.at<float>(i);
			int leftIntensity = static_cast<int>(leftBinVal * hpt / maxVal[0]);
			int rightIntensity = static_cast<int>(rightBinVal * hpt / maxVal[0]);

			cv::line(histImage, cv::Point((i - 1) * 3, 2 * histSize - leftIntensity), cv::Point(i * 3, 2 * histSize - rightIntensity),
				cv::Scalar(0, 0, 255), 2, 8, 0);


			leftBinVal = greenHist.at<float>(i - 1);
			rightBinVal = greenHist.at<float>(i);
			leftIntensity = static_cast<int>(leftBinVal * hpt / maxVal[1]);
			rightIntensity = static_cast<int>(rightBinVal * hpt / maxVal[1]);

			cv::line(histImage, cv::Point((i - 1) * 3, 2 * histSize - leftIntensity), cv::Point(i * 3, 2 * histSize - rightIntensity),
				cv::Scalar(0, 255, 0), 2, 8, 0);

			leftBinVal = blueHist.at<float>(i - 1);
			rightBinVal = blueHist.at<float>(i);
			leftIntensity = static_cast<int>(leftBinVal * hpt / maxVal[2]);
			rightIntensity = static_cast<int>(rightBinVal * hpt / maxVal[2]);

			cv::line(histImage, cv::Point((i - 1) * 3, 2 * histSize - leftIntensity), cv::Point(i * 3, 2 * histSize - rightIntensity),
				cv::Scalar(255, 0, 0), 2, 8, 0);
		}

		// Draw vertical line for each bin in each channel
		for (int h = 0; h < histSize; h++) {

			float binVal = redHist.at<float>(h);
			if (binVal > 0) {
				int intensity = static_cast<int>(binVal * hpt / maxVal[0]);
				cv::line(histImage, cv::Point(histSize * 2 + h, histSize), cv::Point(histSize * 2 + h, histSize - intensity),
					cv::Scalar(0, 0, 255), 2, 8, 0);
			}
		}

		for (int h = 0; h < histSize; h++) {

			float binVal = greenHist.at<float>(h);
			if (binVal > 0) {
				int intensity = static_cast<int>(binVal * hpt / maxVal[1]);
				cv::line(histImage, cv::Point(histSize + h, histSize), cv::Point(histSize + h, histSize - intensity),
					cv::Scalar(0, 255, 0), 2, 8, 0);
			}
		}

		for (int h = 0; h < histSize; h++) {

			float binVal = blueHist.at<float>(h);
			if (binVal > 0) {
				int intensity = static_cast<int>(binVal * hpt / maxVal[2]);
				cv::line(histImage, cv::Point(h, histSize), cv::Point(h, histSize - intensity),
					cv::Scalar(255, 0, 0), 2, 8, 0);
			}
		}
		return histImage;
	}
	else
	{
		// Set the number of bin
		int histBinNum = 255;

		// Set value range
		float range[] = { 0, 255 };
		const float* histRange = { range };

		bool uniform = true;
		bool accumulate = false;

		cv::Mat hist;

		// Calculate the histogram
		cv::calcHist(&image, 1, 0, cv::Mat(), hist, 1, &histBinNum, &histRange, uniform, accumulate);

		// get histogram size
		int histSize = hist.rows;

		cv::Mat histImage(histSize * 2, histSize * 2, CV_8UC1, cv::Scalar(255));

		// set highest point at 90% of nbins (i.e. image height)
		int hpt = static_cast<int>(0.9 * histSize);

		double maxVal;
		double minVal;

		// Get min and max bin values
		cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);

		// Draw vertical line for each bin in each channel
		for (int h = 0; h < histSize; h++) {

			float binVal = hist.at<float>(h);
			if (binVal > 0) {
				int intensity = static_cast<int>(binVal * hpt / maxVal);
				cv::line(histImage, cv::Point(2 * h, 2 * histSize), cv::Point(2 * h, 2 * (histSize - intensity)),
					cv::Scalar(0), 2, 8, 0);
			}
		}
		return histImage;
	}
}

void colorReduce(const cv::Mat& inImage, cv::Mat& outImage, int div)
{
	cv::Mat lookUp(1, 256, CV_8U);
	uchar* p = lookUp.data;
	for (int i = 0; i < 256; i++)
		p[i] = i / div * div + div / 2;
	cv::LUT(inImage, lookUp, outImage);
}

void colorReverse(const cv::Mat& inImage, cv::Mat& outImage)
{
	cv::Mat lookUp(1, 256, CV_8U);
	uchar* p = lookUp.data;
	for (int i = 0; i < 256; i++)
		p[i] = 255 - i;
	cv::LUT(inImage, lookUp, outImage);

	//// unknown reason wrong
	//cv::Mat lookup(1, 256, CV_8U);
	//for (int i = 0; i < 256; i++)
	//{
	//	lookup.at<uchar>(i) = 255 - i;
	//	cv::LUT(inImage, lookup, outImage);
	//}
}

void morphologyOption(const cv::Mat& input, cv::Mat& output, const int morphOperator,const int morphElem,const int mrophSize)
{
	//point 也可以调整
	cv::Mat element = cv::getStructuringElement(morphElem, cv::Size(2 * mrophSize + 1, 2 * mrophSize + 1), cv::Point(mrophSize, mrophSize));
	cv::morphologyEx(input, output,
		morphOperator,
		element);
}