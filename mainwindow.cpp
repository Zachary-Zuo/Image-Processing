#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ImageProcessing.h"

void MainWindow::iniUI()
{
    setCentralWidget(ui->tabWidget);

}


MainWindow::MainWindow(QWidget *parent)
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

void MainWindow::displayImage(QLabel *outputLabel, const cv::Mat& image)
{
    if (image.empty())return;
    outputLabel->setFixedWidth(image.cols);
    outputLabel->setFixedHeight(image.rows);
    QImage qimage(image.data,
        image.cols,
        image.rows,
        image.step,
        QImage::Format_RGB888);
    QSize picSize(outputLabel->height(), outputLabel->width());
    QPixmap outputQImage = QPixmap::fromImage(qimage.rgbSwapped()).scaled(picSize, Qt::KeepAspectRatio);
    outputLabel->setPixmap(outputQImage);
}

void MainWindow::displayImageAndLabel(QLabel* outputLabel, QLabel* outputExplainLabel, const cv::Mat& image, const char* text)
{
    displayImage(outputLabel, image);
    outputExplainLabel->setText(codec->toUnicode(text));
}

cv::Mat MainWindow::openImage(cv::Mat& image)
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Select Output Image",
        QDir::currentPath(),
        "*.jpg;;*.png;;*.bmp");
    if (!fileName.isEmpty())
    {
        image = cv::imread(fileName.toStdString());
        return image;
    }
}

void MainWindow::on_Open_triggered()
{
    if (!openImage(inputImage).empty())
    {
        displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel, inputImage, "原图");
    }
}

void MainWindow::on_histogramRadioButton_pressed()
{
    if (inputImage.empty())
    {
        int result = QMessageBox::warning(this,
                    "Warning",
                    codec->toUnicode("您还未打开图片，是否先打开需要处理的原始图片？"),
                    QMessageBox::Yes,
                    QMessageBox::No);
        if (result == QMessageBox::Yes)
        {
            if (!openImage(inputImage).empty())
            {
                displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel, inputImage, "原图");
                cv::Mat outputImage = getImageOfHistogram(inputImage);
                displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel, outputImage, "各通道色彩直方图");
            }
        }
    }
    else
    {
        cv::Mat outputImage = getImageOfHistogram(inputImage);
        displayImage(ui->singleOutputLabel, outputImage);
    }
}
