#include "curvature.h"
#include "ui_curvature.h"

curvature::curvature(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::curvature)
{
    //    QWidget::setCursor(QCursor(Qt::BlankCursor));
    ui->setupUi(this);
    this->setWindowTitle("Curvature Measure");
    setWindowIcon(QIcon("://icon.png"));
    //    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);//需要去掉标题栏


    setAttribute(Qt::WA_TranslucentBackground);
    //    connect(ui->actionopen,&QAction::triggered,this,&curvature::fileOpen);
    //    connect(ui->actiongrid,&QAction::triggered,this,&curvature::gridOn);
    //    connect(ui->actionscale,&QAction::triggered,this,&curvature::scaleSet);
    //    connect(ui->actioncurvature,&QAction::triggered,this,&curvature::curvatureSet);
    //    connect(ui->actiondataexp,&QAction::triggered,this,&curvature::dataExport);
    ui->scaleCalibration->setEnabled(false);
    ui->rowAddbutton->setEnabled(false);
    this->showMaximized();

    /* 创建数据模型 */
    myModel = new QStandardItemModel();
    /* 设置表格标题行(输入数据为QStringList类型) */
    myModel->setHorizontalHeaderLabels({"Time", "Curvature", "CurvatuerEorr", "Length","LengthEorr", "Angle","AngleEorr","MeasureCount"});
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setModel(myModel);



    readConfig();


    //创建自动备份数据路径
    QString fileName = QCoreApplication::applicationDirPath();
    fileName+="/autoSave";
    QDir* dir = new QDir();
    if(!dir->exists(fileName))
    {
        dir->mkpath(fileName);
    }
}

curvature::~curvature()
{
    writeConfig();
    delete myModel;
    delete ui;
}

void curvature::keyPressEvent(QKeyEvent *ev)
{
    switch (ev->key())
    {
    case Qt::Key_Control:
        disconnect(ui->draw,&paintable::mouseMidpos,0,0);
        disconnect(ui->draw,&paintable::mouseMidmove,0,0);
        disconnect(ui->draw,&paintable::mouseZoom,0,0);
        connect(ui->draw,&paintable::mouseMidpos,this,&curvature::mousePicpos);
        connect(ui->draw,&paintable::mouseMidmove,this,&curvature::mousePicmove);
        connect(ui->draw,&paintable::mouseZoom,this,&curvature::mousePiczoom);
        break;
    case Qt::Key_Escape:
        disconnect(ui->draw,&paintable::mouseMidpos,0,0);
        disconnect(ui->draw,&paintable::mouseMidmove,0,0);
        disconnect(ui->draw,&paintable::mouseZoom,0,0);
        ui->draw->picZoom(1);
        ui->draw->picMove(0,0);
        break;
    default:
        break;

    }
}
void curvature::keyReleaseEvent(QKeyEvent *ev)
{
    switch (ev->key())
    {
    case Qt::Key_Control:
        disconnect(ui->draw,&paintable::mouseMidpos,0,0);
        disconnect(ui->draw,&paintable::mouseMidmove,0,0);
        disconnect(ui->draw,&paintable::mouseZoom,0,0);
        break;
    default:
        break;
    }
}
void curvature::fullScreenmode()
{
    this->showFullScreen();
}
void curvature::mousePicmove(int x,int y)
{

    ui->draw->picMove(x-focusX,y-focusY);

}
void curvature::mousePicpos(int x,int y)
{
    focusX=x-ui->draw->zoomX;
    focusY=y-ui->draw->zoomY;
    //    ui->draw->picMove(focusX,focusY);
}
void curvature::mousePiczoom(double zoom)
{
    //    zoomValue=zoomValue*zoom;

    ui->draw->picZoom(zoom*ui->draw->zoomCoefficient);
}



void curvature::curvatureSet()
{

    disconnect(ui->draw,&paintable::mousePos,0, 0);
    disconnect(ui->draw,&paintable::mouseMove,0, 0);
    disconnect(ui->draw,&paintable::mouseEndpos,0, 0);
    connect(ui->draw,&paintable::mousePos,this, &curvature::mouseMark1);
    connect(ui->draw,&paintable::mouseMove,this, &curvature::mouseMark2);
    connect(ui->draw,&paintable::mouseEndpos,this, &curvature::mouseMark3);
    ui->stateMessage->setText(tr("Measure Mode"));
}
void curvature::scaleSet()
{

    disconnect(ui->draw,&paintable::mousePos,0, 0);
    disconnect(ui->draw,&paintable::mouseMove,0, 0);
    disconnect(ui->draw,&paintable::mouseEndpos,0, 0);
    connect(ui->draw,&paintable::mousePos,this, &curvature::mouseMark1);
    connect(ui->draw,&paintable::mouseMove,this, &curvature::mouseMark2);
    ui->scaleCalibration->setEnabled(true);

}


void curvature::gridOn()
{
    ui->draw->gridSet(!(ui->draw->grid));
}
void curvature::fileOpen()
{
    photoPath.clear();
    photoPath.squeeze();
    ui->photoList->clear();
    curvatureVec.clear();
    curvatureVec.squeeze();
    lengthVec.clear();
    lengthVec.squeeze();
    angleVec.clear();
    angleVec.squeeze();
    myModel->clear();
    Dir = QFileDialog::getExistingDirectory(this);//获取文件所在的具体路径
    //    ui->photoPath->setText(Dir);//显示打开的文件的具体路径
    QDir dir(Dir);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QStringList filters;
    filters << "*.png"<<"*.jpg";
    dir.setNameFilters(filters);
    QFileInfoList fileList = dir.entryInfoList(filters); //获取目录下的png文件
    QString fileDir; //保存图片所在的路径

    //列出目录下的文件
    for(int i=0;i<fileList.count();i++)
    {
        QFileInfo info = fileList.at(i);
        fileDir.clear();
        fileDir.append(Dir + "/");
        QString filename = info.fileName();
        fileDir.append(filename);
        photoPath.append(filename);// 把图片的路径装到容器中

        if(info.fileName() == "." || info.fileName() == "..") //跳过这两个目录
        {
            continue;
        }
        //      QListWidgetItem *item = new QListWidgetItem(QIcon(fileDir),info.fileName());//建立文件缩小图标
        QListWidgetItem *item = new QListWidgetItem(info.fileName());//
        ui->photoList->addItem(item);//把图片相对路径显示到窗口中
        //        ui->photoList->setCurrentItem(0);
    }
    ui->photoIndex->setMaximum(ui->photoList->count());
    ui->photoIndex->setValue(2);
    ui->photoIndex->setValue(1);

    myModel->setHorizontalHeaderLabels({"Time", "Curvature", "CurvatuerEorr", "Length","LengthEorr", "Angle","AngleEorr","MeasureCount"});
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setModel(myModel);

    dataCount=0;
}
//窗口图片
void curvature::on_photoList_itemSelectionChanged()
{
    tempDir.clear();
    tempDir.append(Dir+"/");
    QString path = ui->photoList->currentItem()->text();
    tempDir.append(path);

    ui->draw->drawBackground(tempDir);
    //    ui->preView->setScaledContents(true);
    //    ui->preView->setPixmap(tempDir);
}
//鼠标事件
void curvature::mouseMark1(int x,int y)
{
    //    qDebug()<<"x="<<x<<",y="<<y;
    ui->draw->setMarkcount(1);
    ui->draw->mark1(x,y);
}
void curvature::mouseMark2(int x,int y)
{
    //    qDebug()<<"x="<<x<<",y="<<y;
    ui->draw->setMarkcount(2);
    ui->draw->mark2(x,y);
    lengthValue=ui->draw->length/ui->draw->zoomCoefficient/scaleValue;
    ui->curPixellength->setText(QString("Length(pixel):%1").arg(ui->draw->length));
    ui->scaleCalibration->setText(QString("%1").arg(lengthValue));
}
void curvature::mouseMark3(int x,int y)
{
    //    qDebug()<<"x="<<x<<",y="<<y;
    ui->draw->setMarkcount(3);
    ui->draw->mark3(x,y);
    curvatureValue=scaleValue/(ui->draw->r/ui->draw->zoomCoefficient)*1000;

    angleValue=ui->draw->spanangle/16;
    ui->curCurvature->setText(QString("Curvature:%1").arg(curvatureValue));
    ui->curAngle->setText(QString("Angle:%1°").arg(angleValue));
}




void curvature::on_photoList_currentRowChanged(int currentRow)
{
    ui->photoIndex->setValue(currentRow+1);
}

void curvature::on_photoIndex_valueChanged(int arg1)
{
    ui->photoList->setCurrentRow(arg1-1);
}

void curvature::on_scaleCalibration_editingFinished()
{
    double lengthP=ui->draw->length;
    double zoomCoe=ui->draw->zoomCoefficient;
    double lengthC=ui->scaleCalibration->text().toDouble();
    scaleValue=lengthP/zoomCoe/lengthC;
    ui->scaleCalibration->setEnabled(false);
    ui->checkScale->setCheckState(Qt::Unchecked);
}


//数据处理

void curvature::on_lineAddbutton_clicked()
{
    int timeIndx=ui->photoIndex->value()-1;

    if(ui->photoList->count()!=0)
    {
        ui->photoList->setCurrentRow(timeIndx);
    }

    myModel->setItem(dataCount,0,new QStandardItem(QString("%1").arg(timeIndx)));
    for(int i=1;i<=7;i++)
    {
        myModel->setItem(dataCount,i,new QStandardItem("0"));
    }
    ui->tableView->selectRow(dataCount);
    dataCount++;
    ui->rowAddbutton->setEnabled(true);
    ui->tableView->show();
}

double curvature::dataSD(QVector<double> resultSet)
{
    double mean=dataMean(resultSet);
    double accum  = 0.0;
    std::for_each (std::begin(resultSet), std::end(resultSet), [&](const double d) {
        accum  += (d-mean)*(d-mean);
    });

    double stdev = sqrt(accum/(resultSet.size()-1)); //方差
    return stdev;
}
double curvature::dataMean(QVector<double> resultSet)
{
    double sum = std::accumulate(std::begin(resultSet), std::end(resultSet), 0.0);
    double mean =  sum / resultSet.size(); //均值
    return mean;
}

void curvature::on_rowAddbutton_clicked()
{
    int dataIndx=ui->tableView->currentIndex().row();

    measureCount++;
    curvatureVec.append(curvatureValue);
    lengthVec.append(lengthValue);
    angleVec.append(angleValue);

    myModel->setItem(dataIndx,1,new QStandardItem(QString("%1").arg(dataMean(curvatureVec))));
    myModel->setItem(dataIndx,2,new QStandardItem(QString("%1").arg(dataSD(curvatureVec))));
    myModel->setItem(dataIndx,3,new QStandardItem(QString("%1").arg(dataMean(lengthVec))));
    myModel->setItem(dataIndx,4,new QStandardItem(QString("%1").arg(dataSD(lengthVec))));
    myModel->setItem(dataIndx,5,new QStandardItem(QString("%1").arg(dataMean(angleVec))));
    myModel->setItem(dataIndx,6,new QStandardItem(QString("%1").arg(dataSD(angleVec))));
    myModel->setItem(dataIndx,7,new QStandardItem(QString("%1").arg(measureCount)));
}

void curvature::on_clearMeasurebutton_clicked()
{
    measureCount=0;
    curvatureVec.clear();
    lengthVec.clear();
    angleVec.clear();
    curvatureVec.squeeze();
    lengthVec.squeeze();
    angleVec.squeeze();
    curvatureSet();
}
void curvature::dataExport()
{
    QString fileName = QFileDialog::getSaveFileName(this,"data", Dir, tr("csv files(*.csv)"));    //设置保存的文件名
    if(fileName != NULL)
    {
        QFile file(fileName);
        file.open( QIODevice::WriteOnly | QIODevice::Text );
        if(!file.isWritable())
        {
            QMessageBox::warning(this,tr("Error"),tr("Fail to save !"));
        }
        else
        {

            //            ui->statusBar->showMessage(tr("Exporting data..."));
            ui->stateMessage->setText(tr("Exporting data..."));
            QTextStream out(&file);

            out<<tr("Time,")<<tr("Curvature,")<<tr("CurvatuerEorr,")<<tr("Length(mm),")
              <<tr("LengthEorr,")<<tr("Angle,")<<tr("AngleEorr,")<<tr("MeasureCount,\n");//表头

            //               out<<tr("%1,").arg(myModel->item(0,1)->text());

            for(int j=0;j<myModel->rowCount();j++)
            {
                for(int k=0;k<myModel->columnCount();k++)
                {
                    out<<tr("%1,").arg(myModel->item(j,k)->text());
                }
                out<<tr("\n");
            }

            file.close();
            //            ui->statusBar->showMessage(tr("Exported completely"));
            ui->stateMessage->setText(tr("Exported completely"));
        }
    }
}

void curvature::on_checkGrid_stateChanged(int arg1)
{
    ui->draw->gridSet(arg1);
}

void curvature::on_openFile_clicked()
{
    fileOpen();
}


void curvature::on_checkScale_stateChanged(int arg1)
{
    disconnect(ui->draw,&paintable::mousePos,0, 0);
    disconnect(ui->draw,&paintable::mouseMove,0, 0);
    disconnect(ui->draw,&paintable::mouseEndpos,0, 0);
    connect(ui->draw,&paintable::mousePos,this, &curvature::mouseMark1);
    connect(ui->draw,&paintable::mouseMove,this, &curvature::mouseMark2);
    ui->scaleCalibration->setEnabled(arg1);
    ui->stateMessage->setText(tr("Scale Calibration"));
}

void curvature::on_closeButton_clicked()
{
    QString fileName = QCoreApplication::applicationDirPath();
    fileName+="/autoSave/dataAutosave.csv";
    QFile file(fileName);
    file.open( QIODevice::WriteOnly | QIODevice::Text );
    if(!file.isWritable())
    {
        QMessageBox::warning(this,tr("Error"),tr("Fail to set autosave !"));
    }
    else
    {
        if(myModel!=NULL)
        {

            QTextStream out(&file);
            out<<tr("Time,")<<tr("Curvature,")<<tr("CurvatuerEorr,")<<tr("Length(mm),")
              <<tr("LengthEorr,")<<tr("Angle,")<<tr("AngleEorr,")<<tr("MeasureCount,\n");//表头
            for(int j=0;j<myModel->rowCount();j++)
            {
                for(int k=0;k<myModel->columnCount();k++)
                {
                    out<<tr("%1,").arg(myModel->item(j,k)->text());
                }
                out<<tr("\n");
            }
            file.close();
        }
    }


    this->close();
}

void curvature::on_exportFile_clicked()
{
    dataExport();
}


void curvature::screenCapture()
{

    this->showMinimized();
    QRect rec=ui->draw->geometry();


    QScreen *pscreen = QApplication::primaryScreen();
    QPixmap tempic=pscreen->QScreen::grabWindow(QApplication::desktop()->winId(),0,0,-1,-1);



    QString fileName = QCoreApplication::applicationDirPath();
    fileName+="/picSave";
    QDir* dir = new QDir();
    if(!dir->exists(fileName))
    {
        dir->mkpath(fileName);
    }
    dir->cd(fileName);
    fileName+=QString("/dataAutosave%1.png").arg(ui->photoIndex->value());

    tempic.copy(rec).save(fileName,"png");
    dataCount=0;
    ui->draw->drawBackground(fileName);
    this->showMaximized();
}
void curvature::on_pushButton_clicked()
{
    screenCapture();
}



void curvature::on_minimizeButton_clicked()
{
    showMinimized();
}
