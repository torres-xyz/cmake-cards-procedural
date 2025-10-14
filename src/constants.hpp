#pragma once

namespace constants
{
    // Inline variables have external linkage by default, so that they are visible to the linker.
    // This is necessary so the linker can de-duplicate the definitions.

    inline constexpr int imguiSideBarWidth{300};
    inline constexpr int screenWidthOriginal{1000};
    inline constexpr int screenWidth{screenWidthOriginal};
    inline constexpr int screenHeight{1000};
    inline constexpr int windowScreenWidth{
#if(DEBUG)
        screenWidthOriginal + imguiSideBarWidth
#else
        1000
#endif
    };
    inline constexpr int windowScreenHeight{1000};
}
