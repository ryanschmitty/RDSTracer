// cs174, assignment 1 sample file (RIGHT HANDED)

camera {
  location  <0, 5, 20>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}


light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}

sphere { <0, 0, 0>, 2
  pigment { color rgb <1.0, 1.0, 1.0>}
  finish {ambient 0 diffuse 0 specular 1.0 roughness 0.0025 reflection 0.9}
  translate <0, 0, 0>
}

sphere { <0, 0, 0>, 2
  pigment { color rgb <1.0, 1.0, 1.0>}
  finish {ambient 0 diffuse 0 specular 1.0 roughness 0.0025 reflection 0.9}
  translate <8, 0, -8>
}

sphere { <0, 0, 0>, 2
  pigment { color rgb <1.0, 1.0, 1.0>}
  finish {ambient 0 diffuse 0 specular 1.0 roughness 0.0025 reflection 0.9}
  translate <-10, 0, -5>
}

sphere { <0, 0, 0>, 2
  pigment { color rgb <1.0, 1.0, 1.0>}
  finish {ambient 0 diffuse 0 specular 1.0 roughness 0.0025 reflection 0.9}
  translate <-20, 0, -100>
}


plane {<0, 1, 0>, -4
      pigment {color rgb <0.2, 0.2, 0.8>}
      finish {ambient 0.4 diffuse 0.8 reflection 0.5}
} 