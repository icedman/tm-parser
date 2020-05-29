#ifndef EXTENSION_H
#define EXTENSION_H

#include "json/json.h"

class Extension {
public:
    std::string path;
    Json::Value package;
};

#endif // EXTENSION_H