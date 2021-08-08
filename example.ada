{*
 * Example with Functions
 *}

program example
-- global variables
declare
  a :constant : integer := 5;
  c :integer;


-- function declaration
procedure add (a: integer; b: integer) return integer
begin
  return a+b;
end;
end add;

-- main block
begin
  c := add(a, 10);
  if (c > 10) then
    print -c;
  else
    print c;
  end if;
  println "Hello World";
end;
end example