#ifndef FOTOWIDGET_H
#define FOTOWIDGET_H

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>

class FotoWidget : public QFrame
{
    Q_OBJECT
public:
    explicit FotoWidget(QPixmap pixmap, QString name, int index, int group = 0, bool isGroup = false, QWidget *parent = 0);

    const QPixmap *getPixmap();
    int getIndex();
    int getGroup();
    bool isFromGroup();
    bool itsGroupIndicator();

    void setItsGroup(bool itsGroup);
    void restoreBorder();

signals:
    void clicked(FotoWidget *fotoWidget);
    void doubleClicked(int gr);

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);

public slots:

private slots:

private:
    QLabel *pixmapLabel;
    QLabel *nameLabel;
    int index;
    int group;
    bool isInGroup;
    bool itIsGroup;
};

#endif // FOTOWIDGET_H
