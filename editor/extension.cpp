#include <QColor>
#include <QDebug>
#include <QDirIterator>
#include <iostream>

#include "extension.h"
#include "reader.h"
#include "stringop.h"

#include "json/json.h"

void load_extensions(const QString path, std::vector<Extension>& extensions)
{
    // Json::Value contribs
    QDirIterator it(path);
    while (it.hasNext()) {
        QString extensionPath = it.next();
        QString package = extensionPath + "/package.json";

        // qDebug() << package;
        Extension ex = {
            .path = extensionPath,
        };

        ex.package = parse::loadJson(package.toStdString());
        if (!ex.package.isObject()) {
            continue;
        }

        bool append = false;
        if (ex.package.isMember("contributes")) {
            std::vector<std::string> keys = ex.package["contributes"].getMemberNames();
            std::vector<std::string>::iterator c_it = keys.begin();
            while (c_it != keys.end()) {
                std::string name = *c_it;

                if (name == "themes" || name == "iconThemes" || name == "languages") {
                    append = true;

                    // Json::Value obj;
                    // obj["name"] = ex.package["name"];
                    // obj["package"] = package.toStdString();
                    // contribs[name].append(obj);

                    break;
                }

                c_it++;
            }
        }

        if (append) {
            // std::cout << ex.package["name"].asString() << std::endl;

            qDebug() << package;

            extensions.emplace_back(ex);
        }
    }

    // std::cout << contribs;
}

// parse::grammar_ptr grammar_from_file(const QString path, std::vector<Extension>& extensions)

language_info_t language_from_file(const QString path, std::vector<Extension>& extensions)
{
    language_info_t lang;

    parse::grammar_ptr grammar;

    QFileInfo info(path);
    std::string suffix = ".";
    suffix += info.suffix().toStdString();

    std::cout << suffix << std::endl;

    // check cache
    Extension resolvedExtension;
    std::string resolvedLanguage;
    Json::Value resolvedGrammars;

    for (auto ext : extensions) {
        Json::Value contribs = ext.package["contributes"];
        if (!contribs.isMember("languages") || !contribs.isMember("grammars")) {
            continue;
        }
        Json::Value langs = contribs["languages"];
        for (int i = 0; i < langs.size(); i++) {
            Json::Value lang = langs[i];
            if (!lang.isMember("extensions") || !lang.isMember("id")) {
                continue;
            }

            Json::Value exts = lang["extensions"];
            for (int j = 0; j < exts.size(); j++) {
                Json::Value ex = exts[j];
                if (ex.asString().compare(suffix) == 0) {
                    resolvedExtension = ext;
                    resolvedLanguage = lang["id"].asString();
                    resolvedGrammars = contribs["grammars"];
                    break;
                }
            }

            if (!resolvedLanguage.empty())
                break;
        }

        if (!resolvedLanguage.empty())
            break;
    }

    if (!resolvedLanguage.empty()) {
        // std::cout << resolvedLanguage << std::endl;
        for (int i = 0; i < resolvedGrammars.size(); i++) {
            Json::Value g = resolvedGrammars[i];
            if (!g.isMember("language")) {
                continue;
            }

            if (g["language"].asString().compare(resolvedLanguage) == 0) {
                // todo .. enable caching..
                QString path = QDir(resolvedExtension.path).filePath(g["path"].asString().c_str());
                Json::Value root = parse::loadJson(path.toStdString());
                grammar = parse::parse_grammar(root);
                qDebug() << "grammar matched";

                lang.grammar = grammar;
                return lang;
                // return grammar;
            }
        }
    }

    return lang;
}

theme_ptr theme_from_name(const QString path, std::vector<Extension>& extensions)
{
    std::string theme_path = path.toStdString();
    bool found = false;
    for (auto ext : extensions) {
        Json::Value contribs = ext.package["contributes"];
        if (!contribs.isMember("themes")) {
            continue;
        }

        Json::Value themes = contribs["themes"];
        for (int i = 0; i < themes.size(); i++) {
            Json::Value theme = themes[i];
            if (theme["id"].asString() == theme_path || theme["label"].asString() == theme_path) {
                theme_path = ext.path.toStdString() + "/" + theme["path"].asString();
                // std::cout << ext.path.toStdString() << "..." << std::endl;
                std::cout << theme_path << std::endl;
                found = true;
                break;
            }
        }

        if (found) {
            break;
        }
    }

    Json::Value json = parse::loadJson(theme_path); // "./dracula-soft.json");
    theme_ptr theme = parse_theme(json);
    ;
    return theme;
}