
camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.3333, 0,  0>
  look_at   <0, 0, -200>
}

light_source {<0, 0, -12> color rgb <1.5, 1.5, 1.5>}

box { <-1.5, -1.5, -1.5>, <1.5, 1.5, 1.5>
   pigment { color rgb <0.2, 0.8, 0.3> }
   finish { ambient 0.2 diffuse 0.6 }
   scale <10, 5, 10>
}

