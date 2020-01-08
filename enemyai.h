#ifndef ENEMYAI_H
#define ENEMYAI_H

#include "objectfeature.h"

class EnemyAI : public ObjectFeature
{
    Q_OBJECT
public:
    EnemyAI(QObject *parent = nullptr);
    void update() override;
};

#endif // ENEMYAI_H
