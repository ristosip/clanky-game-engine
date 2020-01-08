#include <QApplication>
#include "demogameview.h"
#include "demogame.h"
#include <QOpenGLWidget>

int main(int argc, char *argv[])
{  
    QApplication app(argc, argv);

    qApp->setOverrideCursor(QCursor(Qt::BlankCursor));

    DemoGameView view;
    view.addGameScene(new DemoGame());
    view.startGame();
    view.setViewport(new QOpenGLWidget());
    view.showFullScreen();

    return app.exec();
}
