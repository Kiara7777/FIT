#include "skupinydialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

SkupinyDialog::SkupinyDialog(QWidget *parent) : QDialog(parent)
{
    labelType = new QLabel(tr("Typ rozdělování:"));
    labelSize = new QLabel(tr("Časový interval skupiny:"));

    comboType = new QComboBox();
    comboType->addItem(tr("Dle datumu a času"));
    comboType->addItem(tr("Dle GPS douřadnic"));
    connect(comboType, SIGNAL(currentIndexChanged(int)), this, SLOT(changeType(int)));

    QRegExp exp("^\\d+$");
    editSize = new QLineEdit;
    editSize->setValidator(new QRegExpValidator(exp, editSize));
    editSize->setText("30");
    labelSize->setBuddy(editSize);

    labelItem = new QLabel(tr("min"));

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QHBoxLayout *typeLayout = new QHBoxLayout;
    typeLayout->addWidget(labelType);
    typeLayout->addWidget(comboType);

    QHBoxLayout *sizeLayout = new QHBoxLayout;
    sizeLayout->addWidget(labelSize);
    sizeLayout->addWidget(editSize);
    sizeLayout->addWidget(labelItem);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(typeLayout);
    mainLayout->addLayout(sizeLayout);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);



}

int SkupinyDialog::getType()
{
    return comboType->currentIndex(); //0 - datum, 1 GPS

}

QString SkupinyDialog::getTypeSize()
{
    return editSize->text(); //text toho co je tam zadane, bude se muset prevest na cislo

}

void SkupinyDialog::changeType(int i)
{
    if (i == 1)
    {
        labelSize->setText(tr("Okruh &vzdálenosti:"));
        labelItem->setText("km");
        editSize->setText("1");
    }
    else
    {
        labelSize->setText(tr("Časový &interval skupiny:"));
        labelItem->setText("min");
        editSize->setText("30");
    }



}
