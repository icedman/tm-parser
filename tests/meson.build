test_files = [
    'main.cpp',
]

executable(
    'tests',
    test_files,
    include_directories: [ parser_inc ],
    dependencies: [ jsoncpp_dep, parser_dep, onigmo_dep ]
)
