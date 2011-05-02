camera {
    location <0, 0, 14>
    up <0, 1, 0>
    right <1.77778, 0, 0>
    look_at <0, 0, 0>
}


light_source { <0, 100, 100> color rgb <0.5, 0.5, 0.5> }
light_source { <-100, 100, 100> color rgb <0.33333, 0.33333, 0.33333> }
light_source { <100, 50, -100> color rgb <0.16667, 0.16667, 0.16667> }

// red
sphere { <0, 0, 0>, 3
    translate <-5, -1, 0>
    pigment { color rgb <1.0, 0.0, 0.0> }
    finish { ambient 0.2 diffuse 0.4 specular 0.8 roughness 0.05 reflection 0.3 }
}

// green
sphere { <0, 0, 0>, 3
    translate <1, -1, -3>
    pigment { color rgb <0.0, 1.0, 0.0> }
    finish { ambient 0.2 diffuse 0.4 specular 0.8 roughness 0.05 reflection 0.3 }
}

// blue
sphere { <0, 0, 0>, 3
    translate <8.5, -1, -6>
    pigment { color rgb <0.0, 0.0, 1.0> }
    finish { ambient 0.2 diffuse 0.4 specular 0.8 roughness 0.05 reflection 0.3 }
}

// glass
sphere { <0, 0, 0>, 1.5
    translate <0, -2.5, 3>
    pigment { color rgbf <1.0, 1.0, 1.0, 0.9> }
    finish { ambient 0.0 diffuse 0.0 specular 0.0 roughness 0.05 reflection 0.1 refraction 1.0 ior 1.51 }
}

// mirror
sphere { <0, 0, 0>, 1.5
    translate <4.5, -2.5, 0>
    pigment { color rgb <0.0, 0.0, 0.0> }
    finish { ambient 0.0 diffuse 0.0 specular 0.8 roughness 0.05 reflection 1.0 }
}

// mirror
box { <-1, -1, -1>, <1, 1, 1>
    rotate <0, 0, 45>
    scale <1, 3, 0.15>
    scale 0.5
    rotate <0, 90, 0>
    rotate <0, 0, -60>
    translate <2.36, -2.88, 4.5>
    pigment { color rgb <1.0, 1.0, 1.0> }
    finish { ambient 0.0 diffuse 0.0 specular 0.0 roughness 0.05 reflection 1.0 }
}

// white
box { <-1, -1, -1>, <1, 1, 1>
    rotate <0, 0, 15>
    scale 0.75
    translate <4, -3.075, 4.5>
    pigment { color rgb <1.0, 1.0, 1.0> }
    finish { ambient 0.2 diffuse 0.4 specular 0.8 roughness 0.05 reflection 0.3 }
}

// floor
plane { <0, 1, 0>, -4
    pigment { color rgb <0.2, 0.2, 0.2> }
    finish { ambient 0.4 diffuse 0.8 reflection 0.3 }
}

// four sided pyramid
triangle { <-1.5, 0, -1.5>, <1.5, 0, -1.5>, <0, 2, 0>
    rotate <0, -20, 0>
    translate <-4, -4, 3.5>
    pigment { color rgb <1.0, 1.0, 0.0> }
    finish { ambient 0.2 diffuse 0.4 specular 0.8 roughness 0.05 reflection 0.3 }
}
triangle { <-1.5, 0, -1.5>, <-1.5, 0, 1.5>, <0, 2, 0>
    rotate <0, -20, 0>
    translate <-4, -4, 3.5>
    pigment { color rgb <1.0, 1.0, 0.0> }
    finish { ambient 0.2 diffuse 0.4 specular 0.8 roughness 0.05 reflection 0.3 }
}
triangle { <-1.5, 0, 1.5>, <1.5, 0, 1.5>, <0, 2, 0>
    rotate <0, -20, 0>
    translate <-4, -4, 3.5>
    pigment { color rgb <1.0, 1.0, 0.0> }
    finish { ambient 0.2 diffuse 0.4 specular 0.8 roughness 0.05 reflection 0.3 }
}
triangle { <1.5, 0, 1.5>, <1.5, 0, -1.5>, <0, 2, 0>
    rotate <0, -20, 0>
    translate <-4, -4, 3.5>
    pigment { color rgb <1.0, 1.0, 0.0> }
    finish { ambient 0.2 diffuse 0.4 specular 0.8 roughness 0.05 reflection 0.3 }
}

