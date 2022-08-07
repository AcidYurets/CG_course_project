#pragma once
#include "Objects/Model/Model.h"
#include "Objects/LightSource/LightSource.h"
#include "Objects/Camera/Camera.h"

using Models = std::vector<std::shared_ptr<Model>>;
using LightSources = std::vector<std::shared_ptr<LightSource>>;

class Scene
{
public:
	Scene();

	Models& getModels();
	LightSources& getLightSources();
	shared_ptr<Camera>& getCamera();

	void setModels(const Models& models);
	void setLightSources(const LightSources& lightSources);
	void setCamera(const shared_ptr<Camera>& camera);

private:
	Models models;
	LightSources lightSources;
	shared_ptr<Camera> camera;
};

