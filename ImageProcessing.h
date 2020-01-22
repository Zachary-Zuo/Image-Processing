#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ImageProcessing.h"

class ImageProcessing : public QMainWindow
{
	Q_OBJECT

public:
	ImageProcessing(QWidget *parent = Q_NULLPTR);

private:
	Ui::ImageProcessingClass ui;
};
