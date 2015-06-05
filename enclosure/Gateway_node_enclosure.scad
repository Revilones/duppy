//total board length = 86.8mm
//total board width = 58.5mm
//center of hole to edge of usb connector is 83.3mm
//width of ethernet port=15.112mm
//height of ethernet port=13
//stand off's 3mm high(shit on bottom of pi)
//pcb width=1.4mm

//pin length should be 35-37mm in length 2mm in diameter.

outer_length=94.6;
outer_width=66.8;
inner_length=outer_length-4;
inner_width=outer_width-4;
inner_height=35;
outer_height=inner_height+4;
base_height=25;
stand_off_height=3;
wall=2;
$fn=25;

part="all";
view();

module base()
{
 union()
 {
     //main shell
     difference()
     {
        cube ([outer_length,outer_width,base_height]);
        translate([2,2,2]) #cube([inner_length,inner_width,inner_height]);
     //hanger 1
        $fn=100;
        translate([10,outer_width/2,-1]) cylinder(r=3,h=6);
        translate([7,outer_width/2,-1]) cylinder(r=2,h=6);
     //lid bevel
        translate([1,1,base_height-1]) cube([inner_length+2,inner_width+2,2]);
     //ethernet port
        translate([outer_length-4,5.194+2,wall+stand_off_height+1.4]) cube([6,15.5,13]);
     //micro usb port
        translate([outer_length-27.375-58+3.26,-1,wall+1.4+3]) cube([7.5,6,2.75]);
        translate([outer_length-27.375-58+3.26-1,-1,wall+1.4+2]) cube([9.5,2,4.75]);
     //air vents
        translate([20,12,-1]) cube([1,40,6]);
        translate([23,12,-1]) cube([1,40,6]);
        translate([26,12,-1]) cube([1,40,6]);
        translate([29,12,-1]) cube([1,40,6]);
     
        translate([45,12,-1]) cube([1,40,6]);
        translate([48,12,-1]) cube([1,40,6]);
        translate([51,12,-1]) cube([1,40,6]);
        translate([54,12,-1]) cube([1,40,6]);
    }
    //pcb mounts
    difference()
    {
        translate([outer_length-27.375,wall+6,wall]) cylinder(h=3,r=3.1);
        translate([outer_length-27.375,wall+6,wall]) cylinder(h=4,r=1);
    }
    difference()
    {
        translate([outer_length-27.375-58,wall+6+49,wall]) cylinder(h=3,r=3.1);
        translate([outer_length-27.375-58,wall+6+49,wall]) cylinder(h=4,r=.5);
    }
    difference()
    {
        translate([outer_length-27.375-58,wall+6,wall]) cylinder(h=3,r=3.1);
        translate([outer_length-27.375-58,wall+6,wall]) #cylinder(h=4,r=.5);
    }
    difference()
    {
        translate([outer_length-27.375,wall+6+49,wall]) cylinder(h=3,r=3.1);
        translate([outer_length-27.375,wall+6+49,wall]) cylinder(h=4,r=.5);
    }
    //pin mounts
    difference()
    {
        translate([3,3,0]) cylinder (h=7+4,r=3, $fn=100);
        translate([3,3,1]) #cylinder (h=7+4,r=1, $fn=100);
    }
    difference()
    {
        translate([outer_length-3,outer_width-3,0]) cylinder(h=7+4,r=3, $fn=100);
        translate([outer_length-3,outer_width-3,1]) #cylinder(h=7+4,r=1, $fn=100);
    }
    difference()
    {
        translate([3,outer_width-3,0]) cylinder(h=7+4,r=3, $fn=100);
        translate([3,outer_width-3,1]) #cylinder(h=7+4,r=1, $fn=100);
    }
    //hanger 2
    difference()
    {
        $fn=100;
        translate([-10,(outer_width/2)-7.5,0]) cube([10,15,2]);
        translate([-12,(outer_width/2)-6,1]) rotate([0,0,60]) cube([5,15,4],true);
        translate([-12,39.5,1]) mirror([0,1,0]) rotate([0,0,60]) cube([5,15,4],true);
        translate([-5,(outer_width/2),-1]) cylinder(r=2,h=4);
    } 
 }
}
module top()
{
    union()
    {
        difference()
        {
        cube ([outer_length,outer_width,17]);
        translate([2,2,2]) cube([inner_length,inner_width,inner_height]);
        //lid bevel
        translate([-1,-1,16]) cube([outer_length+2,2,2]);
        translate([-1,-1,16]) cube([2,outer_width+2,2]);
        translate([outer_length-1,-1,16]) cube([2,outer_width+2,2]);
        translate([-1,outer_width-1,16]) cube([outer_length+2,2,2]);    
        }
        //pin holes
        difference()
        {
        translate([3,3,0]) cylinder (h=7+4,r=3, $fn=100);
        translate([3,3,1]) #cylinder (h=7+4,r=1, $fn=100);
        }
        difference()
        {
        translate([outer_length-3,3,0]) cylinder(h=7+4,r=3, $fn=100);
        translate([outer_length-3,3,1]) cylinder(h=7+4,r=1, $fn=100);
        }
        difference()
        {
        translate([3,outer_width-3,0]) cylinder(h=7+4,r=3, $fn=100);
        translate([3,outer_width-3,1]) cylinder(h=7+4,r=1, $fn=100);
        }
        difference()
        {
        translate([outer_length-27.375,outer_width-(wall+6),wall]) cylinder(h=3,r=3.1);
        translate([outer_length-27.375,outer_width-(wall+6),wall]) cylinder(h=4,r=1);
        }
        
    }
}
module view_all()
{
    union()
    {
        translate([10,0,0]) base();
        translate([10,outer_width+3,0]) top();
    }
}
module view()
{
    if(part=="base") base();
    if(part=="top") top();
    if(part=="all") view_all();
}

//pin length should be 38mm in length. the lid was extended upwards to create space for longer pin (was the best fit i could find at ACE)
//pcb mounting holes 1mm in diameter, 4mm in depth
//you'll notice that there are only 3 pin holes. the board gets in the way of the 4th pin location due to how close it has to be to the edge of the case. This can be solved by placing a 2mm pin (same one used for the main supports) through the pcb, into that hole, and into the adjacent hole in the lid. you'll still have 4 spport contacts and 3 is DEF enough to hold that board down. the friction from the pin should be enough to keep the lid from flexing as well.
//there is one PCB mounting hole that is 2mm diameter for pin instead of screw
