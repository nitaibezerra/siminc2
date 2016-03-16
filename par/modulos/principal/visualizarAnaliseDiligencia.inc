<?php 
$inuid = $_SESSION['par']['inuid'];

if( !$inuid ){
	echo "<script> 
			alert('Falta o inuid');
			window.close();
		  </script>";
	exit;
}

$sql = "SELECT * FROM (

			SELECT
				'<center><img border=\"0\" onclick=\"visualizarSubacao( ' || sbaid || ', ' || ano || ' )\" src=\"../imagens/consultar.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" title=\"Visualizar Subação\" /><center>' as acao,
				codigo || ' - ' || sbadsc as descricao,
				CASE WHEN count(pendencia1) > 0 THEN 'Item Recusado<br><br>' ELSE '' END || CASE WHEN count(pendencia2) > 0 THEN 'Quantidade Modificada de itens<br><br>' ELSE '' END || CASE WHEN count(pendencia3) > 0 THEN 'Escola Recusada<br><br>' ELSE '' END || CASE WHEN count(pendencia4) > 0 THEN 'Quantidade Modificada na Escola<br><br>' ELSE '' END || CASE WHEN count(pendencia5) > 0 THEN 'Subação em Diligência<br><br>' ELSE '' END as pendencia,
				status,
				ano
			FROM (
			
				SELECT DISTINCT
					(SELECT par.retornacodigosubacao(s.sbaid)) as codigo,
					s.sbaid,
					s.sbadsc,
					CASE WHEN s.frmid = ".FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA." OR ( s.frmid = ".FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO." AND s.ptsid = ".FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO_COM_ITENS." ) OR s.frmid = ".ASSISTENCIA_FINANCEIRA_EXTRAORDINARIA." OR ( s.frmid = ".FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO." AND s.ptsid = ".FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO_COM_ITENS." ) THEN 
						CASE WHEN ( sic.icovalidatecnico = 'N') THEN 'Item Recusado' END 
					END as pendencia1,
					CASE WHEN s.sbacronograma = 2 THEN 
						CASE WHEN ( sei.seiqtd <> sei.seiqtdtecnico AND sei.seiqtdtecnico IS NOT NULL AND sic.icovalidatecnico = 'S' ) THEN 'Quantidade Modificada de itens' END
					ELSE
						CASE WHEN ( sic.icoquantidade <> sic.icoquantidadetecnico AND sic.icoquantidadetecnico IS NOT NULL AND sic.icovalidatecnico = 'S' ) THEN 'Quantidade Modificada de itens' END
					END as pendencia2,
					CASE WHEN s.sbacronograma = 2 AND ( s.frmid = ".FORMA_EXECUCAO_ASSITENCIA_TECNICA." OR s.frmid = ".FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO." OR s.frmid = ".FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO." ) THEN 
						CASE WHEN ( ses.sesvalidatecnico = 'N') THEN 'Escola Recusada' END 
					END as pendencia3,
					CASE WHEN s.sbacronograma = 2 AND ( s.frmid = ".FORMA_EXECUCAO_ASSITENCIA_TECNICA." OR s.frmid = ".FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO." OR s.frmid = ".FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO." ) THEN 
						CASE WHEN ( COALESCE(ses.sesquantidade,0) <> COALESCE(ses.sesquantidadetecnico,0) AND ses.sesvalidatecnico = 'S' ) THEN 'Quantidade Modificada na Escola' END
					END as pendencia4,
					CASE WHEN esd.esdid = ".WF_SUBACAO_DILIGENCIA." THEN
						'Subação em Diligência'
					END as pendencia5,
					esddsc as status,
					COALESCE(sbd.sbdano, 2012) as ano
				FROM
					par.subacao s
				LEFT  JOIN par.subacaodetalhe sbd ON sbd.sbaid = s.sbaid AND sbd.ssuid in ( 7,10)
				INNER JOIN par.acao 	 a  ON a.aciid  = s.aciid AND a.acistatus IN ('A')
				INNER JOIN par.pontuacao p  ON p.ptoid  = a.ptoid AND p.ptostatus IN ('A')

				INNER  JOIN workflow.documento doc ON doc.docid = s.docid
				INNER  JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid

				LEFT  JOIN par.subacaoitenscomposicao sic ON sic.sbaid = s.sbaid AND sic.icoano = sbd.sbdano
				LEFT  JOIN par.subacaoescolas ses ON ses.sbaid = s.sbaid AND sic.icoano = ses.sesano
				LEFT  JOIN par.subescolas_subitenscomposicao sei ON sei.sesid = ses.sesid AND sic.icoid = sei.icoid
				WHERE
					p.inuid = ".$inuid." AND
					esd.esdid = ".WF_SUBACAO_DILIGENCIA." 
				) as foo
				GROUP BY
					codigo, sbadsc, sbaid, status, ano
			) as foo2
			WHERE
				pendencia <> ''
			ORDER BY
				descricao, ano";
//ver(simec_htmlentities($sql), d);
/************************* CABEÇALHO E TÍTULO ******************************/
$db->cria_aba( $abacod_tela, $url, '' );
//cte_montaTitulo( $titulo_modulo, '&nbsp;' );
?>
<head>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<script type="text/javascript">
		function visualizarSubacao(sbaid, ano)
		{
			var local = "par.php?modulo=principal/subacao&acao=A&sbaid=" + sbaid + "&anoatual=" + ano;
			window.open(local, 'popupSubacao', "height=650,width=900,scrollbars=yes,top=50,left=200" );
		}
	</script>
</head>
	<body>
		<?php
			$cabecalho = array("Ação","Descrição da Subação","Pendência","Status da Subação","Ano");
			$db->monta_lista($sql,$cabecalho,501,5,"N","100%","S");
		?>
	</body>
</html>