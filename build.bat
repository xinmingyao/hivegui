echo "begin clone ............."
call git clone http://github.com/xinmingyao/hive.git
echo "begin make hive ............."
cd hive
call git checkout gui
make win
copy /Y hivecore.dll ..\\Debug
copy /Y lua52\\lua52.dll ..\\Debug
copy /Y cell.lua ..\\Debug
copy /Y hive.lua ..\\Debug
mkdir  ..\\Debug\\hive

xcopy /Y  hive ..\\Debug\\hive /s/e


copy /Y hivecore.lib ..\\hivegui\\lib
copy /Y src\\hive.h ..\\hivegui\\lib
copy /Y lua52\\lua52.lib ..\\hivegui\\lib
cd ..

call git clone http://github.com/xinmingyao/desktopdriver.git
echo "begin make desktopdriver ............."
cd desktopdriver
make win
copy /Y desktop_driver.lib ..\\hivegui\\lib
copy /Y src\\desktop_driver.h ..\\hivegui\\lib
copy /Y desktop_driver.dll ..\\Debug
cd ..