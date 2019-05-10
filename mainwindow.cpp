#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->savedList->addItems(images);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_browseButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,"Open a file",QDir::homePath());
    QString extension = file_name.split(".")[1];

    if (extension.compare("png")==0 || extension.compare("jpg")==0 || extension.compare("jpeg")==0) {
        current_image = file_name;
        ui->imageLabel->setPixmap(QPixmap(current_image));
    } else {
        QMessageBox::information(this,"Warning","This is not an image!");
    }

}

void MainWindow::on_saveButton_clicked()
{
    QString save_success = isValidNewImage();
    if (save_success.compare("valid")==0) {
        images.push_back(current_image);
        ui->savedList->addItem(current_image);
    } else {
        QMessageBox::information(this,"Warning",save_success);
    }
}

QString MainWindow::isValidNewImage()
{
    QString valid = "valid";

    if (current_image.compare("")==0) valid = "Choose an image to save!";
    for (int i=0; i<images.size(); i++){
        if (current_image.compare(images.at(i))==0) valid = "Image is already saved!";
    }

    return valid;
}


