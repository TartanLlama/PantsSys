project "fmt"
    kind "StaticLib"
    language "C++"
    flags {"C++14"}
    targetdir "build/lib/%{cfg.buildcfg}"
    includedirs {"fmt"}
	disablewarnings {"4127"}
    files {"fmt/fmt/**.h", "fmt/fmt/**.cc"}

