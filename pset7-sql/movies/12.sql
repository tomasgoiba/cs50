-- Movies in which both Johnny Depp and Helena Bonham Carter starred
SELECT title
FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE name IN ("Johnny Depp", "Helena Bonham Carter")
GROUP BY movies.id
HAVING COUNT(movies.id) = 2