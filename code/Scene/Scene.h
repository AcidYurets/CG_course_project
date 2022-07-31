#pragma once
#include "Objects/Model/PolygonalModel/PolygonalModel.h"
#include "Objects/LightSource/LightSource.h"
#include "Objects/Camera/Camera.h"

using Models = std::vector<std::shared_ptr<PolygonalModel>>;
using LightSources = std::vector<std::shared_ptr<LightSource>>;

class Scene
{
public:
	Scene();

	Models& getModels();
	LightSources& getLightSources();
	Camera& getCamera();

	void setModels(const Models& models);
	void setLightSources(const LightSources& lightSources);
	void setCamera(const Camera& camera);

private:
	Models models;
	LightSources lightSources;
	Camera camera;
};

