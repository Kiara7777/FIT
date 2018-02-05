#ifndef SKUPINYDIALOG_H
#define SKUPINYDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QRegExp>
#include <QValidator>
#include <QDialogButtonBox>

class SkupinyDialog : public QDialog
{
    Q_OBJECT

public:
    SkupinyDialog(QWidget *parent = 0);

    int getType();
    QString getTypeSize();

private slots:
    void changeType(int i);
//    void findGroups();

private:
    QLabel *labelType;
    QLabel *labelSize;
    QComboBox *comboType;
    QLineEdit *editSize;
    QLabel *labelItem;
    QDialogButtonBox *buttonBox;

};

#endif // SKUPINYDIALOG_H
