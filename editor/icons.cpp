#include <QDebug>
#include <QPainter>

#include <iostream>
#include <sstream>
#include <string>

#include "icons.h"

struct pixmap_wrapper_t {
    QPixmap img;
};
typedef std::shared_ptr<pixmap_wrapper_t> pixmap_wrapper_ptr;

std::string to_utf8(uint32_t cp)
{
    /*
    https://stackoverflow.com/questions/28534221/c-convert-asii-escaped-unicode-string-into-utf8-string/47734595

    if using C++11 or later, you can do this:
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.to_bytes( (char32_t)cp );

    Otherwise...
    */

    std::string result;

    int count;
    if (cp <= 0x007F)
        count = 1;
    else if (cp <= 0x07FF)
        count = 2;
    else if (cp <= 0xFFFF)
        count = 3;
    else if (cp <= 0x10FFFF)
        count = 4;
    else
        return result; // or throw an exception

    result.resize(count);

    if (count > 1) {
        for (int i = count - 1; i > 0; --i) {
            result[i] = (char)(0x80 | (cp & 0x3F));
            cp >>= 6;
        }

        for (int i = 0; i < count; ++i)
            cp |= (1 << (7 - i));
    }

    result[0] = (char)cp;

    return result;
}

std::string wstring_convert(std::string str)
{
    std::string::size_type startIdx = 0;
    do {
        startIdx = str.find("x\\", startIdx);
        if (startIdx == std::string::npos)
            break;
        std::string::size_type endIdx = str.length();
        //str.find_first_not_of("0123456789abcdefABCDEF", startIdx+2);
        if (endIdx == std::string::npos)
            break;
        std::string tmpStr = str.substr(startIdx + 2, endIdx - (startIdx + 2));
        std::istringstream iss(tmpStr);

        uint32_t cp;
        if (iss >> std::hex >> cp) {
            std::string utf8 = to_utf8(cp);
            str.replace(startIdx, 2 + tmpStr.length(), utf8);
            startIdx += utf8.length();
        } else
            startIdx += 2;
    } while (true);

    return str;
}

QPixmap image_from_icon(icon_theme_ptr icons, QString& filename, QString& suffix, std::vector<Extension>& _extensions)
{
    if (!icons) {
        return QPixmap();
    }

    std::string _suffix = suffix.toStdString();
    static std::map<std::string, pixmap_wrapper_ptr> cache;
    auto it = cache.find(_suffix);
    if (it != cache.end()) {
        // std::cout << "cached icon.." << std::endl;
        return it->second->img;
    }

    Json::Value definitions = icons->definition["iconDefinitions"];

    pixmap_wrapper_ptr px = std::make_shared<pixmap_wrapper_t>();

    std::cout << "generate icon.." << std::endl;

    std::string iconName;
    std::string fontCharacter = "x";
    std::string fontColor;

    Json::Value extensions = icons->definition["fileExtensions"];
    if (extensions.isMember(_suffix)) {
        iconName = extensions[_suffix].asString();
    }

    if (!iconName.length()) {
        Json::Value languageIds = icons->definition["languageIds"];

        std::string _fileName = "xx." + _suffix;
        language_info_ptr lang = language_from_file(_fileName.c_str(), _extensions);
        if (lang) {
            if (languageIds.isMember(lang->id)) {
                iconName = languageIds[lang->id].asString();
            }
        }

        if (!iconName.length()) {
            if (languageIds.isMember(_suffix)) {
                iconName = languageIds[_suffix].asString();
            }
        }
    }

    if (!iconName.length()) {
        iconName = icons->definition["file"].asString();
    }

    if (definitions.isMember(iconName)) {
        Json::Value def = definitions[iconName];
        fontCharacter += def["fontCharacter"].asString();
        fontCharacter += "x";
        fontCharacter = wstring_convert(fontCharacter);
        fontColor = def["fontColor"].asString();
        // std::cout << fontCharacter << std::endl;
    }

    int w = 16;
    int h = 16;
    px->img = QPixmap(w, h);
    px->img.fill(Qt::transparent);

    QPainter p(&px->img);

    if (fontCharacter.length()) {
        p.setPen(QColor(fontColor.c_str()));
        p.setFont(icons->font);
        int fh = QFontMetrics(icons->font).height();
        int fw = QFontMetrics(icons->font).width('w');
        p.drawText(2, -2, w, h, Qt::AlignRight, fontCharacter.c_str());
    }

    cache.emplace(_suffix, px);
    return px->img;
}