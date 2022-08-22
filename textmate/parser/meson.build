parser_files = [
    'grammar.cpp',
    'reader.cpp',
    'pattern.cpp',
    'parser.cpp'
]

parser_lib = static_library('parser',
    parser_files,
    include_directories : [ extension_inc, theme_inc ],
    dependencies : [ jsoncpp_dep, onigmo_dep, scopes_dep ],
    cpp_args : '-Dload_plist_or_json=loadJson'
)

parser = executable('parser',
    parser_files,
    'main.cpp',
    include_directories : [ extension_inc, theme_inc ],
    dependencies : [ jsoncpp_dep, onigmo_dep, scopes_dep ],
    cpp_args : '-Dload_plist_or_json=loadJson'
)

parser_dep = declare_dependency(
    include_directories : [ parser_inc, scopes_inc, extension_inc ],
    link_with: parser_lib)