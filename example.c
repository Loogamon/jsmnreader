/*
* MIT License
*
* Copyright (c) 2023 Zachary Tabikh
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include <stdio.h>
#include "jsmnreader.h"

int main()
{
    int testload;
    char * txt;
    unsigned int i;
    unsigned int * arrays;
    unsigned int arrays_size;
	jsmnreader_obj myjsmn;

    jsmnreader_init(&myjsmn);
    testload = jsmnreader_fileload("library.json", &myjsmn);
	if (testload != JSMN_SUCCESS)
    {
        printf("Failure to load library.json\n");
        switch(testload)
        {
            case JSMN_ERROR_NOMEM: printf("Not enough or too much memory.\n"); break;
            case JSMN_ERROR_INVAL: printf("Invalid format.\n"); break;
            case JSMN_ERROR_PART: printf("Fragmented JSON.\n"); break;
            case JSMN_ERROR_NOFILE: printf("File not found.\n"); break;
        }
		return EXIT_FAILURE;
    }

    //Print String
    printf("\n====[File Contents]====\n");
    jsmnreader_print_string(&myjsmn);

    //Tokens
    printf("\n====[Tokens]====\n");
    jsmnreader_print_tokens(&myjsmn);

    //Tree Display
    printf("\n====[Tree Display]====\n");
    jsmnreader_tree_print("",0,&myjsmn);
    printf("\n");
    jsmnreader_tree_print("repository\\sub",0,&myjsmn);

    //Tree Get
    printf("\n====[Tree Get]====\n");
    printf("Int (number): %d\n", jsmnreader_tree_get_int("number",0,&myjsmn));
    printf("Negative int (signed): %d\n", jsmnreader_tree_get_int("signed",0,&myjsmn));
    printf("Unsigned int (signed): %u\n", jsmnreader_tree_get_int("signed",0,&myjsmn));
    printf("Int is true? (truthfully): %d\n", jsmnreader_tree_get_int("truthfully",0,&myjsmn));
    printf("Negative converted int (floating_neg): %d\n", jsmnreader_tree_get_int("floating_neg",0,&myjsmn));
    printf("Float (floating): %g\n", jsmnreader_tree_get_float("floating", 0, &myjsmn));
    printf("Float x2 (floating): %g\n", jsmnreader_tree_get_float("floating", 0, &myjsmn)*2);
    printf("Negative float (floating_neg): %g\n", jsmnreader_tree_get_float("floating_neg", 0, &myjsmn));

    txt = jsmnreader_tree_get_string("name", 0, &myjsmn);
    printf("String (name): %s\n", txt);
    free(txt);

    txt = jsmnreader_tree_get_string("repository\\type", 0, &myjsmn);
    printf("Sub-String (repository\\type): %s\n", txt);
    free(txt);

    txt = jsmnreader_tree_get_string("repository\\sub\\monkey", 0, &myjsmn);
    printf("Sub-String (repository\\sub\\monkey): %s\n", txt);
    free(txt);

    txt = jsmnreader_tree_get_string("dumb", 0, &myjsmn);
    printf("String with quotes (dumb): %s\n", txt);
    free(txt);

    txt = jsmnreader_tree_get_string("another", 0, &myjsmn);
    printf("String with blackslash (another): %s\n", txt);
    free(txt);

    printf("Obj (repository\\sub): %u\n", jsmnreader_tree_get_object("repository\\sub", 0, &myjsmn));
    printf("Array (examples): %u\n", jsmnreader_tree_get_array("examples", 0, &myjsmn));

    printf("Any, tree_get_x (repository\\sub\\reddit): %u\n", jsmnreader_tree_get_any("repository\\sub\\reddit", 0, &myjsmn));
    printf("Any, tree_get_any (frameworks): %u\n", jsmnreader_tree_get_any("frameworks", 0, &myjsmn));
    printf("Any, tree_anyprint (examples): "); jsmnreader_tree_anyprint("examples", 0, &myjsmn);

    //Token Get
    printf("\n====[Token Get]====\n");
    printf("Int (59): %d\n", jsmnreader_token_get_int(59, &myjsmn));
    printf("Float (92): %g\n", jsmnreader_token_get_float(92, &myjsmn));

    txt = jsmnreader_token_get_string(39, &myjsmn);
    printf("String (39): %s\n", txt);
    free(txt);

    printf("Object (51): %d\n", jsmnreader_token_get_object(51, &myjsmn));
    printf("Array (47): %d\n", jsmnreader_token_get_array(47, &myjsmn));
    printf("Token Size (47): %d\n", jsmnreader_token_size(47, &myjsmn));

    //Arrays
    printf("\n====[Arrays]====\n");
    printf("Array Tokens (47)\n");
    jsmnreader_token_array_tokens(&arrays, &arrays_size, 47, &myjsmn); //22, 31
    for (i = 0; i < arrays_size; i++)
    {
        if (i > 0)
            printf(", ");
        txt = jsmnreader_token_get_string(*(arrays+i), &myjsmn);
        printf("%s (%d)", txt , *(arrays+i));
        free(txt);
		}
    printf("\n");
    printf("Token Array (1,47): %d", jsmnreader_token_array(1, 47, &myjsmn));
    txt = jsmnreader_token_get_string(jsmnreader_token_array(1, 47, &myjsmn), &myjsmn);
    printf(" [%s]", txt);
    free(txt);
    printf("\n\n====[Objects]====\n");
    printf("Object Tokens (0,JSMNR_KEYONLY)\n");

    free(arrays);
    jsmnreader_token_object_tokens(&arrays, &arrays_size, 0, JSMNR_KEYONLY, &myjsmn); //22, 31
    for (i = 0; i < arrays_size; i++)
    {
        if (i > 0)
            printf(",");
        printf("%d",*(arrays+i));
    }
    printf("\n");
    printf("Object Array (2,0,JSMNR_KEYONLY): %d", jsmnreader_token_object(2, 0, JSMNR_KEYONLY, &myjsmn));

    printf("\n");
    free(arrays);
    jsmnreader_free(&myjsmn);
    return EXIT_SUCCESS;
}
