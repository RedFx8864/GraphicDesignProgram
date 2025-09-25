#ifndef MODEL_TYPE_H
#define MODEL_TYPE_H

#include <string>

enum class ModelType {
    Cube,
    Sniper,
};

// Returns the .obj file path for a given model type
std::string getModelFile(ModelType type);

#endif // MODEL_TYPE_H
