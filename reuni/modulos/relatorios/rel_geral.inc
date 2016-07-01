<?php

header("Content-Type: application/vnd.ms-excel");
header("Content-disposition: attachment; filename=RelatorioGeral.xls");
ini_set( "memory_limit", "1024M" ); // ...

$sql="select
r.unicod as unidade,
u.unidsc as nome,
gp.grpdsc as item,
p.prgdsc as subitem,
r.rspdsc as texto,
pardsc as adhoc,
sitdsc as qualificacao
from
reuni.grupopergunta as gp,
reuni.pergunta as p,
reuni.resposta as r,
reuni.parecer as par,
reuni.situacao as s,
public.unidade as u
where
gp.grpcod = p.grpcod and
p.prgcod = r.prgcod and
r.rspcod = par.rspcod and
s.sitcod = par.sitcod and
r.unicod = u.unicod and
gp.grpcodfil in (6, 13)
order by r.unicod, item, subitem
--limit(10)
";

$cabecalho = array('Unidade', 'Nome', 'Item', 'SubItem', 'Texto', 'ADHOC', 'Qualificacao');

//$db->sql_to_excel($sql,"Rel_Geral",$cabecalho, array('s', 's', 's', 's', 's', 's', 's'));

$dados = $db->carregar($sql);
echo implode("\t", $cabecalho) , "\n";
while (list(, $linha) = each($dados)) {
	echo $linha['unidade'] , "\t" ,
		 $linha['nome']    , "\t" ,
		 $linha['item']    , "\t" ,
		 $linha['subitem'] , "\t" ,
		 strip_tags($linha['texto'])   , "\t" ,
		 $linha['adhoc']   , "\t" ,
		 $linha['qualificacao'] , "\n";
}


die;


