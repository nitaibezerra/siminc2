<?php

	/*
		Sistema Simec
		Setor responsável: SPO-MEC
		Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
		Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima (e-mail: renandelima@gmail.com), Renê de Lima (renedelima@gmail.com)
		Módulo: www/geral/saldoDespesa.php
		Finalidade: Listar sub itens para a o modulo principal/propostaorcamentaria/configuracao/momento.inc
	*/

	include "config.inc";
	header( 'Cache-Control: no-store, no-cache, must-revalidate' );
	header( 'Cache-Control: post-check=0, pre-check=0', false );
	header( 'Cache-control: private, no-cache' );
	header( 'Last-Modified: ' . gmdate( 'D, d M Y H:i:s' ) . ' GMT' );
	header( 'Pragma: no-cache' );
	header( 'Content-Type: text/html; charset=iso-8859-1' );

	include APPRAIZ."includes/classes_simec.inc";
	include APPRAIZ."includes/funcoes.inc";
	$db = new cls_banco();
	
	$ppoid  = addslashes( $_REQUEST['proposta'] );
	$unicod = addslashes( $_REQUEST['unidade'] );
	$prgcod = addslashes( $_REQUEST['programa'] );
	$acacod = addslashes( $_REQUEST['acao'] );
	$loccod = addslashes( $_REQUEST['localizador'] );
	$ndpcod = addslashes( $_REQUEST['natureza'] );
	$iducod = addslashes( $_REQUEST['iduso'] );
	$foncod = addslashes( $_REQUEST['fonte'] );
	$idocod = addslashes( $_REQUEST['idoc'] );
	
	// não captura as despesas da remessa que estã sendo editada
	// captura as despesas das outras remessas e despesas que não estão em nenhuma remessa
	$remid = (integer) $_REQUEST['remid'];
	$whereRemid = $remid ? ' and ( remid is null or remid !=  ' . $remid . ' ) ' : '' ;
	
	$sql = <<<EOF
select
	sum( coalesce( d.dpavalor, 0 ) ) as saldo
from
	elabrev.despesaacao d
	inner join elabrev.ppaacao_orcamento a on d.acaid = a.acaid
	inner join public.naturezadespesa n on d.ndpid = n.ndpid
	inner join public.idoc i on d.idoid = i.idoid
where
	d.ppoid  = '$ppoid'  and a.unicod = '$unicod' and a.prgcod = '$prgcod' and
	a.acacod = '$acacod' and a.loccod = '$loccod' and n.ndpcod = '$ndpcod' and
	d.iducod = '$iducod' and d.foncod = '$foncod' and i.idocod = '$idocod'
	$whereRemid
group by
	a.unicod, a.prgcod, a.acacod, a.loccod, n.ndpcod, d.iducod, d.foncod, i.idocod
EOF;
	print number_format( $db->pegaUm( $sql ), 0, ',', '.' );

?>