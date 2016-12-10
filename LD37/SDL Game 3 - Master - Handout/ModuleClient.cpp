#include "Globals.h"
#include "Application.h"
#include "ModuleClient.h"

ModuleClient::ModuleClient(Application* app, bool start_enabled) : Module(app, start_enabled), graphics(NULL)
{
	collider = NULL;
}

ModuleClient::~ModuleClient()
{}

// Load assets
bool ModuleClient::Start()
{
	LOG("Loading Clients");
	current_animation = NULL;
	graphics = App->textures->Load("rtype/ship.png");
	// idle animation normal client
	normal.idle.frames.PushBack({ 66, 1, 32, 14 });
	return true;
}

// Unload assets
bool ModuleClient::CleanUp()
{
	LOG("Unloading clients");
	App->textures->Unload(graphics);
	return true;
}

// Update: draw background
update_status ModuleClient::Update()
{
	p2List_item<Client*>* tmp = active.getFirst();
	p2List_item<Client*>* tmp_next = active.getFirst();
	current_animation = &normal.idle;

	while (tmp != NULL)
	{
		Client* c = tmp->data;
		tmp_next = tmp->next;

		if (c->Update() == false)
		{
			active.del(tmp);
			delete c;
		}
		else if (SDL_GetTicks() >= c->born)
		{
			App->renderer->Blit(graphics, c->position.x, c->position.y, &(current_animation->GetCurrentFrame()));
			if (c->fx_played == false)
			{
				c->fx_played = true;
				App->audio->PlayFx(c->fx);
			}
		}

		tmp = tmp_next;
	}

	return UPDATE_CONTINUE;
}

// Collision detection
void ModuleClient::OnCollision(Collider* c1, Collider* c2)
{

	p2List_item<Client*>* tmp = active.getFirst();


	while (tmp != NULL)
	{

		Collider* aux = tmp->data->collider;

		//Colision player cliente.
		if (aux == c1 && c2->type == COLLIDER_PLAYER)
		{
			App->player->giveTowel = true;
			break;
		}
	}

	tmp = tmp->next;

}
void ModuleClient::AddClient(const Client& client, int x, int y, COLLIDER_TYPE collider_type, Uint32 delay)
{
	Client* p = new Client(client);
	p->born = SDL_GetTicks() + delay;
	p->position.x = x;
	p->position.y = y;
	

	if (collider_type != COLLIDER_NONE)
	{
		p->collider = App->collision->AddCollider({ p->position.x, p->position.y, 32, 14 }, collider_type, this);
	}

	active.add(p);
}

Client::Client()
{
	
}

Client::Client(const Client& c) 
{
	fx = c.fx;
	born = c.born;
	life = c.life;
	timeBath = c.timeBath;
	timeWaiting = c.timeWaiting;
	timeSink = c.timeSink;
	
}


bool Client::Update()
{




	return true;
}

p2Point<int> Client::GoToPosition(p2Point<int> target)
{


	float max_speed = 1.0f;

	int length = position.Length();

	int distance = position.DistanceTo(target);

	p2Point<int> vec = p2Point<int>(target);
	vec -= position;

	//normalizar y escalar
	p2Point<int> velocity =  vec.Normalize().Scale(max_speed);

	return velocity;
}

p2Point<int> Client::SearchBath()
{
	
	//Si tenemos un ba�o asignado
	if (ocuppied != false) 
	{
		return GoToPosition(assignedBath->position);
	}
	return GoToPosition(position); // sino devolver mi position
}

void Client::WaitForBath()
{
	waiting = true;
	//Sleep?
	Sleep(4000);
	//Cambiar animacion a quejarse;
	waiting = false;
}



void ModuleClient::AssignBaths() 
{
	p2List_item<Client*>* tmp = active.getFirst();
	p2List_item<Client*>* tmp_next = active.getFirst();


	while (tmp != NULL)
	{
		//Cogemos el cliente siguiente
		Client* c = tmp->data;
		tmp_next = tmp->next;

		//Si no tiene ba�o asignado, comprobamos que no este esperando quejandose
		if (c->assignedBath == NULL && c->waiting == false) 
		{
			// buscamos entre todos los ba�os
			p2List_item<Bath*>* tmp2 = App->bathrooms->active.getFirst();
			p2List_item<Bath*>* tmp_next2 = App->bathrooms->active.getFirst();

			while (tmp2 != NULL)
			{
				Bath* b = tmp2->data;
				tmp_next2 = tmp2->next;

				//asignamos el 
				if (b->busy == false) 
				{
					c->assignedBath = b;
					c->ocuppied = true;
					break;
				}

				tmp2 = tmp_next2;

			}

			//si no se ha podido asignar ningun ba�o al cliente: Esperar y quejarse
			if ((tmp2 == NULL) && (c->assignedBath == NULL))
			{
				// HACER QUE ESPERE EL CLIENTE: sleep(4000) for eixample
				c->WaitForBath();

			}

			//Si se ha podido asignar un ba�o al cliente: hacer un searchPosition
			if (c->assignedBath != NULL)
			{
				//c ocupado hasta el final
				//c->SearchBath();
			}

		}

		


		tmp = tmp_next;
	}
	



}
Client* ModuleClient:: getClient(p2Point<int> pos)
{
	p2List_item<Client*>* tmp = active.getFirst();
	p2List_item<Client*>* tmp_next = active.getFirst();


	while (tmp != NULL)
	{
		Client* c = tmp->data;
		tmp_next = tmp->next;

		if (c->Update() == false)
		{
			active.del(tmp);
			delete c;
		}
		
		if (c->position.x == pos.x && c->position.y == pos.y)
		{
			return c;
		}
			

		tmp = tmp_next;
	}
	return NULL;

}


