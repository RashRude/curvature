#ifndef PAINTABLE_H
#define PAINTABLE_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <curvature.h>
#include <QPainter>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>
#include <math.h>
class paintable : public QLabel
{
    Q_OBJECT
public:
    explicit paintable(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

    void wheelEvent(QWheelEvent *ev);
//    void mouseDoubleClickEvent(QMouseEvent *ev);
    void gridSet(bool onOff);
    void drawBackground(QString backpath);
    void setMarkcount(int num);
    void mark1(int x,int y);
    void mark2(int x,int y);
    void mark3(int x,int y);
    void arcDraw();
    void picZoom(double zoomcoe);
    void picMove(int zoomx,int zoomy);


signals:

//    void mousePressed();
    void mousePos(int x,int y);
    void mouseMove(int x,int y);
    void mouseEndpos(int x,int y);
    void mouseMidpos(int x,int y);
    void mouseMidmove(int x,int y);
    void mouseZoom(double zoom);
//    void mouseLeft();

public:
    double x=0,y=0,r=0,length=0;
    double  startangle=0;
    double  spanangle=0;
    bool grid=false;
    int markcount=0;
    int px=0,py=0,lx=0,ly=0,rx=0,ry=0;
    double zoomCoefficient=1;
    int zoomX=0,zoomY=0;
    int backColor=255;
    QString backImage=NULL;
};

#endif // PAINTABLE_H
