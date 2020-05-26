#include "json/json.h"
#include "reader.h"
#include "theme.h"
#include "scope.h"

#include <iostream>

int main(int argc, char** argv)
{
    // std::cout << "theme!" << std::endl;

    Json::Value root = parse::loadJson("test-cases/themes/light_vs.json");
    // std::cout << root << std::endl;
    
    theme_ptr theme = parse_theme(root);
    scope::scope_t scope("comment");
    
    style_t s = theme->styles_for_scope(scope);

    std::cout << s.foreground.red << ","
        << s.foreground.green << ","
        << s.foreground.blue
        << std::endl;
    return 0;
}