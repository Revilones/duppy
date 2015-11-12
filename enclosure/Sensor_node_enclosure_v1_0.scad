wall = 2;
pcb_thickness = 1.6;
pcb_stand_h = 3;
guide_wall = 1.2;
height_upper = 16.3+wall;
height_lower = 10.7+wall;
width = 73+4;
length = 89+4;

//Width of hole for pins to connect upper and lower half
pin_hole_width = 1.5;
//Width of pilot hole for mounting screws
pilot_hole_width = .75;

//Button Dimensions
button_height = wall+pcb_stand_h+pcb_thickness+1.9;
button_width = 4.2;
button_1_pos_y = 22 + wall;
button_2_pos_y = button_1_pos_y + button_width + 8;
button_3_pos_y = button_2_pos_y + button_width + 8;
button_4_pos_y = button_3_pos_y + button_width + 8;

//LCD
lcd_pcb_pos_x = wall + 18;
lcd_pcb_pos_y = wall + 18;
lcd_pos_x = lcd_pcb_pos_x + 4.7;
lcd_pos_y = lcd_pcb_pos_y + 1.1;
lcd_width = 43.6;
lcd_height = 34;

//Light Sensor
ls_pcb_pos_x = wall + 2;
ls_pcb_pos_y = wall + 57;
ls_pos_x = ls_pcb_pos_x + 5.5;
ls_pos_y = ls_pcb_pos_y + 5.5;
ls_width = 3;
ls_height = 3;

part="all";

view();

module upper_simple()
{
    $fn=20;
    difference()
    {
        cube([length,width,height_upper], false);  //inner height of 9mm
        translate([2,2,2]) cube([length - wall*2,width-wall*2,height_upper],false);
        //creating ledge for bottom 
        translate([-1,-1, height_upper-1]) cube([length+2,2,2], false);
        translate([-1,width-1, height_upper-1]) cube([length+2,2,2], false);
        translate([-1,-1, height_upper-1]) cube([2,width+2,2], false);
        translate([length-1,-1, height_upper-1]) cube([2,width+2,2], false);

		//LCD 43.6x34
        translate([lcd_pos_x,lcd_pos_y,-1]) cube([lcd_width,lcd_height,4], false);

		//LS
	    translate([ls_pos_x,ls_pos_y,-1]) cube([ls_width,ls_height,4], false);

        //button holes
		translate([length-3,button_1_pos_y,height_upper-1]) cube([4,4.2,13]);
		translate([length-3,button_2_pos_y,height_upper-1]) cube([4,4.2,13]);
		translate([length-3,button_3_pos_y,height_upper-1]) cube([4,4.2,13]);
		translate([length-3,button_4_pos_y,height_upper-1]) cube([4,4.2,13]);

        //power supply hole
        translate([length-3,wall+3.5,height_upper-(12-(height_lower-wall-pcb_stand_h-pcb_thickness))]) cube([6,10,13]);

        //vent holes
        translate([20,-1,4]) cube([1,width+2,10]);
        translate([22,-1,5]) cube([1,width+2,10]);
        translate([24,-1,6]) cube([1,width+2,10]);
        translate([26,-1,6]) cube([1,width+2,10]);
        translate([28,-1,5]) cube([1,width+2,10]);
        translate([30,-1,4]) cube([1,width+2,10]);

        translate([60,-1,4]) cube([1,width+2,10]);
        translate([62,-1,5]) cube([1,width+2,10]);
        translate([64,-1,6]) cube([1,width+2,10]);
        translate([66,-1,6]) cube([1,width+2,10]);
        translate([68,-1,5]) cube([1,width+2,10]);
        translate([70,-1,4]) cube([1,width+2,10]);

    }

    //Pin Holes for connecting upper and lower half
    difference()
    {
        translate([3,3,0]) cylinder (h=7+4,r=3, $fn=100);
        translate([3,3,1]) #cylinder (h=7+4,r=pin_hole_width, $fn=100);
    }
    difference()
    {
        translate([length-5,2,0]) cube([3,3,11]);
        translate([length-3,3,1]) cylinder (h=12,r=pin_hole_width, $fn=100);
    }
    difference()
    {
        translate([length-3,width-3,0]) cylinder (h=7+4,r=3, $fn=100);
        translate([length-3,width-3,1]) #cylinder (h=12,r=pin_hole_width, $fn=100);
    }
    difference()
    {
        translate([3,width-3,0]) cylinder (h=11,r=3, $fn=100);
        translate([3,width-3,1]) #cylinder (h=12,r=pin_hole_width, $fn=100);
    }

	//Light Sensor Mounting Holes
	ls_mount_hole_1_x = ls_pcb_pos_x + 9;
	ls_mount_hole_1_y = ls_pcb_pos_y + 2;
    difference()
    {
        translate([ls_mount_hole_1_x,ls_mount_hole_1_y,0]) cylinder(h=wall+1,r=2, $fn=100);
        translate([ls_mount_hole_1_x,ls_mount_hole_1_y,1]) #cylinder(h=wall+1,r=pilot_hole_width, $fn=100);
    }
	ls_mount_hole_2_x = ls_pcb_pos_x + 9;
	ls_mount_hole_2_y = ls_pcb_pos_y + 12;
    difference ()
    {
        translate([ls_mount_hole_2_x,ls_mount_hole_2_y,0]) cylinder(h=wall+1,r=2, $fn=100);
        translate([ls_mount_hole_2_x,ls_mount_hole_2_y,1]) #cylinder(h=wall+1,r=pilot_hole_width, $fn=100);
    }

    //LCD mounting Holes -> extruded hole is 1mm into the face of the cover.
    //total depth of screw hole is 2.5mm
	mount_hole_1_x = lcd_pcb_pos_x + 2.4;
	mount_hole_1_y = lcd_pcb_pos_y + 2.4;
    difference()
    {
        translate([mount_hole_1_x,mount_hole_1_y,0]) cylinder(h=wall+1,r=2, $fn=100);
        translate([mount_hole_1_x,mount_hole_1_y,1]) #cylinder(h=wall+1,r=pilot_hole_width, $fn=100);
    }
	mount_hole_2_x = lcd_pcb_pos_x + 50.7;
	mount_hole_2_y = lcd_pcb_pos_y + 2.4;
    difference ()
    {
        translate([mount_hole_2_x,mount_hole_2_y,0]) cylinder(h=wall+1,r=2, $fn=100);
        translate([mount_hole_2_x,mount_hole_2_y,1]) #cylinder(h=wall+1,r=pilot_hole_width, $fn=100);
    }
	mount_hole_3_x = lcd_pcb_pos_x + 2.4;
	mount_hole_3_y = lcd_pcb_pos_y + 34.6;
    difference ()
    {
        translate([mount_hole_3_x,mount_hole_3_y,0]) cylinder(h=wall+1,r=2, $fn=100);
        translate([mount_hole_3_x,mount_hole_3_y,1]) #cylinder(h=wall+1,r=pilot_hole_width, $fn=100);
    }
	mount_hole_4_x = lcd_pcb_pos_x + 50.7;
	mount_hole_4_y = lcd_pcb_pos_y + 34.6;
    difference ()
    {
        translate([mount_hole_4_x,mount_hole_4_y,0]) cylinder(h=wall+1,r=2, $fn=100);
        translate([mount_hole_4_x,mount_hole_4_y,1]) #cylinder(h=wall+1,r=pilot_hole_width, $fn=100);
    }
}
module base_simple()
{
    //main shell
    difference ()
    {
       cube([length,width,height_lower]);
       translate([2,2,2]) cube([length-wall*2,width-wall*2,height_lower]);
       translate([1,1,height_lower-1]) cube([length-wall,width-wall,5]);

       //hanger 1
       translate([15+wall,width/2,-1]) cylinder(r=4,h=6);
       translate([15+wall,(width/2)+5,-1]) cylinder(r=2.7,h=6);

       //hanger 2
       translate([length-15,width/2,-1]) cylinder(r=4,h=6);
       translate([length-15,(width/2)+5,-1]) cylinder(r=2.7,h=6);

       //power supply cutout 
       //4.4mm above face (3mm for mounting height+1.6mm for pcb width
       translate([length-3,wall+3.5,wall+pcb_stand_h+pcb_thickness]) #cube([6,10,12]);//10x10mm hole

	   //Buttons
       translate([89,button_1_pos_y,button_height]) #cube([4.2,4.2,4.2]);
       translate([89,button_2_pos_y,button_height]) #cube([4.2,4.2,4.2]);
       translate([89,button_3_pos_y,button_height]) #cube([4.2,4.2,4.2]);
       translate([89,button_4_pos_y,button_height]) #cube([4.2,4.2,4.2]);
    }

    //Pin Holes for connecting upper and lower halfs
    difference()
    {
        translate([3,3,0]) cylinder (h=7+4,r=3, $fn=100);
        translate([3,3,1]) #cylinder (h=7+4,r=pin_hole_width, $fn=100);
    }
    difference()
    {
        translate([length-5,2,0]) cube([3,3,11]);
        translate([length-3,3,1]) #cylinder (h=12,r=pin_hole_width, $fn=100);
        
    }
    difference()
    {
        translate([length-3,width-3,0]) cylinder (h=7+4,r=3, $fn=100);
        translate([length-3,width-3,1]) #cylinder (h=12,r=pin_hole_width, $fn=100);
    }
    difference()
    {
        translate([3,width-3,0]) cylinder (h=11,r=3, $fn=100);
        translate([3,width-3,1]) #cylinder (h=12,r=pin_hole_width, $fn=100);
    }

    //pcb mounting holes
    difference()
    {
        translate([wall+10,wall+10,wall]) cylinder(h=pcb_stand_h,r=3,$fn=20);
        translate([wall+10,wall+10,wall-1]) #cylinder(h=5,r=pilot_hole_width,$fn=20);
    }
    difference()
    {
        translate([wall+70,wall+6,wall]) cylinder(h=pcb_stand_h,r=3,$fn=20);
        translate([wall+70,wall+6,wall-1]) #cylinder(h=5,r=pilot_hole_width,$fn=20);
    }
    difference()
    {
        translate([wall+43,wall+64,wall]) cylinder(h=pcb_stand_h,r=3,$fn=20);
        translate([wall+43,wall+64,wall-1]) #cylinder(h=5,r=pilot_hole_width,$fn=20);
    }
    difference()
    {
        translate([wall+74,wall+64,wall]) cylinder(h=pcb_stand_h,r=3,$fn=20);
        translate([wall+74,wall+64,wall-1]) #cylinder(h=5,r=pilot_hole_width,$fn=20);
    }    
}
module view_all()
{
    union()
    {
        mirror([0,1,0]) translate([10,-width,0]) upper_simple();
        mirror([0,0,0]) translate([10,width+2,0]) base_simple();
    }
}
module view()
{
    if(part=="lower") base_simple();
    if(part=="upper") upper_simple();
    if(part=="all") view_all();
}
//inner height is 25mm, 9mm in the lid, 16mm in the bottom.
//power jack location fixed, upper modified for new location of power jack
//added wall mounts (you can keep them both or remove the ugly one.
//all pcb/lcd/DTH mounting holes are 1mm DIAMETER, we should be able to use all the same screws for everything. the only iffy one is the DHT. so worst case is we need to buy 1 screw. 

