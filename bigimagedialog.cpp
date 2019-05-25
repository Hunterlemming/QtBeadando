#include "bigimagedialog.h"
#include "ui_bigimagedialog.h"

BigImageDialog::BigImageDialog(QWidget *parent, QString imageUrl) :
    QDialog(parent),
    ui(new Ui::BigImageDialog)
{
    this->imageUrl=imageUrl;
    ui->setupUi(this);
    load();
}

BigImageDialog::~BigImageDialog()
{
    delete ui;
}

void BigImageDialog::load()
{
    QPixmap picture = QPixmap(imageUrl);
    if (1280<picture.width() || 720<picture.height()){
        ui->imageLabel->setPixmap(picture.scaled(1280,720,Qt::KeepAspectRatio));
    } else ui->imageLabel->setPixmap(picture);
}
