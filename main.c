#include <math.h>
#include <stdio.h>

const char *notes[] = {"C-","C#","D-","D#","E-","F-","F#","G-","G#","A-","A#","B-"};
double D = 16777216;
double PAL = 985250;
double NTSC = 1022730;

double freqs[8*12]; // this will contain the real frequencies
int c64freqs[8*12]; // this will contain the freqs suited for the SID chip
unsigned char c64FreqHigh[8*12]; // for the file i/o we need to separate
unsigned char c64FreqLow[8*12];  // the high and low bytes of the freqs

int main(int argc, char *argv[])
{
    for(int i=0; i<8*12; i++) // 8*12, because we have 8 octaves and 12 notes
    {
        freqs[i] = (double)16.3515*pow(2, (double)i/(double)12.0); // 16.3515 = Freq. of C-0
        c64freqs[i] = (int)floor(0.5 + freqs[i] * D / PAL); // change PAL to NTSC here, if needed
        c64FreqHigh[i] = (c64freqs[i] & 0xff00) >> 8;
        c64FreqLow[i] = c64freqs[i] & 0xff;
        printf("%s%i = %d (%i Hz)\n", notes[i%12], i/12, c64freqs[i], (int)floor(0.5+freqs[i]));
    }

    // write data to files
    FILE *f;
    f = fopen("c64hi.bin", "w+");
    if(f==NULL)
    {
        printf("Cannot open file for writing.\n");
        return -1;
    }
    fwrite(c64FreqHigh, 1, 8*12, f);
    fclose(f);
    
    f = fopen("c64lo.bin", "w+");
    if(f==NULL)
    {
        printf("Cannot open file for writing.\n");
        return -1;
    }
    fwrite(c64FreqLow, 1, 8*12, f);
    fclose(f);
    
    return 0;
}
