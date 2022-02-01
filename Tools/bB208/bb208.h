#ifndef BB208_H
#define BB208_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class bB208; }
QT_END_NAMESPACE

class bB208 : public QMainWindow
{
    Q_OBJECT

public:
    bB208(QWidget *parent = nullptr);
    ~bB208();
    void qSleep(int ms);

private slots:
    void on_open_pushButton_clicked();

    void on_send_pushButton_clicked();

private:
    Ui::bB208 *ui;
};
#endif // BB208_H
