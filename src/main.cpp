#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>

#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "texture.hpp"
#include "world.hpp"
#include "map.hpp"
#include "ui.hpp"
#include "province.hpp"
#include "path.hpp"

#ifdef WINDOWS
#include <windows.h>
#endif

World world;

const int width = 1280;
const int height = 800;

UI::Context ui_ctx;

UI::Widget help_win, help_label[12], help_win_close_btn;
void do_help(UI::Widget * widget, void * data) {
	ui_ctx.add_widget(&help_win);
	ui_ctx.add_widget(&help_win_close_btn);
	for(size_t i = 0; i < 12; i++) {
		ui_ctx.add_widget(&help_label[i]);
	}
}

UI::Widget tov_win, tov_win_close_btn, tov_owner_label, tov_owner_flag_image, tov_population_label;
void do_tile_overview() {
	ui_ctx.add_widget(&tov_win);
	ui_ctx.add_widget(&tov_win_close_btn);
	ui_ctx.add_widget(&tov_owner_label);
	ui_ctx.add_widget(&tov_owner_flag_image);
	ui_ctx.add_widget(&tov_population_label);
}

UI::Widget econ_win, econ_win_close_btn, econ_label[512];
void do_economy_on_click(UI::Widget * widget, void * data) {
	ui_ctx.add_widget(&econ_win);
	ui_ctx.add_widget(&econ_win_close_btn);
	for(size_t i = 0; i < 128; i++) {
		ui_ctx.add_widget(&econ_label[i]);
	}
}

void do_economy_on_update(UI::Widget * widget, void * data) {
	size_t n_prod = 1;
	size_t n_products = world.products.size();

	char * str = new char[255];
	for(size_t i = 0; i < n_products; i++) {
		Product * product = &world.products[i];
		size_t y = n_prod * 24 + 48;
		
		if(product->price_vel > 0.f) {
			UI_Widget_TextColor(0, 255, 0);
			sprintf(str, "+%4.2f", product->price_vel);
		} else {
			UI_Widget_TextColor(255, 0, 0);
			sprintf(str, "%4.2f", product->price_vel);
		}
		econ_label[n_prod * 4 + 0].text(&ui_ctx, str);

		UI_Widget_TextColor(0, 0, 0);
		sprintf(str, "%4.2f", product->price);
		econ_label[n_prod * 4 + 1].text(&ui_ctx, str);
		econ_label[n_prod * 4 + 2].text(&ui_ctx, world.provinces[product->origin_id].name.c_str());
		econ_label[n_prod * 4 + 3].text(&ui_ctx, world.goods[product->good_id].name.c_str());

		n_prod++;
		if(n_prod >= (128 / 4)) break;
	}
	delete[] str;
}

UI::Widget overview_win, overview_time_label, overview_flag_image;
void do_exit(UI::Widget * widget, void * data) {
	exit(EXIT_FAILURE);
}

int run = 1, mx, my;
float fmx, fmy;
int tx, ty;

size_t current_player_nation_id;
size_t selected_province_id;

Map prov_map, pol_map, topo_map;
Map map;
void do_view_prov_map(UI::Widget * widget, void * data) {
	map = prov_map;
}

void do_view_pol_map(UI::Widget * widget, void * data) {
	map = pol_map;
}

void do_view_topo_map(UI::Widget * widget, void * data) {
	map = topo_map;
}

typedef struct {
	float vx;
	float vy;
	float vz;
	float x;
	float y;
	float z;
}Camera;

#include <mutex>
std::mutex world_mutex;
void rendering_main(void) {
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	current_player_nation_id = 0;
	
	SDL_Window * window = SDL_CreateWindow("superleaf1995", 0, 0, width, height, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	const GLubyte * version = glGetString(GL_VERSION);
	printf("OpenGL Version: %s\n", version);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	ui_ctx = UI::Context(Resource_GetPath(""));
	ui_ctx.load_textures();

	world_mutex.lock();
	// Render world stuff now that we are in opengl
	for(auto& nation: world.nations) {
		nation.default_flag.to_opengl();
	}

	prov_map = Map(&world, MAP_PROVINCIAL);
	pol_map = Map(&world, MAP_POLITICAL);
	topo_map = Map(&world, MAP_TOPOGRAPHIC);
	map = prov_map;
	world_mutex.unlock();

	Texture title;
	title.from_file(Resource_GetPath("title.png").c_str());

	Texture admin_icon;

	admin_icon.from_file(Resource_GetPath("icons/admin.png").c_str());
	admin_icon.to_opengl();

	Texture supply_icon;
	supply_icon.from_file(Resource_GetPath("icons/supply.png").c_str());
	supply_icon.to_opengl();

	Texture factory_icon;
	factory_icon.from_file(Resource_GetPath("icons/factory.png").c_str());
	factory_icon.to_opengl();

	Texture jap_troop;
	jap_troop.from_file(Resource_GetPath("troop1.png").c_str());
	jap_troop.to_opengl();

	Texture rus_troop;
	rus_troop.from_file(Resource_GetPath("troop2.png").c_str());
	rus_troop.to_opengl();

	title.to_opengl();

	/* left sidebar buttons */
	UI::Widget help_btn, help_btn_icon;
	Texture help_icon;

	help_icon.from_file(Resource_GetPath("icons/help.png").c_str());
	help_icon.to_opengl();

	UI_Widget_CreateButton(&ui_ctx, NULL, &help_btn, 8, 8, 64, 64);
	ui_ctx.add_widget(&help_btn);
	help_btn.on_click = &do_help;
	UI_Widget_CreateImage(&ui_ctx, &help_btn, &help_btn_icon, 0, 0, 64, 64, &help_icon);
	ui_ctx.add_widget(&help_btn_icon);

	UI::Widget budget_btn, budget_btn_icon;
	Texture budget_icon;

	budget_icon.from_file(Resource_GetPath("icons/budget.png").c_str());
	budget_icon.to_opengl();

	UI_Widget_CreateButton(&ui_ctx, NULL, &budget_btn, 8, (8 * 2) + 64, 64, 64);
	ui_ctx.add_widget(&budget_btn);
	budget_btn.on_click = &do_economy_on_click;
	UI_Widget_CreateImage(&ui_ctx, &budget_btn, &budget_btn_icon, 0, 0, 64, 64, &budget_icon);
	ui_ctx.add_widget(&budget_btn_icon);

	UI::Widget pol_view_btn, pol_view_btn_icon;
	Texture pol_view_icon;

	pol_view_icon.from_file(Resource_GetPath("icons/pol_view.png").c_str());
	pol_view_icon.to_opengl();
	UI_Widget_CreateButton(&ui_ctx, NULL, &pol_view_btn, 8, (8 * 3) + (64 * 2), 64, 64);
	ui_ctx.add_widget(&pol_view_btn);
	pol_view_btn.on_click = &do_view_pol_map;
	UI_Widget_CreateImage(&ui_ctx, &pol_view_btn, &pol_view_btn_icon, 0, 0, 64, 64, &pol_view_icon);
	ui_ctx.add_widget(&pol_view_btn_icon);

	UI::Widget prov_view_btn, prov_view_btn_icon;
	Texture prov_view_icon;

	prov_view_icon.from_file(Resource_GetPath("icons/prov_view.png").c_str());
	prov_view_icon.to_opengl();
	UI_Widget_CreateButton(&ui_ctx, NULL, &prov_view_btn, 8, (8 * 4) + (64 * 3), 64, 64);
	ui_ctx.add_widget(&prov_view_btn);
	prov_view_btn.on_click = &do_view_prov_map;
	UI_Widget_CreateImage(&ui_ctx, &prov_view_btn, &prov_view_btn_icon, 0, 0, 64, 64, &prov_view_icon);
	ui_ctx.add_widget(&prov_view_btn_icon);

	UI::Widget topo_view_btn, topo_view_btn_icon;
	Texture topo_view_icon;

	topo_view_icon.from_file(Resource_GetPath("icons/topo_view.png").c_str());
	topo_view_icon.to_opengl();
	UI_Widget_CreateButton(&ui_ctx, NULL, &topo_view_btn, 8, (8 * 5) + (64 * 4), 64, 64);
	ui_ctx.add_widget(&topo_view_btn);
	topo_view_btn.on_click = &do_view_topo_map;
	UI_Widget_CreateImage(&ui_ctx, &topo_view_btn, &topo_view_btn_icon, 0, 0, 64, 64, &topo_view_icon);
	ui_ctx.add_widget(&topo_view_btn_icon);

	UI::Widget exit_btn, exit_btn_icon;
	Texture exit_icon;

	exit_icon.from_file(Resource_GetPath("icons/exit.png").c_str());
	exit_icon.to_opengl();
	UI_Widget_CreateButton(&ui_ctx, NULL, &exit_btn, 8, (8 * 6) + (64 * 5), 64, 64);
	ui_ctx.add_widget(&exit_btn);
	exit_btn.on_click = &do_exit;
	UI_Widget_CreateImage(&ui_ctx, &exit_btn, &exit_btn_icon, 0, 0, 64, 64, &exit_icon);
	ui_ctx.add_widget(&exit_btn_icon);

	/* help window */
	UI_Widget_CreateWindow(&ui_ctx, NULL, &help_win, width - 520, 32, 512, 512);
	help_win.text(&ui_ctx, "Russo-Japanesse War 1904");
	UI_Widget_CreateButton(&ui_ctx, &help_win, &help_win_close_btn, 512 - 24, -24, 24, 24);
	help_win_close_btn.text(&ui_ctx, "X");
	help_win_close_btn.on_click = &default_close_button_on_click;
	const char * text[] = {
		"Thank you for downloading this game",
		"In this scneario you will control",
		"the japanesse in the historical",
		"sino-japanesse war.",
		"Create a full logistic plan to move",
		"your soldiers into the korean",
		"peninsula.",
		"Remember to build forts to improve",
		"your defences.",
		"Recruit soldiers from your cities.",
		"And last but not least important:",
		"Have fun! :D"
	};
	for(size_t i = 0; i < 12; i++) {
		UI_Widget_CreateLabel(&ui_ctx, &help_win, &help_label[i], 0, (i + 1) * 24, text[i]);
	}

	/* tile overview */
	UI_Widget_CreateWindow(&ui_ctx, NULL, &tov_win, width - 520, 32, 512, 512);
	tov_win.text(&ui_ctx, "Province overview");
	UI_Widget_CreateButton(&ui_ctx, &tov_win, &tov_win_close_btn, 512 - 24, -24, 24, 24);
	tov_win_close_btn.text(&ui_ctx, "X");
	tov_win_close_btn.on_click = &default_close_button_on_click;
	UI_Widget_CreateLabel(&ui_ctx, &tov_win, &tov_owner_label, 0, 24, "?");
	UI_Widget_CreateImage(&ui_ctx, &tov_win, &tov_owner_flag_image, (16) * 6, 0, 32, 24, &world.nations[current_player_nation_id].default_flag);
	UI_Widget_CreateLabel(&ui_ctx, &tov_win, &tov_population_label, 0, 48, "?");

	/* overview bottom window */
	UI_Widget_CreateWindow(&ui_ctx, NULL, &overview_win, 0, height - 128, width, 128);
	overview_win.text(&ui_ctx, world.nations[current_player_nation_id].ref_name.c_str());
	ui_ctx.add_widget(&overview_win);
	UI_Widget_CreateLabel(&ui_ctx, &overview_win, &overview_time_label, 128 + 32 + 8, 24, "?");
	ui_ctx.add_widget(&overview_time_label);
	UI_Widget_CreateImage(&ui_ctx, &overview_win, &overview_flag_image, 8, 8, 128 + 32, 128 - 16, &world.nations[current_player_nation_id].default_flag);
	ui_ctx.add_widget(&overview_flag_image);

	/* economy window */
	UI_Widget_CreateWindow(&ui_ctx, NULL, &econ_win, 128, 32, 512, 512);
	UI_Widget_CreateButton(&ui_ctx, &econ_win, &econ_win_close_btn, 512 - 24, -24, 24, 24);
	econ_win_close_btn.on_click = &default_close_button_on_click;
	UI_Widget_CreateLabel(&ui_ctx, &econ_win, &econ_label[0], 128 * 0, 24, "% Change");
	UI_Widget_CreateLabel(&ui_ctx, &econ_win, &econ_label[1], 128 * 1, 24, "Price");
	UI_Widget_CreateLabel(&ui_ctx, &econ_win, &econ_label[2], 128 * 2, 24, "Province");
	UI_Widget_CreateLabel(&ui_ctx, &econ_win, &econ_label[3], 128 * 3, 24, "Good");
	for(size_t i = (4 / 4); i < (128 / 4); i++) {
		size_t y = i * 24 + 24;
		UI_Widget_CreateLabel(&ui_ctx, &econ_win, &econ_label[i * 4 + 0], 128 * 0, y, "?");
		UI_Widget_CreateLabel(&ui_ctx, &econ_win, &econ_label[i * 4 + 1], 128 * 1, y, "?");
		UI_Widget_CreateLabel(&ui_ctx, &econ_win, &econ_label[i * 4 + 2], 128 * 2, y, "?");
		UI_Widget_CreateLabel(&ui_ctx, &econ_win, &econ_label[i * 4 + 3], 128 * 3, y, "?");
	}
	econ_win.on_update = &do_economy_on_update;

	Camera cam;
	cam.x = -1.f;
	cam.y = 1.f;
	cam.z = -100.f;

	cam.vx = 0.f;
	cam.vy = 0.f;
	cam.vz = 0.f;
	glClearColor(1.0f, 0.0f, 1.0f, 0.0f);

	while(run) {
		SDL_Event event;
		int r;

		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_MOUSEBUTTONDOWN:
				SDL_GetMouseState(&mx, &my);
				
				r = ui_ctx.check_click(mx, my);

				if(fmx > 0 && fmx < world.width
				&& fmy > 0 && fmy < world.height
				&& !r) {
					Tile * tile = &world.tiles[tx + ty * world.width];
					char * str = (char *)malloc(255);
					const char * name = (tile->owner_id != (size_t)-1) ? world.nations[tile->owner_id].name.c_str() : "none";
					sprintf(str, "Owner:   %s", name);
					tov_owner_label.text(&ui_ctx, str);

					if(tile->province_id == (size_t)-1) {
						tov_win.text(&ui_ctx, "unnamed land");
					} else {
						tov_win.text(&ui_ctx, world.provinces[tile->province_id].name.c_str());

						/* some heartbleed techniques were applied here */
						sprintf(str, "Population: %lu", world.provinces[tile->province_id].population);
						tov_population_label.text(&ui_ctx, str);
					}

					free(str);

					tov_owner_flag_image.current_texture = &world.nations[tile->owner_id].default_flag;

					selected_province_id = tile->province_id;
					do_tile_overview();
				}
				break;
			case SDL_MOUSEMOTION:
				SDL_GetMouseState(&mx, &my);
				ui_ctx.check_hover(mx, my);

				fmx = mx - (width / 2.f);
				fmx /= ((float)width / (float)(-cam.z * 1.33f));

				fmy = my - (height / 2.f);
				fmy /= ((float)height / (float)(-cam.z * 0.83f));

				fmx += -cam.x;
				fmy += cam.y;

				fmx = (int)fmx;
				fmy = (int)fmy;
				tx = (int)fmx;
				ty = (int)fmy;
				break;
			case SDL_MOUSEWHEEL:
				cam.vz += event.wheel.y;
				break;
			case SDL_TEXTINPUT:
				ui_ctx.check_text_input((const char *)&event.text.text);
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
				case SDLK_UP:
					cam.vy -= 0.05f * -cam.z;
					break;
				case SDLK_DOWN:
					cam.vy += 0.05f * -cam.z;
					break;
				case SDLK_LEFT:
					cam.vx += 0.05f * -cam.z;
					break;
				case SDLK_RIGHT:
					cam.vx -= 0.05f * -cam.z;
					break;
				}
				break;
			case SDL_QUIT:
				run = 0;
				break;
			default:
				break;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, (float)width / (float)height, 1.0f, 1024.0f);
		glViewport(0, 0, width, height);
		glTranslatef(cam.x, cam.y, cam.z);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(180.f, 1.0f, 0.0f, 0.0f);
		glRotatef(0.f, 0.0f, 1.0f, 0.0f);
		glRotatef(0.f, 0.0f, 0.0f, 1.0f);
		for(size_t i = 0; i < map.n_horz_quads; i++) {
			for(size_t j = 0; j < map.n_vert_quads; j++) {
				glCallList(map.quads_gl_list_num[i + j * map.n_horz_quads]);
			}
		}

		glBegin(GL_POLYGON);
		glColor3f(1.f, 1.f, 1.f);
		glVertex2f(fmx, fmy);
		glColor3f(1.f, 1.f, 1.f);
		glVertex2f(fmx + 1.f, fmy);
		glColor3f(1.f, 1.f, 1.f);
		glVertex2f(fmx + 1.f, fmy + 1.f);
		glColor3f(1.f, 1.f, 1.f);
		glVertex2f(fmx, fmy + 1.f);
		glEnd();
		
		glBindTexture(GL_TEXTURE_2D, world.nations[current_player_nation_id].default_flag.gl_tex_num);
		GLUquadricObj * sphere = NULL;
		sphere = gluNewQuadric();
		gluQuadricDrawStyle(sphere, GLU_FILL);
		gluQuadricTexture(sphere, 1);
		gluQuadricNormals(sphere, GLU_SMOOTH);
		gluSphere(sphere, 0.5f, 20, 20);
		gluDeleteQuadric(sphere);
		glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();

		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.f, (float)width, (float)height, 0.f, 0.0f, 1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.f, 0.f, 0.f);
		ui_ctx.render_all();
		glPopMatrix();

		glLoadIdentity();
		glRasterPos2f(-3.0f, -2.0f);

		int hour = world.time % 24;
		int day = world.time / 24;

		int is_leap = day / (365 * 4);
		if(is_leap) day += (day / (365 * 4));
		int month = day / 31;
		int year = month / 12;

		day %= 31;
		month %= 12;
		
		char str[255];
		sprintf((char *)&str, "%u/%u/%u - %u", year, month, day, hour);
		overview_time_label.text(&ui_ctx, (char *)&str);

		if(cam.vx >= 0.9f) {
			cam.vx -= 0.8f;
		} else if(cam.vx <= -0.9f) {
			cam.vx += 0.8f;
		} else {
			cam.vx = 0.f;
		}

		if(cam.vy >= 0.9f) {
			cam.vy -= 0.8f;
		} else if(cam.vy <= -0.9f) {
			cam.vy += 0.8f;
		} else {
			cam.vy = 0.f;
		}

		if(cam.vz >= 0.9f) {
			cam.vz -= 0.8f;
		} else if(cam.vz <= -0.9f) {
			cam.vz += 0.8f;
		} else {
			cam.vz = 0.f;
		}

		cam.x += cam.vx;
		cam.y += cam.vy;
		cam.z += cam.vz;

		SDL_GL_SwapWindow(window);
	}

	TTF_Quit();
	SDL_Quit();
	return;
}

#include <thread>
int main(int argc, char ** argv) {
	world_mutex.lock();
	world = World("map_topo.png", "map_pol.png", "map_div.png");
	world.time = 695459;
	world.time -= (8600 * 76);
	world.time -= 24 * 190;
	world_mutex.unlock();

#ifndef UNIT_TEST
	std::thread t1(rendering_main);

	while(run) {
		world.do_tick();
	}
	t1.join();
#endif
	return 0;
}

#ifdef WINDOWS
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpszArgument, int iShow) {
	char *argv[1];
	argv[0] = "/";
	main(1, argv);
}
#endif