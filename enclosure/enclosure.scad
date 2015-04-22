//
// Box for L/C meter
// LC 100S
//
// by Egil Kvaleberg
//
// mod by Michael Witt 6/27/2014 for LC-100A
// add 4th button, 
// adjusted power, lead and port locations  
// slightly increased LCD cutout, 
// place pieces in print layout
// clarify documentation
//
// further mod by Egil:
// fine tuned, moved func-button
// added proper func-button
//
//  note on building:
//  make sure support_w is actually built. depending on your settings, 
//  you may have to increase it slightly
//

//  part is any of:
//  "upper" "lower" "button" "all"
//

part = "lower"; 
has4buttons = true; // true if 4 buttons in a row
has_support = true; // true if support around LCD (to be removed afterwards)

//*****************************************************************************
// Adjustables

support_w = 0.50; // one line wide

core_x = 88.0; // core size, pplus margin
core_y = 61.0;
core_z = 20.0; // lower half
lip_dz = 0.7; 
corner_r = 2;
wall = 2.0;
floor = 2.0;
chamfer1 = 0.8; // chamfer around edge

dht_x = 70;
dht_y = 24; //
dht_z = 8.0; 
dht_h = 4.0; // max support height
dht_dy = 20; //24.2
dht_dx = 15.1; // 71.1

lcd_x = 19.7;
lcd_y = 16; //
lcd_z = 8.0; 
lcd_h = 4.0; // max support height
lcd_dy = 34; //24.2
lcd_dx = 43.6; // 71.1

mnt_dy = 45.8; 
mnt_lcd_dy = 31.0; 
mnt_lcd_dz = 3.0; 
mnt_dx = 75.0; // was 75.3
mnt_d = 3.2; 
mnt_head = 5.5; // screw head

button_x = 8.51;
button_d = 7.62; // hole for pushbutton, diameter 
buttonFunc_d = button_d; // function key hole, diameter

button0_y = 15.5;
enter_y   = button0_y;
back_y    = button0_y+(button_d+3)*1;
up_y      = button0_y+(button_d+3)*2;
down_y    = button0_y+(button_d+3)*3;

switch_d1  =  7.0; // on off height
switch_d2  = 7.0; // on off width
switch_dy  = -18.0; // on off left/right
switch_dz  = 5.0; // on off up/down
switch_frame  = 2.5; // recess around switch

fudge = 0.05;
$fs = 0.1;

//*****************************************************************************
// Derived


//*****************************************************************************
// View and printing

view(); 

//*****************************************************************************
// 

module rcyl(rr, hh,chamfer)
{
	union () {
		if (chamfer > 0) {
			cylinder(r1=rr-chamfer, r2=rr, h=chamfer); // bottom first: 45 deg slope max
			//cylinder(r1=0, r2=rr, h=chamfer); // upside down: no limitation
		}
		// main
		translate([0, 0, chamfer]) cylinder(r=rr, h=hh-chamfer);
	}
}

module rcube(dx, dy, dz, chamfer)
{
	hull() {
		translate([corner_r, corner_r, 0]) rcyl(corner_r, dz, chamfer);
		translate([dx - corner_r, corner_r, 0]) rcyl(corner_r, dz, chamfer);
		translate([corner_r, dy - corner_r, 0]) rcyl(corner_r, dz, chamfer);
		translate([dx - corner_r, dy - corner_r, 0]) rcyl(corner_r, dz, chamfer);
	}
}


module core()
{
	union () {
		translate([-core_x/2, -core_y/2, floor]) rcube(core_x, core_y, core_z+fudge, chamfer1);
		translate([-(core_x/2+wall/2), -(core_y/2+wall/2), floor+core_z]) rcube(core_x+wall, core_y+wall, lip_dz+fudge, 0);
	}
}

module plugs() // for lower part
{
	//swtch and frame
	translate([core_x/2-fudge, switch_dy-switch_d2/2, switch_dz-switch_d1/2]) 
		cube(size = [wall+2*fudge, switch_d2, switch_d1]);
	translate([core_x/2+wall/2, switch_dy-(switch_d2/2+switch_frame), switch_dz-(switch_d1/2+switch_frame)]) 
		cube(size = [wall/2+fudge, switch_d2+2*switch_frame, switch_d1+2*switch_frame]);

	// mounting holes
	translate([mnt_dx/2, mnt_dy/2, -fudge]) cylinder(r2=mnt_d/2, r1=mnt_head/2, h=floor+2*fudge);
	translate([-mnt_dx/2, mnt_dy/2, -fudge]) cylinder(r2=mnt_d/2, r1=mnt_head/2, h=floor+2*fudge);
	translate([mnt_dx/2, -mnt_dy/2, -fudge]) cylinder(r2=mnt_d/2, r1=mnt_head/2, h=floor+2*fudge);
	translate([-mnt_dx/2, -mnt_dy/2, -fudge]) cylinder(r2=mnt_d/2, r1=mnt_head/2, h=floor+2*fudge);

}


module pattern_sub() // pattern on rear wall
{
//	intersection () {
//		for ( y = [0 : 1.5 : core_y] ) {
//			translate([-core_x/2, y - core_y/2, 0]) cube([core_x, 1.5-0.5, 0.2]);
//		}
		for ( x = [0 : 1.5 : core_x] ) {
			translate([x - core_x/2, - core_y/2, 0]) cube([1.5-0.5, core_y, 0.2]);
		}
//	}
}

module shell()
{

	difference () {
		union () {
			difference () {
				translate([-wall-core_x/2, -wall-core_y/2, 0]) 
					rcube(core_x+2*wall, core_y+2*wall, core_z+floor+lip_dz, chamfer1);
				pattern_sub();
			}
		}
		union () {
			plugs();
		}
	}
}

module lower()
{
	difference () {
		union () {
		    	difference () {
				shell();
				core();
		    	}	
	    	}
	}
}

module upper()
{
	union () {
		difference () {
			union () {
				// lower lid	
				translate([wall/2, wall/2, 0]) 
					rcube(core_x+wall, core_y+wall, lip_dz, 0);
				translate([0, 0, lip_dz]) 
					rcube(core_x+2*wall, core_y+2*wall, floor-lip_dz, wall/2);

				// lcd outer casing
				translate([0, 0, floor]) 
					rcube(core_x+2*wall, core_y+2*wall, lcd_z, 0); 
			}
			union () {
				// lcd pcb
				translate([wall, wall, -fudge]) 
					rcube(core_x, core_y, lcd_z+fudge, 0);

				// lcd opening
				translate([lcd_x, lcd_y, lcd_z-fudge]) 
					cube(size = [lcd_dx, lcd_dy, floor+2*fudge]);

				// dht22 opening
				translate([dht_x, dht_y, dht_z-fudge]) 
					cube(size = [dht_dx, dht_dy, floor+2*fudge]);

				// buttons
				translate([button_x, enter_y, lcd_z-fudge]) cylinder(r=button_d/2, h=floor+2*fudge);
				translate([button_x, back_y, lcd_z-fudge]) cylinder(r=button_d/2, h=floor+2*fudge);
				translate([button_x, up_y, lcd_z-fudge]) cylinder(r=button_d/2, h=floor+2*fudge);
				translate([button_x, down_y, lcd_z-fudge]) cylinder(r=buttonFunc_d/2, h=floor+2*fudge); 

				// mounting holes
				translate([mnt_dx/2, -(mnt_lcd_dy-mnt_dy/2), lcd_z-fudge]) cylinder(r1=mnt_d/2, r2=mnt_head/2,  h=floor+2*fudge);
				translate([-mnt_dx/2, -(mnt_lcd_dy-mnt_dy/2), lcd_z-fudge]) cylinder(r1=mnt_d/2, r2=mnt_head/2,  h=floor+2*fudge);
				translate([mnt_dx/2, mnt_dy/2, lcd_z-fudge]) cylinder(r1=mnt_d/2, r2=mnt_head/2,  h=floor+2*fudge);
				translate([-mnt_dx/2, mnt_dy/2, lcd_z-fudge]) cylinder(r1=mnt_d/2 ,r2=mnt_head/2,  h=floor+2*fudge);
			}
		}
	}
}

module fbutton()
{
	fb_square_d = 7.2; // square cutout side 
	fb_square_h = 4.0; // square cutout height
	fb_square_w = 0.8; // square cutout wall
	fb_internal_h = 7.0; // internal height 
	fb_external_h = 5.0; 
	fb_external_d = 6.0;

	difference() {
		translate([-fb_square_d/2-fb_square_w, -fb_square_d/2-fb_square_w, 0]) 
			cube([fb_square_d+2*fb_square_w, fb_square_d+2*fb_square_w, fb_square_h+fb_internal_h]);
		union() {
			translate([-fb_square_d/2, -fb_square_d/2, -fudge]) 
				cube([fb_square_d, fb_square_d, fudge+fb_square_h-1]);
			cylinder(r = 5.0/2, h=fudge+fb_square_h);
		}

	}
	translate([0, 0, fb_square_h+fb_internal_h]) {
		cylinder(r = fb_external_d/2, h = fb_external_h);
	}

}

module view()
{

	if (part == "lower" || part == "all") lower();

	if (part == "upper" || part == "all") {
		//translate([0, 0, core_z+floor+10])	
	 	translate([0,0,0])
		upper();
	}
	if (part == "button" || part == "all") {
    	translate([55,0,0])	
		fbutton();
	}
}

//*****************************************************************************
