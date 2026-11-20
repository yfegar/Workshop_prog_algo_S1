#include <sil/sil.hpp>
#include "random.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <complex>
#include <glm/gtx/matrix_transform_2d.hpp>


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
    for (float x {0.f}; x < image.width() - 1; x++)
    {
        for (float y {0.f}; y < image.height(); y++)
        {
            image.pixel(x, y).r = x / image.width();  
            image.pixel(x, y).g = x / image.width();  
            image.pixel(x, y).b = x / image.width();  
        }
    }
}

sil::Image mirror(sil::Image& image)
{
    sil::Image new_image = image;

    for (int x {0}; x < image.width(); x++)
    {
        for (int y {0}; y < image.height(); y++)
        {
            new_image.pixel(x, y) = image.pixel(image.width() - x - 1, y);     
        }
    }
    return new_image;
}

void noise(sil::Image& image)
{
    for (int i {0}; i < (random_int(image.width() * image.width(), image.width() * image.height())); i++)
    {
        int x = random_int(0, image.width() - 1);
        int y = random_int(0, image.height() - 1);

        image.pixel(x,y).r = random_float(0.f, 1.f);
        image.pixel(x,y).g = random_float(0.f, 1.f);
        image.pixel(x,y).b = random_float(0.f, 1.f);     
    }  
}

sil::Image rotate_to_right(const sil::Image& image)
{
    sil::Image new_image{image.height(), image.width()};

    for (int x {0}; x < image.width() - 1; x++)
    {
        for (int y {0}; y < image.height() - 1; y++)
        {
            new_image.pixel(image.height() - 1 - y, x) = image.pixel(x, y);     
        }
    } 
    return new_image;
}

void rgb_split(sil::Image& image)
{
    sil::Image imageref = image;
    int gap {20};
    
    for (int x {20}; x < image.width() - gap - 1; x++)
    {
        for (int y {0}; y < image.height(); y++)
        {
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

    for (int x {0}; x < image.width() - 1; x++)
    {
        for (int y {0}; y < image.height(); y++)
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

    for (int x {0}; x < image.width() - 1; x++)
    {
        for (int y {0}; y < image.height(); y++)
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

    for (int x {0}; x < image.width() - 1; x++)
    {
        for (int y {0}; y < image.height(); y++)
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

    for (int x {0}; x < image.width() - 1; x++)
    {
        for (int y {0}; y < image.height(); y++)
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

sil::Image mosaique(sil::Image& image)
{
    sil::Image imageref = image;
    sil::Image canvas {5 * image.width(), 5 * image.height()};

    for (int x {0}; x < canvas.width(); x ++)
    {
        for (int y {0}; y < canvas.height(); y ++)
        {
            canvas.pixel(x,y) = image.pixel(x % image.width(), y % image.height());
        } 
    };

    return canvas;
}

sil::Image mosaique_miroir(sil::Image& image)
{
    // les variantes de l'image
    sil::Image horizontal = mirror(image);
    sil::Image rot180 = rotate_to_right(rotate_to_right(image));
    sil::Image vertical = rotate_to_right(rotate_to_right(horizontal));

    sil::Image canvas {5 * image.width(), 5 * image.height()};
  
    for (int x {0}; x < canvas.width(); x++)
    {
        for (int y {0}; y < canvas.height(); y++)
        {
            // indice x et y de la colonne et de la ligne du logo
            int colonne = x / image.width();
            int ligne = y / image.height();

            // coordonnées dans le canvas
            int local_x = x % image.width();
            int local_y = y % image.height();

            bool col_impaire {colonne % 2 == 1};
            bool ligne_impaire {ligne % 2 == 1};

            if (!col_impaire && !ligne_impaire)
            {
                canvas.pixel(x,y) = image.pixel(local_x, local_y);
            } else if (col_impaire && !ligne_impaire)
            {
                canvas.pixel(x,y) = horizontal.pixel(local_x, local_y);
            } else if (!col_impaire && ligne_impaire)
            {
                canvas.pixel(x,y) = vertical.pixel(local_x, local_y);
            }else 
            {
                canvas.pixel(x,y) = rot180.pixel(local_x, local_y);
            }
   
        } 
    };
    return canvas;
}

void glitch(sil::Image& image)
{
    int nombre_glitchs {random_int(10, 100)};

    for (int i {0}; i < nombre_glitchs; i++)
    {
        int rec_width = random_int(10, 60);
        int rec_height = random_int(1, 5); 

        int rec1_x {random_int(0, image.width() - rec_width)};
        int rec1_y {random_int(0, image.height() - rec_height)};

        int rec2_x {random_int(0, image.width() - rec_width)};
        int rec2_y {random_int(0, image.height() - rec_height)};

        for (int x {0}; x < rec_width; x++)
        {
            for (int y {0}; y < rec_height; y++)
            {
                std::swap(
                    image.pixel(rec1_x + x, rec1_y + y),
                    image.pixel(rec2_x + x, rec2_y + y)
                );
            }
        }
    }
}


float brightness(glm::vec3 const& color)
{
    return color.r + color.g + color.b;
}

void pixel_sort(sil::Image& image)
{
    std::sort(image.pixels().begin(), image.pixels().end(), [](glm::vec3 const& color1, glm::vec3 const& color2)
    {
        return brightness(color1) < brightness(color2); 
    });
}

void pixel_sort_ligne(sil::Image& image)
{
    for(int y {0}; y < image.height(); y++)
    {
        auto ligne {y * image.width()};   
        std::sort(
            image.pixels().begin() + ligne,
            image.pixels().begin() + ligne + image.width() ,
            [](glm::vec3 const& color1, glm::vec3 const& color2)
            {
                return brightness(color1) < brightness(color2); 
            });
    }
  
}

void pixel_sort_random(sil::Image& image)
{
    for(int y {0}; y < image.height(); y++)
    {
        unsigned long total_pixel_number {image.pixels().size()};
        int sorted_pixel_number {random_int(10, 100)};

        auto begin {image.pixels().begin() + random_int(0, total_pixel_number - sorted_pixel_number)};
        auto end {begin + sorted_pixel_number};

        std::sort(
            begin,end,[](glm::vec3 const& color1, glm::vec3 const& color2)
            {
                return brightness(color1) < brightness(color2); 
            });
    }
}

void mandelbrot(sil::Image& image)
{
    const int max_iterations {50};

    const float x_min {-2.0f};
    const float x_max {1.0f};
    const float y_min {-1.5f};
    const float y_max {1.0f};

    for (int x {0}; x < image.width(); x++)
    {
        for (int y {0}; y < image.height(); y++)
        {
            float reel = x_min + (static_cast<float>(x) / image.width()) * (x_max - x_min);
            float imaginaire = y_min + (static_cast<float>(y) / image.height()) * (y_max - y_min);
            std::complex<float> c(reel, imaginaire);

            std::complex<float> z (0.0f, 0.0f);
            int iterations {0};

            while (std::norm(z) <= 4.0f && iterations < max_iterations)
            {
                z = z * z + c;
                iterations++;
            }
            
            if (iterations == max_iterations)
            {
                image.pixel(x,y) = glm::vec3{1.0f};
            } else
            {
                float brightness = static_cast<float>(iterations) / max_iterations;
                image.pixel(x,y) = glm::vec3{brightness}; 
            }  
        }
    }
}

/* EX 20 : OKLAB */

struct RGB {
    float r,
    g, 
    b;
};

struct Lab {
    float L,
    a, 
    b;
};


float sRGB_to_linear(float s)
{
    float seuil {0.04045f};
    float l {0.0f};

    if (0 <= s && s <= seuil)
    {
        l = s / 12.92f;
    } else if (seuil < s && s <= 1)
    {
        l = std::pow(((s + 0.055f) / 1.055f), 2.4f);
    }
    return l;
}


inline Lab linear_srgb_to_oklab(RGB c)
{
    float l = 0.4122214708f * c.r + 0.5363325363f * c.g + 0.0514459929f * c.b;
    float m = 0.2119034982f * c.r + 0.6806995451f * c.g + 0.1073969566f * c.b;
    float s = 0.0883024619f * c.r + 0.2817188376f * c.g + 0.6299787005f * c.b;

    float l_ = std::cbrt(l);
    float m_ = std::cbrt(m);
    float s_ = std::cbrt(s);

    return {
        0.2104542553f * l_ + 0.7936177850f * m_ - 0.0040720468f * s_,
        1.9779984951f * l_ - 2.4285922050f * m_ + 0.4505937099f * s_,
        0.0259040371f * l_ + 0.7827717662f * m_ - 0.8086757660f * s_,
    };
}

inline Lab srgb8_to_oklab(float r, float g, float b)
{
    RGB lin {
        sRGB_to_linear(r),
        sRGB_to_linear(g),
        sRGB_to_linear(b)
    };
    return linear_srgb_to_oklab(lin);

}

inline RGB oklab_to_linear_srgb(Lab c)
{
    float l_ = c.L + 0.3963377774f * c.a + 0.2158037573f * c.b;
    float m_ = c.L - 0.1055613458f * c.a - 0.0638541728f * c.b;
    float s_ = c.L - 0.0894841775f * c.a - 1.2914855480f * c.b;

    float l = l_ * l_ * l_;
    float m = m_ * m_ * m_;
    float s = s_ * s_ * s_;

    return {
        +4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s,
        -1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s,
        -0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s,
    };
}

inline float linear_to_srgb(float l)
{
    return (l <= 0.0031308f)
        ? l * 12.92f
        : 1.055f * std::pow(l, 1.0f / 2.4f) - 0.055f;
}

inline glm::vec3 oklab_to_srgb(Lab c)
{
    RGB lin = oklab_to_linear_srgb(c);
    return glm::vec3{
        linear_to_srgb(std::clamp(lin.r, 0.0f, 1.0f)),
        linear_to_srgb(std::clamp(lin.g, 0.0f, 1.0f)),
        linear_to_srgb(std::clamp(lin.b, 0.0f, 1.0f)),
    };
}


void gradient_oklab(sil::Image& image)
{
    Lab c0 {srgb8_to_oklab(1.0f, 0.0f, 1.0f)}; // violet
    Lab c1 {srgb8_to_oklab(0.0f, 1.0f, 1.0f)}; // vert

    for (uint x {0}; x < image.width(); x++)
    {
        float t = static_cast<float>(x) / (image.width() - 1);

        Lab c{
            c0.L + t * (c1.L - c0.L),
            c0.a + t * (c1.a - c0.a),
            c0.b + t * (c1.b - c0.b),

        };

        glm::vec3 rgb {oklab_to_srgb(c)};

        for (uint y {0}; y < image.height(); y++)
        {
            image.pixel(x, y) = rgb;   
        }
    }
}

void gradient_srgb(sil::Image& image)
{
    for (float x {0.f}; x < image.width(); x++)
    {
        float t = static_cast<float>(x) / (image.width() - 1);
        glm::vec3 couleur {1.0f - t, t, 1.0f};

        for (float y {0.f}; y < image.height(); y++)
        {
            image.pixel(x, y) = couleur;   
        }
    }
}

/* FIN EX 20 : OKLAB */

void dithering(sil::Image& image)
{
    const int bayer_n = 4;
    float bayer_matrix_4x4[][bayer_n] = {
        {    -0.5,       0,  -0.375,   0.125 },
        {    0.25,   -0.25,   0.375, - 0.125 },
        { -0.3125,  0.1875, -0.4375,  0.0625 },
        {  0.4375, -0.0625,  0.3125, -0.1875 },
    };

    for (int y {0}; y < image.height(); y++) 
    {
        for (int x {0}; x < image.width(); x++)
         {  
            glm::vec3 c = image.pixel(x, y);

            float gray {0.5f * c.r + 0.5f * c.g + 0.5f * c.b};

            gray += bayer_matrix_4x4[y % bayer_n][x % bayer_n];
            
            float result {0.0f};

            if (gray < 0.5f)
            {
                result = 0.0f;
            } else
            {
                result = 1.0f;
            }

            image.pixel(x, y) = glm::vec3{result};
        }
    }
}

glm::vec2 rotated(glm::vec2 point, glm::vec2 center_of_rotation, float angle)
    {
        return glm::vec2{glm::rotate(glm::mat3{1.f}, angle) * glm::vec3{point - center_of_rotation, 0.f}} + center_of_rotation;
    };

void vortex(sil::Image& image)
{
    sil::Image imageref = image;

    const glm::vec2 center {image.width() / 2.f, image.height() / 2.f};
    const float strength {0.05f};

    for (int y {0}; y < image.height(); y++)
    {
        for (int x {0}; x < image.width(); x++)
        {
            glm::vec2 p {static_cast<float>(x), static_cast<float>(y)};

            float distance {glm::distance(p, center)};
            float angle {distance * strength};

            glm::vec2 src {rotated(p, center, angle)};

            int sx {static_cast<int>(std::round(src.x))};
            int sy {static_cast<int>(std::round(src.y))};

            if (sx < 0 || sy < 0 || sx >= static_cast<int>(image.width()) || sy >= static_cast<int>(image.height()) )
            {
                image.pixel(x,y) = glm::vec3{0.f};
            } else
            {
                image.pixel(x,y) = imageref.pixel(sx, sy);
            }
        }
    }    
}


int main()
{
    {
        /* 1. KEEP GREEN ONLY */
        // sil::Image image{"images/logo.png"};
        // keep_green_only(image);
        // image.save("output/keep_green_only.png");
    }
    {
        /* 2. CHANNEL SWAP */
        // sil::Image image{"images/logo.png"};
        // channel_swap(image);
        // image.save("output/channel_swap.png");
    }
    {
        /* 3. BLACK AND WHITE */
        // sil::Image image{"images/logo.png"};
        // black_and_white(image);
        // image.save("output/black_and_white.png");
    }
    {
        /* 4. NEGATIVE */
        // sil::Image image{"images/logo.png"};
        // negative(image);
        // image.save("output/negative.png");
    }
    {
        /* 5. GRADIENT */
        // sil::Image image{300/*width*/, 200/*height*/};
        // gradient(image);
        // image.save("output/gradient.png");
    }
    {
        /* 6. MIRROR */
        // sil::Image image{"images/logo.png"};
        // mirror(image).save("output/mirror.png");
    }
    {
        /* 7. RANDOM NOISE */
        // sil::Image image{"images/logo.png"};
        // noise(image);
        // image.save("output/noise.png");
    }
    {
        /* 8. ROTATE IMAGE BY 90 DEGREES */
        // sil::Image image{"images/logo.png"};
        // rotate_to_right(image).save("output/rotate_to_right.png"); // ça tourne vers la gauche ;(
    }
    {
        /* 9. RGB SPLIT */
        // sil::Image image{"images/logo.png"};
        // rgb_split(image);
        // image.save("output/rgb_split.png");
    }
    {
        /* 1O. LOW LUMINANCE */
        // sil::Image image{"images/photo.jpg"};
        // low_luminance(image);
        // image.save("output/low_luminance.png");
    }
    {
        /* 1O.bis HIGH LUMINANCE */
        // sil::Image image{"images/photo.jpg"};
        // high_luminance(image);
        // image.save("output/high_luminance.png");
    }
    {
        /* 11. DISK */
        // sil::Image image{500, 500};
        // disk(image, 250, 250);
        // image.save("output/disk.png");
    }
    {
        /* 12. CIRCLE */
        // sil::Image image{500, 500};
        // circle(image, 250, 250);
        // image.save("output/circle.png");
    }
    {
        /* 13. DISK ANIMATION */
        // sil::Image image{500, 500};
        // for (int x {0}; x < image.width(); x++)
        // {
        //     disk(image, x, 250);
        //     image.save("output/disk" + std::to_string(x) + ".png");
        // }  
    }
    {
        /* 14. ROSACE */
        // sil::Image image{500, 500};
        // rosace(image);
        // image.save("output/rosace.png");
    }
    {
        /* 15. MOSAIQUE */
        // sil::Image image{"images/logo.png"};
        // mosaique(image).save("output/mosaique.png");
    }
    {
        /* 16. MOSAIQUE MIROIR */
        // sil::Image image{"images/logo.png"};
        // mosaique_miroir(image).save("output/mosaique_miroir.png");
    }
    {
        /* 17. GLITCH */
        // sil::Image image{"images/logo.png"};
        // glitch(image);
        // image.save("output/glitch.png");
    }
    {
        /* 18. PIXEL SORT */
        // Tri de tous les pixels
        // sil::Image image{"images/logo.png"};
        // pixel_sort(image);
        // image.save("output/pixel_sort.png"); 
    }
    {
        /* 18.bis PIXEL SORT */
        // Tri des lignes
        // sil::Image image{"images/logo.png"};
        // pixel_sort_ligne(image);
        // image.save("output/pixel_sort_ligne.png"); 
    }
    {
        /* 18.ter PIXEL SORT */
        // Tri des parties aléatoires
        // sil::Image image{"images/logo.png"};
        // pixel_sort_random(image);
        // image.save("output/pixel_sort_random.png"); 
    }
    {
        /* 19. FRACTALE DE MANDELBROT */
        // sil::Image image{500, 500};
        // mandelbrot(image);
        // image.save("output/mandelbrot.png"); 
    }
    {
        /* 20. DÉGRADÉ SRGB */
        // sil::Image image{300/*width*/, 200/*height*/};
        // gradient_srgb(image);
        // image.save("output/gradient_srgb.png");
    }
    {
        /* 20.bis DÉGRADÉ OKLAB */
        // sil::Image image{300/*width*/, 200/*height*/};
        // gradient_oklab(image);
        // image.save("output/gradient_oklab.png");
    }
    {
        /* 21. DITHERING */
        // sil::Image image{"images/photo.jpg"};
        // dithering(image);
        // image.save("output/dithering.png");
    }
     {
        /* 22. VORTEX */
        // sil::Image image{"images/logo.png"};
        // vortex(image);
        // image.save("output/vortex.png");
    }
}