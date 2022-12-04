#include "MainWindow.h"
#include "../Exceptions/Exceptions.h"
#include "../Config.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent) {
	ui = new Ui::MainWindowUI();
	ui->setupUi(this);
	setupScene();
	Eigen::initParallel();

	QString fileName = "../data/for_tests/plant.sol";
	//QString fileName = "../data/scenes/thor.sol";
	this->scene = fileManager.loadScene(fileName.toStdString());
	renewObjectList();

	// Двигаем камеру с центр сцены
	transformManager.transformCamera(scene->getCamera(), Vector3d(screenCenter.x(), screenCenter.y()-50, 0), Vector3d(0, 0, 0));
	// Поворачиваем камеры
	//transformManager.transformCamera(scene->getCamera(), Vector3d(0, 0, 0), Vector3d(M_PI + M_PI/6, 0, 0));
	// Увеличиваем тор, чтобы было лучше видно
	//transformManager.transformModel(scene->getModels()[0], Vector3d(0, 0, 0), Vector3d(2, 2, 2), Vector3d(0, 0, 0));
	
	//ui->display->resetTransform();


	connect(this->ui->open, &QAction::triggered, this, &MainWindow::openFileSlot);
	connect(this->ui->loadModels, &QAction::triggered, this, &MainWindow::loadSlot);
	connect(this->ui->addCube, &QPushButton::clicked, this, &MainWindow::addCube);
	connect(this->ui->addSphere, &QPushButton::clicked, this, &MainWindow::addSphere);
	connect(this->ui->addPiramid, &QPushButton::clicked, this, &MainWindow::addPiramid);
	connect(this->ui->addThor, &QPushButton::clicked, this, &MainWindow::addThor);
	connect(this->ui->addLS, &QPushButton::clicked, this, &MainWindow::addLS);

	connect(this->ui->modeBox, &QComboBox::currentIndexChanged, this, &MainWindow::modeChanged);
	connect(this->ui->parallelBox, &QCheckBox::stateChanged, this, &MainWindow::parallelChanged);
	connect(this->ui->evaluationButton, &QPushButton::clicked, this, &MainWindow::evaluateTime);

	connect(this->ui->display, &Display::mouseClickSignal, this, &MainWindow::mouseClickSlot);

	connect(this->ui->display, &Display::objectMoveSignal, this, &MainWindow::objectMoveSlot);
	connect(this->ui->display, &Display::objectScaleSignal, this, &MainWindow::objectScaleSlot);
	connect(this->ui->display, &Display::objectRotateSignal, this, &MainWindow::objectRotateSlot);
	connect(this->ui->display, &Display::cameraMoveSignal, this, &MainWindow::cameraMoveSlot);
	connect(this->ui->display, &Display::cameraRotateSignal, this, &MainWindow::cameraRotateSlot);

	this->showMaximized();
	renderScene();
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

void MainWindow::addLS() {
	try {
		int lsCount = this->scene->getLightSources().size();

		shared_ptr<LightSource> ls = make_shared<LightSource>(Vertex(0, 0, 0), "Источник света " + to_string(lsCount + 1));
		this->scene->getLightSources().push_back(ls);
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
		case lightSourceMode:
			selectionManager.selectLightSources(scene, pos);
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

		case faceMode: {
				Faces faces = selectionManager.getSelectedFaces();
				for (auto& f : faces) {
					transformManager.transformFace(f, globalMoveParams, Vector3d(1, 1, 1), Vector3d(0, 0, 0));
				}
				renderScene();
			}
			break;

		case edgeMode: {
				Edges edges = selectionManager.getSelectedEdges();
				for (auto& e : edges) {
					transformManager.transformEdge(e, globalMoveParams, Vector3d(1, 1, 1), Vector3d(0, 0, 0));
				}
				renderScene();
			}
			break;

		case vertexMode: {
				Vertices vertices = selectionManager.getSelectedVertices();
				for (auto& v : vertices) {
					transformManager.transformVertex(v, globalMoveParams, Vector3d(1, 1, 1), Vector3d(0, 0, 0));
				}
				renderScene();
			}
			break;
		case lightSourceMode: {
				LightSources lightSources = selectionManager.getSelectedLightSources();
				for (auto& ls : lightSources) {
					transformManager.transformLightSource(ls, globalMoveParams, Vector3d(1, 1, 1), Vector3d(0, 0, 0));
				}
				renderScene();
			}
			break;
		}
	}
	catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::objectScaleSlot(Vector2i lastPos, Vector2i newPos) {
	try {
		// Определяем центр вращения
		Vector3d center;
		switch (renderManager.mode) {
			case objectMode: {
				shared_ptr<Model> model = selectionManager.getSelectedModel();
				if (model == nullptr) return;
				center = model->getDetails()->getCenter().getScreenPosition(scene->getCamera(), renderManager.getPerspective(),
					screenCenter);
			}
			break;
			case edgeMode: {
				Edges edges = selectionManager.getSelectedEdges();
				if (edges.size() == 0) return;
				center = edges[0]->getCenter().getScreenPosition(scene->getCamera(), renderManager.getPerspective(),
					screenCenter);
			}
						 break;
			case faceMode: {
				Faces faces = selectionManager.getSelectedFaces();
				if (faces.size() == 0) return;
				center = faces[0]->getCenter().getScreenPosition(scene->getCamera(), renderManager.getPerspective(),
					screenCenter);
			}
			break;
		}

		Vector2d center2d = center.head<2>();
		double dist1 = getDistance2D(center2d, lastPos.cast<double>()), dist2 = getDistance2D(center2d, newPos.cast<double>());
		double k = dist2 / dist1;
		Vector3d scaleParams(k, k, k);

		switch (renderManager.mode) {
			case objectMode: {
				shared_ptr<Model> model = selectionManager.getSelectedModel();
				if (model) {
					showStatusMessage("Now " + model->getName() + " is scaling");

					transformManager.transformModel(model, Vector3d(0, 0, 0), scaleParams, Vector3d(0, 0, 0));
					renderScene();
				}
			}
			break;
			case edgeMode: {
				Edges edges = selectionManager.getSelectedEdges();
				for (auto& e : edges) {
					transformManager.transformEdge(e, Vector3d(0, 0, 0), scaleParams, Vector3d(0, 0, 0));
				}
				renderScene();
			}
			break;
			case faceMode: {
				Faces faces = selectionManager.getSelectedFaces();
				for (auto& f : faces) {
					transformManager.transformFace(f, Vector3d(0, 0, 0), scaleParams, Vector3d(0, 0, 0));
				}
				renderScene();
			}
			break;
		}
	}
	catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::objectRotateSlot(Vector2i lastPos, Vector2i newPos) {
	try {
		// Определяем центр вращения
		Vector3d center;
		switch (renderManager.mode) {
			case objectMode: {
				shared_ptr<Model> model = selectionManager.getSelectedModel();
				if (model == nullptr) return;
				center = model->getDetails()->getCenter().getScreenPosition(scene->getCamera(), renderManager.getPerspective(),
					screenCenter);
			}
			break;
			case edgeMode: {
				Edges edges = selectionManager.getSelectedEdges();
				if (edges.size() == 0) return;
				center = edges[0]->getCenter().getScreenPosition(scene->getCamera(), renderManager.getPerspective(),
					screenCenter);
			}
			break;
			case faceMode: {
				Faces faces = selectionManager.getSelectedFaces();
				if (faces.size() == 0) return;
				center = faces[0]->getCenter().getScreenPosition(scene->getCamera(), renderManager.getPerspective(),
					screenCenter);
			}
			break;
		}
		

		double a1 = atan((lastPos.y() - center.y()) / double((lastPos.x() - center.x())));
		double a2 = atan((newPos.y() - center.y()) / double((newPos.x() - center.x())));
		double res = a1 - a2;
		if (res < -M_PI / 2) {
			res += M_PI;
		}
		else if (res > M_PI / 2) {
			res -= M_PI;
		}

		Vector4d rotateParams4(0, 0, res, 1);
		Vector4d globalRotateParams4 = scene->getCamera()->getTransMatrix() * rotateParams4;
		Vector3d globalRotateParams;
		globalRotateParams << globalRotateParams4.x(), globalRotateParams4.y(), globalRotateParams4.z();

		switch (renderManager.mode) {
			case objectMode: {
				shared_ptr<Model> model = selectionManager.getSelectedModel();
				if (model) {
					showStatusMessage("Now " + model->getName() + " is rotating");

					transformManager.transformModel(model, Vector3d(0, 0, 0), Vector3d(1, 1, 1), globalRotateParams);
					renderScene();
				}
			}
			break;
			case edgeMode: {
				Edges edges = selectionManager.getSelectedEdges();
				for (auto& e : edges) {
					transformManager.transformEdge(e, Vector3d(0, 0, 0), Vector3d(1, 1, 1), globalRotateParams);
				}
				renderScene();
			}
			break;
			case faceMode: {
				Faces faces = selectionManager.getSelectedFaces();
				for (auto& f : faces) {
					transformManager.transformFace(f, Vector3d(0, 0, 0), Vector3d(1, 1, 1), globalRotateParams);
				}
				renderScene();
			}
			break;
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

void MainWindow::on_frameButton_clicked() {
	config.isWireframe = !config.isWireframe;

	renderScene();
}

void MainWindow::on_rerenderButton_clicked() {
	renderScene();
}

void MainWindow::modeChanged(int index) {
	renderManager.mode = ui->modeBox->currentIndex();
}

void MainWindow::parallelChanged(int state) {
	if (state == Qt::Checked)
		config.isParallel = true;
	else if (state == Qt::Unchecked)
		config.isParallel = false;
}

void MainWindow::evaluateTime() {
	try {
		config.of.open("../evaluation/time_evaluation.tsv");
		if (config.of.fail()) {
			throw FileOpenException(EXCEPCION_ARGS, "Can't open file for evaluation");
		}

		int cnt = 50; // Кол-во замеров

		// Замеряем время без распараллеливания
		for (int i = 0; i < cnt; i++) {
			config.isParallel = false;
			renderScene();
		}

		long long avgTime = 0;
		for (long long time : config.tmp) {
			avgTime += time;
		}
		avgTime /= config.tmp.size();
		config.of << 0 << "\t" << avgTime << endl;
		config.tmp.clear();

		// Замеряем время для разного количества потоков
		for (int k = 1; k <= 128; k *= 2) {
			for (int i = 0; i < cnt; i++) {
				config.isParallel = true;
				config.threadCount = k;
				renderScene();
			}
			long long avgTime = 0;
			for (long long time : config.tmp) {
				avgTime += time;
			}
			avgTime /= config.tmp.size();
			config.of << config.threadCount << "\t" << avgTime << endl;
			config.tmp.clear();
		}

		config.of.close();
	}
	catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}

void MainWindow::setupScene() {
	shared_ptr<QImage> image = make_shared<QImage>(ui->display->width(), ui->display->height(), QImage::Format_RGB32);
	ui->display->initImage(image);
	renderManager.initImage(image);

	// TODO: Это костыль
	ui->display->setGeometry(9, 57, 758, 549);
	ui->display->setFocusPolicy(Qt::StrongFocus);

	// Заполняю текст информационного виджета
	ui->infoLabel->setText("### Информация о работе программы\n\
Клавиши для выбора режима преобразований:\n\
* *G* - режим переноса\n\
* *R* - режим поворота\n\
* *S* - режим масштабирования\n\
\n\
Выбор медели, ее составляющей части или  \n\
источника освещения осуществляется \n\
при помощи *левой кнопки мыши*.\n\
\n\
Преобразований будет применено к выбранному \n\
объекту сцены\n\
\n\
Управление камерой:\n\
* *Правая кнопка мыши* - перенос камеры \n\
* *Средняя кнопка мыши* - поворот камеры \n\
");

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

		auto lsTree = ui->objectsWidget->findItems(tr("Источники света"), Qt::MatchExactly)[0];

		auto& ls = scene->getLightSources();
		auto lsChildern = lsTree->takeChildren();
		lsChildern.clear();
		for (size_t i = 0; i < ls.size(); i++) {
			lsChildern.append(new QTreeWidgetItem(lsTree, QStringList(QString(ls[i]->getName().c_str()))));
		}
		lsTree->addChildren(lsChildern);
		ui->objectsWidget->addTopLevelItem(lsTree);
	}
	catch (BaseException ex) { QMessageBox::critical(this, "Error", ex.what()); }
}


void MainWindow::showStatusMessage(string message) {
	ui->statusbar->showMessage(QString::fromStdString(message));
}
