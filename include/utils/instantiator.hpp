#pragma once

class Instantiator
{
public:
	template <class C, class R>
	static R* instantiate()
	{
		return new C;
	}
	
	template<class R>
	using FunctionType = R* (*)(void);
};