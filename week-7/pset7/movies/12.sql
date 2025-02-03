SELECT title
FROM movies
JOIN stars ON movies.id = stars.movie_id
WHERE stars.person_id IN (
    SELECT id
    FROM people
    WHERE name = 'Bradley Cooper'
)

INTERSECT

SELECT title
FROM movies
JOIN stars ON movies.id = stars.movie_id
WHERE stars.person_id IN (
    SELECT id
    FROM people
    WHERE name = 'Jennifer Lawrence'
);
