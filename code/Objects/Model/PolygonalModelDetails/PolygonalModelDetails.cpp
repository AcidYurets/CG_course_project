#include "PolygonalModelDetails.h"

PolygonalModelDetails::PolygonalModelDetails() : vertices(0), edges(0), faces(0), center(0, 0, 0) { }

Vertices PolygonalModelDetails::getVertices() {
	return vertices;
}

Edges PolygonalModelDetails::getEdges() {
	return edges;
}

Faces PolygonalModelDetails::getFaces() {
	return faces;
}

void PolygonalModelDetails::addVertex(std::shared_ptr<Vertex> vert) {
	vertices.push_back(vert);
}

void PolygonalModelDetails::addEdge(std::shared_ptr<Edge> edge) {
	edges.push_back(edge);
}

void PolygonalModelDetails::addFace(std::shared_ptr<Face> face) {
	faces.push_back(face);
}

