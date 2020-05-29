#include "highlighter.h"
#include "parse.h"
#include "reader.h"

#include <QTextDocument>
#include <iostream>

Highlighter::Highlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent),
        theme(0),
        grammar(0)
{
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

void Highlighter::setGrammar(parse::grammar_ptr _grammar)
{
    grammar = _grammar;
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

void Highlighter::setFormatFromStyle(size_t start, size_t length, style_t &style, const char* line, HighlightBlockData *blockData)
{
    if (style.bold == bool_true ||
        style.italic == bool_true ||
        style.underlined == bool_true ||
        style.strikethrough == bool_true ||
        !style.foreground.is_blank()) {

        QColor clr = QColor(style.foreground.red * 255, style.foreground.green * 255, style.foreground.blue * 255, 255);
        QTextCharFormat f;
        f.setFontWeight(style.bold == bool_true ? QFont::Medium : QFont::Normal);
        f.setFontItalic(style.italic == bool_true);
        f.setFontUnderline(style.underlined == bool_true);
        f.setFontStrikeOut(style.strikethrough == bool_true);
        f.setForeground(clr);

        if (!style.foreground.is_blank()) {
            int s = -1;
            for(int i=start; i<start+length; i++) {
                if (s == -1) {
                    if (line[i] != ' ') {
                        s = i;
                    }
                    continue;
                }
                if (line[i] == ' ' || i+1 == start+length) {
                    if (s != -1) {
                        SpanInfo span = {
                            .start = s,
                            .length = i-s,
                            .red = style.foreground.red * 255,
                            .green = style.foreground.green * 255,
                            .blue = style.foreground.blue * 255
                        };
                        blockData->spans.push_back(span);
                    }
                    s = -1;
                }
            }
        }

        setFormat(start, length, f);
    }
}

void Highlighter::highlightBlock(const QString& text)
{
    if (!theme || !grammar) {
        return;
    }
    
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

    // std::cout << str << "<<<<" << std::endl;

    parser_state = parse::parse(first, last, parser_state, scopes, firstLine);

    blockData->spans.clear();

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
            setFormatFromStyle(si, n - si, s, first, blockData);
        }

        prevScopeName = scopeName;
        si = n;
    }

    n = last - first;
    if (n > si) {
        style_t s = theme->styles_for_scope(prevScopeName);
        setFormatFromStyle(si, n - si, s, first, blockData);
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