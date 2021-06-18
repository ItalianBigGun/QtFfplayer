#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
//#include <ffplay.h>
#include "QFfplayer.h"
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

    void on_actionmute_triggered();

    void on_actiongetImage_triggered();

    void on_actionloadsubtfile_triggered();

    void on_timeHSlider_valueChanged(int value);

    void on_actionaddRate_triggered();

    void on_actionreduceRate_triggered();

    void on_timeHSlider_sliderMoved(int position);

private:
    void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindow *ui;
    QThread *ffplay_thd;
    QString cur_file;
    QString status_message;
    QFfplayer *m_ffplayer;
};
#endif // MAINWINDOW_H
