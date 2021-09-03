#include "paintable.h"

paintable::paintable(QWidget *parent) : QLabel(parent)
{

}
void paintable::mousePressEvent(QMouseEvent *ev)
{
    if(ev->buttons()==Qt::LeftButton)
    {
        int mx=0;
        int my=0;
        mx=ev->x();
        my=ev->y();
//        if(mx > this->width()||mx<0 || my<0||my>this->height())
//        {
//            emit mousePos(0,0);
//        }
//        else{
//            emit mousePos(mx,my);
//        }
        emit mousePos(mx,my);
    }
    else if(ev->buttons()==Qt::MidButton)
    {
        int mx=0;
        int my=0;
        mx=ev->x();
        my=ev->y();
        emit mouseMidpos(mx,my);
    }

}
void paintable::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->buttons()==Qt::LeftButton)
    {
        int mx=0;
        int my=0;
        mx=ev->x();
        my=ev->y();
//        if(mx > this->width()||mx<0 || my<0||my>this->height())
//        {
//            emit mouseMove(0,0);
//        }
//        else{
//            emit mouseMove(mx,my);
//        }
    emit mouseMove(mx,my);
    }
    else if(ev->buttons()==Qt::RightButton)
    {
        int mx=0;
        int my=0;
        mx=ev->x();
        my=ev->y();
//        if(mx > this->width()||mx<0 || my<0||my>this->height())
//        {
//            emit mouseEndpos(0,0);
//        }
//        else{
//            emit mouseEndpos(mx,my);
//        }
        emit mouseEndpos(mx,my);
    }
    else if(ev->buttons()==Qt::MidButton)
    {
        int mx=0;
        int my=0;
        mx=ev->x();
        my=ev->y();
        emit mouseMidmove(mx,my);
    }
}
void paintable::wheelEvent(QWheelEvent *ev)
{
    if(ev->delta()>0)
    {
        double coe=1.05;
        emit mouseZoom(coe);

    }
    else
    {
        double coe=0.95;
        emit mouseZoom(coe);
    }
}



//void paintable::mouseDoubleClickEvent(QMouseEvent *ev)
//{
//    if(ev->buttons()==Qt::LeftButton)
//    {
//        int mx=0;
//        int my=0;
//        mx=ev->x();
//        my=ev->y();
//        if(mx > this->width()||mx<0 || my<0||my>this->height())
//        {
//            emit mousePos(0,0);
//        }
//        else{
//            emit mousePos(mx,my);
//        }
//    }
//}

void paintable::drawBackground(QString backpath)
{
    backImage=backpath;
    update();
}
void paintable::setMarkcount(int num)
{
    markcount=num;
    update();
}
void paintable::mark1(int x,int y)
{
    px=x;
    py=y;
    update();
}
void paintable::mark2(int x,int y)
{

    lx=x;
    ly=y;
    update();
}
void paintable::mark3(int x,int y)
{
    rx=x;
    ry=y;
    update();
}
void paintable::picMove(int zoomx, int zoomy)
{
    zoomX=zoomx;
    zoomY=zoomy;
    update();
}
void paintable::picZoom(double zoomcoe)
{
    zoomCoefficient=zoomcoe;
    update();
}

void paintable::arcDraw()
{
    double a, b, e,OA,OB,OC;
    a = (px + lx) * (px - lx) + (py + ly) * (py - ly);
    b = (rx + lx) * (rx - lx) + (ry + ly) * (ry - ly);
    e = (px - lx) * (ry - ly) - (lx - rx) * (ly - py);

    x = (a * (ry - ly) + b * (ly - py)) / (2 * e);
    y = (a * (lx - rx) + b * (px - lx)) / (2 * e);
    r = sqrt((lx - x) * (lx - x) + (ly - y) * (ly - y));

    if(lx==px&&ry==y)
    {
        if(ly>py)
        {
            OA=90;
            OB=-90;
            OC=0;
        }
        else
        {
            OA=-90;
            OB=90;
            OC=0;
        }
    }
    else
    {
        OA = atan2(-py + y,px - x)*(180/atan2(0,-1));
//        qDebug()<<"OA"<<OA;
        OB = atan2(-ly + y,lx - x)*(180/atan2(0,-1));
//        qDebug()<<"OB"<<OB;
        OC = atan2(-ry + y,rx - x)*(180/atan2(0,-1));
//        qDebug()<<"OC"<<OC;
    }

//    startangle = OA*16;
//    if(OB>OA)
//    {
//    spanangle=(OB-OA)*16;
//    }
//    else
//    {
//        spanangle=(OA-OB)*16;
//    }

    if (OA>OB)
    {
        if(OC>OA||OC<OB)
        {
            startangle = OA*16;
            spanangle=(360-OA+OB)*16;
        }
        else
        {
            startangle = OB*16;
            spanangle=(OA-OB)*16;
        }
    }
    else
    {
        if(OC>OB||OC<OA)
        {
            startangle = OB*16;
            spanangle=(360-OB+OA)*16;
        }
        else
        {
            startangle = OA*16;
            spanangle=(OB-OA)*16;
        }
    }



}
void paintable::gridSet(bool onOff)
{
    grid=onOff;
    update();
}
void paintable::paintEvent(QPaintEvent *)
{

    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing);

    //background
    painter.setPen(Qt::NoPen);
    if (grid==true)
    {
        backColor=255;

    }
    else
    {
        backColor=1;
    }
    painter.setBrush(QColor(255,255,255,backColor));
    painter.drawRect(0,0,width(),height());

//    painter.setOpacity(0.4);
    if(backImage!=NULL)
    {
    QPixmap bImage(backImage);
    bImage=bImage.scaled(width()*zoomCoefficient, height()*zoomCoefficient, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放

    painter.drawPixmap((width()-bImage.width())/2+zoomX,(height()-bImage.height())/2+zoomY,bImage);
    }
   //画mark点
    QPen markPoint;
    markPoint.setWidth(5);
    markPoint.setCapStyle(Qt::RoundCap);
    markPoint.setColor(Qt::red);
    painter.setPen(markPoint);
    switch (markcount) {
    case 1:
        painter.drawPoint(px,py);
        break;
    case 2:
        painter.drawPoint(px,py);
        painter.drawPoint(lx,ly);
        length=sqrt((lx - px) * (lx - px) + (ly - py) * (ly - py));
        markPoint.setWidth(2);
        markPoint.setStyle(Qt::DashLine);
        painter.setPen(markPoint);
        painter.drawLine(lx,ly,px,py);
        break;
    case 3:
        arcDraw();
        painter.drawPoint(px,py);

        painter.drawPoint(lx,ly);
        markPoint.setColor(Qt::blue);
        markPoint.setWidth(2);
        painter.setPen(markPoint);

        painter.drawPoint(rx,ry);

        painter.drawPoint(x,y);
        painter.drawLine(x,y,px,py);
        painter.drawLine(x,y,lx,ly);
        markPoint.setColor(Qt::red);
        markPoint.setWidth(2);

        markPoint.setStyle(Qt::DashLine);
        painter.setPen(markPoint);
        painter.drawLine(x,y,rx,ry);
        painter.drawArc(x-r,y-r,2*r,2*r,startangle,spanangle);
        break;

    default:
        break;
    }
//    painter.drawPoint(px,py);


    if(grid==true)
    {
        QPen pen2;
        pen2.setWidth(1);
        pen2.setStyle(Qt::DotLine);
        pen2.setColor(QColor(0,0,0,128));
        painter.setPen(pen2);
        int numx=0;
        while (numx<=width()/20) {
            painter.drawLine(20*numx,0,20*numx,height());
            numx++;
        }
        int numy=0;
        while (numy<=height()/20) {
            painter.drawLine(0,20*numy,width(),20*numy);
            numy++;
        }

    }



}
