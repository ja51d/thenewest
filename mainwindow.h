#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QWidget>
#include <QUdpSocket>
#include <QUrl>
#include <QVideoWidget>
#include <QMediaContent>
#include <QMediaRecorder>
#include <QMediaPlayer>
#include <qqml.h>
#include <QTextStream>
#include <QTimer>
#include <QVariant>
#include <QtCore>
#include <QtGui>
#include <QtQuick>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow;
}
QT_END_NAMESPACE

 struct CanSatdata   // DATA NAMES
{
    short int packetnumber = 0;
    float cansatstatus = 0;
    unsigned char errorcode = 0;
    int64_t sendtime = 0;
    float basinc1 = 0;
    float basinc2 = 0;
    float height1 = 0;
    float height2 = 0;
    float irtifafarki = 0;
    float landingspeed = 0;
    float temperature = 0;
    float voltage = 0;
    float lat = 39.159076080;
    float lon = 33.2915507836;
    float alt = 0;
    float pitch = 0;
    float roll = 0;
    float yaw = 0;
    int rhrh = 0;
    float iot = 0;
   int teamno = 0;

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void timerFunc();
    void readData();
    void writeData();

    void on_rgb_clicked();
    void rgbData();
    void iotData();
    void baslat_transfer();
    void on_ayril_clicked();

    void on_pushButton_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTimer *timer2;
    QUdpSocket *socket;
    QUdpSocket *socket2;
    QUdpSocket *socket5;
    QUdpSocket *socket6;
    QUdpSocket *socket4;
    CanSatdata datas;
    QMediaPlayer* player;
  QMediaPlayer* sound;
signals:

    void setCenter(QVariant, QVariant);
    void eulerFunction(QVariant, QVariant, QVariant);
    void setLocMarker(QVariant,QVariant);
};


#endif // MAINWINDOW_H
