#ifndef DEMOGAME_H
#define DEMOGAME_H

#include "gamescene.h"
#include "gameview.h"
#include "gameobject.h"

class DemoGame : public GameScene
{
    Q_OBJECT
public:
    void createGame() override;
private slots:
    void winEvent();
    void loseEvent();
private:
    GameObject *m_player;
};

#endif // DEMOGAME_H
