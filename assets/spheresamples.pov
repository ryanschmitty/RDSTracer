// Cornell Box
// Adapted from original
// Author: Ryan Schmitt

camera {
  location  <2, 8, 5>
  up        <0,  1,  0>
  right     <1, 0,  0>
  look_at   <0, 8, 0>
}

light_source {<5, 10, 0> color rgb <0.8, 0.8, 0.8>}
//light_source {<0, 9.5, 0> color rgb <0.5, 0.5, 0.5>}

plane { <0,1,0>, -3
   pigment { color rgb <0.5, 0, 0.5> }
   finish { ambient 0.1 diffuse 0.6 reflection 0.6 }
}

sphere { <0,0,0>, 1
   pigment { color rgb <1,1,1> }
   finish { emissive 1.0 }
}

sphere { <0,0,0>, 2
   pigment { color rgb <1,1,1> }
   finish { emissive 1.0 }
}

sphere { <0,0,0>, 3
   pigment { color rgb <1,1,1> }
   finish { emissive 0.0 }
}


