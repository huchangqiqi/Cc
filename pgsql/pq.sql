create function and_em() returns integer as $$
select $1 + $2;
$$ language sql;
