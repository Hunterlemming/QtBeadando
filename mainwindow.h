#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidgetItem>
#include <QMainWindow>

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

    void on_saveButton_clicked();

    void on_pushButton_clicked();

    void on_savedList_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;

    QString current_image;
    QList<QString> images;

    QString isValidNewImage();
    void showImage(QString name);
    void load();
    void save();
};

#endif // MAINWINDOW_H
