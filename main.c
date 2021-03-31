#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct{
    char red;
    char green;
    char blue;
}pixel;

typedef struct{
    int width;
    int height;
    pixel **data;
}image; 

image *input;

int histogram[5] = {0}; 

int filter[3][3] = { {0, -1, 0},
                    {-1, 5, -1},
                  {0, -1, 0}
                          };
pixel p;

void sharpen(pixel matrix[][3])
{   
    p.red = 0;
    p.green = 0;
    p.blue = 0;

    for(int i = 0; i < 3; i++) 
        for(int j = 0; j < 3; j++)
        {
            p.red += matrix[i][j].red * filter[i][j];
            p.green += matrix[i][j].green * filter[i][j];
            p.blue += matrix[i][j].blue * filter[i][j];
        }

}

void read_file(char *file)
{
    char buff[5];
    char *output_name = "test.ppm";
    int constant;
    FILE *fp_in, *fp_out;

    fp_in = fopen(file, "r");
    if(fp_in == NULL) {
        fprintf(stderr, "Could not open file '%s'\n", file);
        exit(1);
    }

    fp_out = fopen(output_name, "w+");
    if(fp_out == NULL) {
        fprintf(stderr, "Could not open file '%s'\n", output_name);
        exit(1);
    }

    if(!fgets(buff, sizeof(buff), fp_in))
    {
        perror(file);
        exit(1);
    }

    if(buff[0] != 'P' || buff[1] != '6')
    {
        fprintf(stderr, "Invalid format: not P6\n");
        exit(1);
    }

    fprintf(fp_out,"P6\n");
    
    input = (image *)malloc(sizeof(image));

    fscanf(fp_in, "%d %d", &input->width, &input->height);
    fscanf(fp_in, "%d", &constant);

    if(constant != 255)
    {
        fprintf(stderr, "Invalid constant: not 255\n");
        exit(1);
    }

    fprintf(fp_out, "%d %d\n", input->width/3 + 1, input->height/3 + 1);

    fprintf(fp_out, "%d\n", 255);

    
    input->data = malloc(3 * input->width * sizeof(pixel *));
    
    for(int block = 0; block < input->height/3 + 1; block++)
        { 
            for(int line = 0; line < 3; line++)
            {  
                fread(input->data[line], sizeof(pixel), input->width, fp_in);
            } 

            for(int col = 0; col < input->width - 2; col++)
            {
                pixel matrix[3][3];
                for(int i = 0; i < 3; i++) 
                    for(int j = 0; j < 3; j++)
                    {
                        matrix[i][j] = input->data[i][col + j];
                    }
                sharpen(matrix);
                fwrite(&p, sizeof(pixel), 1, fp_out);
            }
        }



    fclose(fp_in);
    fclose(fp_out);
}

int edge_pixel(int i, int j, image *img) {
    if(i == 0 || j == 0 || i == img->height - 1 || j == img->width - 1)
        return 1;
    return 0;
}

int limit(int n)
{
    if(n > 255)
        return 255;
    else
        if(n < 0)
            return 0;
        else
            return n;
}

/*void calc_histogram(int r, int g, int b) {
    int greyscale = (int)(roundf(0.2126f * (float)r + 0.7152f * (float)g + 0.0722f * (float)b));

    if (greyscale < 50)
        histogram[0]++;
    else 
        if (greyscale < 101)
            histogram[1]++;
        else 
            if (greyscale < 152)
                histogram[2]++;
            else 
                if (greyscale < 203)
                    histogram[3]++;
                else
                    histogram[4]++;
}*/
/*
void sharpen(image *img, char *file)
{
    FILE *fp_in;
    fp_in = fopen(file, "w+");
    if(fp_in == NULL) {
        fp_inrintf(stderr, "Could not open file '%s'\n", file);
        exit(1);
    }

    fp_inrintf(fp_in,"P6\n");

    fp_inrintf(fp_in, "%d %d\n", img->width, img->height);

    fp_inrintf(fp_in, "%d\n", 255);    
    if(img)
    {
        for(int i = 0; i < img->height; i++)
        { 
            for(int j = 0; j < img->width; j++)
            {  
                if(edge_pixel(i,j,img))
                    fwrite(img->data[i][j], 1, img->width*3, fp_in);
                else
                {
                    img->out[j]->red = limit(img->data[i][j-1].red * -1 +
                                            img->data[i-1][j].red * -1 +
                                            img->data[i][j].red * 5 +
                                            img->data[i+1][j].red * -1 +
                                            img->data[i][j+1].red * -1
                                            );
                    img->out[j]->green = limit(img->data[i][j-1].green * -1 +
                                            img->data[i-1][j].green * -1 +
                                            img->data[i][j].green * 5 +
                                            img->data[i+1][j].green * -1 +
                                            img->data[i][j+1].green * -1
                                            );
                    img->out[j]->blue = limit(img->data[i][j-1].blue * -1 +
                                            img->data[i-1][j].blue * -1 +
                                            img->data[i][j].blue * 5 +
                                            img->data[i+1][j].blue * -1 +
                                            img->data[i][j+1].blue * -1
                                            );
                }
                calc_histogram(img->out[j]->red,img->out[j]->green, img->out[j]->blue);
            }
            fwrite(img->out, 1, img->width*3, fp_in);  
        }
    }
    fclose(fp_in);
}
*/
int main(int argc, char **argv)
{   
    if(argc != 2)
    {
        printf ("ERROR\nPlease insert .ppm file\n");
		exit(1);
    }
    read_file("vit_small.ppm");
    //sharpen(image,"merge??.ppm");
    //write_file("merge?.ppm", image);
    
    return 0;
}

