#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include "gameobject.h"
#include "gameobjectfactory.h"
#include "soundmanager.h"
#include <QUuid>
#include <chrono>

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GameScene(QObject* parent = nullptr);
    ~GameScene();

    const QList<GameObject*>* trackedObjects();
    void startGameScene();
    void pauseGameScene();
    void continueGameScene();
    void stopGameScene();
    bool isPaused();
    bool isRunning();
    void addBackgroundImageLayer(QGraphicsItem *background_item, qreal parallax_factor = 1.0, qreal initial_pos_x = 0.0, qreal initial_pos_y = 0.0);
    void setActiveAreaMargins(qreal margin_size);

public slots:
    void addGameObject(GameObject *gameobject, bool tracked = false);
    void removeGameObject(GameObject *gameobject);
    void updateBackgroundParallax(qreal viewpoint_shift_dx, qreal viewpoint_shift_dy);
    void updateActiveGameArea(QRectF visible_area);

signals:
    void command(int movement, int actions, int modifiers);
    void gameCycleFinished();
    void trackedObjectChanged();
    void started();
    void stopped();
    void paused();

protected:
    virtual void createGame();
    GameObjectFactory *m_factory;
    SoundManager *m_sound_manager;
    QUuid m_uuid;

private:
    QList<GameObject*> m_gameobject_list;
    QList<GameObject*> m_object_to_delete;
    QList<GameObject*> m_objects_to_add;
    QList<GameObject*> m_tracked_objects;
    QList<GameObject*> m_tracked_objects_to_add;
    QList<QGraphicsItem*> m_background_image_layers;
    QList<qreal> m_background_layer_parallax_factors;
    int m_phase;
    bool m_objectlist_changed;
    bool m_paused;
    bool m_running;
    QTimer *m_timer;
    std::chrono::steady_clock::time_point m_previous_time_point;
    long long m_delta_time;
    bool m_first_cycle;
    QRectF m_active_area;
    qreal m_active_area_margins;

    void makeObjectListChanges();
    void advanceGame();
};

#endif // GAMESCENE_H
