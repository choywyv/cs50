SELECT people.name FROM people WHERE people.id IN
(SELECT person_id FROM stars
JOIN movies ON movies.id = stars.movie_id
WHERE title = 'Toy Story');
