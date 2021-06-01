#include "curvature.h"
#include "ui_curvature.h"

curvature::curvature(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::curvature)
{
    ui->setupUi(this);
}

curvature::~curvature()
{
    delete ui;
}
