#pragma once
#include "OnionMath.h"
#include "GameObject.h"
#include "Rect.h"
#include <list>
namespace onion::object {
	class WorldMapCell
	{
	public:
#define MAX_CELL_PLAYER 500
		WorldMapCell();
		~WorldMapCell();
		WorldMapCell(const math::Rect& rect);
		void SetRect(const math::Rect& rect);
		void AddGameObject(GameObject* object);
		void RemoveGameObject(GameObject* object);
		math::Rect& GetRect();
		const std::list<GameObject*>& GetGameObjects() const;
	private:
		math::Rect m_rect;
		std::list<GameObject*> m_gameObjects;
	};
	 
	class WorldMap
	{
	public:
#define MAX_MAP_CELL_SIZE_X 5
#define MAX_MAP_CELL_SIZE_Y 5
#define MAX_MAP_CELL_SIZE (MAX_MAP_CELL_SIZE_X*MAX_MAP_CELL_SIZE_Y)

		WorldMap(const math::Rect& rect);
		~WorldMap();

		void AddGameObject(GameObject* object);
		void UpdateGameObject(GameObject* object);
		void RemoveGameObject(GameObject* object);
		WorldMapCell* FindCell(GameObject* object);
		const std::list<GameObject*>& FindGameObject(const math::Vector2& pos);
	private:
		math::Rect m_rect;
		WorldMapCell* m_mapCells[MAX_MAP_CELL_SIZE_Y][MAX_MAP_CELL_SIZE_X];
	};




}