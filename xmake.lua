add_rules("mode.debug", "mode.release")

add_requires("glib")

set_license("MIT")

set_optimize("aggressive")
-- add_cxflags("-fno-stack-protector", "-fomit-frame-pointer", "-flto", "-O2", "-funroll-loops")

if is_mode("release") then
    add_cxflags("-fno-stack-protector", "-fomit-frame-pointer", "-flto")
    add_ldflags("-flto", "-s")

    if is_plat("linux", "macosx") then
        add_cxflags(
            "-O3", 
            "-march=native", 
            "-fstrict-aliasing",
            "-funroll-loops",
            "-ffast-math",
            "-fno-exceptions"
        )
    end
    if is_plat("windows") then
        add_cxflags(
            "/O2", 
            "/fp:fast",
            "/GL",
            "/Qpar",
            "/arch:AVX2"
        )
        add_ldflags("/LTCG")
    end

    add_ldflags("-Wl,--gc-sections")

    set_policy("build.optimization.lto", true)

    set_symbols("hidden")
end

target("braingear")
    set_kind("binary")
    add_files("src/*.c")
    add_packages("glib")