#include "demogameview.h"
#include <QKeyEvent>
#include <QApplication>
#include "demogame.h"

void DemoGameView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        showNormal();
        resize(800, 600);
        qApp->setOverrideCursor(QCursor(Qt::ArrowCursor));
    }
    if(event->key() == Qt::Key_Q){
        close();
    }

    GameView::keyPressEvent(event);
}

void DemoGameView::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_R){
        if(!m_gamescene_list.empty() && m_current_scene_index >= 0 && m_current_scene_index < m_gamescene_list.length()){
            deleteGameScene(m_current_scene_index);
            addGameScene(new DemoGame());
            startGame();
        }
    }

    GameView::keyReleaseEvent(event);
}
