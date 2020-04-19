#include "Editor.h"


Editor& Editor::Get()
{
	static Editor* impl = new Editor();
	return *impl;
}
