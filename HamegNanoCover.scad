/* Housing for the Arduino Nano Interface 
 *  See readme.md for further details
 *
 * Print with PLA, perhaps support for USB socket hole. Use 40 % infill (screw domes).
 *
 * Copyright, 2023, Mathias Moog, Deutschland, CC-BY-NC-SA
 */
 
$fn=30;
 
// Wall Thickness
w = 1.2;
// Space arround Nano
s = 0.3;
// ISP heigt
h_isp = 8;
// Top of Nano PCB to top of matrix board
h_nm  = 12.5;

// Diameter for the screw holes, arduino 1.8 mm
d = 1.4;
// You need 4 screws with 1.6 mm diameter and 20 mm length

// USB Heigt
u_h = 4;
// USB Width
u_w = 8;
// Space arround USB
u_s = 2;

// Arduino nano dimenisons from Datasheet

// length
n_l  = 43.2;
// hole distance in length direction
n_hl = 40.64;

// width
n_w  = 18;
n_hw = 15.24;

difference() {
  // complet material
  cube([n_w+2*(s+w),n_l+2*(s+w),h_isp+h_nm+s+w]);
  // Below Arduino
  translate([w,w,h_isp+s+w]) cube([n_w+2*s,n_l+2*s,h_nm+s]);
  // Arduion, without area for mounting hole
  // widht, a bit more for mountings
  wm=3;
  translate([w+wm+s,w,w])
    cube([n_w-2*wm,n_l+2*s,h_isp+2*s]);
  // length a bit less
  wl=2.54;
  translate([w,w+wl+s,w])
    cube([n_w+2*s,n_l-2*wl,h_isp+2*s]);
  // Screw holes
  h_w = w+s+(n_w-n_hw)/2;
  h_l = w+s+(n_l-n_hl)/2;
  translate([h_w,h_l,w])
    cylinder(d=d,h=h_isp+2*s);
  translate([h_w+n_hw,h_l,w])
    cylinder(d=d,h=h_isp+2*s);
  translate([h_w,h_l+n_hl,w])
    cylinder(d=d,h=h_isp+2*s);
  translate([h_w+n_hw,h_l+n_hl,w])
    cylinder(d=d,h=h_isp+2*s);
  // USB Hole
  u_W = w+s+(n_w-u_w)/2-u_s;
  u_H = w+s+h_isp-u_h-u_s;
  translate([u_W,-w,u_H])
    cube([u_w+2*u_s,3*w,u_h+2*u_s]); 
}  
