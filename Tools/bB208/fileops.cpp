#include "bb208.h"
#include "ui_bb208.h"

#include <QMidiOut.h>
#include <QMidiIn.h>
#include <QMidiFile.h>
#include <QDebug>
#include <QFile>
#include <QFontDatabase>
#include <QFileDialog>

extern  QMidiOut MidiOut;

QByteArray blob;
void bB208::on_open_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,  tr("Open sequence"), "/Devel/drumkit", tr("seq Files (*seq.txt)"));
    ui->lineEdit_wavfile->setText(fileName);
}


void bB208::on_send_pushButton_clicked()
{
    int i,index=0;
    QFile file(ui->lineEdit_wavfile->text());
    if (!file.open(QIODevice::ReadOnly))
    {
        return ;
    }
    blob = file.readAll();
    int len = file.size();
    file.close();
    ui->statusbar->showMessage("bB208 : Sending");

    QByteArray sdata; // = QByteArray::fromHex("F0305555F7");
    sdata.resize(36);
    sdata[0] = 0xf0;
    sdata[1] = 0x01;
    sdata[2] = 0x00;
    sdata[35] = 0xf7;

    while(index < len-32)
    {
        for(i=3;i<35;i++)
            sdata[i] = blob[index+i-3];
        MidiOut.sendSysEx(sdata);
        index += 32;
        sdata[1] = 0x02;
        qSleep(10);
    }

    sdata[1] = 0x03;
    for(i=3;i<len-index+3;i++)
        sdata[i] = blob[index+i-3];
    MidiOut.sendSysEx(sdata);
    ui->statusbar->showMessage("bB208 : Done");
}

