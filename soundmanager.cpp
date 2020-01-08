#include "soundmanager.h"
#include <QSoundEffect>
#include <QUuid>
#include <QPropertyAnimation>

SoundWorker::SoundWorker() : QObject()
{ 
    m_soundtrack_player = nullptr;
}

SoundWorker::~SoundWorker()
{
    while(!m_soundeffect_map.isEmpty()){
        QMap<QString, QSoundEffect*> *map = m_soundeffect_map.take(m_soundeffect_map.firstKey());
        while(!map->isEmpty()){
            QSoundEffect *effect = map->take(map->firstKey());
            delete effect;           
        }
        delete map;
    }
    delete m_soundtrack_player;
}

void SoundWorker::handleMediaPlayerStatusChange(bool playing)
{
    if(!playing){
        emit trackEnded();
    }
}

void SoundWorker::handleSoundEffectEnding(QUuid user_id, QSoundEffect *effect)
{
    if(m_currently_playing_effects.count(user_id) == 1)
        m_currently_playing_effects.remove(user_id);
    else if(m_currently_playing_effects.count(user_id) > 1){
        QList<QSoundEffect*> temp_list = m_currently_playing_effects.values(user_id);
        m_currently_playing_effects.remove(user_id);
        for(int i = 0; i < temp_list.length(); i++){
            QSoundEffect *temp = temp_list.value(i);
            if(temp == effect){
                temp_list.removeAt(i);
                break;
            }
        }      
        for(int i = temp_list.length() - 1; i > -1; i--){
            m_currently_playing_effects.insertMulti(user_id, temp_list.value(i));
        }
    }  
}

bool SoundWorker::registerSource(QUuid user_id, QString audio_folder)
{
    if(!user_id.isNull() && !(audio_folder.isNull() || audio_folder.isEmpty())){
        QMap<QString, QSoundEffect*> *map = new QMap<QString, QSoundEffect*>();
        m_audio_folders.insert(user_id, audio_folder);
        m_soundeffect_map.insert(user_id, map);
        return true;
    }
    return false;
}

void SoundWorker::unRegisterSource(QUuid user_id)
{
    QMap<QString, QSoundEffect*> *map = m_soundeffect_map.take(user_id);
    while(!map->isEmpty()){
        QSoundEffect *effect = map->take(map->firstKey());
        delete effect;
    }
    delete map;
}

void SoundWorker::playEffect(QUuid user_id, QString sound_name, qreal volume, int loops)
{
    QMap<QString, QSoundEffect*> *map = m_soundeffect_map.value(user_id);

    if(map == nullptr){
        map = new QMap<QString, QSoundEffect*>();
    }

    if(map->isEmpty()){
        m_soundeffect_map.insert(user_id, map);
    }

    if(map->value(sound_name) == nullptr || map->value(sound_name)->source().isEmpty()){
        QString file_path = m_audio_folders.value(user_id) + sound_name + ".wav";
        QSoundEffect *effect = new QSoundEffect();
        effect->setSource(QUrl("qrc" + file_path));
        map->insert(sound_name, effect);
        connect(effect, &QSoundEffect::playingChanged, this, [=](){
            if(!effect->isPlaying())
                this->handleSoundEffectEnding(user_id, effect);
        });
    }

    QSoundEffect *effect = map->value(sound_name);
    effect->setVolume(volume);
    if(loops < 0)
        loops = QSoundEffect::Infinite;
    effect->setLoopCount(loops);
    // allows only one sound effect from the same source at a time
    // starts a crossfade from the previous effect to the next
    QList<QSoundEffect*> previous_effects = m_currently_playing_effects.values(user_id);
    if(!previous_effects.isEmpty() && previous_effects.first() != effect){
        QPropertyAnimation *fade_out = new QPropertyAnimation(previous_effects.first(), "volume");
        fade_out->setStartValue(previous_effects.first()->volume());
        fade_out->setEndValue(0.0);
        fade_out->setDuration(5);
        fade_out->start(QAbstractAnimation::DeleteWhenStopped);
        connect(fade_out, &QPropertyAnimation::finished, previous_effects.first(), [=](){
            previous_effects.first()->stop();
        });
        if(!previous_effects.contains(effect)){
            m_currently_playing_effects.insertMulti(user_id, effect);
        }
    }
    else if(previous_effects.isEmpty()){
        m_currently_playing_effects.insertMulti(user_id, effect);
    }
    effect->play();
}

void SoundWorker::playSoundtrack(QUuid user_id, QString track_name, qreal volume, int loops)
{
    if(m_soundtrack_player == nullptr){
        m_soundtrack_player = new QSoundEffect();
    }

    if(m_soundtrack_player->source().isEmpty()){
        connect(m_soundtrack_player, &QSoundEffect::playingChanged, this, [=](){
            this->handleMediaPlayerStatusChange(this->m_soundtrack_player->isPlaying());
        });
    }

    m_soundtrack_player->stop();
    QString file_path = m_audio_folders.value(user_id) + track_name + ".wav";
    m_soundtrack_player->setSource(QUrl("qrc" + file_path));
    m_soundtrack_player->setVolume(volume);
    if(loops < 0)
        loops = QSoundEffect::Infinite;
    m_soundtrack_player->setLoopCount(loops);
    m_soundtrack_player->play();
}

SoundManager::SoundManager() : QObject(), GameObject(nullptr)
{
    SoundWorker *sound_worker = new SoundWorker;
    sound_worker->moveToThread(&m_worker_thread);
    connect(&m_worker_thread, &QThread::finished, sound_worker, &QObject::deleteLater);
    connect(this, &SoundManager::registerationRequested, sound_worker, &SoundWorker::registerSource, Qt::QueuedConnection);
    connect(this, &SoundManager::unRegisterationRequested, sound_worker, &SoundWorker::unRegisterSource, Qt::QueuedConnection);
    connect(this, &SoundManager::soundEffectRequested, sound_worker, &SoundWorker::playEffect, Qt::QueuedConnection);
    connect(this, &SoundManager::soundtrackRequested, sound_worker, &SoundWorker::playSoundtrack, Qt::QueuedConnection);
    connect(sound_worker, &SoundWorker::trackEnded, this, &SoundManager::trackEnded, Qt::QueuedConnection);
    m_worker_thread.start();
}

SoundManager::~SoundManager()
{
    m_worker_thread.quit();
    m_worker_thread.wait();
}

bool SoundManager::registerSource(QUuid user_id, QString audio_folder)
{
    emit registerationRequested(user_id, audio_folder);
    if(!user_id.isNull() && !(audio_folder.isNull() || audio_folder.isEmpty())){
        return true;
    }
    return false;
}
void SoundManager::unRegisterSource(QUuid user_id)
{
    emit unRegisterationRequested(user_id);
}
void SoundManager::playEffect(QUuid user_id, QString sound_name, qreal volume, int loops)
{
    emit soundEffectRequested(user_id, sound_name, volume, loops);
}
void SoundManager::playSoundtrack(QUuid user_id, QString track_name, qreal volume, int loops)
{
    emit soundtrackRequested(user_id, track_name, volume, loops);
}
