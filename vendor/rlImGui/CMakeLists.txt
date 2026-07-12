add_library(rlimgui)

target_sources(rlimgui
    PRIVATE
        rlImGui.cpp

    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS 
		.
        FILES
            rlImGui.h
            rlImGuiColors.h
            imgui_impl_raylib.h
            extras/IconsFontAwesome6.h 
            extras/FA6FreeSolidFontData.h
)

target_link_libraries(rlimgui
    PUBLIC
        imgui
        raylib
)

