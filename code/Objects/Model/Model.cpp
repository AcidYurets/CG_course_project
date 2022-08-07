#include "Model.h"

Model::Model() : details(nullptr), name("Nameless") { }

Model::Model(shared_ptr<PolygonalModelDetails> details, string name) : details(details), name(name) { }


