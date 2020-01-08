#ifndef HUMANOIDSOUNDS_H
#define HUMANOIDSOUNDS_H

#include "objectfeature.h"
#include "soundmanager.h"
#include <QUuid>

class HumanoidSounds : public ObjectFeature
{
    Q_OBJECT
public:
    HumanoidSounds(const QString &fileFolder, SoundManager &sound_manager, QObject *parent = nullptr);
    ~HumanoidSounds() override;
    void update() override;
private:
    QUuid m_uuid;
    SoundManager *m_sound_manager;
    bool m_registered;
    int m_previous_state;
};

#endif // HUMANOIDSOUNDS_H
