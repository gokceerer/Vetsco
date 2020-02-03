#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "circuit.h"
#include "currentcircuit.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <QtCharts>
#include <sys/wait.h>
#include <sys/types.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
