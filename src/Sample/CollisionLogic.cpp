#include "CollisionLogic.h"
#include "Collider.h"
#include <iostream>

void CollisionLogic::OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide side)
{
	switch (side)
	{
	case None:
		std::cout << "None" << std::endl;
		break;
	case Left:
		std::cout << "Left" << std::endl;
		break;
	case Right:
		std::cout << "Right" << std::endl;
		break;
	case Top:
		std::cout << "Top" << std::endl;
		break;
	case Bottom:
		std::cout << "Bottom" << std::endl;
		break;
	}
}
