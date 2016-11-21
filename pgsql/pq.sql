<<<<<<< HEAD
#存储过程
create function and_em() returns integer as $$
select $1 + $2;
$$ language sql;

#触发器
create function select_ts() returns integer as $$
begin
if(NEW.date >= 3) then
insert into ts_audit NEW.name;
endif;
end;
$$ language plpgsql;

create trigger ts_audit
after update on ts
for each row execute procedure select_ts();


=======
create function and_em() returns integer as $$
select $1 + $2;
$$ language sql;
>>>>>>> origin/master
