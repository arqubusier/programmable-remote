vec3d@[] mirror_x(vec3d@[] v) {
   vec3d@[] res;
   for (uint i=0; i<v.size(); i++) {
      res.push_back(vec3d(-v[i].x(), v[i].y(), v[i].z()));
   }
   return res;
}

vec3d@[] mirror_y(vec3d@[] v) {
   vec3d@[] res;
   for (uint i=0; i<v.size(); i++) {
      res.push_back(vec3d(v[i].x(), -v[i].y(), v[i].z()));
   }
   return res;
}

solid@ place_snap_objects(vec3d@[] points_x, vec3d@[] points_y, solid@ obj_x, solid@ obj_y) {
   solid@ first = translate(points_x[0])*obj_x;
   solid@ result = first + mirror(1,0,0)*first;
   for(uint i=1; i<points_x.size(); i++) {
      solid @obj = translate(points_x[i])*obj_x;
      @result=result + obj + mirror(1,0,0)*obj;
   }
   for(uint i=0; i<points_y.size(); i++) {
      solid @obj = translate(points_y[i])*obj_y;
      @result=result + obj + mirror(0,1,0)*obj;
   }
   return result;
}

shape@ main_shape()
{

   double height=5;
   double length=50;
   double width=40;
   double chamfer=2;
   double wall_thick=2;
   
   solid@ negative = translate(0, 0, height/2)*cuboid(width-2*chamfer, length-2*chamfer, height - chamfer, center:true);
   
   solid@ chamfer_add = cone(h:chamfer, r1:chamfer, r2:0, center:true);
   solid@ positive = minkowski3d(negative, chamfer_add);
   
   solid@ shell = positive - scale(1, 1, 3)*translate(0,0,-1)*negative;
   
   
   double niche_w = 0.5;
   double niche_l = 1;
   double niche_h = 1;
   double niche_z = 1+niche_l/2;
   
   double niche_x = width/2-wall_thick+niche_w/2;
   vec3d@[] niche_points_x = {
      vec3d(niche_x, -15, niche_z),
      vec3d(niche_x,  5, niche_z),
      vec3d(niche_x, -5, niche_z),
      vec3d(niche_x,  15, niche_z)
   };
   double niche_y = length/2-wall_thick+niche_w/2;
   vec3d@[] niche_points_y = {
      vec3d(-5, niche_y, niche_z),
      vec3d(5, niche_y, niche_z)
   };
   
   solid@ niche = cuboid(niche_w, niche_l, 1, center:true);
   @shell = shell - place_snap_objects(niche_points_x, niche_points_y, niche, rotate_z(90)*niche);
   
   double plate_w = width-2*wall_thick;
   double plate_l = length-2*wall_thick;
   double plate_h = 1.5;
   solid@ plate = translate(0, 0, plate_h/2)*cuboid(plate_w, plate_l, plate_h, center:true);
   solid@ plate_lip = translate(0, 0, .5/2)*cuboid(width-wall_thick, length-wall_thick, .5, center:true);
   solid@ plate_lip_clearance = translate(0, 0, .6/2)*cuboid(width-wall_thick+.2, length-wall_thick+.2, .6, center:true);
   @plate = plate + plate_lip;
   double hook_h = niche_h;
   double hook_w = niche_w;
   double hook_l = niche_l-.2;
   solid@ hook = linear_extrude(polygon(pos2d(0,0), pos2d(0,hook_h), pos2d(hook_w, 0)), hook_l);
   @hook = translate(-hook_w/2-.1, hook_l/2, -hook_h/2)*rotate_x(90)*hook;
   @plate = plate + place_snap_objects(niche_points_x, niche_points_y, hook, rotate_z(90)*hook);
   
   @shell = shell - plate_lip_clearance;
   return shell;
   return plate;
}

void main()
{
   shape@ obj = main_shape();
   obj.write_xcsg(GetInputFullPath(),secant_tolerance:-1.0);
}