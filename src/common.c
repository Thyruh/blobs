double max(double a, double b) {
   return a > b ? a : b;
}

void int_to_string(int num, char* str) {
    int i = 0;
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    while (num > 0) {
        str[i++] = (num % 10) + '0';
        num /= 10;
    }

    for (int j = 0; j < i / 2; j++) {
        char tmp = str[j];
        str[j] = str[i - 1 - j];
        str[i - 1 - j] = tmp;
    }

    str[i] = '\0';
}


/* void print_vector2(Vector2* vec) { */
/* 	printf("(%f, %f)\n", vec->x, vec->y); */
/* } */
