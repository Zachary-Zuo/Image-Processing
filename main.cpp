#include "mainwindow.h"
#include "ImageProcessing.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec* codec = QTextCodec::codecForName("GBK");
	MainWindow w;
	w.show();
	return a.exec();
}
