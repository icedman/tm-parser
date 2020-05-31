#include <QColor>
#include <QWidget>

#include "settings.h"

bool theme_color(theme_ptr theme, std::string name, QColor& qcolor)
{
    color_info_t clr;
    theme->theme_color(name, clr);
    if (clr.is_blank()) {
        return false;
    }
    qcolor.setRed(clr.red * 255);
    qcolor.setGreen(clr.green * 255);
    qcolor.setBlue(clr.blue * 255);
    return true;
}

bool theme_scrollbar(theme_ptr theme, std::string name, QWidget& scrollbar)
{
    QColor bgColor;

    if (!theme_color(theme, name, bgColor)) {
        return false;
    }

    scrollbar.setStyleSheet(" \
        QScrollBar:vertical { \
          width: 12px; \
          background: "
        + bgColor.darker(105).name() + "; \
          padding: 3px; \
          border: none; \
        } \
        QScrollBar::handle:vertical { \
          background: "
        + bgColor.lighter(150).name() + "; \
          border-radius: 3px; \
        } \
        QScrollBar::add-line:vertical { \
          border: none; \
          background: none; \
        } \
        QScrollBar::sub-line:vertical { \
          border: none; \
          background: none; \
        } \
        QScrollBar::handle:hover:vertical { \
          background: "
        + bgColor.lighter(180).name() + "; \
        } \
        QScrollBar:horizontal { \
          height: 12px; \
          background: "
        + bgColor.darker(105).name() + "; \
          padding: 3px; \
          border: none; \
        } \
        QScrollBar::handle:horizontal { \
          background: "
        + bgColor.lighter(150).name() + "; \
          border-radius: 3px; \
        } \
        QScrollBar::add-line:horizontal { \
          border: none; \
          background: none; \
        } \
        QScrollBar::sub-line:horizontal { \
          border: none; \
          background: none; \
        } \
        QScrollBar::handle:hover:horizontal { \
          background: "
        + bgColor.lighter(180).name() + "; \
        } \
        ");

    return true;
}

bool theme_splitter(theme_ptr theme, std::string name, QWidget& splitter)
{
    QColor bgColor;
    if (!theme_color(theme, name, bgColor)) {
        return false;
    }

    splitter.setStyleSheet("QSplitter::handle { background: red" + bgColor.name() + " }");
    return true;
}

bool theme_sidebar(theme_ptr theme, std::string name, QWidget& tree)
{
    QColor bgColor;
    QColor fgColor;
    if (!theme_color(theme, name + ".background", bgColor)) {
        return false;
    }
    if (!theme_color(theme, name + ".foreground", fgColor)) {
      theme_color(theme, "editor.foreground", fgColor);
      fgColor = fgColor.darker(110);
    }

    QColor activeBg;
    QColor activeFg;

    if (theme_color(theme, "list.activeSelectionBackground", activeBg)) {
        theme_color(theme, "list.activeSelectionForeground", activeFg);
    } else if (theme_color(theme, "list.focusBackground", activeBg)) {
        theme_color(theme, "list.focusForeground", activeFg);
    } else {
        activeBg = bgColor.lighter(150);
        activeFg = fgColor;
    }

    tree.setStyleSheet(" \
      QTreeView { \
       background: "
        + bgColor.name() + "; \
       color: "
        + fgColor.name() + "; \
       border: none; \
       show-decoration-selected: 0 \
      } \
      QTreeView::item { \
        border: none; \
        padding: 4px; \
        show-decoration-selected: 0 \
      } \
      QTreeView::item::active::hover, \
      QTreeView::item::active::!selected::focus, \
      QTreeView::item::active::selected { \
        background: "
        + activeBg.name() + "; \
        color: "
        + activeFg.name() + "; \
        border: none; \
      }\
      ");

    return true;
}

bool theme_statusbar(theme_ptr theme, std::string name, QWidget& statusbar)
{
    QColor bgColor;
    QColor fgColor;
    if (!theme_color(theme, name + ".background", bgColor)) {
        return false;
    }
    theme_color(theme, name + ".foreground", fgColor);
    statusbar.setStyleSheet(" \
        QStatusBar { \
          background: "
        + bgColor.name() + "; \
          color: "
        + fgColor.name() + "; \
        } \
        QSizeGrip { background: transparent } \
         ");

    return true;
}