#pragma once

#include <QtWidgets>

class Display : public QGraphicsView {
    Q_OBJECT

public:
    Display(QWidget* parent = nullptr) : QGraphicsView(parent) {};

};

