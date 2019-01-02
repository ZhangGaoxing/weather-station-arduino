$fn=50;

case1();

module case2(){
    difference(){
        cube([89.7,69.7,1.5]);
    
        translate([4,23,0])union(){
            translate([7,0,0])cube([71,24,1.5]);
            cube([7,7,1.5]);
            translate([64.5,24,0])cube([3.5,4.5,1.5]);
        }
    }
}

module case1(){
    difference(){
        minkowski()
        {
          cube([90,70,31.5]);
          cylinder(r=1.5,h=31.5);
        }
        
        translate([0,0,1.5])cube([90,70,63]);
        
        translate([7.5,-2.5,0])lines1();
        translate([7.5,69,0])lines1();
        
        translate([-2.5,6.5,0])lines2();
        translate([89,6.5,0])lines2();
    }
}

module lines1(){
    union(){
        translate([0,0,10.5])cube([3,5,42]);
        translate([6,0,10.5])cube([3,5,42]);
        translate([12,0,10.5])cube([3,5,42]);
        translate([18,0,10.5])cube([3,5,42]);
        translate([24,0,10.5])cube([3,5,42]);
        translate([30,0,10.5])cube([3,5,42]);
        translate([36,0,10.5])cube([3,5,42]);
        translate([42,0,10.5])cube([3,5,42]);
        translate([48,0,10.5])cube([3,5,42]);
        translate([54,0,10.5])cube([3,5,42]);
        translate([60,0,10.5])cube([3,5,42]);
        translate([66,0,10.5])cube([3,5,42]);
        translate([72,0,10.5])cube([3,5,42]);
    }
}

module lines2(){
    union(){
        translate([0,0,10.5])cube([5,3,42]);
        translate([0,6,10.5])cube([5,3,42]);
        translate([0,12,10.5])cube([5,3,42]);
        translate([0,18,10.5])cube([5,3,42]);
        translate([0,24,10.5])cube([5,3,42]);
        translate([0,30,10.5])cube([5,3,42]);
        translate([0,36,10.5])cube([5,3,42]);
        translate([0,42,10.5])cube([5,3,42]);
        translate([0,48,10.5])cube([5,3,42]);
        translate([0,54,10.5])cube([5,3,42]);
    }
    
}