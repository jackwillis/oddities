/* mandelbrot.c -- Paints a Mandelbrot set on a GameBoy.
                   With sound! Takes some time to run.
                   
   Requires GBDK <http://gbdk.sourceforge.net/> to compile.
*/

#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/drawing.h>

void
main (void)
{
  // Dimensions of GameBoy screen
  const UBYTE width = 160;
  const UBYTE height = 144;

  // Rendering settings
  const UBYTE max_iters = 20;
  const short escape_val = 800;

  // Floating point numbers are not available.
  const short x_min = -400;
  const short x_max = 250;
  const short y_min = -400;
  const short y_max = 600;

  const UBYTE dx = (x_max - x_min) / width;
  const UBYTE dy = (y_max - y_min) / height;

  short graph_x = x_min;
  short graph_y = y_min;
  short z_real, z_real_new, z_imag;

  UBYTE x, y, n;
  unsigned short freq;

  NR52_REG = 0x80;  // Turn sound chip on
  NR50_REG = 0xFF;  // Master volume to max
  NR51_REG = 0xFF;  // Output sounds
  NR12_REG = 0xFF;  // Turn CH1 on
  NR11_REG = 0x80;  // Set CH1 duty cycle to 50%

  DISPLAY_ON;

  // cGB only
  cpu_fast ();

  for (x = 0; x != width; x++)
    {
      graph_y = y_min;
      
      for (y = 0; y != height; y++)
        {
          z_real = 0;
          z_imag = 0;
          n = 0;

          while (n != max_iters)
            {
              z_real_new = z_real * z_real - z_imag * z_imag;
              z_imag = 2 * z_real * z_imag + graph_y;
              z_real = z_real_new + graph_x;

              // Account for weird graph scale
              z_imag >>= 4;
              z_real >>= 4;

              if ((z_real * z_real + z_imag * z_imag) > escape_val)
                goto short_circuit;

              n++;
            }
          
          // Using only black and white saves time
          plot_point (x, y);

          short_circuit:
            freq = n * 400 + 500;
            
            // Set CH1 frequency to `freq'
            NR13_REG = (unsigned char) freq;
            NR14_REG = 0x80 | (freq >> 8);
          
          graph_y += dy;
        }
      graph_x += dx;
    }
  
  NR52_REG = 0;  // turn sound chip off
}
