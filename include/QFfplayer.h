#ifndef QFFPLAYER_H
#define QFFPLAYER_H

#include <QObject>
extern "C" {
    #include "ffplay.h"
    #include <libavdevice/avdevice.h>
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavformat/avio.h>
    
    extern VideoState *is;
    extern SDL_Event event;
    extern int event_loop_flag;
    extern int is_event_loop_running;
    extern int seek_by_bytes;
    extern int show_loop;

    /*  播放开始    */
    extern int ffplay(const char *argv);
    /*  需要重新修改函数体修改    */
    extern void video_image_display(VideoState *is);
    extern void video_audio_display(VideoState *s);

    /*  退出播放线程    */
    extern int do_exit(VideoState *is);
    /*  调整播放框大小  */
    extern void fill_rectangle(int x, int y, int w, int h);
    /*  获取播放时间？  */
    extern double get_clock(Clock *c);
    /*  设置播放时间？  */
    extern void set_clock(Clock *c, double pts, int serial);
    /*  设置播放速度    */
    extern void set_clock_speed(Clock *c, double speed);

    extern double get_master_clock(VideoState *is);
    /*  快进操作    */
    extern void stream_seek(VideoState *is, int64_t pos, int64_t rel, int seek_by_bytes);
    extern void seek_chapter(VideoState *is, int incr);
    
    /*  视频启停操作    */
    extern void toggle_pause(VideoState *is);
    /*  静音启停操作    */
    extern void toggle_mute(VideoState *is);
    /*  调节音量    */
    extern void update_volume(VideoState *is, int sign, double step);
    /*  ?   */
    extern double get_rotation(AVStream *st);
    /*  音频启停操作    */
    extern void toggle_audio_display(VideoState *is);
    /*  改变显示操作    */
    extern void change_show_mode(VideoState *is);
}

class QFfplayer : public QObject
{
    Q_OBJECT
public:
    explicit QFfplayer(QObject *parent = nullptr);
    void setVideoState(VideoState *is);
signals:
    void sendPicture(const char *pData, size_t s);
    void sendVoice(const char *pData, size_t s);
public slots:

private:
    VideoState **m_is;

};

#endif // QFFPLAYER_H