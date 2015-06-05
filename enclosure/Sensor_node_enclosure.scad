height_upper = 7+4;
width = 61+4;
length = 88+4;
wall = 2;
button_rad=3.81;
stopper_rad=button_rad+1;
stopper_diam=2*stopper_rad;

part="all";

view();
module upper_simple()
{
    $fn=20;
    difference()
    {
        cube([length,width,height_upper], false);  //inner height of 9mm
        translate([2,2,2]) cube([length - wall*2,width-wall*2,height_upper-wall+1],false);
        //creating ledge for bottom 
        translate([-1,-1, height_upper-1]) cube([length+2,2,2], false);
        translate([-1,width-1, height_upper-1]) cube([length+2,2,2], false);
        translate([-1,-1, height_upper-1]) cube([2,width+2,2], false);
        translate([length-1,-1, height_upper-1]) cube([2,width+2,2], false);
        //LCD cutouts 
        translate([24.7+2,15.1+2,-1]) /*LCD1 43.6x34*/ cube([43.6,34,4], false);
        translate([3+2,25+2,-1]) /*LCD2 20x15.1*/ cube([15.1, 20, 4], false);
        //button holes
        translate([length-8.51,19.05+2,-1]) #cylinder (h=4,r=button_rad);
        translate([length-8.51,29.21+2,-1]) #cylinder (h=4,r=button_rad);
        translate([length-8.51,39.37+2,-1]) #cylinder (h=4,r=button_rad);
        translate([length-8.51,49.53+2,-1]) #cylinder (h=4,r=button_rad);
        //power supply hole
        translate([length-3,wall+3.15,height_upper-1]) cube([4,10,13]);
    }
    //main mounting holes depth of 7mm, outer radius of 2mm inner radius of 1mm
    difference()
    {
        translate([3,3,0]) cylinder (h=7+4,r=3, $fn=100);
        translate([3,3,1]) #cylinder (h=7+4,r=1, $fn=100);
        translate([-1,-1, height_upper-1]) cube([length+2,2,2], false);
        translate([-1,-1, height_upper-1]) cube([2,width+2,2], false);
    }
    difference()
    {
        translate([length-5,2,0]) cube([3,3,11]);
        translate([length-3,3,1]) cylinder (h=12,r=1, $fn=100);
        translate([-1,-1, height_upper-1]) cube([length+2,2,2], false);
        translate([length-1,-1, height_upper-1]) cube([2,width+2,2], false);
    }
    difference()
    {
        translate([length-3,width-3,0]) cylinder (h=7+4,r=3, $fn=100);
        translate([length-3,width-3,1]) #cylinder (h=12,r=1, $fn=100);
        translate([-1,width-1, height_upper-1]) cube([length+2,2,2], false);
        translate([length-1,-1, height_upper-1]) cube([2,width+2,2], false);
    }
    difference()
    {
        translate([3,width-3,0]) cylinder (h=11,r=3, $fn=100);
        translate([3,width-3,1]) #cylinder (h=12,r=1, $fn=100);
        translate([-1,width-1, height_upper-1]) cube([length+2,2,2], false);
        translate([-1,-1, height_upper-1]) cube([2,width+2,2], false);
    }
    //LCD mounting Holes -> extruded hole is 1mm into the face of the cover. total depth of screw hole is 2.5mm
    difference()
    {
        translate([24.4, width-14.2,0]) cylinder(h=wall+1,r=2, $fn=100);
        translate([24.4, width-14.2,1]) #cylinder(h=wall+1,r=.5, $fn=100);
    }
    difference ()
    {
        translate([wall+22.4,wall+16.4,0]) cylinder(h=wall+1,r=2, $fn=100);
        translate([wall+22.4,wall+16.4,1]) #cylinder(h=wall+1,r=.5, $fn=100);
    }
    difference ()
    {
        translate([length-19.4,width-14.2,0]) cylinder(h=wall+1,r=2, $fn=100);
        translate([length-19.4,width-14.2,1]) #cylinder(h=wall+1,r=.5, $fn=100);
    }
    difference ()
    {
        translate([length-19.4, 18.4,0]) cylinder(h=wall+1,r=2, $fn=100);
        translate([length-19.4, 18.4,1]) #cylinder(h=wall+1,r=.5, $fn=100);
    }
    //DHT mounting hole
    difference()
    {
        translate([5,23,2]) cube([15.1,4,3], false);
        translate([12.55,25.22,1]) #cylinder(h=wall+4,r=.5,$fn=100,center=false);
    }
}
module base_simple()
{
    //main shell
    difference ()
    {
       cube([92,65,18]); //inner height of 16mm 
       translate([2,2,2]) cube([88,61,18]);
       translate([1,1,17]) cube([90,63,5]);

       //hanger 1
       translate([15+wall,width/2,-1]) cylinder(r=4,h=6);
       translate([15+wall,(width/2)+5,-1]) cylinder(r=2.7,h=6);

       translate([length-15,width/2,-1]) cylinder(r=4,h=6);
       translate([length-15,(width/2)+5,-1]) cylinder(r=2.7,h=6);

       //power supply cutout 
         //4.4mm above face (3mm for mounting height+1.4mm for pcb width
       translate([length-3,wall+3.18,wall+5]) #cube([6,10,12]);//10x10mm hole
       //vent holes
       translate([10,-1,4]) cube([1,width+2,10]);
       translate([12,-1,5]) cube([1,width+2,10]);
       translate([14,-1,6]) cube([1,width+2,10]);
       translate([20,-1,4]) cube([1,width+2,10]);
       translate([18,-1,5]) cube([1,width+2,10]);
       translate([16,-1,6]) cube([1,width+2,10]);
       translate([50,-1,4]) cube([1,width+2,10]);
       translate([52,-1,5]) cube([1,width+2,10]);
       translate([54,-1,6]) cube([1,width+2,10]);
       translate([60,-1,4]) cube([1,width+2,10]);
       translate([58,-1,5]) cube([1,width+2,10]);
       translate([56,-1,6]) cube([1,width+2,10]);
    }
    //main screw holes
    difference()
    {
        translate([3,3,0]) cylinder (h=7+4,r=3, $fn=100);
        translate([3,3,1]) #cylinder (h=7+4,r=1, $fn=100);
    }
    difference()
    {
        translate([length-5,2,0]) cube([3,3,11]);
        translate([length-3,3,1]) #cylinder (h=12,r=1, $fn=100);
        
    }
    difference()
    {
        translate([length-3,width-3,0]) cylinder (h=7+4,r=3, $fn=100);
        translate([length-3,width-3,1]) #cylinder (h=12,r=1, $fn=100);
    }
    difference()
    {
        translate([3,width-3,0]) cylinder (h=11,r=3, $fn=100);
        translate([3,width-3,1]) #cylinder (h=12,r=1, $fn=100);
    }
    //pcb mounting holes
    difference()
    {
        translate([wall+8.9,wall+6.35,wall]) cylinder(h=3,r=3,$fn=20);
        translate([wall+8.9,wall+6.35,wall-1]) #cylinder(h=5,r=.5,$fn=20);
    }
    difference()
    {
        translate([wall+9.65,width-8.35,wall]) cylinder(h=3,r=3,$fn=20);
        translate([wall+9.65,width-8.35,wall-1]) #cylinder(h=5,r=.5,$fn=20);
    }
    difference()
    {
        translate([length-17.61,width-8.35,wall]) cylinder(h=3,r=3,$fn=20);
        translate([length-17.61,width-8.35,wall-1]) #cylinder(h=5,r=.5,$fn=20);
    }
    difference()
    {
        translate([length-21.42,8.35,wall]) cylinder(h=3,r=3,$fn=20);
        translate([length-21.42,8.35,wall-1]) #cylinder(h=5,r=.5,$fn=20);
    }    
    //hanger 2
//    difference()
//    {
//        translate([-10,(width/2)-7.5,0]) cube([10,15,2]);
//        translate([-12,(width/2)-6,1]) rotate([0,0,60]) cube([5,15,4],true);
//        translate([-12,38.5,1]) mirror([0,1,0]) rotate([0,0,60]) cube([5,15,4],true);
//        translate([-5,(width/2),-1]) cylinder(r=2,h=4);
//    } 
}
module button()
{
    $fn=20;
    union()
    {
        //stopper
        cylinder(h=wall,r=stopper_rad);
        //button height
        cylinder(h=wall+wall+4,r=button_rad-0.50);
        //actuation height
        mirror([0,0,1]) cylinder(h=17,r=2);
    }
}
module view_all()
{
    union()
    {
        mirror([0,1,0]) translate([10,-width,0]) upper_simple();
        mirror([0,0,0]) translate([10,width+2,0]) base_simple();
        mirror([0,0,1]) translate([44,3*stopper_diam+stopper_rad+4,-8]) button();
        mirror([0,0,1]) translate([44,2*stopper_diam+stopper_rad+2,-8]) button();
        mirror([0,0,1]) translate([55,3*stopper_diam+stopper_rad+4,-8]) button();
        mirror([0,0,1]) translate([55,2*stopper_diam+stopper_rad+2,-8]) button();
    }
}
module view()
{
    if(part=="lower") base_simple();
    if(part=="upper") upper_simple();
    if(part=="button") button();
    if(part=="all") view_all();
}
//inner height is 25mm, 9mm in the lid, 16mm in the bottom.
//power jack location fixed, upper modified for new location of power jack
//added wall mounts (you can keep them both or remove the ugly one.
//all pcb/lcd/DTH mounting holes are 1mm DIAMETER, we should be able to use all the same screws for everything. the only iffy one is the DHT. so worst case is we need to buy 1 screw. 

