#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>
#include <QTimer>
#include <QMap>
#include "gameobject.h"
#include "gamescene.h"

class GameView : public QGraphicsView
{
    Q_OBJECT
public:
    GameView(QGraphicsView *parent = nullptr);
    ~GameView();

    void addGameScene(GameScene *scene);
    void startGame();

signals:
    void command(int movement, int actions, int modifiers);
    void viewpointChanged(qreal dx, qreal dy);
    void visibleGameAreaChanged(QRectF visible_area);

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

    void switchScenes(int scene_index);
    bool deleteGameScene(int scene_index);

    QMap<int, bool> m_keys;
    QList<GameScene*> m_gamescene_list;
    int m_current_scene_index;
    const GameObject* m_tracked_object;
    QPointF m_camera_position;

private:
    void adjustView();
    void handleUserInput();

private slots:
    void resetViewpoint();
    void advanceView();
    void updateTrackedObject();
};

#endif // GAMEVIEW_H
