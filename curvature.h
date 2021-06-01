#ifndef CURVATURE_H
#define CURVATURE_H

#include <QMainWindow>

namespace Ui {
class curvature;
}

class curvature : public QMainWindow
{
    Q_OBJECT

public:
    explicit curvature(QWidget *parent = nullptr);
    ~curvature();

private:
    Ui::curvature *ui;
};

#endif // CURVATURE_H
