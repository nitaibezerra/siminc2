<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "pde/www/_funcoes_enem.php";
include_once APPRAIZ . "pde/www/_funcoes_enem.php";
include_once APPRAIZ . "pde/www/_constantes.php";
include_once APPRAIZ . 'includes/workflow.php';

//ver(APPRAIZ,d);
if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select a.atiid, a._atinumero as numero
			from pde.atividade a
			left join pde.estadoatividade e on e.esaid = a.esaid
			left join pde.observacaoatividade oa on a.atiid = oa.atiid and oa.obsstatus = 'A'
			where 
	        	a.atiid = 115819 and 
	            atistatus = 'A'";
$arAtividade = $db->carregar( $sql );
$arAtividade = $arAtividade ? $arAtividade : array();

foreach ($arAtividade as $v) {
	
	$sql = "SELECT DISTINCT
			    icl.iclid,
			    icl.icldsc,
			    icl.docid
			FROM 
			    pde.itemchecklist icl
			WHERE
			    icl.atiid = {$v['atiid']}";
	
	$listaChecklist = $db->carregar($sql);
	$listaChecklist = $listaChecklist ? $listaChecklist : array();
	
	$boFinalizado = false;
	if( !empty($listaChecklist[0]) ){
		$atiidFilho = $v['atiid'];
		
		$boFinalizado = true;
		foreach ($listaChecklist as $check) {
		
			$docid = (integer) $check['docid'];
			$atual = wf_pegarEstadoAtual( $docid );
			
			if( $atual['esdid'] != ENEM_EST_EM_FINALIZADO ){
				$boFinalizado = false;
				break;
			}
		}
		if( !$boFinalizado ){
			$atiidPai = alteraSituacaoAtividade( $atiidFilho, 5, 100 );
		}
	}	
}
function pegaPaiAtividade( $atiid ){
	global $db;
	$atiidPai = $db->pegaUm("select atiidpai from pde.atividade where atistatus = 'A' and atiid = ".$atiid);
	$boPai = $db->pegaUm("select atiidpai from pde.atividade where atistatus = 'A' and atiid = ".$atiidPai);
	if($boPai){
		verificaSituacaoAtividade( $atiidPai );
	} else {
		$db->commit();
		return true;
	}
}
function verificaSituacaoAtividade( $atiid ){
	global $db;
	
	$totalFilhos = $db->pegaUm("select count(atiid) from pde.atividade a 
											where atiidpai = $atiid and a.atistatus = 'A'");	
	$totalFilhosPerc = $db->pegaUm("select count(atiid) from pde.atividade a 
											where atiidpai = $atiid and (atiporcentoexec <> 100 or atiporcentoexec is null) and a.atistatus = 'A'");	
	$totalPerc = $db->pegaUm( "select atiporcentoexec from pde.atividade where atiid = $atiid and atistatus = 'A'" );
	
	$atiporcentoexec = (float) 100 / (float) $totalFilhos;
	$atiporcentoexec = round($atiporcentoexec) + $totalPerc;
	$atiporcentoexec = round( $atiporcentoexec );
	
	#arredondando os valor inteiro
	$atiporcentoexec = $atiporcentoexec / 10;
	$atiporcentoexec = round( $atiporcentoexec ) * 10;
	
	$esaid = 2;
	if( $totalFilhosPerc == 0 ){
		$esaid = 5;
		$atiporcentoexec = 100;
	}
	
	/*$atiidNome = $db->pegaUm("select atidescricao from pde.atividade where atiid = ".$atiid);
	ver('Atividade Nome: '.$atiidNome.'<br>Atividade: '.$atiid.'<br>Porcentagem: '.$atiporcentoexec.
		'<br>Total Filhos: '.$totalFilhos.'<br>Filhos <> 100%: '.$totalFilhosPerc.'<br>Total Perc: '.$totalPerc );*/	
		
	alteraSituacaoAtividade( $atiid, $esaid, $atiporcentoexec );
}
function pegaFilhosAtividade(){
	global $db;
	
	$sql = "";
}
function alteraSituacaoAtividade($atiid, $esaid = '', $atiporcentoexec = ''){
	global $db;
	
	$sql = "UPDATE pde.atividade SET 
		  		esaid = $esaid,
		  		atiporcentoexec = '$atiporcentoexec'		 
			WHERE 
		  		atiid = $atiid";
	
	$db->executar( $sql );
	pegaPaiAtividade( $atiid );
}
function arredondaValor(){}
?>