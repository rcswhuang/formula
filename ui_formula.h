/********************************************************************************
** Form generated from reading UI file 'formula.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMULA_H
#define UI_FORMULA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Formula
{
public:
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QTextEdit *IDC_EDIT;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QPushButton *okBtn;
    QSpacerItem *verticalSpacer_2;
    QPushButton *cancleBtn;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gridLayout;
    QPushButton *IDC_7;
    QPushButton *IDC_8;
    QPushButton *IDC_9;
    QPushButton *IDC_PLUS;
    QPushButton *IDC_GREATE;
    QPushButton *IDC_4;
    QPushButton *IDC_5;
    QPushButton *IDC_6;
    QPushButton *IDC_MINUS;
    QPushButton *IDC_LOWER;
    QPushButton *IDC_1;
    QPushButton *IDC_2;
    QPushButton *IDC_3;
    QPushButton *IDC_DIDVDE;
    QPushButton *IDC_EQUAL;
    QPushButton *IDC_0;
    QPushButton *IDC_10;
    QPushButton *IDC_DOT;
    QPushButton *IDC_MUTIPLY;
    QPushButton *IDC_NEQUAL;
    QPushButton *IDC_LPARENTHESIS;
    QPushButton *IDC_RPARENTHESIS;
    QPushButton *IDC_DAT;
    QPushButton *IDC_POWER;
    QPushButton *IDC_GEQUAL;
    QPushButton *IDC_AND;
    QPushButton *IDC_OR;
    QPushButton *IDC_XOR;
    QPushButton *IDC_NOT;
    QPushButton *IDC_LEQUAL;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QComboBox *IDC_STATION;
    QLabel *label_2;
    QComboBox *IDC_COMBOMODE;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QComboBox *IDC_TYPE;
    QLabel *label_4;
    QComboBox *IDC_ATTRIB;
    QListWidget *IDC_LIST;

    void setupUi(QDialog *Formula)
    {
        if (Formula->objectName().isEmpty())
            Formula->setObjectName(QStringLiteral("Formula"));
        Formula->resize(705, 444);
        Formula->setMinimumSize(QSize(0, 0));
        Formula->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout_3 = new QVBoxLayout(Formula);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        IDC_EDIT = new QTextEdit(Formula);
        IDC_EDIT->setObjectName(QStringLiteral("IDC_EDIT"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(IDC_EDIT->sizePolicy().hasHeightForWidth());
        IDC_EDIT->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(10);
        IDC_EDIT->setFont(font);
        IDC_EDIT->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(IDC_EDIT);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        okBtn = new QPushButton(Formula);
        okBtn->setObjectName(QStringLiteral("okBtn"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font1.setPointSize(13);
        okBtn->setFont(font1);

        verticalLayout->addWidget(okBtn);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        cancleBtn = new QPushButton(Formula);
        cancleBtn->setObjectName(QStringLiteral("cancleBtn"));
        cancleBtn->setFont(font1);

        verticalLayout->addWidget(cancleBtn);

        verticalSpacer = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 1);
        verticalLayout->setStretch(2, 2);
        verticalLayout->setStretch(3, 5);

        horizontalLayout->addLayout(verticalLayout);

        horizontalLayout->setStretch(0, 8);
        horizontalLayout->setStretch(2, 1);

        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        IDC_7 = new QPushButton(Formula);
        IDC_7->setObjectName(QStringLiteral("IDC_7"));
        sizePolicy.setHeightForWidth(IDC_7->sizePolicy().hasHeightForWidth());
        IDC_7->setSizePolicy(sizePolicy);
        QFont font2;
        font2.setFamily(QStringLiteral("Helvetica"));
        font2.setPointSize(16);
        IDC_7->setFont(font2);
        IDC_7->setAutoDefault(false);

        gridLayout->addWidget(IDC_7, 0, 0, 1, 1);

        IDC_8 = new QPushButton(Formula);
        IDC_8->setObjectName(QStringLiteral("IDC_8"));
        sizePolicy.setHeightForWidth(IDC_8->sizePolicy().hasHeightForWidth());
        IDC_8->setSizePolicy(sizePolicy);
        IDC_8->setFont(font2);
        IDC_8->setAutoDefault(false);

        gridLayout->addWidget(IDC_8, 0, 1, 1, 1);

        IDC_9 = new QPushButton(Formula);
        IDC_9->setObjectName(QStringLiteral("IDC_9"));
        sizePolicy.setHeightForWidth(IDC_9->sizePolicy().hasHeightForWidth());
        IDC_9->setSizePolicy(sizePolicy);
        IDC_9->setFont(font2);
        IDC_9->setAutoDefault(false);

        gridLayout->addWidget(IDC_9, 0, 2, 1, 1);

        IDC_PLUS = new QPushButton(Formula);
        IDC_PLUS->setObjectName(QStringLiteral("IDC_PLUS"));
        sizePolicy.setHeightForWidth(IDC_PLUS->sizePolicy().hasHeightForWidth());
        IDC_PLUS->setSizePolicy(sizePolicy);
        IDC_PLUS->setFont(font2);
        IDC_PLUS->setAutoDefault(false);

        gridLayout->addWidget(IDC_PLUS, 0, 3, 1, 1);

        IDC_GREATE = new QPushButton(Formula);
        IDC_GREATE->setObjectName(QStringLiteral("IDC_GREATE"));
        sizePolicy.setHeightForWidth(IDC_GREATE->sizePolicy().hasHeightForWidth());
        IDC_GREATE->setSizePolicy(sizePolicy);
        IDC_GREATE->setFont(font2);
        IDC_GREATE->setAutoDefault(false);

        gridLayout->addWidget(IDC_GREATE, 0, 4, 1, 1);

        IDC_4 = new QPushButton(Formula);
        IDC_4->setObjectName(QStringLiteral("IDC_4"));
        sizePolicy.setHeightForWidth(IDC_4->sizePolicy().hasHeightForWidth());
        IDC_4->setSizePolicy(sizePolicy);
        IDC_4->setFont(font2);
        IDC_4->setAutoDefault(false);

        gridLayout->addWidget(IDC_4, 1, 0, 1, 1);

        IDC_5 = new QPushButton(Formula);
        IDC_5->setObjectName(QStringLiteral("IDC_5"));
        sizePolicy.setHeightForWidth(IDC_5->sizePolicy().hasHeightForWidth());
        IDC_5->setSizePolicy(sizePolicy);
        IDC_5->setFont(font2);
        IDC_5->setAutoDefault(false);

        gridLayout->addWidget(IDC_5, 1, 1, 1, 1);

        IDC_6 = new QPushButton(Formula);
        IDC_6->setObjectName(QStringLiteral("IDC_6"));
        sizePolicy.setHeightForWidth(IDC_6->sizePolicy().hasHeightForWidth());
        IDC_6->setSizePolicy(sizePolicy);
        IDC_6->setFont(font2);
        IDC_6->setAutoDefault(false);

        gridLayout->addWidget(IDC_6, 1, 2, 1, 1);

        IDC_MINUS = new QPushButton(Formula);
        IDC_MINUS->setObjectName(QStringLiteral("IDC_MINUS"));
        sizePolicy.setHeightForWidth(IDC_MINUS->sizePolicy().hasHeightForWidth());
        IDC_MINUS->setSizePolicy(sizePolicy);
        IDC_MINUS->setFont(font2);
        IDC_MINUS->setAutoDefault(false);

        gridLayout->addWidget(IDC_MINUS, 1, 3, 1, 1);

        IDC_LOWER = new QPushButton(Formula);
        IDC_LOWER->setObjectName(QStringLiteral("IDC_LOWER"));
        sizePolicy.setHeightForWidth(IDC_LOWER->sizePolicy().hasHeightForWidth());
        IDC_LOWER->setSizePolicy(sizePolicy);
        IDC_LOWER->setFont(font2);
        IDC_LOWER->setAutoDefault(false);

        gridLayout->addWidget(IDC_LOWER, 1, 4, 1, 1);

        IDC_1 = new QPushButton(Formula);
        IDC_1->setObjectName(QStringLiteral("IDC_1"));
        sizePolicy.setHeightForWidth(IDC_1->sizePolicy().hasHeightForWidth());
        IDC_1->setSizePolicy(sizePolicy);
        IDC_1->setFont(font2);
        IDC_1->setAutoDefault(false);

        gridLayout->addWidget(IDC_1, 2, 0, 1, 1);

        IDC_2 = new QPushButton(Formula);
        IDC_2->setObjectName(QStringLiteral("IDC_2"));
        sizePolicy.setHeightForWidth(IDC_2->sizePolicy().hasHeightForWidth());
        IDC_2->setSizePolicy(sizePolicy);
        IDC_2->setFont(font2);
        IDC_2->setAutoDefault(false);

        gridLayout->addWidget(IDC_2, 2, 1, 1, 1);

        IDC_3 = new QPushButton(Formula);
        IDC_3->setObjectName(QStringLiteral("IDC_3"));
        sizePolicy.setHeightForWidth(IDC_3->sizePolicy().hasHeightForWidth());
        IDC_3->setSizePolicy(sizePolicy);
        IDC_3->setFont(font2);
        IDC_3->setAutoDefault(false);

        gridLayout->addWidget(IDC_3, 2, 2, 1, 1);

        IDC_DIDVDE = new QPushButton(Formula);
        IDC_DIDVDE->setObjectName(QStringLiteral("IDC_DIDVDE"));
        sizePolicy.setHeightForWidth(IDC_DIDVDE->sizePolicy().hasHeightForWidth());
        IDC_DIDVDE->setSizePolicy(sizePolicy);
        IDC_DIDVDE->setFont(font2);
        IDC_DIDVDE->setAutoDefault(false);

        gridLayout->addWidget(IDC_DIDVDE, 2, 3, 1, 1);

        IDC_EQUAL = new QPushButton(Formula);
        IDC_EQUAL->setObjectName(QStringLiteral("IDC_EQUAL"));
        sizePolicy.setHeightForWidth(IDC_EQUAL->sizePolicy().hasHeightForWidth());
        IDC_EQUAL->setSizePolicy(sizePolicy);
        IDC_EQUAL->setFont(font2);
        IDC_EQUAL->setIconSize(QSize(16, 16));
        IDC_EQUAL->setAutoDefault(false);

        gridLayout->addWidget(IDC_EQUAL, 2, 4, 1, 1);

        IDC_0 = new QPushButton(Formula);
        IDC_0->setObjectName(QStringLiteral("IDC_0"));
        sizePolicy.setHeightForWidth(IDC_0->sizePolicy().hasHeightForWidth());
        IDC_0->setSizePolicy(sizePolicy);
        IDC_0->setFont(font2);
        IDC_0->setAutoDefault(false);

        gridLayout->addWidget(IDC_0, 3, 0, 1, 1);

        IDC_10 = new QPushButton(Formula);
        IDC_10->setObjectName(QStringLiteral("IDC_10"));
        sizePolicy.setHeightForWidth(IDC_10->sizePolicy().hasHeightForWidth());
        IDC_10->setSizePolicy(sizePolicy);
        IDC_10->setFont(font2);
        IDC_10->setAutoDefault(false);

        gridLayout->addWidget(IDC_10, 3, 1, 1, 1);

        IDC_DOT = new QPushButton(Formula);
        IDC_DOT->setObjectName(QStringLiteral("IDC_DOT"));
        sizePolicy.setHeightForWidth(IDC_DOT->sizePolicy().hasHeightForWidth());
        IDC_DOT->setSizePolicy(sizePolicy);
        IDC_DOT->setFont(font2);
        IDC_DOT->setAutoDefault(false);

        gridLayout->addWidget(IDC_DOT, 3, 2, 1, 1);

        IDC_MUTIPLY = new QPushButton(Formula);
        IDC_MUTIPLY->setObjectName(QStringLiteral("IDC_MUTIPLY"));
        sizePolicy.setHeightForWidth(IDC_MUTIPLY->sizePolicy().hasHeightForWidth());
        IDC_MUTIPLY->setSizePolicy(sizePolicy);
        IDC_MUTIPLY->setFont(font2);
        IDC_MUTIPLY->setAutoDefault(false);

        gridLayout->addWidget(IDC_MUTIPLY, 3, 3, 1, 1);

        IDC_NEQUAL = new QPushButton(Formula);
        IDC_NEQUAL->setObjectName(QStringLiteral("IDC_NEQUAL"));
        sizePolicy.setHeightForWidth(IDC_NEQUAL->sizePolicy().hasHeightForWidth());
        IDC_NEQUAL->setSizePolicy(sizePolicy);
        IDC_NEQUAL->setFont(font2);
        IDC_NEQUAL->setAutoDefault(false);

        gridLayout->addWidget(IDC_NEQUAL, 3, 4, 1, 1);

        IDC_LPARENTHESIS = new QPushButton(Formula);
        IDC_LPARENTHESIS->setObjectName(QStringLiteral("IDC_LPARENTHESIS"));
        sizePolicy.setHeightForWidth(IDC_LPARENTHESIS->sizePolicy().hasHeightForWidth());
        IDC_LPARENTHESIS->setSizePolicy(sizePolicy);
        IDC_LPARENTHESIS->setFont(font2);
        IDC_LPARENTHESIS->setAutoDefault(false);

        gridLayout->addWidget(IDC_LPARENTHESIS, 4, 0, 1, 1);

        IDC_RPARENTHESIS = new QPushButton(Formula);
        IDC_RPARENTHESIS->setObjectName(QStringLiteral("IDC_RPARENTHESIS"));
        sizePolicy.setHeightForWidth(IDC_RPARENTHESIS->sizePolicy().hasHeightForWidth());
        IDC_RPARENTHESIS->setSizePolicy(sizePolicy);
        IDC_RPARENTHESIS->setFont(font2);
        IDC_RPARENTHESIS->setAutoDefault(false);

        gridLayout->addWidget(IDC_RPARENTHESIS, 4, 1, 1, 1);

        IDC_DAT = new QPushButton(Formula);
        IDC_DAT->setObjectName(QStringLiteral("IDC_DAT"));
        sizePolicy.setHeightForWidth(IDC_DAT->sizePolicy().hasHeightForWidth());
        IDC_DAT->setSizePolicy(sizePolicy);
        IDC_DAT->setFont(font2);
        IDC_DAT->setAutoDefault(false);

        gridLayout->addWidget(IDC_DAT, 4, 2, 1, 1);

        IDC_POWER = new QPushButton(Formula);
        IDC_POWER->setObjectName(QStringLiteral("IDC_POWER"));
        sizePolicy.setHeightForWidth(IDC_POWER->sizePolicy().hasHeightForWidth());
        IDC_POWER->setSizePolicy(sizePolicy);
        IDC_POWER->setFont(font2);
        IDC_POWER->setAutoDefault(false);

        gridLayout->addWidget(IDC_POWER, 4, 3, 1, 1);

        IDC_GEQUAL = new QPushButton(Formula);
        IDC_GEQUAL->setObjectName(QStringLiteral("IDC_GEQUAL"));
        sizePolicy.setHeightForWidth(IDC_GEQUAL->sizePolicy().hasHeightForWidth());
        IDC_GEQUAL->setSizePolicy(sizePolicy);
        IDC_GEQUAL->setFont(font2);
        IDC_GEQUAL->setAutoDefault(false);

        gridLayout->addWidget(IDC_GEQUAL, 4, 4, 1, 1);

        IDC_AND = new QPushButton(Formula);
        IDC_AND->setObjectName(QStringLiteral("IDC_AND"));
        sizePolicy.setHeightForWidth(IDC_AND->sizePolicy().hasHeightForWidth());
        IDC_AND->setSizePolicy(sizePolicy);
        IDC_AND->setFont(font2);
        IDC_AND->setAutoDefault(false);

        gridLayout->addWidget(IDC_AND, 5, 0, 1, 1);

        IDC_OR = new QPushButton(Formula);
        IDC_OR->setObjectName(QStringLiteral("IDC_OR"));
        sizePolicy.setHeightForWidth(IDC_OR->sizePolicy().hasHeightForWidth());
        IDC_OR->setSizePolicy(sizePolicy);
        IDC_OR->setFont(font2);
        IDC_OR->setAutoDefault(false);

        gridLayout->addWidget(IDC_OR, 5, 1, 1, 1);

        IDC_XOR = new QPushButton(Formula);
        IDC_XOR->setObjectName(QStringLiteral("IDC_XOR"));
        sizePolicy.setHeightForWidth(IDC_XOR->sizePolicy().hasHeightForWidth());
        IDC_XOR->setSizePolicy(sizePolicy);
        IDC_XOR->setFont(font2);
        IDC_XOR->setAutoDefault(false);

        gridLayout->addWidget(IDC_XOR, 5, 2, 1, 1);

        IDC_NOT = new QPushButton(Formula);
        IDC_NOT->setObjectName(QStringLiteral("IDC_NOT"));
        sizePolicy.setHeightForWidth(IDC_NOT->sizePolicy().hasHeightForWidth());
        IDC_NOT->setSizePolicy(sizePolicy);
        IDC_NOT->setFont(font2);
        IDC_NOT->setAutoDefault(false);

        gridLayout->addWidget(IDC_NOT, 5, 3, 1, 1);

        IDC_LEQUAL = new QPushButton(Formula);
        IDC_LEQUAL->setObjectName(QStringLiteral("IDC_LEQUAL"));
        sizePolicy.setHeightForWidth(IDC_LEQUAL->sizePolicy().hasHeightForWidth());
        IDC_LEQUAL->setSizePolicy(sizePolicy);
        IDC_LEQUAL->setFont(font2);
        IDC_LEQUAL->setAutoDefault(false);

        gridLayout->addWidget(IDC_LEQUAL, 5, 4, 1, 1);


        horizontalLayout_2->addLayout(gridLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(1);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label = new QLabel(Formula);
        label->setObjectName(QStringLiteral("label"));
        QFont font3;
        font3.setFamily(QStringLiteral("Songti SC"));
        font3.setPointSize(13);
        label->setFont(font3);

        horizontalLayout_3->addWidget(label);

        IDC_STATION = new QComboBox(Formula);
        IDC_STATION->setObjectName(QStringLiteral("IDC_STATION"));
        sizePolicy.setHeightForWidth(IDC_STATION->sizePolicy().hasHeightForWidth());
        IDC_STATION->setSizePolicy(sizePolicy);
        IDC_STATION->setMinimumSize(QSize(0, 25));
        IDC_STATION->setFont(font1);

        horizontalLayout_3->addWidget(IDC_STATION);

        label_2 = new QLabel(Formula);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font3);

        horizontalLayout_3->addWidget(label_2);

        IDC_COMBOMODE = new QComboBox(Formula);
        IDC_COMBOMODE->setObjectName(QStringLiteral("IDC_COMBOMODE"));
        sizePolicy.setHeightForWidth(IDC_COMBOMODE->sizePolicy().hasHeightForWidth());
        IDC_COMBOMODE->setSizePolicy(sizePolicy);
        IDC_COMBOMODE->setMinimumSize(QSize(0, 25));
        IDC_COMBOMODE->setFont(font1);

        horizontalLayout_3->addWidget(IDC_COMBOMODE);

        horizontalLayout_3->setStretch(0, 1);
        horizontalLayout_3->setStretch(1, 6);
        horizontalLayout_3->setStretch(2, 1);
        horizontalLayout_3->setStretch(3, 6);

        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_3 = new QLabel(Formula);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font3);

        horizontalLayout_4->addWidget(label_3);

        IDC_TYPE = new QComboBox(Formula);
        IDC_TYPE->setObjectName(QStringLiteral("IDC_TYPE"));
        sizePolicy.setHeightForWidth(IDC_TYPE->sizePolicy().hasHeightForWidth());
        IDC_TYPE->setSizePolicy(sizePolicy);
        IDC_TYPE->setMinimumSize(QSize(0, 25));
        IDC_TYPE->setFont(font1);

        horizontalLayout_4->addWidget(IDC_TYPE);

        label_4 = new QLabel(Formula);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setFont(font3);

        horizontalLayout_4->addWidget(label_4);

        IDC_ATTRIB = new QComboBox(Formula);
        IDC_ATTRIB->setObjectName(QStringLiteral("IDC_ATTRIB"));
        sizePolicy.setHeightForWidth(IDC_ATTRIB->sizePolicy().hasHeightForWidth());
        IDC_ATTRIB->setSizePolicy(sizePolicy);
        IDC_ATTRIB->setMinimumSize(QSize(0, 25));
        IDC_ATTRIB->setFont(font1);

        horizontalLayout_4->addWidget(IDC_ATTRIB);

        horizontalLayout_4->setStretch(0, 1);
        horizontalLayout_4->setStretch(1, 6);
        horizontalLayout_4->setStretch(2, 1);
        horizontalLayout_4->setStretch(3, 6);

        verticalLayout_2->addLayout(horizontalLayout_4);

        IDC_LIST = new QListWidget(Formula);
        IDC_LIST->setObjectName(QStringLiteral("IDC_LIST"));
        sizePolicy.setHeightForWidth(IDC_LIST->sizePolicy().hasHeightForWidth());
        IDC_LIST->setSizePolicy(sizePolicy);
        QFont font4;
        font4.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font4.setPointSize(10);
        IDC_LIST->setFont(font4);

        verticalLayout_2->addWidget(IDC_LIST);


        horizontalLayout_2->addLayout(verticalLayout_2);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 1);

        verticalLayout_3->addLayout(horizontalLayout_2);


        retranslateUi(Formula);

        QMetaObject::connectSlotsByName(Formula);
    } // setupUi

    void retranslateUi(QDialog *Formula)
    {
        Formula->setWindowTitle(QApplication::translate("Formula", "Dialog", 0));
        okBtn->setText(QApplication::translate("Formula", "\347\241\256\345\256\232", 0));
        cancleBtn->setText(QApplication::translate("Formula", "\345\217\226\346\266\210", 0));
        IDC_7->setText(QApplication::translate("Formula", "7", 0));
        IDC_8->setText(QApplication::translate("Formula", "8", 0));
        IDC_9->setText(QApplication::translate("Formula", "9", 0));
        IDC_PLUS->setText(QApplication::translate("Formula", "+", 0));
        IDC_GREATE->setText(QApplication::translate("Formula", ">", 0));
        IDC_4->setText(QApplication::translate("Formula", "4", 0));
        IDC_5->setText(QApplication::translate("Formula", "5", 0));
        IDC_6->setText(QApplication::translate("Formula", "6", 0));
        IDC_MINUS->setText(QApplication::translate("Formula", "-", 0));
        IDC_LOWER->setText(QApplication::translate("Formula", "<", 0));
        IDC_1->setText(QApplication::translate("Formula", "1", 0));
        IDC_2->setText(QApplication::translate("Formula", "2", 0));
        IDC_3->setText(QApplication::translate("Formula", "3", 0));
        IDC_DIDVDE->setText(QApplication::translate("Formula", "/", 0));
        IDC_EQUAL->setText(QApplication::translate("Formula", "=", 0));
        IDC_0->setText(QApplication::translate("Formula", "0", 0));
        IDC_10->setText(QApplication::translate("Formula", "$", 0));
        IDC_DOT->setText(QApplication::translate("Formula", ".", 0));
        IDC_MUTIPLY->setText(QApplication::translate("Formula", "*", 0));
        IDC_NEQUAL->setText(QApplication::translate("Formula", "!=", 0));
        IDC_LPARENTHESIS->setText(QApplication::translate("Formula", "(", 0));
        IDC_RPARENTHESIS->setText(QApplication::translate("Formula", ")", 0));
        IDC_DAT->setText(QApplication::translate("Formula", "Dat", 0));
        IDC_POWER->setText(QApplication::translate("Formula", "x^y", 0));
        IDC_GEQUAL->setText(QApplication::translate("Formula", ">=", 0));
        IDC_AND->setText(QApplication::translate("Formula", "And", 0));
        IDC_OR->setText(QApplication::translate("Formula", "Or", 0));
        IDC_XOR->setText(QApplication::translate("Formula", "Xor", 0));
        IDC_NOT->setText(QApplication::translate("Formula", "Not", 0));
        IDC_LEQUAL->setText(QApplication::translate("Formula", "<=", 0));
        label->setText(QApplication::translate("Formula", "\347\253\231\345\220\215", 0));
        label_2->setText(QApplication::translate("Formula", "\351\227\264\351\232\224", 0));
        label_3->setText(QApplication::translate("Formula", "\347\261\273\345\236\213", 0));
        label_4->setText(QApplication::translate("Formula", "\345\261\236\346\200\247", 0));
    } // retranslateUi

};

namespace Ui {
    class Formula: public Ui_Formula {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMULA_H
