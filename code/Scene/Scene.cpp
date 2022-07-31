#include "Scene.h"

Models& Scene::getModels()
{
	return models;
}

LightSources& Scene::getLightSources()
{
	return lightSources;
}

Camera& Scene::getCamera()
{
	return camera;
}

void Scene::setModels(const Models& models)
{
	this->models = models;
}

void Scene::setLightSources(const LightSources& lightSources)
{
	this->lightSources = lightSources;
}

void Scene::setCamera(const Camera& camera)
{
	this->camera = camera;
}

