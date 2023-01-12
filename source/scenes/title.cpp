#include "scenes/title.hpp"

void TitleScene::init()
{
	Memory::clear((void*)0x06000000, 0x18000);
	GenericScene::init();
	
	first_piece_tile_id = translate_tile_id(3, pieces_gfx, 0);
	create_meshes();
	
	int vwf_color = reserve_background_color(Colors::White);
	
	vwf.set_render_space(dialog_bg->resource->offset(),2,10);
	VwfEngine::prepare_map(vwf, (unsigned short*)Video::bgGetMapPtr(1), 32, 10, 14, vwf_color>>4);
	vwf.put_text(" Press START", Video::Pal4bit, SolidColorBrush(vwf_color & 0xF));
	
	schedule_task(&switch_start_visibility_task);
	
	key_down.add_event(&TitleScene::on_key_down, this);
	
	Video::bgSetScroll(3, 8, 0);
	Video::bgSetScroll(2, 0, 0);
	Video::bgUpdate();	
}

void TitleScene::frame()
{
	for(int i=0;i<meshes.size();i++)
	{
		draw_mesh(meshes[i]);		
	}	
	
	Video::bgScroll(2, 1, 0);
	Video::bgUpdate();
	GenericScene::frame();
}

void TitleScene::on_key_down(void* sender, void* _keys) 
{
	int keys = (int)_keys;
	if(keys & Keys::Start)
	{
		close()->next(nullptr);
	}
}

TitleScene::~TitleScene()
{
	unschedule_task(&switch_start_visibility_task);
	key_down.remove_event(&TitleScene::on_key_down, this);
}

void TitleScene::create_meshes()
{
	meshes.push_back(Mesh(0,2,4,4,mesh_gfx[4]));
	meshes.back().rotate_cw();
	meshes.back().replace(1, mk_block(3,3));	
	
	meshes.push_back(Mesh(0,4,4,4,mesh_gfx[6]));
	meshes.back().rotate_ccw();
	meshes.back().replace(1, mk_block(1,2));
	
	meshes.push_back(Mesh(0,5,4,4,mesh_gfx[5]));
	meshes.back().rotate_cw();
	meshes.back().replace(1, mk_block(4,1));
	
	
	meshes.push_back(Mesh(4,4,4,4,mesh_gfx[5]));
	meshes.back().rotate_cw();
	meshes.back().rotate_cw();
	meshes.back().replace(1, mk_block(2,0));
		
	meshes.push_back(Mesh(6,4,4,4,mesh_gfx[0]));	
	meshes.back().replace(1, mk_block(5,1));
	
	
	meshes.push_back(Mesh(9,4,4,4,mesh_gfx[5]));
	meshes.back().rotate_ccw();
	meshes.back().replace(1, mk_block(7,3));
	
	meshes.push_back(Mesh(9,5,4,4,mesh_gfx[6]));
	meshes.back().rotate_cw();
	meshes.back().replace(1, mk_block(6,2));
		
	meshes.push_back(Mesh(8,8,4,4,mesh_gfx[0]));
	meshes.back().rotate_cw();
	meshes.back().replace(1, mk_block(3,1));
	
	
	meshes.push_back(Mesh(14,5,4,4,mesh_gfx[0]));	
	meshes.back().replace(1, mk_block(2,0));
	
	meshes.push_back(Mesh(14,2,4,4,mesh_gfx[6]));	
	meshes.back().rotate_cw();
	meshes.back().rotate_cw();
	meshes.back().replace(1, mk_block(5,3));
	
	
	meshes.push_back(Mesh(17,5,4,4,mesh_gfx[0]));	
	meshes.back().replace(1, mk_block(3,1));
	
	meshes.push_back(Mesh(18,4,4,4,mesh_gfx[3]));	
	meshes.back().rotate_cw();
	meshes.back().replace(1, mk_block(4,2));
	
	
	meshes.push_back(Mesh(21,6,4,4,mesh_gfx[4]));	
	meshes.back().replace(1, mk_block(1,0));	
	
	meshes.push_back(Mesh(21,2,4,4,mesh_gfx[1]));	
	meshes.back().replace(1, mk_block(7,1));
	
	
	meshes.push_back(Mesh(25,3,4,4,mesh_gfx[5]));
	meshes.back().rotate_ccw();
	meshes.back().replace(1, mk_block(2,0));
	
	meshes.push_back(Mesh(25,5,4,4,mesh_gfx[0]));
	meshes.back().rotate_cw();
	meshes.back().replace(1, mk_block(3,1));
	
	meshes.push_back(Mesh(25,6,4,4,mesh_gfx[5]));
	meshes.back().rotate_cw();
	meshes.back().replace(1, mk_block(6,3));
}	

void TitleScene::draw_mesh(const Mesh& mesh)
{
	if(mesh.width()==0 || mesh.height()==0)
		return;
	unsigned short* map = ((unsigned short*)Video::bgGetMapPtr(3)) + 3*32 + 1;
	for(int y=mesh.y();y<mesh.y()+mesh.height();y++)
	{
		for(int x=mesh.x();x<mesh.x()+mesh.width();x++)
		{
			if(0<=x && x<30 && 0<=y && y<18)
			{
				int val = mesh.coord_at(x,y);
				
				if(val!=0)
				{
					map[32*y+x]=(val&0x7F)+first_piece_tile_id;
				}
			}
		}
	}
}

void TitleScene::switch_start_visibility(void*, void*)
{	
	if(Video::bgIsVisible(1))	
		Video::bgHide(1);
	else 
		Video::bgShow(1);	
}
