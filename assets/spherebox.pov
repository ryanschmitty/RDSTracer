
camera {
  location  <-14, 0, 0>
  up        <0,  1,  0>
  right     <0, 0,  1.3333>
  look_at   <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}

sphere { <0, 0, 0>, 2
  pigment { color rgb <1.0, 0.0, 1.0>}
  finish {ambient 0.2 diffuse 0.4}
  scale <1, 1, 2>
}

box { <-1.5, -1.5, -1.5>, <1.5, 1.5, 1.5>
   pigment { color rgb <0.2, 0.8, 0.3> }
   finish { ambient 0.6 diffuse 0.4 }
}

plane {<0, 1, 0>, -4
      pigment {color rgb <0.2, 0.2, 0.8>}
      finish {ambient 0.4 diffuse 0.8}
}

