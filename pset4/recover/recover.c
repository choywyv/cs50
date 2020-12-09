#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef uint8_t BYTE;

int main(int argc, char *argv[])
{

  char s [22];

  if (argc != 2) {
    sprintf (s, "Usage: ./recover image");
    puts (s);
    return 1;
  }


  BYTE buffer[512];
  char filename[8];
  FILE *img, *file = fopen(argv[1], "r");

  for (int counter = 0; fread (buffer, 1, 512, file) == 512; ) {
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) {
      if (counter > 0) fclose (img);

      sprintf (filename, "%03i.jpg", counter);
      counter++;
      img = fopen (filename, "w");
      fwrite (buffer, sizeof (buffer), 1, img);
    }
    else if (counter > 0) fwrite (buffer, sizeof (buffer), 1, img);

  }

}
