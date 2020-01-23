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
