#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "../Scene/Scene.h"
#include "../SelectionManagement/SelectionManager.h"
#include "../TransformManagement/TransformManager.h"
#include "../FileManagement/FileManager.h"
#include "../RenderManagement/RenderManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = Q_NULLPTR);
    ~MainWindow();

public slots:
    void openFileSlot();

    void mouseClickSlot(Vector2i pos);

    void objectMoveSlot(Vector2i lastPos, Vector2i newPos);
    void objectScaleSlot(Vector2i lastPos, Vector2i newPos);
    void objectRotateSlot(Vector2i lastPos, Vector2i newPos);

    void on_projectionButton_clicked();
    void on_rerenderButton_clicked();
    void modeChanged(int index);

private:
    void setupScene();
    void renderScene();
    void resizeEvent(QResizeEvent* event);

    void showStatusMessage(string message);

    Ui::MainWindowUI *ui;
    RenderManager renderManager;
    SelectionManager selectionManager;
    TransformManager transformManager;
    FileManager fileManager;
    
    shared_ptr<Scene> scene;
    Vector2d screenCenter;
};
