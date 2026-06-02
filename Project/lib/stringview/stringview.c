/*
Course number: CSC 3350
Assignment Title: Wash Shell
Professor: Andy Cameron
Date: 06/02/2026
*/
#include "stringview.h"
#include <string.h>

int view_length(stringview* strview){
    if(strview == NULL || strview->start>strview->end){
        return -1;
    }
    return (int)(strview->end-strview->start);
}

int view_size(stringview* strview){
    int length = view_length(strview);
    if(length == -1){
        return -1;
    }
    return length+1;
}

void copy_nsubstring(stringview *strview, int n, char *dest){
    if(strview == NULL || dest == NULL || n <= 0){
        return;
    }
    int len = view_length(strview);
    if(len == -1){
        return;
    }
    if(n > len){
        n = len;
    }
    // Copy the stringview into the destination buffer for ease of use.
    memcpy(dest, strview->start, n);
    dest[n] = '\0';
}

void copy_substring(stringview* strview, char* dest){
    copy_nsubstring(strview, view_length(strview), dest);
}

stringview to_nstringview(char *start, int n){
    stringview view = {
        .start = NULL, 
        .end = NULL
    };

    if(start == NULL || n <= 0){
        return view;
    }
    
    int len = (int)strlen(start);
    if(len < n){
        return view;
    }
    //return the stringview struct with the appropriate values.
    view.start = start;
    view.end = start + n;
    return view;
}

stringview to_stringview(char *start, char *end){
    stringview view = {
        .start = NULL, 
        .end = NULL
    };

    if(start == NULL || end == NULL){
        return view;
    }

    if(start > end){
        return view;
    }
    //just calls the other implementation for simplicity sake.
    return to_nstringview(start, (int)(end - start));
}