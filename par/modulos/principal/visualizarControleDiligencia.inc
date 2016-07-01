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
				dimcod || '.' || arecod || '.' || indcod || '.' || sbaordem || ' - ' || sbadsc as descricao,
				status,
				ano
			FROM (
			
				SELECT DISTINCT
					s.sbaid,
					d.dimcod,
					ar.arecod,
					i.indcod,
					s.sbaordem,
					s.sbadsc,
					esddsc as status,
					sbd.sbdano as ano
				FROM par.subacao s
				INNER JOIN par.subacaodetalhe sbd ON sbd.sbaid = s.sbaid --AND sbd.ssuid = 7
				INNER JOIN par.acao 	 a  ON a.aciid  = s.aciid AND a.acistatus IN ('A')
				INNER JOIN par.pontuacao p  ON p.ptoid  = a.ptoid AND p.ptostatus IN ('A')
				INNER JOIN par.criterio  c  ON c.crtid  = p.crtid AND c.crtstatus IN ('A') 
				INNER JOIN par.indicador i  ON i.indid  = c.indid AND i.indstatus = 'A'
				INNER JOIN par.area 	 ar ON ar.areid = i.areid AND ar.arestatus = 'A'
				INNER JOIN par.dimensao  d  ON d.dimid  = ar.dimid AND d.dimstatus = 'A'
				INNER JOIN workflow.documento doc on doc.docid = s.docid
				INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
				INNER JOIN workflow.historicodocumento hd ON hd.aedid = ".WF_SUBACAO_RETORNADO_DE_DILIGENCIA."  AND hd.docid = s.docid
				WHERE p.inuid = ".$inuid."
				and s.sbastatus = 'A'
				and ( ".WF_SUBACAO_RETORNADO_DE_DILIGENCIA." = 
						(select
									hd.aedid
								from workflow.historicodocumento hd
									inner join workflow.acaoestadodoc ac on
										ac.aedid = hd.aedid
									inner join workflow.estadodocumento ed on
										ed.esdid = ac.esdidorigem
								where
									hd.docid = s.docid
								order by
									hd.htddata desc limit 1))
				ORDER BY d.dimcod, ar.arecod, i.indcod, s.sbaordem 
				
				) as foo
				GROUP BY
					dimcod, arecod, indcod, sbaordem, sbadsc, sbaid, status, ano
			) as foo2

			ORDER BY
				descricao, ano";
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
			$cabecalho = array("Ação","Descrição da Subação","Status da Subação","Ano");
			$db->monta_lista($sql,$cabecalho,501,5,"N","100%","S");
		?>
	</body>
</html>