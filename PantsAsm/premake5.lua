project "PantsAsm"
    kind "ConsoleApp"
    language "C++"
    flags {"C++14"}
    targetdir "build/bin/%{cfg.buildcfg}"
    includedirs { "../Common/include", "PantsAsm/include", "../ext/catch" }
    files { "src/**.cpp", "include/**.hpp", "../Common/include/**.hpp", "../Common/src/**.cpp"}
