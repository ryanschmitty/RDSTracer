// Cornell Box
// Adapted from original
// Author: Ryan Schmitt

camera {
  location  <0, 5, 15.5>
  up        <0,  1,  0>
  right     <1, 0,  0>
  look_at   <0, 5, 0>
}

//TOP LIGHT
triangle { <-1,10,1>, <-1,10,-1>, <1,10,-1>
   pigment { color rgb <1, 0.85, 0.43>}
   finish { emissive 2 }
}
triangle { <1,10,1>, <-1,10,1>, <1,10,-1>
   pigment { color rgb <1, 0.85, 0.43>}
   finish { emissive 2 }
}

//Sphere on top of short box
sphere { <0,0,0>, 1
   scale 1.5
   translate <1.5, 4.4, 1.5>
   pigment { color rgbf <1, 1, 1, 0.8> }
   finish { ambient 0 diffuse 0.1 refraction 1.0 ior 1.3333 }
}

//Sphere on the ground
sphere { <0,0,0>, 1
   scale 1.5
   translate <-2.5, 1.5, 2.5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.005 diffuse 0.1 specular 1.0 roughness 0.005 reflection 0.8 }
}

//Short box
box { <-1,-1,-1>, <1,1,1>
   rotate <0, -20, 0>
   scale 1.5
   translate <1.5, 1.5, 1.5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.005 diffuse 0.1 }
}

//Tall box
box { <-1,-1,-1>, <1,1,1>
   rotate <0, 20, 0>
   scale <1.5, 3, 1.5>
   translate <-2, 3, -2>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.005 diffuse 0.1 }
}


// SPHERES ON BACK WALL
//row1
sphere { <-4, 9, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
sphere { <-2, 9, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
sphere { <0, 9, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
sphere { <2, 9, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
sphere { <4, 9, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
//row2
sphere { <-4, 7, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
sphere { <-2, 7, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
sphere { <0, 7, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
sphere { <2, 7, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
sphere { <4, 7, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
//row3
sphere { <-4, 5, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
sphere { <-2, 5, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
sphere { <0, 5, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
sphere { <2, 5, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
sphere { <4, 5, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
//row4
sphere { <-4, 3, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
sphere { <-2, 3, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
sphere { <0, 3, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
sphere { <2, 3, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
sphere { <4, 3, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
//row5
sphere { <-4, 1, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
sphere { <-2, 1, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
sphere { <0, 1, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
sphere { <2, 1, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}
sphere { <4, 1, -4.5>, 0.5
   pigment { color rgb <0.05,0,0.7> }
   finish { ambient 0.005 diffuse 0.1 specular 0.15 roughness 0.01 reflection 0.1 }
}


// THE ROOM

// floor @ y=0
triangle { <-5,0,5>, <5,0,-5>, <-5,0,-5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.005 diffuse 0.1 }
}
triangle { <-5,0,5>, <5,0,5>, <5,0,-5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.005 diffuse 0.1 }
}

// right wall
triangle { <5,0,5>, <5,10,5>, <5,0,-5>
   pigment { color rgb <0.15,0.48,.09> }
   finish { ambient 0.005 diffuse 0.1 }
}
triangle { <5,10,5>, <5,10,-5>, <5,0,-5>
   pigment { color rgb <0.15,0.48,.09> }
   finish { ambient 0.005 diffuse 0.1 }
}

// left wall
triangle { <-5,0,5>, <-5,0,-5>, <-5,10,-5>
   pigment { color rgb <0.63,0.06,.04> }
   finish { ambient 0.005 diffuse 0.1 }
}
triangle { <-5,10,5>, <-5,0,5>, <-5,10,-5>
   pigment { color rgb <0.63,0.06,.04> }
   finish { ambient 0.005 diffuse 0.1 }
}

// back wall
triangle { <5,10,-5>, <-5,10,-5>, <5,0,-5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.005 diffuse 0.1 }
}
triangle { <5,0,-5>, <-5,10,-5>, <-5,0,-5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.005 diffuse 0.1 }
}

// ceiling @ y=10, w/ a 1x1 hole in the middle
//big left part
triangle { <-5,10,5>, <-5,10,-5>, <-1,10,5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.1 }
}
triangle { <-1,10,5>, <-5,10,-5>, <-1,10,-5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.1 }
}
//big right part
triangle { <5,10,5>, <1,10,5>, <1,10,-5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.1 }
}
triangle { <5,10,5>, <1,10,-5>, <5,10,-5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.1 }
}
//little front part
triangle { <-1,10,5>, <1,10,1>, <1,10,5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.1 }
}
triangle { <-1,10,5>, <-1,10,1>, <1,10,1>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.1 }
}
//little back part
triangle { <-1,10,-1>, <1,10,-5>, <1,10,-1>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.1 }
}
triangle { <-1,10,-1>, <-1,10,-5>, <1,10,-5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.1 }
}



