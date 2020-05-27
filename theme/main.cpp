#include "json/json.h"
#include "reader.h"
#include "scope.h"
#include "theme.h"

#include <iostream>

int main(int argc, char** argv)
{
    // std::cout << "theme!" << std::endl;

    Json::Value root = parse::loadJson("test-cases/themes/dark_vs.json");
    // std::cout << root << std::endl;

    theme_ptr theme = parse_theme(root);

    scope::scope_t scope1("comment.block.c");
    style_t s1 = theme->styles_for_scope(scope1);
    std::cout << s1.foreground.red << ","
              << s1.foreground.green << ","
              << s1.foreground.blue
              << std::endl;

    scope::scope_t scope2("punctuation.whitespace.function.leading.c");
    style_t s2 = theme->styles_for_scope(scope2);

    std::cout << s2.foreground.red << ","
              << s2.foreground.green << ","
              << s2.foreground.blue
              << std::endl;

    // scope::scope_t scope3("variable.parameter.probably.c");
    scope::scope_t scope3("variable.parameter.probably.c");
    style_t s3 = theme->styles_for_scope(scope3);

    std::cout << s3.foreground.red << ","
              << s3.foreground.green << ","
              << s3.foreground.blue
              << std::endl;

    return 0;
}