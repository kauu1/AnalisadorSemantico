program Test1;		{Este arquivo representa um programa correto}

var			{Existem diversos erros que podem ser gerados neste arquivo. Alguns exemplos:}
 
  a,b,c: integer;      	 
  x,y,z: real;		{Declare novamente a variável "a" como real para que gere um erro "variável já declarada"}
  chave: boolean;

  procedure LimparTela;

  var a: integer;

  procedure oioi(b:real);

  begin
   LimparTela;
   oioi;
  end;	
  begin			
   oioi;
  end;

  procedure Somatorio (entrada:integer);
  var
     
     resultado: integer; {Declare a variável "a" novamente. Neste caso não deve gerar erro poiso escopo é outro}
  
  begin

     resultado := 0;
     {LimparTela; adicione este procedimento}
      while (input>0) do {no lugar de "entrada" use "input" e veja se gera o erro "variável não declarada" }
      resultado := resultado + entrada;
      entrada := entrada - 1;		{troque "1" por "chave" e veja se gera o erro "tipos incompatíveis"}	
     
  end;


begin
   
   LimparTela;
   
   Somatorio(a);
   chave := y + z * (5.5 - chave + c) / 2.567 > 2; {troque "x" por "a" e veja se gera o erro "tipos incompatíveis"}
  
   if chave then 

      if (x<>2) or x then {substitua x por chave} 
 
         z := 5.0; {troque "5.0" por "test1" e veja se gera erro "Nome do programa não pode ser usado"}
  
      else
   
      chave := not chave;

end.