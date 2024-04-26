// Calculate your half of a restaurant bill
// Data types, operations, type casting, return value

#include <cs50.h>
#include <stdio.h>

float half(float bill, float tax, int tip);

int main(void)
{
    float bill_amount = get_float("Bill before tax and tip: ");
    float tax_percent = get_float("Sale Tax Percent: ");
    int tip_percent = get_int("Tip percent: ");

    printf("%.2f", half(bill_amount, tax_percent, tip_percent));

    // TODO: Complete the function


}

float half(float bill, float tax, int tip)
{
    double tax_amount = tax * bill / 100;
    double tip_amount = (double) tip * (bill + tax_amount) / 100;
    return (bill + tip_amount + tax_amount) /2;
}