// 3 Spheres
// Author: Ryan Schmitt

camera {
  location  <0, 5, 15.5>
  up        <0,  1,  0>
  right     <1, 0,  0>
  look_at   <0, 5, 0>
}

// THE SPHERES
sphere { <0,0,0>, 2
    pigment { color rgb <0.15, 0.48, 0.09> }
    finish { ambient 0.15 diffuse 0.5 }
    translate <-5, 2, 0>
}
sphere { <0,0,0>, 2
    pigment { color rgb <1.0, 0.85, 0.43> }
    finish { emissive 2 }
    translate <0, 5, 0>
}
sphere { <0,0,0>, 2
    pigment { color rgb <0.63, 0.06, 0.04> }
    finish { ambient 0.15 diffuse 0.5 }
    translate <5, 2, 0>
}

// THE ROOM

// floor @ y=0
triangle { <-20,0,20>, <20,0,-20>, <-20,0,-20>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.15 diffuse 0.5 }
}
triangle { <-20,0,20>, <20,0,20>, <20,0,-20>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.15 diffuse 0.5 }
}
