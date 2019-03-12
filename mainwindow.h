#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <matrix.h>
#include "qcustomplot.h"
#include "math.h"

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

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QCPItemLine *vec1, *vec2;
    Matrix A, B, eye, MN, M, N, temp, temp2, temp3, x, y;

    //my
    const double PI = 3.14159265358979323;
    const double EULER = 2.7182818284595;
    const double ls = 1e-3;
    const double rs = 0.001;
    const double cd = 1000e-6;
    const float rload = 20;
    const double deltat = 25e-6;
    const float freq = 60;
    const float thalf = 1/(2*freq);
    const float ampl = 170;
    const double w = 2*PI*freq;
    double alfa0[5000], il[5000], vc[5000], vs[5000], time[5000] = {};
    int arrlength = 5000;
    double vc0 = 0;
    double il0 = 0;
    int k = 0;
    float time1, il1, vc1 = 0;
};

#endif // MAINWINDOW_H
