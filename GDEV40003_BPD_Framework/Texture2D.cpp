#include "Texture2D.h"
#include <SDL_image.h>
#include <iostream>
using namespace std;

// Constructor — store the renderer for later use
Texture2D::Texture2D(SDL_Renderer* renderer)
{
    m_renderer = renderer;

}

// Destructor — always clean up before the object is destroyed
Texture2D::~Texture2D()
{
    Free();                 // destroy the SDL texture
    m_renderer = nullptr;   // don't leave a dangling pointer
}

// Free — releases GPU memory if a texture exists
void Texture2D::Free()
{
    if (m_texture != nullptr)
    {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
        m_width = 0;
        m_height = 0;
    }
}

// LoadFromFile — replaces the old global LoadTextureFromFile function
bool Texture2D::LoadFromFile(std::string path)
{
    Free(); // clear any existing texture first

    SDL_Surface* p_surface = IMG_Load(path.c_str());
    if (p_surface != nullptr)
    {
        // Make cyan (0, 255, 255) pixels transparent
        SDL_SetColorKey(p_surface, SDL_TRUE,
            SDL_MapRGB(p_surface->format, 0, 0xFF, 0xFF));

        m_texture = SDL_CreateTextureFromSurface(m_renderer, p_surface);

        if (m_texture == nullptr)
        {
            cout << "Failed to create texture: " << SDL_GetError() << endl;
        }
        else
        {
            // Store dimensions from the surface before freeing it
            m_width = p_surface->w;
            m_height = p_surface->h;
        }

        SDL_FreeSurface(p_surface);
    }
    else
    {
        cout << "Failed to load image: " << IMG_GetError() << endl;
    }

    return m_texture != nullptr; // true = success
}

// Render — draws the texture at new_position with optional flip and rotation
void Texture2D::Render(Vector2D new_position, SDL_RendererFlip flip, double angle)
{
    
    SDL_Rect renderLocation = 
    {
        (int)new_position.x,
        (int)new_position.y,
        m_width,
        m_height
    };
    SDL_RenderCopyEx(m_renderer, m_texture, NULL, &renderLocation, angle, NULL, flip);
}

void Texture2D::setSize(int height, int width)
{
    m_width = width;
    m_height = height;
}

