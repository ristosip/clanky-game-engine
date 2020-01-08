#ifndef DRONESOUNDS_H
#define DRONESOUNDS_H

#include "objectfeature.h"
#include "soundmanager.h"
#include <QUuid>

class DroneSounds : public ObjectFeature
{
    Q_OBJECT
public:
    DroneSounds(const QString &fileFolder, SoundManager &sound_manager, QObject *parent = nullptr);
    ~DroneSounds() override;
    void update() override;
private:
    QUuid m_uuid;
    SoundManager *m_sound_manager;
    bool m_registered;
    int m_previous_command;
    int m_previous_state;
    bool m_time_switch_done;
    int m_elapsed_time;
};

#endif // DRONESOUNDS_H
