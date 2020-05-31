#ifndef SETTINGS_H
#define SETTINGS_H

#include "theme.h"

bool theme_color(theme_ptr theme, std::string name, QColor& qcolor);
bool theme_scrollbar(theme_ptr theme, std::string name, QWidget& scrollbar);
bool theme_splitter(theme_ptr theme, std::string name, QWidget& splitter);
bool theme_sidebar(theme_ptr theme, std::string name, QWidget& tree);
bool theme_statusbar(theme_ptr theme, std::string name, QWidget& statusbar);

#endif // SETTINGS_H