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



void MainWindow::on_Open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Select Output Image",
        QDir::currentPath(),
        "*.jpg;;*.png;;*.bmp");
    if (!fileName.isEmpty())
    {
        inputImage = cv::imread(fileName.toStdString());
        inputImage = getImageOfHistogram(inputImage);
        ui->label->setFixedWidth(inputImage.cols);
        ui->label->setFixedHeight(inputImage.rows);

        QImage image(inputImage.data,
            inputImage.cols,
            inputImage.rows,
            inputImage.step,
            QImage::Format_RGB888);
        QSize picSize(ui->label->height(),ui->label->width());
        QPixmap outputQImage = QPixmap::fromImage(image.rgbSwapped()).scaled(picSize, Qt::KeepAspectRatio);
        ui->label->setPixmap(outputQImage);
        
    }
}
