#ifndef BIGIMAGEDIALOG_H
#define BIGIMAGEDIALOG_H

#include <QDialog>
#include <QPixmap>

namespace Ui {
class BigImageDialog;
}

class BigImageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BigImageDialog(QWidget *parent = nullptr, QString imageUrl = "");
    ~BigImageDialog();

private:
    Ui::BigImageDialog *ui;
    QString imageUrl;

    void load();
};

#endif // BIGIMAGEDIALOG_H
