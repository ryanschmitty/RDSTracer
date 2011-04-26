#!/usr/bin/perl

use strict;
use warnings;
use Cwd;
use Env qw(@PATH);
use IPC::Open3;
use Symbol qw(gensym);

if (@ARGV == 0) {
    print "Usage: build.pl <target> [<nvcc path>] [<gcc path>]";
}

my $target = "";
my $nvcc = "";
my $gcc = "";

if (@ARGV > 0) {
    $target = $ARGV[0];
}
if (@ARGV > 1) {
    $nvcc = $ARGV[1];
}
if (@ARGV > 2) {
    $gcc = $ARGV[2];
}

my $dir = getcwd;
my @oldPath = @PATH;
@PATH = ( "$dir/cubuild" );
if (length $nvcc != 0) {
    push @PATH, $nvcc;
}
if (length $gcc != 0) {
    push @PATH, $gcc;
}
push @PATH, @oldPath;

chdir 'src' or die $!;
my $pid = open3(gensym, \*OUT, \*OUT, 'cubuild', $target);
while (<OUT>) {
    print $_;
}
waitpid($pid, 0);
close(OUT);
