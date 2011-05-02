// cs174, assignment 1 sample file (RIGHT HANDED)

// default camera pointing down negative z - case 0
camera {
location  <0, 0, 18>
up        <0,  1,  0>
right     <1.33333, 0,  0>
look_at   <0, 0, 0>
}

light_source {<0, 5, 5> color rgb <0.4, 0.4, 0.4>}
light_source {<-3, 5, 1> color rgb <0.6, 0.6, 0.8>}

plane {<0, 1, 0>, -4.2
      pigment {color rgb <0.4, 0.3, 0.3>}
      finish {diffuse 1}
}

plane {<0, 1, 0>, 8
  pigment {color rgb <0.4, 0.3, 0.3>}
  finish {diffuse 1}
}

plane {<0.4, 0, 1>, -12
  pigment {color rgb <0.5, 0.3, 0.5>}
  finish {diffuse 0.6}
}

//Big Sphere to the right
sphere { <2, 0.8, 5>, 2.5
  pigment {color rgb <1.0, 1.0, 1.0>}
  finish {refraction 0.8 ior 1.3}
}

//Sphere to the left on the wall
sphere { <-5.5, -1, -6>, 2
  pigment {color rgb <0.7, 0.7, 1>}
  finish {reflection 0.8 diffuse 0.1}
}

//Little sphere
sphere { <-1.5, -4, 10>, 1.5
  pigment {color rgb <1.0, 1.0, 1.0>}
  finish {specular 0.8 roughness 0.0005 refraction 0.5 ior 1.3 diffuse 0.3}
}

//Sphere grid

// col 1
sphere { <-4.5, -4, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-4.5, -2.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-4.5, -0.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-4.5, 1, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-4.5, 2.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-4.5, 4, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-4.5, 5.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-4.5, 7, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

// col 2
sphere { <-3, -4, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-3, -2.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-3, -0.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-3, 1, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-3, 2.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-3, 4, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-3, 5.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-3, 7, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

// col 3
sphere { <-1.5, -4, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-1.5, -2.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-1.5, -0.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-1.5, 1, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-1.5, 2.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-1.5, 4, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-1.5, 5.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <-1.5, 7, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

// col 4
sphere { <0, -4, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <0, -2.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <0, -0.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <0, 1, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <0, 2.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <0, 4, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <0, 5.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <0, 7, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

// col 5
sphere { <1.5, -4, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <1.5, -2.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <1.5, -0.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <1.5, 1, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <1.5, 2.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <1.5, 4, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <1.5, 5.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <1.5, 7, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

// col 6
sphere { <3, -4, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <3, -2.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <3, -0.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <3, 1, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <3, 2.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <3, 4, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <3, 5.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <3, 7, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

// col 7
sphere { <4.5, -4, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <4.5, -2.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <4.5, -0.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <4.5, 1, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <4.5, 2.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <4.5, 4, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <4.5, 5.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <4.5, 7, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

// col 8
sphere { <6, -4, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <6, -2.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <6, -0.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <6, 1, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <6, 2.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <6, 4, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <6, 5.5, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}

sphere { <6, 7, -10>, 0.3
  pigment {color rgb <0.3, 1, 0.4>}
  finish {specular 0.6 diffuse 0.6}
}


