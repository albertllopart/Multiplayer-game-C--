#include "Networks.h"
#include "ReplicationManagerClient.h"

void ReplicationManagerClient::Read(const InputMemoryStream& packet)
{
	while (packet.RemainingByteCount() > sizeof(uint32))
	{
		uint32 networkId;
		packet >> networkId;
		ReplicationAction action;
		packet >> action;

		switch (action)
		{
			case ReplicationAction::Create:
			{
				vec2 position;
				float angle;
				vec4 color;

				packet >> position.x;
				packet >> position.y;

				packet >> angle;

				packet >> color.r;
				packet >> color.g;
				packet >> color.b;
				packet >> color.a;

				GameObject* newGO = Instantiate();

				if (newGO)
				{
					packet >> newGO->position;

					packet >> newGO->angle;

					//packet >> newGO->order;

					//packet >> newGO->pivot.x;
					//packet >> newGO->pivot.y;

					//packet >> newGO->size.x;
					//packet >> newGO->size.y;

					packet >> newGO->color;

					App->modLinkingContext->registerNetworkGameObjectWithNetworkId(newGO, networkId);
				}
				break;
			}

			case ReplicationAction::Destroy:
			{
				GameObject* GO = App->modLinkingContext->getNetworkGameObject(networkId);

				if (GO)
				{
					App->modLinkingContext->unregisterNetworkGameObject(GO);
					Destroy(GO);
				}

				break;
			}

			case ReplicationAction::None:
			{
				break;
			}

			case ReplicationAction::Update:
			{
				vec2 position;
				float angle;
				vec4 color;

				packet >> position.x;
				packet >> position.y;

				packet >> angle;

				packet >> color.r;
				packet >> color.g;
				packet >> color.b;
				packet >> color.a;

				GameObject* GO = App->modLinkingContext->getNetworkGameObject(networkId);

				if (GO)
				{
					packet >> GO->position;
					packet >> GO->angle;
					packet >> GO->color;
				}
				break;
			}
		}
	}
}
