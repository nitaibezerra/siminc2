<?php

$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);

include_once "/var/www/simec/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "www/pdeinterativo/_constantes.php";
include_once APPRAIZ . "includes/workflow.php";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

/*
$db = new cls_banco();
$sql = "  update projovemurbano.cadastroestudante x set caeqtdbolsaprojovem=foo.num from (
select p.caeid, count(*) as num from projovemurbano.pagamentoestudante p inner join workflow.documento d on d.docid = p.docid where d.esdid=529 group by p.caeid
) foo where foo.caeid = x.caeid";
$db->executar($sql);
$db->commit();
*/

$servidor_bd = '';
$porta_bd = '5432';
$nome_bd = '';
$usuario_db = '';
$senha_bd = '';

$db = new cls_banco();

/////////////////////// ESCOLAS PRA VOLTAR
/*
$comentario = "Verificamos que existe um saldo de recursos não programado na segunda parcela (ver a coluna \"Restante PDE\" da aba \"PDE Escola\") e recomendamos que seja feita uma revisão do plano, acrescentando (caso o saldo seja POSITIVO) ou excluindo (caso o saldo seja NEGATIVO) itens e serviços ou alterando quantidades e/ou valores unitários, de forma que este saldo fique zerado. A existência deste saldo decorre do fato de que houve mudanças no número de matrículas entre os Censos Escolares de 2010 e 2011, alterando o montante a ser repassado pelo MEC. Para que o plano possa ser validado, não pode haver nenhum saldo de recursos (nem positivo nem negativo) em nenhuma das parcelas";

$sql = "select p.pdicodinep, p.pdenome, p.docid ,r.* from pdeinterativo.pdinterativo p 
		inner join workflow.documento d on d.docid = p.docid 
		inner join pdeinterativo.relatorio_saldo r on r.pdeid = p.pdeid
		where pdistatus='A' and d.esdid=310 and pditempdeescola=true and (rlsprimeiraparcela != rlstotalprimeiraparcela or rlssegundaparcela != rlstotalsegundaparcela)";

$lista = $db->carregar($sql);

echo "Voltando escolas...<br>";

if($lista[0]) {
	foreach($lista as $l) {
		$docid = $l['docid'];
		$aedid = 1260;
		$dados = array();
		$result = wf_alterarEstado( $docid, $aedid, $comentario, $dados);
		echo $result.";";
	
	}
	echo "<br><br>";
}
echo "Todas as ".(($lista[0])?count($lista):"0")." escolas voltadas com sucesso...<br>";
*/
/////////////////////// ESCOLAS PRA VOLTAR

$sql = "select p.pdicodinep, p.pdenome, r.*, i.codinep from pdeinterativo.pdinterativo p 
		inner join workflow.documento d on d.docid = p.docid 
		inner join pdeinterativo.relatorio_saldo r on r.pdeid = p.pdeid 
		left  join pdeinterativo.inepenviado i on i.codinep = p.pdicodinep
		where pdistatus='A' and d.esdid=310 and pditempdeescola=true and rlsprimeiraparcela = rlstotalprimeiraparcela and rlssegundaparcela = rlstotalsegundaparcela and i.codinep is null 
		limit 30";

$pde = $db->carregar($sql);

echo "Enviando escolas para pagamentos...<br>";

if($pde[0]) {
	echo "<table>";
	foreach($pde as $p) {
	
		include_once APPRAIZ."/www/pdeinterativo/pdeWs.php";
		$ws = new pdeWs();
			
		$coProgramaFNDE = 96;
		$anoAtual = date('Y');
		$entcodent = $p['pdicodinep'];
		
		echo "<tr>";
		echo "<td>{$entcodent}</td>";
		echo "<td>";
		echo "<pre>";
		$teste = $ws->pdeEscolaWs('atualizaAnaliseEscola', $anoAtual, $entcodent, $coProgramaFNDE);
		print_r($teste);
		echo "</pre>";
		echo "</td>";
		echo "</tr>";
		
		$sql = "INSERT INTO pdeinterativo.inepenviado(codinep)
    			VALUES ('".$entcodent."')";
		$db->executar($sql);
		$db->commit();
		
		
		if( $teste && $teste != "errowebservice" ) {
			$db->executar("UPDATE pdeinterativo.pdinterativo SET pdiretornofnde='t' WHERE pdistatus = 'A' and pdicodinep = '{$entcodent}'");
			$db->commit();
		} else {
			$db->executar("UPDATE pdeinterativo.pdinterativo SET pdiretornofnde='f' WHERE pdistatus = 'A' and pdicodinep = '{$entcodent}'");
			$db->commit();
		}
		
	}
	echo "</table>";
}


?>
