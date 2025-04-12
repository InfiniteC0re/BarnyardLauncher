IncludeDir = {}
IncludeDir.fmod = "%{wks.location}/OpenBarnyard/Toshi/Vendor/fmod/include"
IncludeDir.trbf = "%{wks.location}/OpenBarnyard/Tools/TRBF/Include"
IncludeDir.libogg = "%{wks.location}/OpenBarnyard/Toshi/Vendor/libogg/include"
IncludeDir.libvorbis = "%{wks.location}/OpenBarnyard/Toshi/Vendor/libvorbis/include"
IncludeDir.libtheora = "%{wks.location}/OpenBarnyard/Toshi/Vendor/libtheora/include"
IncludeDir.theoraplay = "%{wks.location}/OpenBarnyard/Toshi/Vendor/theoraplay/include"
IncludeDir.stb = "%{wks.location}/OpenBarnyard/Toshi/Vendor/stb"
IncludeDir.dx8 = "%{wks.location}/OpenBarnyard/Toshi/Vendor/DX81/include"
IncludeDir.bink = "%{wks.location}/OpenBarnyard/Toshi/Vendor/bink/include"
IncludeDir.detours = "%{wks.location}/OpenBarnyard/SDK/Vendor/Detours/include"
IncludeDir.glm = "%%{wks.location}/OpenBarnyard/Toshi/Vendor/glm/include"
IncludeDir.glew = "%{wks.location}/OpenBarnyard/Toshi/Vendor/glew/include"
IncludeDir.sdl2 = "%{wks.location}/OpenBarnyard/Toshi/Vendor/sdl2/include"
IncludeDir.toshi = "%{wks.location}/OpenBarnyard/Toshi/Source"
IncludeDir.imgui = "%{wks.location}/ThirdParty/ImGui"

LibDir = {}
LibDir.fmod = "%{wks.location}/OpenBarnyard/Toshi/Vendor/fmod/lib"
LibDir.dx8 = "%{wks.location}/OpenBarnyard/Toshi/Vendor/DX81/lib"
LibDir.bink = "%{wks.location}/OpenBarnyard/Toshi/Vendor/bink/lib"
LibDir.detours = "%{wks.location}/OpenBarnyard/SDK/Vendor/Detours/lib"
LibDir.glew = "%{wks.location}/OpenBarnyard/Toshi/Vendor/glew/lib"
LibDir.sdl2 = "%{wks.location}/OpenBarnyard/Toshi/Vendor/sdl2/lib"

-- content of these folders should be copied to any client application
ClientContentCommon = "%{wks.location}/OpenBarnyardContent/Common/"
ClientContentArch   = "%{wks.location}/OpenBarnyardContent"

-- modify paths based on architecture
LibDir.fmod = LibDir.fmod .. "/x86/"
LibDir.bink = LibDir.bink .. "/x86/"
LibDir.detours = LibDir.detours .. "/x86/"
ClientContentArch = ClientContentArch .. "/x86/"