rmdir /s/q D:\intensity_data
mkdir D:\intensity_data
adb remount
adb pull /data/vendor/camera/img/ D:\intensity_data\
adb shell "rm -rf /data/vendor/camera/img/*"