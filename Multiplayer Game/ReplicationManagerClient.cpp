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

					packet >> newGO->textureType;

					switch (newGO->textureType)
					{
						case TextureType::TEX_1:
						{
							newGO->texture = App->modResources->spacecraft1;
							break;
						}
						case TextureType::TEX_2:
						{
							newGO->texture = App->modResources->spacecraft2;
							break;
						}
						case TextureType::TEX_3:
						{
							newGO->texture = App->modResources->spacecraft3;
							break;
						}
						case TextureType::TEX_LASER:
						{
							newGO->texture = App->modResources->laser;
							break;
						}
					}

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
}
