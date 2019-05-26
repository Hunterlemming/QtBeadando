#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QInputDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator trans;
    QStringList langs;

    langs << "English" << "Magyar";
    const QString lang = QInputDialog::getItem(nullptr,"Language","Select a language",langs);

    if (lang.compare("English")==0) {
        trans.load(":ling/progi_en.qm");
    } else {
        trans.load(":ling/progi_hu.qm");
    }

    a.installTranslator(&trans);

    MainWindow w;
    w.show();
    w.setFixedSize(w.size());

    return a.exec();
}
