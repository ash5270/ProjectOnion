#include "WorldMap.h"

onion::object::WorldMapCell::WorldMapCell()
{
}

onion::object::WorldMapCell::~WorldMapCell()
{
	m_gameObjects.clear();
}

onion::object::WorldMapCell::WorldMapCell(const math::Rect& rect)
{
	m_rect = rect;
}

void onion::object::WorldMapCell::SetRect(const math::Rect& rect)
{
	m_rect = rect;
}

void onion::object::WorldMapCell::AddGameObject(GameObject* object)
{
	m_gameObjects.push_back(object);
}

void onion::object::WorldMapCell::RemoveGameObject(GameObject* object)
{
	m_gameObjects.remove(object);
}

 onion::object::math::Rect& onion::object::WorldMapCell::GetRect() 
{
	return m_rect;
}

const std::list<onion::object::GameObject*>& onion::object::WorldMapCell::GetGameObjects() const
{
	return m_gameObjects;
}

//-----
//
//-----

onion::object::WorldMap::WorldMap(const math::Rect& rect)
{
	m_rect = rect;

	for(int y=0; y<MAX_MAP_CELL_SIZE_Y; y++)
	{
		for(int x=0; x<MAX_MAP_CELL_SIZE_X; x++)
		{
			int x_size =(abs(rect.leftTop.x) + abs(rect.rightBtm.x))/MAX_MAP_CELL_SIZE_X;
			int y_size =(abs(rect.leftTop.y) + abs(rect.rightBtm.y))/MAX_MAP_CELL_SIZE_Y;

			math::Rect new_rect(math::Vector2(rect.leftTop.x + x * x_size, rect.leftTop.y - y * y_size),
				math::Vector2(rect.leftTop.x + (x+1) * x_size, rect.leftTop.y - (y+1) * y_size));
			m_mapCells[y][x] = new WorldMapCell(new_rect);
		}
	}
}

onion::object::WorldMap::~WorldMap()
{
	for (int y = 0; y < MAX_MAP_CELL_SIZE_Y; y++)
	{
		for (int x = 0; x < MAX_MAP_CELL_SIZE_X; x++)
		{
			delete m_mapCells[y][x];
			m_mapCells[y][x] = nullptr;
		}
	}
}

void onion::object::WorldMap::AddGameObject(GameObject* object)
{
	for (int y = 0; y < MAX_MAP_CELL_SIZE_Y; y++)
	{
		for (int x = 0; x < MAX_MAP_CELL_SIZE_X; x++)
		{
			if (m_mapCells[y][x]->GetRect().BoundRect(object->transform.position))
				m_mapCells[y][x]->AddGameObject(object);
		}
	}
}

void onion::object::WorldMap::UpdateGameObject(GameObject* object)
{

}

void onion::object::WorldMap::RemoveGameObject(GameObject* object)
{
	for (int y = 0; y < MAX_MAP_CELL_SIZE_Y; y++)
	{
		for (int x = 0; x < MAX_MAP_CELL_SIZE_X; x++)
		{
			if (m_mapCells[y][x]->GetRect().BoundRect(object->transform.position))
				m_mapCells[y][x]->RemoveGameObject(object);
		}
	}
}

onion::object::WorldMapCell* onion::object::WorldMap::FindCell(GameObject* object)
{
	for (int y = 0; y < MAX_MAP_CELL_SIZE_Y; y++)
	{
		for (int x = 0; x < MAX_MAP_CELL_SIZE_X; x++)
		{
			if (m_mapCells[y][x]->GetRect().BoundRect(object->transform.position))
				return m_mapCells[y][x];
		}
	}
}

const std::list<onion::object::GameObject*>& onion::object::WorldMap::FindGameObject(const math::Vector2& pos)
{
	for (int y = 0; y < MAX_MAP_CELL_SIZE_Y; y++)
	{
		for (int x = 0; x < MAX_MAP_CELL_SIZE_X; x++)
		{
			if (m_mapCells[y][x]->GetRect().BoundRect(pos))
				return m_mapCells[y][x]->GetGameObjects();
		}
	}
	
	return std::list<GameObject*>(NULL);
}
