#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIntValidator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QIntValidator *v = new QIntValidator(ui->sizeLineEdit);
    v->setBottom(2);
    ui->sizeLineEdit->setValidator(v);

    v = new QIntValidator(0, 255, ui->ruleLineEdit);
    ui->ruleLineEdit->setValidator(v);

    gl = new GLWidget(this);
    gl->setSize(9);
    gl->setRule(193);

    setFixedSize(geometry().width(), geometry().height());
    move(100, 100);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_sizeLineEdit_editingFinished(){
    int pos = 0;
    QString str = ui->sizeLineEdit->text();
    QIntValidator *v = (QIntValidator*)ui->sizeLineEdit->validator();

    if( v->validate(str, pos) != QValidator::Acceptable ){
        ui->sizeLineEdit->setText(QString::number(gl->getSize()));
        return;
    }

    gl->setSize(str.toUInt());
}

void MainWindow::on_ruleLineEdit_editingFinished(){
    int pos = 0;
    QString str = ui->ruleLineEdit->text();
    QIntValidator *v = (QIntValidator*)ui->ruleLineEdit->validator();

    if( v->validate(str, pos) != QValidator::Acceptable ){
        ui->ruleLineEdit->setText(QString::number(gl->getRule()));
        return;
    }

    gl->setRule(str.toUInt());
}

void MainWindow::on_stopPushButton_clicked(){
    gl->reset();
    gl->hide();

    ui->stopPushButton->setEnabled(false);
    ui->pausePushButton->setEnabled(false);
    ui->playPushButton->setEnabled(true);
    ui->sizeLineEdit->setEnabled(true);
    ui->ruleLineEdit->setEnabled(true);
}

void MainWindow::on_pausePushButton_clicked(){
    gl->pause();

    ui->stopPushButton->setEnabled(true);
    ui->pausePushButton->setEnabled(false);
    ui->playPushButton->setEnabled(true);
}

void MainWindow::on_playPushButton_clicked(){
    gl->play();
    gl->show();

    ui->stopPushButton->setEnabled(true);
    ui->pausePushButton->setEnabled(true);
    ui->playPushButton->setEnabled(false);
    ui->sizeLineEdit->setEnabled(false);
    ui->ruleLineEdit->setEnabled(false);
}
