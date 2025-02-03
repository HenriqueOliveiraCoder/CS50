-- Keep a log of any SQL queries you execute as you solve the mystery.|
-----------------------------------------------------------------------

-- First I searched for the schema of the whole database.
.sch

-- For getting the description of the report I searched for this.
SELECT description
FROM crime_scene_reports
WHERE month = 7
AND day = 28
AND street = 'Humphrey Street';

-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.

-- Searching for these three interviews
SELECT i.transcript
FROM interviews i
WHERE month = 7
AND day = 28;

-- Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
-- I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
-- As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- Selecting the car owners who left the bakery's parking lot within ten minutes after the theft
SELECT p.id
FROM bakery_security_logs b
JOIN people p ON p.license_plate = b.license_plate
WHERE month = 7
AND day = 28
AND hour = 10
AND minute > 14
AND minute < 26;

-- Selecting people from the earliest flight in the next morning after the theft, wich leaves fiftyville
SELECT pe.id
FROM flights f
JOIN passengers pa ON pa.flight_id = f.id
JOiN people pe ON pe.passport_number = pa.passport_number
WHERE f.id = (
    SELECT f.id
    FROM flights f
    JOIN airports a ON a.id = f.origin_airport_id
    WHERE f.month = 7
    AND f.day = 29
    AND a.city = 'Fiftyville'
    ORDER BY f.hour ASC, f.minute ASC
    LIMIT 1
);

-- Seecting people from the leggett street's atm
SELECT p.id
FROM atm_transactions a
JOIN bank_accounts b ON b.account_number = a.account_number
JOIN people p ON p.id = b.person_id
WHERE day = 28
AND month = 7
AND a.atm_location = 'Leggett Street';

-- Joining all three codes toguether
SELECT p.name
FROM bakery_security_logs b
JOIN people p ON p.license_plate = b.license_plate
WHERE month = 7
AND day = 28
AND hour = 10
AND minute > 14
AND minute < 26
AND p.id IN (
    SELECT pe.id
    FROM flights f
    JOIN passengers pa ON pa.flight_id = f.id
    JOiN people pe ON pe.passport_number = pa.passport_number
    WHERE f.id = (
        SELECT f.id
        FROM flights f
        JOIN airports a ON a.id = f.origin_airport_id
        WHERE f.month = 7
        AND f.day = 29
        AND a.city = 'Fiftyville'
        ORDER BY f.hour ASC, f.minute ASC
        LIMIT 1
        )
    )
AND p.id IN (
    SELECT p.id
    FROM atm_transactions a
    JOIN bank_accounts b ON b.account_number = a.account_number
    JOIN people p ON p.id = b.person_id
    WHERE day = 28
    AND month = 7
    AND a.atm_location = 'Leggett Street'
    );

-- Checking who spent less than a minute in a phone call, and finding the accomplice
SELECT p.*, p_c.name, p_r.name
FROM phone_calls p
JOIN people p_c ON p_c.phone_number = p.caller
JOIN people p_r ON p_r.phone_number = p.receiver
WHERE (p_c.name IN ('Luca', 'Bruce') OR p_r.name IN ('Luca', 'Bruce'))
AND duration < 60
AND day = 28;

-- Bruce, Robin

-- Getting the city the theft is going to
SELECT a_d.city
FROM flights f
JOIN airports a_o ON a_o.id = f.origin_airport_id
JOIN airports a_d ON a_d.id = f.destination_airport_id
WHERE f.month = 7
AND f.day = 29
AND a_o.city = 'Fiftyville'
ORDER BY f.hour ASC, f.minute ASC
LIMIT 1;

-- New York City
