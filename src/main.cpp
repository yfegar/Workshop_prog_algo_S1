#include <sil/sil.hpp>
#include "random.hpp"

void keep_green_only(sil::Image& image)
{
    for (glm::vec3& color : image.pixels())
    {
        color.r = 0.f;
        color.b = 0.f;
    }
}

void channel_swap(sil::Image& image)
{
    for (glm::vec3& color : image.pixels())
    {
        std::swap(color.r, color.b);
    }
}

void black_and_white(sil::Image& image)
{
    for (glm::vec3& color : image.pixels())
    {
        color.r = (color.r + color.g + color.b) / 3;
        color.g = color.r;
        color.b = color.r;
    }
}

void negative(sil::Image& image)
{
    for (glm::vec3& color : image.pixels())
    {
        color.r = 1 - color.r;
        color.g = 1 - color.g;
        color.b = 1 - color.b;
    }
}

void gradient(sil::Image& image)
{
    for (float x{0.f}; x < image.width()-1; x++)
    {
        for (float y{0.f}; y < image.height(); y++)
        {
            image.pixel(x, y).r = x / image.width();  
            image.pixel(x, y).g = x / image.width();  
            image.pixel(x, y).b = x / image.width();  
        }
    }
}

void mirror(sil::Image& image)
{
    sil::Image imageref = image;
    for (int x{0}; x < image.width()-1; x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            image.pixel(x, y) = imageref.pixel(image.width() - (x+1),y);     
        }
    }
}

void noise(sil::Image& image)
{
    // le nombre de pixels altérés varie de 300*300 à 300*345 (qui correspond à son exposition)
    for (int i{0}; i < (random_int(image.width()*image.width(), image.width()*image.height())); i++)
    {
        int x = random_int(0, image.width()-1);
        int y = random_int(0, image.height()-1);

        image.pixel(x,y).r = random_float(0.f, 1.f);
        image.pixel(x,y).g = random_float(0.f, 1.f);
        image.pixel(x,y).b = random_float(0.f, 1.f);    
        
    }  
}

void rotate_to_right(sil::Image& image)
{
    // la nouvelle image aura une width de 345 et une height de 300
    sil::Image new_image{image.height(), image.width()};
    //                 = 300 - 1
    for (int x{0}; x < image.width()-1; x++)
    {
        //                 = 345 - 1
        for (int y{0}; y < image.height()-1; y++)
        {
            //                 = 300 - x
            //new_image.pixel(y, new_image.height()-x) = image.pixel(x, y); 
            image.pixel(x, y) = new_image.pixel(y, new_image.width() - (x+1));     
        }
    } 
}

int main()
{
    {
        // sil::Image image{"images/logo.png"};
        // keep_green_only(image);
        // image.save("output/keep_green_only.png");
    }
    {
        // sil::Image image{"images/logo.png"};
        // channel_swap(image);
        // image.save("output/channel_swap.png");
    }
    {
        // sil::Image image{"images/logo.png"};
        // black_and_white(image);
        // image.save("output/black_and_white.png");
    }
    {
        // sil::Image image{"images/logo.png"};
        // negative(image);
        // image.save("output/negative.png");
    }
    {
        // sil::Image image{300/*width*/, 200/*height*/};
        // gradient(image);
        // image.save("output/gradient.png");
    }
    {
        // sil::Image image{"images/logo.png"};
        // mirror(image);
        // image.save("output/mirror.png");
    }
    {
        // sil::Image image{"images/logo.png"};
        // noise(image);
        // image.save("output/noise.png");
    }
    {
        sil::Image image{"images/logo.png"};
        rotate_to_right(image);
        image.save("output/rotate_to_right.png");
    }

}