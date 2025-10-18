#include <string>

void GetGameTextureFromCardType_ReturnsTexture();

int main(int argc, char *argv[])
{
    if (argc < 2 || argv[1] == std::string("GetGameTextureFromCardType_ReturnsTexture"))
    {
        GetGameTextureFromCardType_ReturnsTexture();
    }
}
