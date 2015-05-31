
part = "upper"; 
has4buttons = true; // true if 4 buttons in a row
has_support = true; // true if support around LCD (to be removed afterwards)

//*****************************************************************************
// Adjustables
core_x = 88.0; // core size, pplus margin
core_y = 61.0;
core_z = 20.0; // lower half
lip_dz = 0.7; 
corner_r = 2;
wall = 2.0;
floor = 2.0;
chamfer1 = 0.8; // chamfer around edge

button_x = 8.51;
button_d = 7.62; // hole for pushbutton, diameter 
buttonFunc_d = button_d; // function key hole, diameter

button0_y = 15.5;
enter_y   = button0_y;
back_y    = button0_y+(button_d+3)*1;
up_y      = button0_y+(button_d+3)*2;
down_y    = button0_y+(button_d+3)*3;

fudge = 0.05;
$fs = 0.1;

//*****************************************************************************
// Derived


//*****************************************************************************
// View and printing

view(); 

//*****************************************************************************
// 

module rcyl(rr, hh, chamfer)
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

mnt_dy = 45.8;
mnt_dx = 75.0;
pcb_mnt_d_inner = 3.2;
pcb_mnt_d_outer = 6;
pcb_mnt_height = floor + 2;
pcb_board_height = 1.6;

module pcbMountingHoles()
{
	// mounting holes
	difference() {
	translate([19.42+wall, 54.65+wall, 0]) cylinder(r2=pcb_mnt_d_outer/2, r1=pcb_mnt_d_outer/2, h=pcb_mnt_height);
	translate([19.42+wall, 54.65+wall, 0]) cylinder(r2=pcb_mnt_d_inner/2, r1=pcb_mnt_d_inner/2, h=pcb_mnt_height);
	}

	difference() {
	translate([15.6+wall, 6.35+wall, 0]) cylinder(r2=pcb_mnt_d_outer/2, r1=pcb_mnt_d_outer/2, h=pcb_mnt_height);
	translate([15.61+wall, 6.35+wall, 0]) cylinder(r2=pcb_mnt_d_inner/2, r1=pcb_mnt_d_inner/2, h=pcb_mnt_height);
	}

	difference() {
	translate([79.1+wall, 54.6+wall, 0]) cylinder(r2=pcb_mnt_d_outer/2, r1=pcb_mnt_d_outer/2, h=pcb_mnt_height);
	translate([79.1+wall, 54.6+wall, 0]) cylinder(r2=pcb_mnt_d_inner/2, r1=pcb_mnt_d_inner/2, h=pcb_mnt_height);
	}

	difference() {
	translate([79.1+wall, 6.35+wall, 0]) cylinder(r2=pcb_mnt_d_outer/2, r1=pcb_mnt_d_outer/2, h=pcb_mnt_height);
	translate([79.1+wall, 6.35+wall, 0]) cylinder(r2=pcb_mnt_d_inner/2, r1=pcb_mnt_d_inner/2, h=pcb_mnt_height);
	}
}

enclosure_conn_h = 5;
enclosure_conn_z = upper_z+fudge-enclosure_conn_h;
enclosure_mnt_r_inner = 1;
enclosure_mnt_r_outer = 2;

enclosure_conn_x0 = wall+enclosure_mnt_r_inner;
enclosure_conn_y0 = wall+enclosure_mnt_r_inner;
enclosure_conn_x = core_x+wall-enclosure_mnt_r_inner;
enclosure_conn_y = core_y+wall-enclosure_mnt_r_inner;

module enclosureConnectH()
{
	difference () {
		union () {
		translate([enclosure_conn_x0, enclosure_conn_y0, enclosure_conn_z])
            cylinder(r1=enclosure_mnt_r_outer, r2=enclosure_mnt_r_outer, h=enclosure_conn_h);
		translate([enclosure_conn_x, enclosure_conn_y0, enclosure_conn_z])
            cylinder(r1=enclosure_mnt_r_outer, r2=enclosure_mnt_r_outer, h=enclosure_conn_h);
		translate([enclosure_conn_x0, enclosure_conn_y, enclosure_conn_z])
            cylinder(r1=enclosure_mnt_r_outer, r2=enclosure_mnt_r_outer, h=enclosure_conn_h);
		translate([enclosure_conn_x, enclosure_conn_y, enclosure_conn_z])
            cylinder(r1=enclosure_mnt_r_outer, r2=enclosure_mnt_r_outer, h=enclosure_conn_h);     
		}
		union () {
		translate([enclosure_conn_x0, enclosure_conn_y0, enclosure_conn_z])
            cylinder(r1=enclosure_mnt_r_inner, r2=enclosure_mnt_r_inner, h=enclosure_conn_h);
		translate([enclosure_conn_x, enclosure_conn_y0, enclosure_conn_z])
            cylinder(r1=enclosure_mnt_r_inner, r2=enclosure_mnt_r_inner, h=enclosure_conn_h);
		translate([enclosure_conn_x0, enclosure_conn_y, enclosure_conn_z])
            cylinder(r1=enclosure_mnt_r_inner, r2=enclosure_mnt_r_inner, h=enclosure_conn_h);
		translate([enclosure_conn_x, enclosure_conn_y, enclosure_conn_z])
            cylinder(r1=enclosure_mnt_r_inner, r2=enclosure_mnt_r_inner, h=enclosure_conn_h);
		}
	}
}

module enclosureConnectL()
{
    union () {
    translate([enclosure_conn_x0, enclosure_conn_y0, enclosure_conn_z])
        cylinder(r1=enclosure_mnt_r_inner, r2=enclosure_mnt_r_inner, h=enclosure_conn_h);
    translate([enclosure_conn_x, enclosure_conn_y0, enclosure_conn_z])
        cylinder(r1=enclosure_mnt_r_inner, r2=enclosure_mnt_r_inner, h=enclosure_conn_h);
    translate([enclosure_conn_x0, enclosure_conn_y, enclosure_conn_z])
        cylinder(r1=enclosure_mnt_r_inner, r2=enclosure_mnt_r_inner, h=enclosure_conn_h);
    translate([enclosure_conn_x, enclosure_conn_y, enclosure_conn_z])
        cylinder(r1=enclosure_mnt_r_inner, r2=enclosure_mnt_r_inner, h=enclosure_conn_h);     
    }
}

switch_d1  = 9;
switch_d2  = 9;
switch_dz  = floor + pcb_mnt_height;
switch_dy  = 48.9;
switch_frame  = 2.5;

module plugs()
{
	//swtch and frame
	translate([0, switch_dy, switch_dz]) 
		rcube(wall+2*fudge, switch_d2, switch_d1, chamfer1);
}

module shell()
{
	difference () {
        translate([0, 0, 0]) 
            rcube(core_x+2*wall, core_y+2*wall, core_z+floor+lip_dz, chamfer1);
		union () {
			plugs();
		}
    }
}

module core()
{
	union () {
		translate([wall, wall, floor]) rcube(core_x, core_y, core_z+fudge, chamfer1);
		translate([wall/2, wall/2, floor+core_z]) rcube(core_x+wall+fudge, core_y+wall+fudge, lip_dz+fudge, 0);
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
			pcbMountingHoles();
	    	}
        enclosureConnectL();
	}
}

upper_z = 5; //Upper Layer Height

lcd_x = 19.7;
lcd_y = 16;
lcd_z = upper_z; 

lcd_h = 4.0;
lcd_dy = 34;
lcd_dx = 43.6;

lcd_mnt_r_inner = 1;
lcd_mnt_r_outer = 2;
lcd_mnt_h = 1.5;
lcd_mnt_z = upper_z+fudge-lcd_mnt_h;

module lcdMountingHoles()
{
	difference () {
		union () {
		translate([lcd_x-2.3, lcd_y, lcd_mnt_z])
            cylinder(r1=lcd_mnt_r_outer, r2=lcd_mnt_r_outer, h=lcd_mnt_h);
		translate([lcd_x-2.3, lcd_y+32.2, lcd_mnt_z])
            cylinder(r1=lcd_mnt_r_outer, r2=lcd_mnt_r_outer,  h=lcd_mnt_h);
		translate([lcd_x+46, lcd_y, lcd_mnt_z])
            cylinder(r1=lcd_mnt_r_outer, r2=lcd_mnt_r_outer,  h=lcd_mnt_h);
		translate([lcd_x+46, lcd_y+32.2, lcd_mnt_z])
            cylinder(r1=lcd_mnt_r_outer ,r2=lcd_mnt_r_outer,  h=lcd_mnt_h);
		}
		union () {
		translate([lcd_x-2.3, lcd_y, lcd_mnt_z])
            cylinder(r1=lcd_mnt_r_inner, r2=lcd_mnt_r_inner, h=lcd_mnt_h);
		translate([lcd_x-2.3, lcd_y+32.2, lcd_mnt_z])
            cylinder(r1=lcd_mnt_r_inner, r2=lcd_mnt_r_inner, h=lcd_mnt_h);
		translate([lcd_x+46, lcd_y, lcd_mnt_z])
            cylinder(r1=lcd_mnt_r_inner, r2=lcd_mnt_r_inner, h=lcd_mnt_h);
		translate([lcd_x+46, lcd_y+32.2, lcd_mnt_z])
            cylinder(r1=lcd_mnt_r_inner, r2=lcd_mnt_r_inner, h=lcd_mnt_h);
		}
	}
}

dht_x = 70;
dht_y = 24;
dht_z = upper_z; 
dht_h = 4.0;
dht_dy = 20;
dht_dx = 15.1;

dht_mnt_h = 1.5;
dht_mnt_d_inner = 1;
dht_mnt_d_outer = 3;
dht_mnt_x = dht_x + (dht_dx/2);
dht_mnt_y = dht_y - (dht_mnt_d_outer/2);
dht_mnt_z = upper_z+fudge-dht_mnt_h;

module dhtMountingHole()
{
	difference () {
		translate([dht_mnt_x, dht_mnt_y, dht_mnt_z])
            cylinder(r1=dht_mnt_d_outer/2, r2=dht_mnt_d_outer/2, h=dht_mnt_h);
		translate([dht_mnt_x, dht_mnt_y, dht_mnt_z])
            cylinder(r1=dht_mnt_d_inner/2, r2=dht_mnt_d_inner/4, h=dht_mnt_h);
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
				translate([wall, wall, 0]) 
					rcube(core_x, core_y, lcd_z, 0);

				// lcd opening
				translate([lcd_x, lcd_y, lcd_z]) 
					cube(size = [lcd_dx, lcd_dy, floor+2*fudge]);

				// dht22 opening
				translate([dht_x, dht_y, dht_z]) 
					cube(size = [dht_dx, dht_dy, floor+2*fudge]);

				// buttons
				translate([button_x, enter_y, lcd_z-fudge]) cylinder(r=button_d/2, h=floor+2*fudge);
				translate([button_x, back_y, lcd_z-fudge]) cylinder(r=button_d/2, h=floor+2*fudge);
				translate([button_x, up_y, lcd_z-fudge]) cylinder(r=button_d/2, h=floor+2*fudge);
				translate([button_x, down_y, lcd_z-fudge]) cylinder(r=buttonFunc_d/2, h=floor+2*fudge); 
			}
		}
		lcdMountingHoles();
		dhtMountingHole();
        enclosureConnectH();
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
