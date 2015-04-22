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

part = "upper"; 
has4buttons = true; // true if 4 buttons in a row
has_support = true; // true if support around LCD (to be removed afterwards)

//*****************************************************************************
// Adjustables

support_w = 0.50; // one line wide

core_x = 81.0 + 2.0; // core size, pplus margin
core_y = 51.0 + 2.0;
core_z = 20.0; // lower half
lip_dz = 0.7; 
corner_r = 2;
wall = 2.0;
floor = 2.0;
chamfer1 = 0.8; // chamfer around edge

lcd_y = 36.0 + 2.0; //
lcd_z = 8.0; 
lcd_h = 4.0; // max support height
lcd_dy = 25.0; //24.2
lcd_dx = 71.9; // 71.1

mnt_dy = 45.8; 
mnt_lcd_dy = 31.0; 
mnt_lcd_dz = 3.0; 
mnt_dx = 75.0; // was 75.3
mnt_d = 3.2; 
mnt_head = 5.5; // screw head


button0_x = -26.3;
buttonHIC_x = button0_x+2.56*4; // NOTE: some models may not have this button
buttonHIL_x = button0_x+2.56*8;
buttonLC_x = button0_x+2.56*12;
buttonFunc_x = button0_x+2.56*22;

button_y = -17;
button_d = 8.0; // hole for pushbutton, diameter 
buttonFunc_d = button_d; // function key hole, diameter

switch_d1  =  4.0; // on off height
switch_d2  = 7.0; // on off width
switch_dy  = 5.0; // on off left/right
switch_dz  = 12.5; // on off up/down
switch_frame  = 2.5; // recess around switch

jack_d  = 8.0; // round power jack diameter 
jack_dz  =  16.3; // round power jack up down 
jack_dx  = 14.0; // round power jack left/right

wire_d  = 4.0; //probe diameters;
wire_dz  = 12.3; //probe up/down
wire_dx  = 15.0; //probe right/left

usb_z  = 4.8; //usb height
usb_x  = 8.2; //usb width
usb_dz  = 11.4; //usb up/down
usb_dx  = 28.8-5.6; //usb right/left
usb_frame  = 2.6; // recess around plug

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
	translate([jack_dx, core_y/2+wall+fudge, jack_dz]) {
 		rotate([90, 0, 0]) cylinder(r=jack_d/2, h=wall+2*fudge);
	}
	translate([wire_dx, -core_y/2+fudge, wire_dz]) {
 		rotate([90, 0, 0]) cylinder(r=wire_d/2, h=wall+2*fudge);
	}
	translate([wire_dx+5.12, -core_y/2+fudge, wire_dz]) {
 		rotate([90, 0, 0]) cylinder(r=wire_d/2, h=wall+2*fudge);
	}

	// usb and frame
	translate([usb_dx, core_y/2+wall+fudge-10, usb_dz-usb_z/2]) {	
		cube(size = [ usb_x, 20+wall+2*fudge, usb_z]);
	}
	translate([usb_dx - usb_frame, core_y/2+wall/2, usb_dz - (usb_z/2 + usb_frame)]) {
		cube(size = [ usb_x + 2*usb_frame, wall/2+fudge, usb_z + 2*usb_frame]);
	}

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

module lcd_mnt_col(dx, dy)
{
	// lcd mounting column
/* impossible without support...
	difference () {
		translate([dx, dy, lcd_z-mnt_lcd_dz]) cylinder(r=mnt_d/2+wall, h=mnt_lcd_dz);
		translate([dx, dy, -fudge]) cylinder(r=mnt_d/2, h=lcd_z+floor+2*fudge);
	}
*/
}

module lcd_mnt_cols()
{
	// lcd mounting columns
	union () {
		lcd_mnt_col(mnt_dx/2, mnt_lcd_dy-mnt_dy/2);
		lcd_mnt_col(-mnt_dx/2, mnt_lcd_dy-mnt_dy/2);
		lcd_mnt_col(mnt_dx/2, -mnt_dy/2);
		lcd_mnt_col(-mnt_dx/2, -mnt_dy/2);
	}
}

module xtabs(dx, dy, dz) // for support
{
	tabs = 5; 
	width = 5.0;
	for (i = [0 : tabs-1]) {
		translate([(1+i*2) * dx/(2*tabs+1), 0, 0]) {
			cube([dx/(2*tabs+1), dy, dz]);
			translate([0, -width/2, 0]) cube([dx/(2*tabs+1), width, support_w]);
		}
	}
}


module ytabs(dx, dy, dz) 
{
	tabs = 2; 
	width = 5.0;
	for (i = [0 : tabs-1]) {
		translate([0, (1+i*2) * dy/(2*tabs+1), 0]) {
			cube([dx, dy/(2*tabs+1), dz]);
			translate([-width/2, 0, 0]) cube([width, dy/(2*tabs+1), support_w]);
		}
	}
}

module lcd_support()
{
	union () {

		// tabs around frame as bridges, to be cut away
		if (has_support) {
			translate([lcd_dx/2, -(lcd_y-core_y/2), 0]) ytabs(support_w, lcd_y, lcd_z);
			translate([-(lcd_dx/2+support_w), -(lcd_y-core_y/2), 0]) ytabs(support_w, lcd_y, lcd_z);
			translate([-core_x/2, (lcd_y-lcd_dy)/2-(lcd_y-core_y/2)-support_w, 0]) xtabs(core_x, support_w, lcd_z);
			translate([-core_x/2, (lcd_y+lcd_dy)/2-(lcd_y-core_y/2), 0]) xtabs(core_x, support_w, lcd_z);
		}

		// 4 narrow tracks around frame 
		translate([lcd_dx/2, -(lcd_y-core_y/2), lcd_z-lcd_h*0.7]) cube(size = [support_w, lcd_y, lcd_h*0.7]);
		translate([-(lcd_dx/2+support_w), -(lcd_y-core_y/2), lcd_z-lcd_h*0.7]) cube(size = [support_w, lcd_y, lcd_h*0.7]);
		translate([-core_x/2, (lcd_y-lcd_dy)/2-(lcd_y-core_y/2)-support_w, lcd_z-lcd_h*0.7]) cube(size = [core_x, support_w, lcd_h*0.7]);
		translate([-core_x/2, (lcd_y+lcd_dy)/2-(lcd_y-core_y/2), lcd_z-lcd_h*0.7]) cube(size = [core_x, support_w, lcd_h*0.7]);

		// wall thick tracks around frame
		translate([lcd_dx/2, (lcd_y-lcd_dy)/2-(lcd_y-core_y/2), lcd_z-lcd_h*0.7]) cube(size = [wall, lcd_dy, lcd_h*0.7]);
		translate([-(lcd_dx/2+wall), (lcd_y-lcd_dy)/2-(lcd_y-core_y/2), lcd_z-lcd_h*0.7]) cube(size = [wall, lcd_dy, lcd_h*0.7]);
		translate([-lcd_dx/2, (lcd_y-lcd_dy)/2-(lcd_y-core_y/2)-wall, lcd_z-lcd_h*0.7]) cube(size = [lcd_dx, wall, lcd_h*0.7]);
		translate([-lcd_dx/2, (lcd_y+lcd_dy)/2-(lcd_y-core_y/2), lcd_z-lcd_h*0.7]) cube(size = [lcd_dx, wall, lcd_h*0.7]);



	}
}

module upper()
{
	union () {
		difference () {
			union () {
				// lower lid	
				translate([-wall/2-core_x/2, -wall/2-core_y/2, 0]) 
					rcube(core_x+wall, core_y+wall, lip_dz, 0);
				translate([-wall-core_x/2, -wall-core_y/2, lip_dz]) 
					rcube(core_x+2*wall, core_y+2*wall, floor-lip_dz, wall/2);

				// lcd outer casing
				translate([-wall-core_x/2, -wall-(lcd_y-core_y/2), floor]) 
					rcube(core_x+2*wall, lcd_y+2*wall, lcd_z, 0); 
			}
			union () {
				// lcd pcb
				translate([-core_x/2, -(lcd_y-core_y/2), -fudge]) 
					rcube(core_x, lcd_y, lcd_z+fudge, 0);

				// lcd opening
				translate([-lcd_dx/2, (lcd_y-lcd_dy)/2-(lcd_y-core_y/2), lcd_z-fudge]) 
					cube(size = [lcd_dx, lcd_dy, floor+2*fudge]);

				// buttons
				if (has4buttons) {
					translate([buttonHIC_x, button_y, -fudge]) cylinder(r=button_d/2, h=floor+2*fudge);
				}
				translate([button0_x, button_y, -fudge]) cylinder(r=button_d/2, h=floor+2*fudge);
				translate([buttonHIL_x, button_y, -fudge]) cylinder(r=button_d/2, h=floor+2*fudge);
				translate([buttonLC_x, button_y, -fudge]) cylinder(r=button_d/2, h=floor+2*fudge);
				translate([buttonFunc_x, button_y-0.5, -fudge]) cylinder(r=buttonFunc_d/2, h=floor+2*fudge); 
				// access to screws
				translate([wire_dx, button_y-4, -fudge]) cylinder(r=1.6, h=floor+2*fudge);
				translate([wire_dx+5.12, button_y-4, -fudge]) cylinder(r=1.6, h=floor+2*fudge);

				// mounting holes
				translate([mnt_dx/2, -(mnt_lcd_dy-mnt_dy/2), lcd_z-fudge]) cylinder(r1=mnt_d/2, r2=mnt_head/2,  h=floor+2*fudge);
				translate([-mnt_dx/2, -(mnt_lcd_dy-mnt_dy/2), lcd_z-fudge]) cylinder(r1=mnt_d/2, r2=mnt_head/2,  h=floor+2*fudge);
				translate([mnt_dx/2, mnt_dy/2, lcd_z-fudge]) cylinder(r1=mnt_d/2, r2=mnt_head/2,  h=floor+2*fudge);
				translate([-mnt_dx/2, mnt_dy/2, lcd_z-fudge]) cylinder(r1=mnt_d/2 ,r2=mnt_head/2,  h=floor+2*fudge);
			}
		}
		lcd_support();
		lcd_mnt_cols();
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
	 	translate([0,65,0])
		upper();
	}
	if (part == "button" || part == "all") {
    	translate([55,0,0])	
		fbutton();
	}
}

//*****************************************************************************
