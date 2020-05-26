#include "theme.h"
#include "util.h"

#include <iostream>

std::vector<style_t> theme_t::global_styles(scope::scope_t const& scope)
{
    static struct {
        std::string name;
        color_info_t style_t::*field;
    } const colorKeys[] = {
        { "foreground", &style_t::foreground },
        { "background", &style_t::background },
        { "caret", &style_t::caret },
        { "selection", &style_t::selection },
        { "invisibles", &style_t::invisibles },
    };

    static struct {
        std::string name;
        bool_t style_t::*field;
    } const booleanKeys[] = {
        { "misspelled", &style_t::misspelled },
        { "bold", &style_t::bold },
        { "italic", &style_t::italic },
        { "underline", &style_t::underlined },
        { "strikethrough", &style_t::strikethrough },
    };

    std::vector<style_t> res;

    /*
    for(auto const& colorKey : colorKeys)
    {
        bundles::item_ptr item;
        plist::any_t const& value = bundles::value_for_setting(colorKey.name, scope, &item);
        if(item)
        {
            res.emplace_back(item->scope_selector());
            res.back().*(colorKey.field) = read_color(plist::get<std::string>(value));
        }
    }

    for(auto const& booleanKey : booleanKeys)
    {
        bundles::item_ptr item;
        plist::any_t const& value = bundles::value_for_setting(booleanKey.name, scope, &item);
        if(item)
        {
            res.emplace_back(item->scope_selector());
            res.back().*(booleanKey.field) = plist::is_true(value) ? bool_true : bool_false;
        }
    }

    bundles::item_ptr fontNameItem;
    plist::any_t const& fontNameValue = bundles::value_for_setting("fontName", scope, &fontNameItem);
    if(fontNameItem)
    {
        res.emplace_back(fontNameItem->scope_selector());
        res.back().font_name = plist::get<std::string>(fontNameValue);
    }

    bundles::item_ptr fontSizeItem;
    plist::any_t const& fontSizeValue = bundles::value_for_setting("fontSize", scope, &fontSizeItem);
    if(fontSizeItem)
    {
        res.emplace_back(fontSizeItem->scope_selector());
        res.back().font_size = read_font_size(plist::get<std::string>(fontSizeValue));
    }
    */

    return res;
}

theme_t::shared_styles_t::shared_styles_t(Json::Value const& themeItem)
{
    setup_styles(themeItem);
}

theme_t::shared_styles_t::~shared_styles_t() {}

void theme_t::shared_styles_t::setup_styles(Json::Value const& themeItem)
{
    _styles.clear();
    // _gutter_styles.clear();

    Json::Value settings = themeItem["settings"];
    for (int i = 0; i < settings.size(); i++) {
        Json::Value item = settings[i];
        Json::Value scope = settings[i]["scope"];
        // std::cout << item << std::endl;

        if (scope.isString()) {
            _styles.push_back(parse_styles(item, scope.asString()));
        } else if (scope.isArray()) {
            for (int j = 0; j < scope.size(); j++) {
                _styles.push_back(parse_styles(item, scope[j].asString()));
            }
        }
    }

    // if(!_color_space)
    //     _color_space = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);

    // =======================================
    // = Find “global” foreground/background =
    // =======================================

    // We assume that the first style is the unscoped root style

    // _foreground     = (_styles.empty() ? CGColorPtr() :
    // OakColorCreateFromThemeColor(_styles[0].foreground, _color_space)) ?:
    // CGColorPtr(CGColorCreate(_color_space, (CGFloat[4]){ 1, 1, 1, 1 }),
    // CGColorRelease); _background     = (_styles.empty() ? CGColorPtr() :
    // OakColorCreateFromThemeColor(_styles[0].background, _color_space)) ?:
    // CGColorPtr(CGColorCreate(_color_space, (CGFloat[4]){ 0, 0, 0, 1 }),
    // CGColorRelease); _is_dark        = color_is_dark(_background.get());
    // _is_transparent = CGColorGetAlpha(_background.get()) < 1;

    // =========================
    // = Default Gutter Styles =
    // =========================

    // ... omit
}

static void get_settings_string(Json::Value const& item, std::string& target)
{
    if (!item.isString()) {
        return;
    }
    target = item.asString();
}

static void get_settings_float(Json::Value const& item, float* target)
{
    if (!item.isString()) {
        return;
    }
    *target = parse_float(item.asString().c_str());
}

static void get_settings_color(Json::Value const& item, color_info_t* target)
{
    if (!item.isString()) {
        return;
    }
    uint32_t color;
    float rgba[4];

    parse_color(item.asString().c_str(), &color);
    color_to_rgba(rgba, color);

    target->red = rgba[0];
    target->green = rgba[1];
    target->blue = rgba[2];
    target->alpha = rgba[3];

    // std::cout<<item.asString()<<std::endl;
}

style_t theme_t::shared_styles_t::parse_styles(Json::Value const& item,
    std::string scope_selector)
{
    style_t res;

    Json::Value settings = item["settings"];

    if (!item.isObject()) {
        return res;
    }

    res.scope_selector = scope_selector;
    get_settings_string(settings["fontName"], res.font_name);
    get_settings_float(settings["fontSize"], &res.font_size);
    get_settings_color(settings["foreground"], &res.foreground);
    get_settings_color(settings["background"], &res.background);
    get_settings_color(settings["caret"], &res.caret);
    get_settings_color(settings["selection"], &res.selection);
    get_settings_color(settings["invisibles"], &res.invisibles);

    // bool flag;
    // res.misspelled = plist::get_key_path(plist, "settings.misspelled", flag) ?
    // (flag ? bool_true : bool_false) : bool_unset;

    if (settings["fontStyle"].isString()) {
        std::string fontStyle = settings["fontStyle"].asString();

        bool hasPlain = fontStyle.find("plain") != std::string::npos;
        bool hasBold = fontStyle.find("bold") != std::string::npos;
        bool hasItalic = fontStyle.find("italic") != std::string::npos;
        bool hasUnderline = fontStyle.find("underline") != std::string::npos;
        bool hasStrikethrough = fontStyle.find("strikethrough") != std::string::npos;

        res.bold = hasBold ? bool_true : (hasPlain ? bool_false : bool_unset);
        res.italic = hasItalic ? bool_true : (hasPlain ? bool_false : bool_unset);
        res.underlined = hasUnderline ? bool_true : (hasPlain ? bool_false : bool_unset);
        res.strikethrough = hasStrikethrough ? bool_true : (hasPlain ? bool_false : bool_unset);
    }

    return res;
}

theme_t::theme_t(Json::Value const& themeItem, std::string const& fontName,
    float fontSize)
    : _font_name(fontName)
    , _font_size(fontSize)
{
    _styles = find_shared_styles(themeItem);
    // _cache.set_empty_key(scope::scope_t{});
}

theme_t::shared_styles_ptr
theme_t::find_shared_styles(Json::Value const& themeItem)
{
    static std::map<std::string, theme_t::shared_styles_ptr> Cache;

    std::string const& uuid = themeItem["scopeName"].asString();
    auto theme = Cache.find(uuid);
    if (theme == Cache.end())
        theme = Cache.emplace(uuid, std::make_shared<shared_styles_t>(themeItem)).first;
    return theme->second;
}

// ==============
// = Public API =
// ==============

theme_ptr parse_theme(Json::Value& themeItem)
{
    static std::map<std::string, theme_ptr> Cache;

    std::string const& uuid = themeItem["scopeName"].asString();
    auto theme = Cache.find(uuid);
    if (theme == Cache.end())
        theme = Cache.emplace(uuid, std::make_shared<theme_t>(themeItem)).first;
    return theme->second;
}

static style_t _res;
style_t const& theme_t::styles_for_scope(scope::scope_t const& scope) const
{
    // implement cache!
    // auto styles = _cache.find(scope);
    // if(styles == _cache.end())

    std::multimap<double, style_t> ordering;
    for (auto const& it : global_styles(scope)) {
        double rank = 0;
        if (it.scope_selector.does_match(scope, &rank))
            ordering.emplace(rank, it);
    }

    for (auto const& it : _styles->_styles) {
        double rank = 0;
        if (it.scope_selector.does_match(scope, &rank))
            ordering.emplace(rank, it);
    }

    style_t base(scope::selector_t(), _font_name, _font_size);
    for (auto const& it : ordering)
        base += it.second;

    style_t res(
        base.font_name,
        base.font_size,
        base.foreground,
        base.background,
        base.caret,
        base.selection,
        base.underlined,
        base.strikethrough,
        base.misspelled);

    _res = res;
    return _res;
}

style_t& style_t::operator+=(style_t const& rhs)
{
    font_name  = rhs.font_name != NULL_STR    ? rhs.font_name : font_name;
    // font_size  = rhs.font_size > 0            ? rhs.font_size : font_size * fabs(rhs.font_size);

    foreground = rhs.foreground.is_blank()    ? foreground : rhs.foreground;
    background = rhs.background.is_blank()    ? background : rhs.background;
    // background = rhs.background.is_blank()    ? background : blend(background, rhs.background);
    caret      = rhs.caret.is_blank()         ? caret      : rhs.caret;
    selection  = rhs.selection.is_blank()     ? selection  : rhs.selection;
    invisibles = rhs.invisibles.is_blank()    ? invisibles : rhs.invisibles;

    bold          = rhs.bold          == bool_unset ? bold          : rhs.bold;
    italic        = rhs.italic        == bool_unset ? italic        : rhs.italic;
    underlined    = rhs.underlined    == bool_unset ? underlined    : rhs.underlined;
    strikethrough = rhs.strikethrough == bool_unset ? strikethrough : rhs.strikethrough;
    misspelled    = rhs.misspelled    == bool_unset ? misspelled    : rhs.misspelled;

    return *this;
}
