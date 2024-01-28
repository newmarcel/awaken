// swift-tools-version:5.8

import PackageDescription

let cxxoptsVersion = "3.1.1"

let package = Package(
    name: "Awaken",
    products: [
        .library(name: "Awaken", targets: ["Awaken"]),
        .executable(name: "awaken", targets: ["awaken-cli"]),
    ],
    dependencies: [
    ],
    targets: [
        .target(
            name: "Awaken",
            dependencies: [],
            path: "src",
            exclude: [
                "meson.build",
                "Waiter/meson.build",
                "include/meson.build",
                "include/Awaken/meson.build",
                "include/Awaken/config.h.in",
            ],
            publicHeadersPath: "include",
            cxxSettings: [
                .headerSearchPath("include"),
            ]
        ),
        .executableTarget(
            name: "awaken-cli",
            dependencies: ["Awaken"],
            path: ".",
            exclude: [
                "build", "src", "include",
                "Makefile", "meson.build", "subprojects",
                "Doxyfile", "LICENSE", "CHANGELOG.md", "README.md",
            ],
            sources: [
                "main.cpp"
            ],
            cxxSettings: [
                .headerSearchPath("subprojects/cxxopts-\(cxxoptsVersion)/include")
            ],
            linkerSettings: [
                .linkedFramework("CoreFoundation"),
                .linkedFramework("IOKit"),
            ]
        )
    ],
    cxxLanguageStandard: .cxx17
)
