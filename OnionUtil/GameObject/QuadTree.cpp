#include "QuadTree.h"
#include <cmath>
onion::object::QuadTree::QuadTree(const math::Vector2& leftTop, const math::Vector2& rightBtm)
{
	rect.leftTop = leftTop;
	rect.rightBtm = rightBtm;

	CreateChildren();
}

onion::object::QuadTree::~QuadTree()
{
}

bool onion::object::QuadTree::Insert(GameObject* game_object)
{
	math::Vector3 pos = game_object->transform.position;
	if (!rect.BoundRect(pos))
		return false;
	m_gameObjects.push_back(game_object);

	if(m_leftTopTree!=nullptr)
	{
		m_leftTopTree->Insert(game_object);
		m_leftBtmTree->Insert(game_object);
		m_rightBtmTree->Insert(game_object);
		m_rightTopTree->Insert(game_object);
	}
	return true;
}

bool onion::object::QuadTree::Remove(GameObject* game_object)
{
	math::Vector3 pos = game_object->transform.position;
	if (!rect.BoundRect(pos))
		return false;
	m_gameObjects.remove(game_object);
	if (m_leftTopTree != nullptr)
	{
		m_leftTopTree->Remove(game_object);
		m_leftBtmTree->Remove(game_object);
		m_rightBtmTree->Remove(game_object);
		m_rightTopTree->Remove(game_object);
	}
	return true;
}

bool onion::object::QuadTree::GetGameObject(std::list<GameObject*>& obj, const math::Rect& rect_pos)
{
	if(!rect.BoundRect(rect_pos))
		return false;
	if (m_leftTopTree == nullptr)
		return false;
	if(m_leftTopTree->rect.BoundRect(rect_pos))
	{
		return m_leftTopTree->GetGameObject(obj, rect_pos);
	}

	if (m_leftBtmTree->rect.BoundRect(rect_pos))
	{
		return m_leftBtmTree->GetGameObject(obj, rect_pos);
	}

	if (m_rightTopTree->rect.BoundRect(rect_pos))
	{
		return m_rightTopTree->GetGameObject(obj, rect_pos);
	}

	if (m_rightBtmTree->rect.BoundRect(rect_pos))
	{
		return m_rightBtmTree->GetGameObject(obj, rect_pos);
	}

	obj = m_gameObjects;
	return true;
}

void onion::object::QuadTree::CreateChildren()
{
	int mid_x = (rect.leftTop.x + rect.rightBtm.x) / 2;
	int mid_y = (rect.leftTop.y + rect.rightBtm.y) / 2;

	int check_x = rect.leftTop.x - mid_x;
	int check_y = rect.leftTop.y - mid_y;

	if (abs(check_x) < 2 * MIN_QUAD_DIMENSION || abs(check_y) < 2 * MIN_QUAD_DIMENSION)
		return;
	m_leftTopTree = new  QuadTree(rect.leftTop, math::Vector2(mid_x, mid_y));
	m_leftBtmTree = new  QuadTree(math::Vector2(rect.leftTop.x, mid_y), math::Vector2(mid_x, rect.rightBtm.y));
	m_rightTopTree = new  QuadTree(math::Vector2(mid_x, rect.leftTop.y), math::Vector2(rect.rightBtm.x, mid_y));
	m_rightBtmTree = new  QuadTree(math::Vector2(mid_x, mid_y), rect.rightBtm);

}
