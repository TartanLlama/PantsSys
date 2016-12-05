project "PantsC"
    kind "ConsoleApp"
    language "C++"
    flags {"C++14"}
    targetdir "../build/bin/%{cfg.buildcfg}"
    includedirs {"include", "../ext/fmt", "../ext", "../Common/include"}
    links {"fmt"}
    files {"include/**.hpp", "src/**.cpp"}

include "test/lexer"    
