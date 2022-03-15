#ifndef SET_UTILITY_H
#define SET_UTILITY_H
#include <vector>
#include <algorithm>
#include <functional>
#include <fstream>
#include <set>

template<typename T>
inline std::vector<T> operator-(std::vector<T> const& first, std::vector<T> const& second)
{
	if (second.empty()) return first;

	std::vector<T> result;
	for (T item : first)
	{
		if (!std::count(std::begin(second), std::end(second), item))
		{
			result.push_back(item);
		}
	}
	return result;
}

template<typename T>
inline std::vector<T> operator+(std::vector<T> const& first, std::vector<T> const& second)
{
	std::vector<T> result = first;
	result.insert(result.end(), second.begin(), second.end());
	return result;
}

template<typename T>
inline std::vector<T>& operator-=(std::vector<T> & first, std::vector<T> const& second)
{
	return first = first - second;
}

template<typename T>
inline std::vector<T>& operator+=(std::vector<T> & first, std::vector<T> const& second)
{
	return first = first + second;
}

template<typename T>
inline std::vector<T>& operator-=(std::vector<T> & first, T const& second)
{
	first.erase(std::remove(first.begin(),first.end(),second));
	return first;
}

template<typename T>
inline std::vector<T> Where(std::vector<T> const& self, std::function<bool(T)> proposition)
{
	std::vector<T> result;
	for (auto& item : self) if (proposition(item)) result.push_back(item);
	return result;
}

template<typename T>
void ReadFromFileToVector(std::ifstream& file, std::vector<T>& vec)
{
	int sz, i;
	T n;

	file >> sz;
	i = 0;
	while (i++ < sz)
	{
		file >> n;
		vec.push_back(n);
	}
}


template<typename T>
void ReadFromFileToSet(std::ifstream& file, std::set<T>& s)
{
	int sz, i;
	T n;

	file >> sz;
	i = 0;
	while (i++ < sz)
	{
		file >> n;
		s.insert(n);
	}
}


#endif // ! SET_UTILITY_H
