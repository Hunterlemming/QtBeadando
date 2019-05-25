#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    load("db.sqlite");
    update();

}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}

void MainWindow::showImage(ImageData image)
{
    current_image = image;
    ui->imageLabel->setPixmap(QPixmap(current_image.getUrl()).scaled(500,500,Qt::KeepAspectRatio));
    if (current_image.getCategory().compare("")==0) ui->categoryCBox->setCurrentIndex(0);
    else ui->categoryCBox->setCurrentIndex(getCBoxIndex(current_image.getCategory()));
    ui->noteTextEdit->setText(current_image.getNote());
}

int MainWindow::getCBoxIndex(QString category)
{
    if (category.at(0)=='1') return 1;
    else if (category.at(0)=='2') return 2;
    else if (category.at(0)=='3') return 3;
    else if (category.at(0)=='4') return 4;
    else if (category.at(0)=='5') return 5;
    else return 6;
}

void MainWindow::load(QString name)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(name);

    if (db.open()){
        qDebug() << "DB is open";

        if (db.tables().size()==0) {
            qDebug() << "Empty database";
            initDb();
        }
    } else {
        QMessageBox::information(this,"Warning","Could not load saved images!");
    }
}

void MainWindow::initDb()
{
    QSqlQuery query(db);
    if (!query.exec("CREATE TABLE images (url VARCHAR(100) PRIMARY KEY, category VARCAR(60), note TEXT)")){
        qDebug() << "Error: " << query.lastError();
    }

}

void MainWindow::update(QString sortName, QString sortCategory)
{
    images.clear();
    ui->urlList->clear();
    QSqlQuery query(db);

    if (sortName.compare("")==0 && sortCategory.compare("")==0) {
        query.exec("SELECT * FROM images");
        ui->nameLineEdit_sort->setText("");
        ui->categoryCBox_sort->setCurrentIndex(0);
    }
    else if (sortName.compare("")!=0 && sortCategory.compare("")==0) query.exec("SELECT * FROM images WHERE url LIKE '%"+sortName+"%'");
    else if (sortName.compare("")==0 && sortCategory.compare("")!=0) query.exec("SELECT * FROM images WHERE category='"+sortCategory+"'");
    else query.exec("SELECT * FROM images WHERE url LIKE '%"+sortName+"%' AND category='"+sortCategory+"'");

    int i=0;
    while (query.next())
    {
        QString url = query.value("url").toString();
        QString category = query.value("category").toString();
        QString note = query.value("note").toString();

        ui->urlList->addItem(url);
        images.push_back(ImageData(url,category,note));
        i++;
    }
    if (i!=0) {
        ui->urlList->setCurrentRow(0);
        showImage(images.at(0));
    }
}

ImageData MainWindow::imageAtUrl(QString url)
{
    int i=0;
    bool found = false;
    while (i<images.size() && found==false) {
        if (images.at(i).getUrl().compare(url)==0) found = true;
        i++;
    }
    if (found==true) return images.at(i-1);
    else return ImageData("x","x","x");
}

void MainWindow::on_browseButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,"Open a file",QDir::homePath());
    if (file_name!="") {
        QString extension = file_name.split(".")[1];

        if (extension.compare("png")==0 || extension.compare("jpg")==0 || extension.compare("jpeg")==0) {
            QSqlQuery query(db);
            query.exec("INSERT INTO images (url) VALUES ('"+ file_name +"')");
            update();
            showImage(imageAtUrl(file_name));
            ui->urlList->setCurrentRow(ui->urlList->count()-1);
        } else {
            QMessageBox::information(this,"Warning","This is not an image!");
        }
    }

}

void MainWindow::on_deleteButton_clicked()
{
    QSqlQuery query(db);
    query.exec("DELETE FROM images WHERE url='"+ current_image.getUrl() +"'");
    update();
}

void MainWindow::on_fullScreenButton_clicked()
{
    bigImage = new BigImageDialog(this,current_image.getUrl());
    bigImage->show();
}

void MainWindow::on_urlList_itemClicked(QListWidgetItem *item)
{
    showImage(imageAtUrl(item->text()));
}

void MainWindow::on_categoryButton_clicked()
{
    QString current_url = current_image.getUrl();
    QString category = ui->categoryCBox->currentText();
    QSqlQuery query(db);
    query.exec("UPDATE images SET category='"+category+"' where url='"+ current_url +"'");
    update();
}

void MainWindow::on_noteButton_clicked()
{
    QString current_url = current_image.getUrl();
    QString note = ui->noteTextEdit->toPlainText();
    QSqlQuery query(db);
    query.exec("UPDATE images SET note='"+note+"' where url='"+ current_url +"'");
    update();
}

void MainWindow::on_pushButton_clicked()
{
    QString nameIsLike = ui->nameLineEdit_sort->text();
    QString categoryIs = ui->categoryCBox_sort->currentText();
    if (ui->categoryCBox_sort->currentIndex()==0) categoryIs="";
    update(nameIsLike,categoryIs);
}
