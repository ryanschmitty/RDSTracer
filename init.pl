#!/usr/bin/perl

use strict;
use warnings;

if (@ARGV != 1) {
    print "Usage: init.pl <sdk path>\n";
}
my $sdk = $ARGV[0];
open FILE, ">", "src/blueprint.lua" or die $!;

print FILE "gpu_sdk_path \"$sdk\"\n\n";
print FILE "default \"release\"\n\n";
print FILE "config \"debug\"\n";
print FILE "    output \"raytrace-debug\"\n";
print FILE "    debugging \"on\"\n";
print FILE "    profiling \"off\"\n";
print FILE "    optimizing \"off\"\n";
print FILE "    includes {\"../glm\"}\n";
print FILE "    compute_capability \"2.0\"\n";
print FILE "    defines {\"DEBUG\"}\n\n";

print FILE "config \"profile\"\n";
print FILE "    output \"raytrace-profile\"\n";
print FILE "    debugging \"on\"\n";
print FILE "    profiling \"on\"\n";
print FILE "    optimizing \"on\"\n";
print FILE "    includes {\"../glm\"}\n";
print FILE "    compute_capability \"2.0\"\n";
print FILE "    defines {\"DEBUG\"}\n\n";

print FILE "config \"release\"\n";
print FILE "    output \"raytrace\"\n";
print FILE "    debugging \"off\"\n";
print FILE "    profiling \"off\"\n";
print FILE "    optimizing \"on\"\n";
print FILE "    defines {\"NDEBUG\", \"RELEASE\"}\n";
print FILE "    includes {\"../glm\"}\n";
print FILE "    compute_capability \"2.0\"\n";
print FILE "    cflags {\"-Wall\", \"-ffast-math\"}\n";
