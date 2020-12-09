SELECT title FROM
  (SELECT movies.id, movies.title FROM movies
    JOIN stars ON stars.movie_id = movies.id
    JOIN people ON people.id = stars.person_id
    WHERE people.name = 'Johnny Depp' AND movies.id IN
      (SELECT movies.id FROM movies
        JOIN stars ON stars.movie_id = movies.id
        JOIN people ON people.id = stars.person_id
        WHERE people.name = 'Helena Bonham Carter'
      )
  );

