// Created by Crowbar 0.72

$modelname "gibs/soldier_elite_torso.mdl"
$body studio "soldier_elite_torso_reference.smd"

$surfaceprop "flesh"

$illumposition -7.093 -1.964 7.809

$cdmaterials "models\Combine_Soldier\"
$cdmaterials "models\zombie_classic\"

$cbox 0 0 0 0 0 0

$bbox -10.369 -23.291 -9.185 14.302 9.109 24.798

$definebone "ValveBiped.Bip01_Pelvis" "" -0.000005 -0.533615 38.566917 0 0 89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine" "ValveBiped.Bip01_Pelvis" 0.000005 3.345127 -2.981901 4.944225 89.999982 89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine1" "ValveBiped.Bip01_Spine" 4.01833 0 0 0 -1.675443 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine2" "ValveBiped.Bip01_Spine1" 3.518559 0 0 0 5.74883 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine4" "ValveBiped.Bip01_Spine2" 8.942638 0 0 0 11.120882 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Clavicle" "ValveBiped.Bip01_Spine4" 2.033356 1.000772 -1.93761 73.71205 168.618792 -95.596516 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_UpperArm" "ValveBiped.Bip01_R_Clavicle" 6.028141 0.000011 0 -0.46209 -33.623806 93.935236 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_R_UpperArm" 11.692543 0 0 -0.000012 -3.45906 0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Hand" "ValveBiped.Bip01_R_Forearm" 11.481699 0 0.000004 6.108065 2.532073 -89.664285 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger0" "ValveBiped.Bip01_R_Hand" 0.806017 -0.359837 -1.318333 38.595469 -45.49033 68.995579 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0" 1.789782 -0.000002 0.000004 -0.361025 13.095237 0.000007 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01" 1.207001 0 0.000002 -0.551078 20.792928 0.000005 0 0 0 0 0 0


$sequence "idle" {
	"soldier_elite_torso_anims\idle.smd"
	fadein 0.2
	fadeout 0.2
	fps 30
}

$sequence "ragdoll" {
	"soldier_elite_torso_anims\ragdoll.smd"
	activity "ACT_DIERAGDOLL" 1
	fadein 0.2
	fadeout 0.2
	fps 30
}

$collisionjoints "soldier_elite_torso_physics.smd"
{
	$mass 40
	$inertia 4
	$damping 0.2
	$rotdamping 0.2
	$rootbone "ValveBiped.bip01_pelvis"


	$jointconstrain "ValveBiped.Bip01_Spine2" x limit -48 48 0.04
	$jointconstrain "ValveBiped.Bip01_Spine2" y limit -25 25 0.04
	$jointconstrain "ValveBiped.Bip01_Spine2" z limit -25 50 0.04
}