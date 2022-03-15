#ifndef ICOLORIZABLE_H
#define ICOLORIZABLE_H
#include <set>


class IColorizable
{
public:
	virtual void ApplyTheme() = 0;

	static void ApplyThemeToAll() { for (auto colorizable : _all_colorizables) colorizable->ApplyTheme(); }
protected:
	static std::set<IColorizable*> _all_colorizables;
};


#endif // ! ICOLORIZABLE_H
