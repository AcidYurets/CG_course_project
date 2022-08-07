#include "LightSource.h"

LightSource::LightSource() : pos(Vertex()), name("Nameless") { }

LightSource::LightSource(Vertex v, string name) : pos(v), name(name) { }
