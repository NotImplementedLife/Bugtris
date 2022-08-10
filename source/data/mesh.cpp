#include "data/mesh.hpp"

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
		int mw = _width<new_width ? _width : new_width;
		int mh = _height<new_height ? _height : new_height;
		
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

#define max(a,b) ((a)>(b) ? (a):(b))
#define min(a,b) ((a)<(b) ? (a):(b))

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

Mesh::~Mesh()
{	
	delete[] data;
}


