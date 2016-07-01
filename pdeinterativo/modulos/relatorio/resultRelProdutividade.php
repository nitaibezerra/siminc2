<?php 
	ini_set("memory_limit","1024M");
	set_time_limit(0);
?>
<html>
<head>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
<center>
	<div id="aguarde" style="background-color:#ffffff;position:absolute;color:#000033;top:50%;left:30%;border:2px solid #cccccc; width:300;font-size:12px;z-index:0;">
		<br><img src="../imagens/wait.gif" border="0" align="middle"> Aguarde! Carregando Dados...<br><br>
	</div>
</center>
<?php
	//qdo for ordenar resultado, buscar direto na sessão, desconsiderando filtros...
	if ($_POST && !isset($_POST['ordemlista'])){
		if($_REQUEST['usunome'])
			$where[] = "upper(usu.usunome) like replace('%".str_to_upper($_REQUEST['usunome'])."%', ' ' , '%')";
		
		if ($_REQUEST['f_aedid'][0] && $_REQUEST['aedid_campo_flag'])
			$where[] = " aed.aedid ".(!$_REQUEST['f_aedid_campo_excludente'] ? ' IN ' : ' NOT IN ')."('".implode("','",$_REQUEST['f_aedid'])."') ";
		
		//Revertendo o formato da Data de dd/mm/aaaa, para aaaa/mm/dd 
		if( $_REQUEST['htddataini'] && $_REQUEST['htddatafim'] ){
			$novadtinicio = substr($_REQUEST['htddataini'],6,4).'/'.substr($_REQUEST['htddataini'],3,2).'/'.substr($_REQUEST['htddataini'],0,2);
			$novadtfim = substr($_REQUEST['htddatafim'],6,4).'/'.substr($_REQUEST['htddatafim'],3,2).'/'.substr($_REQUEST['htddatafim'],0,2);
			//Condição que é utilizada, quando as datas de inicio e fim forem selecionadas. 	
			$where[] = "hdo.htddata BETWEEN '{$novadtinicio}'	AND	'{$novadtfim}'";
		}
		
		if ($_REQUEST['f_estuf'][0] && $_REQUEST['estuf_campo_flag'])
			$where[] = " mun.estuf ".(!$_REQUEST['f_estuf_campo_excludente'] ? ' IN ' : ' NOT IN ')."('".implode("','",$_REQUEST['f_estuf'])."') ";
		
		if ($_REQUEST['f_municipio'][0] && $_REQUEST['municipio_campo_flag'])
			$where[] = " mun.muncod ".(!$_REQUEST['f_municipio_campo_excludente'] ? ' IN ' : ' NOT IN ')."('".implode("','",$_REQUEST['f_municipio'])."') ";
		
		if ($_REQUEST['f_pflcod'][0] && $_REQUEST['pflcod_campo_flag'])
			$where[] = " per.pflcod ".(!$_REQUEST['f_pflcod_campo_excludente'] ? ' IN ' : ' NOT IN ')."('".implode("','",$_REQUEST['f_pflcod'])."') ";
		
		$where = !$where ? array() : $where;
		
		$sql = "
			SELECT DISTINCT
					CASE 
						WHEN aed.aeddscrealizada is null 
						THEN '--' 
						WHEN aed.esdiddestino = 76
						THEN 'Em Elaboração'
						WHEN aed.esdiddestino = 35
						THEN 'Aguard. correção (Cadastramento)'
						WHEN aed.esdiddestino = 86
						THEN 'Avaliação comitê mun. ou estadual'
						WHEN aed.esdiddestino = 36
						THEN 'Aguardando correção (Comitê)'
						WHEN aed.esdiddestino = 87
						THEN 'Avaliação MEC'
						WHEN aed.esdiddestino = 90
						THEN 'Finalizado'
						WHEN aed.esdiddestino = 37
						THEN 'Devolvido para Escola'
						WHEN aed.esdiddestino = 38
						THEN 'Devolvido para Comitê'
						ELSE aed.aeddscrealizada
					END as esddsc, 
					ent.entcodent AS codigo,
					'' || ent.entnome || '' AS nome,
					mun.estuf AS estado,
					mun.mundescricao AS municipio,
					to_char(hdo.htddata, 'dd/mm/yyyy HH24:MI:SS')AS data,
					usu.usunome as nomeusuario 
			FROM pdeinterativo.pdinterativo pde 
				INNER JOIN entidade.entidade           ent ON ent.entid  = pde.entid 
				INNER JOIN entidade.endereco           ene ON ene.entid  = ent.entid
				INNER JOIN territorios.municipio       mun ON mun.muncod = ene.muncod
				INNER JOIN workflow.documento          doc ON doc.docid  = pde.docid 
				INNER JOIN workflow.historicodocumento hdo ON hdo.docid  = pde.docid
				INNER JOIN workflow.acaoestadodoc      aed ON aed.aedid  = hdo.aedid
				LEFT  JOIN workflow.estadodocumento    edo on edo.esdid  = doc.esdid
				AND                                           edo.esdid  = aed.esdiddestino
				INNER JOIN seguranca.usuario           usu ON usu.usucpf = hdo.usucpf
				INNER JOIN seguranca.usuario_sistema   usi ON usu.usucpf = usi.usucpf 
				LEFT  JOIN seguranca.perfilusuario     pus ON pus.usucpf = usu.usucpf 
				LEFT  JOIN seguranca.perfil            per on per.pflcod = pus.pflcod 
			".((count($where) > 0) ? " WHERE ".implode(' AND ', $where) : '')."
			ORDER BY codigo
		";
	
		//jogando na sessão caso não esteje via $_POST 
		$_SESSION['pdeescola_var']['sql_rel'] = $sql;	
	}
	else{
		//se não estiver via $_POST pegará da $_SESSION
		$sql = 	$_SESSION['pdeescola_var']['sql_rel'];
	}
	$cabecalho = array("Situação", "INEP", "Escola","Estado", "Municipio", "Data", "Nome");
	if($_REQUEST['relatorio'] == 'html')
		$db->monta_lista($sql,$cabecalho,500,20,'N','center',$par2);
	elseif($_REQUEST['relatorio'] == 'xls'){
		ob_clean();
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_Relatorio_de_Produtividade".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_Relatorio_de_Produtividade_".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->sql_to_excel($sql, 'Relatorio_de_Produtividade', $cabecalho);
	}
	
?>
</body>
<script language="JavaScript">
	document.getElementById('aguarde').style.display = "none";
</script>