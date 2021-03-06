#include "Globals.h"
#include "Application.h"
#include "ModuleCollision.h"


ModuleCollision::ModuleCollision(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	matrix[COLLIDER_SILK][COLLIDER_WALL] = true;
	matrix[COLLIDER_SILK][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_SILK][COLLIDER_CLIENT] = true;
	matrix[COLLIDER_SILK][COLLIDER_ENEMY_SHOT] = true;
	matrix[COLLIDER_SILK][COLLIDER_BATHROOM] = false;
	
	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER_SHOT] = true;
	matrix[COLLIDER_WALL][COLLIDER_ENEMY_SHOT] = true;
	matrix[COLLIDER_WALL][COLLIDER_BATHROOM] = false;
	matrix[COLLIDER_WALL][COLLIDER_CLIENT] = true;
	

	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_CLIENT] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY_SHOT] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_BATHROOM] = true;

	matrix[COLLIDER_CLIENT][COLLIDER_WALL] = true;
	matrix[COLLIDER_CLIENT][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_CLIENT][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_CLIENT][COLLIDER_PLAYER_SHOT] = true;
	matrix[COLLIDER_CLIENT][COLLIDER_ENEMY_SHOT] = false;
	matrix[COLLIDER_CLIENT][COLLIDER_BATHROOM] = true;
	matrix[COLLIDER_CLIENT][COLLIDER_CLIENT] = false;

	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_ENEMY_SHOT] = false;

	matrix[COLLIDER_BATHROOM][COLLIDER_WALL] = false;
	matrix[COLLIDER_BATHROOM][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_BATHROOM][COLLIDER_BATHROOM] = false;

	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_WALL] = true;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_ENEMY_SHOT] = false;
}

// Destructor
ModuleCollision::~ModuleCollision()
{}

// Called before render is available
update_status ModuleCollision::Update()
{
	p2List_item<Collider*>* tmp = colliders.getFirst();

	Collider* c1;
	Collider* c2;

	while(tmp != NULL)
	{
		c1 = tmp->data;

		p2List_item<Collider*>* tmp2 = tmp->next; // avoid checking collisions already checked
		while(tmp2 != NULL)
		{
			c2 = tmp2->data;

			if(c1->CheckCollision(c2->rect) == true)
			{
				if(matrix[c1->type][c2->type] && c1->callback) 
					c1->callback->OnCollision(c1, c2);
				
				if(matrix[c2->type][c1->type] && c2->callback) 
					c2->callback->OnCollision(c2, c1);
			}

			tmp2 = tmp2->next;
		}

		tmp = tmp->next;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		debug = !debug;

	}
	if (debug == true)
		DebugDraw();
	


	return UPDATE_CONTINUE;
}

void ModuleCollision::DebugDraw()
{
	p2List_item<Collider*>* tmp = colliders.getFirst();
	while(tmp != NULL)
	{ 
		App->renderer->DrawQuad(tmp->data->rect, 255, 0, 0, 80, true);
		tmp = tmp->next;
	}
		
}

// Called before quitting
bool ModuleCollision::CleanUp()
{
	LOG("Freeing all colliders");

	p2List_item<Collider*>* item = colliders.getLast();

	while(item != NULL)
	{
		delete item->data;
		item = item->prev;
	}

	colliders.clear();
	return true;
}


Collider* ModuleCollision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, Module* callback)
{
	Collider* ret = new Collider(rect, type, callback);
	colliders.add(ret);
	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(SDL_Rect r) const
{
	return (rect.x < r.x + r.w &&
			rect.x + rect.w > r.x &&
			rect.y < r.y + r.h &&
			rect.h + rect.y > r.y);
}