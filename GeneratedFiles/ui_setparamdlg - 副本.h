/********************************************************************************
** Form generated from reading UI file 'setparamdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETPARAMDLG_H
#define UI_SETPARAMDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>
#include"MyButton.h"
QT_BEGIN_NAMESPACE

class Ui_SetParamDlg
{
public:
    QGroupBox *groupBox_4;
    QLabel *TitleLabel_Name;
    QGroupBox *groupBox_3;
    QScrollArea *ScrollArea_MinBtn;
    QWidget *ScrollAreaWidgetContents_MinBtn;
    MyButton *MinBtn;
    QScrollArea *ScrollArea_CloseBtn;
    QWidget *ScrollAreaWidgetContents_CloseBtn;
    MyButton *CloseBtn;
    QLabel *TiltleLabel;
    QGroupBox *groupBox;
    QGroupBox *groupBox_1;
    QPushButton *WTimeBtn;
    QPushButton *RTimeBtn;
    QDateTimeEdit *StationTimeEdit;
    QGroupBox *groupBox_2;
    QPushButton *DownBtn;
    QDateTimeEdit *BdateTimeEdit;
    QDateTimeEdit *EdateTimeEdit;

    void setupUi(QWidget *SetParamDlg)
    {
        if (SetParamDlg->objectName().isEmpty())
            SetParamDlg->setObjectName(QStringLiteral("SetParamDlg"));
        SetParamDlg->resize(342, 322);
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        SetParamDlg->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral("../Image/png/Weather.ico"), QSize(), QIcon::Normal, QIcon::Off);
        SetParamDlg->setWindowIcon(icon);
        SetParamDlg->setStyleSheet(QStringLiteral("background:rgb(77,77, 77);color:white"));
        groupBox_4 = new QGroupBox(SetParamDlg);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 10, 321, 301));
        groupBox_4->setFont(font);
        TitleLabel_Name = new QLabel(groupBox_4);
        TitleLabel_Name->setObjectName(QStringLiteral("TitleLabel_Name"));
        TitleLabel_Name->setGeometry(QRect(45, 20, 121, 16));
        TitleLabel_Name->setFont(font);
        groupBox_3 = new QGroupBox(groupBox_4);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(270, 15, 46, 26));
        groupBox_3->setFont(font);
        ScrollArea_MinBtn = new QScrollArea(groupBox_3);
        ScrollArea_MinBtn->setObjectName(QStringLiteral("ScrollArea_MinBtn"));
        ScrollArea_MinBtn->setGeometry(QRect(2, 3, 20, 20));
        ScrollArea_MinBtn->setFont(font);
        ScrollArea_MinBtn->setWidgetResizable(true);
        ScrollAreaWidgetContents_MinBtn = new QWidget();
        ScrollAreaWidgetContents_MinBtn->setObjectName(QStringLiteral("ScrollAreaWidgetContents_MinBtn"));
        ScrollAreaWidgetContents_MinBtn->setGeometry(QRect(0, 0, 18, 18));
      	MinBtn = new MyButton("../Image/png/Mini.png", 1, ScrollAreaWidgetContents_MinBtn);
        MinBtn->setObjectName(QStringLiteral("MinBtn"));
        MinBtn->setGeometry(QRect(1, 1, 18, 18));
        MinBtn->setFont(font);
        ScrollArea_MinBtn->setWidget(ScrollAreaWidgetContents_MinBtn);
        ScrollArea_CloseBtn = new QScrollArea(groupBox_3);
        ScrollArea_CloseBtn->setObjectName(QStringLiteral("ScrollArea_CloseBtn"));
        ScrollArea_CloseBtn->setGeometry(QRect(24, 3, 20, 20));
        ScrollArea_CloseBtn->setFont(font);
        ScrollArea_CloseBtn->setWidgetResizable(true);
        ScrollAreaWidgetContents_CloseBtn = new QWidget();
        ScrollAreaWidgetContents_CloseBtn->setObjectName(QStringLiteral("ScrollAreaWidgetContents_CloseBtn"));
        ScrollAreaWidgetContents_CloseBtn->setGeometry(QRect(0, 0, 18, 18));
    		CloseBtn = new MyButton("../Image/png/Close.png", 1, ScrollAreaWidgetContents_CloseBtn);
        CloseBtn->setObjectName(QStringLiteral("CloseBtn"));
        CloseBtn->setGeometry(QRect(1, 1, 18, 18));
        CloseBtn->setFont(font);
        ScrollArea_CloseBtn->setWidget(ScrollAreaWidgetContents_CloseBtn);
        TiltleLabel = new QLabel(groupBox_4);
        TiltleLabel->setObjectName(QStringLiteral("TiltleLabel"));
        TiltleLabel->setGeometry(QRect(10, 15, 32, 32));
        TiltleLabel->setFont(font);
        TiltleLabel->setPixmap(QPixmap(QString::fromUtf8("../Image/png/control.png")));
        groupBox = new QGroupBox(groupBox_4);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 50, 301, 241));
        groupBox->setFont(font);
        groupBox_1 = new QGroupBox(groupBox);
        groupBox_1->setObjectName(QStringLiteral("groupBox_1"));
        groupBox_1->setGeometry(QRect(10, 10, 231, 101));
        groupBox_1->setFont(font);
        WTimeBtn = new QPushButton(groupBox_1);
        WTimeBtn->setObjectName(QStringLiteral("WTimeBtn"));
        WTimeBtn->setGeometry(QRect(120, 60, 75, 23));
        WTimeBtn->setFont(font);
        RTimeBtn = new QPushButton(groupBox_1);
        RTimeBtn->setObjectName(QStringLiteral("RTimeBtn"));
        RTimeBtn->setGeometry(QRect(20, 60, 75, 23));
        RTimeBtn->setFont(font);
        StationTimeEdit = new QDateTimeEdit(groupBox_1);
        StationTimeEdit->setObjectName(QStringLiteral("StationTimeEdit"));
        StationTimeEdit->setGeometry(QRect(30, 20, 161, 22));
        StationTimeEdit->setFont(font);
        StationTimeEdit->setDateTime(QDateTime(QDate(2018, 10, 1), QTime(0, 0, 0)));
        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 130, 271, 91));
        groupBox_2->setFont(font);
        DownBtn = new QPushButton(groupBox_2);
        DownBtn->setObjectName(QStringLiteral("DownBtn"));
        DownBtn->setGeometry(QRect(190, 20, 75, 23));
        DownBtn->setFont(font);
        BdateTimeEdit = new QDateTimeEdit(groupBox_2);
        BdateTimeEdit->setObjectName(QStringLiteral("BdateTimeEdit"));
        BdateTimeEdit->setGeometry(QRect(10, 20, 171, 22));
        BdateTimeEdit->setFont(font);
        BdateTimeEdit->setDateTime(QDateTime(QDate(2018, 10, 1), QTime(0, 0, 0)));
        EdateTimeEdit = new QDateTimeEdit(groupBox_2);
        EdateTimeEdit->setObjectName(QStringLiteral("EdateTimeEdit"));
        EdateTimeEdit->setGeometry(QRect(10, 60, 171, 22));
        EdateTimeEdit->setFont(font);
        EdateTimeEdit->setDateTime(QDateTime(QDate(2018, 10, 1), QTime(0, 0, 0)));
        QWidget::setTabOrder(StationTimeEdit, RTimeBtn);
        QWidget::setTabOrder(RTimeBtn, WTimeBtn);
        QWidget::setTabOrder(WTimeBtn, BdateTimeEdit);
        QWidget::setTabOrder(BdateTimeEdit, EdateTimeEdit);
        QWidget::setTabOrder(EdateTimeEdit, DownBtn);

        retranslateUi(SetParamDlg);

        QMetaObject::connectSlotsByName(SetParamDlg);
    } // setupUi

    void retranslateUi(QWidget *SetParamDlg)
    {
        SetParamDlg->setWindowTitle(QApplication::translate("SetParamDlg", "\345\256\207\345\256\231\345\260\204\347\272\277\345\214\272\345\237\237\346\260\264\345\210\206\344\273\252\350\256\276\345\244\207\346\216\247\345\210\266", nullptr));
        groupBox_4->setTitle(QString());
        TitleLabel_Name->setText(QApplication::translate("SetParamDlg", "\345\256\207\345\256\231\345\260\204\347\272\277\345\214\272\345\237\237\346\260\264\345\210\206\344\273\252", nullptr));
        groupBox_3->setTitle(QString());
        MinBtn->setText(QApplication::translate("SetParamDlg", "PushButton", nullptr));
        CloseBtn->setText(QApplication::translate("SetParamDlg", "PushButton", nullptr));
        TiltleLabel->setText(QString());
        groupBox->setTitle(QString());
        groupBox_1->setTitle(QApplication::translate("SetParamDlg", "\350\256\276\345\244\207\346\227\266\351\227\264", nullptr));
        WTimeBtn->setText(QApplication::translate("SetParamDlg", "\350\256\276\347\275\256\346\227\266\351\222\237", nullptr));
        RTimeBtn->setText(QApplication::translate("SetParamDlg", "\350\257\273\345\217\226\346\227\266\351\222\237", nullptr));
        groupBox_2->setTitle(QApplication::translate("SetParamDlg", "\346\225\260\346\215\256\350\241\245\346\212\204", nullptr));
        DownBtn->setText(QApplication::translate("SetParamDlg", "\350\241\245\346\212\204\346\225\260\346\215\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SetParamDlg: public Ui_SetParamDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETPARAMDLG_H
