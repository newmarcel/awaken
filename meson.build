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
subdir('vendor')

dependencies = [
  dependency('CoreFoundation'),
  dependency('IOKit'),
]

install_headers('include/Awaken.hpp', subdir: 'awaken')

lib = static_library('awaken', project_sources,
  include_directories: includes,
  dependencies: dependencies,
  install: true)

exe = executable('awaken', 'main.cpp',
  include_directories: [include_directories('include'), include_directories('vendor')],
  link_with: [lib],
  install : true)
