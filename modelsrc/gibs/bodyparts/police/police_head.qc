// Created by Crowbar 0.72
$modelname "gibs/police_head.mdl"
$body studio "police_head_reference.smd"

$surfaceprop "flesh"

$illumposition 0.058 -1.846 7.37

$cdmaterials "models\Police\"
$cdmaterials "models\zombie_classic\"

$sequence idle "police_head_reference.smd"

$collisionmodel "police_head_reference.smd"
{
	$mass 2
	$inertia 1
	$damping 1
	$rotdamping 0
	$rootbone " "
	$concave
	$maxconvexpieces 1
}

$keyvalues
{
	prop_data
	{
		"base" "Flesh.Small"
	}
}