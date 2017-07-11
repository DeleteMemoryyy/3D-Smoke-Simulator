#include <imgui.h>
#include "imgui_impl_glfw.h"
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <ctime>
#include <Windows.h>
#include "CFD.h"
#include "texture.h"
#include "render.h"
#include <iostream>

using namespace std;

#define Window_W	(1280)
#define	Window_H	(720)
#define Window_D	(720)
#define FPS			(10)
#define PI			(3.1415926)

C_CFD grid = C_CFD();
P_TEXTURE texture[8];
GLuint NoForAll[8];
bool Pause = false;
float rhoAngle = 2.0f;
float phiAngle = 0;
int curFrame = 0;
int add_or_sub = 1;
int totalFrame = 100;
bool initializedGl = false;
bool play_back = false;
int last_color = 7;


const vec3f color_table[7] = {
	vec3f(1.f,0.f,0.f),
	vec3f(1.f,1.f,0.f),
	vec3f(1.f,0.f,1.f),
	vec3f(0.f,1.f,0.f),
	vec3f(0.f,1.f,1.f),
	vec3f(0.f,0.f,1.f),
	vec3f(1.f,1.f,1.f)};

const time_t tInterval = (time_t)(1000.f / (float)FPS);
time_t tLast = 0, tNow, tRest;

enum DisplayState
{
	Setting,
	Rendering,
	Observing,
};

DisplayState dState = Setting;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS)
		return;
	if (key == GLFW_KEY_BACKSPACE)
	{
		play_back ^= 1;
	}
	if (key == 'p' || key == 'P')
	{
		Pause = !Pause;
	}
	if (key == 'd' || key == 'D')
	{
		rhoAngle += 5;
		if (rhoAngle >= 360)
			rhoAngle -= 360;
	}
	if (key == 'a' || key == 'A')
	{
		rhoAngle -= 5;
		if (rhoAngle < 0)
			rhoAngle += 360;
	}
	if (key == 'w' || key == 'W')
	{
		phiAngle -= 5;
		if (phiAngle >= 90)
			phiAngle = 90;

	}
	if (key == 's' || key == 'S')
	{
		phiAngle += 5;
		if (phiAngle <= 0)
			phiAngle = 0;
	}
	if (key == 'c' || key == 'C')
	{
		grab(Window_W,Window_H);
	}
}

void initGL()
{
	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	glViewport(280, 0, 720, 720);
	glOrtho(-Window_W / 2, Window_W / 2,
		-Window_H / 2, Window_H / 2,
		-Window_D / 2, Window_D / 2);
	glEnable(GL_BLEND);
	glReadBuffer(GL_BACK);
	glDrawBuffer(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_DEPTH_TEST);

	float DiffuseLight1[4] = { 1, 1, 1, 1 };
	float DiffuseLight2[4] = { 1, 1, 1, 1 };
	float DiffuseLight3[4] = { 1, 1, 1, 1 };
	float DiffuseLight4[4] = { 1, 1, 1, 1 };
	float AmbientLight[4] = { 0, 0, 0, 0 };

	float Position0[4] = { 0, 0, 0, 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_POSITION, Position0);
	glEnable(GL_LIGHT0);
	float Position1[4] = { -Window_W / 2, Window_H / 2, Window_D / 2, 1 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, DiffuseLight1);
	glLightfv(GL_LIGHT1, GL_POSITION, Position1);
	glEnable(GL_LIGHT1);
	float Position2[4] = { -Window_W / 2, Window_H / 2, -Window_D / 2, 1 };
	glLightfv(GL_LIGHT2, GL_DIFFUSE, DiffuseLight2);
	glLightfv(GL_LIGHT2, GL_POSITION, Position2);
	glEnable(GL_LIGHT2);
	float Position3[4] = { Window_W / 2, Window_H / 2, -Window_D / 2, 1 };
	glLightfv(GL_LIGHT3, GL_DIFFUSE, DiffuseLight3);
	glLightfv(GL_LIGHT3, GL_POSITION, Position3);
	glEnable(GL_LIGHT3);
	float Position4[4] = { Window_W / 2, Window_H / 2, Window_D / 2, 1 };
	glLightfv(GL_LIGHT4, GL_DIFFUSE, DiffuseLight4);
	glLightfv(GL_LIGHT4, GL_POSITION, Position4);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHTING);

	glGenTextures(8, NoForAll);
	for (int i = 0; i < 8; i++)
	{
		texture[i].init();
		texture[i].load(NoForAll, i);
	}
	glEnable(GL_TEXTURE_2D);

}


int main()
{
	// Setup window
	srand((unsigned)clock());
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		return 1;
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Smoke-Simulator", NULL, NULL);
	glfwMakeContextCurrent(window);

	// Setup ImGui binding
	ImGui_ImplGlfw_Init(window, true);
	ImGuiIO& io = ImGui::GetIO();

	glClearColor(0.8f, 0.8f, 0.8f, 0.8f);
	bool add_new_source = false;
	bool full_source = false;
	bool load_from_file = false;
	bool start_to_draw = false;
	bool start_to_load = false;
	bool load_false = false;
	bool render_cancel = false;
	bool render_save_file = false;
	bool render_save_complete = false;
	bool render_to_display = false;
	string last_save_name;

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplGlfw_NewFrame();

		// Rendering
		switch (dState)
		{
		case Setting:
		{
			glClear(GL_COLOR_BUFFER_BIT);
			{
				ImGui::SetNextWindowSize(ImVec2(550, 700), ImGuiSetCond_Once);
				ImGui::SetNextWindowPos(ImVec2(20, 5), ImGuiSetCond_Once);
				ImGui::Begin("SmokeSource",NULL, ImGuiWindowFlags_NoResize);

				ImGui::SliderInt("frames to generate.", &totalFrame,10,300);

				if (ImGui::Button(" Generate "))
					start_to_draw ^= 1;

				ImGui::SameLine(100.f);
				if (ImGui::Button("Load File"))
					load_from_file ^= 1;

				int nx = grid.NX;
				
				int sourceNum = grid.sourceNum;

				ImGui::Text("");

				if (sourceNum == 0)
					ImGui::Text("No smoke source exists.");
				else
					ImGui::Text("%d smoke sources exist.",sourceNum);

				if (full_source != true)
				{
					ImGui::SameLine(180.f);
					if (ImGui::Button("Add New Source"))
						add_new_source ^= 1;
				}

				for (int a_s = 0; a_s < sourceNum; ++a_s)
				{
					ImGui::PushID(a_s);
					ImGui::Separator();
					ImGui::Text("Source %d", a_s);
					ImGui::Text("Shape:");ImGui::SameLine();
					int t_shape = grid.source[a_s]->shape;
					ImGui::RadioButton("Ball", &t_shape, Ball); ImGui::SameLine();
					ImGui::RadioButton("Cuboid", &t_shape, Cuboid);
					static float t_color[3];

					if (t_shape == Ball)
					{
						C_SmokeSource * ps = grid.source[a_s];
						ps->shape = Ball;

						int v[3];
						v[0] = ps->pos.x;
						v[1] = ps->pos.y;
						v[2] = ps->pos.z;
						ImGui::SliderInt3("Position(x,y,z)", v, 1, nx);
						ps->pos.x = v[0];
						ps->pos.y = v[1];
						ps->pos.z = v[2];

						ImGui::SliderFloat("Radius", &ps->r, 0.5f, 5.f,"%.1f");

						ImGui::PushStyleColor(ImGuiCol_Button, ImColor(ps->color.x, ps->color.y, ps->color.z, 0.7f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(ps->color.x, ps->color.y, ps->color.z, 0.8f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(ps->color.x, ps->color.y, ps->color.z, 0.85f));
						ImGui::Button("Color");
						ImGui::PopStyleColor(3);

						int colorId = -1;

						for (int cnt = 0; cnt < 7; ++cnt)
						{
							if (cnt > 0)
							{
								ImGui::SameLine();
							}
							ImGui::PushID(a_s*10+cnt);
							ImGui::PushStyleColor(ImGuiCol_Button, ImColor(color_table[cnt].x, color_table[cnt].y, color_table[cnt].z, 0.7f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(color_table[cnt].x, color_table[cnt].y, color_table[cnt].z, 0.8f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(color_table[cnt].x, color_table[cnt].y, color_table[cnt].z, 0.85f));
							if (ImGui::Button("", ImVec2(40.f, 0.f)))
								colorId = cnt;
							ImGui::PopStyleColor(3);
							ImGui::PopID();

						}

						if (colorId != -1)
							ps->color = color_table[colorId];

						ImGui::SliderFloat("Initial Density", &ps->initialD, 0.2f, 1.f);

						ImGui::Checkbox("Fixed Density", &ps->fixedDensity);

						if (ps->fixedDensity == false)
						{
							ImGui::SliderFloat("Density Chaging per Sec", &ps->dD, -0.3f, 0.3f);
						}

						ImGui::SliderFloat("Rising Velocity", &ps->initialVC, 1.5f, 4.5f,"%.1f");
						ImGui::SliderFloat("Initial Temperature", &ps->initialTC, 100.f, 300.f, "%.1f");

					}
					else
					{
						C_RecSmokeSource * rps = (C_RecSmokeSource *)grid.source[a_s];
						rps->shape = Cuboid;

						int rv[3];
						rv[0] = rps->pos.x;
						rv[1] = rps->pos.y;
						rv[2] = rps->pos.z;
						ImGui::SliderInt3("Position(x,y,z)", rv, 1, nx);
						rps->pos.x = rv[0];
						rps->pos.y = rv[1];
						rps->pos.z = rv[2];

						float fv[3];
						fv[0] = rps->w;
						fv[1] = rps->h;
						fv[2] = rps->d;
						ImGui::SliderFloat3("Size(w,h,d)", fv, 0.1f, (float)nx/2.f);
						rps->w = fv[0];
						rps->h = fv[1];
						rps->d = fv[2];

						ImGui::PushStyleColor(ImGuiCol_Button, ImColor(rps->color.x, rps->color.y, rps->color.z, 0.7f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(rps->color.x, rps->color.y, rps->color.z, 0.8f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(rps->color.x, rps->color.y, rps->color.z, 0.85f));
						ImGui::Button("Color");
						ImGui::PopStyleColor(3);

						int colorId = -1;

						for (int cnt = 0; cnt < 7; ++cnt)
						{
							if (cnt > 0)
							{
								ImGui::SameLine();
							}
							ImGui::PushID(a_s*10+cnt);
							ImGui::PushStyleColor(ImGuiCol_Button, ImColor(color_table[cnt].x, color_table[cnt].y, color_table[cnt].z, 0.7f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(color_table[cnt].x, color_table[cnt].y, color_table[cnt].z, 0.8f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(color_table[cnt].x, color_table[cnt].y, color_table[cnt].z, 0.85f));
							if (ImGui::Button("", ImVec2(40.f, 0.f)))
								colorId = cnt;
							ImGui::PopStyleColor(3);
							ImGui::PopID();

						}

						if (colorId != -1)
							rps->color = color_table[colorId];

						ImGui::SliderFloat("Initial Density", &rps->initialD, 0.2f, 1.f);

						ImGui::Checkbox("Fixed Density", &rps->fixedDensity);

						if (rps->fixedDensity == false)
						{
							ImGui::SliderFloat("Density Chaging per Sec", &rps->dD, -0.3f, 0.3f);
						}

						ImGui::SliderFloat("Rising Velocity", &rps->initialVC, 1.5f, 4.5f, "%.1f");
						ImGui::SliderFloat("Initial Temperature", &rps->initialTC, 100.f, 300.f, "%.1f");
					}
					ImGui::PopID();
				}
				ImGui::End();
			}
			if (add_new_source)
			{
				//if (grid.addSource(vec3f(0.56f, 0.86f, 0.70f), vec3f(grid.NX / 2, 1, grid.NX / 2),Cuboid,2.5f,0.6f,0.05f,false,3.5f) == -1)
				int cId = rand() % 7;
				while (cId == last_color)
					cId = rand() % 7;
				last_color = cId;

				float t_C = 1.8f + (float)(rand() % 10) / 10.f, t_T = 150.f + (float)(rand() % 1000) / 10.f;
				if (grid.addSource(color_table[cId], vec3f(grid.NX / 2, 1, grid.NX / 2),Cuboid,4.5,0.6f,0.05f,false,t_C,t_T) == -1)
					full_source = true;
				add_new_source ^= 1;
			}

			if (start_to_draw)
			{
				dState = Rendering;
				curFrame = 0;
				load_false = false;
			}

			if (load_from_file)
			{
				ImGui::SetNextWindowSize(ImVec2(530, 90), ImGuiSetCond_Once);
				ImGui::SetNextWindowPos(ImVec2(630, 100), ImGuiSetCond_Once);				
				ImGui::Begin("Load File", &load_from_file, ImGuiWindowFlags_NoResize);
				static char buf1[64] = ""; ImGui::InputText(".savefile	File Name", buf1, 64);
				
				if (ImGui::Button("Load File and Display"))
					start_to_load ^= 1;

				if (start_to_load)
				{
					string fileName = string(buf1) + ".savefile";
					//cout << fileName << endl;
					if (grid.loadBuf(fileName))
					{
						curFrame = 0;
						totalFrame = grid.bufFrameNum();
						dState = Observing;
					}
					else
					{
						load_false = true;
						start_to_load ^= 1;
					}
				}

				if (load_false)
				{
					ImGui::SameLine();
					ImGui::Text("	File not exist.");
				}
				ImGui::End();

				//TODO
			}
		}
		ImGui::Render();
			break;

		case Rendering:
			glClear(GL_COLOR_BUFFER_BIT);
			
			if (curFrame == 0)
			{
				grid.initBuf(totalFrame);
			}
			
			{
				ImGui::SetNextWindowSize(ImVec2(680, 130), ImGuiSetCond_Once);
				ImGui::SetNextWindowPos(ImVec2(300, 185), ImGuiSetCond_Once);
				ImGui::Begin("Render", NULL, ImGuiWindowFlags_NoResize);
				ImGui::Text("Rendering: %d / %d frames.", curFrame, totalFrame);
				float progress = (float)curFrame / (float)totalFrame;
				ImGui::ProgressBar(progress, ImVec2(0.f, 0.f));

				if (curFrame < totalFrame)
				{
					grid.render(1, curFrame,
						vec3f(-Window_H * 0.285f, -Window_H * 0.285f, -Window_H * 0.285f),
						vec3f(Window_H * 0.285f, Window_H * 0.285f, Window_H * 0.285f),
						NoForAll, 8);
					grid.update(curFrame);
					curFrame++;

					ImGui::SameLine();
					if (ImGui::Button(" Cancel "))
						render_cancel ^= 1;
					
				}
				else
				{
					ImGui::Text("");
					ImGui::Text("");
					ImGui::SameLine(120.f);
					if (ImGui::Button(" Display "))
					{
						render_to_display = true;
					}

					ImGui::SameLine(290.f);
					if (ImGui::Button(" Save File "))
					{
						render_save_file ^= 1;
					}
				}
				ImGui::End();

				if (render_save_file)
				{
					ImGui::SetNextWindowSize(ImVec2(580, 90), ImGuiSetCond_Once);
					ImGui::SetNextWindowPos(ImVec2(440, 350), ImGuiSetCond_Once);
					ImGui::Begin("Save File", &render_save_file, ImGuiWindowFlags_NoResize);
					static char buf2[64] = ""; ImGui::InputText(".savefile	File Name", buf2, 64);
					if (ImGui::Button(" Save "))
					{
						string fileName = string(buf2) + ".savefile";
						grid.saveBuf(fileName);
						last_save_name = fileName;
						render_save_complete = true;
					}

					if (render_save_complete)
					{
						ImGui::SameLine();
						ImGui::Text("	%s has been saved.",last_save_name.c_str());
					}

					ImGui::End();
				}

				if (render_cancel)
				{
					add_new_source = full_source = load_from_file = start_to_draw = 
						start_to_load = load_false = render_cancel = render_save_file = 
						render_save_complete = render_to_display = false;
					curFrame = 0;
					grid.clearField();
					dState = Setting;
				}

				if (render_to_display)
				{
					curFrame = 0;
					dState = Observing;
				}

			}

			ImGui::Render();
			break;
		case Observing:
			if (initializedGl == false)
			{
				initGL();
				glfwSetKeyCallback(window, key_callback);
				initializedGl = true;
			}
			glClear(GL_COLOR_BUFFER_BIT);

			tNow = clock();
			if (tNow - tLast >= tInterval)
			{
				tLast = tNow;
				if (!Pause)
				{
					//grid.update(currFrame);

					if (play_back == false)
						curFrame++;
					else
						curFrame--;

					if (curFrame < 0)
						curFrame += totalFrame;
					if (curFrame >= totalFrame)
						curFrame -= totalFrame;

					rhoAngle += 0.5f;
					phiAngle += add_or_sub * 0.2f;
					if (rhoAngle >= 360)
						rhoAngle -= 360;
					if (phiAngle >= 20)
					{
						phiAngle = 20;
						add_or_sub = -add_or_sub;
					}
					if (phiAngle <= 0)
					{
						phiAngle = 0;
						add_or_sub = -add_or_sub;
					}
				}

				glPushMatrix();

				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
				glRotated(phiAngle, 1, 0, 0);
				glRotatef(rhoAngle, 0, 1, 0);
				grid.render(2, curFrame,
					vec3f(-Window_H * 0.285f, -Window_H / 3 * 0.285f, -Window_H * 0.285f),
					vec3f(Window_H * 0.285f, Window_H* 0.285f, Window_H * 0.285f),
					NoForAll, 8);
				int idx[] = { 0, 0, 0, 0, 0, 0 };
				Draw_Box(-Window_H * 0.285f, Window_H * 0.285f,
					-Window_H * 0.285f, Window_H * 0.285f,
					-Window_H * 0.285f, Window_H * 0.285f,
					0.3f, 0.3f, 0.3f, 0.3f,
					0.f, 0.f, 0.f, 1, NoForAll, idx);

				glPopMatrix();
			}
			else
			{
				continue;
			}
			break;

		default:
			break;
		}

		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplGlfw_Shutdown();
	glfwTerminate();

	return 0;
}
