#include <stdio.h>

struct Brick
{
    double height;
    double width;
    double length;
};

void set_size(struct Brick *brick);
double get_positive_input(const char *prompt);
double calc_volume(struct Brick brick);
double calc_surface(struct Brick brick);
void chek_for_square(struct Brick brick);


int main(){

    struct Brick brick1;
    set_size(&brick1);

    printf("\nThe Volume of the brick: %.2lf cm^3\nThe Surface are of the brick: %.2lf cm^2",calc_volume(brick1), calc_surface(brick1));
    chek_for_square(brick1);
    
    return 0;
}

void set_size(struct Brick *brick) {
    printf("Provide the measurements of the brick in cm:\n");

    brick->height = get_positive_input("Height: ");
    brick->width = get_positive_input("Width: ");
    brick->length = get_positive_input("Length: ");
}

double get_positive_input(const char *prompt) {
    double value;
    int valid;

    do {
        printf("%s", prompt);
        valid = scanf("%lf", &value);

        if (valid != 1 || value <= 0) {
            printf("Invalid input! Please enter a positive number.\n");
            while (getchar() != '\n');  // Clear input buffer
        }

    } while (valid != 1 || value <= 0);  // Repeat until valid input

    return value;
}

double calc_volume(struct Brick brick){
    double volume = brick.height*brick.width*brick.length;
    return volume;
}

double calc_surface(struct Brick brick){
    double surface = 2*(brick.height * brick.length) + 2*(brick.height * brick.width) + 2*(brick.length * brick.width);
    return surface;
}

void chek_for_square(struct Brick brick){
    if(brick.height == brick.width || brick.height == brick.length || brick.length == brick.width){
        printf("\nThe brick has a sqaure side");
        
    }
    else{
        printf("\nNo square side");
    }
}