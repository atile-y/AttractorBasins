#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "glwidget.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_sizeLineEdit_editingFinished();
    void on_ruleLineEdit_editingFinished();
    void on_stopPushButton_clicked();
    void on_pausePushButton_clicked();
    void on_playPushButton_clicked();

private:
    Ui::MainWindow *ui;
    GLWidget *gl;
};

#endif // MAINWINDOW_H
