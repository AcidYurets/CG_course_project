#include "Loader.h"
#include "Exceptions/Exceptions.h"

#define MAX_LEN 100

Loader::Loader() : srcFile(nullptr) { }

Loader::Loader(string srcFilename) {
	this->srcFile = std::make_shared<std::ifstream>(srcFilename);
	if (!*(this->srcFile)) throw FileOpenException(EXCEPCION_ARGS, "Could not open source file");
}

void Loader::openFile(const std::string& src_name) {
	this->srcFile = std::make_shared<std::ifstream>(src_name);
	if (!*(this->srcFile)) throw FileOpenException(EXCEPCION_ARGS, "Could not open source file");
}

void Loader::closeFile() {
	this->srcFile->close();
}

Models Loader::loadModels() {
	if (!*(this->srcFile)) throw FileOpenException(EXCEPCION_ARGS, "File was't open yet");

	Models models(0);
		
	shared_ptr<PolygonalModelDetails> details = nullptr;
	string name = "Nameless";
	Edges modelEdges(0);

	size_t verticesReaded = 0;

	string line;
	while (getline(*srcFile, line)) {
		string key = "";
		istringstream lineStream(line);

		lineStream >> key;
		if (key == "o") {
			// Если это не первая модель в файле
			if (details) {
				details->setEdges(modelEdges);
				details->setArithmeticCenter();
				shared_ptr<Model> m = make_shared<Model>(details, name);
				models.push_back(m);

				verticesReaded += details->getVertices().size();
			}

			if (!(lineStream >> name)) throw FileFormatException(EXCEPCION_ARGS, "Invalid file format: can't read model name");
			shared_ptr<PolygonalModelDetails> details = make_shared<PolygonalModelDetails>();
		}
		else if (key == "v") {
			if (!details) throw FileFormatException(EXCEPCION_ARGS, "Invalid file format: details is nullptr");
			shared_ptr<Vertex> v = make_shared<Vertex>(readPosition(lineStream));
			details->addVertex(v);
		}
		else if (key == "f") {
			if (!details) throw FileFormatException(EXCEPCION_ARGS, "Invalid file format: details is nullptr");
			vector<int> vNumbers = readFaceDetails(lineStream);
			Vertices vertices(0);
			Edges faceEdges(0);

			for (int num : vNumbers) vertices.push_back(details->getVertices()[num - verticesReaded]);
			for (int i = 0; i < vertices.size(); i++) {
				shared_ptr<Edge> e = make_shared<Edge>(vertices[i], vertices[(i + 1) % vertices.size()]);
				e = addEdgeToModel(modelEdges, e);
				faceEdges.push_back(e);
			}

			shared_ptr<Face> f = make_shared<Face>(Edges(), vertices);
			details->addFace(f);
		}
	}
	
	// Сохраним последнюю считанную модель
	if (details) {
		details->setEdges(modelEdges);
		details->setArithmeticCenter();
		shared_ptr<Model> m = make_shared<Model>(details, name);
		models.push_back(m);
	}

	return models;
}

shared_ptr<Scene> Loader::loadScene()
{
	if (!*(this->srcFile)) throw FileOpenException(EXCEPCION_ARGS, "File was't open yet");

	Models models(0);
	LightSources lightSources(0);
	shared_ptr<Camera> camera(nullptr);

	shared_ptr<PolygonalModelDetails> details(nullptr);
	string modelName = "Nameless model";
	Edges modelEdges(0);
	shared_ptr<LightSource> ls(nullptr);
	string lsName = "Nameless light source";

	size_t verticesReaded = 0;

	string line;
	while (getline(*srcFile, line)) {
		string key = "";
		istringstream lineStream(line);

		lineStream >> key;
		if (key == "o") { // Считываем модель
			// Если это не первая модель в файле
			if (details) {
				details->setEdges(modelEdges);
				details->setArithmeticCenter();
				shared_ptr<Model> m = make_shared<Model>(details, modelName);
				models.push_back(m);

				verticesReaded += details->getVertices().size();
			}

			if (!(lineStream >> modelName)) throw FileFormatException(EXCEPCION_ARGS, "Invalid file format: can't read model name");
			details = make_shared<PolygonalModelDetails>();
		}
		else if (key == "v") { // Считываем вершину модели
			if (!details) throw FileFormatException(EXCEPCION_ARGS, "Invalid file format: details is nullptr");
			shared_ptr<Vertex> v = make_shared<Vertex>(readPosition(lineStream));
			details->addVertex(v);
		}
		else if (key == "f") { // Считываем грань модели
			if (!details) throw FileFormatException(EXCEPCION_ARGS, "Invalid file format: details is nullptr");
			vector<int> vNumbers = readFaceDetails(lineStream);
			Vertices vertices(0);
			Edges faceEdges(0);

			for (int num : vNumbers) vertices.push_back(details->getVertices()[num - verticesReaded]);
			for (int i = 0; i < vertices.size(); i++) {
				shared_ptr<Edge> e = make_shared<Edge>(vertices[i], vertices[(i + 1) % vertices.size()]);
				e = addEdgeToModel(modelEdges, e);
				faceEdges.push_back(e);
			}

			shared_ptr<Face> f = make_shared<Face>(faceEdges, vertices);
			details->addFace(f);
		}
		else if (key == "ls") { // Считываем источние света
			// Если это не первый источник света в файле
			if (ls) {
				lightSources.push_back(ls);
			}

			if (!(lineStream >> lsName)) throw FileFormatException(EXCEPCION_ARGS, "Invalid file format: can't read ls name");
			Vertex v = Vertex(readPosition(lineStream));
			ls = make_shared<LightSource>(v, lsName);
		}
		else if (key == "cam") { // Считываем источние света
			Vertex v = Vertex(readPosition(lineStream));
			camera = make_shared<Camera>(v);
		}
	}

	// Сохраним последнюю считанную модель
	if (details) {
		details->setEdges(modelEdges);
		details->setArithmeticCenter();
		shared_ptr<Model> m = make_shared<Model>(details, modelName);
		models.push_back(m);
	}
	
	shared_ptr<Scene> scene = make_shared<Scene>();
	scene->setModels(models);
	scene->setLightSources(lightSources);
	scene->setCamera(camera);

	return scene;
}

Vector3d Loader::readPosition(istringstream &lineStream) {
	double x, y, z;
	if (!(lineStream >> x >> y >> z)) throw FileFormatException(EXCEPCION_ARGS, "Invalid file format: can't read position");
	return Vector3d(x, y, z);
}

vector<int> Loader::readFaceDetails(istringstream &lineStream) {
	vector<int> faceVertices(0);
	int v = 0;
	while (lineStream >> v) {
		faceVertices.push_back(v - 1);
		// TODO: Добавить считывание нормалей
	}
	if (faceVertices.size() < 3) throw FileFormatException(EXCEPCION_ARGS, "Invalid file format: less then 3 vertices if face");
	return faceVertices;
}

shared_ptr<Edge> Loader::addEdgeToModel(Edges &modelEdges, const shared_ptr<Edge> &newEdge) {
	bool found = false;
	shared_ptr<Edge> foundEdge(newEdge);
	if (modelEdges.size() == 0)
		modelEdges.push_back(newEdge);
	else {
		for (auto& e : modelEdges) {
			// Сравниваем вершины ребра в прямом и обратном поядке. 
			// TODO: Убедиться, что сравнение работает.
			if (e->getVertices().x() == newEdge->getVertices().x() && e->getVertices().y() == newEdge->getVertices().y() || 
				e->getVertices().x() == newEdge->getVertices().y() && e->getVertices().y() == newEdge->getVertices().x()) {
				// Если ребро уже есть в списке:
				found = true;
				foundEdge = e;
				break;
			}
		}
		if (!found) {
			modelEdges.push_back(newEdge);
		}
	}
		

	return foundEdge;
}
