
    DEF_OPER( end , END,   0)

    DEF_OPER( ret , RET,   1)      //с вас
                                                //0 - не готово //декларация "х стоит" если 0 или "х равно"
    DEF_OPER( +   , ADD,  10)      //с добавлением

    DEF_OPER( -   , SUB,  11)      //без

    DEF_OPER( *   , MUL,  12)      //на
                                                                //в конце '.'
    DEF_OPER( /   , DIV,  13)
                                   //разрезать на
    DEF_OPER( ^   , POW,  14)      //прожаренный на

    DEF_OPER( =   , ASS,  15)      //приготовить х как

    DEF_OPER( get , GET,  16)      //здравствуйте, можно х

    DEF_OPER( put , PUT,  17)      //вот держите х приятного аппетита

    DEF_OPER( op  , OP ,  18)

    DEF_OPER( ;   , SEM,  19)      //новая функция Заказать x

    DEF_OPER( if  , IF ,  20)      //Если "" то можете ... всё, отлично

    DEF_OPER( else, ELS,  21)      //Иначе давайте ... ну да, так тоже хорошо

    DEF_OPER(if-else,ELF, 22)      //Ну а если "" то наверное ... ну и всё, пожалуй

    DEF_OPER(while, WHL , 23)      //пока ""  //////////////////////////////////////////////////

    DEF_OPER( &=  , DEC , 24)

    DEF_OPER( ',' , COM,  25)

    DEF_OPER( sqr , SQR,  26)

    DEF_OPER( der , DER,  27)

    DEF_OPER( >   , JA ,  31)      //JNBE //вкуснее

    DEF_OPER( >=  , JAE,  32)      //JNB  //немного вкуснее

    DEF_OPER( ==  , JE ,  33)      //JNNE //то же самое, что

    DEF_OPER( <=  , JBE,  34)      //JNA  //не так вкусно

    DEF_OPER( <   , JB ,  35)      //JNAE //гораздо хуже

    DEF_OPER( !=  , JNE,  36)      //JNE  //- это не  //вкусно != 0


    //DEF_OPER( ,   , COM,  20)

