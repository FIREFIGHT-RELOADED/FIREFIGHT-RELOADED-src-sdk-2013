// Created by Crowbar 0.72

$modelname "Zombie/zombie_soldier_torso.mdl"
$body studio "Zombie_soldier_torso_reference.smd"

$surfaceprop "zombieflesh"

$illumposition 0.487 2.7 47.355

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

$cbox 0 0 0 0 0 0

$bbox -12 -12 0 12 12 24

$hboxset "default"
$hbox 2 "ValveBiped.Bip01_Spine1" -8 -8.91 -7.12 12.5 4.77 7.73
$hbox 4 "ValveBiped.Bip01_L_UpperArm" 0 -2 -3.05 15.16 5.38 2.23
$hbox 4 "ValveBiped.Bip01_L_Forearm" -1.05 -1.86 -1.9 18.530001 3.11 2.36
$hbox 4 "ValveBiped.Bip01_L_Hand" 0 -1.78 -2 14 1.23 3
$hbox 5 "ValveBiped.Bip01_R_UpperArm" 0 -1.95 -2.25 14.86 4.75 3.7
$hbox 5 "ValveBiped.Bip01_R_Forearm" -0.02 -1.89 -1.6 18.530001 3.09 2.66
$hbox 5 "ValveBiped.Bip01_R_Hand" 0 -1.78 -2 14 1.23 3

$definebone "ValveBiped.Bip01" "" -0.000005 1.515989 38.353996 0 0 90.000037 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Pelvis" "ValveBiped.Bip01" 0 -0.000001 0 -0.000057 0 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Thigh" "ValveBiped.Bip01_Pelvis" 3.890453 -0.000001 -0.000004 -6.298009 -90.530886 -89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Thigh" "ValveBiped.Bip01_Pelvis" -3.890452 -0.000001 0.000007 -6.255897 -90.309895 -89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine" "ValveBiped.Bip01" 0.000005 3.345117 -2.981992 4.944339 89.999982 89.999927 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine1" "ValveBiped.Bip01_Spine" 4.126705 0 0 0 5.074516 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine2" "ValveBiped.Bip01_Spine1" 3.613468 0 0 0 7.43745 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine4" "ValveBiped.Bip01_Spine2" 9.183786 0 0 0 20.151327 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Clavicle" "ValveBiped.Bip01_Spine4" 1.673226 1.247213 2.595464 -73.711914 168.587701 91.510847 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_UpperArm" "ValveBiped.Bip01_L_Clavicle" 6.028144 0.000002 0 -12.91292 -32.801775 -101.833749 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_L_UpperArm" 11.692552 0 -0.000002 -0.000001 0.000001 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_L_Forearm" 12.075578 0 -0.000001 -7.742148 1.798745 -178.061242 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger0" "ValveBiped.Bip01_L_Hand" 1.782818 1.657451 0.069279 -48.216464 72.036783 -69.320608 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Clavicle" "ValveBiped.Bip01_Spine4" 1.673228 1.247218 -2.595471 73.711914 168.587141 -95.626931 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_UpperArm" "ValveBiped.Bip01_R_Clavicle" 6.028143 -0.000001 -0.000001 15.946161 -31.150741 105.625228 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_R_UpperArm" 11.692555 0 0 0.000002 0.000001 0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Hand" "ValveBiped.Bip01_R_Forearm" 12.075577 0 -0.000002 2.648442 2.781883 -87.398179 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger0" "ValveBiped.Bip01_R_Hand" 1.819736 -0.264272 -1.112211 39.081058 -74.64918 -87.270126 0 0 0 0 0 0


$sequence "ragdoll" {
	"zombie_soldier_torso_anims\ragdoll.smd"
	activity "ACT_DIERAGDOLL" 1
}

$collisionjoints "zombie_soldier_torso_physics.smd"
{
	$mass 40
	$inertia 5
	$damping 0.01
	$rotdamping 1.5
	$rootbone "ValveBiped.Bip01_Pelvis"


	$jointconstrain "ValveBiped.Bip01_Spine" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_Spine" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_Spine" z limit 0 0 0

	$jointconstrain "ValveBiped.Bip01_Spine1" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_Spine1" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_Spine1" z limit 0 0 0

	$jointconstrain "ValveBiped.Bip01_Spine2" x limit -30 30 1
	$jointconstrain "ValveBiped.Bip01_Spine2" y limit -41 41 1
	$jointconstrain "ValveBiped.Bip01_Spine2" z limit -53 53 1

	$jointconstrain "ValveBiped.Bip01_Spine4" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_Spine4" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_Spine4" z limit 0 0 0

	$jointmassbias "ValveBiped.Bip01_L_UpperArm" 2
	$jointconstrain "ValveBiped.Bip01_L_UpperArm" x limit -17 17 1
	$jointconstrain "ValveBiped.Bip01_L_UpperArm" y limit -45 30 1
	$jointconstrain "ValveBiped.Bip01_L_UpperArm" z limit -73 40 1

	$jointmassbias "ValveBiped.Bip01_R_UpperArm" 2
	$jointconstrain "ValveBiped.Bip01_R_UpperArm" x limit -28 28 1
	$jointconstrain "ValveBiped.Bip01_R_UpperArm" y limit -30 59 1
	$jointconstrain "ValveBiped.Bip01_R_UpperArm" z limit -73 40 1

	$jointmassbias "ValveBiped.Bip01_L_Forearm" 3
	$jointconstrain "ValveBiped.Bip01_L_Forearm" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Forearm" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Forearm" z limit -120 4 1

	$jointmassbias "ValveBiped.Bip01_R_Forearm" 3
	$jointconstrain "ValveBiped.Bip01_R_Forearm" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Forearm" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Forearm" z limit -100 35 1

	$jointconstrain "ValveBiped.Bip01_L_Hand" x limit -32 32 1
	$jointconstrain "ValveBiped.Bip01_L_Hand" y limit -19 19 1
	$jointconstrain "ValveBiped.Bip01_L_Hand" z limit -44 44 1

	$jointconstrain "ValveBiped.Bip01_R_Hand" x limit -23 23 1
	$jointconstrain "ValveBiped.Bip01_R_Hand" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Hand" z limit -50 50 1
}