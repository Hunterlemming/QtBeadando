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
    /*int width = ui->imageLabel->width();
    int height = ui->imageLabel->height();*/
    ui->imageLabel->setPixmap(QPixmap(imageUrl));
}
