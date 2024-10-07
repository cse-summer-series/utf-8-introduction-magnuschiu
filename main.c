#include <stdint.h>
#include <stdio.h>
#include <string.h>

int32_t is_ascii(char str[]){
    for(int i = 0; str[i] != '\0'; i++){
        return str[i] <= 127 && str[i]>0; //ascii must have more than 0 and less than 127 bytes
    } 
    
}
int count =0;
int32_t capitalize_ascii(char str[]){
    for(int i = 0; str[i] != '\0'; i++){
        if(str[i] >= 97 && str[i]<=122){//lowercase are ascii 97 to 122
            str[i] = str[i] -32; //lower to uppercase 
            count++; //this is ret, we already update the values of str in the previous line
        }
    }
    return count;
}

int main(){
    printf("Is 🔥 ASCII? %d\n", is_ascii("🔥"));

    printf("Is abcd ASCII? %d\n", is_ascii("abcd"));

    int32_t ret = 0;
    char str[] = "abcd";
    ret = capitalize_ascii(str);
    printf("Capitalized String: %s\nCharacters updated: %d\n", str, ret);
}