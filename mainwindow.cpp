#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    on_actionOriginal_triggered();
    connect (ui->urlList,SIGNAL(customContextMenuRequested(QPoint)),SLOT(customMenuRequested(QPoint)));

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

void MainWindow::sort()
{
    QString nameIsLike = ui->nameLineEdit_sort->text();
    QString categoryIs = ui->categoryCBox_sort->currentText();
    if (ui->categoryCBox_sort->currentIndex()==0) categoryIs="";
    update(nameIsLike,categoryIs);
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
    sort();
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

void MainWindow::on_sortButton_clicked()
{
    sort();
}

void MainWindow::customMenuRequested(QPoint pos)
{
    QMenu *menu = new QMenu(this);
        QAction *showBig = new QAction("Show",this);
        connect(showBig,SIGNAL(triggered()),this,SLOT(on_fullScreenButton_clicked()));
        QAction *deleteCurrent = new QAction("Delete",this);
        connect(deleteCurrent,SIGNAL(triggered()),this,SLOT(on_deleteButton_clicked()));
    menu->addAction(showBig);
    menu->addAction(deleteCurrent);
    menu->popup(ui->urlList->viewport()->mapToGlobal(pos));
}

void MainWindow::on_actionBrowse_triggered()
{
    on_browseButton_clicked();
}

void MainWindow::on_actionShow_triggered()
{
    on_fullScreenButton_clicked();
}

void MainWindow::on_actionOriginal_triggered()
{
    this->setStyleSheet("QWidget#backGroundwidget { background-color: white }"

                        "QMenuBar { background-color: #BDBDBD; color: black }"
                        "QMenuBar::item:selected { background-color: #E8E8E8 }"
                        "QMenu { background-color: #BDBDBD; color: black }"
                        "QMenu::item:selected { background-color: #E8E8E8; font-weight: bold }"

                        "QListWidget { background-color: white; color: black }"
                        "QListWidget::item:selected { background-color: #E8E8E8; color: black; font-weight: bold }"
                        "QLineEdit { background-color: white; color: black; selection-background-color: #E8E8E8 }"
                        "QTextEdit { background-color: white; color: black; selection-background-color: #E8E8E8 }"

                        "QComboBox { background-color: #BDBDBD; color: black }"
                        "QComboBox QAbstractItemView { background-color: #BDBDBD; color: black }"
                        "QComboBox::item:selected { background-color: #E8E8E8; color: black; font-weight: bold }"
                        "QPushButton { background-color: #BDBDBD; color: black };");
}

void MainWindow::on_actionDark_triggered()
{
    this->setStyleSheet("QWidget#backGroundwidget { background-color: #444444 }"

                        "QMenuBar { background-color: black; color: #FF8000 }"
                        "QMenuBar::item:selected { color: lightyellow }"
                        "QMenu { background-color: black; color: #FF8000 }"
                        "QMenu::item:selected { color: lightyellow; font-weight: bold }"

                        "QListWidget { background-color: #444444; color: #FF8000 }"
                        "QListWidget::item:selected { background-color: lightyellow; color: #FF8000; font-weight: bold }"
                        "QLineEdit { background-color: #444444; color: #FF8000; selection-background-color: lightyellow }"
                        "QTextEdit { background-color: #444444; color: #FF8000; selection-background-color: lightyellow }"

                        "QComboBox { background-color: black; color: #FF8000 }"
                        "QComboBox QAbstractItemView { background-color: black; color: #FF8000 }"
                        "QComboBox::item:selected { background-color: lightyellow; color: #FF8000; font-weight: bold }"
                        "QPushButton { background-color: black; color: #FF8000 };");
}

void MainWindow::on_actionNeon_triggered()
{ //#FF3500 #FFEC12
    this->setStyleSheet("QWidget#backGroundwidget { background-color: #FFEC12 }"

                        "QMenuBar { background-color: #FF3500; color: #BE00FF }"
                        "QMenuBar::item:selected { color: #00E572 }"
                        "QMenu { background-color: #FF3500; color: #BE00FF }"
                        "QMenu::item:selected { color: #00E572; font-weight: bold }"

                        "QListWidget { background-color: #FFEC12; color: #BE00FF }"
                        "QListWidget::item:selected { background-color: #00E572; color: #BE00FF; font-weight: bold }"
                        "QLineEdit { background-color: #FFEC12; color: #BE00FF; selection-background-color: #00E572 }"
                        "QTextEdit { background-color: #FFEC12; color: #BE00FF; selection-background-color: #00E572 }"

                        "QComboBox { background-color: #FF3500; color: #BE00FF }"
                        "QComboBox QAbstractItemView { background-color: #FF3500; color: #BE00FF }"
                        "QComboBox::item:selected { background-color: #00E572; color: #BE00FF; font-weight: bold }"
                        "QPushButton { background-color: #FF3500; color: #BE00FF };");
}
