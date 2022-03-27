#ifndef VIDEO_INFO_FETCHER_H
#define VIDEO_INFO_FETCHER_H

#include <QImage>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QVideoFrame>
#include <QtMultimedia/QVideoSink>

QMediaPlayer* sharedPlayer()
{    //返回共享播放器
    static auto g_player = new QMediaPlayer();
    return g_player;
}

bool fetchFirstFrameFromVideoUrl(const QString& url, std::function<void(const QImage&)> callback)
{
    qDebug() << "fetchFirstFrameFromVideoUrl";
    //sharedPlayer()->setSource(QUrl(url));
    //auto sink = new QVideoSink; //内存泄露？
    //sharedPlayer()->setVideoSink(sink);

    QMediaPlayer* player=new QMediaPlayer();
    player->setSource(QUrl(url));
    auto sink = new QVideoSink;
    player->setVideoSink(sink);

    QObject::connect(
        sink, &QVideoSink::videoFrameChanged,
        player,    // sharedPlayer(),
        [=](const QVideoFrame& frame) {
            static int hitTimes = 0;
            if (frame.isValid() && hitTimes++ % 3 == 0) {
                //sharedPlayer()->stop();
                player->stop();
                callback(frame.toImage());
            }
            player->deleteLater();
        },
        Qt::QueuedConnection);
    //sharedPlayer()->play();
    player->play();
    return true;
}

#endif    // VIDEO_INFO_FETCHER_H
