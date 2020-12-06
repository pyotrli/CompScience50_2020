SELECT
title
FROM
movies
JOIN
(
	SELECT
	movie_id, rating
	FROM
	ratings
	WHERE movie_id IN(
		SELECT
		movie_id
		FROM
		stars
		WHERE
		person_id IN(
			SELECT
			id
			FROM
			people
			WHERE
			name = "Chadwick Boseman"
		)
	)
	ORDER BY rating DESC LIMIT 5
) as SUBQUERY
ON movies.id = SUBQUERY.movie_id
ORDER BY SUBQUERY.rating DESC