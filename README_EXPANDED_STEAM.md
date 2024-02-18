
# SOURCE 2013 EXPANDED STEAM

https://github.com/Blixibon/source-2013-expanded-steam

---

This integrates a newer version of the Steam API for Source SDK Base 2013 Singleplayer, providing access to modern Steam features. It also comes with its own integration of Steam Deck and Steam Input support, complete with button icons being displayed on HUD hints.

Currently, directly upgrading the Steam API requires replacing `steam_api.dll` in the engine's `bin` folder. This is easy if you are responsible for a standalone Steam mod with its own engine binaries, but that isn't conventionally possible for regular mods based on stock Source SDK Base 2013.

This project comes with an optional hack which uses DLL redirection to force the client/server DLLs to integrate a `steam_api.dll` adjacent to them in the mod's own `bin` folder rather than the engine's `bin` folder. However, this sometimes conflicts with the Steam API implemented by the rest of the engine and modules like Steam Input may not work correctly. There's also not an equivalent solution for native Linux.

The instructions below will assume you are integrating this into a standalone Steam app.

---

## Instructions for integrating Expanded Steam

Follow these instructions if you just want to integrate Steam Input and Steam Deck support into your mod.

### Updating steam_api.dll

First, you must download the latest version of the Steamworks SDK. Assuming you are a Steamworks partner, this will be available through the partner.steamgames.com dashboard.

In the downloaded zip file, go to `redistributable_bin` and extract `steam_api.dll`. Move this file to your app's engine bin folder (i.e. `steamapps/common/my_steam_mod/bin`) and replace the existing `steam_api.dll`.

If you are developing a Linux port, return to the downloaded zip file and go to `redistributable_bin/linux32`. Extract `libsteam_api.so` and move it to your app's Linux engine bin folder, replacing the existing `libsteam_api.so`.

#### Using the DLL redirection hack

If you cannot replace the engine's `steam_api.dll`, extract it to your mod's `bin` folder instead. After merging the code (which comes later), go to `steamworks_update.vpc` and enable the `STEAM_API_MOD_HACK` conditional. This will build the server and client DLLs with the required DLL redirection.

### Integrating Steam Input

*This step will assume you are a Steamworks partner with your own standalone app. It's not currently possible to follow this step if you are building a regular mod.*

If you just want to integrate the same controls as Half-Life 2, you will need to copy the `steamapps/common/Half-Life 2/steam_input` folder to your Steam mod's top-level folder. Then, in your app's Steamworks settings, set the Steam Input Default Configuration to "Custom Configuration (Bundled with game)" and specify the path as `steam_input\action_manifest_hl2.vdf`. You can rename `action_manifest_hl2` if you would like.

You can modify these controller configs and add custom button actions if you need to. Please see [the official Steam Input documentation on the Steamworks website](https://partner.steamgames.com/doc/features/steam_controller) for more information on how this can be done. If you add any new actions, you will need to add them to `scripts/steaminput_actionbinds.txt` to let Expanded Steam bind them to console commands.

### Integrating Expanded Steam

Expanded Steam has three branches available:

- **`master`** - Includes Expanded Steam + Steam Deck and Steam Input support.
- **`mapbase`** - Integrates Expanded Steam with [Mapbase](https://github.com/mapbase-source/source-sdk-2013)-specific features. For example, letting button icons appear on the Game Instructor/`env_instructor_hint`. You should use this branch if your mod is using Mapbase.
- **`updated-sdk-only`** - Integrates compatibility for building the code with updated Steam API headers and *nothing* else.

Merge the branch of your choice using Git. If you are not using any custom code, use the `2013-sp` binaries provided in the latest release. If you are using stock Mapbase code, use the `mapbase-sp` binaries provided in the latest release.

If you are aiming to integrate Steam Input and Steam Deck support, download the scripts and resources provided in the latest release. Merge all of these files with your mod's content.

The files related to Steam Input itself should work out of the box. However, for Steam Deck support, you will need to go into your `resource/clientscheme.res` file and add this to the bottom of it:

```
#base "clientscheme_steaminput.res"
```

This will mount certain Deck-specific fonts. If you have made your own modifications to `clientscheme.res`, you may need to adapt them here.

You will also need to integrate the following files from Half-Life 2 or its episodes:

- `scripts/hudlayout.res`
- `scripts/hudanimations.txt`

These files contain Deck-specific HUD changes which Expanded Steam will recognize.

#### Other Steam Input notes

* If you are using custom weapons, you will need to specify `bucket_360` and `bucket_position_360` keyvalues in their scripts for the "directional" weapon selection screen used by controllers.
* If you are using custom HUD hints which mention directional movement keys (i.e. +forward, etc.), you will need to have joystick counterparts. Expanded Steam can automatically remap HUD hint text keys based on the user's controller scheme. See `scripts/steaminput_hintremap.txt` for examples of how it does this with stock HL2 hints. You can integrate new hints through a separate file called `scripts/mod_hintremap.txt`.

---

That should be everything you need to do in order to integrate Expanded Steam and its features. If you experience any issues, please open an issue on the repository's GitHub page.

---

## Instructions for modifying Expanded Steam

Follow these instructions if you want to modify Expanded Steam's source code. This assumes you have already upgraded `steam_api.dll` as per the instructions above.

### Updating Steam API headers

Open the zip file for the latest version of the Steamworks SDK. Go to `public/steam` and extract all of the `.h` files (`steam_api.json` and the `lib` folder are unnecessary). Then, move these files to your codebase's `src/public/steam` folder, replacing all of the existing headers.

Expanded Steam already comes with the v1.55 version of `steam_api.lib` and its Linux counterpart, so that is all you will need from the SDK. We don't include these headers because that would be in violation of the Steamworks SDK Access Agreement. **You are advised to avoid committing these files as well if you plan to share your fork of the code.**

### Editing the VPCs

In your codebase, go to `vpc_scripts` and open `default.vgc`. There should be a conditional called `BUILD_EXPANDED_STEAM`. Turn it on to add Expanded Steam to your solution/makefile.

Then, open `steamworks_update.vpc`. There should be a macro called `STEAMWORKS_VERSION` near the top. This should be changed to match the version of the Steamworks SDK you're using. More information can be found within that file.

---

That should be everything you need to do in order to add Expanded Steam to your project.

---

## FAQ

### Can I leave Expanded Steam in my code even if I don't currently have an upgraded Steam API?

Not really. This code comes with an updated `steam_api.lib`, which will crash if it attempts to link with an older DLL. You can fix this by just going to `src/lib/public` and replacing `steam_api.lib` with the stock Source 2013 version, but this will stop Expanded Steam from compiling and you will need to remove its library link in `client_base.vpc`.

### Does this support [Gamepad UI?](https://github.com/Joshua-Ashton/HL2-GamepadUI)

I am planning to open my own branch for Gamepad UI which integrates Expanded Steam in the near-future.
