// Persistence Of Vision Ray Tracer Scene Description File
// File: cornell.pov
// Vers: 3.5
// Desc: Radiosity demo scene. See also http://www.Graphics.Cornell.EDU/online/box/
// Date: August 2001
// Auth: Kari Kivisalo

// +w300 +h300

   //location  <27.8, 27.3,-80.0>
camera {
   location  <30, 33, -71>
   up        <0, 1, 0>
   right     <-1.3333, 0, 0>
   look_at   <30, 32, -57>
}

light_source { <30, 52, 28> color rgb <1.0, 1.0, 1.0> }


// ------------------------ OBJECTS ----------------------------

// Light Patch

box {
   <21.3,54.87,33.2>, <34.3,54.88,22.7> 
   pigment { color rgb <1,0,0> }
   finish { ambient 0.2 diffuse 0.8 }
}

// Floor
triangle {
   <55.28, 0.0, 0.0>, <0.0, 0.0, 0.0>, <0.0, 0.0, 55.92>
   pigment { color rgb <1,1,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <55.28, 0.0, 0.0>, <0.0, 0.0, 55.92>, <54.96, 0.0, 55.92>
   pigment { color rgb <1,1,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
// Ceiling
triangle {
   <55.60, 54.88, 0.0>, <55.60, 54.88, 55.92>, <0.0, 54.88, 55.92>
   pigment { color rgb <1,1,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <55.60, 54.88, 0.0>, <0.0, 54.88, 55.92>, <0.0, 54.88, 0.0>
   pigment { color rgb <1,1,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
// Back wall
triangle {
   <0.0, 54.88, 55.92>, <55.60, 54.88, 55.92>, <54.96, 0.0, 55.92>
   pigment { color rgb <1,1,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <0.0, 54.88, 55.92>, <54.96, 0.0, 55.92>, <0.0, 0.0, 55.92>
   pigment { color rgb <1,1,1> }
   finish { ambient 0.2 diffuse 0.8 }
}

// Right wall
triangle {
   <0.0, 54.88, 0.0>, <0.0, 54.88, 55.92>, <0.0, 0.0, 55.92>
   pigment { color rgb <1,0,0> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <0.0, 54.88, 0.0>, <0.0, 0.0, 55.92>, <0.0, 0.0, 0.0>
   pigment { color rgb <1,0,0> }
   finish { ambient 0.2 diffuse 0.8 }
}
//texture {Green}

// Left wall
triangle {
   <55.28, 0.0, 0.0>, <54.96, 0.0, 55.92>, <55.60, 54.88, 55.92>
   pigment { color rgb <0,1,0> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <55.28, 0.0, 0.0>, <55.60, 54.88, 55.92>, <55.60, 54.88, 0.0>
   pigment { color rgb <0,1,0> }
   finish { ambient 0.2 diffuse 0.8 }
}
//texture {Red}

// Short block
triangle {
   <13.00, 16.50, 6.50>, <8.20, 16.50, 22.50>, <24.00, 16.50, 27.20>
   pigment { color rgb <1,1,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <13.00, 16.50, 6.50>, <24.00, 16.50, 27.20>, <29.00, 16.50, 11.40>
   pigment { color rgb <1,1,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <29.00, 0.0, 11.40>, <29.00, 16.50, 11.40>, <24.00, 16.50, 27.20>
   pigment { color rgb <1,1,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <29.00, 0.0, 11.40>, <24.00, 16.50, 27.20>, <24.00, 0.0, 27.20>
   pigment { color rgb <1,1,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <13.00, 0.0, 6.50>, <13.00, 16.50, 6.50>, <29.00, 16.50, 11.40>
   pigment { color rgb <1,1,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <13.00, 0.0, 6.50>, <29.00, 16.50, 11.40>, <29.00, 0.0, 11.40>
   pigment { color rgb <1,1,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <8.20, 0.0, 22.50>, <8.20, 16.50, 22.50>, <13.00, 16.50, 6.50>
   pigment { color rgb <1,1,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <8.20, 0.0, 22.50>, <13.00, 16.50, 6.50>, <13.00, 0.0, 6.50>
   pigment { color rgb <1,1,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <24.00, 0.0, 27.20>, <24.00, 16.50, 27.20>, <8.20, 16.50, 22.50>
   pigment { color rgb <1,1,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <24.00, 0.0, 27.20>, <8.20, 16.50, 22.50>, <8.20, 0.0, 22.50>
   pigment { color rgb <1,1,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
//texture { White }

// Tall block
triangle {
   <42.30, 33.00, 24.70>, <26.50, 33.00, 29.60>, <31.40, 33.00, 45.60>
   pigment { color rgb <0,1,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <42.30, 33.00, 24.70>, <31.40, 33.00, 45.60>, <47.20, 33.00, 40.60>
   pigment { color rgb <0,0,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <42.30, 0.0, 24.70>, <42.30, 33.00, 24.70>, <47.20, 33.00, 40.60>
   pigment { color rgb <0,0,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <42.30, 0.0, 24.70>, <47.20, 33.00, 40.60>, <47.20, 0.0, 40.60>
   pigment { color rgb <0,0,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <47.20, 0.0, 40.60>, <47.20, 33.00, 40.60>, <31.40, 33.00, 45.60>
   pigment { color rgb <0,0,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <47.20, 0.0, 40.60>, <31.40, 33.00, 45.60>, <31.40, 0.0, 45.60>
   pigment { color rgb <0,0,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <31.40, 0.0, 45.60>, <31.40, 33.00, 45.60>, <26.50, 33.00, 29.60>
   pigment { color rgb <0,0,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <31.40, 0.0, 45.60>, <26.50, 33.00, 29.60>, <26.50, 0.0, 29.60>
   pigment { color rgb <0,0,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <26.50, 0.0, 29.60>, <26.50, 33.00, 29.60>, <42.30, 33.00, 24.70>
   pigment { color rgb <0,0,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
triangle {
   <26.50, 0.0, 29.60>, <42.30, 33.00, 24.70>, <42.30, 0.0, 24.70>
   pigment { color rgb <0,0,1> }
   finish { ambient 0.2 diffuse 0.8 }
}
//texture {White}

