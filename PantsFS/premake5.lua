project "mkfs.pantsfs"
    kind "ConsoleApp"
    language "C++"
    flags {"C++14"}
    targetdir "../build/bin/%{cfg.buildcfg}"
    includedirs {"include", "../ext/fmt"}
    links {"fmt"}
    files {"src/mkfs.cpp", "src/PantsFS.cpp"}
