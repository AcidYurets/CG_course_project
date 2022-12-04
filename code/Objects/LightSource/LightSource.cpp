#include "LightSource.h"

LightSource::LightSource() : pos(Vertex()), name("Nameless") { }

LightSource::LightSource(Vertex v, string name) : pos(v), name(name) { }

Vertex LightSource::getPosition() {
    return pos;
}

string LightSource::getName() {
    return name;
}

void LightSource::transform(Vector3d move_params, Vector3d scale_params, Vector3d rotate_params) {
    pos.transform(move_params, scale_params, rotate_params);
}
