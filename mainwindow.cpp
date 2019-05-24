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

void MainWindow::showImage(QString name)
{
    current_image = name;
    ui->imageLabel->setPixmap(QPixmap(current_image).scaled(500,500,Qt::KeepAspectRatio));
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
    if (!query.exec("CREATE TABLE images (url VARCHAR(100) PRIMARY KEY)")){
        qDebug() << "Error: " << query.lastError();
    }

}

void MainWindow::update()
{
    images.clear();
    ui->savedList->clear();
    QSqlQuery query(db);
    query.exec("SELECT * FROM images");
    int i=0;
    while (query.next())
    {
        ui->savedList->addItem(query.value("url").toString());
        images.push_back(query.value("url").toString());
        i++;
    }
    if (i!=0) {
        ui->savedList->setCurrentRow(0);
        showImage(ui->savedList->currentItem()->text());
    }
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
            showImage(file_name);
            ui->savedList->setCurrentRow(ui->savedList->count()-1);
        } else {
            QMessageBox::information(this,"Warning","This is not an image!");
        }
    }

}

void MainWindow::on_savedList_itemClicked(QListWidgetItem *item)
{
    showImage(item->text());
}

void MainWindow::on_deleteButton_clicked()
{
    QSqlQuery query(db);
    query.exec("DELETE FROM images WHERE url='"+ current_image +"'");
    update();
}
