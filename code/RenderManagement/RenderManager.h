#pragma once
#include "Scene/Scene.h"
#include "QGraphicsScene"

class RenderManager
{
public:
	void renderScene(QGraphicsScene *graphicsScene, const shared_ptr<Scene> &scene);

private:
};
