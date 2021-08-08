program funcallfunc

declare
    a:integer;
    b:=20;

procedure func(a:integer;b:integer) return integer
begin
    return a+b;
end;
end func;

procedure func2(c:integer;d:integer) return integer
begin
    return func(c,d);
end;
end func2;

begin
    a:= func2(50,b);
    println a;
end;
end funcallfunc