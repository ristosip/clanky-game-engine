#ifndef DEMOGAMEVIEW_H
#define DEMOGAMEVIEW_H

#include "gameview.h"

class DemoGameView : public GameView
{
    Q_OBJECT
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // DEMOGAMEVIEW_H
