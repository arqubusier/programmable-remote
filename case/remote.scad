include <cherrymx.scad>

$fn = 20;
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
            cylinder(h=chamfer, r1=chamfer, r2=0.4);
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


battery_y = 44+1.5;
battery_d = 10;
battery_z = battery_d + 1.1;
battery_clip_x = 6.9 + .2;
battery_clip_y = .5 + .2;
battery_clip_plus_y = 2;
battery_clip_wall_thick = .8;
battery_clip_holder_y = battery_clip_plus_y + 2*battery_clip_wall_thick;
battery_holder_y = battery_y + 2*battery_clip_holder_y;
battery_holder_side_thick = 1;
battery_holder_x = 2*battery_d + battery_holder_side_thick;

module BatteryClipHolderNegative() {
    translate([0,.1,0]){
            rotate([90, 0, 0])
                cylinder(h= battery_clip_wall_thick+.2, d=battery_clip_x - 1);
            translate([battery_d, 0, 0])
                rotate([90, 0, 0])
                    cylinder(h= battery_clip_wall_thick+.2, d=battery_clip_x - 1);
    }
    
    translate([-battery_clip_x/2, -battery_clip_wall_thick-battery_clip_y, -battery_clip_x/2])
    {
        cube([battery_clip_x, battery_clip_y, battery_d]);
        translate([battery_d,0,0])
            cube([battery_clip_x, battery_clip_y, battery_d]);
    }
    
    translate([-battery_clip_x/2, -battery_clip_wall_thick-battery_clip_y, -battery_d/2]) {
        cube([battery_d + battery_clip_x, battery_clip_y, battery_d]);
    }
    
    translate([-battery_clip_y/2, -battery_clip_holder_y - battery_clip_wall_thick, -battery_clip_y + battery_d/2-1]) {
        cube([battery_clip_y,battery_clip_holder_y,battery_clip_y+1]);
        translate([battery_d, 0, 0])
            cube([battery_clip_y,battery_clip_holder_y,battery_clip_y+1]);
    }
    
    
    translate([-(battery_clip_x - 1)/2,-battery_clip_wall_thick - .1,0]) {
        cube([battery_clip_x - 1, battery_clip_wall_thick + .2, battery_d/2+1]);
        translate([battery_d, 0, 0])
            cube([battery_clip_x - 1, battery_clip_wall_thick + .2, battery_d/2+1]);
    }
}

module BatteryHolderNegative() {
    translate([battery_d/2, battery_clip_holder_y, battery_d/2]){
        rotate([-90, 0, 0])
            #cylinder(battery_y,d=battery_d);
        translate([battery_d, 0, 0])
            rotate([-90, 0, 0])
                cylinder(battery_y,d=battery_d);
        BatteryClipHolderNegative();
        translate([battery_d, battery_y, 0])
            rotate([0,0,180])
                BatteryClipHolderNegative();
     }

    translate([0,battery_clip_holder_y,battery_d/2])
        cube([2*battery_d, battery_y, battery_d]);
}

plate_thick = 2;
wall_thick = 2.7;
chamfer = 1.6;

pcb_y = 72.3;
pcb_x = switch_sep*2+ 2*2.7;
pcb_z = .8;
pcb_pos_z = 3.5 + plate_thick;
pcb_pos_y = wall_thick + battery_holder_y + 1;

switch_row0_pos_y = pcb_pos_y + 9 + switch_sep/2;


width_outer = pcb_x + 2*wall_thick;
width_inner = width_outer - 2*wall_thick;
height_high_inner = battery_z + plate_thick;
height_high_outer = height_high_inner + wall_thick;
height_low_outer = pcb_pos_z + pcb_z + 5.1;
height_low_inner = height_low_outer - wall_thick;

box1_outer_y = switch_row0_pos_y - switch_sep/2;
box1_inner_y = box1_outer_y - 2*wall_thick;

box2_outer_y = 3*switch_sep + 4*chamfer;
box2_inner_y = box2_outer_y - 2*wall_thick;
box2_pos_y = box1_outer_y - 2*chamfer;
box2_pos = [0, box2_pos_y, 0];

box3_inner_y = 3.8;
box3_outer_y = box3_inner_y + 2*wall_thick;
box3_pos = box2_pos + [0, box2_outer_y - 2*chamfer, 0];

outer_z = box2_pos_y+ box2_outer_y + box3_outer_y - 2*chamfer;
inner_z = outer_z - 2*wall_thick;

plate_lip_z = .6;
plate_lip_x = width_inner +2*.5;
plate_lip_y = inner_z +2*.5;

module BatteryHolder() {
    
    translate([wall_thick + battery_holder_x, wall_thick-battery_clip_wall_thick, height_high_inner])
        rotate([0,180,0])
        difference() {
            cube([battery_holder_x, battery_holder_y, battery_d]);
            translate([battery_holder_side_thick, 0, 0])
            BatteryHolderNegative();
        }
}

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
    
    #translate([wall_thick, switch_row0_pos_y - switch_side_inner/2-7+.1, pcb_pos_z])
        cube([width_inner, 7, 3.8]);
    #translate([wall_thick, switch_row0_pos_y +2*switch_sep+ switch_side_inner/2-.1, pcb_pos_z])
        cube([width_inner, 7, 3.8]);
}

niche_pos_z = 1.6;
niche_x = 3.2;
niche_y = 1.2;
niche_z = 1.2;
module Niche() {
    translate([0,0,niche_pos_z+niche_z/2])
        cube([niche_x, niche_y, niche_z], center=true);
}

module PlaceNiches() {
    nxs = [-9, 9];
    for (x = nxs) {
        translate([width_outer/2+x,wall_thick-niche_y/2+.1,0]) {
            rotate([0,0,180])
                children();
            translate([0, inner_z + niche_y -.2, 0])
                children();
        }
    }
    
    nys = [-45,-25, 0, 25, 45];
    for (y = nys) {
        translate([wall_thick-niche_y/2+.1,y+outer_z/2,0]) {
            rotate([0,0,90])
                children();
            translate([width_inner-.2+niche_y, 0, 0])
                rotate([0,0,-90])
                    children();
        }
    }
}

hook_x = niche_x - .2;
hook_y = niche_y;
hook_z = niche_z - .2;
module Hook() {
    translate([hook_x/2, -hook_y/2 -.2, niche_pos_z])
       rotate([0,-90,0])
        linear_extrude(hook_x)
            polygon([[0, 0], [hook_y, 0], [0,hook_z]]);
}




module Plate(clearance=0) {
    PlaceNiches()
        Hook();
    
    translate([wall_thick-clearance, wall_thick-clearance, 0])
        cube([width_inner+2*clearance, inner_z+2*clearance, plate_thick+clearance]);
   
    translate([wall_thick-.5-clearance, wall_thick -.5-clearance, 0])
        cube([plate_lip_x+2*clearance, plate_lip_y+2*clearance, plate_lip_z+clearance]);
}



ir_tx_x = 3.4;
ir_tx_rel_x = -3.8-ir_tx_x;
ir_rx_x = 6.6;
ir_rx_rel_x = 3.5;
module SensorHoles() {
    #translate([width_outer/2, 2*chamfer + outer_z-1*wall_thick, pcb_pos_z + pcb_z]) {
        translate([ir_tx_rel_x, -.1, 0])
            rotate([90, 0, 0])
                BoxFilletSides(ir_rx_x, 2, 4, .5);
        translate([ir_rx_rel_x, 0, ir_tx_x/2])
            rotate([90, 0, 0])
                cylinder(h = 10, d=ir_tx_x);
    }
    
    BoxFilletSides(2,2,wall_thick,.5);
}

module SwitchHoles() {
    translate ([width_outer/2-switch_sep/2, switch_row0_pos_y, height_low_outer - switch_height])
        for (col = [0:1]) {
            for (row = [0:2]) {
                translate([col*switch_sep, row*switch_sep, 0])
                    switch_neg(3);
            }
        }
}

module Pcb() {
    translate([wall_thick,pcb_pos_y,pcb_pos_z]) 
        cube([pcb_x, pcb_y, pcb_z]);
}

module Shell() {
    difference() {
        union() {
            difference() {
                ShellPositive();
                ShellNegative();
            }
            BatteryHolder();
        }

        SensorHoles();
        SwitchHoles();
        Plate(.2);
        PlaceNiches()
            Niche();
    }
}

Shell();
//Pcb();
//Plate();