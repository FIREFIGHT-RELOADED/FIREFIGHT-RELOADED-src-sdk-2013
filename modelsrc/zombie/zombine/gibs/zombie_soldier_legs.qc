// Created by Crowbar 0.72

$modelname "Zombie/zombie_soldier_legs.mdl"
$body studio "Zombie_soldier_legs_reference.smd"

$surfaceprop "zombieflesh"

$illumposition 0.63 2.653 19.042

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

$bbox -12.89 -8.353 -5.461 7.584 9.613 43.545

$hboxset "default"
$hbox 6 "ValveBiped.Bip01_L_Thigh" 0 -4 -3.73 17.25 4 4.29
$hbox 6 "ValveBiped.Bip01_L_Calf" 0 -4.87 -3.54 17.25 3.18 2.63
$hbox 6 "ValveBiped.Bip01_L_Foot" -1 -5 -3.38 3.98 8 2.63
$hbox 7 "ValveBiped.Bip01_R_Thigh" 0 -4 -3.73 17.25 4 4.29
$hbox 7 "ValveBiped.Bip01_R_Calf" 0 -4.92 -3.12 17.25 3.25 2.65
$hbox 7 "ValveBiped.Bip01_R_Foot" -1 -5 -3.38 3.98 8 2.63

$definebone "ValveBiped.Bip01" "" -0.000005 1.515989 38.353996 0 0 90.000037 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Pelvis" "ValveBiped.Bip01" 0 -0.000001 0 -0.000057 0 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Thigh" "ValveBiped.Bip01_Pelvis" 3.890453 -0.000001 -0.000004 -6.298009 -90.530886 -89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Calf" "ValveBiped.Bip01_L_Thigh" 17.593168 0 0 0 16.008157 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Foot" "ValveBiped.Bip01_L_Calf" 17.699579 0 0 -2.794963 -66.211956 -0.906613 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Toe0" "ValveBiped.Bip01_L_Foot" 6.879456 0 0 2.831041 -33.495514 -3.654004 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Thigh" "ValveBiped.Bip01_Pelvis" -3.890452 -0.000001 0.000007 -6.255897 -90.309895 -89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Calf" "ValveBiped.Bip01_R_Thigh" 17.593164 0 0 0 16.011995 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Foot" "ValveBiped.Bip01_R_Calf" 17.699577 0 0 2.33062 -66.226587 2.220957 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Toe0" "ValveBiped.Bip01_R_Foot" 6.879456 0 0 -3.168915 -33.46875 -1.473704 0 0 0 0 0 0
$definebone "ValveBiped.Cod_Bone" "ValveBiped.Bip01_Pelvis" 0.003197 1.916844 7.635197 0 0 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine" "ValveBiped.Bip01" 0.000005 3.345117 -2.981992 4.944339 89.999982 89.999927 0 0 0 0 0 0


$sequence "ragdoll" {
	"zombie_soldier_legs_anims\ragdoll.smd"
	activity "ACT_DIERAGDOLL" 1
}

$collisionjoints "zombie_soldier_legs_physics.smd"
{
	$mass 40
	$inertia 5
	$damping 0.01
	$rotdamping 1.5
	$rootbone "valvebiped.bip01_pelvis"


	$jointconstrain "ValveBiped.Bip01_R_Thigh" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Thigh" y limit -10 12 1
	$jointconstrain "ValveBiped.Bip01_R_Thigh" z limit -35 55 1

	$jointconstrain "ValveBiped.Bip01_R_Calf" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Calf" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Calf" z limit -8 102 1

	$jointconstrain "ValveBiped.Bip01_R_Foot" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Foot" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Foot" z limit -15 35 1

	$jointconstrain "ValveBiped.Bip01_L_Thigh" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Thigh" y limit -10 12 1
	$jointconstrain "ValveBiped.Bip01_L_Thigh" z limit -35 55 1

	$jointconstrain "ValveBiped.Bip01_L_Calf" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Calf" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Calf" z limit -8 102 1

	$jointconstrain "ValveBiped.Bip01_L_Foot" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Foot" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Foot" z limit -15 35 1

	$jointconstrain "ValveBiped.Cod_Bone" x limit -70 10 1
	$jointconstrain "ValveBiped.Cod_Bone" y limit 0 0 0
	$jointconstrain "ValveBiped.Cod_Bone" z limit 0 0 0
}