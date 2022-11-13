#include "MainWindow.h"
#include "../Exceptions/Exceptions.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent) {
	ui = new Ui::MainWindowUI();
	ui->setupUi(this);
	setupScene();

	QString fileName = "../data/scenes/cube.sol";
	this->scene = fileManager.loadScene(fileName.toStdString());
	renewObjectList();

	renderScene();
	transformManager.transformCamera(scene->getCamera(), Vector3d(screenCenter.x(), screenCenter.y(), 0), Vector3d(0, 0, 0));

	//ui->display->resetTransform();


	connect(this->ui->open, &QAction::triggered, this, &MainWindow::openFileSlot);
	connect(this->ui->loadModels, &QAction::triggered, this, &MainWindow::loadSlot);
	connect(this->ui->addCube, &QPushButton::clicked, this, &MainWindow::addCube);
	connect(this->ui->addSphere, &QPushButton::clicked, this, &MainWindow::addSphere);
	connect(this->ui->addPiramid, &QPushButton::clicked, this, &MainWindow::addPiramid);
	connect(this->ui->addThor, &QPushButton::clicked, this, &MainWindow::addThor);

	connect(this->ui->modeBox, &QComboBox::currentIndexChanged, this, &MainWindow::modeChanged);

	connect(this->ui->display, &Display::mouseClickSignal, this, &MainWindow::mouseClickSlot);

	connect(this->ui->display, &Display::objectMoveSignal, this, &MainWindow::objectMoveSlot);
	connect(this->ui->display, &Display::objectScaleSignal, this, &MainWindow::objectScaleSlot);
	connect(this->ui->display, &Display::objectRotateSignal, this, &MainWindow::objectRotateSlot);
	connect(this->ui->display, &Display::cameraMoveSignal, this, &MainWindow::cameraMoveSlot);
	connect(this->ui->display, &Display::cameraRotateSignal, this, &MainWindow::cameraRotateSlot);
}

MainWindow::~MainWindow() { 
	delete ui; 
}

void MainWindow::openFileSlot() {
   QString fileName = QFileDialog::getOpenFileName(this,
	   tr("Открыть файл"), "../data/scenes", tr("*.sol"));

	try {
		this->scene = fileManager.loadScene(fileName.toStdString());
		renewObjectList();

		transformManager.transformCamera(scene->getCamera(), Vector3d(screenCenter.x(), screenCenter.y(), 0), Vector3d(0, 0, 0));

		renderScene();
	}
	catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::loadSlot() {
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Загрузить модель"), "../data/objects", tr("*.obj"));

	try {
		Models loadedModels = fileManager.loadModels(fileName.toStdString());
		this->scene->getModels().insert(this->scene->getModels().end(), loadedModels.begin(), loadedModels.end());
		renewObjectList();

		renderScene();
	}
	catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::addCube() {
	try {
		Models loadedModels = fileManager.loadModels("../data/objects/cube.obj");
		this->scene->getModels().insert(this->scene->getModels().end(), loadedModels.begin(), loadedModels.end());
		renewObjectList();

		renderScene();
	}
	catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::addSphere() {
	try {
		Models loadedModels = fileManager.loadModels("../data/objects/icosphere.obj");
		this->scene->getModels().insert(this->scene->getModels().end(), loadedModels.begin(), loadedModels.end());
		renewObjectList();

		renderScene();
	}
	catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::addPiramid() {
	try {
		Models loadedModels = fileManager.loadModels("../data/objects/tetrahedron.obj");
		this->scene->getModels().insert(this->scene->getModels().end(), loadedModels.begin(), loadedModels.end());
		renewObjectList();

		renderScene();
	}
	catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::addThor() {
	try {
		Models loadedModels = fileManager.loadModels("../data/objects/thor.obj");
		this->scene->getModels().insert(this->scene->getModels().end(), loadedModels.begin(), loadedModels.end());
		renewObjectList();

		renderScene();
	}
	catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::mouseClickSlot(Vector2i pos) {
	try {
		showStatusMessage("Mouse clicked to " + to_string(pos.x()) + ", " + to_string(pos.y()));

		// Сбрасываем выбранные элементы
		selectionManager.clearSelecteds(scene);

		switch (renderManager.mode) {
		case objectMode:
			selectionManager.selectModel(scene, pos);
			break;
		case faceMode:
			selectionManager.selectFace(scene, pos);
			break;
		case edgeMode:
			selectionManager.selectEdge(scene, pos);
			break;
		case vertexMode:
			selectionManager.selectVertex(scene, pos);
			break;
		}

		renderScene();
	}
	catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::objectMoveSlot(Vector2i lastPos, Vector2i newPos) {
	try {
		Vector4d moveParams4((newPos.x() - lastPos.x()), (newPos.y() - lastPos.y()), 0, 1);
		Vector4d globalMoveParams4 = scene->getCamera()->getTransMatrix() * moveParams4;
		Vector3d globalMoveParams;
		globalMoveParams << globalMoveParams4.x(), globalMoveParams4.y(), globalMoveParams4.z();

		switch (renderManager.mode) {
		case objectMode: {
				shared_ptr<Model> model = selectionManager.getSelectedModel();
				if (model) {
					showStatusMessage("Now " + model->getName() + " is moving");

					transformManager.transformModel(model, globalMoveParams, Vector3d(1, 1, 1), Vector3d(0, 0, 0));
					renderScene();
				}
			}
			break;

		case vertexMode: {
				Vertices vertices = selectionManager.getSelectedVertices();
				for (auto& v : vertices) {
					transformManager.transformVertex(v, globalMoveParams, Vector3d(1, 1, 1), Vector3d(0, 0, 0));
					renderScene();
				}
			}
			break;
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
				screenCenter);
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

			// TODO: Сделать поворот относительно всех осей
			// Vector2d center = model->getDetails()->getCenter().getScreenPosition();
			Vector3d rotate_params((lastPos.y() - newPos.y())*0.01, -(lastPos.x() - newPos.x())*0.01, 0); // TODO: Сделать нормальный поворот
			transformManager.transformModel(model, Vector3d(0, 0, 0), Vector3d(1, 1, 1), rotate_params);
			renderScene();
		}
	}
	catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::cameraMoveSlot(Vector2i lastPos, Vector2i newPos) {
	try {
		showStatusMessage("Now camera is moving");

		Vector3d move_params(newPos.x() - lastPos.x(), newPos.y() - lastPos.y(), 0);
		transformManager.transformCamera(scene->getCamera(), move_params, Vector3d(0, 0, 0));
		renderScene();
	}
	catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::cameraRotateSlot(Vector2i lastPos, Vector2i newPos) {
	try {
		showStatusMessage("Now camera is rotating");

		Vector3d rotate_params(-(lastPos.y() - newPos.y()) * 0.01, (lastPos.x() - newPos.x()) * 0.01, 0);
		transformManager.transformCamera(scene->getCamera(),  Vector3d(0, 0, 0), rotate_params);
		renderScene();
	}
	catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::on_projectionButton_clicked() {
	selectionManager.isPerspective = !renderManager.getPerspective();
	renderManager.setPerspective(!renderManager.getPerspective());
	
	renderScene();
}

void MainWindow::on_rerenderButton_clicked() {
	renderScene();
}

void MainWindow::modeChanged(int index) {
	renderManager.mode = ui->modeBox->currentIndex();
}

void MainWindow::setupScene() {
	shared_ptr<QImage> image = make_shared<QImage>(ui->display->width(), ui->display->height(), QImage::Format_RGB32);
	ui->display->initImage(image);
	renderManager.initImage(image);

	// TODO: Это костыль
	ui->display->setGeometry(9, 57, 758, 549);
	ui->display->setFocusPolicy(Qt::StrongFocus);

	this->screenCenter = Vector2d(ui->display->geometry().center().x(), ui->display->geometry().center().y());
}

void MainWindow::renderScene() {
	try {
		renderManager.renderScene(scene, ui->display->geometry());

		selectionManager.setFaceBuffer(renderManager.getFaceBuffer());
		selectionManager.setModelBuffer(renderManager.getModelBuffer());
		this->screenCenter = Vector2d(ui->display->geometry().center().x(), ui->display->geometry().center().y());
		selectionManager.screenCenter = this->screenCenter;

		ui->display->update();
	}
	catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::resizeEvent(QResizeEvent* event) {
	renderScene();
}

void MainWindow::renewObjectList() {
	try {
		if (!scene) throw EmptyException(EXCEPCION_ARGS, "Scene is null");
		auto modelsTree = ui->objectsWidget->findItems(tr("Модели"), Qt::MatchExactly)[0];

		auto &models = scene->getModels();
		auto modelsChildern = modelsTree->takeChildren();
		modelsChildern.clear();
		for (size_t i = 0; i < models.size(); i++) {
			modelsChildern.append(new QTreeWidgetItem(modelsTree, QStringList(QString(models[i]->getName().c_str()))));
		}
		modelsTree->addChildren(modelsChildern);
		ui->objectsWidget->addTopLevelItem(modelsTree);
	}
	catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}


void MainWindow::showStatusMessage(string message) {
	ui->statusbar->showMessage(QString::fromStdString(message));
}
