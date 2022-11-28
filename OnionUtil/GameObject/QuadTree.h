#pragma once
#include <list>
#include "GameObject.h"
#include "Rect.h"
namespace onion::object
{
	class QuadTree
	{
#define MIN_QUAD_DIMENSION 5
	public:
		QuadTree(const math::Vector2& leftTop, const math::Vector2& rightBtm);
		~QuadTree();

		bool Insert(GameObject* game_object);
		bool Remove(GameObject* game_object);
		bool GetGameObject(std::list<GameObject*>&obj, const math::Rect& rect);
		math::Rect rect;
	private:
		std::list<GameObject*> m_gameObjects;

		QuadTree* m_leftTopTree = nullptr;
		QuadTree* m_leftBtmTree = nullptr;
		QuadTree* m_rightTopTree = nullptr;
		QuadTree* m_rightBtmTree = nullptr;



		void CreateChildren();

	};

}
