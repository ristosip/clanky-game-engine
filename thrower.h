#ifndef THROWER_H
#define THROWER_H

#include "objectfeature.h"
#include "gameobjectfactory.h"

class Thrower : public ObjectFeature
{
    Q_OBJECT
public:
    Thrower(int throwable_object_type, int throwable_object_count, GameObjectFactory *factory, QObject *parent = nullptr);
    void update() override;
    void setThrowableObjectType(int object_type);

private:
    int m_throwable_object_type;
    int m_throwable_object_count;
    GameObjectFactory *m_factory;
    int m_last_known_move_command;
    qreal m_power;
    bool m_launched;
    bool m_loaded;
};

#endif // THROWER_H
