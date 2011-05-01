camera {
  location  <0, 14, 14>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}

sphere { <0, 0, 0>, 2
  pigment { color rgb <1.0, 1.0, 1.0>}
  finish {ambient 0 diffuse 0 refraction 1 ior 1.33}
  translate <0, 5, 0>
}

// floor
plane {<0, 1, 0>, -4
  pigment {color rgb <0.07, 0.25, 0.67>}
  finish {ambient 0.2 diffuse 0.4}
  translate <0, -1, 0>
}

// tile the floor
// leftmost column
triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-22, -4.9999, -20>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

// one column to the right
triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-18, -4.9999, -12>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-18, -4.9999, -16>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-18, -4.9999, -20>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

// one column to the right
triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-14, -4.9999, -4>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-14, -4.9999, -8>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-14, -4.9999, -12>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}
triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-14, -4.9999, -16>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-14, -4.9999, -20>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-14, -4.9999, -24>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

// one column to the right
triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-10, -4.9999, 4>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-10, -4.9999, 0>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-10, -4.9999, -4>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-10, -4.9999, -8>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-10, -4.9999, -8>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-10, -4.9999, -12>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-10, -4.9999, -16>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-10, -4.9999, -20>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-10, -4.9999, -24>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-10, -4.9999, -28>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-10, -4.9999, -32>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-10, -4.9999, -36>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-10, -4.9999, -40>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}


// one column to the right
triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-6, -4.9999, 4>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-6, -4.9999, 0>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-6, -4.9999, -4>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-6, -4.9999, -8>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-6, -4.9999, -12>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-6, -4.9999, -16>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-6, -4.9999, -20>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-6, -4.9999, -24>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-6, -4.9999, -28>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-6, -4.9999, -32>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-6, -4.9999, -36>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-6, -4.9999, -40>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-6, -4.9999, -44>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-6, -4.9999, -48>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}


// one more column to the right
triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-2, -4.9999, 4>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-2, -4.9999, 0>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-2, -4.9999, -4>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-2, -4.9999, -8>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-2, -4.9999, -12>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-2, -4.9999, -16>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-2, -4.9999, -20>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-2, -4.9999, -24>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-2, -4.9999, -28>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-2, -4.9999, -32>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <-2, -4.9999, -36>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}


// one more column to the right

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <2, -4.9999, 4>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}


triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <2, -4.9999, 0>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}


triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <2, -4.9999, -4>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}


triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <2, -4.9999, -8>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}


triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <2, -4.9999, -12>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}


triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <2, -4.9999, -16>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}


triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <2, -4.9999, -20>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}


triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <2, -4.9999, -24>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}


triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <2, -4.9999, -28>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}


// one more column to the right
triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <6, -4.9999, 4>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <6, -4.9999, 0>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <6, -4.9999, -4>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <6, -4.9999, -8>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <6, -4.9999, -12>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <6, -4.9999, -16>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <6, -4.9999, -20>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <6, -4.9999, -24>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <6, -4.9999, -28>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

// one more column to the right
triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <10, -4.9999, 4>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <10, -4.9999, 0>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <10, -4.9999, -4>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <10, -4.9999, -8>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <10, -4.9999, -12>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <10, -4.9999, -16>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <10, -4.9999, -20>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <10, -4.9999, -24>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <10, -4.9999, -28>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

// one more column to the right
triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <14, -4.9999, -4>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <14, -4.9999, -8>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <14, -4.9999, -12>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <14, -4.9999, -16>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <14, -4.9999, -20>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <14, -4.9999, -24>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

// one more column to the right
triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <18, -4.9999, -8>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <18, -4.9999, -12>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <18, -4.9999, -16>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <18, -4.9999, -20>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <18, -4.9999, -24>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <18, -4.9999, -28>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

// one more column to the right
triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <22, -4.9999, -12>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <22, -4.9999, -16>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <22, -4.9999, -20>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}

triangle {
  <0, 0, 0>,
  <1, 0, 0>,
  <0, 0, -1>
  scale <4, 4, 4>
  translate <22, -4.9999, -24>
  pigment {color rgb <1.0, 0.67, 0.0>}
  finish {ambient 0.4 diffuse 0.5}
}
