#ifndef EXIFDATEITEMWIDGET_H
#define EXIFDATEITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QDateTimeEdit>
#include <QHBoxLayout>
#include <QString>

class ExifDateItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExifDateItemWidget(QString exifName, QString exifNameData = "", QWidget *parent = 0);

    QDateTimeEdit *getExifDataWidget();
    void clearWidget();

private slots:

private:
    QLabel *exifLavel;
    QDateTimeEdit *exifData;
};

#endif // EXIFDATEITEMWIDGET_H
