#include "Networks.h"
#include "ReplicationManagerClient.h"

void ReplicationManagerClient::Read(const InputMemoryStream& packet)
{
	uint32 networkId;
	packet >> networkId;
	ReplicationAction action;
	packet >> action;

	switch (action)
	{
		case ReplicationAction::Create:
		{
			GameObject* newGO = Instantiate();

			if (newGO)
			{
				packet >> newGO->position.x;
				packet >> newGO->position.y;

				packet >> newGO->angle;

				packet >> newGO->order;

				packet >> newGO->pivot.x;
				packet >> newGO->pivot.y;

				packet >> newGO->size.x;
				packet >> newGO->size.y;

				packet >> newGO->color.r;
				packet >> newGO->color.g;
				packet >> newGO->color.b;
				packet >> newGO->color.a;

				packet >> newGO->tag;

				App->modLinkingContext->registerNetworkGameObjectWithNetworkId(newGO, networkId);
			}
			break;
		}

		case ReplicationAction::Destroy:
		{
			GameObject* gameObject = App->modLinkingContext->getNetworkGameObject(networkId);

			if (gameObject)
			{
				App->modLinkingContext->unregisterNetworkGameObject(gameObject);
				Destroy(gameObject);
			}

			break;
		}

		case ReplicationAction::None:
		{
			break;
		}

		case ReplicationAction::Update:
		{
			GameObject* gameObject = App->modLinkingContext->getNetworkGameObject(networkId);

			if (gameObject)
			{
				packet >> gameObject->position.x;
				packet >> gameObject->position.y;

				packet >> gameObject->angle;
			}
			break;
		}
	}

}