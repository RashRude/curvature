
#include "curvature.h"
#include "ui_curvature.h"
/*
 *

    double curvatureValue=0;
    double scaleValue=1;



 *
 *
 */
void curvature::writeConfig()
{
    QString fileName;
    fileName = QCoreApplication::applicationDirPath();
    fileName+="/autoSet/config.ini";
    QSettings settings(fileName, QSettings::IniFormat);

    settings.setValue(tr("scaleValue"), scaleValue);



}
void curvature::readConfig()
{
    QString fileName;
    fileName = QCoreApplication::applicationDirPath();
    fileName+="/autoSet/config.ini";
    QSettings settings(fileName, QSettings::IniFormat);
    scaleValue=settings.value("scaleValue").toDouble();


}
