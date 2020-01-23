#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_inputPushButton_pressed()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Open Input Image",
        QDir::currentPath(),
        "Images (*.jpg *.png *.bmp)");
    if (QFile::exists(fileName))
    {
        ui->inputLineEdit->setText(fileName);
    }
}

void MainWindow::on_outputPushButton_pressed()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Select Output Image",
        QDir::currentPath(),
        "*.jpg;;*.png;;*.bmp");
    if (!fileName.isEmpty())
    {
        ui->outputLineEdit->setText(fileName);
        cv::Mat inImg, outImg;
        inImg = cv::imread(ui->inputLineEdit->text().toStdString());
        cv::imshow("image",inImg);
    }
}
