#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* define inteiros de tamanho específico */

#pragma pack(push, 1) /* diz pro compilador não alterar alinhamento
                        ou tamanho da struct */
/*struct pixel {
    uint16_t r:5,
             g:6,
             b:5;
};*/

struct pixel{uint8_t r,g,b;};

unsigned char scale[70] = {'$','@','B','%','8','&','W','M','#','*','o','a','h','k','b','d','p','q','w','m','Z','O','0','Q','L','C','J','U','Y','X','z','c','v','u','n','x','r','j','f','t','/',';','|','(',')','1','{','}','[',']','?','-','_','+','~','<','>','i','!',' '};

struct teste{uint16_t c1;uint32_t c2;};

struct bmpHeader {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1,
            reserved2;
    uint32_t offset,
            header_size;
    int32_t width,
        height;
    uint16_t planes,
            bits;
    uint32_t compression,
                imagesize;
    int32_t xresolution,
        yresolution;
    uint32_t ncolours,
                importantcolours;
    uint32_t redbitmask,greenbitmask,bluebitmask,alphabitmask;
    uint32_t ColorSpaceType;
    uint32_t ColorSpaceEndPoints[9];
    uint32_t Gamma_Red,Gamma_Green,Gamma_Blue,intent,ICCProfileData,ICCProfileSize,Reserved;
};

#pragma pack(pop) /* restaura comportamento do compilador */

void printbin(char texto[], unsigned int valor)
{
     int i;
     printf("%s",texto);
     for (i=31;i>=0;i--) printf("%d",valor >> i & 1);
     printf("\n");
 }

int main() {
    FILE *imagem;
    FILE *nova;
    imagem = fopen("C:\\Users\\marce\\Desktop\\Atom\\C_Uni\\Images\\liberty.bmp", "rb");
    nova = fopen("C:\\Users\\marce\\Desktop\\Atom\\C_Uni\\Text Files\\asciiLiberty.txt", "wt");

    struct bmpHeader bH;

    if (imagem == NULL)
        perror("Erro ao abrir a imagem");

    fread(&bH, sizeof(struct bmpHeader), 1, imagem);
    fwrite(&bH, sizeof(struct bmpHeader), 1, nova);

    struct pixel **matriz;

    matriz = (struct pixel **) malloc(sizeof(struct pixel *) * bH.height);

    int i, j;

    for (i = 0; i < bH.height; i++) {
        matriz[i] = (struct pixel *) malloc(sizeof(struct pixel) * bH.width);
    }

    /* leitura */
    for (i = 0; i < bH.height; i++) {
        for (j = 0; j < bH.width; j++) {
            fread(&matriz[i][j], sizeof(struct pixel), 1, imagem); }
    }

    for (i = bH.height-1; i >= 0; i--) {
        for (j = 0; j < bH.width-1; j++) {

            int media=((matriz[i][j].g)+(matriz[i][j].r)+(matriz[i][j].b))/3;
            media = media/6;
            fwrite(&scale[media], sizeof(char), 1, nova);
            }
            char barran = '\n';
            fwrite(&barran, sizeof(char), 1, nova);
    }

    /* escrita */
    for (i = 0; i < bH.height; i++) {
        for (j = 0; j < bH.width; j++) {
            fwrite(&matriz[i][j], sizeof(struct pixel), 1, nova);
        }

    }

    for (i = 0; i < bH.height; i++) {
        free(matriz[i]);
    }
    free(matriz);


    printf("Image size = %d x %d\n", bH.width, bH.height);
    printf("Tamanho do arquivo = %d\n", bH.size);
    printf("Number of colour planes is %hu\n", bH.planes);
    printf("Bits per pixel is %hu\n", bH.bits);
    printf("Compression type is %u\n", bH.compression);
    printf("Number of colours is %u\n", bH.ncolours);
    printf("Number of required colours is %u\n", bH.importantcolours);
    printf("X resolution: %d - Y resolution: %d (ps: pixels per meter)\n", bH.xresolution, bH.yresolution);
    printf("Header size is %d\n", bH.header_size);
    printbin("Red channel bitmask ", bH.redbitmask);
    printbin("Blue channel bitmask ", bH.bluebitmask);
    printbin("Green channel bitmask ", bH.greenbitmask);
    printbin("Alpha channel bitmask ", bH.alphabitmask);
    printf("Tamanho do bmp header %d\n",sizeof(struct bmpHeader));

    fclose(imagem);
    fclose(nova);
    system("pause");
    return 0;
}
