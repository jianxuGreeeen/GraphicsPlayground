#pragma once
#include <utility>
/*
* Since direct 3d objects needs to be released,
* I didn't want to keep managing release calls all over.
* This is a wrapper class to do it for me.
*/
template <class T>
class GraphicsInterfaceObject final
{
public:
	void Release()
	{
		if (Ptr != nullptr)
		{
			Ptr->Release();
			Ptr = nullptr;
		}
	}

	GraphicsInterfaceObject() = default;
	explicit GraphicsInterfaceObject(T* aPtr)
		: Ptr(aPtr)
	{
	}

	explicit GraphicsInterfaceObject(GraphicsInterfaceObject<T>&& aObj)
	{
		this = std::move(aObj);
	}

	explicit GraphicsInterfaceObject(GraphicsInterfaceObject<T>& aObj)
	{
		this = aObj;
	}

	~GraphicsInterfaceObject()
	{
		Release();
	}

	GraphicsInterfaceObject& operator = (T* aPtr)
	{
		Release();
		Ptr = aPtr;
		return *this;
	}

	GraphicsInterfaceObject& operator = (GraphicsInterfaceObject<T>&& aObj)
	{
		Release();
		Ptr = aObj.aPtr;
		aObj.Ptr = nullptr;
		return *this;
	}

	GraphicsInterfaceObject& operator = (GraphicsInterfaceObject<T>& aObj)
	{
		Release();
		Ptr = aObj.Ptr;
		aObj.Ptr = nullptr;
		return *this;
	}

	T& operator * ()
	{
		return *Get();
	}

	T* operator -> ()
	{
		return Get();
	}

	inline T* Get() { return Ptr; }

private:
	T* Ptr = nullptr;
};

