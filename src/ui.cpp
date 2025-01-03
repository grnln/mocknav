#include "../imgui/imgui.h"
#include "../imgui/misc/cpp/imgui_stdlib.h"
#include "../imgui/backends/imgui_impl_sdl2.h"
#include "../imgui/backends/imgui_impl_sdlrenderer2.h"

#include "types.h"
#include "context.h"
#include "screen.h"

/* Focus variables */
static bool sidebar_focus;
static s32 bg_x, bg_y, btn_w, btn_h;
static std::string name, path, screen;

void sidebar_render(void) {    
    if (ImGui::Begin("Page", NULL, ImGuiWindowFlags_NoCollapse)) {
        sidebar_focus = ImGui::IsWindowFocused();
	
        /* Fix window */
        ImGui::SetWindowPos(ImVec2(4 * context.window_w / 6, 0));
	    ImGui::SetWindowSize(ImVec2(2 * context.window_w / 6, context.window_h));
    
        /* General page info. */
        ImGui::InputText("Page name", &context.page_info.name);
        ImGui::InputText("Page icon", &context.page_info.icon);
    
        if (ImGui::Button("Save")) {
            page_save();
        }
    
        /* Screens */
        ImGui::NewLine();
    
        if (ImGui::BeginListBox("Screens")) {
            for (s32 i = 0; i < context.page_info.screens.size(); i++) {
                std::string label;

                label = std::to_string(i) + " - ";
                label += context.page_info.screens[i].name;

                if (ImGui::Selectable(label.c_str())) {
                    context.current_button = -1;
                    context.current_screen = i;
                }
            }
            ImGui::EndListBox();
        }

        if (ImGui::Button("+##1")) {
            ImGui::OpenPopup("Create screen");
        }
        
        if (ImGui::BeginPopupModal("Create screen", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            screen_t screen;

            INPUT_LOCK();
            ImGui::InputText("Filename", &name);
            ImGui::InputText("Mockup", &path);

            if (ImGui::Button("Create##1")) {            
                screen = screen_create(name, path);
                context.page_info.screens.push_back(screen);
                
                INPUT_UNLOCK();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        ImGui::SameLine();
        ImGui::Text("Add screen");
    
        ImGui::NewLine();
        ImGui::Text("Current screen: %i", context.current_screen);
        ImGui::NewLine();

        if (context.current_screen >= 0) {
            ImGui::InputText("Name##Input", &(context.page_info.screens[context.current_screen].name));
            
            if (ImGui::InputText("Mockup##Input", &(context.page_info.screens[context.current_screen].texture_path), ImGuiInputTextFlags_EnterReturnsTrue)) {
                texture_free(&context.page_info.screens[context.current_screen].texture);
                context.page_info.screens[context.current_screen].texture = texture_load(context.page_info.screens[context.current_screen].texture_path.c_str());
            }

            if (ImGui::Button("Delete##Screen")) {
                screen_free(&context.page_info.screens[context.current_screen]);
                context.page_info.screens.erase(context.page_info.screens.begin() + context.current_screen);
                context.current_screen = -1;
                context.current_button = -1;
            }
        }
        ImGui::NewLine();

        if (context.current_screen >= 0) {
            if (ImGui::BeginListBox("Buttons")) {
                for (u32 i = 0; i < context.page_info.screens[context.current_screen].buttons.size(); i++) {
                    std::string label;

                    label = std::to_string(i) + " - To " + context.page_info.screens[context.current_screen].buttons[i].screen;

                    if (ImGui::Selectable(label.c_str())) {
                        context.current_button = i;
                    }
                }
                ImGui::EndListBox();
            }
        
            if (ImGui::Button("+##2")) {
                ImGui::OpenPopup("Create button");
            }
        
            if (ImGui::BeginPopupModal("Create button", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                button_t button;

                INPUT_LOCK();
                ImGui::InputText("Screen##Input", &screen);
                ImGui::InputInt("Width", &btn_w);
                ImGui::InputInt("Height", &btn_h);
            
                if (ImGui::Button("Create##1")) {            
                    screen_add_button(
                        &(context.page_info.screens[context.current_screen]),
                        0,
                        0,
                        btn_w,
                        btn_h,
                        bg_x,
                        bg_y,
                        screen
                    );
                    INPUT_UNLOCK();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
            ImGui::SameLine();
            ImGui::Text("Add button");
        }
        ImGui::NewLine();
        ImGui::Text("Current button: %i", context.current_button);
        ImGui::NewLine();

        if (context.current_button >= 0) {
            ImGui::InputText("Screen##Input2", &(context.page_info.screens[context.current_screen].buttons[context.current_button].screen));
            ImGui::InputInt("X", &(context.page_info.screens[context.current_screen].buttons[context.current_button].x));
            ImGui::InputInt("Y", &(context.page_info.screens[context.current_screen].buttons[context.current_button].y));
            ImGui::InputInt("W", &(context.page_info.screens[context.current_screen].buttons[context.current_button].w));
            ImGui::InputInt("H", &(context.page_info.screens[context.current_screen].buttons[context.current_button].h));
            
            if (ImGui::Button("Delete##Button")) {
                context.page_info.screens[context.current_screen].buttons.erase(
                    context.page_info.screens[context.current_screen].buttons.begin() + context.current_button
                );
                context.current_button = -1;
            }
        }
        ImGui::NewLine();

        ImGui::InputText("Output Folder", &context.folder);
        
        if (ImGui::Button("Compile")) {
            page_compile();
        }
        ImGui::End();
    }
}

void viewport_render(void) {
    f32 speed;

    speed = 8.0f * (f32)context.window_w / (f32)WINDOW_W;
    
    if (context.current_screen >= 0) {
        if (!sidebar_focus) {
            if (INPUT_PRESS(INPUT_UP)) {
                bg_y += speed;
            }
    
            if (INPUT_PRESS(INPUT_DOWN)) {
                bg_y -= speed;
            }

            if (INPUT_PRESS(INPUT_LEFT)) {
                bg_x += speed;
            }
    
            if (INPUT_PRESS(INPUT_RIGHT)) {
                bg_x -= speed;
            }
        }
        texture_render(
            &(context.page_info.screens[context.current_screen].texture),
            bg_x,
            bg_y,
            0,
            0,
            context.page_info.screens[context.current_screen].texture.w,
            context.page_info.screens[context.current_screen].texture.h
        );
        screen_render_buttons(&(context.page_info.screens[context.current_screen]), bg_x, bg_y);
    }
}