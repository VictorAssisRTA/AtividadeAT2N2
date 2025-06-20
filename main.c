#include <stdio.h>
#include <string.h>
#include "expressao.h"

int main() {
    Expressao exp;

    printf("Digite a expressão infixa (ex: ( 3 + 4 ) * 5 ):\n");
    fgets(exp.inFixa, 512, stdin);
    exp.inFixa[strcspn(exp.inFixa, "\n")] = 0;

    strcpy(exp.posFixa, getFormaPosFixa(exp.inFixa));
    exp.Valor = getValorInFixa(exp.inFixa);

    printf("\n--- Resultados ---\n");
    printf("Infixa: %s\n", exp.inFixa);
    printf("Pós-fixa: %s\n", exp.posFixa);
    printf("Valor: %.2f\n", exp.Valor);

    printf("\n--- Testando avaliação da pós-fixa ---\n");
    printf("Valor da pós-fixa: %.2f\n", getValorPosFixa(exp.posFixa));

    printf("\n--- Convertendo pós-fixa de volta para infixa ---\n");
    printf("Infixa reconstruída: %s\n", getFormaInFixa(exp.posFixa));

    return 0;
}
