/*   Enrique Javier Davis Gutiérrez  -  Venezuela
          Licencia GPLv3 --> https://www.gnu.org/licenses/gpl-3.0.en.html
Referencia al autor y Respeta y Reproduce la Licencia y sus Libertades
*/

// Objetivo: 
//     |===-->  Convertir archivos PSSE (curvas de simulaciones, Siemens) a ASCII CSV

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

int main(void) {

FILE *fichero_entrada;
FILE *fichero_salida;

// declaración de variables locales
float x;
char nombre_fichero_punto_out[20];
char titulo;
int byte1_titulos = 20; // 0x14 Byte inicial de Titulos de las Variables
long int inicio_bloque = 2320;   // 0x910 
long int longitud_archivo;

printf("nombre del fichero con extención .OUT : ");
scanf("%s", nombre_fichero_punto_out);

fichero_entrada = fopen(nombre_fichero_punto_out, "rb");
if (fichero_entrada == NULL) {
  printf("ERROR: No existe el fichero");
  return 2;
}

fichero_salida = fopen("psse_01.TXT", "wt");

fseek(fichero_entrada, 12, SEEK_SET);  // 0xC número de variables, y separador de filas
fscanf(fichero_entrada, "%4c", &x);  // Lee y almacena el número de variables
printf("Número de variables (sin contar Time) = %.0f\n", x);

long int longitud_fila = (x + 1) * 4;

fprintf(fichero_salida,"Tiempo(s), ");
fseek(fichero_entrada, byte1_titulos, SEEK_SET);

// son 32 caracteres por campo de cada título de variable
for (int n = 1; n <= x * 32; n++) {
    fscanf(fichero_entrada, "%c", &titulo);
    fprintf(fichero_salida,"%c",titulo);    
    if (!(n % 32)) {   
        fprintf(fichero_salida,",");   
    } 
}

fprintf(fichero_salida,"\n"); 

printf("Longitud total de Fila (Bytes) = %i + 4 Bytes de marcador\n", longitud_fila);

fseek(fichero_entrada, 0, SEEK_END);
longitud_archivo = ftell(fichero_entrada);
printf("Tamaño del archivo = %li\n",longitud_archivo);

fseek(fichero_entrada, 2320, SEEK_SET);

for (long int i = inicio_bloque; i <= longitud_archivo; i = i + longitud_fila + 4) {
  long int p = ftell(fichero_entrada);
  if (p <= longitud_archivo - 16) {
  for (long int n = 0; n <=longitud_fila/4 ; n++) {
    
    fscanf(fichero_entrada, "%4c", &x);
    if (!(n==69)) {
      fprintf(fichero_salida,"%.8f,",x);
      }
    }
  }
fprintf(fichero_salida,"\n");
}

fclose(fichero_entrada);
fclose(fichero_salida);

return 0;

}