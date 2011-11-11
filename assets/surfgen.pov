//Testing surfel generation

//camera pointing around 45 degrees
//camera {
//  location  <-10, 10, 10> 
//  up        <0,  1,  0>
//  right     <.94, 0,  -.94>
//  look_at   <0, 0, 0>
//}

//standard camera down z axis
//camera {
//  location  <0, 0, 10>
//  up        <0, 1, 0>
//  right     <1.33333, 0,  0>
//  look_at   <0, 0, 0>
//}

// Cornell Box cam
camera {
  location  <0, 5, 15.5>
  up        <0,  1,  0>
  right     <1, 0,  0>
  look_at   <0, 5, 0>
}

light_source {<-100, 100, 100> color rgb <1, 1, 1>}

//box { <-1,-1,-1>, <1,1,1>
//    pigment { color rgb <1.0, 0.0, 0.0>}
//    finish {ambient 0.2 diffuse 0.8}
//    translate <0, 0, 0>
//    scale <1, 1, 1>
//}

box { <-1,-1,-1>, <1,1,1>
   rotate <0, 80, 0>
//   rotate <0, 45, 0>
   scale 1.5
   translate <1.5, 1.5, 1.5>
   pigment { color rgb <0.76,0.75,0.5> }
   finish { ambient 0.6 diffuse 0.8 }
}

//sphere { <0, 0, 0>, 1
//  pigment { color rgb <1.0, 0.0, 1.0>}
//  finish {ambient 0.5 diffuse 0.9}
//  translate <0, 0, 0>
//  scale <1, 4, 1>
//}

//sphere { <0, 0, 0>, 1
//  pigment { color rgb <1.0, 0.1, 1.0>}
//  finish {ambient 0.2 diffuse 0.4 specular 0.5 roughness 0.05}
//  scale <1, 2, 1>
//  rotate <0, 0, 25>
//  translate <-1.2, 0 ,0>
//}

//triangle { <-2,-2,0>, <0,2,0>, <2,-2,0>
//  pigment { color rgb <1.0, 0.0, 1.0>}
//  finish {ambient 0.2 diffuse 0.4}
//  translate <0, 0, 0>
//  scale <1, 2, 1>
//}
