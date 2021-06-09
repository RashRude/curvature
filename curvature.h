#ifndef CURVATURE_H
#define CURVATURE_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include <QCoreApplication>
#include <QSettings>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QScreen>
#include <QDesktopWidget>

namespace Ui {
class curvature;
}

class curvature : public QMainWindow
{
    Q_OBJECT

public:
    explicit curvature(QWidget *parent = nullptr);

    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);
    void screenCapture();

    ~curvature();
private slots:
     void fileOpen();
     void gridOn();
     void scaleSet();
     void curvatureSet();
     void on_photoList_itemSelectionChanged();
     void mouseMark1(int x,int y);
      void mouseMark2(int x,int y);
     void mouseMark3(int x,int y);
     void mousePicmove(int x,int y);
     void mousePicpos(int x,int y);
     void mousePiczoom(double zoom);
     void dataExport();
     void on_photoList_currentRowChanged(int currentRow);

     void on_photoIndex_valueChanged(int arg1);

     void on_scaleCalibration_editingFinished();
     void on_lineAddbutton_clicked();

     void on_rowAddbutton_clicked();

     void on_clearMeasurebutton_clicked();
     void fullScreenmode();
     void on_checkGrid_stateChanged(int arg1);

     void on_openFile_clicked();


     void on_checkScale_stateChanged(int arg1);

     void on_closeButton_clicked();

     void on_exportFile_clicked();


     void on_pushButton_clicked();



     void on_minimizeButton_clicked();

protected:
     void writeConfig();
     void readConfig();
     double dataMean(QVector<double> resultSet);
     double dataSD(QVector<double> resultSet);

private:
    Ui::curvature *ui;
    QString Dir;//打开文件的路径
    QString tempDir;
    int dataCount=0;
    int measureCount=0;
    int focusX=0;
    int focusY=0;
    double curvatureValue=0;
    double angleValue=0;
    double lengthValue=0;
    double scaleValue=1;
    QStandardItemModel* myModel=NULL;
    QVector<double> curvatureVec;
    QVector<double> lengthVec;
    QVector<double> angleVec;
    QVector<QString> photoPath;
};

#endif // CURVATURE_H
