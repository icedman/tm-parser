project('text-edit', 'c', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++20']
)

parser_inc = include_directories(['parser'])
scopes_inc = include_directories(['scopes'])
theme_inc = include_directories(['theme'])
extension_inc = include_directories(['extensions'])
resources_inc = include_directories(['resources'])

subdir('scopes')
subdir('parser')
subdir('theme')
subdir('extensions')

textmate_files = [
'textmate.cpp',
'parser/grammar.cpp',
'parser/reader.cpp',
'parser/pattern.cpp',
'parser/parser.cpp',
'scopes/scope.cpp',
'scopes/types.cpp',
'scopes/parse.cpp',
'scopes/match.cpp',
'theme/theme.cpp',
'extensions/util.cpp',
'extensions/utf8.cpp',
'extensions/extensions.cpp',
'resources/grammars.cpp',
'resources/themes.cpp'
]

textmate_lib = static_library('textmate', textmate_files, include_directories
: [ textmate_inc, parser_inc, scopes_inc, theme_inc, extension_inc ], dependencies
: [ jsoncpp_dep, onigmo_dep ])

textmate_dep = declare_dependency(include_directories
: [textmate_inc], link_with
: textmate_lib)
