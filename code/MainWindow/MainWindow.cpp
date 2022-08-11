#include "MainWindow.h"
#include "Exceptions/Exceptions.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    ui = new Ui::MainWindowUI();
    ui->setupUi(this);
    setupScene();


    connect(this->ui->open, &QAction::triggered, this, &MainWindow::openFileSlot);

    connect(this->ui->display, &Display::mouseClickSignal, this, &MainWindow::mouseClickSlot);

    connect(this->ui->display, &Display::objectMoveSignal, this, &MainWindow::objectMoveSlot);
    connect(this->ui->display, &Display::objectScaleSignal, this, &MainWindow::objectScaleSlot);
}

MainWindow::~MainWindow() { 
    delete ui; 
}

void MainWindow::openFileSlot() {
   // QString fileName = QFileDialog::getOpenFileName(this,
   //     tr("Открыть файл"), "../data/scenes", tr("*.sol"));

    QString fileName = "../data/scenes/simpleScene.sol";

    this->scene = fileManager.loadScene(fileName.toStdString());

    renderScene();
}

void MainWindow::mouseClickSlot(Vector2i pos) {
    try {
        selectionManager.selectModel(scene, pos);

        renderScene();
    }
    catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::objectMoveSlot(Vector2i lastPos, Vector2i newPos) {
    shared_ptr<Model> model = selectionManager.getSelectedModel();
    try {
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
    shared_ptr<Model> model = selectionManager.getSelectedModel();
    try {
        if (model) {
            showStatusMessage("Now " + model->getName() + " is scaling");

            Vector3d center = model->getDetails()->getCenter();
            double distance = getDistance2D()
            Vector3d scale_params(newPos.x() - lastPos.x(), newPos.y() - lastPos.y(), 0); // TODO !!!
            transformManager.transformModel(model, Vector3d(0, 0, 0), scale_params, Vector3d(0, 0, 0));
            renderScene();
        }
    }
    catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::setupScene() {
    graphicsScene = std::make_shared<QGraphicsScene>(ui->display);
    this->graphicsScene->setSceneRect(0, 0, ui->display->width(), ui->display->height());
    ui->display->setScene(graphicsScene.get());

    ui->display->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->display->setInteractive(true);
}

void MainWindow::renderScene() {
    try {
        if (!scene) throw EmptyException(EXCEPCION_ARGS, "Scene was't created");
        renderManager.renderScene(graphicsScene.get(), scene);
    }
    catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::showStatusMessage(string message) {
    ui->statusbar->showMessage(QString::fromStdString(message));
}
