#ifndef EXTENSION_H
#define EXTENSION_H

#include <QFont>

#include <memory>
#include <string>
#include <vector>

#include "grammar.h"
#include "theme.h"
#include "json/json.h"

class Extension {
public:
    QString path;
    Json::Value package;
};

struct language_info_t {

    std::string id;

    bool blockComment;
    std::string blockCommentStart;
    std::string blockCommentEnd;

    bool brackets;
    std::vector<std::string> bracketOpen;
    std::vector<std::string> bracketClose;

    parse::grammar_ptr grammar;
};

struct icon_theme_t {
    QFont font;
    std::string font_path;
    Json::Value definition;
};

typedef std::shared_ptr<language_info_t> language_info_ptr;
typedef std::shared_ptr<icon_theme_t> icon_theme_ptr;

void load_settings(const QString path, Json::Value& settings);
void load_extensions(const QString path, std::vector<Extension>& extensions);
icon_theme_ptr icon_theme_from_name(const QString path, std::vector<Extension>& extensions);
theme_ptr theme_from_name(const QString path, std::vector<Extension>& extensions);
language_info_ptr language_from_file(const QString path, std::vector<Extension>& extensions);

#endif // EXTENSION_H