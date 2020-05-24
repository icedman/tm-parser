scopes_files = [
    'scope.cpp',
    'types.cpp',
    'parse.cpp',
    'main.cpp'
]

scopes_lib = library(
    'scopes_lib',
    scopes_files,
    dependencies: [ jsoncpp_dep, onigmo_dep ]
    )

scopes = executable(
    'scopes',
    'match.cpp',
    scopes_files,
    'main.cpp',
    dependencies: [ jsoncpp_dep, onigmo_dep ]
    )

scopes_dep = declare_dependency(
  include_directories : [ onigmo_inc, scopes_inc ],
  link_with : scopes_lib,
  sources : scopes_files)