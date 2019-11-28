#include "Networks.h"
#include "ReplicationManagerServer.h"
using namespace std;

void ReplicationManagerServer::Create(uint32 networkId)
{
	commands.push_back(new ReplicationCommand(ReplicationAction::Create, networkId));
}

void ReplicationManagerServer::Update(uint32 networkId)
{
	commands.push_back(new ReplicationCommand(ReplicationAction::Update, networkId));
}

void ReplicationManagerServer::Destroy(uint32 networkId)
{
	commands.push_back(new ReplicationCommand(ReplicationAction::Destroy, networkId));
}

void ReplicationManagerServer::Write(OutputMemoryStream& packet)
{

	for (list<ReplicationCommand*>::iterator it = commands.begin(); it != commands.end(); ++it)
	{
		packet << (*it)->networkId;
		packet << (*it)->action;

		GameObject* go = App->modLinkingContext->getNetworkGameObject((*it)->networkId);

		switch ((*it)->action)
		{
		case ReplicationAction::None:
			break;

		case ReplicationAction::Create:
			if (go)
			{
				packet << go->position.x;
				packet << go->position.y;

				packet << go->angle;

				packet << go->order;

				packet << go->pivot.x;
				packet << go->pivot.y;

				packet << go->size.x;
				packet << go->size.y;

				packet << go->color.r;
				packet << go->color.g;
				packet << go->color.b;
				packet << go->color.a;

				packet << go->textureType;

				packet << go->tag;
			}
			break;

		case ReplicationAction::Update:
			if (go)
			{

				packet << go->position.x;
				packet << go->position.y;
				packet << go->angle;
			}
			break;

		case ReplicationAction::Destroy:
			break;

		default:
			break;
		}

		delete* it;
		*it = nullptr;
	}
	commands.clear();

}



