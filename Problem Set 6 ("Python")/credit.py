from cs50 import get_int

card = get_int("Number: ")

# 1st step of the check
# Remove the last digit

card_without_last = int(card / 10)

# Break card into individual digits, multiply every other and add up
sum_of_multiplied = 0

while card_without_last > 0:
    multiplied_digit = card_without_last % 10 * 2
    if multiplied_digit >= 10:
        second_digit = multiplied_digit % 10
        first_digit = int(multiplied_digit / 10)
        sum_of_multiplied += first_digit + second_digit
    else:
        sum_of_multiplied += multiplied_digit
    card_without_last = int(card_without_last / 100)

# 2nd step of the check
    sum_of_not_multiplied = 0
    card_for_2nd_step = card
    while card_for_2nd_step > 0:
        sum_of_not_multiplied += card_for_2nd_step % 10
        card_for_2nd_step = int(card_for_2nd_step / 100)
    overall_sum = sum_of_multiplied + sum_of_not_multiplied

# 3rd step of the check
if overall_sum % 10 == 0:
    if (card >= 340000000000000 and card < 350000000000000) or (card >= 370000000000000 and card < 380000000000000):
        print("AMEX")
    elif card >= 5100000000000000 and card < 5600000000000000:
        print("MASTERCARD")
    elif (card >= 4000000000000000 and card < 5000000000000000) or (card >= 4000000000000 and card < 5000000000000):
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")
