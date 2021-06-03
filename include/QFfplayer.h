#ifndef QFFPLAYER_H
#define QFFPLAYER_H

#include <QObject>

class QFfplayer : public QObject
{
    Q_OBJECT
public:
    explicit QFfplayer(QObject *parent = nullptr);

signals:
    void sendPicture(const char *pData, size_t s);
    void sendVoice(const char *pData, size_t s);
public slots:

};

#endif // QFFPLAYER_H