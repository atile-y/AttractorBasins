/********************************************************************************
** Form generated from reading UI file 'formwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMWINDOW_H
#define UI_FORMWINDOW_H

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

class Ui_FormWindow
{
public:
    QWidget *centralWidget;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *sizeLabel;
    QLineEdit *sizeLineEdit;
    QLabel *ruleLabel;
    QLineEdit *ruleLineEdit;
    QPushButton *runPushButton;

    void setupUi(QMainWindow *FormWindow)
    {
        if (FormWindow->objectName().isEmpty())
            FormWindow->setObjectName(QStringLiteral("FormWindow"));
        FormWindow->resize(240, 140);
        QFont font;
        font.setPointSize(12);
        FormWindow->setFont(font);
        centralWidget = new QWidget(FormWindow);
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

        formLayout->setWidget(0, QFormLayout::FieldRole, sizeLineEdit);

        ruleLabel = new QLabel(formLayoutWidget);
        ruleLabel->setObjectName(QStringLiteral("ruleLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, ruleLabel);

        ruleLineEdit = new QLineEdit(formLayoutWidget);
        ruleLineEdit->setObjectName(QStringLiteral("ruleLineEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, ruleLineEdit);

        runPushButton = new QPushButton(centralWidget);
        runPushButton->setObjectName(QStringLiteral("runPushButton"));
        runPushButton->setGeometry(QRect(80, 100, 80, 23));
        FormWindow->setCentralWidget(centralWidget);

        retranslateUi(FormWindow);

        QMetaObject::connectSlotsByName(FormWindow);
    } // setupUi

    void retranslateUi(QMainWindow *FormWindow)
    {
        FormWindow->setWindowTitle(QApplication::translate("FormWindow", "Attractors", 0));
        sizeLabel->setText(QApplication::translate("FormWindow", "Size", 0));
        ruleLabel->setText(QApplication::translate("FormWindow", "Rule", 0));
        runPushButton->setText(QApplication::translate("FormWindow", "Run", 0));
    } // retranslateUi

};

namespace Ui {
    class FormWindow: public Ui_FormWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMWINDOW_H
