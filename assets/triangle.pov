//One triangle

camera {
  location  <0, 0, 7>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}


light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}

triangle {
   <0, 0, 0>,
   <1, 1, 0>,
   <-1, 1, 0>

   pigment {color rgb <0.9, 0.9, 0.0>}
   finish {ambient 0.4 diffuse 0.6}
}

sphere {
   <0, 0, 0>, 0.1
   pigment {color rgb <1.0, 0.0, 0.0>}
   finish {ambient 0.2 diffuse 0.4}
}

sphere {
   <-1, 1, 0>, 0.1
   pigment {color rgb <0.0, 1.0, 0.0>}
   finish {ambient 0.2 diffuse 0.4}
}


sphere {
   <1, 1, 0>, 0.1
   pigment {color rgb <0.0, 0.0, 1.0>}
   finish {ambient 0.2 diffuse 0.4}
}



