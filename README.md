# Awaken 
A command line tool that prevents your Mac from going to sleep.

## Description
`awaken` is a re-implementation of the [`caffeinate`](https://web.archive.org/web/20140604153141/https://developer.apple.com/library/mac/documentation/Darwin/Reference/ManPages/man8/caffeinate.8.html) command line tool included in macOS. The project includes a `libawaken.a` static library to embed this behavior in other applications, like my macOS application **[KeepingYouAwake](https://github.com/newmarcel/KeepingYouAwake)**.

## Installation
`awaken` can be built and installed using the.[`meson`](https://mesonbuild.com) build system and a `clang` compiler that supports C++17 (e.g. from a recent Xcode version). For convenience an Xcode project file is also included.

```bash
meson build --buildtype=release
cd build
meson install
awaken —-help
```

## Usage
```
awaken - prevents your Mac from going to sleep.
Usage:
  ./build/awaken [OPTION...]

  -d, --display-sleep    prevent the display from idle sleeping
  -s, --system-sleep     prevent the system from idle sleeping (default:
                         true)
  -t, --timeout N        timeout in seconds until the sleep assertion expires
                         (default: 0)
  -b, --battery-level N  a minimum battery level on devices with a built-in
                         battery that causes the sleep assertion to expire
                         (e.g. 20 for <= 20% remaining battery). Values above 95
                         are unreliable and depend on the battery health.
                         (default: 0)

 Help options:
  -h, --help     print this help
  -v, --version  print version information
```

## Documentation
You can use [doxygen](http://www.doxygen.nl) (`brew install doxygen`) to generate the class documentation for this project.

## Why Is This Written in C++17 Instead of C, Objective-C or Swift?
I started learning modern C++ a while ago and have never published a project in pure C++17 before using a modern build system I wanted to try: `meson`.
I don’t rule out alternative implementations in `C`, `Rust` or `Swift` at all, it just depends on my own motivation to try these 😀.

Do you see any problems with my code or a potential optimization? Please create an issue or PR, I'm eager to learn 👍.

## License
All files in this project are covered by the [MIT license](http://opensource.org/licenses/MIT).
