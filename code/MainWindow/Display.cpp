#include "Display.h"

Display::Display(QWidget* parent = nullptr) : QOpenGLWidget(parent) {
    // setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true); 
}

void Display::initImage(shared_ptr<QImage> image) {
    this->image = image;

    update();
}

void Display::mousePressEvent(QMouseEvent* mouse) {
    switch (mouse->button()) {
    case(Qt::RightButton):
        rightButtonClicked = true;
        break;
    }
}

void Display::mouseReleaseEvent(QMouseEvent* mouse) {
    qDebug() << "Mouse clicked";

    switch (mouse->button()) {
    case(Qt::RightButton):
        // тут тут тут тут тут
        rightButtonClicked = false;
        break;
    }

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
    else if (keyRClicked) {
        emit objectRotateSignal(Vector2i(cursor.x(), cursor.y()), Vector2i(mouse->x(), mouse->y()));
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
        qDebug() << "Key S clicked";
        break;
    case(Qt::Key_R):
        keyGClicked = false;
        keySClicked = false;
        keyRClicked = true;
        qDebug() << "Key R clicked";
        break;
    }
}

void Display::paintGL() {
    QPainter painter(this);

    painter.drawImage(0, 0, *image);
}
