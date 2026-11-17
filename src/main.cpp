#include <sil/sil.hpp>
#include "random.hpp"
#include <cmath>

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

sil::Image rotate_to_right(sil::Image& image)
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
            new_image.pixel(y, new_image.height() - 1 - x) = image.pixel(x, y);     
        }
    } 
    return new_image;
}

void rgb_split(sil::Image& image)
{
    sil::Image imageref = image;
    int gap {20};
    for (int x{20}; x < image.width()-gap - 1; x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            // if(x <= gap) // si x est entre 0 et 20
            // {
            //     image.pixel(x, y).r = imageref.pixel(x , y).r;  
            //     image.pixel(x, y).g = imageref.pixel(x, y).g; 
            //     image.pixel(x, y).b = imageref.pixel(x + gap , y).b; // on décale juste le bleu
            // } else if (x >= image.width() - gap)
            // {
            //     image.pixel(x, y).r = imageref.pixel(x - gap, y).r;  
            //     image.pixel(x, y).g = imageref.pixel(x, y).g;  
            //     image.pixel(x, y).b = imageref.pixel(x , y).b;  
            // }
            image.pixel(x, y).r = imageref.pixel(x - gap, y).r;  
            image.pixel(x, y).g = imageref.pixel(x, y).g;  
            image.pixel(x, y).b = imageref.pixel(x + gap, y).b;    
        }
    }
}

void low_luminance(sil::Image& image)
{
    sil::Image imageref = image;
    float low_var {1.5f};
    for (int x{0}; x < image.width() - 1; x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            image.pixel(x, y).r = std::pow(imageref.pixel(x, y).r, low_var);  
            image.pixel(x, y).g = std::pow(imageref.pixel(x, y).g, low_var);  
            image.pixel(x, y).b = std::pow(imageref.pixel(x, y).b, low_var);    
        }
    }
}

void high_luminance(sil::Image& image)
{
    sil::Image imageref = image;
    float high_var {0.5f};
    for (int x{0}; x < image.width() - 1; x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            image.pixel(x, y).r = std::pow(imageref.pixel(x, y).r, high_var);  
            image.pixel(x, y).g = std::pow(imageref.pixel(x, y).g, high_var);  
            image.pixel(x, y).b = std::pow(imageref.pixel(x, y).b, high_var);    
        }
    }
}

void disk(sil::Image& image, int x_centre, int y_centre)
{
    int rayon {100};
    // int x_centre {image.width()/2};
    // int y_centre {image.height()/2};
    for (int x{0}; x < image.width()-1; x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            if ((x - x_centre) * (x - x_centre) + (y - y_centre) * (y - y_centre) <= rayon * rayon)
            {
                image.pixel(x, y).r = 1;   
                image.pixel(x, y).g = 1;   
                image.pixel(x, y).b = 1;   
            }  
            else {
                image.pixel(x, y).r = 0;   
                image.pixel(x, y).g = 0;   
                image.pixel(x, y).b = 0; 
            }
        }
    }
}

void circle(sil::Image& image, int x_centre, int y_centre)
{
    int rayon {100};
    int thickness {5};
    // int x_centre {image.width()/2};
    // int y_centre {image.width()/2};
    for (int x{0}; x < image.width()-1; x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            if ((rayon - thickness) * (rayon - thickness) <= (x - x_centre) * (x - x_centre) + (y - y_centre) * (y - y_centre) && (x - x_centre) * (x - x_centre) + (y - y_centre) * (y - y_centre) <= (rayon * rayon))
            {
                image.pixel(x, y).r = 1;   
                image.pixel(x, y).g = 1;   
                image.pixel(x, y).b = 1;   
            }  
        }
    }
}

void rosace(sil::Image& image)
{
    float x_centre {image.width()/ 2.0f};
    float y_centre {image.height()/ 2.0f};
    int rayon {100};
    const float pi { 3.14159265359f};

    circle(image, x_centre, y_centre); // cercle central

    // création des 6 cercles autour
    for (int i {0}; i < 6; i++)
    {
        float angle {(pi/6 * 2) * i};

        float x {x_centre + rayon * std::cos(angle)};
        float y {y_centre + rayon * std::sin(angle)};

        circle(image, x, y);
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
        // sil::Image image{"images/logo.png"};
        // rotate_to_right(image).save("output/rotate_to_right.png"); // ça tourne vers la gauche ;(
    }
    {
        // sil::Image image{"images/logo.png"};
        // rgb_split(image);
        // image.save("output/rgb_split.png");
    }
    {
        // sil::Image image{"images/photo.jpg"};
        // low_luminance(image);
        // image.save("output/low_luminance.png");
    }
    {
        // sil::Image image{"images/photo.jpg"};
        // high_luminance(image);
        // image.save("output/high_luminance.png");
    }
    {
        /* DISQUE */
        // sil::Image image{500, 500};
        // disk(image, 250, 250);
        // image.save("output/disk.png");
    }
    {
        /* CERCLE */
        // sil::Image image{500, 500};
        // circle(image, 250, 250);
        // image.save("output/circle.png");
    }
    {
        /* ANIMATION DISQUE */
        // sil::Image image{500, 500};
        // for (int x {0}; x < image.width(); x++)
        // {
        //     disk(image, x, 250);
        //     image.save("output/disk" + std::to_string(x) + ".png");
        // }  
    }
    {
         /* ROSACE */
        sil::Image image{500, 500};
        rosace(image);
        image.save("output/rosace.png");
    }



}