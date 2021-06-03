#ifndef QFFPLAYER_H
#define QFFPLAYER_H

#include <QObject>

class QFfplayer : public QObject
{
    Q_OBJECT
public:
    explicit QFfplayer(QObject *parent = nullptr);

signals:

public slots:
};

#endif // QFFPLAYER_H