<?php
$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
include_once "config.inc";
include_once "_funcoes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = 'SELECT distinct pt.refid, pt.ptrid
		FROM emenda.ptminreformulacao pt
		INNER JOIN emenda.planotrabalho p on p.ptrid = pt.ptrid
		where pt.refid not in (select distinct refid from emenda.reformulatipos)
		and p.docid not in (select docid from workflow.documento where tpdid = 8 and esdid in (121,123 ) )';
/*
$sql = 'select * from emenda.ptminreformulacao
			where  refid not in (select distinct refid from emenda.reformulatipos )
			and refid = 321';
*/

$arrreformulacao = $db->carregar($sql);
$cont = 0;
if(is_array($arrreformulacao)){
	foreach ($arrreformulacao as $reformulacao){
		$tiposReformulacao = pegaTipoReformulacao( $reformulacao['ptrid'] );
		if($tiposReformulacao[0]){
			foreach ($tiposReformulacao as $tipo){
				$sql = 'SELECT count(rftid) FROM emenda.reformulatipos WHERE trefid = '.$tipo["codigo"].' AND refid =  '.$reformulacao["refid"];
				$exite = $db->pegaUm($sql);
				if($exite !== false){
					$sql = 'INSERT INTO emenda.reformulatipos (trefid, refid) VALUES ('.$tipo["codigo"].', '.$reformulacao["refid"].');';
					$cont++;
					$db->executar($sql);
				}
			}
		}
		
	}

	if($db->commit()){
		echo "Foram inseridos na tabela emenda.reformulatipos ".$cont." linhas.";
		
	}

}
die();


?>