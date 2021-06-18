#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#if 0
extern "C" {
#include "ffplay.h"
#include <libavdevice/avdevice.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
extern int ffplay(const char *argv);
extern VideoState *is;
extern int do_exit(VideoState *is);
extern SDL_Event event;
extern int event_loop_flag;
extern int is_event_loop_running;
extern int seek_by_bytes;
}

//char winID[32] = {0};
void* winID;
#endif
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_ffplayer = new QFfplayer(this);
    m_ffplayer->setWinID((void*)ui->videoShowWindow->winId());
    cur_file = QString("1.mp4");
}

MainWindow::~MainWindow()
{
    delete m_ffplayer;
    delete ui;
}


void MainWindow::on_playBtn_clicked()
{
    int ret;
    event_loop_flag = 1;
    ret = m_ffplayer->playVideo(cur_file.toStdString().c_str());//ffplay(cur_file.toStdString().c_str());
    qDebug() << "The ret of the function ffplay is:" << ret;
    if (ret == 0) {
        status_message = QString("Started showing.");
        ui->timeHSlider->setRange(0, 320);
        ui->statusbar->clearMessage();
        ui->statusbar->showMessage(status_message);
    }
}

void MainWindow::recvPicture(const char *pData, size_t s)
{
//    QPixmap *m_pixmap;
//    m_pixmap = new QPixmap(pData);
//    ui->videoShowWindow->setPixmap(*m_pixmap);
}

void MainWindow::recvVoice(const char *pData, size_t s)
{

}

void MainWindow::on_stopBtn_clicked()
{
    //ui->videoShowWindow->close();
    int ret;
    ret = m_ffplayer->stopVideo();
    qDebug() << ret;
    status_message = QString("Stopped showing.");
    ui->statusbar->clearMessage();
    ui->statusbar->showMessage(status_message);
}

void MainWindow::on_openAction_triggered()
{
    cur_file = QFileDialog::getOpenFileName(this);
    m_ffplayer->setDisplayFile(cur_file);
    qDebug() << cur_file;
    //
    status_message = QString("Set current show file name is:.") + cur_file;
    ui->statusbar->clearMessage();
    ui->statusbar->showMessage(status_message);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    on_stopBtn_clicked();
}

//  To set the volume of video.
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    int old_volume = is->audio_volume;
    int new_volume = is->audio_volume + value;
    if (new_volume <= 0)  {
        is->audio_volume = 0;
    } else if (new_volume > 128) {
        is->audio_volume = 128;
    } else {
        is->audio_volume = new_volume;
    }
}

void MainWindow::on_pauseBtn_clicked()
{
    int ret;
    ret = m_ffplayer->pauseVideo();
    qDebug() <<ret;
}

void MainWindow::on_actionmute_triggered()
{
    if (is) {
        m_ffplayer->muteAudio();
    }
}

void MainWindow::on_actiongetImage_triggered()
{
    m_ffplayer->getCurrentImage().save("1.jpg");
}

void MainWindow::on_actionloadsubtfile_triggered()
{

}


void MainWindow::on_timeHSlider_valueChanged(int value)
{

}

void MainWindow::on_actionaddRate_triggered()
{
    m_ffplayer->updateSpeed(0.1, 1);
    on_pauseBtn_clicked();on_pauseBtn_clicked();
}

void MainWindow::on_actionreduceRate_triggered()
{
    m_ffplayer->updateSpeed(-0.1, 1);
    on_pauseBtn_clicked();on_pauseBtn_clicked();
}

void MainWindow::on_timeHSlider_sliderMoved(int position)
{
    static int o_postion = 0;
    qDebug() << o_postion << "," << position;
    int diff = position - o_postion;
    o_postion = position;
    m_ffplayer->stream_seek_safe(diff, 0);
}
