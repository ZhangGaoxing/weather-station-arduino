$fn=200;

case1();

module case2(){
    difference(){
        cube([89.7,69.7,1.5]);
        
        translate([20,2,0])cube([9,4,1.5]);
    }
}

module case1(){
    difference(){
        minkowski()
        {
          cube([90,70,31.5]);
          cylinder(r=2,h=33);
        }
        
        translate([0,0,1.5])cube([90,70,66]);
        
        translate([7.5,-2.5,0])lines1();
        translate([7.5,69,0])lines1();
        
        translate([-2.5,6.5,0])lines2();
        translate([89,6.5,0])lines2();
    }
}

module lines1(){
    union(){
        translate([0,0,47])cube([3,5,7]);
        translate([6,0,47])cube([3,5,7]);
        translate([12,0,47])cube([3,5,7]);
        translate([18,0,47])cube([3,5,7]);
        translate([24,0,47])cube([3,5,7]);
        translate([30,0,47])cube([3,5,7]);
        translate([36,0,47])cube([3,5,7]);
        translate([42,0,47])cube([3,5,7]);
        translate([48,0,47])cube([3,5,7]);
        translate([54,0,47])cube([3,5,7]);
        translate([60,0,47])cube([3,5,7]);
        translate([66,0,47])cube([3,5,7]);
        translate([72,0,47])cube([3,5,7]);
    }
}

module lines2(){
    union(){
        translate([0,0,47])cube([5,3,7]);
        translate([0,6,47])cube([5,3,7]);
        translate([0,12,47])cube([5,3,7]);
        translate([0,18,47])cube([5,3,7]);
        translate([0,24,47])cube([5,3,7]);
        translate([0,30,47])cube([5,3,7]);
        translate([0,36,47])cube([5,3,7]);
        translate([0,42,47])cube([5,3,7]);
        translate([0,48,47])cube([5,3,7]);
        translate([0,54,47])cube([5,3,7]);
    }
    
}