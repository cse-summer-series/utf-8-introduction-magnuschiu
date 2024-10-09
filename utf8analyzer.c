#include <stdint.h>
#include <stdio.h>
#include <string.h>

int32_t is_ascii(char str[]){
    for(int i = 0; str[i] != '\0'; i++){
        if (str[i] < 0 || str[i] > 127){
            return 0;
        }
        i++;
    } 
    return 1;
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
int32_t width_from_start_byte(char start_byte){
    
    if((start_byte & 0b11000000) == 0b10000000){
        return -1; //not ascii/continutation starts with 0b10
    }
    else if((start_byte & 0b10000000) != 0b10000000){
        return 1; //ascii never starts with 0b1
    }
    else if((start_byte & 0b11100000) == 0b11000000){
        return 2;//two bytes 0b110
    }
    else if((start_byte & 0b11110000) == 0b11100000){
        return 3;//three bytes 0b1110
    }
    else if((start_byte & 0b11111000) == 0b11110000){
        return 4; // four bytes 0b11110
    }
    
}

int32_t utf8_strlen(char str[]){
    int count = 0;
    for(int i = 0; str[i] != '\0'; i++){
        count = count + width_from_start_byte(str[i]); //adds the -1 for two bit chars and etc
    }
    return count;
}

int32_t codepoint_index_to_byte_index(char str[], int32_t cpi){
    int codepointindx = 0;
    for(int byteindx = 0; str[byteindx] != cpi;){ //using byteindex to count instead of i *edit* i forgot that its better to use cpi cause i know the value
        int charlen = width_from_start_byte(str[byteindx]);
        if (codepointindx == cpi){ //check to see if index has been reached if so then return the index of the byte
            return byteindx;
        }
        byteindx += charlen; //since there are special characters must add them
        codepointindx++; 
        
        
    }
    return -1;
}

void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char res[]){
    if(cpi_start > cpi_end || cpi_end < 0|| cpi_start < 0){
        return;
    }
    int bytestart = codepoint_index_to_byte_index(str, cpi_start);//calling the function to save time
    int byteend = codepoint_index_to_byte_index(str, cpi_end);
    int resindx =0;
    for(int i=bytestart; i<byteend; i++){ //looping though bytestart to byteend and putting in the characters
        res[resindx] = str[i];
        resindx++;
    }
    res[resindx] = '\0';
}

int32_t codepoint_at(char str[], int32_t cpi){
    int codepointindx = 0;
    for(int byteindx = 0; str[byteindx] != cpi;){ //using byteindex to count instead of i *edit* i forgot that its better to use cpi cause i know the value
        int charlen = width_from_start_byte(str[byteindx]);
        if (codepointindx == cpi){ //check to see if index has been reached if so then return the index of the byte
            if(width_from_start_byte(str[byteindx]) == 1) {
                
                return str[byteindx];
            }
            else if(charlen == 2){
                
                return (str[byteindx] & 0b00011111) * 64 + (str[byteindx+1] & 0b00111111);
            }
            else if(charlen == 3){
                
                return (str[byteindx] & 0b00001111) * 4096 + (str[byteindx+1] & 0b00111111) * 64 + (str[byteindx+2] & 0b00111111);
            }
            else if(charlen == 4){
                
                return (str[byteindx] & 0b00000111) * 262144 +(str[byteindx+1] & 0b00111111) * 4096 +(str[byteindx+2] & 0b00111111) * 64 +(str[byteindx+3] & 0b00111111);   
            }
            else if(charlen == -1){
                return -1;
            }
    }

        byteindx += charlen; //since there are special characters must add them
        codepointindx++; 
        
        
    }
    return -1;

    
}
char is_animal_emoji_at(char str[], int32_t cpi){
    int32_t codep = codepoint_at(str, cpi);
    int32_t rat = codepoint_at("🐀", 0); //theres probably an easier way of doing this but i prefer doing it this way
    int32_t squirrel = codepoint_at("🐿️", 0);
    int32_t crab = codepoint_at("🦀", 0);
    int32_t dog = codepoint_at("🦮", 0);
    
    if((codep >= rat && codep<= squirrel)||(codep >= crab && codep <= dog)){
        return 1;
    }
    return 0;
}

int main(){
    printf("Enter a UTF-8 encoded string: ");
    char sentence[50]; //50 byte limit seems reasonable
    fgets(sentence, 50, stdin);
    
    //need this placeholder because capitalize directly changes the sentence
    char changing[50];
    char changing2[50];
    for(int i =0; sentence[i] != changing[i]; i++){
        changing[i] = sentence[i];
    }
    for(int j =0; sentence[j] != changing2[j]; j++){
        changing2[j] = sentence[j];
    }

    printf("Valid ASCII: %s\n", is_ascii(sentence)? "true":"false"); //found this online
    int32_t ret = capitalize_ascii(changing2);
    printf("Uppercased ASCII: %s\n", changing2);
    
    printf("Length in bytes: %d\n", codepoint_index_to_byte_index(sentence, utf8_strlen(sentence))); //almost tricked me! 
    
    printf("Number of Codepoints: %d\n", utf8_strlen(sentence));
    
    printf("Bytes per code point: ");
    for(int i =0; sentence[i] != '\0';){
        printf("%d ", width_from_start_byte(sentence[i]));
        i += width_from_start_byte(sentence[i]);
    }
    
    printf("\n");
    
    char res[30]; 
    
    utf8_substring(changing, 0, 6, res);
    
    printf("Substring of the first 6 code points: \"%s\"\n", res);
    printf("Code points as decimal numbers: ");
    for(int j = 0; sentence[j] != '\0';){
        int codepoint = codepoint_at(sentence, j); // Get code point at index
        if (codepoint != 0){
            printf("%d ",codepoint);
        }
        
        j+= width_from_start_byte(sentence[j]); 
    }
    printf("\n");
    printf("Animal emojis: ");
    char resu[30] = "";
    for (int k = 0; sentence[k] != '\0'; ) {
        if (is_animal_emoji_at(sentence, k) == 1) {
            utf8_substring(sentence, k, k+1, resu);
            printf("%s\n",resu);
        }
        
        k += width_from_start_byte(sentence[k]); // Move to the next character or emoji
        printf("%i", k);
    }
    printf("\n");    
    
    

}