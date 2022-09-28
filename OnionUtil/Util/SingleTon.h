#pragma once
#include <iostream>

namespace onion::util
{
	template<class T>
	class SingleTon
	{
	protected:
		SingleTon(){}
		virtual  ~SingleTon(){}
	public:
		SingleTon(const SingleTon&) =delete;
		SingleTon& operator=(const SingleTon&) = delete;

		static T& getInstance()
		{
			static T instance;
			return instance;
		}

	};


}

