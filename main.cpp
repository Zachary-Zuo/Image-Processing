#include "mainwindow.h"
#include <QtWidgets/QApplication>
//#include<opencv2/opencv.hpp>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    //cv::Mat image = cv::imread("C:/Users/Admin/Desktop/photo.jpg");
    //cv::imshow("output", image);
	MainWindow w;
	w.show();
	return a.exec();
}
