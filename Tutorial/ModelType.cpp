#include "ModelType.h"

std::string getModelFile(ModelType type) {
    switch (type) {
    case ModelType::Cube:   return "./models/cube.obj";
    case ModelType::Sniper: return "./models/Sniper_Rifle.obj";
    default:                return "models/cube.obj"; 
    }
}
