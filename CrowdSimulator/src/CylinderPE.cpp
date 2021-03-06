#include "CylinderPE.h"

void CylinderPE::checkCollision(Agent *_a1, Agent *_a2)
{
    float distance = _a1->distance(_a2);
    float radiusSum = _a1->getCollisionRadius()+_a2->getCollisionRadius();

    if (distance < s_epsilon)
    {
        std::cout << "CylinderPE: WARNING: Agent " << _a1->getAgentID()
                  << " and " << _a2->getAgentID() << " are overlapped" << std::endl;
        return;
    }

    if (distance < radiusSum)
    {
        //THERE IS COLLISION

        ngl::Vec4 distanceVector;
        distanceVector.m_x = _a2->getPosition().m_x - _a1->getPosition().m_x;
        distanceVector.m_z = _a2->getPosition().m_z - _a1->getPosition().m_z;
        //vector tangent to the collision
        ngl::Vec4 tangentVector;
        tangentVector.m_x = - distanceVector.m_z;
        tangentVector.m_z = distanceVector.m_x;
        //normalize
        tangentVector = tangentVector / tangentVector.length();

        float magnitude;
        float step = Agent::getStep();
        float angleA1, angleA2;
        // angleBetween returns in radians, not degrees.
        // NGL doc is mistaken

        //FIXING AGENT1
        ngl::Vec4 velocityA1;
        velocityA1 = _a1->getVelocity();

        if (velocityA1.length() > 0)
        {
            angleA1 = velocityA1.angleBetween(distanceVector);

            if (angleA1 < M_PI/2.0)
            {
                // FIX!
                // fixing velocity of Agent1
                magnitude= velocityA1.dot(tangentVector);
                velocityA1 = magnitude*tangentVector;
                _a1->setVelocity(velocityA1);
                // fixing position of Agent1
                _a1->setPosition(_a1->getPreviousPos()+velocityA1*step);
            }
        }

        //FIXING AGENT2
        ngl::Vec4 velocityA2;
        velocityA2 = _a2->getVelocity();

        if (velocityA2.length() > 0)
        {
            angleA2 = velocityA2.angleBetween(-distanceVector);

            if (angleA2 < M_PI/2.0)
            {
                //FIX!
                //fixing velocity of Agent2
                magnitude = velocityA2.dot(tangentVector);
                velocityA2 = magnitude*tangentVector;
                _a2->setVelocity(velocityA2);
                // fixing position of Agent2
                _a2->setPosition(_a2->getPreviousPos()+velocityA2*step);
            }
        }

        //std::cout << "COLLISION; "
        //          << a1->getAgentID() << " angleA1: " << angleA1 << "\t"
        //          << a2->getAgentID() << " angleA2: " << angleA2 << std::endl;

    }
}
