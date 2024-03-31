program Test4;
var
   a, b, c: integer;      	 
   x, y, z: real;		{Declare novamente a variável "a" como real para que gere um erro "variável já declarada"}
   chave: boolean;

procedure LimparTela;	

begin			
   
end;

procedure teste (A,C:integer, B,E:real);
begin
   
end;  {verifique se é necessário um ";" no fechamento de um procedimento}

 procedure Somatorio ( entrada : integer);
   
  var
      
     resultado: integer; {Declare a variável "a" novamente. Neste caso não deve gerar erro poiso escopo é outro}
  
  begin

     resultado := 0;
      
     {LimparTela; adicione este procedimento}
     while (entrada>0) do {no lugar de "entrada" use "input" e veja se gera o erro "variável não declarada" }
                         resultado := resultado + entrada;
     entrada := entrada - 1		{troque "1" por "chave" e veja se gera o erro "tipos incompatíveis"}	
     
  end;

begin
   
   LimparTela;
   
   Somatorio(a);
   x:= y + z * (5.5 - c) / 2.567; {troque "x" por "a" e veja se gera o erro "tipos incompatíveis"}

 
         z := 5.0 {troque "5.0" por "test1" e veja se gera erro "Nome do programa não pode ser usado"}
  
   

end.

{retirar algumas palavras reservadas para gerar erros sintáticos}