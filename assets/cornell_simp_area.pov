// Cornell Box
// Adapted from original
// Author: Ryan Schmitt

camera {
  location  <0, 5, 15.5>
  up        <0,  1,  0>
  right     <1, 0,  0>
  look_at   <0, 5, 0>
}

//light_source {<0, 5, 15> color rgb <0.1, 0.1, 0.1>}
//light_source {<0, 9.5, 0> color rgb <1.5, 1.5, 1.3>}
//TOP LIGHT
triangle { <-1,10,1>, <-1,10,-1>, <1,10,-1>
   pigment { color rgb <1, 0.95, 0.80>}
   finish { emissive 2 }
}
triangle { <1,10,1>, <-1,10,1>, <1,10,-1>
   pigment { color rgb <1, 0.95, 0.80>}
   finish { emissive 2 }
}


//Short box
box { <-1,-1,-1>, <1,1,1>
   rotate <0, -20, 0>
   scale 1.5
   translate <1.5, 1.5, 1.5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.2 }
}

//Tall box
box { <-1,-1,-1>, <1,1,1>
   rotate <0, 20, 0>
   scale <1.5, 3, 1.5>
   translate <-2, 3, -2>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.2 }
}

// THE ROOM

// floor @ y=0
triangle { <-5,0,5>, <5,0,-5>, <-5,0,-5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.2 }
}
triangle { <-5,0,5>, <5,0,5>, <5,0,-5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.2 }
}

// right wall
triangle { <5,0,5>, <5,10,5>, <5,0,-5>
   pigment { color rgb <0.15,0.48,.09>}
   finish { ambient 0.05 diffuse 0.2 }
}
triangle { <5,10,5>, <5,10,-5>, <5,0,-5>
   pigment { color rgb <0.15,0.48,.09>}
   finish { ambient 0.05 diffuse 0.2 }
}

// left wall
triangle { <-5,0,5>, <-5,0,-5>, <-5,10,-5>
   pigment { color rgb <0.63,0.06,.04>}
   finish { ambient 0.05 diffuse 0.2 }
}
triangle { <-5,10,5>, <-5,0,5>, <-5,10,-5>
   pigment { color rgb <0.63,0.06,.04>}
   finish { ambient 0.05 diffuse 0.2 }
}

// back wall
triangle { <5,10,-5>, <-5,10,-5>, <5,0,-5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.2 }
}
triangle { <5,0,-5>, <-5,10,-5>, <-5,0,-5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.2 }
}

// ceiling @ y=10, w/ a 1x1 hole in the middle
//big left part
triangle { <-5,10,5>, <-5,10,-5>, <-1,10,5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.2 }
}
triangle { <-1,10,5>, <-5,10,-5>, <-1,10,-5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.2 }
}
//big right part
triangle { <5,10,5>, <1,10,5>, <1,10,-5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.2 }
}
triangle { <5,10,5>, <1,10,-5>, <5,10,-5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.2 }
}
//little front part
triangle { <-1,10,5>, <1,10,1>, <1,10,5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.2 }
}
triangle { <-1,10,5>, <-1,10,1>, <1,10,1>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.2 }
}
//little back part
triangle { <-1,10,-1>, <1,10,-5>, <1,10,-1>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.2 }
}
triangle { <-1,10,-1>, <-1,10,-5>, <1,10,-5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.05 diffuse 0.2 }
}

