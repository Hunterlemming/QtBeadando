#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <QString>

class ImageData
{
public:

    ImageData();

    ImageData(QString url, QString category, QString note);

    QString getUrl() const;

    QString getCategory() const;

    QString getNote() const;

    void setUrl(const QString &value);

    void setCategory(const QString &value);

    void setNote(const QString &value);

private:
    QString url;
    QString category;
    QString note;
};

#endif // IMAGEDATA_H
