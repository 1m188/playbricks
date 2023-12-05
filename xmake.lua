add_rules("mode.debug", "mode.release")

target("playbricks_qt5")

    add_rules("qt.widgetapp")
    add_files("src/**.cpp")
    add_files("playbricks_qt5.qrc")
    add_files("src/**.h")

    if is_mode("debug") then
        add_defines("DEBUG")
    elseif is_mode("release") then
        add_defines("RELEASE")
    end

    -- 递归获取头文件目录
    on_load(function (target) 
        local dir = target:scriptdir().."/src/**"
        target:add("includedirs", os.dirs(dir))
    end)

target_end()