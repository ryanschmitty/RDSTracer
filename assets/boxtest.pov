// cs473, assignment 1 recursion test (RIGHT HANDED)
camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.5, 0,  0>
  look_at   <0, 0, 0>
}


light_source {<-5, 3, 0> color rgb <0.3, 0.5, 0.3>}
light_source {<5, 10, 5> color rgb <0.3, 0.4, 0.5>}

box { <-2, -5, -5>, <2, 5, 5>
  pigment { color rgb <1.0, 0.2, 1.0>}
  finish {ambient 0.2 diffuse 0.8}
  rotate <0, -45, 0>
}

