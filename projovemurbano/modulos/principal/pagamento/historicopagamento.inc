<?php
extract($_GET);
include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';
$sql = "SELECT
			perperiodo
		FROM
			projovemurbano.periodocurso
		WHERE
			perid = {$perid}";
$periodo = $db->pegaUm($sql);
monta_titulo('Histórico de Pagamentos','Período:'||$periodo);
$_SESSION['projovemurbano']['modalidade'] = '';

$bloq = 'S'

?>
<?

$sql = "
		SELECT DISTINCT
			data_,
			usunome,
			SUM(qtd1) as enviadas,
			SUM(qtd2) as rejeitados
		FROM
		(
			SELECT 
				to_char(hst.htddata,'yyyy-MM-DD') as data_,
				usunome,
				CASE WHEN esdidorigem = 527 AND esdiddestino = 528 AND dia.perid = {$perid}
					THEN 1
					ELSE 0
				END as qtd1,
				CASE WHEN esdidorigem = 560 AND esdiddestino = 528 AND dia.perid = {$perid}
					THEN 1
					ELSE 0
				END as qtd2
			FROM projovemurbano.pagamentoestudante pge
			INNER JOIN workflow.historicodocumento hst		 ON hst.hstid = doc.hstid	
			INNER JOIN projovemurbano.cadastroestudante cae  ON pge.caeid = cae.caeid
			INNER JOIN projovemurbano.diario dia             ON pge.diaid = dia.diaid
			INNER JOIN workflow.acaoestadodoc aed 		 	 ON aed.aedid = hst.aedid
			INNER JOIN seguranca.usuario usu 		 		 ON usu.usucpf = hst.usucpf
			WHERE
				dia.perid = {$perid}
				AND	aed.esdidorigem IN (527,560)
				AND     aed.esdiddestino = 528
			GROUP BY
				htddata,
				usunome,
				esdidorigem,
				esdiddestino,
				dia.perid
			Order by 
				data_
		) as foo
		GROUP BY
			data_,
			usunome
		ORDER BY
			data_
";

$cabecalho = array("Data de envio","Responsável","Bolsas Enviadas","Bolsas Rejeitadas");
$alinhamento	= array( 'center', 'center','center', 'center');                   
$db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', '', '','',$alinhamento);

?>