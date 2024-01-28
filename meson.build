project('awaken', 'cpp',
  version : '1.2.0',
  default_options : ['warning_level=3', 'c_std=c11', 'cpp_std=c++17'])

config = configuration_data()
config.set('version', meson.project_version())

project_sources = []
project_headers = []
includes = [include_directories('include')]

subdir('include')
subdir('src')

cxxopts = dependency('cxxopts')

dependencies = [
  dependency('CoreFoundation'),
  dependency('IOKit'),
]

install_headers(project_headers, subdir: 'Awaken')

lib = static_library('awaken', project_sources,
  include_directories: includes,
  dependencies: dependencies,
  install: true)

exe = executable('awaken', 'main.cpp',
  include_directories: include_directories('include'),
  dependencies: [cxxopts],
  link_with: [lib],
  install : true)
