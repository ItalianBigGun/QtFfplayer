#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
//#include <ffplay.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_playBtn_clicked();
    void recvPicture(const char *pData, size_t s);
    void recvVoice(const char *pData, size_t s);

    void on_stopBtn_clicked();

    void on_openAction_triggered();
    void on_horizontalSlider_valueChanged(int value);

    void on_pauseBtn_clicked();

private:
    void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindow *ui;
    QThread *ffplay_thd;
    QString cur_file;
    QString status_message;
    //Ffplay *ffplay;
};
#endif // MAINWINDOW_H
