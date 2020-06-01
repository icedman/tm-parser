#ifndef ICONS_H
#define ICONS_H

#include <QPixmap>

#include "extension.h"

QPixmap image_from_icon(icon_theme_ptr icons, QString& filename, QString& suffix, std::vector<Extension>& extensions);

#endif // ICONS_H