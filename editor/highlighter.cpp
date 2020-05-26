#include "highlighter.h"
#include "reader.h"
#include "parse.h"

#include <iostream>

Highlighter::Highlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    Json::Value json_grammar = parse::loadJson("../test-cases/first-mate/fixtures/c.json");
    grammar = parse::parse_grammar(json_grammar);

    Json::Value json_theme = parse::loadJson("../test-cases/themes/dark_vs.json");
    std::cout << json_theme << std::endl;

    theme = parse_theme(json_theme);

    scope::scope_t scope("comment");
    style_t s = theme->styles_for_scope("scope");

    std::cout << s.foreground.red << ","
        << s.foreground.green << ","
        << s.foreground.blue << std::endl;
}

void Highlighter::highlightBlock(const QString& text)
{
    scope::scope_t source("source.c");
    std::map<size_t, scope::scope_t> scopes;
    scopes.emplace(0, source);

    std::string str;
    str.assign(text.toUtf8().constData(), text.length());

    const char* first = str.c_str();
    const char* last = first + text.length();

    printf("\n-------------\n%d\n%s\n", text.length(), first);

    parse::stack_ptr parser_state = grammar->seed();
    parser_state = parse::parse(first, last, parser_state, scopes, true);

    std::string prevScopeName;
    size_t si = 0;
    size_t n = 0;
    std::map<size_t, scope::scope_t>::iterator it = scopes.begin();
    while (it != scopes.end()) {
        n = it->first;
        scope::scope_t scope = it->second;
        std::string scopeName = scope.back().c_str();
        std::cout << n << ":" << scope.back().c_str() << std::endl;
        it++;

        if (n != 0) {
            style_t s = theme->styles_for_scope(prevScopeName);
            if (!s.foreground.is_blank()) {
                QColor clr(s.foreground.red * 255, s.foreground.green * 255, s.foreground.blue * 255, 255);
                setFormat(si, n-si, clr);
            }
        }

        prevScopeName = scopeName;
        si = n;
    }

    style_t s = theme->styles_for_scope(prevScopeName);
    if (!s.foreground.is_blank()) {
            QColor clr(s.foreground.red * 255, s.foreground.green * 255, s.foreground.blue * 255, 255);
            setFormat(si, n-si, clr);
        }
}