header_files = [
    'Awaken.hpp',
    'Waiter.hpp',
#    'DispatchWaiter.hpp', // don't use… yet?
    'ThreadWaiter.hpp',
    'IOPowerAssertion.hpp',
    'IOPowerSource.hpp',
]
project_headers += files(header_files)

config_file = configure_file(input: 'config.h.in', output: 'config.h', configuration: config)
project_headers += config_file
