#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Module.h"
#include "Dummy.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleBath.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneIntro.h"
#include "ModuleParticles.h"
#include "ModuleCollision.h"
#include "ModuleClient.h"
#include "ModuleAI.h"
#include "ModuleBathroom.h"
#include "ModuleSilk.h"
#include "ModuleCloset.h"
#include "ModulePoints.h"



class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleBath* bath_scene;
	ModulePlayer* player;
	ModuleClient* client;
	ModuleFadeToBlack* fade;
	ModuleSceneIntro* scene_intro;
	ModuleParticles* particles;
	ModuleBathroom* bathrooms;
	ModuleSilk* silks;
	ModuleCollision* collision;
	ModuleAI* ai;
	ModuleCloset* closet;
	ModulePoints* point;


private:

	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
};