SELECT title FROM ratings, movies, stars, people
WHERE ratings.movie_id = movies.id
AND movies.id = stars.movie_id
AND stars.person_id = people.id
AND name = 'Chadwick Boseman'
ORDER BY rating DESC LIMIT 5;

