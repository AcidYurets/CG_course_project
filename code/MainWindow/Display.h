#pragma once
#include <QtWidgets>
#include <Eigen/Dense>
using namespace Eigen;

class Display : public QGraphicsView {
    Q_OBJECT

public:
    Display(QWidget* parent);

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

private:
    QPoint cursor;
    bool keyGClicked = false;
    bool keySClicked = false;
    bool keyRClicked = false;

};

