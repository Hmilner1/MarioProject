#include "Collisions.h"
#include "Character.h"
Collisions* Collisions::m_instance = nullptr;

//Collisions::Collisions()
//{

//}

Collisions::~Collisions()
{
	m_instance = nullptr;
}
 
Collisions* Collisions::Instance()
{
	if (!m_instance)
	{
		m_instance = new Collisions;
	}
	return m_instance;
}

bool Collisions::Circle(Character* character1, Character* character2)
{


	
}