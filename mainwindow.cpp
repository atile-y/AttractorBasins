#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QIntValidator *v = new QIntValidator(ui->sizeLineEdit);
    v->setBottom(1);
    ui->sizeLineEdit->setValidator(v);

    v = new QIntValidator(0, 255, ui->ruleLineEdit);
    ui->ruleLineEdit->setValidator(v);

    m_nSize = 20;
    m_nRule = 60;
    G = new Graph(this);

    setFixedSize(geometry().width(), geometry().height());
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_sizeLineEdit_editingFinished(){
    int pos = 0;
    QString str = ui->sizeLineEdit->text();
    QIntValidator *v = (QIntValidator*)ui->sizeLineEdit->validator();

    if( v->validate(str, pos) != QValidator::Acceptable ){
        ui->sizeLineEdit->setText(QString::number(m_nSize));
        return;
    }

    m_nSize = str.toUInt();
}

void MainWindow::on_ruleLineEdit_editingFinished(){
    int pos = 0;
    QString str = ui->ruleLineEdit->text();
    QIntValidator *v = (QIntValidator*)ui->ruleLineEdit->validator();

    if( v->validate(str, pos) != QValidator::Acceptable ){
        ui->ruleLineEdit->setText(QString::number(m_nRule));
        return;
    }

    m_nRule = str.toUInt();
}

void MainWindow::on_stopPushButton_clicked(){

}

void MainWindow::on_pausePushButton_clicked(){

}

void MainWindow::on_playPushButton_clicked(){
    G->show();
}
