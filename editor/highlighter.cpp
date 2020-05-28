#include "highlighter.h"
#include "parse.h"
#include "reader.h"

#include <QTextDocument>
#include <iostream>

Highlighter::Highlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    Json::Value json_grammar = parse::loadJson("../extensions/cpp/syntaxes/c.tmLanguage.json");
    grammar = parse::parse_grammar(json_grammar);

    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(onUpdate()));
    updateTimer.start(50);
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

void Highlighter::setDeferRendering(bool defer)
{
    deferRendering = defer;
}

void dump_color(color_info_t clr)
{
    std::cout << " r:" << (int)(clr.red * 255)
        << " g:" << (int)(clr.green * 255)
        << " b:" << (int)(clr.blue * 255);
}

void Highlighter::setFormatFromStyle(size_t start, size_t length, style_t &style)
{
    if (style.bold == bool_true ||
        style.italic == bool_true ||
        style.underlined == bool_true ||
        style.strikethrough == bool_true ||
        !style.foreground.is_blank()) {
        QTextCharFormat f;
        f.setFontWeight(style.bold == bool_true ? QFont::Medium : QFont::Normal);
        f.setFontItalic(style.italic == bool_true);
        f.setFontUnderline(style.underlined == bool_true);
        f.setFontStrikeOut(style.strikethrough == bool_true);
        f.setForeground(QColor(style.foreground.red * 255, style.foreground.green * 255, style.foreground.blue * 255, 255));
        setFormat(start, length, f);
    }
}

void Highlighter::highlightBlock(const QString& text)
{
    std::map<size_t, scope::scope_t> scopes;

    bool firstLine = true;
    parse::stack_ptr parser_state = NULL;

    //----------------------
    // gather block data
    //----------------------
    HighlightBlockData* blockData = reinterpret_cast<HighlightBlockData*>(currentBlock().userData());
    if (!blockData) {
        if (deferRendering) {
            return;
        }
        blockData = new HighlightBlockData;
    }

    QTextBlock prevBlock = currentBlock().previous();
    HighlightBlockData* prevBlockData = reinterpret_cast<HighlightBlockData*>(prevBlock.userData());
    if (prevBlockData) {
        parser_state = prevBlockData->parser_state;
        if (parser_state->rule) {
            blockData->last_prev_block_rule = parser_state->rule->rule_id;
        }
        firstLine = !(parser_state != NULL);
    }

    if (!parser_state) {
        parser_state = grammar->seed();
        firstLine = true;
    }

    //----------------------
    // parse the line
    //----------------------
    std::string str;
    str.assign(text.toUtf8().constData(), text.length());
    str += "\n";

    const char* first = str.c_str();
    const char* last = first + text.length() + 1;

    std::cout << str << "<<<<" << std::endl;

    parser_state = parse::parse(first, last, parser_state, scopes, firstLine);

    std::string prevScopeName;
    size_t si = 0;
    size_t n = 0;    
    std::map<size_t, scope::scope_t>::iterator it = scopes.begin();
    while (it != scopes.end()) {
        n = it->first;
        scope::scope_t scope = it->second;
        std::string scopeName = to_s(scope);
        it++;

        if (n > si) {
            style_t s = theme->styles_for_scope(prevScopeName);
            setFormatFromStyle(si, n - si, s);
        }

        prevScopeName = scopeName;
        si = n;
    }

    n = last - first;
    if (n > si) {
        style_t s = theme->styles_for_scope(prevScopeName);
        setFormatFromStyle(si, n - si, s);
    }

    blockData->parser_state = parser_state;
    blockData->dirty = false;
    currentBlock().setUserData(blockData);

    //----------------------
    // mark next block for highlight
    // .. if necessary
    //----------------------
    if (parser_state->rule) {
        QTextBlock next = currentBlock().next();
        if (next.isValid()) {
            HighlightBlockData* blockData = reinterpret_cast<HighlightBlockData*>(next.userData());
            if (blockData && parser_state->rule->rule_id != blockData->last_prev_block_rule) {
                blockData->dirty = true;
            }   
        }
    }
}

void Highlighter::onUpdate()
{
    QTextDocument *doc = document();

    int rendered = 0;
    QTextBlock block = doc->begin();
    while(block.isValid() && rendered<200) {
        HighlightBlockData* blockData = reinterpret_cast<HighlightBlockData*>(block.userData());
        if (blockData && blockData->dirty) {
            rendered++;
            rehighlightBlock(block);
        }
        block = block.next();
    }
}