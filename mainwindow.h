#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private:
    Ui::MainWindow *ui;

    QString current_image;
    QList<QString> images;

    QString isValidNewImage();
};

#endif // MAINWINDOW_H
