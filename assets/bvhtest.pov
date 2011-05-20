// cs174, assignment 1 sample file (RIGHT HANDED)

camera {
  location  <0, 5, 14>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}


light_source {<-100, 100, 100> color rgb <1, 1, 1>}

sphere { <0, 0, 0>, 1
  pigment { color rgb <1.0, 0.0, 0.0>}
  finish {ambient 0.2 diffuse 0.4}
  translate <-4, 0, 0>
}

sphere { <0, 0, 0>, 1
  pigment { color rgb <0.0, 1.0, 0.0>}
  finish {ambient 0.2 diffuse 0.4}
  translate <0, 0, -4>
}

sphere { <0, 0, 0>, 1
  pigment { color rgb <0.0, 0.0, 1.0>}
  finish {ambient 0.2 diffuse 0.4}
  translate <4, 0, 0>
}

sphere { <0, 0, 0>, 1
  pigment { color rgb <1.0, 1.0, 1.0>}
  finish {ambient 0.2 diffuse 0.4}
  translate <0, 0, 4>
}

