// Created by Crowbar 0.72

$modelname "Zombie/Classic.mdl"
$body studio "Zombie_reference.smd"
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
	replacemodel "Zombie_reference.smd" "Zombie_reference_lod1.smd"
	replacemodel "Headcrab_reference.smd" "Headcrab_reference_lod1.smd"
	replacebone "ValveBiped.Bip01_L_Finger31" "ValveBiped.Bip01_L_Finger3"
	replacebone "ValveBiped.Bip01_L_Finger32" "ValveBiped.Bip01_L_Finger31"
	replacebone "ValveBiped.Bip01_L_Finger22" "ValveBiped.Bip01_L_Finger21"
	replacebone "ValveBiped.Bip01_L_Finger12" "ValveBiped.Bip01_L_Finger11"
	replacebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3"
	replacebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31"
	replacebone "ValveBiped.Bip01_R_Finger22" "ValveBiped.Bip01_R_Finger21"
	replacebone "ValveBiped.Bip01_R_Finger12" "ValveBiped.Bip01_R_Finger11"
	replacebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0"
	replacebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01"
	replacebone "ValveBiped.Bip01_L_Finger01" "ValveBiped.Bip01_L_Finger0"
	replacebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01"
	nofacial
}
$lod 20
{
	replacemodel "Zombie_reference.smd" "Zombie_reference_lod2.smd"
	replacemodel "Headcrab_reference.smd" "Headcrab_reference_lod2.smd"
	replacebone "ValveBiped.Bip01_L_Finger31" "ValveBiped.Bip01_L_Finger3"
	replacebone "ValveBiped.Bip01_L_Finger32" "ValveBiped.Bip01_L_Finger31"
	replacebone "ValveBiped.Bip01_L_Finger21" "ValveBiped.Bip01_L_Finger2"
	replacebone "ValveBiped.Bip01_L_Finger22" "ValveBiped.Bip01_L_Finger21"
	replacebone "ValveBiped.Bip01_L_Finger11" "ValveBiped.Bip01_L_Finger1"
	replacebone "ValveBiped.Bip01_L_Finger12" "ValveBiped.Bip01_L_Finger11"
	replacebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3"
	replacebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31"
	replacebone "ValveBiped.Bip01_R_Finger21" "ValveBiped.Bip01_R_Finger2"
	replacebone "ValveBiped.Bip01_R_Finger22" "ValveBiped.Bip01_R_Finger21"
	replacebone "ValveBiped.Bip01_R_Finger11" "ValveBiped.Bip01_R_Finger1"
	replacebone "ValveBiped.Bip01_R_Finger12" "ValveBiped.Bip01_R_Finger11"
	replacebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0"
	replacebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01"
	replacebone "ValveBiped.Bip01_L_Finger01" "ValveBiped.Bip01_L_Finger0"
	replacebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01"
	nofacial
}
$lod 35
{
	replacemodel "Zombie_reference.smd" "Zombie_reference_lod3.smd"
	replacemodel "Headcrab_reference.smd" "Headcrab_reference_lod3.smd"
	replacemodel "Headcrab_shell_reference.smd" "Headcrab_shell_reference_lod1.smd"
	replacebone "ValveBiped.Bip01_L_Toe0" "ValveBiped.Bip01_L_Foot"
	replacebone "ValveBiped.Bip01_R_Toe0" "ValveBiped.Bip01_R_Foot"
	replacebone "ValveBiped.Bip01_L_Finger3" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger31" "ValveBiped.Bip01_L_Finger3"
	replacebone "ValveBiped.Bip01_L_Finger32" "ValveBiped.Bip01_L_Finger31"
	replacebone "ValveBiped.Bip01_L_Finger2" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger21" "ValveBiped.Bip01_L_Finger2"
	replacebone "ValveBiped.Bip01_L_Finger22" "ValveBiped.Bip01_L_Finger21"
	replacebone "ValveBiped.Bip01_L_Finger1" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger11" "ValveBiped.Bip01_L_Finger1"
	replacebone "ValveBiped.Bip01_L_Finger12" "ValveBiped.Bip01_L_Finger11"
	replacebone "ValveBiped.Bip01_R_Finger3" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3"
	replacebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31"
	replacebone "ValveBiped.Bip01_R_Finger2" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger21" "ValveBiped.Bip01_R_Finger2"
	replacebone "ValveBiped.Bip01_R_Finger22" "ValveBiped.Bip01_R_Finger21"
	replacebone "ValveBiped.Bip01_R_Finger1" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger11" "ValveBiped.Bip01_R_Finger1"
	replacebone "ValveBiped.Bip01_R_Finger12" "ValveBiped.Bip01_R_Finger11"
	replacebone "ValveBiped.Bip01_R_Finger0" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0"
	replacebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01"
	replacebone "ValveBiped.Bip01_L_Finger0" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger01" "ValveBiped.Bip01_L_Finger0"
	replacebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01"
	replacebone "ValveBiped.HC_Head_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ThighL_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_CalfL_Bone" "ValveBiped.HC_ThighL_Bone"
	replacebone "ValveBiped.HC_ThighR_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_CalfR_Bone" "ValveBiped.HC_ThighR_Bone"
	replacebone "ValveBiped.HC_UpperArmL_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ForeArmL_Bone" "ValveBiped.HC_UpperArmL_Bone"
	replacebone "ValveBiped.HC_UpperArmR_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ForeArmR_Bone" "ValveBiped.HC_UpperArmR_Bone"
	nofacial
}
$lod 60
{
	replacemodel "Zombie_reference.smd" "Zombie_reference_lod4.smd"
	replacemodel "Headcrab_reference.smd" "Headcrab_reference_lod4.smd"
	replacemodel "Headcrab_shell_reference.smd" "Headcrab_shell_reference_lod2.smd"
	replacebone "ValveBiped.Bip01_L_Foot" "ValveBiped.Bip01_L_Calf"
	replacebone "ValveBiped.Bip01_L_Toe0" "ValveBiped.Bip01_L_Foot"
	replacebone "ValveBiped.Bip01_R_Foot" "ValveBiped.Bip01_R_Calf"
	replacebone "ValveBiped.Bip01_R_Toe0" "ValveBiped.Bip01_R_Foot"
	replacebone "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_L_Forearm"
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
	replacebone "ValveBiped.Bip01_R_Finger3" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3"
	replacebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31"
	replacebone "ValveBiped.Bip01_R_Finger2" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger21" "ValveBiped.Bip01_R_Finger2"
	replacebone "ValveBiped.Bip01_R_Finger22" "ValveBiped.Bip01_R_Finger21"
	replacebone "ValveBiped.Bip01_R_Finger1" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger11" "ValveBiped.Bip01_R_Finger1"
	replacebone "ValveBiped.Bip01_R_Finger12" "ValveBiped.Bip01_R_Finger11"
	replacebone "ValveBiped.Bip01_R_Finger0" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0"
	replacebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01"
	replacebone "ValveBiped.Bip01_L_Finger0" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger01" "ValveBiped.Bip01_L_Finger0"
	replacebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01"
	replacebone "ValveBiped.HC_Body_Bone" "ValveBiped.Bip01_Spine4"
	replacebone "ValveBiped.HC_Head_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ThighL_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_CalfL_Bone" "ValveBiped.HC_ThighL_Bone"
	replacebone "ValveBiped.HC_ThighR_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_CalfR_Bone" "ValveBiped.HC_ThighR_Bone"
	replacebone "ValveBiped.HC_UpperArmL_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ForeArmL_Bone" "ValveBiped.HC_UpperArmL_Bone"
	replacebone "ValveBiped.HC_UpperArmR_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ForeArmR_Bone" "ValveBiped.HC_UpperArmR_Bone"
	nofacial
}
$lod 90
{
	replacemodel "Zombie_reference.smd" "Zombie_reference_lod5.smd"
	replacemodel "Headcrab_reference.smd" "Headcrab_reference_lod5.smd"
	replacemodel "Headcrab_shell_reference.smd" "Headcrab_shell_reference_lod3.smd"
	replacebone "ValveBiped.Bip01_L_Foot" "ValveBiped.Bip01_L_Calf"
	replacebone "ValveBiped.Bip01_L_Toe0" "ValveBiped.Bip01_L_Foot"
	replacebone "ValveBiped.Bip01_R_Foot" "ValveBiped.Bip01_R_Calf"
	replacebone "ValveBiped.Bip01_R_Toe0" "ValveBiped.Bip01_R_Foot"
	replacebone "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_L_Forearm"
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
	replacebone "ValveBiped.Bip01_R_Finger3" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3"
	replacebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31"
	replacebone "ValveBiped.Bip01_R_Finger2" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger21" "ValveBiped.Bip01_R_Finger2"
	replacebone "ValveBiped.Bip01_R_Finger22" "ValveBiped.Bip01_R_Finger21"
	replacebone "ValveBiped.Bip01_R_Finger1" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger11" "ValveBiped.Bip01_R_Finger1"
	replacebone "ValveBiped.Bip01_R_Finger12" "ValveBiped.Bip01_R_Finger11"
	replacebone "ValveBiped.Bip01_R_Finger0" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0"
	replacebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01"
	replacebone "ValveBiped.Bip01_L_Finger0" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger01" "ValveBiped.Bip01_L_Finger0"
	replacebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01"
	replacebone "ValveBiped.HC_Body_Bone" "ValveBiped.Bip01_Spine4"
	replacebone "ValveBiped.HC_Head_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ThighL_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_CalfL_Bone" "ValveBiped.HC_ThighL_Bone"
	replacebone "ValveBiped.HC_ThighR_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_CalfR_Bone" "ValveBiped.HC_ThighR_Bone"
	replacebone "ValveBiped.HC_UpperArmL_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ForeArmL_Bone" "ValveBiped.HC_UpperArmL_Bone"
	replacebone "ValveBiped.HC_UpperArmR_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ForeArmR_Bone" "ValveBiped.HC_UpperArmR_Bone"
	nofacial
}
$shadowlod
{
	replacemodel "Zombie_reference.smd" "Zombie_reference_lod6.smd"
	replacemodel "Headcrab_reference.smd" "Headcrab_reference_lod6.smd"
	replacemodel "Headcrab_shell_reference.smd" "Headcrab_shell_reference_lod3.smd"
	replacebone "ValveBiped.Bip01_L_Foot" "ValveBiped.Bip01_L_Calf"
	replacebone "ValveBiped.Bip01_L_Toe0" "ValveBiped.Bip01_L_Foot"
	replacebone "ValveBiped.Bip01_R_Foot" "ValveBiped.Bip01_R_Calf"
	replacebone "ValveBiped.Bip01_R_Toe0" "ValveBiped.Bip01_R_Foot"
	replacebone "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_L_Forearm"
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
	replacebone "ValveBiped.Bip01_R_Finger3" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3"
	replacebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31"
	replacebone "ValveBiped.Bip01_R_Finger2" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger21" "ValveBiped.Bip01_R_Finger2"
	replacebone "ValveBiped.Bip01_R_Finger22" "ValveBiped.Bip01_R_Finger21"
	replacebone "ValveBiped.Bip01_R_Finger1" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger11" "ValveBiped.Bip01_R_Finger1"
	replacebone "ValveBiped.Bip01_R_Finger12" "ValveBiped.Bip01_R_Finger11"
	replacebone "ValveBiped.Bip01_R_Finger0" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0"
	replacebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01"
	replacebone "ValveBiped.Bip01_L_Finger0" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_L_Finger01" "ValveBiped.Bip01_L_Finger0"
	replacebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01"
	replacebone "ValveBiped.HC_Body_Bone" "ValveBiped.Bip01_Spine4"
	replacebone "ValveBiped.HC_Head_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ThighL_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_CalfL_Bone" "ValveBiped.HC_ThighL_Bone"
	replacebone "ValveBiped.HC_ThighR_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_CalfR_Bone" "ValveBiped.HC_ThighR_Bone"
	replacebone "ValveBiped.HC_UpperArmL_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ForeArmL_Bone" "ValveBiped.HC_UpperArmL_Bone"
	replacebone "ValveBiped.HC_UpperArmR_Bone" "ValveBiped.HC_Body_Bone"
	replacebone "ValveBiped.HC_ForeArmR_Bone" "ValveBiped.HC_UpperArmR_Bone"
	nofacial
}

$surfaceprop "zombieflesh"

$illumposition -2.887 -4.81 33.483

$eyeposition 0 0 64

$mostlyopaque

$cdmaterials "models\zombie_classic\"
$cdmaterials "models\headcrab_classic\"
$cdmaterials "models\headcrab_armor\"

$attachment "headcrab" "ValveBiped.HC_Body_Bone" -4.49 -1.32 -0.02 rotate 0 20.25 29.25
$attachment "eyes" "ValveBiped.HC_Body_Bone" 0 0 0 rotate 0 0 -50
$attachment "head" "ValveBiped.HC_Body_Bone" 0 0 0 rotate 0 0 -50
$attachment "chest" "ValveBiped.Bip01_Spine4" -3 6 0 rotate 0 60 90
$attachment "maw" "ValveBiped.Bip01_Spine2" 0 8 0 rotate 0 90 90
$attachment "Blood_Left" "ValveBiped.Bip01_L_Finger2" 0 0 0 rotate 0 0 0
$attachment "Blood_Right" "ValveBiped.Bip01_R_Finger2" 0 0 0 rotate 0 0 0

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

$definebone "ValveBiped.Bip01_Pelvis" "" -0.000005 1.515989 38.353996 0 -0.000057 90.000037 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Thigh" "ValveBiped.Bip01_Pelvis" 3.890453 -0.000001 -0.000004 -6.298009 -90.530886 -89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Calf" "ValveBiped.Bip01_L_Thigh" 17.593168 0 0 0 16.008157 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Foot" "ValveBiped.Bip01_L_Calf" 17.699579 0 0 -2.794963 -66.211956 -0.906613 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Toe0" "ValveBiped.Bip01_L_Foot" 6.879456 0 0 2.831041 -33.495514 -3.654004 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Thigh" "ValveBiped.Bip01_Pelvis" -3.890452 -0.000001 0.000007 -6.255897 -90.309895 -89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Calf" "ValveBiped.Bip01_R_Thigh" 17.593164 0 0 0 16.011995 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Foot" "ValveBiped.Bip01_R_Calf" 17.699577 0 0 2.33062 -66.226587 2.220957 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Toe0" "ValveBiped.Bip01_R_Foot" 6.879456 0 0 -3.168915 -33.46875 -1.473704 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine" "ValveBiped.Bip01_Pelvis" 0.000002 3.345117 -2.981992 4.944339 89.999982 89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine1" "ValveBiped.Bip01_Spine" 4.126705 0 0 0 5.074516 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine2" "ValveBiped.Bip01_Spine1" 3.613468 0 0 0 7.43745 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine4" "ValveBiped.Bip01_Spine2" 9.183786 0 0 0 20.151327 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Clavicle" "ValveBiped.Bip01_Spine4" 1.673226 1.247213 2.595464 -73.711914 168.587701 91.510847 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_UpperArm" "ValveBiped.Bip01_L_Clavicle" 6.028144 0.000002 0 -12.91292 -32.801775 -101.833749 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_L_UpperArm" 11.692552 0 -0.000002 -0.000001 0.000001 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_L_Forearm" 12.075551 0 -0.000034 -7.742148 1.798745 -178.061242 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger3" "ValveBiped.Bip01_L_Hand" 5.564225 -0.55798 -0.100454 -51.159985 -75.906092 -14.492053 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger31" "ValveBiped.Bip01_L_Finger3" 5.508581 0.000001 0.000001 -0.128403 -78.099989 0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger32" "ValveBiped.Bip01_L_Finger31" 3.800001 0.000001 0 4.526078 -66.592992 0.000004 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger2" "ValveBiped.Bip01_L_Hand" 5.744637 0.55798 0.100453 -61.369393 -58.71764 8.796274 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger21" "ValveBiped.Bip01_L_Finger2" 6.052545 0 -0.000001 -23.542209 -84.10796 0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger22" "ValveBiped.Bip01_L_Finger21" 3.325016 -0.000002 0.000001 -2.203884 -59.043413 0.000004 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger1" "ValveBiped.Bip01_L_Hand" 5.613006 1.658157 0.708444 -70.499478 -87.096974 76.889618 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger11" "ValveBiped.Bip01_L_Finger1" 5.643139 0.000001 0.000001 -51.519625 -88.553491 0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger12" "ValveBiped.Bip01_L_Finger11" 3.079117 -0.000003 0 -7.424503 -67.370787 0.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Clavicle" "ValveBiped.Bip01_Spine4" 1.673228 1.247218 -2.595471 73.711914 168.587141 -95.626931 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_UpperArm" "ValveBiped.Bip01_R_Clavicle" 6.028143 -0.000001 -0.000001 15.946161 -31.150741 105.625228 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_R_UpperArm" 11.692555 0 0 0.000002 0.000001 0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Hand" "ValveBiped.Bip01_R_Forearm" 12.075577 0 -0.000002 3.975641 5.609831 -177.6521 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger3" "ValveBiped.Bip01_R_Hand" 5.943744 -0.602343 0.006442 74.957945 -83.501617 28.170793 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3" 5.043456 0.000001 0 -13.470006 -76.728501 0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31" 3.384895 0.000001 0 -9.064307 -97.57958 -0.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger2" "ValveBiped.Bip01_R_Hand" 6.213897 0.602343 -0.006442 74.259169 -34.667622 -19.526964 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger21" "ValveBiped.Bip01_R_Finger2" 5.536302 0 0 65.687206 -54.304481 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger22" "ValveBiped.Bip01_R_Finger21" 3.312718 0.000001 0 29.106206 -76.658198 -0.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger1" "ValveBiped.Bip01_R_Hand" 5.899514 1.775719 -0.435098 84.408202 -51.263237 -93.161936 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger11" "ValveBiped.Bip01_R_Finger1" 5.285242 0 0 68.230564 26.411804 0.000007 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger12" "ValveBiped.Bip01_R_Finger11" 3.09885 0.000001 -0.000003 49.308976 76.043699 0.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger0" "ValveBiped.Bip01_R_Hand" 4.015265 1.836178 -0.57813 61.681943 75.212404 147.088698 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0" 2.400002 -0.000001 0 -42.749987 0.000001 0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01" 1.399998 0 0.000001 -52.875008 0.000002 0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger0" "ValveBiped.Bip01_L_Hand" 3.420513 1.944044 0.267537 -56.41616 68.849393 -156.975915 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger01" "ValveBiped.Bip01_L_Finger0" 2.524998 -0.000002 0 38.249975 -0.000005 0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01" 1.582001 0.000001 0 48.374996 -0.000004 -0.000001 0 0 0 0 0 0
$definebone "ValveBiped.HC_Body_Bone" "ValveBiped.Bip01_Spine4" 7.856863 0.357139 2.41031 -9.085849 72.317298 154.900057 0 0 0 0 0 0
$definebone "ValveBiped.HC_Head_Bone" "ValveBiped.HC_Body_Bone" 3.738598 -2.965121 -0.136398 -4.500011 -4.50001 12.374975 0 0 0 0 0 0
$definebone "ValveBiped.HC_ThighL_Bone" "ValveBiped.HC_Body_Bone" -9.104416 0.438527 -6.643026 -2.90762 -52.706941 -163.395364 0 0 0 0 0 0
$definebone "ValveBiped.HC_CalfL_Bone" "ValveBiped.HC_ThighL_Bone" 6.098811 -0.000001 0 -0.000002 -0.000006 -0.000001 0 0 0 0 0 0
$definebone "ValveBiped.HC_ThighR_Bone" "ValveBiped.HC_Body_Bone" -7.019177 -5.213707 4.114696 22.168719 -70.327227 173.782074 0 0 0 0 0 0
$definebone "ValveBiped.HC_CalfR_Bone" "ValveBiped.HC_ThighR_Bone" 6.09881 0 0 0.000002 -130.088644 -0.000005 0 0 0 0 0 0
$definebone "ValveBiped.HC_UpperArmL_Bone" "ValveBiped.HC_Body_Bone" 2.630715 -3.155085 -5.016712 17.05689 -28.796166 1.880337 0 0 0 0 0 0
$definebone "ValveBiped.HC_ForeArmL_Bone" "ValveBiped.HC_UpperArmL_Bone" 8.255678 0 0.000001 0.000006 -121.038766 0.000001 0 0 0 0 0 0
$definebone "ValveBiped.HC_UpperArmR_Bone" "ValveBiped.HC_Body_Bone" 2.630416 -3.155087 5.016868 -4.143286 -63.369641 -131.616994 0 0 0 0 0 0
$definebone "ValveBiped.HC_ForeArmR_Bone" "ValveBiped.HC_UpperArmR_Bone" 8.255675 -0.000001 0 0.000012 110.674589 -0.000002 0 0 0 0 0 0

$poseparameter "move_yaw" -180 180 loop 360

$ikchain "rhand" "ValveBiped.Bip01_R_Hand" knee 0.707 0.707 0
$ikchain "lhand" "ValveBiped.Bip01_L_Hand" knee 0.707 0.707 0
$ikchain "rfoot" "ValveBiped.Bip01_R_Foot" knee 0.707 -0.707 0
$ikchain "lfoot" "ValveBiped.Bip01_L_Foot" knee 0.707 -0.707 0

$ikautoplaylock "rfoot" 1 0.1
$ikautoplaylock "lfoot" 1 0.1

$weightlist "weights_flinch1inDelta" {
	 "ValveBiped.Bip01_Pelvis" 1
	 "ValveBiped.Bip01_L_Thigh" 0
	 "ValveBiped.Bip01_L_Calf" 0
	 "ValveBiped.Bip01_L_Foot" 0
	 "ValveBiped.Bip01_L_Toe0" 0
	 "ValveBiped.Bip01_R_Thigh" 0
	 "ValveBiped.Bip01_R_Calf" 0
	 "ValveBiped.Bip01_R_Foot" 0
	 "ValveBiped.Bip01_R_Toe0" 0
	 "ValveBiped.Bip01_Spine" 1
	 "ValveBiped.Bip01_Spine1" 1
	 "ValveBiped.Bip01_Spine2" 1
	 "ValveBiped.Bip01_Spine4" 1
	 "ValveBiped.Bip01_L_Clavicle" 1
	 "ValveBiped.Bip01_L_UpperArm" 1
	 "ValveBiped.Bip01_L_Forearm" 1
	 "ValveBiped.Bip01_L_Hand" 1
	 "ValveBiped.Bip01_L_Finger3" 1
	 "ValveBiped.Bip01_L_Finger31" 1
	 "ValveBiped.Bip01_L_Finger32" 1
	 "ValveBiped.Bip01_L_Finger2" 1
	 "ValveBiped.Bip01_L_Finger21" 1
	 "ValveBiped.Bip01_L_Finger22" 1
	 "ValveBiped.Bip01_L_Finger1" 1
	 "ValveBiped.Bip01_L_Finger11" 1
	 "ValveBiped.Bip01_L_Finger12" 1
	 "ValveBiped.Bip01_R_Clavicle" 1
	 "ValveBiped.Bip01_R_UpperArm" 1
	 "ValveBiped.Bip01_R_Forearm" 1
	 "ValveBiped.Bip01_R_Hand" 1
	 "ValveBiped.Bip01_R_Finger3" 1
	 "ValveBiped.Bip01_R_Finger31" 1
	 "ValveBiped.Bip01_R_Finger32" 1
	 "ValveBiped.Bip01_R_Finger2" 1
	 "ValveBiped.Bip01_R_Finger21" 1
	 "ValveBiped.Bip01_R_Finger22" 1
	 "ValveBiped.Bip01_R_Finger1" 1
	 "ValveBiped.Bip01_R_Finger11" 1
	 "ValveBiped.Bip01_R_Finger12" 1
	 "ValveBiped.Bip01_R_Finger0" 1
	 "ValveBiped.Bip01_R_Finger01" 1
	 "ValveBiped.Bip01_R_Finger02" 1
	 "ValveBiped.Bip01_L_Finger0" 1
	 "ValveBiped.Bip01_L_Finger01" 1
	 "ValveBiped.Bip01_L_Finger02" 1
	 "ValveBiped.HC_Body_Bone" 1
	 "ValveBiped.HC_Head_Bone" 1
	 "ValveBiped.HC_ThighL_Bone" 1
	 "ValveBiped.HC_CalfL_Bone" 1
	 "ValveBiped.HC_ThighR_Bone" 1
	 "ValveBiped.HC_CalfR_Bone" 1
	 "ValveBiped.HC_UpperArmL_Bone" 1
	 "ValveBiped.HC_ForeArmL_Bone" 1
	 "ValveBiped.HC_UpperArmR_Bone" 1
	 "ValveBiped.HC_ForeArmR_Bone" 1
}

$weightlist "weights_flinch1" {
	 "ValveBiped.Bip01_Pelvis" 0
	 "ValveBiped.Bip01_L_Thigh" 0
	 "ValveBiped.Bip01_L_Calf" 0
	 "ValveBiped.Bip01_L_Foot" 0
	 "ValveBiped.Bip01_L_Toe0" 0
	 "ValveBiped.Bip01_R_Thigh" 0
	 "ValveBiped.Bip01_R_Calf" 0
	 "ValveBiped.Bip01_R_Foot" 0
	 "ValveBiped.Bip01_R_Toe0" 0
	 "ValveBiped.Bip01_Spine" 0
	 "ValveBiped.Bip01_Spine1" 0
	 "ValveBiped.Bip01_Spine2" 0
	 "ValveBiped.Bip01_Spine4" 0
	 "ValveBiped.Bip01_L_Clavicle" 0
	 "ValveBiped.Bip01_L_UpperArm" 0
	 "ValveBiped.Bip01_L_Forearm" 0
	 "ValveBiped.Bip01_L_Hand" 0
	 "ValveBiped.Bip01_L_Finger3" 0
	 "ValveBiped.Bip01_L_Finger31" 0
	 "ValveBiped.Bip01_L_Finger32" 0
	 "ValveBiped.Bip01_L_Finger2" 0
	 "ValveBiped.Bip01_L_Finger21" 0
	 "ValveBiped.Bip01_L_Finger22" 0
	 "ValveBiped.Bip01_L_Finger1" 0
	 "ValveBiped.Bip01_L_Finger11" 0
	 "ValveBiped.Bip01_L_Finger12" 0
	 "ValveBiped.Bip01_R_Clavicle" 0
	 "ValveBiped.Bip01_R_UpperArm" 0
	 "ValveBiped.Bip01_R_Forearm" 0
	 "ValveBiped.Bip01_R_Hand" 0
	 "ValveBiped.Bip01_R_Finger3" 0
	 "ValveBiped.Bip01_R_Finger31" 0
	 "ValveBiped.Bip01_R_Finger32" 0
	 "ValveBiped.Bip01_R_Finger2" 0
	 "ValveBiped.Bip01_R_Finger21" 0
	 "ValveBiped.Bip01_R_Finger22" 0
	 "ValveBiped.Bip01_R_Finger1" 0
	 "ValveBiped.Bip01_R_Finger11" 0
	 "ValveBiped.Bip01_R_Finger12" 0
	 "ValveBiped.Bip01_R_Finger0" 0
	 "ValveBiped.Bip01_R_Finger01" 0
	 "ValveBiped.Bip01_R_Finger02" 0
	 "ValveBiped.Bip01_L_Finger0" 0
	 "ValveBiped.Bip01_L_Finger01" 0
	 "ValveBiped.Bip01_L_Finger02" 0
	 "ValveBiped.HC_Body_Bone" 0
	 "ValveBiped.HC_Head_Bone" 0
	 "ValveBiped.HC_ThighL_Bone" 0
	 "ValveBiped.HC_CalfL_Bone" 0
	 "ValveBiped.HC_ThighR_Bone" 0
	 "ValveBiped.HC_CalfR_Bone" 0
	 "ValveBiped.HC_UpperArmL_Bone" 0
	 "ValveBiped.HC_ForeArmL_Bone" 0
	 "ValveBiped.HC_UpperArmR_Bone" 0
	 "ValveBiped.HC_ForeArmR_Bone" 0
}

$animation "@flinch1inDelta_corrective_animation" "classic_anims\@flinch1inDelta_corrective_animation.smd" {
}

$animation "@flinch1CoreDelta_corrective_animation" "classic_anims\@flinch1CoreDelta_corrective_animation.smd" {
}

$animation "@flinch1outDelta_corrective_animation" "classic_anims\@flinch1outDelta_corrective_animation.smd" {
}

$animation "@flinch2inDelta_corrective_animation" "classic_anims\@flinch2inDelta_corrective_animation.smd" {
}

$animation "@flinch2CoreDelta_corrective_animation" "classic_anims\@flinch2CoreDelta_corrective_animation.smd" {
}

$animation "@flinch2outDelta_corrective_animation" "classic_anims\@flinch2outDelta_corrective_animation.smd" {
}

$animation "@flinch3inDelta_corrective_animation" "classic_anims\@flinch3inDelta_corrective_animation.smd" {
}

$animation "@flinch3CoreDelta_corrective_animation" "classic_anims\@flinch3CoreDelta_corrective_animation.smd" {
}

$animation "@flinch3outDelta_corrective_animation" "classic_anims\@flinch3outDelta_corrective_animation.smd" {
}

$animation "a_walk1" "classic_anims\a_walk1.smd" {
	fps 24
	loop
	ikrule "lfoot" footstep contact 5 fakeorigin 8 5.16 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 44 5 15 21 target 3
	ikrule "lfoot" footstep contact 28 fakeorigin 7.27 7.55 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 21 28 38 44 target 3
	ikrule "rfoot" footstep contact 16 fakeorigin 12.16 -6.61 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 12 16 28 33 target 2
	ikrule "rfoot" footstep contact 39 fakeorigin 14.61 -5.2 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 33 39 8 12 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 46 46 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 46 46 target 1
	walkframe 46 LX LY
}

$animation "a_walk2" "classic_anims\a_walk2.smd" {
	fps 24
	loop
	ikrule "lfoot" footstep contact 7 fakeorigin 16.98 5.2 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 2 7 23 27 target 3
	ikrule "lfoot" footstep contact 31 fakeorigin 12.66 6.61 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 27 31 43 2 target 3
	ikrule "rfoot" footstep contact 21 fakeorigin 7.02 -4.81 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 15 21 31 37 target 2
	ikrule "rfoot" footstep contact 43 fakeorigin 7.77 -7.55 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 37 43 10 15 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 46 46 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 46 46 target 1
	walkframe 46 LX LY
}

$animation "a_walk3" "classic_anims\a_walk3.smd" {
	fps 24
	loop
	ikrule "lfoot" footstep contact 7 fakeorigin 8.74 5.23 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 0 7 21 26 target 3
	ikrule "lfoot" footstep contact 32 fakeorigin 7.57 7.55 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 26 32 43 50 target 3
	ikrule "rfoot" footstep contact 18 fakeorigin 14.11 -6.6 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 14 18 31 35 target 2
	ikrule "rfoot" footstep contact 40 fakeorigin 21.74 -5.2 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 35 40 10 14 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 50 50 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 50 50 target 1
	walkframe 50 LX LY
}

$animation "a_Walk4" "classic_anims\a_Walk4.smd" {
	fps 24
	loop
	ikrule "lfoot" footstep contact 3 fakeorigin 9.39 8.49 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 71 3 10 16 target 3
	ikrule "lfoot" footstep contact 22 fakeorigin 4.91 10.56 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 16 22 29 36 target 3
	ikrule "lfoot" footstep contact 43 fakeorigin 3.6 8.14 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 36 43 50 54 target 3
	ikrule "lfoot" footstep contact 58 fakeorigin 5.79 9.71 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 54 58 67 71 target 3
	ikrule "rfoot" footstep contact 16 fakeorigin 1.08 -5.5 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 12 16 20 24 target 2
	ikrule "rfoot" footstep contact 29 fakeorigin 9.25 -4.8 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 24 29 41 45 target 2
	ikrule "rfoot" footstep contact 50 fakeorigin 8 -5.97 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 45 50 55 61 target 2
	ikrule "rfoot" footstep contact 68 fakeorigin 4.83 -4.09 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 61 68 9 12 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 72 72 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 72 72 target 1
	walkframe 72 LX LY
}

$animation "a_WalkS" "classic_anims\a_WalkS.smd" {
	fps 24
	loop
	ikrule "lfoot" footstep contact 6 fakeorigin -4.07 7.11 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 44 6 15 21 target 3
	ikrule "lfoot" footstep contact 27 fakeorigin -3.43 6.68 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 21 27 37 44 target 3
	ikrule "rfoot" footstep contact 21 fakeorigin -8.4 -8.48 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 13 21 28 32 target 2
	ikrule "rfoot" footstep contact 36 fakeorigin -11.78 -5.5 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 32 36 4 13 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 45 45 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 45 45 target 1
	walkframe 45 LX LY
}

$animation "a_WalkSE" "classic_anims\a_WalkSE.smd" {
	fps 24
	loop
	ikrule "lfoot" footstep contact 6 fakeorigin -5.72 1.69 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 44 6 15 21 target 3
	ikrule "lfoot" footstep contact 27 fakeorigin -0.27 -1.89 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 21 27 37 44 target 3
	ikrule "rfoot" footstep contact 20 fakeorigin -1.07 -17.58 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 12 20 26 32 target 2
	ikrule "rfoot" footstep contact 37 fakeorigin -4.1 -18.74 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 32 37 4 12 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 45 45 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 45 45 target 1
	walkframe 45 LX LY
}

$animation "a_WalkE" "classic_anims\a_WalkE.smd" {
	fps 24
	loop
	ikrule "lfoot" footstep contact 6 fakeorigin 7.38 0.84 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 43 6 14 21 target 3
	ikrule "lfoot" footstep contact 27 fakeorigin 9.64 -5.21 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 21 27 35 43 target 3
	ikrule "rfoot" footstep contact 16 fakeorigin -2.78 -17.76 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 10 16 26 32 target 2
	ikrule "rfoot" footstep contact 38 fakeorigin -2.84 -17.69 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 32 38 4 10 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 45 45 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 45 45 target 1
	walkframe 45 LX LY
}

$animation "a_WalkNE" "classic_anims\a_WalkNE.smd" {
	fps 24
	loop
	ikrule "lfoot" footstep contact 7 fakeorigin 5.48 2.1 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 44 7 15 22 target 3
	ikrule "lfoot" footstep contact 28 fakeorigin 7.97 -2.05 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 22 28 35 44 target 3
	ikrule "rfoot" footstep contact 17 fakeorigin 7.55 -18.55 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 12 17 26 32 target 2
	ikrule "rfoot" footstep contact 38 fakeorigin 16.86 -14.38 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 32 38 6 12 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 45 45 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 45 45 target 1
	walkframe 45 LX LY
}

$animation "a_WalkNW" "classic_anims\a_WalkNW.smd" {
	fps 24
	loop
	ikrule "lfoot" footstep contact 7 fakeorigin 11.1 10.32 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 44 7 15 22 target 3
	ikrule "lfoot" footstep contact 28 fakeorigin 9.28 8.83 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 22 28 35 44 target 3
	ikrule "rfoot" footstep contact 17 fakeorigin 9.27 0.74 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 11 17 26 32 target 2
	ikrule "rfoot" footstep contact 38 fakeorigin 18.52 -1.41 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 32 38 4 11 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 45 45 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 45 45 target 1
	walkframe 45 LX LY
}

$animation "a_WalkW" "classic_anims\a_WalkW.smd" {
	fps 24
	loop
	ikrule "lfoot" footstep contact 7 fakeorigin 7.51 13.77 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 44 7 15 22 target 3
	ikrule "lfoot" footstep contact 28 fakeorigin 6.39 11.88 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 22 28 35 44 target 3
	ikrule "rfoot" footstep contact 17 fakeorigin -1.8 5.26 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 11 17 26 32 target 2
	ikrule "rfoot" footstep contact 38 fakeorigin 0.1 4.78 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 32 38 4 11 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 45 45 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 45 45 target 1
	walkframe 45 LX LY
}

$animation "a_WalkSW" "classic_anims\a_WalkSW.smd" {
	fps 24
	loop
	ikrule "lfoot" footstep contact 7 fakeorigin -1.06 14.64 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 44 7 15 22 target 3
	ikrule "lfoot" footstep contact 28 fakeorigin 6 10.67 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 22 28 35 44 target 3
	ikrule "rfoot" footstep contact 17 fakeorigin -13.59 -0.91 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 11 17 26 32 target 2
	ikrule "rfoot" footstep contact 38 fakeorigin -8.91 -2.5 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 32 38 4 11 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 45 45 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 45 45 target 1
	walkframe 45 LX LY
}

$sequence "Idle01" {
	"classic_anims\Idle01.smd"
	activity "ACT_IDLE" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
	ikrule "lfoot" footstep contact 0 fakeorigin -0.45 7.1 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 53 53 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin 1.83 -10.44 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 53 53 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 53 53 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 53 53 target 1
}

$sequence "walk" {
	"a_WalkS"
	"a_WalkSE"
	"a_WalkE"
	"a_WalkNE"
	"a_walk1"
	"a_WalkNW"
	"a_WalkW"
	"a_WalkSW"
	"a_WalkS"
	activity "ACT_WALK" 1
	blend "move_yaw" -180 180
	blendwidth 9
	{ event AE_ZOMBIE_STEP_LEFT 5 "" }
	{ event AE_ZOMBIE_STEP_RIGHT 15 "" }
	{ event AE_ZOMBIE_SCUFF_LEFT 20 "" }
	{ event AE_ZOMBIE_STEP_LEFT 28 "" }
	{ event AE_ZOMBIE_STEP_RIGHT 38 "" }
	{ event AE_ZOMBIE_SCUFF_LEFT 40 "" }
	fadein 0.2
	fadeout 0.2
}

$sequence "walk2" {
	"a_WalkS"
	"a_WalkSE"
	"a_WalkE"
	"a_WalkNE"
	"a_walk2"
	"a_WalkNW"
	"a_WalkW"
	"a_WalkSW"
	"a_WalkS"
	activity "ACT_WALK" 1
	blend "move_yaw" -180 180
	blendwidth 9
	{ event AE_ZOMBIE_STEP_LEFT 7 "" }
	{ event AE_ZOMBIE_SCUFF_RIGHT 10 "" }
	{ event AE_ZOMBIE_STEP_RIGHT 21 "" }
	{ event AE_ZOMBIE_STEP_LEFT 31 "" }
	{ event AE_ZOMBIE_SCUFF_RIGHT 34 "" }
	{ event AE_ZOMBIE_STEP_RIGHT 44 "" }
	fadein 0.2
	fadeout 0.2
}

$sequence "walk3" {
	"a_WalkS"
	"a_WalkSE"
	"a_WalkE"
	"a_WalkNE"
	"a_walk3"
	"a_WalkNW"
	"a_WalkW"
	"a_WalkSW"
	"a_WalkS"
	activity "ACT_WALK" 1
	blend "move_yaw" -180 180
	blendwidth 9
	{ event AE_ZOMBIE_STEP_LEFT 7 "" }
	{ event AE_ZOMBIE_STEP_RIGHT 18 "" }
	{ event AE_ZOMBIE_SCUFF_LEFT 21 "" }
	{ event AE_ZOMBIE_STEP_LEFT 32 "" }
	{ event AE_ZOMBIE_STEP_RIGHT 40 "" }
	{ event AE_ZOMBIE_SCUFF_LEFT 44 "" }
	fadein 0.2
	fadeout 0.2
}

$sequence "walk4" {
	"a_Walk4"
	activity "ACT_WALK" 1
	{ event AE_ZOMBIE_STEP_LEFT 4 "" }
	{ event AE_ZOMBIE_STEP_RIGHT 16 "" }
	{ event AE_ZOMBIE_STEP_LEFT 22 "" }
	{ event AE_ZOMBIE_STEP_RIGHT 28 "" }
	{ event AE_ZOMBIE_STEP_LEFT 43 "" }
	{ event AE_ZOMBIE_STEP_RIGHT 49 "" }
	{ event AE_ZOMBIE_STEP_LEFT 58 "" }
	{ event AE_ZOMBIE_STEP_RIGHT 67 "" }
	fadein 0.2
	fadeout 0.2
}

$sequence "FireWalk" {
	"classic_anims\FireWalk.smd"
	activity "ACT_WALK_ON_FIRE" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
	ikrule "lfoot" footstep contact 2 fakeorigin 6.31 8.8 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 25 2 18 25 target 3 usesequence 
	ikrule "rfoot" footstep contact 17 fakeorigin 5.16 -6.5 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 10 17 4 10 target 2 usesequence 
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0 usesequence 
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1 usesequence 
	walkframe 30 LX LY
}

$sequence "FireIdle" {
	"classic_anims\FireIdle.smd"
	activity "ACT_IDLE_ON_FIRE" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
	ikrule "lfoot" footstep contact 0 fakeorigin -0.71 0.77 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 30 30 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin 1.56 -16.77 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 30 30 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1
}

$sequence "attackA" {
	"classic_anims\attackA.smd"
	activity "ACT_MELEE_ATTACK1" 2
	{ event AE_ZOMBIE_ATTACK_SCREAM 5 "" }
	{ event AE_ZOMBIE_ATTACK_RIGHT 25 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -0.45 6.89 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 0 0 22 34 target 3 usesequence 
	ikrule "lfoot" footstep contact 46 fakeorigin 0.26 5.74 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 34 46 56 56 target 3 usesequence 
	ikrule "rfoot" footstep contact 0 fakeorigin 1.83 -10.64 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 0 0 22 29 target 2 usesequence 
	ikrule "rfoot" footstep contact 36 fakeorigin 4.77 -10.44 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 29 36 56 56 target 2 usesequence 
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 56 56 target 0 usesequence 
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 56 56 target 1 usesequence 
	walkframe 47 LX LY
}

$sequence "attackB" {
	"classic_anims\attackB.smd"
	activity "ACT_MELEE_ATTACK1" 2
	{ event AE_ZOMBIE_ATTACK_SCREAM 5 "" }
	{ event AE_ZOMBIE_ATTACK_RIGHT 25 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -0.45 6.89 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 56 56 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin 1.83 -10.64 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 56 56 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 56 56 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 56 56 target 1
}

$sequence "attackC" {
	"classic_anims\attackC.smd"
	activity "ACT_MELEE_ATTACK1" 2
	{ event AE_ZOMBIE_ATTACK_SCREAM 5 "" }
	{ event AE_ZOMBIE_ATTACK_LEFT 25 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -0.92 6.83 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 0 0 22 28 target 3 usesequence 
	ikrule "lfoot" footstep contact 34 fakeorigin 3.47 7.06 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 28 34 56 56 target 3 usesequence 
	ikrule "rfoot" footstep contact 0 fakeorigin 1.72 -11.24 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 0 0 22 34 target 2 usesequence 
	ikrule "rfoot" footstep contact 46 fakeorigin 1.83 -10.42 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 34 46 56 56 target 2 usesequence 
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 56 56 target 0 usesequence 
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 56 56 target 1 usesequence 
	walkframe 47 LX LY
}

$sequence "attackD" {
	"classic_anims\attackD.smd"
	activity "ACT_MELEE_ATTACK1" 2
	{ event AE_ZOMBIE_ATTACK_SCREAM 5 "" }
	{ event AE_ZOMBIE_ATTACK_LEFT 25 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -0.5 6.83 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 56 56 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin 1.84 -10.74 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 56 56 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 56 56 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 56 56 target 1
}

$sequence "attackE" {
	"classic_anims\attackE.smd"
	activity "ACT_MELEE_ATTACK1" 2
	{ event AE_ZOMBIE_ATTACK_SCREAM 5 "" }
	{ event AE_ZOMBIE_ATTACK_BOTH 25 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -0.5 6.83 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 0 0 41 41 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin 1.84 -10.74 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 0 0 6 15 target 2
	ikrule "rfoot" footstep contact 23 fakeorigin 13.99 -14.48 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 15 23 32 35 target 2
	ikrule "rfoot" footstep contact 38 fakeorigin 1.84 -10.74 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 35 38 41 41 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 41 41 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 41 41 target 1
}

$sequence "attackF" {
	"classic_anims\attackF.smd"
	activity "ACT_MELEE_ATTACK1" 2
	{ event AE_ZOMBIE_ATTACK_SCREAM 5 "" }
	{ event AE_ZOMBIE_ATTACK_BOTH 25 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -0.77 6.81 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 41 41 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin 1.98 -10.7 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 41 41 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 41 41 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 41 41 target 1
}

$sequence "swatleftmid" {
	"classic_anims\swatleftmid.smd"
	activity "ACT_ZOM_SWATLEFTMID" 1
	{ event AE_ZOMBIE_STARTSWAT 1 "" }
	{ event AE_ZOMBIE_SWATITEM 9 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -0.5 6.83 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 36 36 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin 1.84 -10.74 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 36 36 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 36 36 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 36 36 target 1
}

$sequence "swatrightmid" {
	"classic_anims\swatrightmid.smd"
	activity "ACT_ZOM_SWATRIGHTMID" 1
	{ event AE_ZOMBIE_STARTSWAT 1 "" }
	{ event AE_ZOMBIE_SWATITEM 9 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -0.45 6.89 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 36 36 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin 1.83 -10.64 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 36 36 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 36 36 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 36 36 target 1
}

$sequence "swatleftlow" {
	"classic_anims\swatleftlow.smd"
	activity "ACT_ZOM_SWATLEFTLOW" 1
	{ event AE_ZOMBIE_STARTSWAT 1 "" }
	{ event AE_ZOMBIE_SWATITEM 15 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -0.92 6.83 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 0 0 17 22 target 3 usesequence 
	ikrule "lfoot" footstep contact 26 fakeorigin 2.29 5.37 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 22 26 45 45 target 3 usesequence 
	ikrule "rfoot" footstep contact 0 fakeorigin 1.72 -11.24 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 0 0 17 28 target 2 usesequence 
	ikrule "rfoot" footstep contact 39 fakeorigin 1.79 -11.08 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 28 39 45 45 target 2 usesequence 
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 45 45 target 0 usesequence 
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 45 45 target 1 usesequence 
	walkframe 36 LX LY
}

$sequence "swatrightlow" {
	"classic_anims\swatrightlow.smd"
	activity "ACT_ZOM_SWATRIGHTLOW" 1
	{ event AE_ZOMBIE_STARTSWAT 1 "" }
	{ event AE_ZOMBIE_SWATITEM 18 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -0.45 6.89 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 0 0 15 27 target 3 usesequence 
	ikrule "lfoot" footstep contact 38 fakeorigin 0.03 5.78 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 27 38 47 47 target 3 usesequence 
	ikrule "rfoot" footstep contact 0 fakeorigin 1.83 -10.64 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 0 0 23 26 target 2 usesequence 
	ikrule "rfoot" footstep contact 29 fakeorigin 4.62 -10.41 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 26 29 47 47 target 2 usesequence 
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 47 47 target 0 usesequence 
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 47 47 target 1 usesequence 
	walkframe 38 LX LY
}

$sequence "releasecrab" {
	"classic_anims\releasecrab.smd"
	activity "ACT_ZOM_RELEASECRAB" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -0.45 6.89 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 30 30 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin 1.83 -10.64 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 30 30 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1
}

$sequence "physflinch1" {
	"classic_anims\physflinch1.smd"
	activity "ACT_FLINCH_PHYSICS" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -0.29 6.81 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 18 18 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin 1.92 -10.39 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 18 18 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 18 18 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 18 18 target 1
}

$sequence "physflinch2" {
	"classic_anims\physflinch2.smd"
	activity "ACT_FLINCH_PHYSICS" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -0.45 7.1 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 20 20 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin 1.83 -10.44 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 20 20 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 1
}

$sequence "physflinch3" {
	"classic_anims\physflinch3.smd"
	activity "ACT_FLINCH_PHYSICS" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -0.45 7.1 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 20 20 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin 1.83 -10.44 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 20 20 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 1
}

$sequence "smashfall64" {
	"classic_anims\smashfall64.smd"
	{ event AE_NPC_BODYDROP_HEAVY 10 "" }
	{ event AE_NPC_BODYDROP_HEAVY 27 "" }
	{ event AE_NPC_BODYDROP_HEAVY 37 "" }
	{ event AE_ZOMBIE_GET_UP 75 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 120 120 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 120 120 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 120 120 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 120 120 target 3
}

$sequence "slump_a" {
	"classic_anims\slump_a.smd"
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 2 2 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 2 2 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 2 2 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 2 2 target 3
}

$sequence "slumprise_a" {
	"classic_anims\slumprise_a.smd"
	{ event AE_NPC_LEFTFOOT 15 "" }
	{ event AE_NPC_RIGHTFOOT 50 "" }
	{ event AE_ZOMBIE_GET_UP 80 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 127 127 target 0 usesequence 
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 127 127 target 1 usesequence 
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 127 127 target 2 usesequence 
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 127 127 target 3 usesequence 
	walkframe 68
	walkframe 127 LX LY
}

$sequence "slumprise_a_attack" {
	"classic_anims\slumprise_a_attack.smd"
	{ event AE_NPC_LEFTFOOT 15 "" }
	{ event AE_NPC_RIGHTFOOT 50 "" }
	{ event AE_ZOMBIE_GET_UP 80 "" }
	{ event AE_ZOMBIE_ATTACK_SCREAM 16 "" }
	{ event AE_ZOMBIE_ATTACK_RIGHT 36 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 127 127 target 0 usesequence 
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 127 127 target 1 usesequence 
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 127 127 target 2 usesequence 
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 127 127 target 3 usesequence 
	walkframe 68
	walkframe 127 LX LY
}

$sequence "slumprise_a2" {
	"classic_anims\slumprise_a2.smd"
	{ event AE_NPC_RIGHTFOOT 16 "" }
	{ event AE_NPC_RIGHTFOOT 22 "" }
	{ event AE_ZOMBIE_GET_UP 24 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 68 68 target 0 usesequence 
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 68 68 target 1 usesequence 
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 68 68 target 2 usesequence 
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 68 68 target 3 usesequence 
	walkframe 68 LX LY
}

$sequence "slump_b" {
	"classic_anims\slump_b.smd"
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 2 2 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 2 2 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 2 2 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 2 2 target 3
}

$sequence "slumprise_b" {
	"classic_anims\slumprise_b.smd"
	{ event AE_NPC_LEFTFOOT 13 "" }
	{ event AE_NPC_RIGHTFOOT 27 "" }
	{ event AE_ZOMBIE_GET_UP 57 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 101 101 target 0 usesequence 
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 101 101 target 1 usesequence 
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 101 101 target 2 usesequence 
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 101 101 target 3 usesequence 
	walkframe 101 LX LY
}

$sequence "Breakthrough" {
	"classic_anims\Breakthrough.smd"
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 63 63 target 0 usesequence 
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 63 63 target 1 usesequence 
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 63 63 target 2 usesequence 
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 63 63 target 3 usesequence 
	walkframe 54 LX LY
}

$sequence "canal5await" {
	"classic_anims\canal5await.smd"
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 3
}

$sequence "canal5aattack" {
	"classic_anims\canal5aattack.smd"
	fadein 0.2
	fadeout 0.2
	snap
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 3
}

$sequence "flinch1inDelta" {
	"classic_anims\flinch1inDelta.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@flinch1inDelta_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	weightlist "weights_flinch1inDelta"
}

$sequence "flinch1inFrame" {
	"classic_anims\flinch1inFrame.smd"
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	weightlist "weights_flinch1inDelta"
}

$sequence "flinch1CoreDelta" {
	"classic_anims\flinch1CoreDelta.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@flinch1CoreDelta_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	weightlist "weights_flinch1inDelta"
}

$sequence "flinch1OutFrame" {
	"classic_anims\flinch1OutFrame.smd"
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	weightlist "weights_flinch1inDelta"
}

$sequence "flinch1outDelta" {
	"classic_anims\flinch1outDelta.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@flinch1outDelta_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	weightlist "weights_flinch1inDelta"
}

$sequence "flinch1" {
	"classic_anims\flinch1.smd"
	activity "ACT_GESTURE_FLINCH_HEAD" 1
	fadein 0.2
	fadeout 0.2
	iklock "rfoot" 1 0
	iklock "lfoot" 1 0
	blendlayer "flinch1inFrame" 0 8 8 12 xfade spline
	blendlayer "flinch1OutFrame" 8 12 12 18 spline
	blendlayer "flinch1inDelta" 0 0 8 8
	blendlayer "flinch1CoreDelta" 8 8 12 12
	blendlayer "flinch1outDelta" 12 12 18 18
	fps 30
	weightlist "weights_flinch1"
}

$sequence "flinch2inDelta" {
	"classic_anims\flinch2inDelta.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@flinch2inDelta_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	weightlist "weights_flinch1inDelta"
}

$sequence "flinch2inFrame" {
	"classic_anims\flinch2inFrame.smd"
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	weightlist "weights_flinch1inDelta"
}

$sequence "flinch2CoreDelta" {
	"classic_anims\flinch2CoreDelta.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@flinch2CoreDelta_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	weightlist "weights_flinch1inDelta"
}

$sequence "flinch2OutFrame" {
	"classic_anims\flinch2OutFrame.smd"
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	weightlist "weights_flinch1inDelta"
}

$sequence "flinch2outDelta" {
	"classic_anims\flinch2outDelta.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@flinch2outDelta_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	weightlist "weights_flinch1inDelta"
}

$sequence "flinch2" {
	"classic_anims\flinch2.smd"
	activity "ACT_GESTURE_FLINCH_HEAD" 1
	fadein 0.2
	fadeout 0.2
	iklock "rfoot" 1 0
	iklock "lfoot" 1 0
	blendlayer "flinch2inFrame" 0 7 7 12 xfade spline
	blendlayer "flinch2OutFrame" 7 12 12 20 spline
	blendlayer "flinch2inDelta" 0 0 7 7
	blendlayer "flinch2CoreDelta" 7 7 12 12
	blendlayer "flinch2outDelta" 12 12 20 20
	fps 30
	weightlist "weights_flinch1"
}

$sequence "flinch3inDelta" {
	"classic_anims\flinch3inDelta.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@flinch3inDelta_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	weightlist "weights_flinch1inDelta"
}

$sequence "flinch3inFrame" {
	"classic_anims\flinch3inFrame.smd"
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	weightlist "weights_flinch1inDelta"
}

$sequence "flinch3CoreDelta" {
	"classic_anims\flinch3CoreDelta.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@flinch3CoreDelta_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	weightlist "weights_flinch1inDelta"
}

$sequence "flinch3OutFrame" {
	"classic_anims\flinch3OutFrame.smd"
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	weightlist "weights_flinch1inDelta"
}

$sequence "flinch3outDelta" {
	"classic_anims\flinch3outDelta.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@flinch3outDelta_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	weightlist "weights_flinch1inDelta"
}

$sequence "flinch3" {
	"classic_anims\flinch3.smd"
	activity "ACT_GESTURE_FLINCH_HEAD" 1
	fadein 0.2
	fadeout 0.2
	iklock "rfoot" 1 0
	iklock "lfoot" 1 0
	blendlayer "flinch3inFrame" 0 10 10 12 xfade spline
	blendlayer "flinch3OutFrame" 10 12 12 20 spline
	blendlayer "flinch3inDelta" 0 0 10 10
	blendlayer "flinch3CoreDelta" 10 10 12 12
	blendlayer "flinch3outDelta" 12 12 20 20
	fps 30
	weightlist "weights_flinch1"
}

$sequence "Tantrum" {
	"classic_anims\Tantrum.smd"
	activity "ACT_ZOMBIE_TANTRUM" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 36 36 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 36 36 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 36 36 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 36 36 target 3
}

$sequence "WallPound" {
	"classic_anims\WallPound.smd"
	activity "ACT_ZOMBIE_WALLPOUND" 1
	{ event AE_ZOMBIE_POUND 14 "" }
	{ event AE_ZOMBIE_POUND 35 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 53 53 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 53 53 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 53 53 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 53 53 target 3
}

$sequence "ragdoll" {
	"classic_anims\ragdoll.smd"
	activity "ACT_DIERAGDOLL" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 1 1 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 1 1 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 1 1 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 1 1 target 3
}

$collisionjoints "classic_physics.smd"
{
	$mass 100
	$inertia 10
	$damping 0.1
	$rotdamping 3
	$rootbone "valvebiped.bip01_pelvis"

	$jointconstrain "ValveBiped.Bip01_Spine2" x limit -44 44 1
	$jointconstrain "ValveBiped.Bip01_Spine2" y limit -39 39 1
	$jointconstrain "ValveBiped.Bip01_Spine2" z limit -32 50 1

	$jointconstrain "ValveBiped.Bip01_R_Clavicle" x limit -30 30 1
	$jointconstrain "ValveBiped.Bip01_R_Clavicle" y limit -46 46 1
	$jointconstrain "ValveBiped.Bip01_R_Clavicle" z limit -26 48 1

	$jointconstrain "ValveBiped.Bip01_R_UpperArm" x limit -39 39 1
	$jointconstrain "ValveBiped.Bip01_R_UpperArm" y limit -79 95 1
	$jointconstrain "ValveBiped.Bip01_R_UpperArm" z limit -93 28 1

	$jointconstrain "ValveBiped.Bip01_L_Clavicle" x limit -30 30 1
	$jointconstrain "ValveBiped.Bip01_L_Clavicle" y limit -46 46 1
	$jointconstrain "ValveBiped.Bip01_L_Clavicle" z limit -26 48 1

	$jointconstrain "ValveBiped.Bip01_L_UpperArm" x limit -30 30 1
	$jointconstrain "ValveBiped.Bip01_L_UpperArm" y limit -95 84 1
	$jointconstrain "ValveBiped.Bip01_L_UpperArm" z limit -86 26 1

	$jointconstrain "ValveBiped.Bip01_L_Forearm" x limit 0 0 1
	$jointconstrain "ValveBiped.Bip01_L_Forearm" y limit 0 0 1
	$jointconstrain "ValveBiped.Bip01_L_Forearm" z limit -149 4 1

	$jointconstrain "ValveBiped.Bip01_L_Hand" x limit -55 55 1
	$jointconstrain "ValveBiped.Bip01_L_Hand" y limit -55 55 1
	$jointconstrain "ValveBiped.Bip01_L_Hand" z limit -55 55 1

	$jointconstrain "ValveBiped.Bip01_R_Forearm" x limit 0 0 1
	$jointconstrain "ValveBiped.Bip01_R_Forearm" y limit 0 0 1
	$jointconstrain "ValveBiped.Bip01_R_Forearm" z limit -149 4 1

	$jointconstrain "ValveBiped.Bip01_R_Thigh" x limit -12 12 1
	$jointconstrain "ValveBiped.Bip01_R_Thigh" y limit -8 75 1
	$jointconstrain "ValveBiped.Bip01_R_Thigh" z limit -97 32 1

	$jointconstrain "ValveBiped.Bip01_R_Calf" x limit 0 0 1
	$jointconstrain "ValveBiped.Bip01_R_Calf" y limit 0 0 1
	$jointconstrain "ValveBiped.Bip01_R_Calf" z limit -12 126 1

	$jointconstrain "ValveBiped.Bip01_R_Foot" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Foot" y limit -25 6 1
	$jointconstrain "ValveBiped.Bip01_R_Foot" z limit -15 35 1

	$jointconstrain "ValveBiped.Bip01_L_Thigh" x limit -12 12 1
	$jointconstrain "ValveBiped.Bip01_L_Thigh" y limit -73 6 1
	$jointconstrain "ValveBiped.Bip01_L_Thigh" z limit -93 30 1

	$jointconstrain "ValveBiped.Bip01_L_Calf" x limit 0 0 1
	$jointconstrain "ValveBiped.Bip01_L_Calf" y limit 0 0 1
	$jointconstrain "ValveBiped.Bip01_L_Calf" z limit -8 126 1

	$jointconstrain "ValveBiped.Bip01_L_Foot" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Foot" y limit -19 19 1
	$jointconstrain "ValveBiped.Bip01_L_Foot" z limit -15 35 1

	$jointconstrain "ValveBiped.Bip01_R_Hand" x limit -55 55 1
	$jointconstrain "ValveBiped.Bip01_R_Hand" y limit -55 55 1
	$jointconstrain "ValveBiped.Bip01_R_Hand" z limit -55 55 1
}