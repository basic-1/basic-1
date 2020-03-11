/*
 BASIC1 interpreter
 Copyright (c) 2020 Nikolay Pletnev
 MIT license

 exvar.cpp: implementing variables cache
*/


#include <cstdlib>
#include <map>

extern "C"
{
#include "b1var.h"
#include "b1err.h"
}


static std::map<B1_T_IDHASH, B1_NAMED_VAR> b1_ex_vars;


extern "C" B1_T_ERROR b1_ex_var_init()
{
	b1_ex_vars.clear();

	return B1_RES_OK;
}

// creates new variable, fills B1_ID structure only (hash, arg. num)
extern "C" B1_T_ERROR b1_ex_var_alloc(B1_T_IDHASH name_hash, uint8_t new_var_flags, B1_NAMED_VAR **var)
{
	B1_T_ERROR err = B1_RES_OK;
	B1_NAMED_VAR *newvar;
	auto var_it = b1_ex_vars.find(name_hash);
	
	if(var_it != b1_ex_vars.end())
	{
		newvar = &var_it->second;
		err = B1_RES_EIDINUSE;
	}
	else
	{
		newvar = &b1_ex_vars[name_hash];
		newvar->id.flags = new_var_flags;
		newvar->id.name_hash = name_hash;
	}

	if(var != NULL)
	{
		*var = newvar;
	}

	return err;
}

// frees memory occupied by B1_NAMED_VAR structure
extern "C" B1_T_ERROR b1_ex_var_free(B1_NAMED_VAR *var)
{
	for(auto var_it = b1_ex_vars.begin(); var_it != b1_ex_vars.end(); var_it++)
	{
		if(var_it->first == var->id.name_hash)
		{
			b1_ex_vars.erase(var_it);
			break;
		}
	}

	return B1_RES_OK;
}
