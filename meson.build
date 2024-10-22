project(
  'awaken',
  'cpp',
  version : '1.2.0',
  meson_version: '>=1.1.0',
  default_options : ['warning_level=3', 'cpp_std=c++23', 'default_library=static']
)

config = configuration_data()
config.set('version', meson.project_version())

project_sources = []
project_headers = []
includes = [include_directories('include')]

subdir('include')
subdir('src')

cxxopts_dep = dependency('cxxopts')

dependencies = [
  dependency('CoreFoundation'),
  dependency('IOKit'),
]

install_headers(project_headers, subdir: 'Awaken')

lib = library(
  'libAwaken',
  project_sources,
  include_directories: includes,
  dependencies: dependencies,
  install: true
)

exe = executable(
  'awaken',
  'main.cpp',
  include_directories: include_directories('include'),
  dependencies: cxxopts_dep,
  link_with: lib,
  install : true
)
