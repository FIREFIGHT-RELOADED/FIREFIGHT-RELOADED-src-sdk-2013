// Created by Crowbar 0.72

$modelname "weapons/v_gauss.mdl"

$bodygroup "gun"
{
	studio "tau_reference.smd"
}


$surfaceprop "default"

$contents "solid"

$illumposition 11.584 -6.561 -11.853

$cdmaterials "models\buggy\"

$attachment "muzzle" "GaussMesh" -1.5 -17 0 rotate 0 0 0

$cbox 0 0 0 0 0 0

$bbox -3.727 -13.124 -23.602 26.898 0.001 -0.103

$hboxset "default"
$hbox 0 "Bip01 R Hand" 0 0 0 1.135289 1.147308 4.113231
$hbox 0 "GaussMesh" -4.08 -15.46 -3.8 4.08 4.09 6.18
$hbox 0 "Object02" -2.48 -1.13 -1.71 2.48 1.14 2.6

$definebone "Bip01" "" 0 3.72426 39.395706 0 -89.999927 0 0 0 0 0 0 0
$definebone "Bip01 Pelvis" "Bip01" 0 0 0 -89.999982 -89.998206 0 0 0 0 0 0 0
$definebone "Bip01 Spine" "Bip01 Pelvis" 1.326316 -0.003921 0.000002 -0.000057 0.045607 -0.000229 0 0 0 0 0 0
$definebone "Bip01 Spine1" "Bip01 Spine" 4.926308 -0.003923 0 0 0 0 0 0 0 0 0 0
$definebone "Bip01 Spine2" "Bip01 Spine1" 4.926318 -0.003923 0 0 0 0 0 0 0 0 0 0
$definebone "Bip01 Spine3" "Bip01 Spine2" 4.926313 -0.003923 0 0 0 0 0 0 0 0 0 0
$definebone "Bip01 Neck" "Bip01 Spine3" 4.926363 -0.001508 0 0 1.718873 0 0 0 0 0 0 0
$definebone "Bip01 R Clavicle" "Bip01 Neck" 0.000001 0.001514 -1.894737 78.540823 178.234743 -0.000802 0 0 0 0 0 0
$definebone "Bip01 R UpperArm" "Bip01 R Clavicle" 4.926313 0 -0.000001 -66.839938 -34.515842 35.224703 0 0 0 0 0 0
$definebone "Bip01 R Forearm" "Bip01 R UpperArm" 11.368423 0.000001 0.000003 -0.000001 -95.965845 0 0 0 0 0 0 0
$definebone "Bip01 R Hand" "Bip01 R Forearm" 11.368424 -0.000008 -0.000001 7.019476 20.651405 92.591527 0 0 0 0 0 0
$definebone "GaussMesh" "Bip01 R Hand" 1.135291 1.147308 4.113228 -4.661414 100.047655 -1.348857 0 0 0 0 0 0
$definebone "Spinner" "GaussMesh" -1.400034 -1.832775 -0.139659 0.000001 -89.999982 89.999982 0 0 0 0 0 0
$definebone "Bone02" "Spinner" -5 -0.000003 0.000002 0.000001 -0.000001 -0.000001 0 0 0 0 0 0
$definebone "Object02" "Bone02" 3.113566 0 0.001083 89.999982 89.999982 0 0 0 0 0 0 0


$sequence "idle" {
	"v_gauss_anims\idle.smd"
	activity "ACT_VM_IDLE" 1
	fadein 0.2
	fadeout 0.2
	fps 15
	loop
}

$sequence "idle2" {
	"v_gauss_anims\idle2.smd"
	activity "ACT_VM_IDLE" 2
	fadein 0.2
	fadeout 0.2
	fps 15
	loop
}

$sequence "fidget" {
	"v_gauss_anims\fidget.smd"
	activity "ACT_VM_IDLE" 3
	fadein 0.2
	fadeout 0.2
	fps 30
}

$sequence "spin" {
	"v_gauss_anims\spin.smd"
	activity ACT_VM_PULLBACK 1
	fadein 0.2
	fadeout 0.2
	fps 60
	loop
}

$sequence "fire" {
	"v_gauss_anims\fire.smd"
	activity "ACT_VM_PRIMARYATTACK" 1
	fadein 0.2
	fadeout 0.2
	fps 30
}

$sequence "fire2" {
	"v_gauss_anims\fire2.smd"
	activity "ACT_VM_SECONDARYATTACK" 1
	fadein 0.2
	fadeout 0.2
	fps 30
}

$sequence "holster" {
	"v_gauss_anims\holster.smd"
	activity "ACT_VM_HOLSTER" 1
	fadein 0.2
	fadeout 0.2
	fps 45
}

$sequence "draw" {
	"v_gauss_anims\draw.smd"
	activity "ACT_VM_DRAW" 1
	fadein 0.2
	fadeout 0.2
	fps 60
}
