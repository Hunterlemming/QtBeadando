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

    void on_savedList_itemClicked(QListWidgetItem *item);

    void on_deleteButton_clicked();

private:
    Ui::MainWindow *ui;

    QSqlDatabase db;
    QString current_image;
    QList<QString> images;

    void showImage(QString name);
    void load(QString name);
    void initDb();
    void update();
};

#endif // MAINWINDOW_H
