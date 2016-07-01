<?
if($_REQUEST['exec_function']) {
	$_REQUEST['exec_function']($_REQUEST);
}
?>
<html>
	<head>
	<title>Inserir Cursos</title>		
	<script language="JavaScript" src="../../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	<script language="JavaScript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="./js/sig.js"></script>
	<script>
	function crtcursos(chk) {
		if(chk.checked) {
			ajaxatualizar('exec_function=inserircampuscurso&tpeid=<? echo $db->pegaUm("SELECT 
																						teu.tpeid 
																					   FROM 
																					   	sig.campus cam 
																					   INNER JOIN entidade.entidade e ON e.entid = cam.entid 		    
																					   INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
																					   INNER JOIN entidade.funentassoc fea ON fea.fueid = fe.fueid
																					   INNER JOIN entidade.entidade ea ON ea.entid = fea.entid 
																					   INNER JOIN entidade.funcaoentidade fe1 ON fe1.entid = ea.entid
																					   INNER JOIN sig.tipoensinouo teu ON teu.funid = fe1.funid 
																					   WHERE 
																					   	cam.cmpid = '". $_REQUEST['cmpid'] ."'");?>&curid='+chk.value,'');
			window.opener.ajaxatualizar('exec_function=carregarvagasporcurso&tpeid=<? echo $db->pegaUm("SELECT 
																					   						teu.tpeid 
																					   					FROM 
																					   						sig.campus cam 
																					   					INNER JOIN entidade.entidade e ON e.entid = cam.entid 
																										INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
																										INNER JOIN entidade.funentassoc fea ON fea.fueid = fe.fueid
																										INNER JOIN entidade.entidade ea ON ea.entid = fea.entid 
																									    INNER JOIN entidade.funcaoentidade fe1 ON fe1.entid = ea.entid
																									    INNER JOIN sig.tipoensinouo teu ON teu.funid = fe1.funid 
																					   					WHERE 
																					   						cam.cmpid = '". $_REQUEST['cmpid'] ."'");?>&cmpid=<? echo $_REQUEST['cmpid']; ?>','tabela_cursos');
		} else {
			var conf = confirm("Essa ação irá remover todos os dados vinculados ao curso neste campus. Deseja continuar?");
			if(conf) {
				ajaxatualizar('exec_function=removercampuscurso&curid='+chk.value,'');
				window.opener.ajaxatualizar('exec_function=carregarvagasporcurso&tpeid=<? echo $db->pegaUm("SELECT 
																					   							teu.tpeid 
																					   						FROM 
																					   							sig.campus cam 
																					   						INNER JOIN entidade.entidade e ON e.entid = cam.entid 
																											INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
																											INNER JOIN entidade.funentassoc fea ON fea.fueid = fe.fueid
																											INNER JOIN entidade.entidade ea ON ea.entid = fea.entid  
																										    INNER JOIN entidade.funcaoentidade fe1 ON fe1.entid = ea.entid
																										    INNER JOIN sig.tipoensinouo teu ON teu.funid = fe1.funid 																					   						
																										    WHERE 
																					   							cam.cmpid = '". $_REQUEST['cmpid'] ."'");?>&cmpid=<? echo $_REQUEST['cmpid']; ?>','tabela_cursos');
			}
		}
	}
	
	function selecionatipocurso(tpcid) {
		if(tpcid) {
			ajaxatualizar('exec_function=listarcursos&cmpid=<? echo $_REQUEST['cmpid']; ?>&tpcid='+tpcid,'listacurso');
			document.getElementById('linhalistacurso').style.display = '';
		} else {
			document.getElementById('linhalistacurso').style.display = 'none';
			document.getElementById('listacurso').innerHTML = '';
		}
	}
	</script>
	</head>
	<body>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
	<td class="SubTituloDireita">Tipo de curso :</td>
	<td>
	<?
	$sql = "SELECT tpc.tpcid as codigo, tpc.tpcdsc as descricao, teu.tpeid
		    FROM sig.campus cam 
		    INNER JOIN entidade.entidade e ON e.entid = cam.entid
		    INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
		    INNER JOIN entidade.funentassoc fea ON fea.fueid = fe.fueid
		    INNER JOIN entidade.entidade ea ON ea.entid = fea.entid
 		    INNER JOIN entidade.funcaoentidade fe1 ON fe1.entid = ea.entid
		    INNER JOIN sig.tipoensinouo teu ON teu.funid = fe1.funid 		    
		    INNER JOIN public.tipocurso tpc ON tpc.tpeid = teu.tpeid 
		    WHERE cam.cmpid = '". $_REQUEST['cmpid'] ."'";

	$db->monta_combo('unidade', $sql, 'S', 'Selecione', 'selecionatipocurso', '', '', '200', 'S', 'unidades');

	?>
	</td>
	</tr>
	<tr id="linhalistacurso" style="display:none">
	<td colspan="2" id="listacurso"></td>
	</tr>
	</table>
	<div id="test"></div>
	<script type="text/javascript" src="../includes/remedial.js"></script>
	<script language="JavaScript" src="../includes/wz_tooltip.js"></script>  
	</body>
</html>