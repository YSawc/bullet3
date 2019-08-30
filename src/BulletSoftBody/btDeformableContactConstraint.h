/*
 Written by Xuchen Han <xuchenhan2015@u.northwestern.edu>
 
 Bullet Continuous Collision Detection and Physics Library
 Copyright (c) 2019 Google Inc. http://bulletphysics.org
 This software is provided 'as-is', without any express or implied warranty.
 In no event will the authors be held liable for any damages arising from the use of this software.
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it freely,
 subject to the following restrictions:
 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef BT_DEFORMABLE_CONTACT_CONSTRAINT_H
#define BT_DEFORMABLE_CONTACT_CONSTRAINT_H
#include "btSoftBody.h"

class btDeformableContactConstraint
{
public:
    // True if the friction is static
    // False if the friction is dynamic
    bool m_static;
    
    // normal of the contact
    btVector3 m_normal;
    
    btDeformableContactConstraint(const btVector3& normal): m_static(false), m_normal(normal)
    {
    }
    
    btDeformableContactConstraint(bool isStatic, const btVector3& normal): m_static(isStatic), m_normal(normal)
    {
    }
    
    btDeformableContactConstraint(const btDeformableContactConstraint& other)
    : m_static(other.m_static)
    , m_normal(other.m_normal)
    {
        
    }
    btDeformableContactConstraint(){}
    
    virtual ~btDeformableContactConstraint(){}
    
    // solve the constraint with inelastic impulse and return the error, which is the square of normal component of dt scaled velocity diffrerence
    // the constraint is solved by calculating the impulse between object A and B in the contact and apply the impulse.
    // if the object is rigid/multibody apply the impulse to change the velocity,
    // if the object is deformable node, change the according dv.
    virtual btScalar solveConstraint() = 0;
    
    // get the velocity of the object A in the contact
    virtual btVector3 getVa() const = 0;
    
    // get the velocity of the object B in the contact
    virtual btVector3 getVb() const = 0;
    
    // get the velocity change of the soft body node in the constraint
    virtual btVector3 getDv(btSoftBody::Node*) const = 0;
};

class btDeformableStaticConstraint : public btDeformableContactConstraint
{
public:
    const btSoftBody::Node* m_node;
    
    btDeformableStaticConstraint(){}
    
    btDeformableStaticConstraint(const btSoftBody::Node* node): m_node(node), btDeformableContactConstraint(false, btVector3(0,0,0))
    {
    }
    
    btDeformableStaticConstraint(const btDeformableStaticConstraint& other)
    : m_node(other.m_node)
    , btDeformableContactConstraint(other)
    {
        
    }
    
    virtual ~btDeformableStaticConstraint(){}
    
    virtual btScalar solveConstraint()
    {
        return 0;
    }
    
    virtual btVector3 getVa() const
    {
        return btVector3(0,0,0);
    }
    
    virtual btVector3 getVb() const
    {
        return btVector3(0,0,0);
    }
    
    virtual btVector3 getDv(btSoftBody::Node* n) const
    {
        return btVector3(0,0,0);
    }
};

class btDeformableRigidContactConstraint : public btDeformableContactConstraint
{
public:
    btVector3 m_total_normal_dv;
    btVector3 m_total_tangent_dv;
    const btSoftBody::DeformableRigidContact* m_contact;
    
    btDeformableRigidContactConstraint(){}
    btDeformableRigidContactConstraint(const btSoftBody::DeformableRigidContact& c);
    btDeformableRigidContactConstraint(const btDeformableRigidContactConstraint& other);
    virtual ~btDeformableRigidContactConstraint()
    {
    }
    
    // object A is the rigid/multi body, and object B is the deformable node
    virtual btVector3 getVa() const;
    
    virtual btScalar solveConstraint();
};



class btDeformableNodeRigidContactConstraint : public btDeformableRigidContactConstraint
{
public:
    const btSoftBody::Node* m_node;
    
    btDeformableNodeRigidContactConstraint(){}
    btDeformableNodeRigidContactConstraint(const btSoftBody::DeformableNodeRigidContact& contact);
    btDeformableNodeRigidContactConstraint(const btDeformableNodeRigidContactConstraint& other);
    
    virtual ~btDeformableNodeRigidContactConstraint()
    {
        
    }
    
    virtual btVector3 getVb() const;
    
    virtual btVector3 getDv(btSoftBody::Node*) const;
    
    const btSoftBody::DeformableNodeRigidContact* getContact() const
    {
        return static_cast<const btSoftBody::DeformableNodeRigidContact*>(m_contact);
    }
};
#endif /* BT_DEFORMABLE_CONTACT_CONSTRAINT_H */
