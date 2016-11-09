/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *sizeLabel;
    QLineEdit *sizeLineEdit;
    QLabel *ruleLabel;
    QLineEdit *ruleLineEdit;
    QPushButton *stopPushButton;
    QPushButton *pausePushButton;
    QPushButton *playPushButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(240, 140);
        QFont font;
        font.setPointSize(12);
        MainWindow->setFont(font);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        formLayoutWidget = new QWidget(centralWidget);
        formLayoutWidget->setObjectName(QStringLiteral("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(20, 20, 203, 62));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        sizeLabel = new QLabel(formLayoutWidget);
        sizeLabel->setObjectName(QStringLiteral("sizeLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, sizeLabel);

        sizeLineEdit = new QLineEdit(formLayoutWidget);
        sizeLineEdit->setObjectName(QStringLiteral("sizeLineEdit"));
        sizeLineEdit->setEnabled(true);

        formLayout->setWidget(0, QFormLayout::FieldRole, sizeLineEdit);

        ruleLabel = new QLabel(formLayoutWidget);
        ruleLabel->setObjectName(QStringLiteral("ruleLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, ruleLabel);

        ruleLineEdit = new QLineEdit(formLayoutWidget);
        ruleLineEdit->setObjectName(QStringLiteral("ruleLineEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, ruleLineEdit);

        stopPushButton = new QPushButton(centralWidget);
        stopPushButton->setObjectName(QStringLiteral("stopPushButton"));
        stopPushButton->setEnabled(false);
        stopPushButton->setGeometry(QRect(50, 90, 32, 32));
        stopPushButton->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        stopPushButton->setIcon(icon);
        stopPushButton->setIconSize(QSize(32, 32));
        stopPushButton->setFlat(true);
        pausePushButton = new QPushButton(centralWidget);
        pausePushButton->setObjectName(QStringLiteral("pausePushButton"));
        pausePushButton->setEnabled(false);
        pausePushButton->setGeometry(QRect(105, 90, 32, 32));
        pausePushButton->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        pausePushButton->setIcon(icon1);
        pausePushButton->setIconSize(QSize(32, 32));
        pausePushButton->setFlat(true);
        playPushButton = new QPushButton(centralWidget);
        playPushButton->setObjectName(QStringLiteral("playPushButton"));
        playPushButton->setGeometry(QRect(160, 90, 32, 32));
        playPushButton->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        playPushButton->setIcon(icon2);
        playPushButton->setIconSize(QSize(32, 32));
        playPushButton->setFlat(true);
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Attractors", 0));
        sizeLabel->setText(QApplication::translate("MainWindow", "Size", 0));
        sizeLineEdit->setText(QApplication::translate("MainWindow", "9", 0));
        ruleLabel->setText(QApplication::translate("MainWindow", "Rule", 0));
        ruleLineEdit->setText(QApplication::translate("MainWindow", "193", 0));
        stopPushButton->setText(QString());
        pausePushButton->setText(QString());
        playPushButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
