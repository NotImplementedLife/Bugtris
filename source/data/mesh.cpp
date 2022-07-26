#include "data/mesh.hpp"

using namespace Astralbrew;

Mesh::Mesh() : _x(0), _y(0), _width(0), _height(0) { data = nullptr; }

Mesh::Mesh(int x, int y, int width, int height)
	: _x(x), _y(y), _width(width), _height(height)
{	
	data = new u8[_width*_height];			
	for(int i=0;i<_width*_height;i++) data[i]=0;
}

Mesh::Mesh(int x, int y, int width, int height, const u8* const source_table)
	: Mesh(x,y,width, height)
{
	for(int i=0;i<width*height;i++)
	{
		data[i]=source_table[i];
	}
}

Mesh::Mesh(const Mesh& m) : _x(m._x), _y(m._y), _width(m._width), _height(m._height)
{		
	data = new u8[_width*_height];	
	for(int i=0;i<_width*_height;i++)
		data[i]=m.data[i];
}

Mesh::Mesh(Mesh&& m) : _x(m._x), _y(m._y), _width(m._width), _height(m._height)
{	
	data = m.data;	
	m.data = nullptr;
	m._x = m._y = m._width = m._height = 0;
}

Mesh& Mesh::operator = (const Mesh& m)
{
	_x = m._x;
	_y = m._y;
	_width = m._width;
	_height = m._height;
	data = new u8[_width*_height];
	for(int i=0;i<_width*_height;i++)
		data[i]=m.data[i];
	return *this;
	
}

Mesh& Mesh::operator = (Mesh&& m)
{
	_x = m._x;
	_y = m._y;
	_width = m._width;
	_height = m._height;
	data = m.data;
	m.data = nullptr;
	return *this;
}

void Mesh::resize(int new_width, int new_height, bool clear)
{
	u8* new_data = new u8[new_width*new_height];		
	for(int i=0;i<new_width*new_height;i++) new_data[i]=0;
	
	if(!clear)
	{
		int mw = min(_width, new_width);
		int mh = min(_height,new_height);
		
		for(int iy = 0; iy < mh; iy++)
		{
			for(int ix = 0; ix < mw; ix++)
				new_data[new_width * iy + ix] = data[_width * iy + ix];
		}	
	}		
	delete[] data;
	data = new_data;
	_width = new_width;
	_height = new_height;
}

int Mesh::at(int px, int py) const
{
	return data[py*_width+px];
}

void Mesh::copy(const u8* src, int w, int h, int dx, int dy, bool overwrite)
{	
	for(int iy = 0; iy < h; iy++)
	{
		for(int ix=0; ix < w; ix++)
		{
			int rx=dx+ix;
			int ry=dy+iy;
			if(rx<_width && ry<_height)
			{
				if(data[ry*_width+rx]==0 || overwrite)
					data[ry*_width+rx] = src[iy*w+ix];
			}
		}
	}
}

void Mesh::copy(const Mesh& src, int dx, int dy, bool overwrite)
{	
	for(int iy = 0; iy < src.height(); iy++)
	{
		for(int ix=0; ix < src.width(); ix++)
		{
			int rx=dx+ix;
			int ry=dy+iy;
			if(rx<_width && ry<_height)
			{
				if(data[ry*_width+rx]==0 || overwrite)
					data[ry*_width+rx] = src.at(ix, iy);
			}
		}
	}
}

int Mesh::coord_at(int cx, int cy) const
{
	if(cx<x() || cy<y() || cx>=x()+width() || cy>=y()+height())
		return 0;
	return data[(cy-y())*width() + cx-x()];
}

Mesh& Mesh::operator += (const Mesh& rhs)
{
	int x1 = min(x(), rhs.x());
	int y1 = min(y(), rhs.y());
	int x2 = max(x()+width(), rhs.x()+rhs.width());
	int y2 = max(y()+height(), rhs.y()+rhs.height());
		
	Mesh m=(Mesh&&)(*this);
	
	_x = x1;
	_y = y1;
	resize(x2-x1, y2-y1, true);
	copy(m, m.x()-x1, m.y()-y1);
	copy(rhs, rhs.x()-x1, rhs.y()-y1);
		
	return *this;
}

bool Mesh::operator || (const Mesh& m) const
{
	if(x()+width() < m.x()+1) return false;
	if(m.x()+m.width() < x()+1) return false;
	
	if(y()+height() < m.y()+1) return false;
	if(m.y()+m.height() < y()+1) return false;
	
	int nrc = 0;
	
	for(int iy=0;iy<height();iy++)
	{		
		for(int ix=0;ix<width();ix++)
		{
			int cy = y()+iy;
			int cx = x()+ix;
			if(at(ix, iy))
			{
				if(m.coord_at(cx, cy)) return false;
				if(m.coord_at(cx-1, cy) || m.coord_at(cx+1, cy)
				|| m.coord_at(cx, cy-1) || m.coord_at(cx, cy+1))
				{
					nrc++;
				}
			}			
		}
	}	
	
	return nrc>0;
}

bool Mesh::operator && (const Mesh& m) const
{
	for(int iy=0;iy<height();iy++)
	{		
		for(int ix=0;ix<width();ix++)
		{
			int cy = y()+iy;
			int cx = x()+ix;
			if(at(ix, iy) && m.coord_at(cx, cy)) 
				return true;			
		}
	}
	return false;
}

void Mesh::replace(int v, int w)
{
	for(int iy=0;iy<height();iy++)
	{		
		for(int ix=0;ix<width();ix++)
		{
			if(data[iy*width()+ix]==v)
			{
				data[iy*width()+ix]=w;
			}
		}
	}
}

void Mesh::set_all_values(int v)
{
	for(int iy=0;iy<height();iy++)
	{		
		for(int ix=0;ix<width();ix++)
		{
			if(data[iy*width()+ix])
			{
				data[iy*width()+ix]=v;
			}
		}
	}	
}

void Mesh::rotate_ccw()
{	
	assert(width()==height());
	
	const int L = width()-1;
	for(int iy=0;iy<=L/2;iy++) 
	{
		for(int ix=0;ix<=L/2;ix++)
		{
			Astralbrew::circular_perm_left(
				data[  iy  * width() + ix  ],
				data[  ix  * width() + L-iy],
				data[(L-iy)* width() + L-ix],
				data[(L-ix)* width() + iy  ]
			);
		}
	}
}

void Mesh::rotate_cw()
{
	assert(width()==height());
	
	const int L = width()-1;
	for(int iy=0;iy<=L/2;iy++) 
	{
		for(int ix=0;ix<=L/2;ix++)
		{
			Astralbrew::circular_perm_left(
				data[(L-ix)* width() + iy  ],
				data[(L-iy)* width() + L-ix],
				data[  ix  * width() + L-iy],
				data[  iy  * width() + ix  ]					
			);
		}
	}		
}

int Mesh::clear_full_lines(int w)
{		
	if(width()<w) return 0;	
		
	
	int result=0;
	for(int iy=height()-1;iy>=0;iy--) 
	{
		int count=0;
		for(int ix=0;ix<width();ix++)
			if(data[iy*width()+ix]) count++;
		if(count==w)
		{
			result++;
		
			for(int jy=iy; jy>0; jy--) 
			{
				for(int ix=0;ix<width();ix++) 
				{
					data[jy*width()+ix] = data[(jy-1)*width()+ix];
				}
			}	
			for(int ix=0;ix<width();ix++) 
			{
				data[ix] = 0;
			}
			iy++;
		}
	}	
	return result;
}

int Mesh::count_full_lines(int w) const
{
	if(width()<w) return 0;	
		
	
	int result=0;
	for(int iy=height()-1;iy>=0;iy--) 
	{
		int count=0;
		for(int ix=0;ix<width();ix++)
			if(data[iy*width()+ix]) count++;
		if(count==w)
		{
			result++;			
		}
	}	
	return result;
}

Mesh::~Mesh()
{	
	delete[] data;
}

const u8 mesh_gfx[7][16] = 
{
	{
		0,1,0,0,
		0,1,0,0,
		0,1,0,0,
		0,1,0,0,
	},
	{
		0,0,0,0,
		0,1,1,0,
		0,1,1,0,
		0,0,0,0,
	},
	{
		0,0,0,0,
		1,1,0,0,
		0,1,1,0,
		0,0,0,0,
	},
	{
		0,0,0,0,
		0,0,1,1,
		0,1,1,0,
		0,0,0,0,
	},
	{
		0,1,1,0,
		0,1,0,0,
		0,1,0,0,
		0,0,0,0,
	},
	{
		0,1,1,0,
		0,0,1,0,
		0,0,1,0,
		0,0,0,0,
	},
	{
		0,0,0,0,
		0,1,1,1,
		0,0,1,0,
		0,0,0,0,
	},
};
