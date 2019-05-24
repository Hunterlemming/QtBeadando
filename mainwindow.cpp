#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QPixmap>
#include <QTextStream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    load();
}

MainWindow::~MainWindow()
{
    save();
    delete ui;
}

void MainWindow::on_browseButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,"Open a file",QDir::homePath());
    if (file_name!="") {
        QString extension = file_name.split(".")[1];

        if (extension.compare("png")==0 || extension.compare("jpg")==0 || extension.compare("jpeg")==0) {
            showImage(file_name);
        } else {
            QMessageBox::information(this,"Warning","This is not an image!");
        }
    }

}

void MainWindow::showImage(QString name)
{
    current_image = name;
    ui->imageLabel->setPixmap(QPixmap(current_image).scaled(500,500,Qt::KeepAspectRatio));
}

void MainWindow::load()
{
    QFile file(":/resources/res/savedImages.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream input (&file);

        input.seek(0);
        while (!input.atEnd()) {
            QString image = input.readLine();
            images.push_back(image);
            ui->savedList->addItem(image);
        }
        file.close();
    } else {
        QMessageBox::information(this,"Warning","Could not load saved images!");
    }
}

void MainWindow::save()
{

    qDebug() << "a";
    QFile file(":/resources/res/savedImages.txt");
    qDebug() << file.isWritable();
    if (file.open(QFile::WriteOnly | QFile::Text)){

        qDebug() << "a";
        QTextStream output (&file);

        qDebug() << "a";
        for (int i=0; i<images.size(); i++){
            output << images.at(i) << "/n";
        }
        qDebug() << "a";

        file.close();
    } else {
        QMessageBox::information(this,tr("Warning"),"Could not save images!");
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

void MainWindow::on_savedList_itemClicked(QListWidgetItem *item)
{
    showImage(item->text());
}

void MainWindow::on_pushButton_clicked()
{
    if (images.size()!=0) {
        for (int i = 0; i < images.size(); i++) {
            if (images.at(i).compare(current_image)==0) {
                images.removeAt(i);
                ui->savedList->takeItem(i);
            }
        }
        if (images.size()!=0) showImage(images.at(0));
    } else {
        QMessageBox::information(this,"Warning","Choose an image to remove!");
    }
}
