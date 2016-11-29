project "PantsAsm"
    kind "ConsoleApp"
    language "C++"
    flags {"C++14"}
    targetdir "../build/bin/%{cfg.buildcfg}"
    includedirs { "../Common/include", "../ext/fmt", "PantsAsm/include", "../ext/catch" }
    links {"fmt"}
    files { "src/**.cpp", "include/**.hpp", "../Common/include/**.hpp", "../Common/src/**.cpp"}
