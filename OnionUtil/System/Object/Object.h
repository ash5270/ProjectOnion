#pragma once
#include <string>

namespace onion::system::object
{
	class Object
	{
		std::wstring m_allocFlie;
		int m_allocLine;
	};

	class NameObject
	{
		std::wstring m_name;
	public:
		std::wstring& name()
		{
			return m_name;
		}

		void SetName(std::wstring name)
		{
			this->m_name = name;
		}
	};

	class Work
	{
	public:
		virtual void Tick() = 0;
		virtual void Init() {}
		virtual void Suspend() {}
		virtual void Start() {}
		virtual void Stop() {}
	};
}
