#ifndef EXPLOSIVE_H
#define EXPLOSIVE_H

#include "objectfeature.h"
#include "gameobjectfactory.h"
#include "soundmanager.h"

class Explosive : public ObjectFeature
{
    Q_OBJECT
public:
    Explosive(const QString &shrapnel_graphics_path, GameObjectFactory *factory, SoundManager *soundmanager = nullptr, qreal image_scale = 1, QObject *parent = nullptr);
    void update() override;
public slots:
    void trigger();
private:
    QString m_shrapnel_graphics_path;
    GameObjectFactory *m_factory;
    bool m_triggered;
    qreal m_image_scale;
    SoundManager *m_soundmanager;
};

#endif // EXPLOSIVE_H
