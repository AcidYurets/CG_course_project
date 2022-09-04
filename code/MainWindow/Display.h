#pragma once
#include <memory>
#include <QtWidgets>
#include <QOpenGLWidget>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;

class Display : public QOpenGLWidget {
    Q_OBJECT

public:
    Display(QWidget* parent);

    void initImage(shared_ptr<QImage> image);

    void mousePressEvent(QMouseEvent* mouse);
    void mouseReleaseEvent(QMouseEvent* mouse);
    void mouseMoveEvent(QMouseEvent* mouse);

    void keyPressEvent(QKeyEvent* mouse);
    void keyReleaseEvent(QKeyEvent* mouse);

signals:
    void mouseClickSignal(Vector2i pos);

    void objectMoveSignal(Vector2i lastPos, Vector2i newPos);
    void objectScaleSignal(Vector2i lastPos, Vector2i newPos);
    void objectRotateSignal(Vector2i lastPos, Vector2i newPos);

protected:
    void paintGL();

private:
    QPoint cursor;
    bool keyGClicked = false;
    bool keySClicked = false;
    bool keyRClicked = false;
    bool rightButtonClicked = false;


    shared_ptr<QImage> image;
};

