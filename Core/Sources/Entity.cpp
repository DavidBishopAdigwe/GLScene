#include "Core/Headers/Entity.h"

 size_t EntityMemoryPool::m_maxEntities{};

Entity EntityManager::createEntity(const std::string& tag)
{
	auto& pool = EntityMemoryPool::Instance();
	int entityId = static_cast<int>(pool.getAvailableId());
	Entity entity{entityId};
	pool.addEntity(entityId, tag);
	entities.push_back(entity);

	++m_totalEntities;
	return entity;
}

int Entity::getID() const
{
	return m_id;
}


const std::string& Entity::getTag() const
{
	return EntityMemoryPool::Instance().m_entityTags[m_id];
}
