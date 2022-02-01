/********************************************************************************
** Form generated from reading UI file 'bb208.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BB208_H
#define UI_BB208_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_bB208
{
public:
    QWidget *centralwidget;
    QPushButton *open_pushButton;
    QLineEdit *lineEdit_wavfile;
    QPushButton *send_pushButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *bB208)
    {
        if (bB208->objectName().isEmpty())
            bB208->setObjectName(QString::fromUtf8("bB208"));
        bB208->resize(800, 600);
        centralwidget = new QWidget(bB208);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        open_pushButton = new QPushButton(centralwidget);
        open_pushButton->setObjectName(QString::fromUtf8("open_pushButton"));
        open_pushButton->setGeometry(QRect(40, 40, 141, 27));
        lineEdit_wavfile = new QLineEdit(centralwidget);
        lineEdit_wavfile->setObjectName(QString::fromUtf8("lineEdit_wavfile"));
        lineEdit_wavfile->setGeometry(QRect(192, 40, 451, 27));
        send_pushButton = new QPushButton(centralwidget);
        send_pushButton->setObjectName(QString::fromUtf8("send_pushButton"));
        send_pushButton->setGeometry(QRect(40, 70, 141, 27));
        bB208->setCentralWidget(centralwidget);
        menubar = new QMenuBar(bB208);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 24));
        bB208->setMenuBar(menubar);
        statusbar = new QStatusBar(bB208);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        bB208->setStatusBar(statusbar);

        retranslateUi(bB208);

        QMetaObject::connectSlotsByName(bB208);
    } // setupUi

    void retranslateUi(QMainWindow *bB208)
    {
        bB208->setWindowTitle(QCoreApplication::translate("bB208", "bB208", nullptr));
        open_pushButton->setText(QCoreApplication::translate("bB208", "Open seq file", nullptr));
        send_pushButton->setText(QCoreApplication::translate("bB208", "Send seq file", nullptr));
    } // retranslateUi

};

namespace Ui {
    class bB208: public Ui_bB208 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BB208_H
