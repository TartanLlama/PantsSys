project "PantsSim"
    kind "ConsoleApp"
    language "C++"
    flags {"C++14"}
    targetdir "../build/bin/%{cfg.buildcfg}"
    libdirs { os.getenv("SDL_LIB"), os.getenv("SDL_TTF_LIB"), os.getenv("SDL_IMAGE_LIB") }
    prebuildcommands {"cp -R PantsSim/assets/* build/bin/%{cfg.buildcfg}"}
    includedirs { "../Common/include", "include",
                  os.getenv("SDL_INCLUDE"), os.getenv("SDL_TTF_INCLUDE"), os.getenv("SDL_IMAGE_INCLUDE") }
    links { "SDL2", "SDL2_image", "SDL2_ttf" }
    files { "src/**.cpp", "include/**.hpp", "include/**.hpp", "../Common/src/**.cpp" }

project "PantsSimTest"
    kind "ConsoleApp"
    language "C++"
    flags {"C++14"}
    targetdir "../build/bin/%{cfg.buildcfg}"
    libdirs { os.getenv("SDL_LIB"), os.getenv("SDL_TTF_LIB"), os.getenv("SDL_IMAGE_LIB") }
    includedirs { "../Common/include", "include", "../ext/catch",
                  os.getenv("SDL_INCLUDE"), os.getenv("SDL_TTF_INCLUDE"), os.getenv("SDL_IMAGE_INCLUDE") }
    links { "SDL2", "SDL2_image", "SDL2_ttf" }
    files { "test/**.cpp", "src/**.cpp", "include/**.hpp", "../Common/include/**.hpp", "../Common/src/**.cpp" }
    excludes { "src/main.cpp" }
