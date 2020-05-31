#ifndef EXTENSION_H
#define EXTENSION_H

#include "json/json.h"
#include <string>
#include <vector>

#include "grammar.h"
#include "theme.h"

class Extension {
public:
    QString path;
    Json::Value package;
};

struct language_info_t {
    Json::Value configuration;
    parse::grammar_ptr grammar;
};

void load_extensions(const QString path, std::vector<Extension>& extensions);
theme_ptr theme_from_name(const QString path, std::vector<Extension>& extensions);
// parse::grammar_ptr grammar_from_file(const QString path, std::vector<Extension>& extensions);
language_info_t language_from_file(const QString path, std::vector<Extension>& extensions);

#endif // EXTENSION_H