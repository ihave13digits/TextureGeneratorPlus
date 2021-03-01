#define OLC_PGE_APPLICATION

#include "olcPixelGameEngine.h"

class Game : public olc::PixelGameEngine
{
public:
    Game()
    {
        sAppName = "TextureGenerator";
    }
    
public:

    // My Stuff

    bool LINE_H = false;
    bool LINE_V = false;
    int SEED = 1;
    int SIZE = 32;
    int TICK = 0;
    int SPEED = 5;
    int ALPHA = 200;
    int AX = 0;
    int AY = 0;
    int SX = 2;
    int SY = 2;
    int BX = 2;
    int BY = 2;
    int X = 0;
    int Y = 0;
    short int shade[3] = {1, 1, 1};
    short int rando[3] = {1, 1, 1};
    short int color[3] = {254, 254, 254};
    short int c_index = 0;
    int image[4*4][32*32][3];
    int i_index = 0;

    void Blur()
    {
        int img[SIZE*SIZE][3];    
        for (int x = 0; x < SIZE; x++)
        {
            for (int y = 0; y < SIZE; y++)
            {
                int I = (y*SIZE)+x;
                int blur[] = {0,0,0,0};
                for (int i = 0; i < BX; i++)
                {
                    for (int j = 0; j < BY; j++)
                    {
                        int B = (j*BX)+i;
                        if (x < SIZE-1 and y < SIZE-1)
                        {
                            blur[0] += image[i_index][I+B][0]; blur[1] += image[i_index][I+B][1]; blur[2] += image[i_index][I+B][2];
                            blur[3]++;
                        }
                        else
                        {
                            blur[0] += image[i_index][0][0]; blur[1] += image[i_index][0][1]; blur[2] += image[i_index][0][2];
                            blur[3]++;
                        }
                    }
                }
                blur[0] = int(blur[0] / blur[3]); blur[1] = int(blur[1] / blur[3]); blur[2] = int(blur[2] / blur[3]);
                img[I][0] = blur[0]; img[I][1] = blur[1]; img[I][2] = blur[2];
            }
        }
        for (int x = 0; x < SIZE; x++)
        {
            for (int y = 0; y < SIZE; y++)
            {
                int I = (y*SIZE)+x;
                image[i_index][I][0] = img[I][0]; image[i_index][I][1] = img[I][1]; image[i_index][I][2] = img[I][2];
            }
        }
    }

    void DrawInfo()
    {
        olc::Pixel tc;
        tc.r = 255-color[0];
        tc.g = 255-color[1];
        tc.b = 255-color[2];
        tc.a = ALPHA;

        DrawStringDecal({128,0}, "Image: " + std::to_string(i_index), tc, { 0.5f, 0.5f });
        DrawStringDecal({128,5}, "Color: " + std::to_string(c_index), tc, { 0.5f, 0.5f });
        
        DrawStringDecal({128,15}, "Align: (" + std::to_string(AX) + ", " + std::to_string(AY) + ")", tc, { 0.5f, 0.5f });
        DrawStringDecal({128,20}, "Spacing: (" + std::to_string(SX) + ", " + std::to_string(SY) + ")", tc, { 0.5f, 0.5f });

        DrawStringDecal({128,30}, "Blur: (" + std::to_string(BX) + ", " + std::to_string(BY) + ")", tc, { 0.5f, 0.5f});
        DrawStringDecal({128,35}, "Shading Value: ("+std::to_string(shade[0])+", "+std::to_string(shade[1])+", "+std::to_string(shade[2])+")", tc, { 0.5f, 0.5f });
        DrawStringDecal({128,40}, "Random Value: ("+std::to_string(rando[0])+", "+std::to_string(rando[1])+", "+std::to_string(rando[2])+")", tc, { 0.5f, 0.5f });
        DrawStringDecal({128,45}, "Color Value: ("+std::to_string(color[0])+", "+std::to_string(color[1])+", "+std::to_string(color[2])+")", tc, { 0.5f, 0.5f });
    }

    void DrawScreen()
    {
        srand(SEED);
        for (int x = 0; x < SIZE; x++)
        {
            for (int y = 0; y < SIZE; y++)
            {
                int i = (y*SIZE)+x;
                image[i_index][i][0] = (rand()%rando[0])+color[0];
                image[i_index][i][1] = (rand()%rando[1])+color[1];
                image[i_index][i][2] = (rand()%rando[2])+color[2];
                if (LINE_H)
                {
                    if (x % SX == AX)
                    {
                        image[i_index][i][0] = (rand()%shade[0])+color[0];
                        image[i_index][i][1] = (rand()%shade[1])+color[1];
                        image[i_index][i][2] = (rand()%shade[2])+color[2];
                    }
                }
                if (LINE_V)
                {
                    if (y % SY == AY)
                    {
                        image[i_index][i][0] = (rand()%shade[0])+color[0];
                        image[i_index][i][1] = (rand()%shade[1])+color[1];
                        image[i_index][i][2] = (rand()%shade[2])+color[2];
                    }
                }
            }
        }
    }

    void ShowScreen()
    {
        Clear(olc::Pixel(color[0], color[1], color[2]));
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                int i1 = j*4+i;
                for (int x = 0; x < SIZE; x++)
                {
                    for (int y = 0; y < SIZE; y++)
                    {
                        int i2 = y*SIZE+x;
                        Draw(x+(i*SIZE), y+(j*SIZE), olc::Pixel(image[i1][i2][0], image[i1][i2][1], image[i1][i2][2]));
                    }
                }
            }
        }
    }

    bool OnUserCreate() override
    {
        // Called once at the start, so create things here

        for (int i = 0; i < 16; i++)
        {
            DrawScreen();
            i_index++;
        }
        i_index = 0;
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // called once per frame

        // Modulus Operations
        if (GetKey(olc::Key::Y).bPressed)
        {
            if (LINE_V) LINE_V = false;
            else LINE_V = true;
        }
        if (GetKey(olc::Key::X).bPressed)
        {
            if (LINE_H) LINE_H = false;
            else LINE_H = true;
        }

        // Change Seed
        if (GetKey(olc::Key::R).bPressed) SEED = rand()%999999999; DrawScreen();

        // Change Speed
        if (GetKey(olc::Key::SHIFT).bHeld) SPEED = 0;
        if (GetKey(olc::Key::SHIFT).bReleased) SPEED = 5;
        if (GetKey(olc::Key::CTRL).bHeld) SPEED = 10;
        if (GetKey(olc::Key::CTRL).bReleased) SPEED = 5;
        
        // Change Blur Values
        if (GetKey(olc::Key::NP6).bPressed) if (BX < SIZE-1) BX += 1;
        if (GetKey(olc::Key::NP4).bPressed) if (BX > 2) BX -= 1;
        if (GetKey(olc::Key::NP8).bPressed) if (BY < SIZE-1) BY += 1;
        if (GetKey(olc::Key::NP2).bPressed) if (BY > 2) BY -= 1;

        // Change Modulus Values
        if (GetKey(olc::Key::L).bPressed) if (AX < SX-1) AX += 1;
        if (GetKey(olc::Key::J).bPressed) if (AX > 0) AX -= 1;
        if (GetKey(olc::Key::K).bPressed) if (AY < SY-1) AY += 1;
        if (GetKey(olc::Key::I).bPressed) if (AY > 0) AY -= 1;

        if (GetKey(olc::Key::G).bPressed) if (SY < SIZE) SY += 1;
        if (GetKey(olc::Key::T).bPressed) if (SY > 2) SY -= 1;
        if (GetKey(olc::Key::H).bPressed) if (SX < SIZE) SX += 1;
        if (GetKey(olc::Key::F).bPressed) if (SX > 2) SX -= 1;

        // Change Selected Color
        if (GetKey(olc::Key::S).bPressed) if (c_index > 0) c_index -= 1;
        if (GetKey(olc::Key::W).bPressed) if (c_index < 2) c_index += 1;

        // Change Selected Image
        if (GetKey(olc::Key::Z).bPressed) if (i_index > 0) i_index -= 1;
        if (GetKey(olc::Key::Q).bPressed) if (i_index < 15) i_index += 1;

        // Change Value of Selected
        if (GetKey(olc::Key::A).bHeld)
        {
            if (TICK < SPEED) TICK++;
            else
            {
                if (color[c_index] > 1) color[c_index] -= 1; rando[c_index] = 255-color[c_index]; shade[c_index] = int(rando[c_index]/2)+1; DrawScreen(); TICK = 0;
            }
        }
        if (GetKey(olc::Key::D).bHeld)
        {
            if (TICK < SPEED) TICK++;
            else
            {
                if (color[c_index] < 254) color[c_index] += 1; rando[c_index] = 255-color[c_index]; shade[c_index] = int(rando[c_index]/2)+1; DrawScreen(); TICK = 0;
            }
        }

        // X
        if (GetKey(olc::Key::LEFT).bHeld)
        {
            if (TICK < SPEED) TICK++;
            else if (X > 0) X -= 1;
        }
        if (GetKey(olc::Key::RIGHT).bHeld)
        {
            if (TICK < SPEED) TICK++;
            else if (X < ScreenWidth()-SIZE) X += 1;
        }
        // Y
        if (GetKey(olc::Key::UP).bHeld)
        {
            if (TICK < SPEED) TICK++;
            else if (Y > 0) Y -= 1;
        }
        if (GetKey(olc::Key::DOWN).bHeld)
        {
            if (TICK < SPEED) TICK++;
            else if (Y < ScreenHeight()-SIZE) Y += 1;
        }

        // Effects
        if (GetKey(olc::Key::B).bHeld) Blur();
        
        ShowScreen();
        DrawInfo();

        return !GetKey(olc::Key::ESCAPE).bPressed;
    }
};



int main()
{
    Game tg;
    if (tg.Construct(256, 128, 4, 4, false, false))
        tg.Start();

    return 0;
}
