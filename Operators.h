
    DEF_OPER( end , END,   0)

    DEF_OPER( ret , RET,   1)      //� ���
                                                //0 - �� ������ //���������� "� �����" ���� 0 ��� "� �����"
    DEF_OPER( +   , ADD,  10)      //� �����������

    DEF_OPER( -   , SUB,  11)      //���

    DEF_OPER( *   , MUL,  12)      //��
                                                                //� ����� '.'
    DEF_OPER( /   , DIV,  13)
                                   //��������� ��
    DEF_OPER( ^   , POW,  14)      //����������� ��

    DEF_OPER( =   , ASS,  15)      //����������� � ���

    DEF_OPER( get , GET,  16)      //������������, ����� �

    DEF_OPER( put , PUT,  17)      //��� ������� � ��������� ��������

    DEF_OPER( op  , OP ,  18)

    DEF_OPER( ;   , SEM,  19)      //����� ������� �������� x

    DEF_OPER( if  , IF ,  20)      //���� "" �� ������ ... ��, �������

    DEF_OPER( else, ELS,  21)      //����� ������� ... �� ��, ��� ���� ������

    DEF_OPER(if-else,ELF, 22)      //�� � ���� "" �� �������� ... �� � ��, �������

    DEF_OPER(while, WHL , 23)      //���� ""  //////////////////////////////////////////////////

    DEF_OPER( &=  , DEC , 24)

    DEF_OPER( ',' , COM,  25)

    DEF_OPER( sqr , SQR,  26)

    DEF_OPER( der , DER,  27)

    DEF_OPER( >   , JA ,  31)      //JNBE //�������

    DEF_OPER( >=  , JAE,  32)      //JNB  //������� �������

    DEF_OPER( ==  , JE ,  33)      //JNNE //�� �� �����, ���

    DEF_OPER( <=  , JBE,  34)      //JNA  //�� ��� ������

    DEF_OPER( <   , JB ,  35)      //JNAE //������� ����

    DEF_OPER( !=  , JNE,  36)      //JNE  //- ��� ��  //������ != 0


    //DEF_OPER( ,   , COM,  20)

