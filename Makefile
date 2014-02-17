GB_CC = lcc

all: mandelbrot.gb

mandelbrot.gb: mandelbrot.c
	$(GB_CC) -o $@ $<

clean:
	$(RM) *.gb *.o
