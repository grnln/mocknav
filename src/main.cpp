#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_sdl2.h"
#include "../imgui/backends/imgui_impl_sdlrenderer2.h"

#include <imgui.h>

#include "context.h"
#include "ui.h"
#include "screen.h"
#include "input.h"

#define WINDOW_NAME 			"MockNav"
#define FPS						60
#define TARGET_DELTA_TIME 		((1.0f / (f32)FPS) * 1000.0f)

static s32 time_start, time_end;
static f32 time_elapsed;
static SDL_Event event;

/* Global context */
context_t context;

static inline void quit(void) {
    for (u32 i = 0; i < context.page_info.screens.size(); i++) {
        screen_free(&(context.page_info.screens[i]));
    }
    context.page_info.screens.clear();

    ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
    
    if (context.renderer != NULL) {
        SDL_DestroyRenderer(context.renderer);
    }

    if (context.window != NULL) {
        SDL_DestroyWindow(context.window);
    }
    SDL_Quit();
}

int main(int argc, char **argv) {
    /* Initialize global context */
    context.current_screen = -1;
	context.current_button = -1;
	context.folder = "html";

    /* Setup SDL2 */
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);

	context.window = SDL_CreateWindow(
		WINDOW_NAME,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H,
		SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE
	);
	context.renderer = SDL_CreateRenderer(context.window, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawBlendMode(context.renderer, SDL_BLENDMODE_BLEND);

    /* Setup ImGui */
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();
	ImGui::GetIO().IniFilename = NULL;
	
	ImGui_ImplSDL2_InitForSDLRenderer(context.window, context.renderer);
	ImGui_ImplSDLRenderer2_Init(context.renderer);

	input_init();

	if (argc > 1) {
		page_load(argv[1]);
	}

    /* Main loop */
    while (1) {
		time_start = SDL_GetTicks();

		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);

			switch (event.type) {
				default:
					break;

				case SDL_QUIT:
					quit();
					return 0;
					break;
			}
		}
		input_poll();

		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		/* UI Rendering */
        sidebar_render();
        ImGui::Render();

		SDL_RenderSetScale(context.renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		SDL_SetRenderDrawColor(context.renderer, 0, 0, 0, 255);
		SDL_RenderClear(context.renderer);
		
		/* SDL Rendering */
        viewport_render();
		
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), context.renderer);

		SDL_RenderPresent(context.renderer);

		/* Enforce a fixed framerate */
        time_end = SDL_GetTicks();
        time_elapsed = (f32)(time_end - time_start);

        if (time_elapsed > 0.0f && time_elapsed < TARGET_DELTA_TIME) {
            SDL_Delay((u32)SDL_floorf(TARGET_DELTA_TIME - time_elapsed));
        }
	}
    quit();
    return 0;
}