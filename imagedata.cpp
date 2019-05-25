#include "imagedata.h"

ImageData::ImageData()
{

}

ImageData::ImageData(QString url, QString category, QString note)
{
    this->url=url;
    this->category=category;
    this->note=note;
}

QString ImageData::getUrl() const
{
    return url;
}

QString ImageData::getCategory() const
{
    return category;
}

QString ImageData::getNote() const
{
    return note;
}

void ImageData::setUrl(const QString &value)
{
    url = value;
}

void ImageData::setCategory(const QString &value)
{
    category = value;
}

void ImageData::setNote(const QString &value)
{
    note = value;
}
