-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Read crime scene reports from that day
SELECT description FROM crime_scene_reports
WHERE year = 2021 AND month = 7
AND day = 28
AND street = 'Humphrey Street';

-- Read interviews with witnesses
SELECT name, transcript FROM interviews
WHERE year = 2021 AND month = 7
AND day = 28 AND transcript LIKE '%bakery%';

-- Cars that left parking lot at that time (within 10 min of 10-15)
SELECT activity, hour, minute, license_plate FROM bakery_security_logs
WHERE year = 2021 AND month = 7
AND day = 28;

-- Has a car that left parking long at that time arrived earlier this morning - and WHEN
SELECT hour, minute, license_plate FROM bakery_security_logs
    WHERE activity = 'exit'
    AND year = 2021 AND month = 7
    AND day = 28
    AND hour = 10
    AND minute > 14 AND minute < 35
    AND license_plate IN
        (SELECT license_plate FROM bakery_security_logs
        WHERE activity = 'entrance'
        AND year = 2021 AND month = 7
        AND day = 28
        AND hour <= 10);

-- When these particular license plates arrived
-- (the time mustn't be earlier than when seen by atm, althought plan b possibly car was already there)
SELECT hour, minute, license_plate FROM bakery_security_logs
    WHERE activity = 'entrance'
    AND year = 2021 AND month = 7
    AND day = 28
    AND hour <= 10
    AND license_plate = '0NTHK55';
    -- 5P2BI95 entered 9-15
    -- 94KL13X entered 8-23
    -- 6P58WS2 -9-20
    -- 4328GD8 9-14
    -- G412CB7 9-28
    -- L93JTIZ 8-18
    -- 322W7JE 8-36
    -- 0NTHK55 - 8-42

-- Owners of the cars that exited
SELECT name, license_plate FROM people
WHERE license_plate IN
    (SELECT license_plate FROM bakery_security_logs
    WHERE activity = 'exit'
    AND year = 2021 AND month = 7
    AND day = 28
    AND hour = 10
    AND minute > 14 AND minute < 35 ORDER BY minute);
vanessa
bruce
barry
luca
sofia
iman
diana
kelsey


-- Thief withdrawing money from ATM on Legget Street
SELECT name FROM people WHERE id IN
(SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions
WHERE transaction_type = 'withdraw'
AND year = 2021 AND month = 7 AND day = 28
AND atm_location = 'Leggett Street'));
    result: bruce iman luca diana



-- Around 10-25 (as was leaving bakery), phone calls
-- 1. First, who called
SELECT name, phone_calls.caller, phone_calls.receiver, duration
    FROM phone_calls
        JOIN people ON phone_calls.caller = people.phone_number
        WHERE month = 7 AND day = 28 AND year = 2021
        AND duration < 60;
    -- result: suspects: bruce or diana (callers);
-- 2. receivers:
SELECT name, phone_calls.caller, phone_calls.receiver, duration
    FROM phone_calls
        JOIN people ON phone_calls.receiver = people.phone_number
        WHERE month = 7 AND day = 28 AND year = 2021
        AND duration < 60;
    -- result: bruce - robin, diana -philip

-- Earliest flight from Fiftyville on 29 July - passengers
SELECT name, origin_airport_id, hour, minute FROM airports, flights, people, passengers
    WHERE airports.id = flights.origin_airport_id
    AND flights.id = passengers.flight_id
    AND passengers.passport_number = people.passport_number
    AND month = 7 AND day = 29 AND year = 2021
    AND city = 'Fiftyville';

    -- result: bruce, destination airport id - 4
-- Now, get the destination airport

    SELECT name, origin_airport_id, destination_airport_id, airports.city, hour, minute FROM airports, flights, people, passengers
    WHERE airports.id = flights.destination_airport_id
    AND flights.id = passengers.flight_id
    AND passengers.passport_number = people.passport_number
    AND month = 7 AND day = 29 AND year = 2021;

    -- results: bruce, New York City, 8-20
-- The accomplice purchased flight ticket
