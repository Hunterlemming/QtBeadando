#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidgetItem>
#include <QMainWindow>
#include <QDebug>
#include <QPixmap>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "bigimagedialog.h"
#include "imagedata.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_browseButton_clicked();

    void on_deleteButton_clicked();

    void on_fullScreenButton_clicked();

    void on_urlList_itemClicked(QListWidgetItem *item);

    void on_categoryButton_clicked();

    void on_noteButton_clicked();

private:
    Ui::MainWindow *ui;
    BigImageDialog *bigImage;

    QSqlDatabase db;
    ImageData current_image;
    QList<ImageData> images;

    void showImage(ImageData image);
    void load(QString name);
    void initDb();
    void update();
    ImageData imageAtUrl(QString url);
    int getCBoxIndex(QString category);
};

#endif // MAINWINDOW_H
