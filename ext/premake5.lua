project "fmt"
    kind "StaticLib"
    language "C++"
    flags {"C++14"}
    targetdir "build/lib/%{cfg.buildcfg}"
    includedirs {"fmt"}
    files {"fmt/fmt/**.h", "fmt/fmt/**.cc"}
    filter "platforms:Win64"
    	disablewarnings {"4127"}

