#include <cstdlib>
#include "../src/textures.hpp"
#include "../src/card.hpp"

void GetGameTextureFromCardType_ReturnsTexture()
{
    if (GetGameTextureFromCardType(CardType::paper) != GameTexture::paperCard)
    {
        std::exit(1); //Outputs a Failed message
    }
}
