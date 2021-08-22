switch_side_outer=19.1;
switch_sep = 19.05;
switch_side_inner=13.6;
switch_height=3;
plate_thick_d=0;
plate_thick=1.5 + plate_thick_d;
inset_h=switch_height - plate_thick;
inset_w=5;
inset_d=0.7;
outset_d=0.2;
switch_pcb_to_plate_top = 4.7;

module inset() {
    translate([0, switch_side_inner/2 + inset_d/2, (switch_height)/2 -inset_h - plate_thick])
        cube([inset_w, inset_d, 2*inset_h], center=true);
}

module outset() {
    translate([0, switch_side_inner/2 - outset_d/2, switch_height/2-plate_thick/2])
        difference() {
            cube([inset_w,outset_d,plate_thick], center=true);
            cube([inset_w-2.5,outset_d,plate_thick], center=true);
        }
}

module switch_pos(height=switch_height, center_height=false) {
    if (center_height) {
        cube([switch_side_outer, switch_side_outer, height], center=true);
    } else {
        translate([0, 0, height/2])
            cube([switch_side_outer, switch_side_outer, height], center=true);
    }
}

module SwitchPos2D() {
       square(switch_side_outer, center=true);
}

module SwitchNeg2D() {
       square(switch_side_inner, center=true);
}

module switch_neg_cube() {
    translate([0,0,switch_height/2])
        cube([switch_side_inner, switch_side_inner, switch_height], center=true);
}

module switch_neg(switch_height_mult=1) {
    translate([0,0,switch_height/2]) {
        difference() {
            cube([switch_side_inner, switch_side_inner, switch_height_mult*switch_height], center=true);
            *outset();
            *mirror([0,1,0])
                outset();
        }
        inset();
        mirror([0,1,0])
            inset();
    }
}

module switch(side_outer=switch_side_outer) {
    difference() {
        switch_pos(side_outer);
        switch_neg(1);
    }
}

module keycap(center=true) {
  cube([18, 18, 8.5],center=center);
}

function switch_corner_outer(offset, corner) = offset +
  ( (corner == "ne" ? [switch_side_outer/2, switch_side_outer/2, 0] :
     (corner == "se" ? [switch_side_outer/2, -switch_side_outer/2, 0] :
      (corner == "sw" ? [-switch_side_outer/2, -switch_side_outer/2, 0] :
       ([-switch_side_outer/2, switch_side_outer/2, 0])))) );
