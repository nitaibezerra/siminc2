<?php
/******** INCLUDES ********/ 
include "_funcoes.php";

/******** DECLARAÇÃO DE VARIAVEIS ********/ 
$ptas 			= array(80,81,82,83);
$ptaFilho 		= array();
$ptaPai 		= array();
$ptaPaiErro 	= array();
$contPai		= 0;
$cont 			= 0;

/******** GERA PTA FILHO ********/ 
foreach($ptas as $pta ){
	$sql = "SELECT * FROM emenda.planotrabalho  WHERE ptrexercicio = '2009' AND ptrcod = '".$pta."'";
	$ptridPai = $db->pegaUm($sql);
	$ptaFilho = insereFilhosPTA( $ptridPai, $boPai = true );
	if($ptaFilho){
		$ptaPai[$cont] 		= $pta;
		$ptaFilho[$cont] 	= $ptaFilho;
		$cont++;
		
		/******** ALTERA ESTADO DE DOCUMENTO ********/ 
		$SQL 	= "SELECT docid FROM emenda.planotrabalho WHERE ptrcod = ".$pta;
		$docid 	= $db->pegaUm($SQL);
		$SQL 	= "update workflow.documento set esdid = 155 where docid = ".$docid;
		$db->executar($SQL);
		
		// INSERE HISTÓRICO DE DOCUMENTO
		$SQL = "insert into workflow.historicodocumento (aedid, docid, usucpf, htddata) values (287, ".$docid.", '', now()) returning hstid";
		$hstid = $db->executar($SQL);
		
		// INSERE COMENTÁRIO
		$SQL = "insert into workflow.comentariodocumento (docid, cmddsc, hstid, cmdstatus, cmddata) values (".$docid.", 'Alteração autorizada pela equipe do FNDE.', ".$hstid.", 'A', now())";
		$db->executar($SQL);
		
		ativaPlanoTrabalhoFilho($ptrid);
		
		$db->commit();
		
	}else{
		$ptaPaiErro[$contPai] = $ptridPai;
		$contPai++;
		
		$db->rollback();
	}
}

// SE OCORREU ERRO COM ALGUM PTA
if(count($ptaPaiErro) > 0){
	
	$strPtaErros 	= implode(",", $ptaPaiErro);
	$strPtaFilho 	= implode(",", $ptaFilho);
	$strptaPai 		= implode(",", $ptaPai);
	echo 'Alguns PTAs filhos não foram gerados. <br> Abaixo segue a lista: <br> '.$strPtaErros;
	echo 'Este PTAs foram gerados: '.$strPtaFilho. 'e os pais são: '.$strptaPai;
}else{
//SE SUCESSO COM TODOS PTAS.
	echo 'Todos os PTAs foram gerados com sucesso.<br>';
	echo 'Segue a lista de PTAS:'.$strPtaFilho. 'e os pais são:'.$strptaPai;
}
?>