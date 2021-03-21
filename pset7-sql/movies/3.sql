-- Titles of movies with a release date in or after 2018, in alphabetical order
SELECT title FROM movies
WHERE year >= 2018
ORDER BY title;
