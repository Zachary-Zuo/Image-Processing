#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ImageProcessing.h"

void MainWindow::iniUI()
{
	setCentralWidget(ui->tabWidget);
	QVBoxLayout* pageVLayout = new QVBoxLayout;
	pageVLayout->addWidget(ui->grayscalePushButton);
	pageVLayout->addWidget(ui->radioButton);
	ui->page->setLayout(pageVLayout);
}


MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	iniUI();
	//loadSettings();
}

MainWindow::~MainWindow()
{
	delete ui;
}





//void MainWindow::on_inputPushButton_pressed()
//{
//    QString fileName = QFileDialog::getOpenFileName(this,
//        "Open Input Image",
//        QDir::currentPath(),
//        "Images (*.jpg *.png *.bmp)");
//    if (QFile::exists(fileName))
//    {
//        ui->inputLineEdit->setText(fileName);
//    }
//}

//void MainWindow::on_outputPushButton_pressed()
//{
//    QString fileName = QFileDialog::getOpenFileName(this,
//        "Select Output Image",
//        QDir::currentPath(),
//        "*.jpg;;*.png;;*.bmp");
//    if (!fileName.isEmpty())
//    {
//        ui->outputLineEdit->setText(fileName);
//        cv::Mat inImg, outImg;
//        inImg = cv::imread(ui->inputLineEdit->text().toStdString());
//        cv::imshow("image",inImg);
//    }
//}

//void MainWindow::saveSettings()
//{
//    QSettings settings("Packt",
//        "Image Processing",
//        this);
//    settings.setValue("inputLineEdit",
//        ui->inputLineEdit->text()
//    );
//    settings.setValue("outputLineEdit",
//        ui->outputLineEdit->text()
//    );
//}

//void MainWindow::loadSettings()
//{
//    QSettings settings("Packt",
//        "Image Processing",
//        this);
//    ui->inputLineEdit->setText(settings.value("inputLineEdit",
//        "").toString());
//    ui->outputLineEdit->setText(settings.value("outputLineEdit",
//        "").toString());
//}

//void MainWindow::closeEvent(QCloseEvent *event)
//{
//    int result = QMessageBox::warning(this,
//        "Exit",
//        "Are you sure you want to close this program?",
//        QMessageBox::Yes,
//        QMessageBox::No);
//    if (result == QMessageBox::Yes)
//    {
//        saveSettings();
//        event->accept();
//    }
//    else
//    {
//        event->ignore();
//    }
//}

void MainWindow::displayImage(QLabel* outputLabel, const QImage& image)
{
	outputLabel->setFixedWidth(image.width());
	outputLabel->setFixedHeight(image.height());
	/*QImage qimage(image.data,
		image.cols,
		image.rows,
		image.step,
		QImage::Format_RGB888);*/
	QSize picSize(outputLabel->width(), outputLabel->height());
	QPixmap outputQImage = QPixmap::fromImage(image.rgbSwapped()).scaled(picSize, Qt::KeepAspectRatio);
	outputLabel->setPixmap(outputQImage);
}

void MainWindow::displayColorImage(QLabel* outputLabel, const cv::Mat& image)
{
	if (image.empty())return;
	QImage qimage(image.data,
		image.cols,
		image.rows,
		image.step,
		QImage::Format_RGB888);
	displayImage(outputLabel, qimage);
}

void MainWindow::displayGrayImage(QLabel* outputLabel, const cv::Mat& image)
{
	if (image.empty())return;

	QImage qimage = QImage((const unsigned char*)(image.data),
		image.cols,
		image.rows,
		image.cols * image.channels(),
		QImage::Format_Grayscale8);
	displayImage(outputLabel, qimage);
}

void MainWindow::displayImageAndLabel(QLabel* outputLabel, QLabel* outputExplainLabel, const cv::Mat& image, const QString& text)
{
	if (image.type() == CV_8UC1)
	{
		displayGrayImage(outputLabel, image);
	}
	else
	{
		displayColorImage(outputLabel, image);
	}
	outputExplainLabel->setText(text);
}

cv::Mat MainWindow::openImage()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		"Select the Image",
		QDir::currentPath(),
		"*.jpg;;*.png;;*.bmp");
	if (!fileName.isEmpty())
	{
		cv::Mat image = cv::imread(fileName.toStdString());
		return image;
	}
	else
	{
		return cv::Mat();
	}
}

void MainWindow::on_Open_triggered()
{
	inputImage = openImage();
	if (!inputImage.empty())
	{
		displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
			inputImage, codec->toUnicode("原图"));
	}
}

void MainWindow::on_histogramRadioButton_pressed()
{
	if (ui->Iterative->isChecked())
	{
		if (outputImage.empty())
		{
			currentImage = inputImage;
		}
		else
		{
			currentImage = outputImage;
		}

	}
	else
	{
		currentImage = inputImage;
	}
	if (currentImage.empty())
	{
		int result = QMessageBox::warning(this,
			"Warning",
			codec->toUnicode("您还未打开图片，是否现在打开需要处理的图片?"),
			QMessageBox::Yes,
			QMessageBox::No);
		if (result == QMessageBox::Yes)
		{
			inputImage = openImage();
			currentImage = inputImage;
			if (!currentImage.empty())
			{
				displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
					currentImage, codec->toUnicode("原图"));
				outputImage = getImageOfHistogram(currentImage);
				displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
					outputImage, codec->toUnicode("直方图"));
			}
			ui->histogramRadioButton->setChecked(true);
		}

	}
	else
	{
		displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
			currentImage, codec->toUnicode("原图"));
		outputImage = getImageOfHistogram(currentImage);
		displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
			outputImage, codec->toUnicode("直方图"));
	}
}

void MainWindow::on_Save_triggered()
{
	if (outputImage.empty())
	{
		QMessageBox::warning(this,
			"Warning",
			codec->toUnicode("您还处理过图片，请先处理图片"),
			QMessageBox::Yes,
			QMessageBox::No);
	}
	else
	{
		QString fileName = QFileDialog::getSaveFileName(this,
			"Save the Image",
			QDir::currentPath(),
			"*.jpg;;*.png;;*.bmp");
		cv::imwrite(fileName.toStdString(), outputImage);
	}
}
void MainWindow::on_edrodeRadioButton_pressed()
{
	if (ui->Iterative->isChecked())
	{
		if (outputImage.empty())
		{
			currentImage = inputImage;
		}
		else
		{
			currentImage = outputImage;
		}

	}
	else
	{
		currentImage = inputImage;
	}
	if (currentImage.empty())
	{
		int result = QMessageBox::warning(this,
			"Warning",
			codec->toUnicode("您还未打开图片，是否现在打开需要处理的图片?"),
			QMessageBox::Yes,
			QMessageBox::No);
		if (result == QMessageBox::Yes)
		{
			inputImage = openImage();
			currentImage = inputImage;
			if (!currentImage.empty())
			{
				displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
					currentImage, codec->toUnicode("原图"));
				cv::erode(currentImage, outputImage, cv::Mat());
				displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
					outputImage, codec->toUnicode("腐蚀图像"));
			}
			ui->edrodeRadioButton->setChecked(true);
		}
	}
	else
	{
		displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
			currentImage, codec->toUnicode("原图"));
		cv::erode(currentImage, outputImage, cv::Mat());
		displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
			outputImage, codec->toUnicode("腐蚀图像"));
	}
}

void MainWindow::on_dilateRadioButton_pressed()
{
	if (ui->Iterative->isChecked())
	{
		if (outputImage.empty())
		{
			currentImage = inputImage;
		}
		else
		{
			currentImage = outputImage;
		}

	}
	else
	{
		currentImage = inputImage;
	}
	if (currentImage.empty())
	{
		int result = QMessageBox::warning(this,
			"Warning",
			codec->toUnicode("您还未打开图片，是否现在打开需要处理的图片?"),
			QMessageBox::Yes,
			QMessageBox::No);
		if (result == QMessageBox::Yes)
		{
			inputImage = openImage();
			currentImage = inputImage;
			if (!currentImage.empty())
			{
				displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
					currentImage, codec->toUnicode("原图"));
				cv::dilate(currentImage, outputImage, cv::Mat());
				displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
					outputImage, codec->toUnicode("膨胀图像"));
			}
			ui->dilateRadioButton->setChecked(true);
		}

	}
	else
	{
		displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
			currentImage, codec->toUnicode("原图"));
		cv::dilate(currentImage, outputImage, cv::Mat());
		displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
			outputImage, codec->toUnicode("膨胀图像"));
	}
}

void MainWindow::on_restorePushButton_pressed()
{
	currentImage = inputImage;
	outputImage = cv::Mat();
	displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
		currentImage, codec->toUnicode("原图"));
	ui->histogramRadioButton->setCheckable(false);
	ui->histogramRadioButton->setCheckable(true);
	ui->edrodeRadioButton->setCheckable(false);
	ui->edrodeRadioButton->setCheckable(true);
	ui->dilateRadioButton->setCheckable(false);
	ui->dilateRadioButton->setCheckable(true);
}

void MainWindow::on_colorInversionRadioButton_pressed()
{
	if (ui->Iterative->isChecked())
	{
		if (outputImage.empty())
		{
			currentImage = inputImage;
		}
		else
		{
			currentImage = outputImage;
		}
	}
	else
	{
		currentImage = inputImage;
	}
	if (currentImage.empty())
	{
		int result = QMessageBox::warning(this,
			"Warning",
			codec->toUnicode("您还未打开图片，是否现在打开需要处理的图片?"),
			QMessageBox::Yes,
			QMessageBox::No);
		if (result == QMessageBox::Yes)
		{
			inputImage = openImage();
			currentImage = inputImage;
			if (!currentImage.empty())
			{
				displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
					currentImage, codec->toUnicode("原图"));
				colorReverse(currentImage, outputImage);
				displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
					outputImage, codec->toUnicode("反色图像"));
			}
			ui->colorInversionRadioButton->setChecked(true);
		}

	}
	else
	{
		displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
			currentImage, codec->toUnicode("原图"));
		colorReverse(currentImage, outputImage);
		displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
			outputImage, codec->toUnicode("反色图像"));
	}
}

void MainWindow::on_loseColorRadioButton_pressed()
{
	if (ui->Iterative->isChecked())
	{
		if (outputImage.empty())
		{
			currentImage = inputImage;
		}
		else
		{
			currentImage = outputImage;
		}
	}
	else
	{
		currentImage = inputImage;
	}
	if (currentImage.empty())
	{
		int result = QMessageBox::warning(this,
			"Warning",
			codec->toUnicode("您还未打开图片，是否现在打开需要处理的图片?"),
			QMessageBox::Yes,
			QMessageBox::No);
		if (result == QMessageBox::Yes)
		{
			inputImage = openImage();
			currentImage = inputImage;
			if (!currentImage.empty())
			{
				displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
					currentImage, codec->toUnicode("原图"));
				colorReduce(currentImage, outputImage);
				displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
					outputImage, codec->toUnicode("减色图像"));
			}
			ui->loseColorRadioButton->setChecked(true);
		}

	}
	else
	{
		displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
			currentImage, codec->toUnicode("原图"));
		colorReduce(currentImage, outputImage);
		displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
			outputImage, codec->toUnicode("减色图像"));
	}
}


void MainWindow::on_GrayscaleRadioButton_pressed()
{
	if (ui->Iterative->isChecked())
	{
		if (outputImage.empty())
		{
			currentImage = inputImage;
		}
		else
		{
			currentImage = outputImage;
		}
	}
	else
	{
		currentImage = inputImage;
	}
	if (currentImage.empty())
	{
		int result = QMessageBox::warning(this,
			"Warning",
			codec->toUnicode("您还未打开图片，是否现在打开需要处理的图片?"),
			QMessageBox::Yes,
			QMessageBox::No);
		if (result == QMessageBox::Yes)
		{
			inputImage = openImage();
			currentImage = inputImage;
			if (!currentImage.empty())
			{
				displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
					currentImage, codec->toUnicode("原图"));
				if (currentImage.type() != CV_8UC3)
				{
					QMessageBox::warning(this,
						"Warning",
						codec->toUnicode("当前图片不是彩色图片，无法进行灰度化，请重新尝试"),
						QMessageBox::Yes,
						QMessageBox::No);
					ui->GrayscaleRadioButton->setChecked(false);
					return;
				}
				
				cv::cvtColor(currentImage, outputImage, CV_BGR2GRAY);
				displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
					outputImage, codec->toUnicode("灰度图像"));
			}
			ui->GrayscaleRadioButton->setChecked(true);
		}

	}
	else
	{
		displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
			currentImage, codec->toUnicode("原图"));
		if (currentImage.type() != CV_8UC3)
		{
			QMessageBox::warning(this,
				"Warning",
				codec->toUnicode("当前图片不是彩色图片，无法进行灰度化，请重新尝试"),
				QMessageBox::Yes,
				QMessageBox::No);
			ui->GrayscaleRadioButton->setChecked(false);
			return;
		}
		cv::cvtColor(currentImage, outputImage, CV_BGR2GRAY);
		displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
			outputImage, codec->toUnicode("灰度图像"));
	}
}

void MainWindow::on_equalizeHistRadioButton_pressed()
{
	if (ui->Iterative->isChecked())
	{
		if (outputImage.empty())
		{
			currentImage = inputImage;
		}
		else
		{
			currentImage = outputImage;
		}
	}
	else
	{
		currentImage = inputImage;
	}
	if (currentImage.empty())
	{
		int result = QMessageBox::warning(this,
			"Warning",
			codec->toUnicode("您还未打开图片，是否现在打开需要处理的图片?"),
			QMessageBox::Yes,
			QMessageBox::No);
		if (result == QMessageBox::Yes)
		{
			inputImage = openImage();
			inputImage.copyTo(currentImage);
			//currentImage = inputImage;
			if (!currentImage.empty())
			{
				displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
					currentImage, codec->toUnicode("原图"));
				if (currentImage.type() == CV_8UC3)
				{
					int equalize = QMessageBox::warning(this,
						"Warning",
						codec->toUnicode("当前图片不是灰度图片，无法进行直方图均衡，是否先转化为灰度图？"),
						QMessageBox::Yes,
						QMessageBox::No);
					if (equalize == QMessageBox::Yes)
					{
						cv::cvtColor(currentImage, currentImage, CV_BGR2GRAY);
						displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
							currentImage, codec->toUnicode("原图"));
					}
				}
				cv::equalizeHist(currentImage, outputImage);
				displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
					outputImage, codec->toUnicode("直方图均衡"));
			}
			ui->equalizeHistRadioButton->setChecked(true);
		}

	}
	else
	{
		displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
			currentImage, codec->toUnicode("原图"));
		if (currentImage.type() == CV_8UC3)
		{
			int equalize = QMessageBox::warning(this,
				"Warning",
				codec->toUnicode("当前图片不是灰度图片，无法进行直方图均衡，是否先转化为灰度图？"),
				QMessageBox::Yes,
				QMessageBox::No);
			if (equalize == QMessageBox::Yes)
			{
				cv::cvtColor(currentImage, currentImage, CV_BGR2GRAY);
				displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
					currentImage, codec->toUnicode("原图"));
			}
		}
		cv::equalizeHist(currentImage, outputImage);
		displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
			outputImage, codec->toUnicode("直方图均衡"));
		ui->equalizeHistRadioButton->setChecked(true);
	}
}

void MainWindow::on_adaptiveThresholdRadioButton_pressed()
{
	if (ui->Iterative->isChecked())
	{
		if (outputImage.empty())
		{
			currentImage = inputImage;
		}
		else
		{
			currentImage = outputImage;
		}
	}
	else
	{
		currentImage = inputImage;
	}
	if (currentImage.empty())
	{
		int result = QMessageBox::warning(this,
			"Warning",
			codec->toUnicode("您还未打开图片，是否现在打开需要处理的图片?"),
			QMessageBox::Yes,
			QMessageBox::No);
		if (result == QMessageBox::Yes)
		{
			inputImage = openImage();
			inputImage.copyTo(currentImage);
			//currentImage = inputImage;
			if (!currentImage.empty())
			{
				displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
					currentImage, codec->toUnicode("原图"));
				if (currentImage.type() == CV_8UC3)
				{
					int equalize = QMessageBox::warning(this,
						"Warning",
						codec->toUnicode("当前图片不是灰度图片，无法进行直方图均衡，是否先转化为灰度图？"),
						QMessageBox::Yes,
						QMessageBox::No);
					if (equalize == QMessageBox::Yes)
					{
						cv::cvtColor(currentImage, currentImage, CV_BGR2GRAY);
						displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
							currentImage, codec->toUnicode("原图"));
					}
				}
				cv::adaptiveThreshold(currentImage,
					outputImage,
					255,
					cv::ADAPTIVE_THRESH_MEAN_C,
					cv::THRESH_BINARY,
					5,
					2);
				displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
					outputImage, codec->toUnicode("自适应阈值分割"));
			}
			ui->adaptiveThresholdRadioButton->setChecked(true);
		}

	}
	else
	{
		displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
			currentImage, codec->toUnicode("原图"));
		if (currentImage.type() == CV_8UC3)
		{
			int equalize = QMessageBox::warning(this,
				"Warning",
				codec->toUnicode("当前图片不是灰度图片，无法进行直方图均衡，是否先转化为灰度图？"),
				QMessageBox::Yes,
				QMessageBox::No);
			if (equalize == QMessageBox::Yes)
			{
				cv::cvtColor(currentImage, currentImage, CV_BGR2GRAY);
				displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
					currentImage, codec->toUnicode("原图"));
			}
		}
		cv::adaptiveThreshold(currentImage,
			outputImage,
			255,
			cv::ADAPTIVE_THRESH_MEAN_C,
			cv::THRESH_BINARY,
			5,
			2);
		displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
			outputImage, codec->toUnicode("自适应阈值分割"));
	}
	ui->adaptiveThresholdRadioButton->setChecked(true);
}
