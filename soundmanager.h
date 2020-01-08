#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include "gameobject.h"
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QUuid>
#include <QThread>

class SoundWorker : public QObject
{
    Q_OBJECT
public:
    SoundWorker();
    ~SoundWorker();
public slots:
    bool registerSource(QUuid user_id, QString audio_folder);
    void unRegisterSource(QUuid user_id);
    void playEffect(QUuid user_id, QString sound_name, qreal volume = 1.0, int loops = 1);
    void playSoundtrack(QUuid user_id, QString track_name, qreal volume = 1.0, int loops = 1);

signals:
    void trackEnded();

private slots:
    void handleMediaPlayerStatusChange(bool playing);
    void handleSoundEffectEnding(QUuid user_id, QSoundEffect* effect);

private:
    QMap<QUuid, QSoundEffect*> m_currently_playing_effects;
    QMap<QUuid, QMap<QString, QSoundEffect*>*> m_soundeffect_map;
    QMap<QUuid, QString> m_audio_folders;
    QSoundEffect *m_soundtrack_player;
};

class SoundManager : public QObject, public GameObject
{
    Q_OBJECT
public:
    SoundManager();
    ~SoundManager();
    bool registerSource(QUuid user_id, QString audio_folder);
    void unRegisterSource(QUuid user_id);
    void playEffect(QUuid user_id, QString sound_name, qreal volume = 1.0, int loops = 1);
    void playSoundtrack(QUuid user_id, QString track_name, qreal volume = 1.0, int loops = 1);

signals:
    void trackEnded();
    void registerationRequested(QUuid user_id, QString audio_folder);
    void unRegisterationRequested(QUuid user_id);
    void soundEffectRequested(QUuid user_id, QString sound_name, qreal volume, int loops);
    void soundtrackRequested(QUuid user_id, QString track_name, qreal volume, int loops);

private:
    QThread m_worker_thread;
};

#endif // SOUNDMANAGER_H
