// Created by Crowbar 0.72

$modelname "weapons/w_oicw_remake.mdl"

$bodygroup "studio"
{
	studio "w_ociw_remake_ref.smd"
}

$surfaceprop "weapon"

$contents "solid"

$illumposition 3.659 0.207 0.766

$cdmaterials "models\Weapons\oicw\"

$attachment "muzzle" "ValveBiped.muzzle" 0 0 0 rotate 0 0 -90
$attachment "1" "ValveBiped.w_shotgun" 1 0 -3 rotate 0 0 0

$cbox 0 0 0 0 0 0

$bbox -18.468 -4.688 -11.284 25.777 2.991 12.815

$hboxset "default"
$hbox 0 "ValveBiped.Bip01_R_Hand" 0 -1.5 -5 5 0 0
$hbox 0 "ValveBiped.Weapon_bone" -13.299 0 -5.976 28.58 2.31 9.364

$definebone "ValveBiped.Bip01_R_Hand" "" 0.312503 -16.71932 0.282627 12.461602 88.326429 177.528802 0 0 0 0 0 0
$definebone "ValveBiped.Weapon_bone" "ValveBiped.Bip01_R_Hand" 5 -1.5 -5.000001 -0.000001 0 0 0 0 0 0 0 0
$definebone "ValveBiped.w_shotgun" "ValveBiped.Weapon_bone" 4.107145 1.396073 -0.366386 2.167786 88.95456 -77.075714 0 0 0 0 0 0
$definebone "ValveBiped.muzzle" "ValveBiped.Weapon_bone" 28.579779 0.623145 -5.976273 12.956009 -0.38921 -91.965868 0 0 0 0 0 0


$sequence "idle1" {
	"w_anims\idle1.smd"
	activity "ACT_VM_IDLE" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
}

$sequence "fire" {
	"w_anims\fire.smd"
	activity "ACT_RANGE_ATTACK_AR2" 1
	{ event AE_NPC_MUZZLEFLASH 0 "SMG1 MUZZLE" }
	{ event 3007 0 "1" }
	{ event 6001 0 "1" }
	fadein 0.2
	fadeout 0.2
	fps 30
}

$collisionmodel "w_ociw_remake_ref.smd"
{
	$mass 15
	$inertia 1
	$damping 0
	$rotdamping 0
	$rootbone " "

}
