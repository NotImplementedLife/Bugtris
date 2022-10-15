#pragma once

#include "bob.h"
#include "cappuccino.h"
#include <string.h>

struct _CatsPicsCollectionItem
{
	const unsigned int* tiles;
    const unsigned short* palette;
};

struct _CatRecord {const char* name; const unsigned int* tiles; const unsigned short* pal;};

template<int N>
struct _CatsPicsCollection
{	
    
    constexpr _CatsPicsCollection(const _CatRecord (&src)[N]) : _name(), _tiles(), _pal()
    {
        for(int i=0;i<N;i++)
        {
            _name[i]=src[i].name;
            _tiles[i]=src[i].tiles;
            _pal[i]=src[i].pal;
        }
    }
    const char* _name[N];
    const unsigned int* _tiles[N];
    const unsigned short* _pal[N];
    
	constexpr _CatsPicsCollectionItem operator[] (const char* name) const
	{
		if(name==nullptr) return {nullptr, nullptr};
		for(int i=0;i<N;i++)
		{
			if(!strcmp(name, _name[i]))
			{
				return {_tiles[i], _pal[i]};
			}
		}
		return {nullptr, nullptr};
	}
	
    constexpr unsigned int* get_tiles(const char* name) const
	{
		if(name==nullptr) return nullptr;
		for(int i=0;i<N;i++)
		{
			if(!strcmp(name, _name[i]))
			{
				return _tiles[i];
			}
		}
		return nullptr;
	}
	
	constexpr unsigned int* get_palette(const char* name) const
	{
		if(name==nullptr) return nullptr;
		for(int i=0;i<N;i++)
		{
			if(!strcmp(name, _name[i]))
			{
				return _pal[i];
			}
		}
		return nullptr;
	}
};

constexpr _CatRecord CatRecords[] = {
	{"Cappuccino", cappuccinoTiles, cappuccinoPal},
	{"Bob", bobTiles, bobPal},
};

constexpr _CatsPicsCollection CatsPics {CatRecords};
