set_project("byopen")
set_xmakever("2.3.5")

add_rules("mode.debug", "mode.release", "mode.minsizerel")
if is_mode("debug") then
    add_defines("BY_DEBUG")
elseif is_mode("minsizerel") then
    add_cxflags("-flto")
    add_shflags("-flto")
    add_ldflags("-flto")
    if is_plat("android") then
        add_shflags("-fuse-ld=lld")
        add_ldflags("-fuse-ld=lld")
    end
end

task("apk_build")
    set_menu {usage = "xmake apk_build [options]", description = "build android apk.", options = {}}
    on_run(function ()
        os.cd("src/android")
        os.exec("./gradlew app:assembleDebug")
    end)

task("apk_install")
    set_menu {usage = "xmake apk_install [options]", description = "install android apk.", options = {}}
    on_run(function ()
        os.cd("src/android")
        os.exec("adb install -r ./app/build/outputs/apk/debug/app-debug.apk")
    end)

task("apk_test")
    set_menu {usage = "xmake apk_test [options]", description = "install and test android apk.", options = {}}
    on_run(function ()
        os.cd("src/android")
        os.exec("./gradlew app:assembleDebug")
        os.exec("adb install -r ./app/build/outputs/apk/debug/app-debug.apk")
        os.exec("adb logcat -s byOpen")
    end)

includes("src/native", "src/demo")
if is_plat("android") then
    includes("src/android/app/jni")
end
