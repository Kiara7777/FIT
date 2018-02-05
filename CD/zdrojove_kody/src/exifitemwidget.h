#ifndef EXIFITEMWIDGET_H
#define EXIFITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QString>

class ExifItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExifItemWidget(QString exifName, QString exifNameData = "", QWidget *parent = 0);

    QLineEdit *getExifDataWidget();
    void clearWidget();

private slots:

private:
    QLabel *exifLavel;
    QLineEdit *exifData;


};

#endif // EXIFITEMWIDGET_H
