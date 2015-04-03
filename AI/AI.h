#pragma once

#include "Bindable.h"

// Current AI is no longer available.
class LostTarget : public std::exception
{
    virtual const char* what() const throw() { return "Lost target."; }
};

class Creature;

class AI: public Bindable
{
public:
	AI(Creature* pOwner);
	virtual ~AI();
};
