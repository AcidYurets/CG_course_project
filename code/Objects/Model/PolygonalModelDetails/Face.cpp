#include "Face.h"
#include "Exceptions/Exceptions.h"

Face::Face() : edges(0), vertices(0) { }

Face::Face(Edges edges, Vertices vertices) : edges(edges), vertices(vertices) {
	if (vertices.size() == 0) throw EmptyException(EXCEPCION_ARGS, "Vertices is empty");
}

Vertices Face::getVertices() {
	return vertices;
}
