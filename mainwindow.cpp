#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    vec1(nullptr),
    vec2(nullptr)
{
    ui->setupUi(this);

    // set the user interactions for the matrix plot
    ui->mainGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->mainGraph->addGraph();
    ui->mainGraph->addGraph();
    ui->mainGraph->addGraph();
    ui->mainGraph->graph(0)->setPen(QPen(QColor(Qt::blue)));
    ui->mainGraph->graph(1)->setPen(QPen(QColor(Qt::red)));
    ui->mainGraph->graph(2)->setPen(QPen(QColor(Qt::green)));

    vec1 = new QCPItemLine(ui->mainGraph);
    vec1->setPen(QPen(QColor(66, 244, 66)));
    vec1->setHead(QCPLineEnding::esSpikeArrow);
    vec1->start->setCoords(0, 0);
    vec1->end->setCoords(0, 0);

    vec2 = new QCPItemLine(ui->mainGraph);
    vec2->setPen(QPen(QColor(244, 66, 66)));
    vec2->setHead(QCPLineEnding::esSpikeArrow);
    vec2->start->setCoords(0, 0);
    vec2->end->setCoords(0, 0);
}

MainWindow::~MainWindow()
{
    delete vec1;
    delete vec2;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QPointF vp1(ui->vector1->item(0, 0)->text().toDouble(), ui->vector1->item(1, 0)->text().toDouble());
    QPointF vp2(ui->vector2->item(0, 0)->text().toDouble(), ui->vector2->item(1, 0)->text().toDouble());
    vec1->end->setCoords(vp1);
    vec2->end->setCoords(vp2);
    ui->mainGraph->replot();
}

void MainWindow::on_pushButton_2_clicked()
{
   A.setSize(2,2);
   B.setSize(2,1);
   eye.setSize(2,2);

   A.setValue(0,0,-rs/ls);
   A.setValue(0,1,-1.0/ls);
   A.setValue(1,0,1.0/cd);
   A.setValue(1,1,-1.0/(cd*rload));

   B.setValue(0,0,1.0/ls);
   B.setValue(1,0,0.0);

   eye.setValue(0,0,1.0); //identity matrix
   eye.setValue(0,1,0.0);
   eye.setValue(1,0,0.0);
   eye.setValue(1,1,1.0);

   temp = A.multiply(deltat/2);
   temp2 = eye.subtract(temp);
   temp2 = temp2.inverse();
   temp3 = eye.add(temp);
   M = temp2.multiply(temp3);

   temp = temp2.multiply(B);
   N = temp.multiply(deltat/2);

   for(int i = 0; i < 41; i++){
       alfa0[i] = 55 + i*0.5;
       vc0 = ampl*sin(alfa0[i]*PI/180);
       il0 = 0;
       k = 0;
       vc[0] = vc0;
       il[0] = il0;
       vs[0] = vc0;
       time[0] = alfa0[i]/(360*freq);
       x.setSize(2,1);
       x.setValue(0, 0, il[0]);
       x.setValue(1, 0, vc[0]);

       while(il[k] >= 0){
           k++;
           time[k] = time[k-1] + deltat;
           temp = M.multiply(x);
           temp2 = N.multiply(ampl*sin(w*time[k]) + ampl*sin(w*time[k-1]));
           y = temp.add(temp2);
           il[k] = y.getValue(0, 0);
           vc[k] = y.getValue(1, 0);
           vs[k] = ampl*sin(w*time[k]);
           x = y;

           ui->mainGraph->graph(0)->addData(time[k], il[k]);
           ui->mainGraph->graph(1)->addData(time[k], vc[k]);
           ui->mainGraph->graph(2)->addData(time[k], vs[k]);

           qDebug() << k;
           qDebug() << il[k];
       }

       time1 = time[k];
       il1 = 0;
       vc1 = vc[k];

       while(vc[k] > ampl*abs(sin(w*time[k]))){
           k++;
           time[k] = time[k-1] + deltat;
           vc[k] = vc1*pow(EULER, -(time[k]-time1)/(cd*rload));
           vs[k] = ampl*abs(sin(w*time[k]));
           il[k] = 0;

           ui->mainGraph->graph(0)->addData(time[k], il[k]);
           ui->mainGraph->graph(1)->addData(time[k], vc[k]);
           ui->mainGraph->graph(2)->addData(time[k], vs[k]);

           qDebug() << k;
           qDebug() << vc[k];
        }
       qDebug() << "i:";
       qDebug() << i;

       if(abs(time[k] - thalf - time[0]) <= 2*deltat){break;}
    }
   ui->mainGraph->rescaleAxes(true);
   ui->mainGraph->replot();

//   qDebug() << eye.getValue(0,0);
//   qDebug() << MN.getValue(0,1);
//   qDebug() << MN.getValue(1,0);
//   qDebug() << MN.getValue(1,1);
}
