#ifndef EXTENSION_H
#define EXTENSION_H

#include "json/json.h"
#include <string>
#include <vector>
#include <memory>

#include "grammar.h"
#include "theme.h"

class Extension {
public:
    QString path;
    Json::Value package;
};

struct language_info_t {

    bool blockComment;
    std::string blockCommentStart;
    std::string blockCommentEnd;

    bool brackets;
    std::vector<std::string> bracketOpen;
    std::vector<std::string> bracketClose;

    parse::grammar_ptr grammar;
};

typedef std::shared_ptr<language_info_t> language_info_ptr;

void load_settings(const QString path, Json::Value& settings);
void load_extensions(const QString path, std::vector<Extension>& extensions);
theme_ptr theme_from_name(const QString path, std::vector<Extension>& extensions);
language_info_ptr language_from_file(const QString path, std::vector<Extension>& extensions);

#endif // EXTENSION_H