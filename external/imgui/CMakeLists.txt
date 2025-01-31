# Define ImGui library
add_library(imgui
    "imgui.h"
    "imgui.cpp"
    "imconfig.h"
    
    
    "imgui_internal.h"
    "imstb_rectpack.h"
    "imstb_textedit.h"
    "imstb_truetype.h"
    "imgui_tables.cpp"
    "imgui_widgets.cpp"

    "imgui_impl_glfw.h"
    "imgui_impl_glfw.cpp"
    "imgui_impl_opengl3.h"
    "imgui_impl_opengl3.cpp"
)

target_link_libraries(imgui PRIVATE
    glad
    glfw
)

