#pragma once
#include "set_utility.h"
#include <iostream>


template<typename T>
class Pool
{
public:
	Pool(std::function<T()> create)
		: _create(create)
	{
	}

	T Create()
	{
		if (!_deleted.empty())
		{
			_all.push_back(_deleted.back());
			_deleted.pop_back();
		}
		else
		{
			_all.push_back(_create());
		}
		_all.back()->init();
		return _all.back();
	}

	bool IsExists(T item)
	{
		return std::find(std::begin(_all), std::end(_all), item) != std::end(_all);
	}

	void Delete(T item)
	{
		_all -= item;
		_deleted.push_back(item);
	}

	void DeleteAll(std::function<bool(T)> prop)
	{
		auto deleted = Where(_all, prop);
		_all -= deleted;
		_deleted += deleted;
	}

	void Foreach(std::function<void(T)> func)
	{
		for (auto item : _all)
		{
			func(item);
		}
	}

protected:
	std::vector<T> _all;
	std::vector<T> _deleted;
	std::function<T()> _create;
};
