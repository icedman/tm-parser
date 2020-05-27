#include "highlighter.h"
#include "reader.h"
#include "parse.h"

#include <iostream>

Highlighter::Highlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    Json::Value json_grammar = parse::loadJson("../extensions/cpp/syntaxes/c.tmLanguage.json");
    grammar = parse::parse_grammar(json_grammar);
}

void Highlighter::setTheme(theme_ptr _theme)
{
    theme = _theme;
    
    // scope::scope_t scope("comment");
    // style_t s = theme->styles_for_scope("scope");

    // std::cout << s.foreground.red << ","
    //     << s.foreground.green << ","
    //     << s.foreground.blue << std::endl;
}

void dump_color(color_info_t clr) {
    std::cout << " r:" << clr.red <<
    " g:" << clr.green <<
    " b:" << clr.blue;
}

void Highlighter::highlightBlock(const QString& text)
{
    std::map<size_t, scope::scope_t> scopes;


    HighlightBlockData *blockData = reinterpret_cast<HighlightBlockData*>(currentBlock().userData());
    if (!blockData) {
        blockData = new HighlightBlockData;
    }

    std::string str;
    str.assign(text.toUtf8().constData(), text.length());
    str += "\n";

    bool firstLine = true;
    parse::stack_ptr parser_state = NULL;

    QTextBlock prevBlock = currentBlock().previous();
    HighlightBlockData *prevBlockData = reinterpret_cast<HighlightBlockData*>(prevBlock.userData());
    if (prevBlockData) {
        parser_state = prevBlockData->parser_state;
        firstLine = !(parser_state != NULL);
    }

    if (!parser_state) {
        parser_state= grammar->seed();
        firstLine = true;
    }

    const char* first = str.c_str();
    const char* last = first + text.length()+1;

    // printf("\n-------------\n%d\n%s\n", text.length(), first);


    parser_state = parse::parse(first, last, parser_state, scopes, firstLine);

    // std::cout << "first: " << firstLine << std::endl;

    std::string prevScopeName;
    size_t si = 0;
    size_t n = 0;
    std::map<size_t, scope::scope_t>::iterator it = scopes.begin();
    while (it != scopes.end()) {
        n = it->first;
        scope::scope_t scope = it->second;
        std::string scopeName = scope.back().c_str();
        it++;

        if (n > si) {
            style_t s = theme->styles_for_scope(prevScopeName);
            if (!s.foreground.is_blank()) {
                // std::cout << si << "-" << n << ":" << prevScopeName << "\t";
                // dump_color(s.foreground);
                // std::cout << std::endl;
                QColor clr(s.foreground.red * 255, s.foreground.green * 255, s.foreground.blue * 255, 255);
                setFormat(si, n-si, clr);
            }
        }

        prevScopeName = scopeName;
        si = n;
    }

    n = last - first;
    if (n > si) {
        style_t s = theme->styles_for_scope(prevScopeName);
        if (!s.foreground.is_blank()) {
                // std::cout << si << "-" << n << ":" << prevScopeName << "\t";
                // dump_color(s.foreground);
                // std::cout << std::endl;
            QColor clr(s.foreground.red * 255, s.foreground.green * 255, s.foreground.blue * 255, 255);
            setFormat(si, n-si, clr);
        }
    }

    blockData->parser_state = parser_state;
    currentBlock().setUserData(blockData);
}