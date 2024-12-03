SET(SOURCES components/ui_comp_container1.c
    components/ui_comp.c
    screens/ui_Task_Page.c
    screens/ui_Focus_Page.c
    screens/ui_Home_Page.c
    ui_theme_manager.c
    ui_themes.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c)

add_library(ui ${SOURCES})
