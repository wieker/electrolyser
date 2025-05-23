// POV-Ray 3.7 Scene File "light_area_tube.pov"
// author: Friedrich A. Lohmueller, March-2010 / Jan-2011
// homepage: www.f-lohmueller.de
//--------------------------------------------------------------------------
#version 3.7;
global_settings{assumed_gamma 1.0}
#default{ finish{ ambient 0.1 diffuse 0.9 conserve_energy}}
//--------------------------------------------------------------------------
#include "colors.inc"
#include "textures.inc"
#include "glass.inc"
#include "metals.inc"
#include "golds.inc"
#include "stones.inc"
#include "woods.inc"
#include "shapes.inc"
#include "shapes2.inc"
#include "functions.inc"
#include "math.inc"
#include "transforms.inc"
//-------------------------------------------------------------------------------------------------------<<<<
//------------------------------------------------------------- Camera_Position, Camera_look_at, Camera_Angle
#declare Camera_Number = 0 ;
//--------------------------------------------------------------------------------------------------------<<<<
#switch ( Camera_Number )
#case (0)
  #declare Camera_Position = < 0.00, 1.60, -3.00> ;  // front view from up
  #declare Camera_Look_At  = < 0.00, 0.65,  0.00> ;
  #declare Camera_Angle    =  45 ;
#break
#case (1)
  #declare Camera_Position = < 2.00, 2.20, -2.00> ;  // diagonal view
  #declare Camera_Look_At  = < 0.00, 1.00,  0.00> ;
  #declare Camera_Angle    =  70 ;
#break
#case (2)
  #declare Camera_Position = < 0.00, 5.00,  0.01> ;  // top view
  #declare Camera_Look_At  = < 0.00, 1.00,  0.00> ;
  #declare Camera_Angle    =  65 ;
#break
#else
  #declare Camera_Position = < 0.00, 1.00,-20.00> ;  // front view
  #declare Camera_Look_At  = < 0.00, 1.00,  0.00> ;
  #declare Camera_Angle    =  65 ;
#break
#end // of "#switch ( Camera_Number )" -----------------------------
//-------------------------------------------------------------------------------------------------------<<<<
camera{ location Camera_Position
        right    x*image_width/image_height
        angle    Camera_Angle
        look_at  Camera_Look_At
      }
//------------------------------------------------------------------------------------------------------<<<<<
//------------------------------------------------------------------------
//------------------------------------------------------------------------
// sun -------------------------------------------------------------------
//light_source{<1500,2500,-2500> color White*0.9}           // sun light
//light_source{ Camera_Position  color rgb<0.9,0.9,1>*0.1}  // flash light

// sky -------------------------------------------------------------------
sky_sphere{ pigment{ gradient <0,1,0>
                     color_map{ [0   color rgb<1,1,1>         ]//White
                                [0.4 color rgb<0.14,0.14,0.56>]//~Navy
                                [0.6 color rgb<0.14,0.14,0.56>]//~Navy
                                [1.0 color rgb<1,1,1>         ]//White
                              }
                     scale 2 }
           } // end of sky_sphere
//------------------------------------------------------------------------

//------------------------------ the Axes --------------------------------
//------------------------------------------------------------------------
#macro Axis_( AxisLen, Dark_Texture,Light_Texture)
 union{
    cylinder { <0,-AxisLen,0>,<0,AxisLen,0>,0.05
               texture{checker texture{Dark_Texture }
                               texture{Light_Texture}
                       translate<0.1,0,0.1>}
             }
    cone{<0,AxisLen,0>,0.2,<0,AxisLen+0.7,0>,0
          texture{Dark_Texture}
         }
     } // end of union
#end // of macro "Axis()"
//------------------------------------------------------------------------
#macro AxisXYZ( AxisLenX, AxisLenY, AxisLenZ, Tex_Dark, Tex_Light)
//--------------------- drawing of 3 Axes --------------------------------
#declare Text_Rotate = <10,-45,0>;
union{
#if (AxisLenX != 0)
 object { Axis_(AxisLenX, Tex_Dark, Tex_Light)   rotate< 0,0,-90>}// x-Axis
 text   { ttf "arial.ttf",  "x",  0.15,  0  texture{Tex_Dark}
          rotate Text_Rotate scale 0.5 translate <AxisLenX+0.15,0.2,-0.05> no_shadow }
#end // of #if
#if (AxisLenY != 0)
 object { Axis_(AxisLenY, Tex_Dark, Tex_Light)   rotate< 0,0,  0>}// y-Axis
 text   { ttf "arial.ttf",  "y",  0.15,  0  texture{Tex_Dark}
          rotate Text_Rotate scale 0.5 translate <-0.35,AxisLenY+0.20,-0.05> no_shadow }
#end // of #if
#if (AxisLenZ != 0)
 object { Axis_(AxisLenZ, Tex_Dark, Tex_Light)   rotate<90,0,  0>}// z-Axis
 text   { ttf "arial.ttf",  "z",  0.15,  0  texture{Tex_Dark}
          rotate Text_Rotate scale 0.65 translate <-0.75,0.2,AxisLenZ+0.10> no_shadow }
#end // of #if
} // end of union
#end// of macro "AxisXYZ( ... )"
//------------------------------------------------------------------------

#declare Texture_A_Dark  = texture {
                               pigment{ color rgb<1,0.45,0>}
                               finish { phong 1}
                             }
#declare Texture_A_Light = texture {
                               pigment{ color rgb<1,1,1>}
                               finish { phong 1}
                             }

//object{ AxisXYZ( 2.5, 3.0, 2.5, Texture_A_Dark, Texture_A_Light) scale 0.2}
//-------------------------------------------------- end of coordinate axes


// ground -----------------------------------------------------------------
//---------------------------------<<< settings of squared plane dimensions
#declare RasterScale = 0.33;
#declare RasterHalfLine  = 0.04;
#declare RasterHalfLineZ = 0.04;
//-------------------------------------------------------------------------
#macro Raster(RScale, HLine)
       pigment{ gradient x scale RScale
                color_map{[0.000   color rgbt<1,1,1,0>*0.6]
                          [0+HLine color rgbt<1,1,1,0>*0.6]
                          [0+HLine color rgbt<1,1,1,1>]
                          [1-HLine color rgbt<1,1,1,1>]
                          [1-HLine color rgbt<1,1,1,0>*0.6]
                          [1.000   color rgbt<1,1,1,0>*0.6]} }
 #end// of Raster(RScale, HLine)-macro
//-------------------------------------------------------------------------


plane { <0,1,0>, 0    // plane with layered textures
        texture { pigment{color White*1.1}
                  finish {ambient 0.1 diffuse 0.9}}
        texture { Raster(RasterScale,RasterHalfLine ) rotate<0,0,0> }
        texture { Raster(RasterScale,RasterHalfLineZ) rotate<0,90,0>}
        rotate<0,0,0>
      }
//------------------------------------------------ end of squared plane XZ

//--------------------------------------------------------------------------
//---------------------------- objects in scene ----------------------------
//--------------------------------------------------------------------------
#declare Tube_Length = 1.00;
#declare Tube_End = 0.02;
#declare Tube =

union{
 cylinder{ < 0,0,0>,<Tube_End,0,0>, 0.025
           texture { Chrome_Metal }
           translate <0,0,0>
         } // end of cylinder  ------------------------------------
 cylinder{ <-Tube_End,0,0>,<0.00,0,0>, 0.025
           texture { Chrome_Metal }
           translate <Tube_Length,0,0>
         } // end of cylinder  ------------------------------------
 cylinder{ <Tube_End,0,0>,<Tube_Length-Tube_End,0,0>, 0.03
           texture { pigment { color rgb<1,1,1>}
                     finish  { ambient 0.8 diffuse 0.2 phong 1 reflection 0.25 }
                   } // end of texture
         } // end of cylinder  ------------------------------------
}// end of union --------------------------------------------------


// soft light (areal_light) ---------  An area light (creates soft shadows)
// WARNING: This special light can significantly slow down rendering times!
light_source { <0,0,0>
               color rgb <1,1,1>     // light's color
               area_light            // kind of light source (Art der Lichtquelle)
               <0.03, 0, 0> <1.47, 0, 0> // lights spread out across this distance (x * z)
               1, 10                // total number of lights in grid (4x*4z = 16 lights)
               adaptive 2          // 0,1,2,3...
               jitter              // adds random softening of light
           // --------------------------
           looks_like {
                        object{ Tube }
                      }
           // --------------------------
               translate <0, 1.50, 0>   // <x,y,z> position of light
             } //---------------- end of area_light

//----------------------------------------------------------------------------
//----------------------------------------------------------------- textures
#declare Chair_Texture =
       texture{ pigment{ color rgb< 0.75, 0.6, 0.45>*0.55 }
                finish { ambient 0.1 diffuse 0.9 phong 1 }
              } // end of texture
//----------------------------------------------------------------------------
#include "chair_s00.inc"
//----------------------------------------------------------------------------
object{ Chair_s00 ( 0.80, // Total_Chair_Height,
                    0.45, // Seat_Height,
                    0.225,// Chair_Half_Width
                    0.015,// Chair_Feet_Radius,
                  ) //----------------------------
        texture{ Chair_Texture }
        scale <1,1,1> rotate<0,45,0> translate< 0.5,0.00,0.00>
      } //--------------------------------------------------------------------
object{ Chair_s00 ( 0.80, // Total_Chair_Height,
                    0.45, // Seat_Height,
                    0.225,// Chair_Half_Width
                    0.015,// Chair_Feet_Radius,
                  ) //----------------------------
        texture{ Chair_Texture }
        scale <1,1,1> rotate<0,-30,0> translate<-0.5,0.00,0.00>
      } //--------------------------------------------------------------------
//----------------------------------------------------------------------------