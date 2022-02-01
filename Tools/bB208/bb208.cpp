#include "bb208.h"
#include "ui_bb208.h"

#include <QMidiOut.h>
#include <QMidiIn.h>
#include <QMidiFile.h>
#include <QDebug>
#include <QFile>
#include <QFontDatabase>
#include <QFileDialog>

QMidiOut    MidiOut;
QMidiIn     MidiIn;
QMidiFile   MidiFile;
QString     bB101_ID;
QByteArray  sysex_msg;

bB208::bB208(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::bB208)
{
    ui->setupUi(this);
    QMap<QString, QString> out_vals = QMidiOut::devices();
    ui->statusbar->showMessage("Looking for bB208 ...");
    for (QString key : out_vals.keys()) {
        QString value = out_vals.value(key);
        qDebug() << "bB208_Midi\nOut Devices :";
        qDebug() << "Key : "  << key.toUtf8().constData() << "  \tID : " << value.toUtf8().constData();
        if ( value == "bB208")
        {
            bB101_ID = value;
            MidiOut.connect(key);
            MidiIn.connect(key);
            MidiIn.start();
            qDebug() << "bB208 connected to out";
            ui->statusbar->showMessage("bB208 : Connected");
        }
    }
}

void bB208:: qSleep(int ms)
{
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}

bB208::~bB208()
{
    delete ui;
}


