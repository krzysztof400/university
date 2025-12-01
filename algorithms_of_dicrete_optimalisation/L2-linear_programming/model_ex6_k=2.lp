\* Camera_placement_k=2 *\
Minimize
Number_of_cameras: Camera_(0,_1) + Camera_(0,_2) + Camera_(0,_3)
 + Camera_(1,_0) + Camera_(1,_1) + Camera_(1,_2) + Camera_(1,_3)
 + Camera_(1,_4) + Camera_(2,_0) + Camera_(2,_1) + Camera_(2,_3)
 + Camera_(2,_4) + Camera_(3,_0) + Camera_(3,_1) + Camera_(3,_2)
 + Camera_(3,_3) + Camera_(3,_4) + Camera_(4,_0) + Camera_(4,_2)
 + Camera_(4,_4)
Subject To
Container_coverage_0_0: Camera_(0,_1) + Camera_(0,_2) + Camera_(1,_0)
 + Camera_(2,_0) >= 1
Container_coverage_0_4: Camera_(0,_2) + Camera_(0,_3) + Camera_(1,_4)
 + Camera_(2,_4) >= 1
Container_coverage_2_2: Camera_(0,_2) + Camera_(1,_2) + Camera_(2,_0)
 + Camera_(2,_1) + Camera_(2,_3) + Camera_(2,_4) + Camera_(3,_2)
 + Camera_(4,_2) >= 1
Container_coverage_4_1: Camera_(2,_1) + Camera_(3,_1) + Camera_(4,_0)
 + Camera_(4,_2) >= 1
Container_coverage_4_3: Camera_(2,_3) + Camera_(3,_3) + Camera_(4,_2)
 + Camera_(4,_4) >= 1
Binaries
Camera_(0,_1)
Camera_(0,_2)
Camera_(0,_3)
Camera_(1,_0)
Camera_(1,_1)
Camera_(1,_2)
Camera_(1,_3)
Camera_(1,_4)
Camera_(2,_0)
Camera_(2,_1)
Camera_(2,_3)
Camera_(2,_4)
Camera_(3,_0)
Camera_(3,_1)
Camera_(3,_2)
Camera_(3,_3)
Camera_(3,_4)
Camera_(4,_0)
Camera_(4,_2)
Camera_(4,_4)
End
