#include <QDebug>
#include <QDirIterator>
#include <iostream>

#include "extension.h"
#include "reader.h"
#include "stringop.h"

void load_extensions(const QString path, std::vector<Extension>& extensions)
{
    QDirIterator it(path);
    while (it.hasNext()) {
        QString extensionPath = it.next();
        QString package = extensionPath + "/package.json";

        // qDebug() << package;

        Extension ex = {
            .path = extensionPath,
        };

        ex.package = parse::loadJson(package.toStdString());
        if (ex.package.isObject()) {
            extensions.emplace_back(ex);
        }
    }
}

parse::grammar_ptr grammar_from_file(const QString path, std::vector<Extension>& extensions)
{
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
        if (!ext.package.isMember("contributes")) {
            continue;
        }
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
            for(int j = 0; j < exts.size(); j++) {
                Json::Value ex = exts[j];
                if (ex.asString().compare(suffix) == 0) {
                    resolvedExtension = ext;
                    resolvedLanguage = lang["id"].asString();
                    resolvedGrammars = contribs["grammars"];
                    break;
                }
            }
        
            if (!resolvedLanguage.empty()) break;
        }

        if (!resolvedLanguage.empty()) break;
    }

    if (!resolvedLanguage.empty()) {
        // std::cout << resolvedLanguage << std::endl;
        for(int i = 0; i < resolvedGrammars.size(); i++) {
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
                return grammar;

            }
        }
    }

    return grammar;
}