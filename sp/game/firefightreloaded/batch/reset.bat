@ECHO OFF
echo Resetting game...
del voice_ban.dt
del serverconfig.vdf
del stats.txt
del firefightreloaded_pak.vpk.sound.cache
del modelsounds.cache
del hl2mp_shared.vpk.sound.cache
del demoheader.tmp
del textwindow_temp.html
del console.log
del singleplayerconfig.vdf
del stats.txt
del gamestate.txt
del ep1_gamestats.dat
del cfg\config.cfg
del cfg\settings.scr
del cfg\user.scr
del bin\client.pdb
del bin\gamepadui.pdb
del bin\server.pdb
del bin\client.so.dbg
del bin\client_srv.so.dbg
del bin\server.so.dbg
del bin\server_srv.so.dbg
del bin\gamepadui.so.dbg
del bin\gamepadui_srv.so.dbg
del sound\sound.cache
del /q maps\graphs\*.ain
rmdir /S /Q materials\vgui\logos\ui\
rmdir /S /Q materials\
rmdir /S /Q models\
rmdir /S /Q download\user_custom\
rmdir /S /Q download\sound\
rmdir /S /Q download\materials\
rmdir /S /Q download\models\
rmdir /S /Q downloadlists\
rmdir /S /Q save\