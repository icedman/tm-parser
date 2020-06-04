#include <QColor>
#include <QWidget>
#include <QDebug>
#include <QFile>
#include <QApplication>
#include <QRegularExpression>
#include <QStatusBar>

#include "settings.h"
#include "mainwindow.h"

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

bool theme_application(theme_ptr theme)
{
  QFile file("./style.css");        

  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    return true;
  }

  QString css = "";

  QColor bgColor;
  if (!theme_color(theme, "editor.background", bgColor)) {
      // return false;
  }
  QColor fgColor;
  if (!theme_color(theme, "editor.foreground", fgColor)) {
      // return false;
  }

  QColor treeBg = bgColor.darker(110);
  QColor treeFg = fgColor.darker(115);
  QColor itemActiveBg;
  QColor itemActiveFg;
  QColor itemHoverBg;
  QColor itemHoverFg;
  if (!theme_color(theme, "list.activeSelectionBackground", itemActiveBg)) {
    itemActiveBg = bgColor.light(150);
  }
  if (!theme_color(theme, "list.activeSelectionForeground", itemActiveFg)) {
    itemActiveFg = fgColor.light(150);
  }
  itemHoverBg = itemActiveBg.darker(120);
  itemHoverFg = itemActiveFg.darker(110);

  QRegularExpression regex(QStringLiteral("\\@[a-zA-Z]*"));

  QMap<QString, QString> colors;
  colors["@bgColor"] = bgColor.name();
  colors["@fgColor"] = fgColor.name();

  colors["@scrollBg"] = bgColor.darker(105).name();
  colors["@scrollHandleBg"] = bgColor.lighter(150).name();
  colors["@scrollHandleHoverBg"] = bgColor.lighter(180).name();
  colors["@splitterBg"] = bgColor.name();

  colors["@treeBg"] = treeBg.name();
  colors["@treeFg"] = treeFg.name();
  colors["@treeItemActiveBg"] = itemActiveBg.name();
  colors["@treeItemHoverBg"] = itemHoverBg.name();
  colors["@treeItemActiveFg"] = itemActiveFg.name();
  colors["@treeItemHoverFg"] = itemHoverFg.name();

  colors["@statusBg"] = treeBg.darker(150).name();
  colors["@statusFg"] = treeFg.name();

  QTextStream in(&file);
  while (!in.atEnd())
  {
    QString line = in.readLine();
    QRegularExpressionMatch match = regex.match(line);
    if (match.hasMatch()) {
      QString color = colors[match.captured()];
      line = line.replace(match.capturedStart(), match.capturedLength(), color);
    }
    // qDebug() << line;
    css += line + "\n";
  }
  file.close();
  
  qobject_cast<QApplication*>(QApplication::instance())->setStyleSheet(css);
  
  return true;
}