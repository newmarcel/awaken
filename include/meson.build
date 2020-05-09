project_headers += files(['Log.hpp', 'Awaken.hpp'])

config_file = configure_file(input: 'config.h.in', output: 'config.h', configuration: config)
project_headers += config_file

subdir('IOPowerAssertion')
subdir('Waiter')
