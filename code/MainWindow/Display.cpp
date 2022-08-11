#include "Display.h"

Display::Display(QWidget* parent = nullptr) : QGraphicsView(parent) { 
    setMouseTracking(true);
}

void Display::mousePressEvent(QMouseEvent* mouse) {
}

void Display::mouseReleaseEvent(QMouseEvent* mouse) {
    if (keyGClicked || keySClicked || keyRClicked) {
        switch (mouse->button()) {
        case(Qt::LeftButton):
            keyGClicked = false;
            keySClicked = false;
            keyRClicked = false;
            break;
        }
    }
    else {
        emit mouseClickSignal(Vector2i(cursor.x(), cursor.y()));
    }
}

void Display::mouseMoveEvent(QMouseEvent* mouse) {

    if (keyGClicked) {
        emit objectMoveSignal(Vector2i(cursor.x(), cursor.y()), Vector2i(mouse->x(), mouse->y()));
    }
    else if (keySClicked) {
        emit objectScaleSignal(Vector2i(cursor.x(), cursor.y()), Vector2i(mouse->x(), mouse->y()));
    }
    cursor = mouse->pos();
}

void Display::keyPressEvent(QKeyEvent* key) {
}

void Display::keyReleaseEvent(QKeyEvent* key) {
    switch (key->key()) {
    case(Qt::Key_G):
        keyGClicked = true;
        keySClicked = false;
        keyRClicked = false;
        qDebug() << "Key G clicked";
        break;
    case(Qt::Key_S):
        keyGClicked = false;
        keySClicked = true;
        keyRClicked = false;
        break;
    case(Qt::Key_R):
        keyGClicked = false;
        keySClicked = false;
        keyRClicked = true;
        break;
    }
}
