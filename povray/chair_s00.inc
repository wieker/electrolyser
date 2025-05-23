// PoVRay 3.6 / 3.7 Include File "Chair_s00.inc"
// author: Friedrich A. Lohmueller, Dec-2005 / May-2014
// homepage: www.f-lohmueller.de

#ifndef( Chair_s00_Inc_Temp)
#declare Chair_s00_Inc_Temp = version;
#version 3.6;

#ifndef(  Shapes_Inc_Temp)
#include "shapes.inc"
#end

//--------------------------------------------------------------------
#macro Chair_s00 ( Chair__Height, // total height
                    Seat__Height,
                   Chair__Half_Width,
                   Chair__Feet_Radius,
                 ) //-------------------------------------------------

#local CR = Chair__Feet_Radius;
#local CW = Chair__Half_Width - CR;
#local CH = Chair__Height - CR;
#local SH =  Seat__Height - CR;

#local SW = Chair__Half_Width ;
#local ST = 2*CR ;

union{ // total union
union{
// seat
box { <-CW, 0.00, -CW>,< CW, 0, CW>  translate<0,SH,0> }
// front feet
cylinder { <0,0,0>,<0,SH,0>, CR translate< CW,0,-CW> }
cylinder { <0,0,0>,<0,SH,0>, CR translate<-CW,0,-CW> }
// frontside horizontal seat level
cylinder { <-CW,0,  0>,< CW,0, 0>, CR  translate<0,SH, CW> }
cylinder { <-CW,0,  0>,< CW,0, 0>, CR  translate<0,SH,-CW> }
cylinder { <  0,0,-CW>,<  0,0,CW>, CR  translate< CW,SH,0> }
cylinder { <  0,0,-CW>,<  0,0,CW>, CR  translate<-CW,SH,0> }
// frontside horizontal seat level
sphere{ < CW,SH,-CW>, CR  }
sphere{ <-CW,SH,-CW>, CR  }
// backside feet
cylinder { <0,0,0>,<0,CH,0>, CR translate< CW,0, CW> }
cylinder { <0,0,0>,<0,CH,0>, CR translate<-CW,0, CW> }

// backside horizontal upper
cylinder { <-CW,CH,0>,< CW,CH,0>, CR  translate <0,0,CW> }
sphere{ < CW,CH, CW>, CR }
sphere{ <-CW,CH, CW>, CR }
// backside horizontal seat level
sphere{ < CW,SH, CW>, CR }
sphere{ <-CW,SH, CW>, CR }

}// end of union


// back
object { // Round_Box(A, B, WireRadius, UseMerge)
         Round_Box(<-SW,SH+(CH-SH)/2,CW-2*CR>,<SW,CH+CR,CW>, CR/2 , 0)
         scale<1,1,1>  rotate<0, 0,0> translate<0,0,0>
       } // ---------------------------------------------------------
// seat
object { // Round_Box(A, B, WireRadius, UseMerge)
         Round_Box(<-SW,0,-SW>,<SW,2*CR,CW>, CR , 0)
         scale<1,1,1>  rotate<0, 0,0> translate<0,SH,0>
       } // ---------------------------------------------------------




}// end of total union
#end
//------------------------------------- end of #macro Chair_s00 (...)
// sample:
/*
//----------------------------------------------------------------------------
//----------------------------------------------------------------- textures
#declare Chair_Texture =
       texture{ pigment{ color rgb< 0.75, 0.5, 0.30>*0.5 }
                finish { ambient 0.1 diffuse 0.9 phong 1 }
              } // end of texture
//----------------------------------------------------------------------------
#include "Chair_s00.inc"
object{ Chair_s00 ( 0.80, // Chair_Height,
                    0.45, // Seat_Height,
                    0.225,// Chair_Half_Width
                    0.015,// Chair_Feet_Radius,
                  ) //------------------------------------------
        texture {Chair_Texture}
        scale <1,1,1> rotate<0,0,0> translate< 0.00,0.00,0.00>}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


*/

#version Chair_s00_Inc_Temp;
#end
//------------------------------------- end of include file
