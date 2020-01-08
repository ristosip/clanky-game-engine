#ifndef DRONEPILOTING_H
#define DRONEPILOTING_H

#include "objectfeature.h"
#include "gameobjectfactory.h"
#include "controller.h"

class RemoteControlling : public ObjectFeature
{
    Q_OBJECT
public:
    RemoteControlling(GameObjectFactory *factory, QObject *parent = nullptr);
    void update() override;
public slots:
    void stopControlling();
signals:
    void sendCommand(int movement, int actions, int modifiers);

private:
    GameObjectFactory *m_factory;
    int m_device_type;
    bool m_piloting;
    bool m_drone_set_up;
};

#endif // DRONEPILOTING_H
