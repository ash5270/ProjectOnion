#pragma once
#include <iostream>

namespace onion::util
{
	template<class T>
	class SingleTon
	{
	protected:
		SingleTon(){}
		~SingleTon(){};
	public:
		SingleTon(const SingleTon&);
		SingleTon& operator=(const SingleTon&);

		static T& getInstance()
		{
			static T instance;
			return instance;
		}

	};


}

