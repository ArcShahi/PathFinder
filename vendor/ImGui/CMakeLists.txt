cmake_minimum_required(VERSION 4.2)

add_library(imgui)

target_sources(imgui
    PRIVATE
        imgui.cpp
        imgui_draw.cpp
        imgui_tables.cpp
        imgui_widgets.cpp

    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS .
        FILES
            imgui.h
            imgui_internal.h
            imconfig.h
            imstb_rectpack.h
            imstb_textedit.h
            imstb_truetype.h
)



target_compile_features(imgui PRIVATE cxx_std_17)


