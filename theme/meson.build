theme_files = [
    'theme.cpp',
    'util.cpp'
]

theme_lib = library(
    'theme_lib',
    theme_files,
    include_directories : [ onigmo_inc, theme_inc, scopes_inc, parser_inc ],
    dependencies: [ jsoncpp_dep, onigmo_dep, scopes_dep, parser_dep ]
    )

theme = executable(
    'theme',
    theme_files,
    'main.cpp',
    include_directories : [ onigmo_inc, theme_inc, scopes_inc, parser_inc ],
    dependencies: [ jsoncpp_dep, onigmo_dep, scopes_dep, parser_dep ]
    )

theme_dep = declare_dependency(
  include_directories : [ onigmo_inc, theme_inc, scopes_inc, parser_inc ],
  link_with : theme_lib,
  sources : theme_files)