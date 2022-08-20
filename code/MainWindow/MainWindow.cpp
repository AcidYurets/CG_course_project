#include "MainWindow.h"
#include "RenderManagement/Drawer/Drawer.h"
#include "Exceptions/Exceptions.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    ui = new Ui::MainWindowUI();
    ui->setupUi(this);
    setupScene();

    QString fileName = "../data/scenes/simpleScene.sol";
    this->scene = fileManager.loadScene(fileName.toStdString());
    renderScene();
    //ui->display->resetTransform();


    connect(this->ui->open, &QAction::triggered, this, &MainWindow::openFileSlot);

    connect(this->ui->display, &Display::mouseClickSignal, this, &MainWindow::mouseClickSlot);

    connect(this->ui->display, &Display::objectMoveSignal, this, &MainWindow::objectMoveSlot);
    connect(this->ui->display, &Display::objectScaleSignal, this, &MainWindow::objectScaleSlot);
    connect(this->ui->display, &Display::objectRotateSignal, this, &MainWindow::objectRotateSlot);
}

MainWindow::~MainWindow() { 
    delete ui; 
}

void MainWindow::openFileSlot() {
   QString fileName = QFileDialog::getOpenFileName(this,
       tr("Открыть файл"), "../data/scenes", tr("*.sol"));

    //QString fileName = "../data/scenes/simpleScene.sol";
    
    try {
        this->scene = fileManager.loadScene(fileName.toStdString());

        renderScene();
    }
    catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::mouseClickSlot(Vector2i pos) {
    try {
        selectionManager.selectModel(scene, pos);

        renderScene();
    }
    catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::objectMoveSlot(Vector2i lastPos, Vector2i newPos) {
    try {
        shared_ptr<Model> model = selectionManager.getSelectedModel();
        if (model) {
            showStatusMessage("Now " + model->getName() + " is moving");

            // TODO: Еще надо будет тут учитывать местополоение камеры
            Vector3d move_params(newPos.x() - lastPos.x(), newPos.y() - lastPos.y(), 0);
            transformManager.transformModel(model, move_params, Vector3d(1, 1, 1), Vector3d(0, 0, 0));
            renderScene();
        }
    }
    catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::objectScaleSlot(Vector2i lastPos, Vector2i newPos) {
    try {
        shared_ptr<Model> model = selectionManager.getSelectedModel();
        if (model) {
            showStatusMessage("Now " + model->getName() + " is scaling");

            Vector2d center = model->getDetails()->getCenter().getScreenPosition();
            double dist1 = getDistance2D(center, lastPos.cast<double>()), dist2 = getDistance2D(center, newPos.cast<double>());
            double k = dist2 / dist1;
            Vector3d scale_params(k, k, k);
            transformManager.transformModel(model, Vector3d(0, 0, 0), scale_params, Vector3d(0, 0, 0));
            renderScene();
        }
    }
    catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::objectRotateSlot(Vector2i lastPos, Vector2i newPos) {
    try {
        shared_ptr<Model> model = selectionManager.getSelectedModel();
        if (model) {
            showStatusMessage("Now " + model->getName() + " is rotating");

            Vector2d center = model->getDetails()->getCenter().getScreenPosition();
            Vector3d rotate_params((lastPos.y() - newPos.y())*0.01, -(lastPos.x() - newPos.x())*0.01, 0); // TODO: Сделать нормальный поворот
            transformManager.transformModel(model, Vector3d(0, 0, 0), Vector3d(1, 1, 1), rotate_params);
            renderScene();
        }
    }
    catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::setupScene() {
    ui->display->setGeometry(15, 57, 730, 549);
    shared_ptr<QGraphicsScene> graphicsScene = make_shared<QGraphicsScene>(ui->display);
    graphicsScene->setSceneRect(0, 0, ui->display->width(), ui->display->height());
    ui->display->setScene(graphicsScene.get());
    shared_ptr<Drawer> drawer = make_shared<Drawer>(graphicsScene);
    renderManager.setDrawer(drawer);

    ui->display->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->display->setInteractive(true);
    ui->display->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->display->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MainWindow::renderScene() {
    try {
        renderManager.renderScene(scene, ui->display->geometry()); 
    }
    catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    renderScene();
}


void MainWindow::showStatusMessage(string message) {
    ui->statusbar->showMessage(QString::fromStdString(message));
}
