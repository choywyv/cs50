SELECT DISTINCT (people.name) FROM people
JOIN movies ON stars.movie_id = movies.id
JOIN stars ON stars.person_id = people.id
AND movies.id IN
  (SELECT movies.id FROM movies
    JOIN stars ON stars.movie_id = movies.id
    JOIN people ON stars.person_id = people.id
    WHERE people.name = 'Kevin Bacon' AND people.birth = 1958
  )
AND people.id NOT IN
(SELECT people.id FROM people WHERE people.name = 'Kevin Bacon' AND people.birth = 1958);

