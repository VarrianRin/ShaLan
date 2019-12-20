#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys\stat.h>
#include <math.h>
#include <locale.h>
#include "Enum.h"

/*
TreeOk, ненулевую инфу, цвета, nill,
*/

#define CHECKNULLPTR(ptr) if(!ptr) { /*printf("\n%s: NULL POINTER TO "#ptr"!\n", __PRETTY_FUNCTION__); */return NULL; }
#define CHECKTREE(tree)   if(TreeOk(tree)) TreeDump(tree);

#define dL                  Diff(elem->left, TreeTex)
#define dR                  Diff(elem->right, TreeTex)
#define cL                  CopyElem(elem->left)
#define cR                  CopyElem(elem->right)
#define LN(right)           CreateElem(FUNCTION, OPER_LN , NULL, right)
#define COS(right)          CreateElem(FUNCTION, OPER_COS, NULL, right)
#define SIN(right)          CreateElem(FUNCTION, OPER_SIN, NULL, right)
#define VAR(ID)             CreateElem(VARIABLE, ID)
#define END                 CreateElem(OPERATOR, OPER_END, NULL, NULL )
#define NUM(number)         CreateElem(NUMBER  , number)
#define PLUS(left, right)   CreateElem(OPERATOR, OPER_ADD, left, right)
#define MINUS(left, right)  CreateElem(OPERATOR, OPER_SUB, left, right)
#define MUL(left, right)    CreateElem(OPERATOR, OPER_MUL, left, right)
#define DIV(left, right)    CreateElem(OPERATOR, OPER_DIV, left, right)
#define POW(left, right)    CreateElem(OPERATOR, OPER_POW, left, right)
#define OPER(left, right)   CreateElem(OPERATOR, OPER_OP , left, right)
#define SEM(left, right)    CreateElem(OPERATOR, OPER_SEM, left, right)
#define ASS(left, right)    CreateElem(OPERATOR, OPER_ASS, left, right)
#define DEC(left, right)    CreateElem(OPERATOR, OPER_DEC, left, right)
#define RET(left)           CreateElem(OPERATOR, OPER_RET, left)
#define GET(left)           CreateElem(OPERATOR, OPER_GET, left)
#define PUT(left)           CreateElem(OPERATOR, OPER_PUT, left)
#define SQR(left)           CreateElem(OPERATOR, OPER_SQR, left)
#define IF(left, right)     CreateElem(OPERATOR, OPER_IF , left, right)
#define JA(left, right)     CreateElem(OPERATOR, OPER_JA , left, right)
#define JAE(left, right)    CreateElem(OPERATOR, OPER_JAE, left, right)
#define JE(left, right)     CreateElem(OPERATOR, OPER_JE , left, right)
#define JBE(left, right)    CreateElem(OPERATOR, OPER_JBE, left, right)
#define JB(left, right)     CreateElem(OPERATOR, OPER_JB , left, right)
#define JNE(left, right)    CreateElem(OPERATOR, OPER_JNE, left, right)
#define COM(left, right)    CreateElem(OPERATOR, OPER_COM, left, right)
#define FUN(id,left, right) CreateElem(FUNCTION, id      , left, right)

#define OPER_ADD(elem)      Calculate(elem->left) + Calculate(elem->right)
#define OPER_SUB(elem)      Calculate(elem->left) - Calculate(elem->right)
#define OPER_MUL(elem)      Calculate(elem->left) * Calculate(elem->right)
#define OPER_DIV(elem)      Calculate(elem->left) / Calculate(elem->right)
#define OPER_POW(elem)      pow(Calculate(elem->left), Calculate(elem->right))

const int POISON     = -1073676287,
          N_VAR      = 20,
          NAMESIZE   = 20,
          OPERSIZE   = 50,
          NUMBER     = 0,
          VARIABLE   = 1,
          OPERATOR   = 2,
          FUNCTION   = 3,
          NULL_POINT = 1,
          HEAD_ERROR = 2;

const char* str = "";

struct Variable {

    char* name;
    int   func;
};

struct Function {

    char* name;
    int   n_param;
};

struct Element {

    char     type;
    int      data;
    Element* right;
    Element* left;
};

struct Tree {

    Element*    head;
    Variable*   VarList;
    int         VarId;
    Function*   FuncList;
    int         FuncId;
    int         IF;
};

int TreeDump(const Tree* tree);

int TreeInit(Tree* tree);

int TreeCoder(const Tree* tree, const char* FileName);

int TreeDecoder(Tree* tree, const char* FileName);

int TreeKill(Tree* tree);

int TreeAsm(Tree* tree, const char* AsmFile);

char* WinReader (const char* FileName, int SizeFile);

int GetFileSize(const char* FileName);

Element* GetG(Tree* tree, const char* data);

Element* GetE(Tree* tree);

int main() {

    //setlocale(LC_ALL, "Rus");
    //system("chcp 1251");

    Tree tree = {};
    TreeInit(&tree);

    int SizeFile = GetFileSize("ShaLan.txt");
    char* code = WinReader("ShaLan.txt", SizeFile);
    tree.head = GetG(&tree, code);

    //TreeDecoder(&tree, "TreeCode.txt");

    TreeDump(&tree);

    TreeAsm(&tree, "AsmFile.txt");
    free(code);
    TreeKill(&tree);

    return 0;
}

char* WinReader(const char* FileName, int SizeFile) {

    assert(FileName != NULL);

    FILE* TextFile = fopen(FileName, "rb");
    assert(TextFile != NULL);

    char* text  = (char*) calloc(SizeFile + 1, sizeof(text[0]));

    assert(text != NULL);

    int ReadReturn = fread(text, sizeof(text[0]), SizeFile, TextFile);
    assert (ReadReturn == SizeFile);
    fclose(TextFile);

    text[SizeFile] = '\0';

    return text;
}

int GetFileSize(const char* FileName) {

    assert(FileName != NULL);

    struct _stat buff = {};
    int StatReturn = _stat(FileName, &buff);
    assert(StatReturn == 0);

    return buff.st_size;
}

int TreeOk(const Tree* tree) {

    if (!tree || !tree->FuncList || !tree->VarList)
        return NULL_POINT;

    if (!tree->head)
        return HEAD_ERROR;

    return 0;
}

bool TypeDump(const Tree* tree, const Element* elem, FILE* TreeDump) {

    if (elem->type == OPERATOR && elem->data != OPER_COM) {

        #define DEF_OPER(operator, name, code) case code:\
                                             fprintf(TreeDump, "\""#operator"\\nelem: %p\\nleft: %p\\nright: %p\"", elem, elem->left, elem->right);\
                                             break;
        switch(elem->data) {
            #include "Operators.h"
        }

        #undef DEF_OPER
    }
    else if (elem->data == OPER_COM)
        fprintf(TreeDump, "\",\\nelem: %p\\nleft: %p\\nright: %p\"", elem, elem->left, elem->right);

    else if (elem->type == FUNCTION)
        fprintf(TreeDump, "\"%s\\nelem: %p\\nleft: %p\\nright: %p\"", tree->FuncList[elem->data].name, elem, elem->left, elem->right);

    else if (elem->type == NUMBER)
        fprintf(TreeDump, "\"number: %d\\nelem: %p\\nleft: %p\\nright: %p\"", elem->data, elem, elem->left, elem->right);

    else if (elem->type == VARIABLE)
        fprintf(TreeDump, "\"%s\\nelem: %p\\nleft: %p\\nright: %p\"", tree->VarList[elem->data].name, elem, elem->left, elem->right);

    return 1;
}

void ColorDump(const Tree* tree, const Element* elem, FILE* TreeDump) {

    if (elem->type == OPERATOR && elem->data != OPER_COM) {

        #define DEF_OPER(operator, name, code) case code:\
                                             fprintf(TreeDump, "\n\""#operator"\\nelem: %p\\nleft: %p\\nright: %p\"[style=\"filled\",fillcolor=\"deeppink\"];\n", elem, elem->left, elem->right);\
                                             break;
        switch(elem->data) {
            #include "Operators.h"
        }

        #undef DEF_OPER
    }
    else if (elem->data == OPER_COM)
        fprintf(TreeDump, "\",\\nelem: %p\\nleft: %p\\nright: %p\"[style=\"filled\",fillcolor=\"deeppink\"];\n", elem, elem->left, elem->right);

    else if (elem->type == NUMBER)
        fprintf(TreeDump, "\n\"number: %d\\nelem: %p\\nleft: %p\\nright: %p\"[style=\"filled\",fillcolor=\"lightslateblue\"];\n", elem->data, elem, elem->left, elem->right);

    else if (elem->type == VARIABLE)
        fprintf(TreeDump, "\n\"%s\\nelem: %p\\nleft: %p\\nright: %p\"[style=\"filled\",fillcolor=\"yellow2\"];\n", tree->VarList[elem->data].name, elem, elem->left, elem->right);


}

bool ElemDump(const Tree* tree, const Element* elem, FILE* TreeDump) {

    TypeDump(tree, elem, TreeDump);
    ColorDump(tree, elem, TreeDump);
    TypeDump(tree, elem, TreeDump);

    if (elem->left) {
        fprintf(TreeDump, "->");
        ElemDump(tree, elem->left, TreeDump);
    }

    fprintf(TreeDump, "\n");
    TypeDump(tree, elem, TreeDump);

    if (elem->right) {
        fprintf(TreeDump, "->");
        ElemDump(tree, elem->right, TreeDump);
    }

    return 1;
}

int TreeDump(const Tree* tree) {

    CHECKNULLPTR(tree)

    FILE* TreeDump = fopen("TreeDump.txt", "w");
    if (!TreeDump) {
        printf("\n COULD NOT OPEN DUMP FILE\n");
        return 0;
    }

    fprintf(TreeDump, "digraph TreeDump {\n");

    ElemDump(tree, tree->head, TreeDump);

    fprintf(TreeDump, "\n}");

    fclose(TreeDump);

    system("dot -Tjpg -o F:\\proga\\Language\\TreeDump.jpg F:\\proga\\Language\\TreeDump.txt");
    system("F:\\proga\\Language\\TreeDump.jpg");

    return 1;
}

int TreeInit(Tree* tree) {

    CHECKNULLPTR(tree)

    Element* reserve = (Element*)calloc(1, sizeof(Element));
    if (!reserve) {
        printf("COULD NOT CALLOC HEAD");
        return 0;
    }
    tree->head = reserve;

    tree->FuncList = (Function*)calloc(N_VAR, sizeof(Function));
    tree->FuncId = 1;
    tree->FuncList[0].name = strdup("$main");

    tree->VarList  = (Variable*)calloc(N_VAR, sizeof(Variable));
    for (int i = 0; i < N_VAR; i++)
        tree->VarList[i].func == -1;
    tree->VarId = 0;
    tree->IF = 0;

    CHECKTREE(tree)
    return 1;
}

int VarId(Tree* tree, const char* name) {

    for(int i = 0; i < tree->VarId; i++)
        if ( !strcmp(tree->VarList[i].name, name) )
            return i;

    return -1;
}

int FuncId(Tree* tree, const char* name) {
                                           //printf("F:'%s', '%s'", name, tree->FuncList[tree->FuncId - 1].name + 1);
    for(int i = 0; i < tree->FuncId; i++)
        if ( !strcmp(tree->FuncList[i].name + 1, name) )
            return i;

    return -1;
}

int ChangeData(Tree* tree, Element* elem, const char* data) {

    CHECKNULLPTR(elem)
    CHECKNULLPTR(data)

    #define DEF_OPER(operator, name, code)     if ( !strcmp(#operator, data) ) {        \
                                               elem->type = OPERATOR;                   \
                                               elem->data = code;                       \
                                               return OPERATOR;                         \
                                           }
    #include "Operators.h"

    #undef DEF_OPER

    if (*data == '$') {

        elem->type = FUNCTION;
        int id = FuncId(tree, data);
        elem->data = (id == -1) ? tree->FuncId : id;

        if (id == -1)
            tree->FuncList[tree->FuncId++].name = strdup(data);
        return FUNCTION;
    }

    if (isdigit(*data)) {

        elem->type = NUMBER;
        elem->data = atoi(data);
        return NUMBER;
    }

    if (isalpha(*data)) {

        elem->type = VARIABLE;
        int id = VarId(tree, data);
        elem->data = (id == -1) ? tree->VarId : id;

        if (id == -1)
            tree->VarList[tree->VarId++].name = strdup(data);
        return VARIABLE;
    }

    return -1;
}

Element* AddRightElem(Element* elem, Tree* tree = nullptr, const char* data = nullptr) {

    CHECKNULLPTR(elem)

    Element* son = (Element*)calloc(1, sizeof(Element));

    if (data)
        ChangeData(tree, son, data);

    elem->right = son;

    return son;
}

Element* AddLeftElem(Element* elem, Tree* tree = nullptr, const char* data = nullptr) {

    CHECKNULLPTR(elem)

    Element* son = (Element*)calloc(1, sizeof(Element));

    if (data)
        ChangeData(tree, son, data);

    elem->left = son;

    return son;
}

bool TypeCoder(const Tree* tree, const Element* elem, FILE* TreeCode) {

    if (elem->type == OPERATOR) {

        #define DEF_OPER(operator, name, code) case code:\
                                             fprintf(TreeCode, #operator);\
                                             break;
        switch(elem->data) {
            #include "Operators.h"
        }

        #undef DEF_OPER
    }
    else if (elem->type == FUNCTION)
        fprintf(TreeCode, "%s", tree->FuncList[elem->data].name);

    else if (elem->type == NUMBER)
        fprintf(TreeCode, "%d", elem->data);

    else if (elem->type == VARIABLE)
        fprintf(TreeCode, "%s", tree->VarList[elem->data].name);

    return 1;
}

bool Coder(const Tree* tree, const Element* elem, FILE* TreeCode) {

    fprintf(TreeCode, "(");

    if (elem->left)
        Coder(tree, elem->left, TreeCode);

    TypeCoder(tree, elem, TreeCode);

    if (elem->right)
        Coder(tree, elem->right, TreeCode);

    fprintf(TreeCode, ")");

    return 1;
}
 /*
int TreeCoder(const Tree* tree, const char* FileName) {

    CHECKTREE(tree)
    CHECKNULLPTR(FileName)

    FILE* TreeCode = fopen(FileName, "w");
    if (!TreeCode) {
        printf("COULD NOT OPEN FILE");
        return 0;
    }

    Coder(tree->head, TreeCode);

    fclose(TreeCode);
    CHECKTREE(tree)
    return 1;
}
*/

bool Decoder(Tree* tree, Element* elem, const char* TreeCode, int* offset) {///////WITHOUT SPACES!!

    char symbol          = 0;
    int  n_bytes         = 0;
    char data[NAMESIZE]  = {};

    sscanf(TreeCode + *offset, "%[^{}@]%n", data, &n_bytes);

    *offset += n_bytes; printf("'%s'\n", data);
    ChangeData(tree, elem, data);
                                              printf("%c", *(TreeCode + *offset));
    if (*(TreeCode + *offset) == '}') {
        *offset -= 1;
        return 0;
    }

    sscanf(TreeCode + *offset, "%c%n", &symbol, &n_bytes);
    *offset += n_bytes;

    if (symbol != '@') {

        Decoder(tree, AddLeftElem(elem), TreeCode, offset);
        sscanf(TreeCode + *offset, "}%n", &n_bytes);
        *offset += n_bytes;
    }
    else
        *offset += 1;

    sscanf(TreeCode + *offset, "%c%n", &symbol, &n_bytes);
    *offset += n_bytes;

    if (symbol != '@') {

        Decoder(tree, AddRightElem(elem), TreeCode, offset);
        sscanf(TreeCode + *offset, "}%n", &n_bytes);
        *offset += n_bytes;
    }
    else
        *offset += 1;

    return 0;
}

int TreeDecoder(Tree* tree, const char* FileName) {

    CHECKTREE(tree)
    CHECKNULLPTR(FileName)

    int SizeFile = GetFileSize(FileName);
    char* TreeCode = WinReader(FileName, SizeFile);

    int offset = 0;
    sscanf(TreeCode + offset, "{");
    offset++;

    Decoder(tree, tree->head, TreeCode, &offset);

    free(TreeCode);
    CHECKTREE(tree)
    return 1;
}

int Search(const Element* elem, int type, int data = POISON) {

    CHECKNULLPTR(elem)

    if (data != POISON) {

        if (elem->type == type && elem->data == data)
            return 2;

        int ret = 0;

        if (elem->left && (ret = Search(elem->left, data)) != 0)
            return 10 * ret + 1;

        if (elem->right && (ret = Search(elem->right, data)) != 0)
            return 10 * ret;
    }
    else {
        //printf("%d, ", elem->type);
        if (elem->type == type)
            return 2;

        int ret = 0;

        if (elem->left && (ret = Search(elem->left, type)) != 0)
            return 10 * ret + 1;

        if (elem->right && (ret = Search(elem->right, type)) != 0)
            return 10 * ret;
    }
    return 0;
}

bool KillElem(Element* elem) {//doesnt free elem

    if (elem->left) {

        KillElem(elem->left);

        free(elem->left);
        elem->left = NULL;
    }

    if (elem->right) {

        KillElem(elem->right);

        free(elem->right);
        elem->right = NULL;
    }

    elem->data = 0;
    elem->type = 0;

    return 0;
}

int TreeKill(Tree* tree) {

    CHECKTREE(tree)
    KillElem(tree->head);

    free(tree->head);
    tree->head = NULL;

    while(tree->FuncId > 0)
        free(tree->FuncList[--tree->FuncId].name);
    while(tree->VarId  > 0)
        free(tree->VarList[--tree->VarId].name);

    free(tree->FuncList);
    free(tree->VarList);

    return 1;
}

int Condition(Element* elem, FILE* asmfile, Tree* tree);

int FuncDec(Element* elem, FILE* asmfile, Tree* tree);

int VarDec(Element* elem, FILE* asmfile, Tree* tree);

Element* GetOP(Tree* tree);

void Params(Element* elem, FILE* asmfile, Tree* tree) {

    if (elem->right)
        fprintf(asmfile, "%s,", tree->VarList[elem->right->data].name);

    if (elem->left)
        Params(elem->left, asmfile, tree);
}

int ElemAsm(Element* elem, FILE* asmfile, Tree* tree, bool pop = 0) {
                                                                      //////////pop = 0 -> push var and num
    if (elem->type == NUMBER) {
        //printf("\nN: %p", elem);
        if (pop == 0)
            fprintf(asmfile, "PUSH %d\n", elem->data);

        return NUMBER;
    }

    if (elem->type == VARIABLE) {
        //printf("\nV: %p", elem);
        if (!pop)
            fprintf(asmfile, "PUSH [%s]\n", tree->VarList[elem->data].name);
        if (pop)
            fprintf(asmfile, "POP [%s]\n", tree->VarList[elem->data].name);
        return VARIABLE;
    }

    if (elem->type == OPERATOR) {
        //printf("\nO: %p", elem);
        switch(elem->data) {

            case OPER_IF:

                if (elem->left->type == OPERATOR && elem->left->data >= OPER_JA && elem->left->data <= OPER_JNE) {

                    int IF = Condition(elem->left, asmfile, tree);
                    ElemAsm(elem->right, asmfile, tree);

                    fprintf(asmfile, "if_%d:", IF);
                    fprintf(asmfile, "\nPOP\nPOP\n");

                }
                return OPERATOR;

            case OPER_GET:

                fprintf(asmfile, "IN\nPOP [%s]\n", tree->VarList[elem->left->data].name);
                return OPERATOR;

            case OPER_PUT:

                ElemAsm(elem->left, asmfile, tree);
                fprintf(asmfile, "\nOUT\n");
                return OPERATOR;

            case OPER_ADD:

                ElemAsm(elem->left, asmfile, tree);
                ElemAsm(elem->right, asmfile, tree);
                fprintf(asmfile, "\nADD\n");
                return OPERATOR;

            case OPER_SUB:

                ElemAsm(elem->left, asmfile, tree);
                ElemAsm(elem->right, asmfile, tree);
                fprintf(asmfile, "\nSUB\n");
                return OPERATOR;

            case OPER_MUL:

                ElemAsm(elem->left, asmfile, tree);
                ElemAsm(elem->right, asmfile, tree);
                fprintf(asmfile, "\nMUL\n");
                return OPERATOR;

            case OPER_DIV:

                ElemAsm(elem->left, asmfile, tree);
                ElemAsm(elem->right, asmfile, tree);
                fprintf(asmfile, "\nDIV\n");
                return OPERATOR;

            case OPER_SQR:

                ElemAsm(elem->left, asmfile, tree);
                fprintf(asmfile, "\nSQRT\n");
                return OPERATOR;

            case OPER_ASS:

                ElemAsm(elem->right, asmfile, tree);
                ElemAsm(elem->left, asmfile, tree, 1);
                return OPERATOR;

            case OPER_DEC:

                fprintf(asmfile, "DEC [%s]", tree->VarList[elem->data].name);
                ElemAsm(elem->right, asmfile, tree);
                ElemAsm(elem->left, asmfile, tree, 1);
                return OPERATOR;

            case OPER_OP:

                if (elem->left)  ElemAsm(elem->left, asmfile, tree);
                if (elem->right) ElemAsm(elem->right, asmfile, tree);
                return OPERATOR;

            case OPER_RET:

                ElemAsm(elem->left, asmfile, tree);
                fprintf(asmfile, "\nPOP [ax]\nRET\n\n");
                return OPERATOR;

            case OPER_END:

                fprintf(asmfile, "\nEND\n");
                return OPERATOR;

            case OPER_SEM:

                if (elem->left && elem->left->type == OPERATOR && elem->left->data == OPER_DEC) {
                    VarDec(elem->left, asmfile, tree);
                    if(!elem->right || elem->right->left->type == FUNCTION)
                        fprintf(asmfile, "CALL $main\n");
                }
                if (elem->right)  ElemAsm(elem->right, asmfile, tree);
                if (elem->left && elem->left->type == FUNCTION) FuncDec(elem->left, asmfile, tree);
                return OPER_SEM;
        }

    }

    if (elem->type == FUNCTION) {
        //printf("\nF: %p", elem);
        if (tree->FuncList[elem->data].n_param == 0) {
            if (!pop) {
                fprintf(asmfile, "\nCALL %s\nPUSH [ax]\n", tree->FuncList[elem->data].name);
                //ElemAsm(
            }
            return FUNCTION;
        }
        else {
            fprintf(asmfile, "\nCALL %s [", tree->FuncList[elem->data].name);
            Params(elem->left, asmfile, tree);
            fprintf(asmfile, "]\n");
            return FUNCTION;
        }
    }

    return POISON;
}

int VarDec(Element* elem, FILE* asmfile, Tree* tree) {
   //printf("%d", elem->left->data);
    fprintf(asmfile, "\nDEC [%s]\n", tree->VarList[elem->left->data].name);

    ElemAsm(elem->right, asmfile, tree);

    fprintf(asmfile, "POP [%s]\n\n", tree->VarList[elem->left->data].name);
     //printf("f");
    return elem->data;
}

int FuncDec(Element* elem, FILE* asmfile, Tree* tree) {

    fprintf(asmfile, "%s:", tree->FuncList[elem->data].name);

    if (tree->FuncList[elem->data].n_param == 0) {

        fprintf(asmfile, "\n");
        ElemAsm(elem->right, asmfile, tree);
        fprintf(asmfile, "\n");
        return elem->data;
    }
    else {

        fprintf(asmfile, " [", tree->FuncList[elem->data].name);
        Params(elem->left, asmfile, tree);
        fprintf(asmfile, "]\n");

        ElemAsm(elem->right, asmfile, tree);
        fprintf(asmfile, "\n");
        return elem->data;
    }

}

int Condition(Element* elem, FILE* asmfile, Tree* tree) {

    ElemAsm(elem->left, asmfile, tree);
    fprintf(asmfile, "\n");

    ElemAsm(elem->right, asmfile, tree);
    fprintf(asmfile, "\n");
                           //мы прыгаем если условие НЕ выполнено => тут нужно противоположное сравнение (см. Operators.h)
    #define DEF_OPER(operator, name, code)  if ( code == ((elem->data - 31) + 3) % 6 + 31) {    \
                                                fprintf(asmfile, #name" if_%d\n", tree->IF);    \
                                                return tree->IF++;                              \
                                            }
    #include "Operators.h"

    #undef DEF_OPER

    return 0;
}

int TreeAsm(Tree* tree, const char* AsmFile) {

    CHECKNULLPTR(tree)
    CHECKNULLPTR(AsmFile)

    FILE* asmfile = fopen(AsmFile, "wb");
    CHECKNULLPTR(asmfile)


    ElemAsm(tree->head, asmfile, tree);

    fclose(asmfile);
    return 1;
}


Element* CreateElem(char type, int data, Element* left = nullptr, Element* right= nullptr) {

    Element* elem = (Element*)calloc(1, sizeof(Element));

    elem->type = type;
    elem->data = data;

    elem->left = left;
    elem->right = right;

    return elem;
}

/*Element* Diff(const Element* elem, FILE* TreeTex) {

    switch(elem->type) {

        case NUMBER:
            NumTex(elem->data, TreeTex);
            return NUM(0);

        case VARIABLE:
            VarTex(elem->data, TreeTex);
            return NUM(1);

        case OPERATOR: {

            int var_l = Search(elem->left, VARIABLE);
            int var_r = Search(elem->right, VARIABLE);

            OperTexBeg(elem, TreeTex, var_l, var_r);
            Element* node = NULL;

            if (!var_l && !var_r)
                return NUM(0);

            switch(elem->data) {

                case OPER_ADD:
                    if      ( var_l && !var_r) node = dL;
                    else if (!var_l &&  var_r) node = dR;
                    else node = PLUS(dL, dR);
                    OperTexEnd(elem, TreeTex, var_l, var_r);
                    return node;

                case OPER_SUB:
                    if      ( var_l && !var_r) node = dL;
                    else if (!var_l &&  var_r) node = MUL( NUM(-1), dR);
                    else node = MINUS(dL, dR);
                    OperTexEnd(elem, TreeTex, var_l, var_r);
                    return node;

                case OPER_MUL:
                    if      ( var_l && !var_r) node = MUL(dL, cR);
                    else if (!var_l &&  var_r) node = MUL(cL, dR);
                    else node = PLUS( MUL(dL, cR), MUL(cL, dR));
                    OperTexEnd(elem, TreeTex, var_l, var_r);
                    return node;

                case OPER_DIV:
                    if      ( var_l && !var_r) node = DIV(dL, cR);
                    else if (!var_l &&  var_r) node = DIV( MINUS( NUM(0), MUL(cL, dR)), MUL(cR, cR));
                    else node = DIV( MINUS( MUL(dL, cR), MUL(cL, dR)), MUL(cR, cR));
                    OperTexEnd(elem, TreeTex, var_l, var_r);
                    return node;

                case OPER_POW:
                    if      ( var_l && !var_r) node = MUL( MUL( POW(cL, MINUS(cR, NUM(1))), cR), dL);
                    else if (!var_l &&  var_r) node = MUL( MUL( POW(cL, cR), LN(cL)), dR);
                    else node = MUL( POW(cL, cR), PLUS( MUL( DIV(dL, cL), cR), MUL( LN(cL), dR)));
                    OperTexEnd(elem, TreeTex, var_l, var_r);
                    return node;
            }
        }

        case FUNCTION:

            int var_r = Search(elem->right, VARIABLE);
            OperTexBeg(elem, TreeTex, 0, var_r);
            Element* node = NULL;

            if (!var_r)
                return NUM(0);

            switch(elem->data) {

                case OPER_LN:
                    //fprintf(TreeTex, "\n(ln(x))' = 1/x * x'\n");
                    node = DIV(dR, cR);
                    OperTexEnd(elem, TreeTex, 0, var_r);
                    return node;

                case OPER_SIN:
                    //fprintf(TreeTex, "\n(sin(x))' = cos(x) * x'\n");
                    node = MUL(COS(cR), dR);
                    OperTexEnd(elem, TreeTex, 0, var_r);
                    return node;

                case OPER_COS:
                    //fprintf(TreeTex, "\n(cos(x))' = -sin(x) * x'\n");
                    node = MUL( MUL( NUM(-1), SIN(cR)), dR);
                    OperTexEnd(elem, TreeTex, 0, var_r);
                    return node;
            }
    }

    return NULL;
}
  */

void SpaceKill() {

    while(isspace(*str))
        str++;
}

Element* GetPa(Tree* tree, int id) {      ////////обратный порядок

    assert(*str == 'с');
    Element* node = NULL;
    char op[OPERSIZE] = {};
    int offset = 0;
    int i = 0;

    for ( ; i < N_VAR && *str != ':' && *str != '.'; i++) {

        str++;
        sscanf(str, " %[^,:.]%n", op, &offset);
        str += offset;

        tree->VarList[tree->VarId].name = strdup(op);
        tree->VarList[tree->VarId].func = id;

        node = COM(node , VAR(tree->VarId++));

    }
    tree->FuncList[id].n_param = i;
    str++;
    return node;
}

Element* GetId(Tree* tree) {

    char name[NAMESIZE] = {};

    for(int i = 0; isalpha(*str); i++, str++)
        name[i] = *str;
                                      //printf("%s\n", name);
    SpaceKill();

    int id = VarId(tree, name);

    if (id == -1) {
        tree->VarList[tree->VarId].name = strdup(name);
        return VAR(tree->VarId++);
    }

    else if (id >= 0)
        return VAR(id);

    return NULL;
}

Element* GetF(Tree* tree) {

    SpaceKill();

    Element* node = NULL;
    char name[NAMESIZE] = {};
    int offset;

    sscanf(str, "%s", name);
    assert( !strcmp("Блюдо", name));
    str += 5;
    SpaceKill();

    sscanf(str, "%[^.0-9\n]%n", name, &offset);  //printf("'%s'", name);
    str += offset;

    SpaceKill();
    int id = FuncId(tree, name);
    assert(id != -1);

    if (tree->FuncList[id].n_param) {

        assert(*str == 'с');
        node = GetPa(tree, id);

        assert(*(str - 1) == '.');
        str++;
    }
    return FUN(id, node, NULL);
}

Element* GetN() {

    int val = 0;

    assert(isdigit(*str));

    while (isdigit(*str)) {

        val = val * 10 + *str - '0';
        str++;
    }
    SpaceKill();
    return NUM(val);
}

Element* GetP(Tree* tree) {

    SpaceKill();

    if (*str == '(') {

        str++;
        Element* node = GetE(tree); //printf("'%c'", *str);

        assert(*str == ')');
        str++;
        return node;
    }
    else if (*str == 'Б')
        return GetF(tree);
    else if (isalpha(*str))
        return GetId(tree);
    else
        return GetN();
}

Element* GetPo(Tree* tree) {

    Element* node = GetP(tree);
     SpaceKill();
    char op[OPERSIZE] = {};
    int offset = 0;


    while (*str == 'п') {

        sscanf(str, "%21[^.\n]%n", op, &offset);   //printf("'%s'", op);
        assert(!strcmp(op, "поджарить на половину"));

        str += offset;
        SpaceKill();

        node = SQR(node);
    }
     SpaceKill();
    return node;
}

Element* GetT(Tree* tree) {

    SpaceKill();
    Element* node = GetPo(tree);
    char op[OPERSIZE] = {};
    int offset = 0;

    while (*str == 'н' || *str == 'р' || *str == 'и') {

        sscanf(str, "%[^a-zA-Z0-9А-Я().]%n", op, &offset);   //printf("'%s'", op);
        assert(!strcmp(op, "разрезать на ") || !strcmp(op, "на ") || !strcmp(op, "и поперчить"));
        char oper = *str;
        str += offset;
        SpaceKill();

        if (oper == 'н')
            node = MUL(node, GetPo(tree));
        else if (oper == 'и')
            node = MUL(node, NUM(-1));
        else
            node = DIV(node, GetPo(tree));
    }
    SpaceKill();
    return node;
}

Element* GetE(Tree* tree) {

    SpaceKill();
    Element* node = GetT(tree);
    char op[OPERSIZE] = {};
    int offset = 0;
    //printf("'%c'", *str);
    while (*str == 'с' || *str == 'б') {

        sscanf(str, "%[^a-zA-Z0-9А-Я()]%n", op, &offset);   //printf("'%s'", op);
        assert(!strcmp(op, "с добавлением ") || !strcmp(op, "без "));
        char oper = *str;
        str += offset;
        SpaceKill();

        if (oper == 'с')
            node = PLUS(node, GetT(tree));
        else
            node = MINUS(node, GetT(tree));   //printf("'%c'", *str);
    }
    SpaceKill();

    return node;
}

Element* GetAs(Tree* tree) {

    char op[OPERSIZE] = {};
    int offset = 0;

    sscanf(str, "%s%n", op, &offset);
    assert(!strcmp("Приготовить", op));
    str += offset;
    SpaceKill();

    Element* node = GetId(tree);

    sscanf(str, "%s%n", op, &offset);
    assert(!strcmp("как", op));
    str += offset;
    SpaceKill();
//printf("'%c'", *str);
    node = ASS(node, GetE(tree));

    SpaceKill(); //printf("'%c'", *str);
    assert(*str == '.');
    str++;

    return node;
}

Element* GetCo(Tree* tree) {

    char op[OPERSIZE] = {};
    int offset = 0;

    Element* node1 = GetE(tree);

    sscanf(str, "%[^0-9a-zA-Z()]%n", op, &offset);
    str += offset;
    SpaceKill();
//printf("'%s'", op);
    Element* node2 = GetE(tree);

    if( !strcmp(op, "вкуснее чем "))
        return JA(node1, node2);
    if( !strcmp(op, "чуть вкуснее чем "))
        return JAE(node1, node2);
    if( !strcmp(op, "так же вкусно, как "))
        return JE(node1, node2);
    if( !strcmp(op, "то же самое, что "))
        return JE(node1, node2);
    if( !strcmp(op, "менее вкусно чем "))
        return JBE(node1, node2);
    if( !strcmp(op, "гораздо хуже чем "))
        return JB(node1, node2);
    if( !strcmp(op, "отличается от "))
        return JNE(node1, node2);

    assert(0);
    return NULL;
}

Element* GetIf(Tree* tree) {

    char op[OPERSIZE] = {};
    int offset = 0;

    sscanf(str, "%s%n", op, &offset);
    assert(!strcmp("Если", op));
    str += offset;
    SpaceKill();

    Element* node = GetCo(tree);
    SpaceKill();

    sscanf(str, "%9[^\n]%n", op, &offset);
    //printf("'%s'", op);
    assert(!strcmp("то можете", op));
    str += offset;
    SpaceKill();

    node = IF(node, GetOP(tree));

    sscanf(str, "%12[^\n]%n", op, &offset);
    assert(!strcmp("всё, отлично", op));
    str += offset;
    SpaceKill();

    return node;
}

Element* GetGe(Tree* tree) {

    char op[OPERSIZE] = {};
    int offset = 0;

    sscanf(str, "%19[^0-9a-zA-Z]%n", op, &offset);
    //printf("'%s'", op);
    assert(!strcmp("Здравствуйте, можно", op));
    str += offset;
    SpaceKill();

    Element* node = GET(GetId(tree));

    SpaceKill();
    assert(*str == '.');
    str++;

    return node;
}

Element* GetPu(Tree* tree) {

    char op[OPERSIZE] = {};
    int offset = 0;

    sscanf(str, "%11[^0-9a-zA-Z]%n", op, &offset);
    //printf("'%s'", op);
    assert(!strcmp("Вот держите", op));
    str += offset;
    SpaceKill();

    Element* node = PUT(GetId(tree));

    sscanf(str, "%20[^0-9a-zA-Z]%n", op, &offset);     //printf("'%s'", op);
    assert(!strcmp(", приятного аппетита", op));
    str += offset;

    assert(*str == '.');
    str++;
    SpaceKill();

    return node;
}

Element* GetRe(Tree* tree) {

    char op[OPERSIZE] = {};
    int offset = 0;

    sscanf(str, "%5[^.\n]%n", op, &offset);
    assert(!strcmp("С вас", op));
    str += offset;
    SpaceKill();

    Element* node = GetE(tree);
    SpaceKill();

    if (*str == 'в') {

        sscanf(str, "%15[^\n]%n", op, &offset); //printf("'%s'", op);
        assert(!strcmp(op, "всего хорошего."));
        str += offset;
        SpaceKill();
        return END;
    }
    assert(*str == '.');
    str++;
    SpaceKill();
    return RET(node);
}

Element* GetIe(Tree* tree) {

    char op[OPERSIZE] = {};
    int offset = 0;

    sscanf(str, "%9s%n", op, &offset);
    assert(!strcmp("Ну а если", op));
    str += offset;
    SpaceKill();

    Element* node = GetCo(tree);
    SpaceKill();

    sscanf(str, "%11[^\n]%n", op, &offset);
    //printf("'%s'", op);
    assert(!strcmp("то наверное", op));
    str += offset;
    SpaceKill();

    ///////////////////////////////node = IE(node, GetOP(tree));

    sscanf(str, "%17[^\n]%n", op, &offset);
    assert(!strcmp("ну и всё, пожалуй", op));
    str += offset;
    SpaceKill();

    return node;
}

Element* GetEl(Tree* tree) {

    char op[OPERSIZE] = {};
    int offset = 0;
    Element* node = NULL;

    sscanf(str, "%13s%n", op, &offset);
    assert(!strcmp("Иначе давайте", op));
    str += offset;
    SpaceKill();

    /////////////////////////////node = EL(node, GetOP(tree));

    sscanf(str, "%22[^\n]%n", op, &offset);
    assert(!strcmp("ну да, так тоже хорошо", op));
    str += offset;
    SpaceKill();

    return node;
}

Element* GetOP(Tree* tree) {

    SpaceKill();

    if (*str == 'П') {
        Element* node = GetAs(tree);
        return OPER(node, GetOP(tree));
    }

    else if (*str == 'Е') {
        Element* node = GetIf(tree);
        return OPER(node, GetOP(tree));
    }

    else if (*str == 'З') {
        Element* node = GetGe(tree);
        return OPER(node, GetOP(tree));
    }

    else if (*str == 'В') {
        Element* node = GetPu(tree);
        return OPER(node, GetOP(tree));
    }

    else if (*str == 'С') {
        Element* node = GetRe(tree);
        return OPER(node, GetOP(tree));
    }

    else if (*str == 'Б') {
        Element* node = GetF(tree);
        return OPER(node, GetOP(tree));
    }

    else if (*str == 'Н') {
        Element* node = GetIe(tree);
        return OPER(node, GetOP(tree));
    }

    else if (*str == 'И') {
        Element* node = GetEl(tree);
        return OPER(node, GetOP(tree));
    }

    //GetWh();

    return NULL;
}

Element* GetDF(Tree* tree) {

    char op[OPERSIZE] = {};
    Element* node = NULL;
    int offset = 0;

    sscanf(str, "%8[^0-9a-zA-Z]%n", op, &offset);
    //printf("'%s'", op);
    assert(!strcmp(op, "Заказать"));
    str += offset;
    SpaceKill();

    sscanf(str, "%s%n", op, &offset);
    str += offset;
    SpaceKill();

    assert(FuncId(tree, op) == -1);

    char* name = (char*)calloc(strlen(op) + 2, sizeof(char));
    name[0] = '$';
    strcpy(name + 1, op);
    tree->FuncList[tree->FuncId++].name = name;    //printf("'%s'1'%d'2'%s'", op, FuncId(tree, op), tree->FuncList[tree->FuncId].name);

    if (*str == 'с')           ////////////////////////////////НИГДЕ НЕ ИСПОЛЬЗОВАТЬ 'с'
        node = GetPa(tree, tree->FuncId - 1);

    node = FUN(tree->FuncId - 1, node, GetOP(tree));

    assert(*str == '.');
    str++;
    return node;
}

Element* GetDV(Tree* tree) {

    char op[OPERSIZE] = {};
    int offset = 0;

    sscanf(str, "%s%n", op, &offset);  //printf("'%s'", op);
    str += offset;
    assert(VarId(tree, op) == -1);
    SpaceKill();

    tree->VarList[tree->VarId].name = strdup(op);
    Element* node = VAR(tree->VarId++);           //printf("%s: %d\n", tree->VarList[tree->VarId - 1].name, tree->VarId - 1);

    sscanf(str, "%[^.0-9a-zA-Z]%n", op, &offset);
    str += offset;
    SpaceKill();

    if (!strcmp("стоит ", op) || !strcmp("равно ", op)) {

        node = DEC(node, GetE(tree));
        assert(*str == '.');
        str++;
        return node;
    }

    if (!strcmp("бесплатно", op)) {

        assert(*str == '.');
        str++;
        return DEC(node, NUM(0));
    }

    assert(0);
    return NULL;
}

Element* GetSe(Tree* tree) {

    SpaceKill();

    if (*str == 'З') {
        Element* node = GetDF(tree);
        return SEM(node, GetSe(tree));
    }

    if (isalpha(*str) && *str != 'Г') {
        Element* node = GetDV(tree);
        return SEM(node, GetSe(tree));
    }

    return NULL;
}

Element* AddToSem(Element* head, Element* son) {

    CHECKNULLPTR(head);
    CHECKNULLPTR(son);
    assert(head->data == OPER_SEM);
    assert(son->data == OPER_SEM);

    Element* node = head;

    while(node->right)
        node = node->right;

    node->right = son;
    return son;
}

Element* GetMa(Tree* tree) {

    char op[OPERSIZE] = {};
    int offset = 0;
    Element* mnode = NULL;
    SpaceKill();

    SpaceKill();
    Element* node = GetSe(tree);
    SpaceKill();
    assert(*str == 'Г');      /////////// Главное блюдо - мейн

    sscanf(str, "%s%n", op, &offset);
    assert(!strcmp(op, "Главное"));
    str += offset;

    sscanf(str, " %s%n", op, &offset);
    assert(!strcmp(op, "блюдо"));
    str += offset;

    SpaceKill();

    if (*str == 'с')           ////////////////////////////////НИГДЕ НЕ ИСПОЛЬЗОВАТЬ 'с'
        mnode = GetPa(tree, tree->FuncId);

    mnode = FUN(0, mnode, GetOP(tree));

    assert(*str == '.');
    str++;
 printf("d");
    AddToSem(node, GetSe(tree));
    return SEM(mnode, node);
}

Element* GetG(Tree* tree, const char* data) {

    str = data;
    SpaceKill();
    Element* node = GetMa(tree);
    SpaceKill();
    assert(*str == '\0');
    //while(*str != '\0')
        //node = OPER(node, GetOP(tree));
    //printf("e");
    return node;
}

