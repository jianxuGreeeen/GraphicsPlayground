#pragma once

namespace Common
{
	template <class T>
	void Release(T*& arPtr)
	{
		if (arPtr != nullptr)
		{
			arPtr->Release();
			arPtr = nullptr;
		}
	}
}
