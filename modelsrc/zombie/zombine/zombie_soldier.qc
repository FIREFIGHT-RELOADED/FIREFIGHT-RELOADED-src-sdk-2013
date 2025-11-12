// Created by Crowbar 0.72

$modelname "Zombie/Zombie_Soldier.mdl"

$body studio "Zombie_Soldier_reference.smd"
$bodygroup "headcrab1"
{
	blank
	studio "Headcrab_reference.smd"
}
$bodygroup "headcrab_shell"
{
	blank
	studio "Headcrab_shell_reference.smd"
}

$lod 10
{
	replacemodel "Zombie_Soldier_reference.smd" "Zombie_Soldier_reference_lod1.smd"
	replacemodel "Headcrab_reference.smd" "Headcrab_reference_lod1.smd"
	nofacial
}
$lod 20
{
	replacemodel "Zombie_Soldier_reference.smd" "Zombie_Soldier_reference_lod2.smd"
	replacemodel "Headcrab_reference.smd" "Headcrab_reference_lod2.smd"
	replacebone "ValveBiped.Bip01_L_Finger01" "ValveBiped.Bip01_L_Finger0"
	replacebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01"
	replacebone "ValveBiped.Bip01_L_Finger31" "ValveBiped.Bip01_L_Finger3"
	replacebone "ValveBiped.Bip01_L_Finger32" "ValveBiped.Bip01_L_Finger31"
	replacebone "ValveBiped.Bip01_L_Finger21" "ValveBiped.Bip01_L_Finger2"
	replacebone "ValveBiped.Bip01_L_Finger22" "ValveBiped.Bip01_L_Finger21"
	replacebone "ValveBiped.Bip01_L_Finger11" "ValveBiped.Bip01_L_Finger1"
	replacebone "ValveBiped.Bip01_L_Finger12" "ValveBiped.Bip01_L_Finger11"
	replacebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0"
	replacebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01"
	replacebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3"
	replacebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31"
	replacebone "ValveBiped.Bip01_R_Finger21" "ValveBiped.Bip01_R_Finger2"
	replacebone "ValveBiped.Bip01_R_Finger22" "ValveBiped.Bip01_R_Finger21"
	replacebone "ValveBiped.Bip01_R_Finger11" "ValveBiped.Bip01_R_Finger1"
	replacebone "ValveBiped.Bip01_R_Finger12" "ValveBiped.Bip01_R_Finger11"
	replacebone "ValveBiped.HC_Head_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_FangL_Bone1" "ValveBiped.HC_Head_Bone"
	replacebone "ValveBiped.HC_FangL_Bone2" "ValveBiped.HC_FangL_Bone1"
	replacebone "ValveBiped.HC_FangR_Bone1" "ValveBiped.HC_Head_Bone"
	replacebone "ValveBiped.HC_FangR_Bone2" "ValveBiped.HC_FangR_Bone1"
	replacebone "ValveBiped.HC_HoleControlL" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_HoleControlR" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_Rear_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ThighL_Bone" "ValveBiped.HC_Rear_Bone"
	replacebone "ValveBiped.HC_CalfL_Bone" "ValveBiped.HC_ThighL_Bone"
	replacebone "ValveBiped.HC_ThighR_Bone" "ValveBiped.HC_Rear_Bone"
	replacebone "ValveBiped.HC_CalfR_Bone" "ValveBiped.HC_ThighR_Bone"
	replacebone "ValveBiped.HC_UpperArmL_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ForeArmL_Bone" "ValveBiped.HC_UpperArmL_Bone"
	replacebone "ValveBiped.HC_UpperArmR_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ForeArmR_Bone" "ValveBiped.HC_UpperArmR_Bone"
	nofacial
}
$lod 35
{
	replacemodel "Zombie_Soldier_reference.smd" "Zombie_Soldier_reference_lod3.smd"
	replacemodel "Headcrab_reference.smd" "Headcrab_reference_lod3.smd"
	replacemodel "Headcrab_shell_reference.smd" "Headcrab_shell_reference_lod1.smd"
	replacebone "ValveBiped.Bip01_L_Toe0" "ValveBiped.Bip01_L_Foot"
	replacebone "ValveBiped.Bip01_R_Toe0" "ValveBiped.Bip01_R_Foot"
	replacebone "ValveBiped.Bip01_L_Finger0" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger01" "ValveBiped.Bip01_L_Finger0"
	replacebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01"
	replacebone "ValveBiped.Bip01_L_Finger3" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger31" "ValveBiped.Bip01_L_Finger3"
	replacebone "ValveBiped.Bip01_L_Finger32" "ValveBiped.Bip01_L_Finger31"
	replacebone "ValveBiped.Bip01_L_Finger2" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger21" "ValveBiped.Bip01_L_Finger2"
	replacebone "ValveBiped.Bip01_L_Finger22" "ValveBiped.Bip01_L_Finger21"
	replacebone "ValveBiped.Bip01_L_Finger1" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger11" "ValveBiped.Bip01_L_Finger1"
	replacebone "ValveBiped.Bip01_L_Finger12" "ValveBiped.Bip01_L_Finger11"
	replacebone "ValveBiped.Bip01_R_Finger0" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0"
	replacebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01"
	replacebone "ValveBiped.Bip01_R_Finger3" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3"
	replacebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31"
	replacebone "ValveBiped.Bip01_R_Finger2" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger21" "ValveBiped.Bip01_R_Finger2"
	replacebone "ValveBiped.Bip01_R_Finger22" "ValveBiped.Bip01_R_Finger21"
	replacebone "ValveBiped.Bip01_R_Finger1" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger11" "ValveBiped.Bip01_R_Finger1"
	replacebone "ValveBiped.Bip01_R_Finger12" "ValveBiped.Bip01_R_Finger11"
	replacebone "ValveBiped.HC_Head_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_FangL_Bone1" "ValveBiped.HC_Head_Bone"
	replacebone "ValveBiped.HC_FangL_Bone2" "ValveBiped.HC_FangL_Bone1"
	replacebone "ValveBiped.HC_FangR_Bone1" "ValveBiped.HC_Head_Bone"
	replacebone "ValveBiped.HC_FangR_Bone2" "ValveBiped.HC_FangR_Bone1"
	replacebone "ValveBiped.HC_HoleControlL" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_HoleControlR" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_Rear_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ThighL_Bone" "ValveBiped.HC_Rear_Bone"
	replacebone "ValveBiped.HC_CalfL_Bone" "ValveBiped.HC_ThighL_Bone"
	replacebone "ValveBiped.HC_ThighR_Bone" "ValveBiped.HC_Rear_Bone"
	replacebone "ValveBiped.HC_CalfR_Bone" "ValveBiped.HC_ThighR_Bone"
	replacebone "ValveBiped.HC_UpperArmL_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ForeArmL_Bone" "ValveBiped.HC_UpperArmL_Bone"
	replacebone "ValveBiped.HC_UpperArmR_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ForeArmR_Bone" "ValveBiped.HC_UpperArmR_Bone"
	nofacial
}
$lod 60
{
	replacemodel "Zombie_Soldier_reference.smd" "Zombie_Soldier_reference_lod4.smd"
	replacemodel "Headcrab_reference.smd" "Headcrab_reference_lod4.smd"
	replacemodel "Headcrab_shell_reference.smd" "Headcrab_shell_reference_lod2.smd"
	replacebone "ValveBiped.Bip01_L_Foot" "ValveBiped.Bip01_L_Calf"
	replacebone "ValveBiped.Bip01_L_Toe0" "ValveBiped.Bip01_L_Foot"
	replacebone "ValveBiped.Bip01_R_Foot" "ValveBiped.Bip01_R_Calf"
	replacebone "ValveBiped.Bip01_R_Toe0" "ValveBiped.Bip01_R_Foot"
	replacebone "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_L_Forearm"
	replacebone "ValveBiped.Bip01_L_Finger0" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger01" "ValveBiped.Bip01_L_Finger0"
	replacebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01"
	replacebone "ValveBiped.Bip01_L_Finger3" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger31" "ValveBiped.Bip01_L_Finger3"
	replacebone "ValveBiped.Bip01_L_Finger32" "ValveBiped.Bip01_L_Finger31"
	replacebone "ValveBiped.Bip01_L_Finger2" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger21" "ValveBiped.Bip01_L_Finger2"
	replacebone "ValveBiped.Bip01_L_Finger22" "ValveBiped.Bip01_L_Finger21"
	replacebone "ValveBiped.Bip01_L_Finger1" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger11" "ValveBiped.Bip01_L_Finger1"
	replacebone "ValveBiped.Bip01_L_Finger12" "ValveBiped.Bip01_L_Finger11"
	replacebone "ValveBiped.Bip01_R_Hand" "ValveBiped.Bip01_R_Forearm"
	replacebone "ValveBiped.Bip01_R_Finger0" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0"
	replacebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01"
	replacebone "ValveBiped.Bip01_R_Finger3" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3"
	replacebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31"
	replacebone "ValveBiped.Bip01_R_Finger2" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger21" "ValveBiped.Bip01_R_Finger2"
	replacebone "ValveBiped.Bip01_R_Finger22" "ValveBiped.Bip01_R_Finger21"
	replacebone "ValveBiped.Bip01_R_Finger1" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger11" "ValveBiped.Bip01_R_Finger1"
	replacebone "ValveBiped.Bip01_R_Finger12" "ValveBiped.Bip01_R_Finger11"
	replacebone "ValveBiped.HC_Body_Bone" "ValveBiped.Bip01_Spine4"
	replacebone "ValveBiped.HC_Head_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_FangL_Bone1" "ValveBiped.HC_Head_Bone"
	replacebone "ValveBiped.HC_FangL_Bone2" "ValveBiped.HC_FangL_Bone1"
	replacebone "ValveBiped.HC_FangR_Bone1" "ValveBiped.HC_Head_Bone"
	replacebone "ValveBiped.HC_FangR_Bone2" "ValveBiped.HC_FangR_Bone1"
	replacebone "ValveBiped.HC_HoleControlL" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_HoleControlR" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_Rear_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ThighL_Bone" "ValveBiped.HC_Rear_Bone"
	replacebone "ValveBiped.HC_CalfL_Bone" "ValveBiped.HC_ThighL_Bone"
	replacebone "ValveBiped.HC_ThighR_Bone" "ValveBiped.HC_Rear_Bone"
	replacebone "ValveBiped.HC_CalfR_Bone" "ValveBiped.HC_ThighR_Bone"
	replacebone "ValveBiped.HC_UpperArmL_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ForeArmL_Bone" "ValveBiped.HC_UpperArmL_Bone"
	replacebone "ValveBiped.HC_UpperArmR_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ForeArmR_Bone" "ValveBiped.HC_UpperArmR_Bone"
	nofacial
}
$lod 90
{
	replacemodel "Zombie_Soldier_reference.smd" "Zombie_Soldier_reference_lod5.smd"
	replacemodel "Headcrab_reference.smd" "Headcrab_reference_lod5.smd"
	replacemodel "Headcrab_shell_reference.smd" "Headcrab_shell_reference_lod3.smd"
	replacebone "ValveBiped.Bip01_L_Foot" "ValveBiped.Bip01_L_Calf"
	replacebone "ValveBiped.Bip01_L_Toe0" "ValveBiped.Bip01_L_Foot"
	replacebone "ValveBiped.Bip01_R_Foot" "ValveBiped.Bip01_R_Calf"
	replacebone "ValveBiped.Bip01_R_Toe0" "ValveBiped.Bip01_R_Foot"
	replacebone "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_L_Forearm"
	replacebone "ValveBiped.Bip01_L_Finger0" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger01" "ValveBiped.Bip01_L_Finger0"
	replacebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01"
	replacebone "ValveBiped.Bip01_L_Finger3" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger31" "ValveBiped.Bip01_L_Finger3"
	replacebone "ValveBiped.Bip01_L_Finger32" "ValveBiped.Bip01_L_Finger31"
	replacebone "ValveBiped.Bip01_L_Finger2" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger21" "ValveBiped.Bip01_L_Finger2"
	replacebone "ValveBiped.Bip01_L_Finger22" "ValveBiped.Bip01_L_Finger21"
	replacebone "ValveBiped.Bip01_L_Finger1" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger11" "ValveBiped.Bip01_L_Finger1"
	replacebone "ValveBiped.Bip01_L_Finger12" "ValveBiped.Bip01_L_Finger11"
	replacebone "ValveBiped.Bip01_R_Hand" "ValveBiped.Bip01_R_Forearm"
	replacebone "ValveBiped.Bip01_R_Finger0" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0"
	replacebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01"
	replacebone "ValveBiped.Bip01_R_Finger3" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3"
	replacebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31"
	replacebone "ValveBiped.Bip01_R_Finger2" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger21" "ValveBiped.Bip01_R_Finger2"
	replacebone "ValveBiped.Bip01_R_Finger22" "ValveBiped.Bip01_R_Finger21"
	replacebone "ValveBiped.Bip01_R_Finger1" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger11" "ValveBiped.Bip01_R_Finger1"
	replacebone "ValveBiped.Bip01_R_Finger12" "ValveBiped.Bip01_R_Finger11"
	replacebone "ValveBiped.HC_Body_Bone" "ValveBiped.Bip01_Spine4"
	replacebone "ValveBiped.HC_Head_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_FangL_Bone1" "ValveBiped.HC_Head_Bone"
	replacebone "ValveBiped.HC_FangL_Bone2" "ValveBiped.HC_FangL_Bone1"
	replacebone "ValveBiped.HC_FangR_Bone1" "ValveBiped.HC_Head_Bone"
	replacebone "ValveBiped.HC_FangR_Bone2" "ValveBiped.HC_FangR_Bone1"
	replacebone "ValveBiped.HC_HoleControlL" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_HoleControlR" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_Rear_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ThighL_Bone" "ValveBiped.HC_Rear_Bone"
	replacebone "ValveBiped.HC_CalfL_Bone" "ValveBiped.HC_ThighL_Bone"
	replacebone "ValveBiped.HC_ThighR_Bone" "ValveBiped.HC_Rear_Bone"
	replacebone "ValveBiped.HC_CalfR_Bone" "ValveBiped.HC_ThighR_Bone"
	replacebone "ValveBiped.HC_UpperArmL_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ForeArmL_Bone" "ValveBiped.HC_UpperArmL_Bone"
	replacebone "ValveBiped.HC_UpperArmR_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ForeArmR_Bone" "ValveBiped.HC_UpperArmR_Bone"
	nofacial
}
$shadowlod
{
	replacemodel "Zombie_Soldier_reference.smd" "Zombie_Soldier_reference_lod6.smd"
	replacemodel "Headcrab_reference.smd" "Headcrab_reference_lod6.smd"
	replacemodel "Headcrab_shell_reference.smd" "Headcrab_shell_reference_lod3.smd"
	replacebone "ValveBiped.Bip01_L_Foot" "ValveBiped.Bip01_L_Calf"
	replacebone "ValveBiped.Bip01_L_Toe0" "ValveBiped.Bip01_L_Foot"
	replacebone "ValveBiped.Bip01_R_Foot" "ValveBiped.Bip01_R_Calf"
	replacebone "ValveBiped.Bip01_R_Toe0" "ValveBiped.Bip01_R_Foot"
	replacebone "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_L_Forearm"
	replacebone "ValveBiped.Bip01_L_Finger0" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger01" "ValveBiped.Bip01_L_Finger0"
	replacebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01"
	replacebone "ValveBiped.Bip01_L_Finger3" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger31" "ValveBiped.Bip01_L_Finger3"
	replacebone "ValveBiped.Bip01_L_Finger32" "ValveBiped.Bip01_L_Finger31"
	replacebone "ValveBiped.Bip01_L_Finger2" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger21" "ValveBiped.Bip01_L_Finger2"
	replacebone "ValveBiped.Bip01_L_Finger22" "ValveBiped.Bip01_L_Finger21"
	replacebone "ValveBiped.Bip01_L_Finger1" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger11" "ValveBiped.Bip01_L_Finger1"
	replacebone "ValveBiped.Bip01_L_Finger12" "ValveBiped.Bip01_L_Finger11"
	replacebone "ValveBiped.Bip01_R_Hand" "ValveBiped.Bip01_R_Forearm"
	replacebone "ValveBiped.Bip01_R_Finger0" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0"
	replacebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01"
	replacebone "ValveBiped.Bip01_R_Finger3" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3"
	replacebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31"
	replacebone "ValveBiped.Bip01_R_Finger2" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger21" "ValveBiped.Bip01_R_Finger2"
	replacebone "ValveBiped.Bip01_R_Finger22" "ValveBiped.Bip01_R_Finger21"
	replacebone "ValveBiped.Bip01_R_Finger1" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger11" "ValveBiped.Bip01_R_Finger1"
	replacebone "ValveBiped.Bip01_R_Finger12" "ValveBiped.Bip01_R_Finger11"
	replacebone "ValveBiped.HC_Body_Bone" "ValveBiped.Bip01_Spine4"
	replacebone "ValveBiped.HC_Head_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_FangL_Bone1" "ValveBiped.HC_Head_Bone"
	replacebone "ValveBiped.HC_FangL_Bone2" "ValveBiped.HC_FangL_Bone1"
	replacebone "ValveBiped.HC_FangR_Bone1" "ValveBiped.HC_Head_Bone"
	replacebone "ValveBiped.HC_FangR_Bone2" "ValveBiped.HC_FangR_Bone1"
	replacebone "ValveBiped.HC_HoleControlL" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_HoleControlR" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_Rear_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ThighL_Bone" "ValveBiped.HC_Rear_Bone"
	replacebone "ValveBiped.HC_CalfL_Bone" "ValveBiped.HC_ThighL_Bone"
	replacebone "ValveBiped.HC_ThighR_Bone" "ValveBiped.HC_Rear_Bone"
	replacebone "ValveBiped.HC_CalfR_Bone" "ValveBiped.HC_ThighR_Bone"
	replacebone "ValveBiped.HC_UpperArmL_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ForeArmL_Bone" "ValveBiped.HC_UpperArmL_Bone"
	replacebone "ValveBiped.HC_UpperArmR_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ForeArmR_Bone" "ValveBiped.HC_UpperArmR_Bone"
	nofacial
}

$surfaceprop "zombieflesh"

$contents "solid"

$illumposition -0.099 1.246 35.512

$eyeposition 0 0 64

$mostlyopaque

$texturegroup skinfamilies
{
	{ "combinesoldiersheet_zombie"  }
	{ "combinesoldiersheet_zombie_prison" }
	{ "combinesoldiersheet_zombie_shotgunner" }
	{ "combinesoldiersheet_zombie_elite_giblimb" }
	{ "combinesoldiersheet_zombie_ace" }
}

$cdmaterials "models\zombie_classic\"
$cdmaterials "models\headcrab_classic\"
$cdmaterials "models\combine_soldier\"
$cdmaterials "models\headcrab_armor\"

$attachment "headcrab" "ValveBiped.HC_Rear_Bone" 0 0 0 rotate 0 0 0
$attachment "eyes" "ValveBiped.HC_Body_Bone" 0 0 0 rotate 0 0 -50
$attachment "head" "ValveBiped.HC_Body_Bone" 0 0 0 rotate 0 0 -50
$attachment "chest" "ValveBiped.Bip01_Spine4" -3 6 0 rotate 0 60 90
$attachment "maw" "ValveBiped.Bip01_Spine2" 0 8 0 rotate 0 90 90
$attachment "Blood_Left" "ValveBiped.Bip01_L_Finger2" 0 0 0 rotate 0 0 0
$attachment "Blood_Right" "ValveBiped.Bip01_R_Finger2" 0 0 0 rotate 0 0 0
$attachment "grenade_attachment" "ValveBiped.Bip01_L_Hand" 4.38 -2.28 1.98 rotate -47.22 2.63 -97.91

$cbox 0 0 0 0 0 0

$bbox -13 -13 0 13 13 72

$hboxset "default"
$hbox 2 "ValveBiped.Bip01_Spine1" -8.5 -3.84 -7.12 15.5 9.84 7.73
$hbox 4 "ValveBiped.Bip01_L_UpperArm" -1 -3.5 -2.5 11 3.5 2.5
$hbox 4 "ValveBiped.Bip01_L_Forearm" -0.5 -1.86 -1.9 10.5 3.11 2.36
$hbox 4 "ValveBiped.Bip01_L_Hand" 0 -1.78 -1 6 1.23 2
$hbox 5 "ValveBiped.Bip01_R_UpperArm" -1 -3.5 -2.5 11 3.5 2.5
$hbox 5 "ValveBiped.Bip01_R_Forearm" -0.5 -1.89 -1.6 10.5 3.09 2.66
$hbox 5 "ValveBiped.Bip01_R_Hand" 0 -1.78 -2.5 6 1.23 0.5
$hbox 6 "ValveBiped.Bip01_L_Thigh" 0 -4 -3.73 17.25 4 4.29
$hbox 6 "ValveBiped.Bip01_L_Calf" 0 -4.87 -3.54 17.25 3.18 2.63
$hbox 6 "ValveBiped.Bip01_L_Foot" -1.5 -2 -2.5 7.5 2 2.5
$hbox 7 "ValveBiped.Bip01_R_Thigh" 0 -4 -3.73 17.25 4 4.29
$hbox 7 "ValveBiped.Bip01_R_Calf" 0 -4.92 -3.12 17.25 3.25 2.65
$hbox 7 "ValveBiped.Bip01_R_Foot" -1.5 -2 -2.5 7.5 2 2.5
$hbox 1 "ValveBiped.HC_Body_Bone" -6.59 -5 -5.76 8.5 5 6.24

$definebone "ValveBiped.Bip01_Pelvis" "" -0.000005 1.516003 38.353996 0 0 89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Thigh" "ValveBiped.Bip01_Pelvis" 3.890453 0 -0.000004 -6.298009 -90.530886 -89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Calf" "ValveBiped.Bip01_L_Thigh" 17.59317 0 0 0 16.008153 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Foot" "ValveBiped.Bip01_L_Calf" 17.699581 0 0 -2.794963 -66.211908 -0.906668 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Toe0" "ValveBiped.Bip01_L_Foot" 6.879457 0.000001 0 2.831042 -33.495514 -3.653923 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Thigh" "ValveBiped.Bip01_Pelvis" -3.890452 0 0.000007 -6.255897 -90.309895 -89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Calf" "ValveBiped.Bip01_R_Thigh" 17.593157 0 0 0 16.011995 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Foot" "ValveBiped.Bip01_R_Calf" 17.699577 0 0 2.330621 -66.226587 2.220957 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Toe0" "ValveBiped.Bip01_R_Foot" 6.879459 -0.000001 0 -3.168914 -33.468733 -1.473705 0 0 0 0 0 0
$definebone "ValveBiped.Cod_Bone" "ValveBiped.Bip01_Pelvis" 0.003197 1.916843 7.635199 0 0 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine" "ValveBiped.Bip01_Pelvis" 0.000002 3.345119 -2.981992 4.944283 89.999982 89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine1" "ValveBiped.Bip01_Spine" 4.126717 0 0 0 5.074513 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine2" "ValveBiped.Bip01_Spine1" 3.613476 0 0 0 7.43745 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine4" "ValveBiped.Bip01_Spine2" 9.183792 0 0 0 20.151326 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Clavicle" "ValveBiped.Bip01_Spine4" 1.673225 1.247211 2.595464 -73.711914 168.587701 91.510847 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_UpperArm" "ValveBiped.Bip01_L_Clavicle" 6.028141 0 0 -12.912923 -32.801772 -101.833749 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_L_UpperArm" 11.692551 0 -0.000004 0.000007 0.000002 0.000004 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_L_Forearm" 12.075582 0 0 -7.742145 1.798745 -178.061242 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger0" "ValveBiped.Bip01_L_Hand" 1.782816 1.657451 0.069283 -48.216457 72.036783 -69.320614 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger01" "ValveBiped.Bip01_L_Finger0" 2.070873 0.000002 0 0.000009 -50.400003 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01" 1.180885 0.000001 -0.000002 0.000003 -29.700022 -0.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger3" "ValveBiped.Bip01_L_Hand" 4.942486 -0.874831 0.248177 -17.957529 -6.592456 -88.934684 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger31" "ValveBiped.Bip01_L_Finger3" 1.980927 -0.000008 0 -0.100783 -39.849957 -0.079473 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger32" "ValveBiped.Bip01_L_Finger31" 1.731651 0.000002 0 4.526141 -26.093018 -0.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger2" "ValveBiped.Bip01_L_Hand" 5.040237 0.509704 0.13031 -9.293721 3.242996 -91.830807 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger21" "ValveBiped.Bip01_L_Finger2" 2.156435 0 0 -0.000002 -58.950017 -0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger22" "ValveBiped.Bip01_L_Finger21" 2.243126 0.000002 0 -0.000003 -38.474968 -0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger1" "ValveBiped.Bip01_L_Hand" 5.046515 1.425083 0.048111 -13.478379 13.535099 -102.595718 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger11" "ValveBiped.Bip01_L_Finger1" 2.069263 -0.000008 0 -0.000007 -48.150009 -0.000005 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger12" "ValveBiped.Bip01_L_Finger11" 2.441574 -0.000004 0 -0.000007 -48.149989 0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Clavicle" "ValveBiped.Bip01_Spine4" 1.673229 1.247223 -2.595471 73.711914 168.587141 -95.626931 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_UpperArm" "ValveBiped.Bip01_R_Clavicle" 6.028142 0 0 15.946169 -31.150765 105.625235 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_R_UpperArm" 11.692566 -0.000001 0.000004 -0.000034 -0.000003 0.000004 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Hand" "ValveBiped.Bip01_R_Forearm" 12.075581 0 0.000008 2.648417 2.781882 -87.398179 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger0" "ValveBiped.Bip01_R_Hand" 1.819733 -0.264271 -1.112211 39.081062 -74.649152 -87.270126 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0" 2.070881 0.000002 -0.000004 -0.000027 -50.400017 0.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01" 1.180882 0 -0.000002 -0.000017 -29.700019 0.000013 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger3" "ValveBiped.Bip01_R_Hand" 4.958584 -0.225746 1.352891 -7.206953 -18.07077 1.005483 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3" 1.980928 -0.000004 0 0.100782 -39.849967 0.079471 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31" 1.731651 0 0 -4.526141 -26.093008 0.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger2" "ValveBiped.Bip01_R_Hand" 5.056061 -0.114651 -0.09717 2.269831 -9.311738 -2.856768 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger21" "ValveBiped.Bip01_R_Finger2" 2.156437 0.000008 0 -0.000002 -58.95001 -0.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger22" "ValveBiped.Bip01_R_Finger21" 2.243126 0.000001 0 -0.000002 -38.474947 -0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger1" "ValveBiped.Bip01_R_Hand" 5.062344 -0.036644 -1.012916 13.092603 -13.908173 3.272965 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger11" "ValveBiped.Bip01_R_Finger1" 2.069262 0.000004 0 -0.000002 -48.149989 -0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger12" "ValveBiped.Bip01_R_Finger11" 2.441589 0.000001 0 -0.000004 -48.149979 0.000003 0 0 0 0 0 0
$definebone "ValveBiped.HC_Body_Bone" "ValveBiped.Bip01_Spine4" 7.856865 0.35714 2.41031 -9.085843 72.317298 154.900057 0 0 0 0 0 0
$definebone "ValveBiped.HC_Head_Bone" "ValveBiped.HC_Body_Bone" 3.7386 -2.965115 -0.136395 -4.500009 -4.500018 12.374987 0 0 0 0 0 0
$definebone "ValveBiped.HC_FangL_Bone1" "ValveBiped.HC_Head_Bone" 1.884798 0.225918 -3.244098 -7.359745 -99.621499 -49.057735 0 0 0 0 0 0
$definebone "ValveBiped.HC_FangL_Bone2" "ValveBiped.HC_FangL_Bone1" 3.698872 -0.000002 0 0 -38.081925 0.000002 0 0 0 0 0 0
$definebone "ValveBiped.HC_FangR_Bone1" "ValveBiped.HC_Head_Bone" 1.884669 0.22591 3.247674 7.36011 -99.621191 -130.939945 0 0 0 0 0 0
$definebone "ValveBiped.HC_FangR_Bone2" "ValveBiped.HC_FangR_Bone1" 3.698872 0 0 -0.000003 38.081918 -0.000022 0 0 0 0 0 0
$definebone "ValveBiped.HC_HoleControlL" "ValveBiped.HC_Body_Bone" -1.923262 -4.27964 -3.138432 -0.000004 -0.00001 0.000015 0 0 0 0 0 0
$definebone "ValveBiped.HC_HoleControlR" "ValveBiped.HC_Body_Bone" -0.320878 -5.869141 3.247988 -0.000004 -0.00001 0.000015 0 0 0 0 0 0
$definebone "ValveBiped.HC_Rear_Bone" "ValveBiped.HC_Body_Bone" -7.842797 -1.67841 -0.934004 -0.000009 20.249976 29.250029 0 0 0 0 0 0
$definebone "ValveBiped.HC_ThighL_Bone" "ValveBiped.HC_Rear_Bone" -0.450934 -0.675697 -6.164909 -30.718441 -70.093416 -172.984649 0 0 0 0 0 0
$definebone "ValveBiped.HC_CalfL_Bone" "ValveBiped.HC_ThighL_Bone" 6.098816 0 0.000004 -0.000024 -0.000012 -0.000002 0 0 0 0 0 0
$definebone "ValveBiped.HC_ThighR_Bone" "ValveBiped.HC_Rear_Bone" -0.450914 -0.675701 6.164912 -7.079968 -90.538652 174.066278 0 0 0 0 0 0
$definebone "ValveBiped.HC_CalfR_Bone" "ValveBiped.HC_ThighR_Bone" 6.098816 -0.000003 -0.000001 0.00001 -130.088657 -0.000015 0 0 0 0 0 0
$definebone "ValveBiped.HC_UpperArmL_Bone" "ValveBiped.HC_Body_Bone" 2.630718 -3.15509 -5.01671 17.056879 -28.79617 1.880354 0 0 0 0 0 0
$definebone "ValveBiped.HC_ForeArmL_Bone" "ValveBiped.HC_UpperArmL_Bone" 8.255686 0 0.000001 0.000029 -121.038779 0.000001 0 0 0 0 0 0
$definebone "ValveBiped.HC_UpperArmR_Bone" "ValveBiped.HC_Body_Bone" 2.630419 -3.155079 5.01687 -4.143266 -63.369635 -131.616994 0 0 0 0 0 0
$definebone "ValveBiped.HC_ForeArmR_Bone" "ValveBiped.HC_UpperArmR_Bone" 8.255684 0 0.000002 0.000027 110.674582 -0.000015 0 0 0 0 0 0


$ikchain "rhand" "ValveBiped.Bip01_R_Hand" knee 0.707 0.707 0
$ikchain "lhand" "ValveBiped.Bip01_L_Hand" knee 0.707 0.707 0
$ikchain "rfoot" "ValveBiped.Bip01_R_Foot" knee 0.707 -0.707 0
$ikchain "lfoot" "ValveBiped.Bip01_L_Foot" knee 0.707 -0.707 0

$ikautoplaylock "rfoot" 1 0.1
$ikautoplaylock "lfoot" 1 0.1

$sequence "refpose" {
	"zombie_soldier_anims\refpose.smd"
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 29 29 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 29 29 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 29 29 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 29 29 target 3
}

$sequence "ragdoll" {
	"zombie_soldier_anims\ragdoll.smd"
	activity "ACT_DIERAGDOLL" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 1 1 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 1 1 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 1 1 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 1 1 target 3
}

$includemodel "zombie\zombie_soldier_animations.mdl"

$collisionjoints "zombie_soldier_physics.smd"
{
	$mass 100
	$inertia 10
	$damping 0.1
	$rotdamping 3
	$rootbone "valvebiped.bip01_pelvis"


	$jointconstrain "ValveBiped.Bip01_Spine2" x limit -44 44 0.04
	$jointconstrain "ValveBiped.Bip01_Spine2" y limit -39 39 0.04
	$jointconstrain "ValveBiped.Bip01_Spine2" z limit -32 50 0.04

	$jointconstrain "ValveBiped.Bip01_R_Clavicle" x limit -30 30 0.04
	$jointconstrain "ValveBiped.Bip01_R_Clavicle" y limit -46 46 0.04
	$jointconstrain "ValveBiped.Bip01_R_Clavicle" z limit -26 48 0.04

	$jointconstrain "ValveBiped.Bip01_R_UpperArm" x limit -39 39 0.04
	$jointconstrain "ValveBiped.Bip01_R_UpperArm" y limit -79 95 0.04
	$jointconstrain "ValveBiped.Bip01_R_UpperArm" z limit -93 28 0.04

	$jointconstrain "ValveBiped.Bip01_L_Clavicle" x limit -30 30 0.04
	$jointconstrain "ValveBiped.Bip01_L_Clavicle" y limit -46 46 0.04
	$jointconstrain "ValveBiped.Bip01_L_Clavicle" z limit -26 48 0.04

	$jointconstrain "ValveBiped.Bip01_L_UpperArm" x limit -30 30 0.04
	$jointconstrain "ValveBiped.Bip01_L_UpperArm" y limit -95 84 0.04
	$jointconstrain "ValveBiped.Bip01_L_UpperArm" z limit -86 26 0.04

	$jointconstrain "ValveBiped.Bip01_L_Forearm" x limit 0 0 0.04
	$jointconstrain "ValveBiped.Bip01_L_Forearm" y limit 0 0 0.04
	$jointconstrain "ValveBiped.Bip01_L_Forearm" z limit -149 4 0.04

	$jointconstrain "ValveBiped.Bip01_L_Hand" x limit -55 55 0.04
	$jointconstrain "ValveBiped.Bip01_L_Hand" y limit -55 55 0.04
	$jointconstrain "ValveBiped.Bip01_L_Hand" z limit -55 55 0.04

	$jointconstrain "ValveBiped.Bip01_R_Forearm" x limit 0 0 0.04
	$jointconstrain "ValveBiped.Bip01_R_Forearm" y limit 0 0 0.04
	$jointconstrain "ValveBiped.Bip01_R_Forearm" z limit -149 4 0.04

	$jointconstrain "ValveBiped.Bip01_R_Thigh" x limit -12 12 0.04
	$jointconstrain "ValveBiped.Bip01_R_Thigh" y limit -8 75 0.04
	$jointconstrain "ValveBiped.Bip01_R_Thigh" z limit -97 32 0.04

	$jointconstrain "ValveBiped.Bip01_R_Calf" x limit 0 0 0.04
	$jointconstrain "ValveBiped.Bip01_R_Calf" y limit 0 0 0.04
	$jointconstrain "ValveBiped.Bip01_R_Calf" z limit -12 126 0.04

	$jointconstrain "ValveBiped.Bip01_R_Foot" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Foot" y limit -25 6 0.04
	$jointconstrain "ValveBiped.Bip01_R_Foot" z limit -15 35 0.04

	$jointconstrain "ValveBiped.Bip01_L_Thigh" x limit -12 12 0.04
	$jointconstrain "ValveBiped.Bip01_L_Thigh" y limit -73 6 0.04
	$jointconstrain "ValveBiped.Bip01_L_Thigh" z limit -93 30 0.04

	$jointconstrain "ValveBiped.Bip01_L_Calf" x limit 0 0 0.04
	$jointconstrain "ValveBiped.Bip01_L_Calf" y limit 0 0 0.04
	$jointconstrain "ValveBiped.Bip01_L_Calf" z limit -8 126 0.04

	$jointconstrain "ValveBiped.Bip01_L_Foot" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Foot" y limit -19 19 0.04
	$jointconstrain "ValveBiped.Bip01_L_Foot" z limit -15 35 0.04

	$jointconstrain "ValveBiped.Bip01_R_Hand" x limit -55 55 0.04
	$jointconstrain "ValveBiped.Bip01_R_Hand" y limit -55 55 0.04
	$jointconstrain "ValveBiped.Bip01_R_Hand" z limit -55 55 0.04

	$jointcollide "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_R_Thigh"
	$jointcollide "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_L_Thigh"
	$jointcollide "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_R_Thigh"
	$jointcollide "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_L_Thigh"
	$jointcollide "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_Pelvis"
	$jointcollide "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_Pelvis"
	$jointcollide "ValveBiped.Bip01_R_Foot" "ValveBiped.Bip01_L_Calf"
	$jointcollide "ValveBiped.Bip01_L_Foot" "ValveBiped.Bip01_R_Calf"
	$jointcollide "ValveBiped.Bip01_L_Foot" "ValveBiped.Bip01_R_Foot"
	$jointcollide "ValveBiped.Bip01_R_Calf" "ValveBiped.Bip01_L_Calf"
	$jointcollide "ValveBiped.Bip01_R_Thigh" "ValveBiped.Bip01_L_Thigh"
	$jointcollide "ValveBiped.Bip01_R_UpperArm" "ValveBiped.Bip01_Spine2"
	$jointcollide "ValveBiped.Bip01_L_UpperArm" "ValveBiped.Bip01_Spine2"
	$jointcollide "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_Spine2"
	$jointcollide "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_Spine2"
}