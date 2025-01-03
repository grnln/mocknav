#include "screen.h"

#include "context.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

screen_t screen_create(std::string name, std::string texture_path) {
    screen_t out;

    out.name = name;
    out.texture_path = texture_path;
    out.texture = texture_load(texture_path.c_str());
    out.h = ((f32)out.texture.h / (f32)out.texture.w) * 100.0f;

    return out;
}

void screen_add_button(screen_t *screen, s32 x, s32 y, s32 w, s32 h, s32 bg_x, s32 bg_y, std::string to) {
    button_t button;

    button.screen = to;
    button.x = x - bg_x;
    button.y = y - bg_y;
    button.w = w;
    button.h = h;
    screen->buttons.push_back(button);
}

static inline bool rect_collision(s32 x1, s32 y1, s32 w1, s32 h1, s32 x2, s32 y2, s32 w2, s32 h2) {
    return ((x1 + w1) >= x2 && x1 <= (x2 + w2) && (y1 + h1) >= y2 && y1 <= (y2 + h2));
}

void screen_render_buttons(screen_t *screen, s32 x, s32 y) {
    s32 radius;

    radius = (s32)(8.0f * ((f32)context.window_w / (f32)WINDOW_W));

    for (u32 i = 0; i < screen->buttons.size(); i++) {
        bool collision;
        u8 red;
        SDL_Rect rect;

        rect.x = x + screen->buttons[i].x;
        rect.y = y + screen->buttons[i].y;
        rect.w = screen->buttons[i].w;
        rect.h = screen->buttons[i].h;

        collision = rect_collision(
            rect.x,
            rect.y,
            rect.w,
            rect.h,
            context.mouse_x - (radius / 2),
            context.mouse_y - (radius / 2),
            radius,
            radius
        );

        if (INPUT_PRESS(INPUT_LCLICK) && collision) {
            if (!INPUT_REPEAT(INPUT_LCLICK)) {
                context.current_button = i;
            } else {
                screen->buttons[i].x += (context.mouse_x - context.prev_mouse_x);
                screen->buttons[i].y += (context.mouse_y - context.prev_mouse_y);
            }
        }
        red = (context.current_button == i) ? 255 : 0;

        SDL_SetRenderDrawColor(context.renderer, red, 0, 0, 128);
        SDL_RenderFillRect(context.renderer, &rect);
    }
}

void screen_free(screen_t *screen) {
    texture_free(&(screen->texture));
}

enum page_read_mode {
    PAGE,
    SCREEN,
    BUTTON
};

void page_load(char *path) {
    u32 mode, line_count;
    std::string line;
    std::ifstream file;

    file.open(path);
    line_count = 0;

    while (std::getline(file, line)) {
        if (!line.empty()) {
            if (line == "[PAGE]") {
                mode = PAGE;
                line_count = 0;
            } else if (line == "[SCREEN]") {
                screen_t screen;

                context.page_info.screens.push_back(screen);
                mode = SCREEN;
                line_count = 0;
            } else if (line == "[BUTTON]") {
                button_t button;

                context.page_info.screens.back().buttons.push_back(button);
                mode = BUTTON;
                line_count = 0;
            } else {
                switch (mode) {
                    default:
                        break;

                    case PAGE:
                        if (line_count == 0) {
                            context.page_info.name = line;
                        } else if (line_count == 1) {
                            context.page_info.icon = line;
                        }
                        line_count++;
                        break;

                    case SCREEN:
                        if (line_count == 0) {
                            context.page_info.screens.back().name = line;
                        } else if (line_count == 1) {
                            context.page_info.screens.back().texture_path = line;
                            context.page_info.screens.back().texture = texture_load(line.c_str());
                        }
                        line_count++;
                        break;

                    case BUTTON:
                        if (line_count == 0) {
                            context.page_info.screens.back().buttons.back().x = (atof(line.c_str()) / 100.0f) * context.page_info.screens.back().texture.w;
                        } else if (line_count == 1) {
                            context.page_info.screens.back().buttons.back().y = (atof(line.c_str()) / 100.0f) * context.page_info.screens.back().texture.w;
                        } else if (line_count == 2) {
                            context.page_info.screens.back().buttons.back().w = (atof(line.c_str()) / 100.0f) * context.page_info.screens.back().texture.w;
                        } else if (line_count == 3) {
                            context.page_info.screens.back().buttons.back().h = (atof(line.c_str()) / 100.0f) * context.page_info.screens.back().texture.w;
                        } else if (line_count == 4) {
                            context.page_info.screens.back().buttons.back().screen = line;
                        }
                        line_count++;
                        break;
                }
            }
        }
    }
    file.close();
}

void page_save(void) {
    std::string path;
    std::ofstream file;

    path = context.page_info.name;
    path += ".mock";
    
    file.open(path);
    
    file << "[PAGE]";
    file << "\n" << context.page_info.name;
    file << "\n" << context.page_info.icon;

    for (u32 i = 0; i < context.page_info.screens.size(); i++) {
        file << "\n\n[SCREEN]";
        file << "\n" << context.page_info.screens[i].name;
        file << "\n" << context.page_info.screens[i].texture_path;
        
        for (u32 j = 0; j < context.page_info.screens[i].buttons.size(); j++) {
            file << "\n\n[BUTTON]";
            file << "\n" << ((f32)context.page_info.screens[i].buttons[j].x / (f32)context.page_info.screens[i].texture.w) * 100.0f;
            file << "\n" << ((f32)context.page_info.screens[i].buttons[j].y / (f32)context.page_info.screens[i].texture.w) * 100.0f;
            file << "\n" << ((f32)context.page_info.screens[i].buttons[j].w / (f32)context.page_info.screens[i].texture.w) * 100.0f;
            file << "\n" << ((f32)context.page_info.screens[i].buttons[j].h / (f32)context.page_info.screens[i].texture.w) * 100.0f;
            file << "\n" << context.page_info.screens[i].buttons[j].screen;
        }
    }
    file.close();
}

void page_compile(void) {
    std::ofstream file;
    
    for (u32 i = 0; i < context.page_info.screens.size(); i++) {
        std::string html_path, css_path, css_name;
        
        css_name = context.page_info.screens[i].name;
        css_name.replace(
            context.page_info.screens[i].name.length() - 5,
            5,
            ".css"
        );
        html_path = context.folder + "/" + context.page_info.screens[i].name;
        css_path = context.folder + "/" + css_name;

        /* Write HTML file */
        file.open(html_path);

        file << "<!DOCTYPE html>\n\n";
        file << "<html>\n";
        
        file << "\t<head>\n";
        file << "\t\t<title>" + context.page_info.name + "</title>\n";
        file << "\t\t<link rel = \"icon\" type = \"image/ico\" href = \"" + context.page_info.icon + "\">\n";
        file << "\t\t<link rel = \"stylesheet\" href = \"" + css_name + "\">\n";
        file << "\t\t<link rel = \"stylesheet\" href = \"global.css\">\n";
        file << "\t</head>\n";

        file << "\t<body>\n";
        file << "\t\t<text class = \"marker\" style = \"top: ";
        file << std::to_string((((f32)context.page_info.screens[i].texture.h / (f32)context.page_info.screens[i].texture.w) * 100.0f) - 5.0f);
        file << "vw\">_</text>\n";
        
        for (u32 j = 0; j < context.page_info.screens[i].buttons.size(); j++) {
            s32 w;
            button_t button;
            std::string text;

            w = context.page_info.screens[i].texture.w;
            button = context.page_info.screens[i].buttons[j];
            
            file << "\t\t<a href = \"" + button.screen + "\">\n";

            text = "\t\t\t<button class = \"nav_button\" ";
            text += "style = \"";
            text += "top: " + std::to_string(((f32)button.y / (f32)w) * 100.0f) + "vw; ";
            text += "left: " + std::to_string(((f32)button.x / (f32)w) * 100.0f) + "vw; ";
            text += "width: " + std::to_string(((f32)button.w / (f32)w) * 100.0f) + "vw; ";
            text += "height: " + std::to_string(((f32)button.h / (f32)w) * 100.0f) + "vw";
            text += "\"></button>\n";
            
            file << text;
            file << "\t\t</a>\n";
        }
        file << "\t</body>\n";
        file << "</html>\n";

        file.close();

        /* Write CSS file */
        file.open(css_path);

        file << "html {\n";
        file << "\tbackground-image: url(\"" + context.page_info.screens[i].texture_path + "\");\n";
        file << "\tbackground-size: 100%;\n";
        file << "\tbackground-repeat: no-repeat;\n";
        file << "\tbackground-attachment: scroll;\n";
        file << "}\n\n";

        file.close();
    }
    /* Write global CSS file */
    file.open(context.folder + "/global.css");

    file << ".nav_button {\n";
    file << "\tposition: absolute;\n";
    file << "\tcolor: transparent;\n";
    file << "\tbackground-color: transparent;\n";
    file << "\tborder-style: none;\n";
    file << "\tborder-radius: 1.5vw;\n";
    file << "}\n\n";

    file << ".nav_button:hover {\n";
    file << "\tbackground-color: rgb(0.0, 0.0, 0.0, 0.25);\n";
    file << "}\n\n";

    file << ".nav_button:active {\n";
    file << "\tbackground-color: rgb(0.0, 0.0, 0.0, 0.5);\t";
    file << "}\n\n";

    file << ".marker {\n";
    file << "\tposition: absolute;\n";
    file << "\tcolor: transparent;\n";
    file << "}\n";

    file.close();
}