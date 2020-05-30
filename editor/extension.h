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

void load_extensions(const QString path, std::vector<Extension>& extensions);
parse::grammar_ptr grammar_from_file(const QString path, std::vector<Extension>& extensions);

bool theme_color(theme_ptr theme, std::string name, QColor& qcolor);

#endif // EXTENSION_H