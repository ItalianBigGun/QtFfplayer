#include "qffplayer.h"
#include "ffplay.h"
#include <QDebug>
void* winID;
QFfplayer::QFfplayer(QObject *parent) : QObject(parent)
{
    m_is_dp = &is;
    show_file_name = QString("");
    winID = NULL;
    pause_state = 0;
    current_state = STATE_STOP;
    m_play_thd = new PlayerThread();
    connect(m_play_thd, &PlayerThread::playStop, this, &QFfplayer::updateState);
    connect(m_play_thd, &PlayerThread::playStart, this, &QFfplayer::updateState);
}

QFfplayer::~QFfplayer()
{
    ::event_loop_flag = 0;
    if (!::is_event_loop_running && (*m_is_dp != NULL))
        do_exit(is);
}

void QFfplayer::setVideoState(VideoState *is)
{
    m_is_dp = &is;
    m_is = is;
}

void QFfplayer::setWinID(void *winID)
{
    ::winID = winID;
}

void QFfplayer::setDisplayFile(QString filename)
{
    show_file_name = filename;
}

extern "C" {
    extern Frame *frame_queue_peek_last(FrameQueue *f);
    extern int frame_queue_nb_remaining(FrameQueue *f);
    extern Frame *frame_queue_peek(FrameQueue *f);
}
const QImage& QFfplayer::getCurrentImage()
{
    Frame *vp;
    Frame *sp = NULL;

    vp = frame_queue_peek_last(&is->pictq);
    if (vp) {
        SwsContext* img_convert_ctx;

            img_convert_ctx = sws_getContext(is->viddec.avctx->width,
                                             is->viddec.avctx->height,
                                             is->viddec.avctx->pix_fmt,
                                             is->viddec.avctx->width,
                                             is->viddec.avctx->height,
                                             AV_PIX_FMT_RGB24,
                                             SWS_BICUBIC, NULL, NULL, NULL);

            AVFrame* frameRGB = av_frame_alloc();
            avpicture_alloc((AVPicture*)frameRGB,
                            AV_PIX_FMT_RGB24,
                            is->viddec.avctx->width,
                            is->viddec.avctx->height);

            sws_scale(img_convert_ctx,
                      vp->frame->data,
                      vp->frame->linesize, 0,
                      is->viddec.avctx->height,
                      frameRGB->data,
                      frameRGB->linesize);

            QImage image(frameRGB->data[0],
                         is->viddec.avctx->width,
                         is->viddec.avctx->height,
                         frameRGB->linesize[0],
                         QImage::Format_RGB888);
            current_image = image;
    }
    return current_image;
}

/*  slots   */
int QFfplayer::playVideo(QString filename)
{
    int ret;
    event_loop_flag = 1;
    if ((filename == nullptr) || (current_state != STATE_STOP)) {
        qDebug() << current_state;
        return -1;
    }current_state = STATE_RUN;
    show_file_name = filename;
    m_play_thd->setDisplayFile(show_file_name);
    m_play_thd->start();
    //ret = ::ffplay(show_file_name.toStdString().c_str());
    //qDebug() << "after ffplayer:" <<ret;
    return ret;
}

int QFfplayer::stopVideo()
{
    int ret = -1;
    if (*m_is_dp && (current_state != STATE_STOP)) {
        event_loop_flag = 0;
        while (is_event_loop_running) {

        }
        ret = do_exit(*m_is_dp);
        if (ret != -1)
            current_state = STATE_STOP;
    }
    qDebug()<< "after stop video:" << current_state << is_event_loop_running;
    m_play_thd->terminate();
    return ret;
}

int QFfplayer::pauseVideo()
{
    if (*m_is_dp && (current_state != STATE_STOP)/* && pause_state*/) {
        return pause_state = toggle_pause(*m_is_dp);
    }
    return -1;
}

int QFfplayer::resumeVideo()
{
    if (is && (current_state != STATE_STOP)) {
        return pause_state = toggle_pause(m_is);
    }
    return -1;
}

int QFfplayer::updateVolume(int sign, double step)
{
    if (*m_is_dp && (current_state != STATE_STOP)) {
        return update_volume(*m_is_dp, sign, step);
    }
    return -1;
}

int QFfplayer::muteAudio()
{
    if (*m_is_dp && (current_state != STATE_STOP)) {
        return ::toggle_mute(*m_is_dp);
    }
    return -1;
}

int QFfplayer::changeShowMode()
{
    if (*m_is_dp && (current_state != STATE_STOP)) {
        return change_show_mode(*m_is_dp);
    }
    return -1;
}

double QFfplayer::changeShowSpeed(double speed)
{
    if (*m_is_dp && (current_state != STATE_STOP)) {
        return ::set_clock_speed(&(*m_is_dp)->extclk, speed);
    }
    return -1;
}


