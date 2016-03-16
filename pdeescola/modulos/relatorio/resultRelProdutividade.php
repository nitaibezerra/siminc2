<!-- <script language="JavaScript">
	document.getElementById('aguarde').style.display = "block";
</script>-->
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
<?php //ob_flush(); flush(); ?>

<?php
//qdo for ordenar resultado, buscar direto na sessão, desconsiderando filtros...
if ($_POST && !isset($_POST['ordemlista'])){
	if($_REQUEST['usunome']) {
		$where[] = "upper(u.usunome) like replace('%".str_to_upper($_REQUEST['usunome'])."%', ' ' , '%')";
	}

	if ($_REQUEST['f_esdid'][0] && $_REQUEST['esdid_campo_flag']){
		$where[] = " ac.esdiddestino ".(!$_REQUEST['f_esdid_campo_excludente'] ? ' IN ' : ' NOT IN ')."('".implode("','",$_REQUEST['f_esdid'])."') ";
	}
	

//Revertendo o formato da Data de dd/mm/aaaa, para aaaa/mm/dd 
if( $_REQUEST['htddataini'] && $_REQUEST['htddatafim'] ){
	$novadtinicio = substr($_REQUEST['htddataini'],6,4).'/'.substr($_REQUEST['htddataini'],3,2).'/'.substr($_REQUEST['htddataini'],0,2);
	$novadtfim = substr($_REQUEST['htddatafim'],6,4).'/'.substr($_REQUEST['htddatafim'],3,2).'/'.substr($_REQUEST['htddatafim'],0,2);
//Condição que é utilizada, quando as datas de inicio e fim forem selecionadas. 	
	$where[] = "hd.htddata BETWEEN '{$novadtinicio}'	AND	'{$novadtfim}'";
}	
	
	
	if ($_REQUEST['f_estuf'][0] && $_REQUEST['estuf_campo_flag']){
		$where[] = " m.estuf ".(!$_REQUEST['f_estuf_campo_excludente'] ? ' IN ' : ' NOT IN ')."('".implode("','",$_REQUEST['f_estuf'])."') ";
	}
	
	if ($_REQUEST['f_municipio'][0] && $_REQUEST['municipio_campo_flag']){
		$where[] = " m.muncod ".(!$_REQUEST['f_municipio_campo_excludente'] ? ' IN ' : ' NOT IN ')."('".implode("','",$_REQUEST['f_municipio'])."') ";
	}
	
	if ($_REQUEST['f_pflcod'][0] && $_REQUEST['pflcod_campo_flag']){
		$where[] = " p.pflcod ".(!$_REQUEST['f_pflcod_campo_excludente'] ? ' IN ' : ' NOT IN ')."('".implode("','",$_REQUEST['f_pflcod'])."') ";
	}
	
	$where = !$where ? array() : $where;
	
	$sql = "SELECT  DISTINCT
				CASE 
					WHEN ac.aeddscrealizada is null 
					THEN '--' 
					WHEN ac.esdiddestino = 76
					THEN 'Em Elaboração'
					WHEN ac.esdiddestino = 35
					THEN 'Aguard. correção (Cadastramento)'
					WHEN ac.esdiddestino = 86
					THEN 'Avaliação comitê mun. ou estadual'
					WHEN ac.esdiddestino = 36
					THEN 'Aguardando correção (Comitê)'
					WHEN ac.esdiddestino = 87
					THEN 'Avaliação MEC'
					WHEN ac.esdiddestino = 90
					THEN 'Finalizado'
					WHEN ac.esdiddestino = 37
					THEN 'Devolvido para Escola'
					WHEN ac.esdiddestino = 38
					THEN 'Devolvido para Comitê'
					ELSE ac.aeddscrealizada
				END as esddsc, 
				e.entcodent AS codigo,
				'' || e.entnome || '' AS nome,
				m.estuf AS estado,
				m.mundescricao AS municipio,
				to_char(hd.htddata, 'dd/mm/yyyy HH24:MI:SS')AS data,
				u.usunome as nomeusuario 
			FROM
			pdeescola.pdeescola pe 
			INNER JOIN entidade.entidade e ON e.entid = pe.entid 
			INNER JOIN entidade.endereco ende ON ende.entid = e.entid
			INNER JOIN territorios.municipio m on m.muncod = ende.muncod
			INNER JOIN workflow.documento d on d.docid = pe.docid 
			INNER JOIN workflow.historicodocumento hd on hd.docid = pe.docid --and hd.htddata = (select max(hd1.htddata) from workflow.historicodocumento hd1 where  hd1.docid = hd.docid)
			INNER JOIN workflow.acaoestadodoc ac on	ac.aedid = hd.aedid
			LEFT JOIN workflow.estadodocumento ed on ed.esdid = d.esdid and ed.esdid = ac.esdiddestino
			INNER JOIN seguranca.usuario u ON u.usucpf = hd.usucpf
			INNER JOIN seguranca.usuario_sistema AS us ON u.usucpf = us.usucpf 
			LEFT JOIN seguranca.perfilusuario pu on pu.usucpf = u.usucpf 
			LEFT JOIN seguranca.perfil p on p.pflcod = pu.pflcod 
			".((count($where) > 0) ? " WHERE ".implode(' AND ', $where) : '')."
			--GROUP BY 
				--ac.aeddscrealizada, ac.esdiddestino, codigo, nome, estado, municipio, htddata, u.usunome
			ORDER BY 
				codigo";

	//jogando na sessão caso não esteje via $_POST 
	$_SESSION['pdeescola_var']['sql_rel'] = $sql;	
}else{
	//se não estiver via $_POST pegará da $_SESSION
	$sql = 	$_SESSION['pdeescola_var']['sql_rel'];
}
	$cabecalho = array("Situação", "INEP", "Escola","Estado", "Municipio", "Data", "Nome");
	$db->monta_lista($sql,$cabecalho,500,20,'N','center',$par2);

?>
</body>
<script language="JavaScript">
	document.getElementById('aguarde').style.display = "none";
</script>