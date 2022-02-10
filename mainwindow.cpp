#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QtMath>

#include "ui_mainwindow.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::readAcmiFile(const QString &fileName)
{
    QRegularExpression regex(R"(^(#)([0-9.]+)|^([A-Fa-f0-9]+)(?:\,)|^(-)([A-Fa-f0-9]+))"
                             R"(|(ReferenceLongitude)=([0-9.+\-]+))"
                             R"(|(ReferenceLatitude)=([0-9.+\-]+))"
                             R"(|(T)=([0-9|.+\-]+))"
                             R"(|(Name)=([A-Za-z0-9\-]+))"
                             R"(|(Type)=([A-Za-z0-9_\-]+))"
                             R"(|(Pilot)=([A-Za-z0-9\-]+))"
                             R"(|(CallSign)=([A-Za-z0-9_\-]+))"
                             R"(|(Color)=([A-Za-z]+))"
                             );
    QString line;
    QStringList values;
    quint32 id = 0;
    float referenceLongitude = 0;
    float referenceLatitude = 0;

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    if(file.readLine().indexOf("FileType") < 0) return false;
    if(file.readLine().indexOf("FileVersion") < 0) return false;

    ui->horizontalSlider->setValue(0);
    ui->comboBox->clear();
    ui->openGLWidget->Cleanup();

    while(!file.atEnd())
    {
        line = file.readLine();

        QRegularExpressionMatchIterator i = regex.globalMatch(line);

        for(int expression=0; i.hasNext(); expression++) {
            QStringList list = i.next().capturedTexts();

            for(int match=0; match < list.size()-1; match++) {
                if(list[match]=="#") {
                    ui->openGLWidget->beginSample();
                }
                else if(list[match]=="-") {
                    ui->openGLWidget->deleteModel(list[match+1].toUInt(nullptr, 16));
                }
                else if(list[match]=="ReferenceLongitude") {
                    referenceLongitude = list[match+1].toFloat();
                }
                else if(list[match]=="ReferenceLatitude") {
                    referenceLatitude = list[match+1].toFloat();
                }
                else if(list[match]=="T") {
                    QStringList values = list[match+1].split('|');
                    if(values.size() >= 3)
                    {
                        bool ok1, ok2;
                        float lon = values[0].toFloat(&ok1);
                        float lat = values[1].toFloat(&ok2);

                        if(ok1 && ok2) {
                            lon+=referenceLongitude;
                            lat+=referenceLatitude;
                        }
                        else {
                            lon = NAN;
                            lat = NAN;
                        }

                        // avoid north/south pole
                        if(fabs(lat) > 80) return false;
                        if(fabs(lon) > 179) return false;

                        // scale to km, 90 deg (equator to pole) = 10000km
                        float xpos = (10000*lon)/90;
                        float ypos = (10000*lat)/90;
                        float alt = 0.001f*values[2].toFloat(&ok1);
                        if(!ok1) alt=NAN;

//						float scaling = cos(lat*M_PI/180);
//						xpos *= scaling;
                        if(values.size() >= 6) {
                            ui->openGLWidget->setWaypoint(id,
                                                          QVector3D(xpos, ypos, alt),
                                                          QVector3D(values[3].toFloat(), values[4].toFloat(), values[5].toFloat()));
                        }
                        else {
                            ui->openGLWidget->setWaypoint(id,
                                                          QVector3D(xpos, ypos, alt),
                                                          QVector3D());
                        }
                    }
                }
                else if(list[match]=="Name" || list[match]=="Type") {
                    ui->openGLWidget->setModel(id, list[match+1]);
                }
                else if(list[match]=="Pilot" || list[match]=="CallSign") {
                    ui->comboBox->addItem(list[match+1], id);
                }
                else if(list[match]=="Color") {
                    QString colorStr = list[match+1].toLower();

                    if(colorStr=="red") {
                        ui->openGLWidget->setColor(id, QColor(Qt::red));
                    }
                    else if(colorStr=="blue") {
                        ui->openGLWidget->setColor(id, QColor(Qt::blue));
                    }
                    else if(colorStr=="yellow") {
                        ui->openGLWidget->setColor(id, QColor(Qt::yellow));
                    }
                }
                else {
                    if(expression==0 && match==2) {
                        // id is hex value
                        id = list[match+1].toUInt(nullptr, 16);
                    }
                }
            }
        }
    }
    file.close();
    ui->openGLWidget->finish();
    ui->openGLWidget->setReference(ui->comboBox->itemData(0).toUInt());
    return true;
}

void MainWindow::on_actionOpen_triggered()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Open Flight Recording"),
                                              QString(), tr("ACMI File (*.acmi)"));

    if(!fn.isEmpty())
    {
        if(fn.toLower().contains(".acmi"))
        {
            if(!readAcmiFile(fn)) {
                QMessageBox::critical(this, tr("Error"), tr("Invalid ACMI-File"));
            }
        }
        ui->openGLWidget->repaint();
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, tr("About"), "<b>ACMIPlot3D</b><br>Viewer for ACMI files<br>Version: [" GIT_HASH "]");
}

void MainWindow::on_comboBox_currentIndexChanged(int)
{
    ui->openGLWidget->setReference(ui->comboBox->currentData().toUInt());
    ui->openGLWidget->repaint();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->openGLWidget->setIndex(value);
}
