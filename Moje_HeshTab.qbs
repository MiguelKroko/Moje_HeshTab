import qbs

CppApplication {
    consoleApplication: true
    files: [
        "main.c",
        "table.c",
        "table.h",
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
