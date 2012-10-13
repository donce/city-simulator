#ifndef INC_CONST_H
#define INC_CONST_H


//const

//lights
//in miliseconds
#define c_yellow (2*1000)


//in seconds
#define c_min_time 5
#define c_max_time 15


const int c_models_frames_per_second = 5;//modeliuose


//-------------------------------------------------------------------
//filenames
//object models
#define c_file_building_models "data/info/object_models/building_models.txt"
#define c_file_vehicle_models "data/info/object_models/vehicle_models.txt"
//map
#define c_file_map "data/info/map/map.txt"
#define c_file_buildings "data/info/map/buildings.txt"
#define c_file_lights "data/info/map/lights.txt"
#define c_file_parameters "data/info/map/parameters.txt"
#define c_file_routes "data/info/map/routes.txt"
//settings
#define c_file_simulation "data/info/settings/simulation.txt"	
#define c_file_window "data/info/settings/window.txt"	

//models
#define c_dir_model_main_path "data/models/"
#define c_file_model_model_end "/model.md2"
#define c_file_model_tex_end "/tex.bmp"

//tex
#define c_dir_tex_main_path "data/tex/"
//-------------------------------------------------------------------

//user controls
#define c_jud_pagr 0.005
#define c_jud_kam 0.1
#define c_mouse_speed 0.5
#define c_mouse_wheel_speed 0.02


//#define c_crossroad_size 1.5



//GUI
#define c_border_height 0.03


#define C_MAX_LENTH_SELECTING_VERTEX 3

#endif
