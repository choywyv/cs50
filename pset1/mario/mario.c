#include <stdio.h>
#include <cs50.h>

int main (void) {

    int x, height, row;

    for (height = 0; height < 1 || height > 8; ) height = get_int ("Height: ");


    for (row = 0; row < height; row++) {

      for (x = 0; x < height - row - 1; x++) printf (" ");
      for (x = 0; x <= row ; x++) printf ("#");

      printf ("  ");

      for (x = 0; x <= row ; x++) printf ("#");
      //for (x = 0; x < height - row - 1; x++) printf (" ");

      printf ("\n");
    }

}