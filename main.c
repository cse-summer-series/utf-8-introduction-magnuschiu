#include <stdint.h>
#include <stdio.h>
#include <string.h>

int32_t is_ascii(char str[]){
    for(int i = 0; str[i] != '\0'; i++){
        return str[i] <= 127 && str[i]>0;
    } 
    
}
int main(){
    printf("Is 🔥 ASCII? %d\n", is_ascii("🔥"));

    printf("Is abcd ASCII? %d\n", is_ascii("abcd"));
}