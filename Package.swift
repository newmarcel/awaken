// swift-tools-version:5.5

import PackageDescription

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
                "vendor/meson.build",
                "vendor/cxxopts/meson.build",
                "vendor/cxxopts/LICENSE",
                "vendor/cxxopts/README.md",
                "vendor/cxxopts/CHANGELOG.md",
                "build", "src", "include",
                "Makefile", "meson.build", "Doxyfile",
                "LICENSE", "CHANGELOG.md", "README.md",
            ],
            sources: [
                "main.cpp"
            ],
            cxxSettings: [
                .headerSearchPath("vendor")
            ],
            linkerSettings: [
                .linkedFramework("CoreFoundation"),
                .linkedFramework("IOKit"),
            ]
        )
    ],
    cxxLanguageStandard: .cxx17
)
