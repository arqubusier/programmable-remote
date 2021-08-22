include <cherrymx.scad>


module TriangleRightAngle(w, h) {
    polygon([[0,0], [w,0], [w,h]]);
}

module BoxChamfer(w,l,h,chamfer) {
    translate([chamfer,chamfer,0]) {
        minkowski() {
            cube([w-2*chamfer,l-2*chamfer,h-2*chamfer]);
            union() {
                translate([0,0,chamfer])
                    cylinder(h=chamfer, r1=chamfer, r2=0);
                cylinder(h=chamfer, r1=0, r2=chamfer);
            }
        }
    }
}

module BoxChamferTop(w,l,h,chamfer) {
    translate([chamfer,chamfer,0]) {
        minkowski() {
            cube([w-2*chamfer,l-2*chamfer,h-chamfer]);
            cylinder(h=chamfer, r1=chamfer, r2=0);
        }
    }
}

module BoxFilletSides(x, y, z, chamfer) {
    translate([chamfer,chamfer,0]) {
        minkowski() {
            cube([x-2*chamfer,y-2*chamfer,z-2*chamfer]);
            cylinder(h=chamfer, r=chamfer);
        }
    }
}

chamfer = 2;
wall_thick = 2;
width_outer = switch_side_inner + switch_sep + 2*chamfer;
width_inner = width_outer - 2*wall_thick;
height_high_inner = 10;
height_high_outer = height_high_inner + wall_thick;
height_low_inner = 5;
height_low_outer = height_low_inner + wall_thick;

box1_inner_y = 30;
box1_outer_y = box1_inner_y + 2*wall_thick;

box2_outer_y = 3*switch_sep + 4*chamfer;
box2_inner_y = box2_outer_y - 2*wall_thick;
box2_pos_z = box1_outer_y - 2*chamfer;
box2_pos = [0, box2_pos_z, 0];

box3_inner_y = 10;
box3_outer_y = box3_inner_y + 2*wall_thick;
box3_pos = box2_pos + [0, box2_outer_y - 2*chamfer, 0];

outer_z = box2_pos_z + box2_outer_y + box3_outer_y;

module ShellPositive() {
            BoxChamfer(width_outer, box1_outer_y, height_high_outer, chamfer);
        translate(box2_pos)
            BoxChamfer(width_outer, box2_outer_y, height_low_outer, chamfer);
        translate(box3_pos)
            BoxChamfer(width_outer, box3_outer_y, height_high_outer, chamfer);
}

module ShellNegative() {
    translate([wall_thick, wall_thick, -.1]) {
        BoxChamferTop(width_inner, box1_inner_y, height_high_inner, chamfer);
        translate(box2_pos + [0, -wall_thick*2, 0])
            BoxChamferTop(width_inner, box2_inner_y+wall_thick*4, height_low_inner, chamfer);
        translate(box3_pos)
            BoxChamferTop(width_inner, box3_inner_y, height_high_inner, chamfer);
    }
}

pcb_pos_z = chamfer + 1;
pcb_z = 1;
module SensorHoles() {
    translate([width_outer/2, outer_z-1.5*wall_thick, pcb_z + pcb_pos_z]) {
        translate([3, 0, 0])
            rotate([90, 0, 0])
                BoxFilletSides(3, 2, 2*wall_thick, .5);
        translate([-3, 0, 0])
            rotate([90, 0, 0])
                cylinder(h = 2*wall_thick, r=3);
    }
}

module SwitchHoles() {
    translate (
        box2_pos +
        [wall_thick+switch_side_inner/2, chamfer+wall_thick+switch_sep/2, height_low_outer - switch_height])
    for (col = [0:1]) {
        for (row = [0:2]) {
            translate([col*switch_sep, row*switch_sep, 0])
                switch_neg();
        }
    }
}

difference() {
    ShellPositive();
    ShellNegative();
    SensorHoles();
    SwitchHoles();
}