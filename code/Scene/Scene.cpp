#include "Scene.h"

Scene::Scene() : models(0), lightSources(0), camera(nullptr) { }

Models& Scene::getModels() {
	return models;
}

LightSources& Scene::getLightSources() {
	return lightSources;
}

shared_ptr<Camera>& Scene::getCamera() {
	return camera;
}

void Scene::setModels(const Models& models) {
	this->models = models;
}

void Scene::setLightSources(const LightSources& lightSources) {
	this->lightSources = lightSources;
}

void Scene::setCamera(const shared_ptr<Camera>& camera) {
	this->camera = camera;
}

