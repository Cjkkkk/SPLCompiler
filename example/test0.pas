program test;
var
  x : integer;
  y : integer;
  z : integer;
  s : string;
  m : char;


procedure findMin(x, y, z: integer; var m: integer);
begin
  x := 3;
  y := 4;
  z := 5;
  m := 6;
  s := "233";
  x := x * 4;
  if x < y then
    m := x
  else
    m := y;

  if z < m then
    m := z;
end
;

begin
  s := "hello world";
  m := 'c';
  x := 4 + 5 * 6;
  y := x;
  z := y;

  while x > 0 do begin
    x := 4;
  end;
  write(x);
end
.