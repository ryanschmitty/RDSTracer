
camera {
  location  <0, 0, 10>
  up        <0,  1,  0>
  right     <1.3333, 0,  0>
  look_at   <0, 0, -200>
}

light_source {<0, 0, -12> color rgb <1.5, 1.5, 1.5>}

sphere { <0,0,0>, 15
   pigment {color rgb <0.2, 0.8, 0.3>}
   finish {ambient 0.2 diffuse 0.6}
}

//box { <-1.5, -1.5, -1.5>, <1.5, 1.5, 1.5>
//   pigment { color rgb <0.2, 0.8, 0.3> }
//   finish { ambient 0.2 diffuse 0.6 }
//   scale <10, 10, 10>
//}

plane {<0, 1, 0>, -4
      pigment {color rgb <0.2, 0.2, 0.8>}
      finish {ambient 0.4 diffuse 0.8}
}

