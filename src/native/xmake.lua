target("byopen")
    set_kind("static")
    if is_plat("iphoneos", "macosx") then
        add_files("byopen_macho.c")
    elseif is_plat("android") then
        add_files("byopen_android.c")
    end
    add_includedirs(".", {interface = true})
    add_headerfiles("*.h")
