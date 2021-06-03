#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
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
}

//char winID[32] = {0};
void* winID;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //sprintf(winID, "SDL_WINDOWID=0x%lx", ui->videoShowWindow->winId());
    winID = (void*)ui->videoShowWindow->winId();
    cur_file = QString("1.mp4");
//    ffplay = new Ffplay("ffplay");
//    if (ffplay) {
//        connect(ffplay, &Ffplay::sendPicture, this, &MainWindow::recvPicture);
//        connect(ffplay, &Ffplay::sendVoice, this, &MainWindow::recvVoice);
//    }

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_playBtn_clicked()
{
    int ret;
    event_loop_flag = 1;
    ret = ffplay(cur_file.toStdString().c_str());
    qDebug() << "The ret of the function ffplay is:" << ret;
    status_message = QString("Started showing.");
    ui->statusbar->clearMessage();
    ui->statusbar->showMessage(status_message);
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
    event_loop_flag = 0;
    if (!is_event_loop_running);
        ret = do_exit(is);
    qDebug() << ret;
    status_message = QString("Stopped showing.");
    ui->statusbar->clearMessage();
    ui->statusbar->showMessage(status_message);
}

void MainWindow::on_openAction_triggered()
{
    cur_file = QFileDialog::getOpenFileName(this);
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

}
