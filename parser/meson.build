parser_files = [
    'grammar.cpp',
    'reader.cpp',
    'pattern.cpp',
    'scope.cpp',
    'parse.cpp'
]

parser_lib = library(
    'parser_lib',
    parser_files,
    dependencies: [ jsoncpp_dep, onigmo_dep ]
    )

parser = executable(
    'parser',
    parser_files,
    'main.cpp',
    dependencies: [ jsoncpp_dep, onigmo_dep ]
    )

parser_dep = declare_dependency(
  include_directories : [ onigmo_inc, parser_inc ],
  link_with : parser_lib,
  sources : parser_files)