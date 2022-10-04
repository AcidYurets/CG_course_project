#include "MainWindow.h"
#include "../Exceptions/Exceptions.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    ui = new Ui::MainWindowUI();
    ui->setupUi(this);
    setupScene();

    QString fileName = "../data/scenes/cube.sol";
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
        showStatusMessage("Mouse clocked on " + to_string(pos.x()) + ", " + to_string(pos.y()));
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

            Vector3d center = model->getDetails()->getCenter().getScreenPosition(scene->getCamera(), renderManager.getPerspective(), 
                Vector2d(ui->display->geometry().center().x(), ui->display->geometry().center().y()));
            Vector2d center2d = center.head<2>();
            double dist1 = getDistance2D(center2d, lastPos.cast<double>()), dist2 = getDistance2D(center2d, newPos.cast<double>());
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

            // Vector2d center = model->getDetails()->getCenter().getScreenPosition();
            Vector3d rotate_params((lastPos.y() - newPos.y())*0.01, -(lastPos.x() - newPos.x())*0.01, 0); // TODO: Сделать нормальный поворот
            transformManager.transformModel(model, Vector3d(0, 0, 0), Vector3d(1, 1, 1), rotate_params);
            renderScene();
        }
    }
    catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::on_projectionButton_clicked() {
    renderManager.setPerspective(!renderManager.getPerspective());

    renderScene();
}

void MainWindow::on_rerenderButton_clicked() {
    renderScene();
}

void MainWindow::setupScene() {
    shared_ptr<QImage> image = make_shared<QImage>(ui->display->width(), ui->display->height(), QImage::Format_RGB32);
    ui->display->initImage(image);
    renderManager.initImage(image);

    // TODO: Это костыль
    ui->display->setGeometry(9, 57, 758, 549);
    ui->display->setFocusPolicy(Qt::StrongFocus);
}

void MainWindow::renderScene() {
    try {
        renderManager.renderScene(scene, ui->display->geometry());

        ui->display->update();
    }
    catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    renderScene();
}


void MainWindow::showStatusMessage(string message) {
    ui->statusbar->showMessage(QString::fromStdString(message));
}
