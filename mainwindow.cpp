#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QFile>
#include <QMediaContent>
#include <QMediaPlayer>
#include <QQuickItem>
#include <QtCore>
#include <QtGui>
#include <QMediaContent>
#include <QMediaPlayer>
#include <QRandomGenerator>
#include <QUdpSocket>
#include <QVariant>
#include <QWebEngineView>
#include <QDesktopServices>
#include <QUrl>
#include <QWidget>
#include <QVBoxLayout>
void MainWindow::readData() {

    while (socket4->hasPendingDatagrams()) {
        {
            while (socket4->hasPendingDatagrams()) {
                QByteArray datagram(socket4->pendingDatagramSize(), Qt::Uninitialized);
                socket4->readDatagram(datagram.data(), datagram.size());

                qDebug() << "Datagram raw data:" << datagram.toHex() << "," << datagram.size();

                int index = 0;
                auto extractData = [&](auto& value, int size) {
                    value = qFromLittleEndian<typename std::remove_reference<decltype(value)>::type>(
                        reinterpret_cast<const uchar*>(datagram.mid(index, size).constData()));
                    index += size;
                };

                extractData(datas.packetnumber, 2);
                datas.cansatstatus = static_cast<unsigned char>(datagram[index++]);
                datas.errorcode = static_cast<unsigned char>(datagram[index++]);
                extractData(datas.sendtime, 8);
                extractData(datas.basinc1, 4);
                extractData(datas.basinc2, 4);
                extractData(datas.height1, 4);
                extractData(datas.height2, 4);
                extractData(datas.irtifafarki, 4);
                extractData(datas.landingspeed, 4);
                extractData(datas.temperature, 4);
                extractData(datas.voltage, 4);
                extractData(datas.lat, 4);
                extractData(datas.lon, 4);
                extractData(datas.alt, 4);
                extractData(datas.pitch, 4);
                extractData(datas.roll, 4);
                extractData(datas.yaw, 4);
                extractData(datas.rhrh, 2);
                extractData(datas.iot, 4);
                extractData(datas.teamno, 4);
            }


        }
    }
}
void MainWindow::writeData(){

    QByteArray datagram;
    datagram.append('1');
    QHostAddress receiverAddress("127.0.0.1");
    socket2->writeDatagram(datagram, datagram.size(), receiverAddress, 5003);
}

void MainWindow::rgbData()

{
    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);
    out << datas.rhrh;
    QHostAddress receiverAddress("127.0.0.1");
    socket->writeDatagram(datagram, receiverAddress, 5001);
}

void MainWindow::iotData()

{
    float iot = 51.51;
    QHostAddress receiverAddress("127.0.0.1");
    QByteArray datagram;
    datagram.resize(sizeof(float));
    qToLittleEndian(iot, reinterpret_cast<uchar *>(datagram.data()));
    socket5->writeDatagram(datagram, receiverAddress, 8885);
}

void MainWindow::baslat_transfer()

{
    QByteArray datagram;
    datagram.append('1');
    QHostAddress receiverAddress("127.0.0.1");
    socket6->writeDatagram(datagram, datagram.size(), receiverAddress, 8883);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{

    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL (timeout()),this,SLOT(timerFunc()));

    QDateEdit *dateEdit = new QDateEdit(this);
    dateEdit->move(350, 10); // x , y
   QFont font2;
    font2.setPointSize(18);         // Set font size
    font2.setFamily("Copperplate");       // Set font style (family)
    font2.setBold(true);            // Set font weight to bold (optional)
    dateEdit->setFont(font2);
    dateEdit->resize(140, 35);
    dateEdit->setDate(QDate::currentDate());
    dateEdit->setDisplayFormat("dd.MM.yyyy");
    dateEdit->setStyleSheet(
        "QDateEdit {"
        "    background-color: #bfe5c4;"
        "    color: black;"
        "    border: 5px solid #305285;"
        "    border-radius: 10px;"
        "    padding: 5px;"
        "}"
        "QDateEdit::drop-down {"
        "    subcontrol-origin: padding;"
        "    subcontrol-position: top right;"
        "    width: 20px;"
        "    border-left-width: 1px;"
        "    border-left-color: darkgray;"
        "    border-left-style: solid;"
        "    border-top-right-radius: 3px;"
        "    border-bottom-right-radius: 3px;"
        "}"
        "QDateEdit::down-arrow {"
        "    image: url(:/);"
        "}"
        );
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(dateEdit);
    setLayout(layout);
    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/testqml.qml")));
    ui->quickWidget->show();
    ui->quickWidget_2->setSource(QUrl(QStringLiteral("qrc:/map.qml")));
    ui->quickWidget_2->show();
    auto objj = ui->quickWidget->rootObject();
    auto obj = ui->quickWidget_2->rootObject();
    connect(this, SIGNAL(setLocMarker(QVariant,QVariant)),obj,SLOT(setLocMarker(QVariant,QVariant)));
    connect(this, SIGNAL(eulerFunction(QVariant,QVariant,QVariant)), objj, SLOT(eulerFunction(QVariant,QVariant,QVariant)));
    emit setCenter(datas.lat,datas.lon);
    emit setLocMarker(datas.lat,datas.lon);
    emit eulerFunction(datas.roll, datas.yaw, datas.pitch);
  QWebEngineView *view = new QWebEngineView(this);
    view->setUrl(QUrl("http://172.20.10.3:6000/"));
    view->setGeometry(1275, 430, 400, 280);
    view->show();



    QPixmap pix(":/itutayf0.png");
    ui->logo->setPixmap(pix);
    QPixmap dur_1("/Users/Javid/desktop/green.png");
    ui->kod1->setPixmap(dur_1.scaled(40,40,Qt::KeepAspectRatio));
    ui->kod2->setPixmap(dur_1.scaled(40,40,Qt::KeepAspectRatio));
    ui->kod3->setPixmap(dur_1.scaled(40,40,Qt::KeepAspectRatio));
    ui->kod4->setPixmap(dur_1.scaled(40,40,Qt::KeepAspectRatio));
    ui->kod5->setPixmap(dur_1.scaled(40,40,Qt::KeepAspectRatio));

    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::Any, 5001);
    socket2 = new QUdpSocket(this);
    socket2->bind(QHostAddress::Any, 5003);
    socket5 = new QUdpSocket(this);
    socket5->bind(QHostAddress::Any, 8885);
    socket6 = new QUdpSocket(this);
    socket6->bind(QHostAddress::Any, 8883);
    socket4 = new QUdpSocket(this);
    socket4->bind(QHostAddress::Any, 8883);

    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssDisc);
    myScatter.setPen(QPen(Qt::black));
    myScatter.setSize(5);
    QFont plotFont = font();
    plotFont.setPointSize(9);
    QPen pen(QColor(169, 169, 169));
    pen.setWidth(13);
    QPen penB(QColor(169, 169, 169));
    pen.setWidth(4);
    ui->bas1->addGraph();
    ui->bas1->graph(0)->setScatterStyle(myScatter);
    ui->bas1->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->bas1->setBackground(QBrush(Qt::transparent));
    ui->bas1->graph(0)->setPen(pen);
    ui->bas1->xAxis->setLabel("Paket Numarası");
    ui->bas1->yAxis->setLabel("Basınç(Pa)");
    ui->bas1->xAxis->setLabelFont(plotFont);
    ui->bas1->xAxis->setTickLabelFont(plotFont);
    ui->bas1->yAxis->setLabelFont(plotFont);
    ui->bas1->yAxis->setTickLabelFont(plotFont);
    ui->bas1->xAxis->setTickLabelColor(Qt::white);
    ui->bas1->xAxis->setLabelColor(Qt::white);
    ui->bas1->xAxis->setSubTickPen(QPen(Qt::white));
    ui->bas1->xAxis->setBasePen(QPen(Qt::white));
    ui->bas1->yAxis->setTickLabelColor(Qt::white);
    ui->bas1->yAxis->setLabelColor(Qt::white);
    ui->bas1->yAxis->setSubTickPen(QPen(Qt::white));
    ui->bas1->yAxis->setBasePen(QPen(Qt::white));
    ui->bas1->xAxis->setRange(-55, -5);
    ui->bas1->yAxis->setRange(1000, 1200);
        ui->bas2->addGraph();
    ui->bas2->graph(0)->setScatterStyle(myScatter);
    ui->bas2->graph(0)->setLineStyle(QCPGraph::lsLine);
 ui->bas2->setBackground(QBrush(Qt::transparent));

    ui->bas2->graph(0)->setPen(pen);
 ui->bas2->xAxis->setLabel("Paket Numarası");
 ui->bas2->yAxis->setLabel("Basınç(Pa)");
 ui->bas2->xAxis->setLabelFont(plotFont);
 ui->bas2->xAxis->setTickLabelFont(plotFont);
 ui->bas2->yAxis->setLabelFont(plotFont);
 ui->bas2->yAxis->setTickLabelFont(plotFont);
 ui->bas2->xAxis->setTickLabelColor(Qt::white);
 ui->bas2->xAxis->setLabelColor(Qt::white);
 ui->bas2->xAxis->setSubTickPen(QPen(Qt::white));
 ui->bas2->xAxis->setBasePen(QPen(Qt::white));
 ui->bas2->yAxis->setTickLabelColor(Qt::white);
 ui->bas2->yAxis->setLabelColor(Qt::white);
 ui->bas2->yAxis->setSubTickPen(QPen(Qt::white));
 ui->bas2->yAxis->setBasePen(QPen(Qt::white));
 ui->bas2->xAxis->setRange(-55, -5);
 ui->bas2->yAxis->setRange(1000, 1200);

    ui->height1->addGraph();

    ui->height1->graph(0)->setScatterStyle(myScatter);
    ui->height1->graph(0)->setLineStyle(QCPGraph::lsLine);
     ui->height1->setBackground(QBrush(Qt::transparent));
      ui->height1->graph(0)->setPen(penB);
    ui->height1->xAxis->setLabel("Paket Numarası");
    ui->height1->yAxis->setLabel("Yükseklik(m)");
    ui->height1->xAxis->setLabelFont(plotFont);
    ui->height1->xAxis->setTickLabelFont(plotFont);
    ui->height1->yAxis->setLabelFont(plotFont);
    ui->height1->yAxis->setTickLabelFont(plotFont);
    ui->height1->xAxis->setTickLabelColor(Qt::white);
    ui->height1->xAxis->setLabelColor(Qt::white);
    ui->height1->xAxis->setSubTickPen(QPen(Qt::white));
    ui->height1->xAxis->setBasePen(QPen(Qt::white));
    ui->height1->yAxis->setTickLabelColor(Qt::white);
    ui->height1->yAxis->setLabelColor(Qt::white);
    ui->height1->yAxis->setSubTickPen(QPen(Qt::white));
    ui->height1->yAxis->setBasePen(QPen(Qt::white));
    ui->height1->xAxis->setRange(-55, 5);
    ui->height1->yAxis->setRange(0, 700);


   ui->height2->addGraph();
    ui->height2->graph(0)->setScatterStyle(myScatter);
    ui->height2->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->height2->graph(0)->setPen(penB);
    ui->height2->setBackground(QBrush(Qt::transparent));
    ui->height2->xAxis->setLabel("Paket Numarası");
    ui->height2->yAxis->setLabel("Yükseklik(m)");
    ui->height2->xAxis->setLabelFont(plotFont);
    ui->height2->xAxis->setTickLabelFont(plotFont);
    ui->height2->yAxis->setLabelFont(plotFont);
    ui->height2->yAxis->setTickLabelFont(plotFont);
    ui->height2->xAxis->setTickLabelColor(Qt::white);
    ui->height2->xAxis->setLabelColor(Qt::white);
    ui->height2->xAxis->setSubTickPen(QPen(Qt::white));
    ui->height2->xAxis->setBasePen(QPen(Qt::white));
    ui->height2->yAxis->setTickLabelColor(Qt::white);
    ui->height2->yAxis->setLabelColor(Qt::white);
    ui->height2->yAxis->setSubTickPen(QPen(Qt::white));
    ui->height2->yAxis->setBasePen(QPen(Qt::white));
    ui->height2->xAxis->setRange(-55, 5);
    ui->height2->yAxis->setRange(0, 700);

   ui->temp->addGraph();
    ui->temp->graph(0)->setScatterStyle(myScatter);
    ui->temp->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->temp->graph(0)->setPen(pen);
    ui->temp->setBackground(QBrush(Qt::transparent));

    ui->temp->xAxis->setLabel("Paket Numarası");
    ui->temp->yAxis->setLabel("Sıcaklık(C°)");
    ui->temp->xAxis->setLabelFont(plotFont);
    ui->temp->xAxis->setTickLabelFont(plotFont);
    ui->temp->yAxis->setLabelFont(plotFont);
    ui->temp->yAxis->setTickLabelFont(plotFont);
    ui->temp->xAxis->setTickLabelColor(Qt::white);
    ui->temp->xAxis->setLabelColor(Qt::white);
    ui->temp->xAxis->setSubTickPen(QPen(Qt::white));
    ui->temp->xAxis->setBasePen(QPen(Qt::white));
    ui->temp->yAxis->setTickLabelColor(Qt::white);
    ui->temp->yAxis->setLabelColor(Qt::white);
    ui->temp->yAxis->setSubTickPen(QPen(Qt::white));
    ui->temp->yAxis->setBasePen(QPen(Qt::white));


 ui->lspeed->addGraph();

    ui->lspeed->graph(0)->setScatterStyle(myScatter);
    ui->lspeed->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->lspeed->graph(0)->setPen(pen);
    ui->lspeed->setBackground(QBrush(Qt::transparent));
    ui->lspeed->xAxis->setLabel("Paket Numarası");
    ui->lspeed->yAxis->setLabel("İniş Hızı(m/s)");
    ui->lspeed->xAxis->setLabelFont(plotFont);
    ui->lspeed->xAxis->setTickLabelFont(plotFont);
    ui->lspeed->yAxis->setLabelFont(plotFont);
    ui->lspeed->yAxis->setTickLabelFont(plotFont);
    ui->lspeed->xAxis->setTickLabelColor(Qt::white);
    ui->lspeed->xAxis->setLabelColor(Qt::white);
    ui->lspeed->xAxis->setSubTickPen(QPen(Qt::white));
    ui->lspeed->xAxis->setBasePen(QPen(Qt::white));
    ui->lspeed->yAxis->setTickLabelColor(Qt::white);
    ui->lspeed->yAxis->setLabelColor(Qt::white);
    ui->lspeed->yAxis->setSubTickPen(QPen(Qt::white));
    ui->lspeed->yAxis->setBasePen(QPen(Qt::white));


   ui->iot->addGraph();
    ui->iot->graph(0)->setScatterStyle(myScatter);
    ui->iot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->iot->graph(0)->setPen(pen);
    ui->iot->setBackground(QBrush(Qt::transparent));
    ui->iot->xAxis->setLabel("Paket Numarası");
    ui->iot->yAxis->setLabel("Nem (g/m^3)");
    ui->iot->xAxis->setLabelFont(plotFont);
    ui->iot->xAxis->setTickLabelFont(plotFont);
    ui->iot->yAxis->setLabelFont(plotFont);
    ui->iot->yAxis->setTickLabelFont(plotFont);
    ui->iot->xAxis->setTickLabelColor(Qt::white);
    ui->iot->xAxis->setLabelColor(Qt::white);
    ui->iot->xAxis->setSubTickPen(QPen(Qt::white));
    ui->iot->xAxis->setBasePen(QPen(Qt::white));
    ui->iot->yAxis->setTickLabelColor(Qt::white);
    ui->iot->yAxis->setLabelColor(Qt::white);
    ui->iot->yAxis->setSubTickPen(QPen(Qt::white));
    ui->iot->yAxis->setBasePen(QPen(Qt::white));
    ui->iot->xAxis->setRange(30, 30);
    ui->iot->yAxis->setRange(0, 20);


    ui->Irtifa_Farki->addGraph();
    ui->Irtifa_Farki->graph(0)->setScatterStyle(myScatter);
    ui->Irtifa_Farki->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->Irtifa_Farki->graph(0)->setPen(QPen(Qt::red));
    ui->Irtifa_Farki->setBackground(QBrush(Qt::transparent));
    ui->Irtifa_Farki->xAxis->setLabel("Paket Numarası");
    ui->Irtifa_Farki->yAxis->setLabel("Fark (m)");
    ui->Irtifa_Farki->xAxis->setLabelFont(plotFont);
    ui->Irtifa_Farki->xAxis->setTickLabelFont(plotFont);
    ui->Irtifa_Farki->yAxis->setLabelFont(plotFont);
    ui->Irtifa_Farki->yAxis->setTickLabelFont(plotFont);
    ui->Irtifa_Farki->xAxis->setTickLabelColor(Qt::white);
    ui->Irtifa_Farki->xAxis->setLabelColor(Qt::white);
    ui->Irtifa_Farki->xAxis->setSubTickPen(QPen(Qt::white));
    ui->Irtifa_Farki->xAxis->setBasePen(QPen(Qt::white));
    ui->Irtifa_Farki->yAxis->setTickLabelColor(Qt::white);
    ui->Irtifa_Farki->yAxis->setLabelColor(Qt::white);
    ui->Irtifa_Farki->yAxis->setSubTickPen(QPen(Qt::white));
    ui->Irtifa_Farki->yAxis->setBasePen(QPen(Qt::white));


    ui->Voltage->addGraph();
  ui->Voltage->graph(0)->setScatterStyle(myScatter);
    ui->Voltage->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->Voltage->graph(0)->setPen(pen);
    ui->Voltage->setBackground(QBrush(Qt::transparent));
    ui->Voltage->xAxis->setLabel("Paket Numarası");
    ui->Voltage->yAxis->setLabel("Gerilim (Volt)");
    ui->Voltage->xAxis->setLabelFont(plotFont);
    ui->Voltage->xAxis->setTickLabelFont(plotFont);
    ui->Voltage->yAxis->setLabelFont(plotFont);
    ui->Voltage->yAxis->setTickLabelFont(plotFont);
    ui->Voltage->xAxis->setTickLabelColor(Qt::white);
    ui->Voltage->xAxis->setLabelColor(Qt::white);
    ui->Voltage->xAxis->setSubTickPen(QPen(Qt::white));
    ui->Voltage->xAxis->setBasePen(QPen(Qt::white));
    ui->Voltage->yAxis->setTickLabelColor(Qt::white);
    ui->Voltage->yAxis->setLabelColor(Qt::white);
    ui->Voltage->yAxis->setSubTickPen(QPen(Qt::white));
    ui->Voltage->yAxis->setBasePen(QPen(Qt::white));
    ui->Voltage->xAxis->setRange(-55, 5);
    ui->Voltage->yAxis->setRange(0, 10);

    QFile File("/Users/javid/Desktop/kayit.csv");
    File.open(QIODevice::Append | QIODevice::WriteOnly);
  QTextStream stream(&File);
    stream << "Pak No" << "," << "State" << "," << "Error Code" << "," << "Saat" << "," << "Basinc1"
           << "," << "Basinc2" << ","
           << "Yukseklik1" << "," << "Yukseklik2" << "," << "Fark" << "," << "Hiz" << ","
           << "Sicaklik" << "," << "Gerilim"
           << "," << "Enlem" << "," << "Boylam" << "," << "GPS Yukseklik" << "," << "Pitch" << ","
           << "Roll" << ","
           << "Yaw" << "," << "RHRH" << "," << "Nem" << "," << "Takim No" << "," << "\n";
    File.close();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked(bool checked)
{
    if(checked==true){
        ui->pushButton->setText("");
        timer->start(1000);
    }
    else{
        ui->pushButton->setText("");
        timer->stop();

    }
}
void MainWindow::timerFunc(){

 datas.packetnumber +=1;
readData();
 iotData();
 emit setCenter(datas.lat, datas.lon);
 emit setLocMarker(datas.lat,datas.lon);
 emit eulerFunction(datas.roll, datas.yaw, datas.pitch);
 datas.roll += 1;
 datas.yaw += 1;
 datas.pitch += 1;
//datas.lon += 0.001;
 //datas.lat += 0.001;
 //datas.height1 += 1;
 QString datetimeStr = QString::number(datas.sendtime);
 QString year = datetimeStr.mid(0, 4);
 QString month = datetimeStr.mid(4, 2);
 QString day = datetimeStr.mid(6, 2);
 QString hour = datetimeStr.mid(8, 2);
 QString minute = datetimeStr.mid(10, 2);
 QString second = datetimeStr.mid(12, 2);
 QString formattedDateTime = day + "/" + month + "/" + year + " " + hour + ":" + minute + ":"
                             + second;
 QString datab = QString::number(datas.packetnumber) + ", " + QString::number(datas.cansatstatus) + ", "
                + QString::number(datas.errorcode) + ", " + formattedDateTime + ", "
                + QString::number(datas.basinc1) + ", " + QString::number(datas.basinc2) + ", "
                + QString::number(datas.height1) + ", " + QString::number(datas.height2) + ", "
                + QString::number(datas.irtifafarki) + ", " + QString::number(datas.landingspeed) + ", "
                + QString::number(datas.temperature) + ", " + QString::number(datas.voltage) + ", "
                + QString::number(datas.lat) + ", " + QString::number(datas.lon) + ", "
                + QString::number(datas.alt) + ", " + QString::number(datas.pitch) + ","
                + QString::number(datas.roll) + ", " + QString::number(datas.yaw) + ", "
                + QString::number(datas.rhrh) + ", " + QString::number(datas.iot) + ", "
                + QString::number(datas.teamno);


 if (ui->listWidget->count() >= 26) {
     delete ui->listWidget->takeItem(0);
 }

 ui->listWidget->addItem(datab);
 ui->listWidget->scrollToBottom();


 QFile File("/Users/javid/Desktop/kayit.csv");
 File.open(QIODevice::Append |QIODevice::WriteOnly);
 QTextStream stream(&File);
     stream << datab + "\n";
     File.close();
ui->listWidget->scrollToBottom();
 ui->lcdNumber->display(datas.pitch);
 ui->lcdNumber_2->display(datas.roll);
 ui->lcdNumber_3->display(datas.yaw);
 ui->lcdNumber_4->display(datas.irtifafarki);
    int  x = datas.packetnumber;
    double y1 = datas.basinc1;
    double y2 = datas.basinc2;
    double y3 = datas.height1;
    double y4 = datas.height2;
    double y5 = datas.temperature;
    double y6 = datas.landingspeed;
    double y7 = datas.irtifafarki;
    double y8 = datas.voltage;


    ui->bas1->addGraph();
    ui->bas1->graph(0)->addData(x, y1);
    ui->bas1->xAxis->setRange(x - 55, x + 5);
    ui->bas1->yAxis->setRange(1000, 1200);
    ui->bas1->replot();
    ui->bas1->update();

    ui->bas2->addGraph();
    ui->bas2->graph(0)->addData(x, y2);
    ui->bas2->xAxis->setRange(x - 55, x + 5);
    ui->bas2->yAxis->setRange(1000, 1200);
    ui->bas2->replot();
    ui->bas2->update();

    ui->height1->addGraph();
    ui->height1->graph(0)->addData(x, y3);
    ui->height1->xAxis->setRange(x - 55, x + 5);
    ui->height1->yAxis->setRange(0, 700);
    ui->height1->replot();
    ui->height1->update();


    ui->height2->addGraph();
    ui->height2->graph(0)->addData(x, y4);
    ui->height2->xAxis->setRange(x - 55, x + 5);
    ui->height2->yAxis->setRange(0, 700);
    ui->height2->replot();
    ui->height2->update();

    ui->temp->addGraph();
    ui->temp->graph(0)->addData(x, y5);
    ui->temp->xAxis->setRange(x - 55, x + 5);
    ui->temp->yAxis->setRange(20, 40);
    ui->temp->replot();
    ui->temp->update();

    ui->lspeed->addGraph();
    ui->lspeed->graph(0)->addData(x, y6);
    ui->lspeed->xAxis->setRange(x - 55, x + 5);
    ui->lspeed->yAxis->setRange(0, 20);
    ui->lspeed->replot();
    ui->lspeed->update();

    ui->Irtifa_Farki->addGraph();
    ui->Irtifa_Farki->graph(0)->addData(x, y7);
    ui->Irtifa_Farki->xAxis->setRange(x - 55, x + 5);
    ui->Irtifa_Farki->yAxis->setRange(0, 250);
    ui->Irtifa_Farki->replot();
    ui->Irtifa_Farki->update();

    ui->Voltage->addGraph();
    ui->Voltage->graph(0)->addData(x, y8);
    ui->Voltage->xAxis->setRange(x - 55, x + 5);
    ui->Voltage->yAxis->setRange(0, 10);
    ui->Voltage->replot();
    ui->Voltage->update();

    ui->iot->addGraph();
    ui->iot->graph(0)->addData(x, y8);
    ui->iot->xAxis->setRange(x - 55, x + 5);
    ui->iot->yAxis->setRange(0, 20);
    ui->iot->replot();
    ui->iot->update();

   QPixmap dur_1("/Users/Javid/desktop/red.png");
    QPixmap dur_2("/Users/Javid/desktop/green.png");

    QMediaPlayer *sound = new QMediaPlayer;
    sound->setMedia(QUrl::fromLocalFile("/Users/Javid/desktop/alarm.mp3"));
    sound->setVolume(10);

    if((datas.errorcode & 0x01) ==1 )
    {
        ui->kod1->setPixmap(dur_2.scaled(40,40,Qt::KeepAspectRatio));
        sound->play();
    }
    else{
        ui->kod1->setPixmap(dur_1.scaled(40,40,Qt::KeepAspectRatio));
    }
    if(((datas.errorcode & 0x02)>>1) ==1){

        ui->kod2->setPixmap(dur_2.scaled(40,40,Qt::KeepAspectRatio));
        sound->play();

    }
    else{
        ui->kod2->setPixmap(dur_1.scaled(40,40,Qt::KeepAspectRatio));

    }
    if(((datas.errorcode & 0x04)>>2) ==1){

        ui->kod3->setPixmap(dur_2.scaled(40,40,Qt::KeepAspectRatio));
        sound->play();

    }
    else{
        ui->kod3->setPixmap(dur_1.scaled(40,40,Qt::KeepAspectRatio));

    }
    if(((datas.errorcode & 0x08)>>3) ==1){

        ui->kod4->setPixmap(dur_2.scaled(40,40,Qt::KeepAspectRatio));
        sound->play();

    }
    else{
        ui->kod4->setPixmap(dur_1.scaled(40,40,Qt::KeepAspectRatio));

    }
    if(((datas.errorcode & 0x10)>>4) ==1){

        ui->kod5->setPixmap(dur_2.scaled(40,40,Qt::KeepAspectRatio));
        sound->play();

    }
    else{
        ui->kod5->setPixmap(dur_1.scaled(40,40,Qt::KeepAspectRatio));
    }

    if(datas.errorcode==0){
        sound->play();
    }

}

void MainWindow::on_rgb_clicked()
{
    datas.rhrh = 0;

    QString rhrh_string = ui->lineEdit->text();

    if (rhrh_string.at(1) == 'G') {
        datas.rhrh = datas.rhrh | 0x01;
    }

    if (rhrh_string.at(1) == 'B') {
        datas.rhrh = datas.rhrh | 0x02;
    }

    if (rhrh_string.at(1) == 'R') {
        datas.rhrh = datas.rhrh | 0x04;
    }
    if (rhrh_string.at(3) == 'G') {
        datas.rhrh = datas.rhrh | (0x01 << 8);
    }
    if (rhrh_string.at(3) == 'B') {
        datas.rhrh = datas.rhrh | (0x02 << 8);
    }
    if (rhrh_string.at(3) == 'R') {
        datas.rhrh = datas.rhrh | (0x04 << 8);
    }

    QChar sec_f = rhrh_string.at(0);
    QChar sec_s = rhrh_string.at(2);

    datas.rhrh = datas.rhrh | (sec_f.digitValue() << 3) | (sec_s.digitValue() << 11);

    rgbData();
}
void MainWindow::on_ayril_clicked()
{
    writeData();
}


