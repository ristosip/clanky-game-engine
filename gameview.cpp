#include "gameview.h"
#include <QGraphicsScene>
#include "gameobject.h"
#include "controller.h"
#include "objectfeature.h"
#include "gamescene.h"
#include <QTimer>
#include <QKeyEvent>
#include <QScrollBar>

GameView::GameView(QGraphicsView *parent) : QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
    m_current_scene_index = 0;
    this->setFocus();
}

GameView::~GameView()
{
    //destroy scenes
    while (!m_gamescene_list.isEmpty())
        delete m_gamescene_list.takeFirst();
}

void GameView::addGameScene(GameScene *scene)
{
    if(scene != nullptr)
        m_gamescene_list.append(scene);
}

void GameView::startGame()
{
    if(!m_gamescene_list.isEmpty())
        switchScenes(0);
}

void GameView::switchScenes(int scene_index)
{
    if(!m_gamescene_list.empty() && scene_index >= 0 && scene_index < m_gamescene_list.length()){

        // pause/stop current scene
        if(m_current_scene_index >= 0 && m_current_scene_index < m_gamescene_list.length()){
            if(m_gamescene_list.at(m_current_scene_index)->isRunning())
                m_gamescene_list.at(m_current_scene_index)->pauseGameScene();
            disconnect(this, &GameView::command, m_gamescene_list.at(m_current_scene_index), &GameScene::command);
            disconnect(m_gamescene_list.at(m_current_scene_index), &GameScene::gameCycleFinished, this, &GameView::advanceView);
            disconnect(this, &GameView::viewpointChanged, m_gamescene_list.at(m_current_scene_index), &GameScene::updateBackgroundParallax);
            disconnect(this, &GameView::visibleGameAreaChanged, m_gamescene_list.at(m_current_scene_index), &GameScene::updateActiveGameArea);
        }

        // start the new scene
        connect(this, &GameView::command, m_gamescene_list.at(scene_index), &GameScene::command);
        connect(m_gamescene_list.at(scene_index), &GameScene::gameCycleFinished, this, &GameView::advanceView);
        connect(this, &GameView::viewpointChanged, m_gamescene_list.at(scene_index), &GameScene::updateBackgroundParallax);
        connect(m_gamescene_list.at(scene_index), &GameScene::stopped, this, &GameView::resetViewpoint);
        connect(this, &GameView::visibleGameAreaChanged, m_gamescene_list.at(scene_index), &GameScene::updateActiveGameArea);

        m_current_scene_index = scene_index;
        setScene(m_gamescene_list.at(scene_index));
        m_tracked_object = nullptr;
        connect(m_gamescene_list.at(scene_index), &GameScene::trackedObjectChanged, this, &GameView::updateTrackedObject);

        if(m_gamescene_list.at(scene_index)->isPaused()){
            m_gamescene_list.at(scene_index)->continueGameScene();
        }
        else{
            resetViewpoint();
            m_gamescene_list.at(scene_index)->startGameScene();
        }
    }
}

bool GameView::deleteGameScene(int scene_index)
{
    if(!m_gamescene_list.isEmpty() && scene_index >= 0 && scene_index < m_gamescene_list.length()){
        GameScene *scene = nullptr;

        if(m_current_scene_index >= 0 && m_current_scene_index < m_gamescene_list.length())
            scene = m_gamescene_list.at(m_current_scene_index);

        delete m_gamescene_list.takeAt(scene_index);

        if(m_current_scene_index != scene_index && scene != nullptr){
            m_current_scene_index = m_gamescene_list.indexOf(scene);
        }
        else if(!m_gamescene_list.isEmpty() && m_current_scene_index == scene_index){
            m_current_scene_index = 0;
        }
        else if(m_gamescene_list.isEmpty()){
            m_current_scene_index = -1;
        }

        return true;
    }
    return false;
}

void GameView::keyPressEvent(QKeyEvent *event)
{
    m_keys[event->key()] = true;
}

void GameView::keyReleaseEvent(QKeyEvent *event)
{
    m_keys[event->key()] = false;
}

void GameView::handleUserInput()
{
    int movement = 0;
    int actions = 0;
    int modifiers = 0;

    if(m_keys[Qt::Key_Left])
        movement += 1;
    if(m_keys[Qt::Key_Right])
        movement += 3;

    if(m_keys[Qt::Key_Up])
        movement += 7;
    if(m_keys[Qt::Key_Down])
        movement += 15;

    if(m_keys[Qt::Key_Space])
        actions += 1;

    if(m_keys[Qt::Key_Shift])
        modifiers += 1;

    emit command(movement, actions, modifiers);
}

void GameView::updateTrackedObject()
{
    if(!static_cast<GameScene*>(scene())->trackedObjects()->isEmpty())
        m_tracked_object = static_cast<GameScene*>(scene())->trackedObjects()->last();
    else
        m_tracked_object = nullptr;
}

void GameView::resetViewpoint(){
    if(!m_gamescene_list.isEmpty() && m_gamescene_list.at(m_current_scene_index) != nullptr){
        centerOn(0, 0);
        m_camera_position = QPointF(0, 0);
    }
}

void GameView::adjustView()
{
    // track m_tracked_object
    if(m_tracked_object != nullptr){

        QPointF distance_from_view_center_in_scene_coordinates = m_tracked_object->pos() - m_camera_position;
        QPointF camera_movement_in_scene_coordinates = 0.05 * distance_from_view_center_in_scene_coordinates;

        centerOn(m_camera_position + camera_movement_in_scene_coordinates);

        QPointF new_camera_position = mapToScene(width()/2, height()/2);
        QPointF viewpointChange = new_camera_position - m_camera_position;
        m_camera_position = new_camera_position;

        emit viewpointChanged(viewpointChange.x(), viewpointChange.y());

        QPointF origin_in_scene = mapToScene(0, 0);
        QRectF visible_scene_area = QRectF(origin_in_scene.x(), origin_in_scene.y(), viewport()->width(), viewport()->height());

        emit visibleGameAreaChanged(visible_scene_area);
    }
}

void GameView::advanceView()
{
    handleUserInput();
    adjustView();
    viewport()->update();
}
