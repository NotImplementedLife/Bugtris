#pragma once

#include <AstralbrewLib>

#define mk_block(color, design) (0x80|(((color)<<2)|(design)))

class Mesh
{
private:
	int _x;
	int _y;
	int _width;
	int _height;
	u8* data;
	
	void copy(const u8* src, int w, int h, int dx, int dy, bool overwrite = false);
public:	
	Mesh();
	Mesh(int x, int y, int width, int height);
	Mesh(int x, int y, int width, int height, const u8* const source_table);
	
	Mesh(const Mesh&);
	Mesh(Mesh&&);
	
	Mesh& operator = (const Mesh&);
	Mesh& operator = (Mesh&&);
	
	void resize(int new_width, int new_height, bool clear = false);
	
	void copy(const Mesh& src, int dx, int dy, bool overwrite = false);
	
	Mesh& operator += (const Mesh&);
	bool operator || (const Mesh&) const;
	
	int width() const { return _width; }
	int height() const { return _height; }
	
	int coord_at(int cx, int cy) const;
	
	void replace(int v, int w);
	
	int at(int px, int py) const;
	
	int x() const { return _x; }
	int y() const { return _y; }
	
	void set_x(int _x) { this->_x = _x; }	
	void set_y(int _y) { this->_y = _y; }	
	
	void move(int _x, int _y) { set_x(x()+_x); set_y(y()+_y); } 
	
	void rotate_cw();
	void rotate_ccw();
	
	~Mesh();
};