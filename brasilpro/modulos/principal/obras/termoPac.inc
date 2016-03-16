<?php
include '_funcoes_termo.php';

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '', array(8667) );
monta_titulo( $titulo_modulo, '&nbsp;' );
?>
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="./js/brailproobras.js"></script>
<script>
function excluirAnexoTermo(terid) {
	var conf = confirm('Deseja excluir o anexo?');
	if(conf) {
		window.location='cte.php?modulo=principal/obras/termoPac&acao=A&urlgo=<? echo md5_encrypt('cte.php?modulo=principal/obras/termoPac&acao=A'); ?>&requisicao=excluiranexotermo&terid='+terid;
	}
}

function enviarAnexo(terid) {
	displayMessage('cte.php?modulo=principal/obras/termoPac&acao=A&urlgo=<? echo md5_encrypt('cte.php?modulo=principal/obras/termoPac&acao=A'); ?>&requisicao=telaanexo&req=inseriranexotermo&terid='+terid);
}

function enviarAnexo2(proid) {
	displayMessage('cte.php?modulo=principal/obras/termoPac&acao=A&urlgo=<? echo md5_encrypt('cte.php?modulo=principal/obras/termoPac&acao=A'); ?>&requisicao=telaanexo&req=inseriranexotermoprocesso&proid='+proid);
}

function assinarTermo(obj) {
	var assinado="";
	if(obj.checked == true) {
		assinado="&terassinado=TRUE";
	} else {
		assinado="&terassinado=FALSE";
	}

	$.ajax({
		type: "POST",
		url: "cte.php?modulo=principal/obras/termoPac&acao=A",
		data: "requisicao=assinarTermo&terid="+obj.value+assinado,
		async: false,
		success: function(msg){}
	});
}

</script>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form method="post" name="formulario" id="formulario">
					<table border="0" cellpadding="3" cellspacing="0">
						<tr>
							<td valign="bottom">
								Município
								<br/>
								<?php 
									$filtro = simec_htmlentities( $_POST['municipio'] );
									$municipio = $filtro;
									echo campo_texto( 'municipio', 'N', 'S', '', 50, 200, '', ''); 
								?>
							</td>
							<td valign="bottom">
								Estado
								<br/>
								<?php
									$estuf = $_POST['estuf'];
									$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
									$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
								?>
							</td>
						</tr>
						<tr>
							<td valign="bottom" colspan="2">
								Resolução
								<br/>
								<?php 
									$resid = $_POST['resid'];
									$sql = "select resid as codigo, resdescricao as descricao from cte.resolucao where resstatus='A'";
									$db->monta_combo( "resid", $sql, 'S', 'Todas as Resolução', '', '' ); 
								?>
							</td>
						</tr>
						<tr>
							<td valign="bottom">
								Tipo de Obra:
							</td>
						</tr>
						<tr>
							<td valign="bottom">
								<input <?php echo $_REQUEST['tipoobra'] == "P" ? "checked='checked'" : "" ?> type="radio" name="tipoobra" value="P" >Proinfancia<input <?php echo $_REQUEST['tipoobra'] == "Q" ? "checked='checked'" : "" ?> type="radio" name="tipoobra" value="Q" >Quadra
							</td>
						</tr>
						<tr>
							<td valign="bottom">
								Termo:
							</td>
						</tr>
						<tr>
							<td valign="bottom">
								<input <?php echo $_REQUEST['termogerado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="termogerado" value="1" >Termo Gerado<input <?php echo $_REQUEST['termogerado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="termogerado" value="2" >Termo Não Gerado
							</td>
						</tr>
						<tr>
							<td valign="bottom">
								Assinatura:
							</td>
						</tr>
						<tr>
							<td valign="bottom">
								<input <?php echo $_REQUEST['assinado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="assinado" value="1" >Assinado<input <?php echo $_REQUEST['assinado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="assinado" value="2" >Não Assinado
							</td>
						</tr>
					</table>
					<div style="float: right;">
						<input type="submit" name="pesquisar" value="Pesquisar" />
						<input type="button" name="todos" value="Ver todos" onclick="window.location='cte.php?modulo=principal/obras/termoPac&acao=A';" />
						<input type="button" name="todos" value="Exportar Excel" onclick="window.location='cte.php?modulo=principal/obras/termoPac&acao=A&exportarexcel=true';" />
					</div>
				</form>
			</td>
		</tr>
	</tbody>
</table>
<?php

if($_REQUEST['pesquisar']) {
	$inner = '';
	if($_REQUEST['municipio']) {
		$where[] = "removeacento(m.mundescricao) ILIKE removeacento('%".$_REQUEST['municipio']."%')";
	}
	
	if($_REQUEST['estuf']) {
		$where[] = "m.estuf='".$_REQUEST['estuf']."'";
	}
	if($_REQUEST['termogerado'] == "1") {
		//$inner = "INNER JOIN cte.termocompromissopac t ON t.muncod = m.mucndo";
		$where[] = "( SELECT 
							t.terstatus
						FROM 
							cte.termocompromissopac t
						where 
							t.muncod = m.muncod AND t.terid = (SELECT max(terid) FROM cte.termocompromissopac t2 WHERE t2.muncod = t.muncod)
						limit 1 ) = 'A'";
	}
	if($_REQUEST['termogerado'] == "2") {
		$where[] = "(
						( SELECT 
							t.terstatus
						FROM 
							cte.termocompromissopac t
						where 
							t.muncod = m.muncod AND t.terid = (SELECT max(terid) FROM cte.termocompromissopac t2 WHERE t2.muncod = t.muncod)
						limit 1 ) = 'I'

						OR
						
						( SELECT 
							t.terstatus
						FROM 
							cte.termocompromissopac t
						where 
							t.muncod = m.muncod AND t.terid = (SELECT max(terid) FROM cte.termocompromissopac t2 WHERE t2.muncod = t.muncod)
						limit 1 ) IS NULL
						
						)";
//		$where[] = "(m.muncod NOT IN ( SELECT DISTINCT t.muncod FROM cte.termocompromissopac t ) OR m.muncod IN ( SELECT DISTINCT t.muncod FROM cte.termocompromissopac t WHERE t.terstatus = 'I' ))";
	}
	if($_REQUEST['tipoobra'] == "P") {
		$where[] = "protipo = 'P'";
	}
	if($_REQUEST['tipoobra'] == "Q") {
		$where[] = "protipo <> 'P'";
	}
	if($_REQUEST['assinado'] == "1") {
		$where[] = "ter.terassinado = TRUE";
	}
	if($_REQUEST['assinado'] == "2") {
				$where[] = "( ter.terassinado = FALSE OR ter.terassinado IS NULL )";
	}
	if($_REQUEST['resid']) {
		$where[] = "r.resid = '".$_REQUEST['resid']."'";
	}
}

if($_REQUEST['exportarexcel']){
	$cabecalho = array("Nº Processo","Resolução","Município","UF", "Tipo obra","Valor total(R$)","Valor empenhado(R$)","Assinado","Qtd obras no processo");
} else {
	$cabecalho = array("&nbsp;","&nbsp;","Nº Processo","Resolução","Município","UF", "Tipo obra","Valor total(R$)","Valor empenhado(R$)","Assinado","Anexo de publicação","Qtd obras no processo");
}

$sql = "SELECT ";
			if(!$_REQUEST['exportarexcel']){
				$sql .= "'<center><img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarListaTermos2(\''||m.muncod||'\', \''|| p.proid ||'\',this);\"></center>' as mais,
						'<center><img src=../imagens/editar_nome_vermelho.gif style=cursor:pointer; onclick=\"window.open(\'cte.php?modulo=principal/obras/gerarTermoObra&acao=A&proid='|| p.proid ||'&muncod=' || m.muncod ||'\',\'Termo\',\'scrollbars=yes,height=500,width=800,status=no,toolbar=no,menubar=no,location=no\');\">
						</center>' as acoes, ";
			} 
		   	$sql .= "p.pronumeroprocesso, 
		   	COALESCE(r.resdescricao,'N/A') as resdescricao,
		   	m.mundescricao, 
		   	m.estuf, 
		   	CASE WHEN protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
				CASE WHEN protipo='P' 
					THEN 
						(
						SELECT sum(round(po.prevalorobra,2))  
						FROM obras.preobra po 
						INNER JOIN cte.empenhoobra eo ON eo.preid = po.preid 
						INNER JOIN cte.empenho emp ON emp.empid = eo.empid  
						INNER JOIN workflow.documento d ON d.docid=po.docid 
						WHERE emp.empnumeroprocesso=p.pronumeroprocesso
						AND po.prestatus='A' 
						AND d.esdid=228
						AND ptoid <> 5
						)
					ELSE
						(
						SELECT sum(round(po.prevalorobra,2))  
						FROM obras.preobra po 
						INNER JOIN cte.empenhoobra eo ON eo.preid = po.preid 
						INNER JOIN cte.empenho emp ON emp.empid = eo.empid  
						INNER JOIN workflow.documento d ON d.docid=po.docid 
						WHERE emp.empnumeroprocesso=p.pronumeroprocesso
						AND po.prestatus='A' 
						AND d.esdid=228
						AND ptoid = 5
						)
					END as valortotal,			   
		   	(SELECT sum(ep.empvalorempenho) FROM cte.empenho ep WHERE ep.empnumeroprocesso=p.pronumeroprocesso) as valorempenhado,";
		   	if($_REQUEST['exportarexcel']){
			   	 $sql .= "CASE WHEN ter.terid IS NULL THEN 
			   	 	'Não criado' 
			   	 ELSE 
			   	 	CASE WHEN ter.terassinado=TRUE THEN
			   	 		'Sim' 
			   	 	ELSE 
			   	 		'Não' 
			   	 	END 
			   	 END as assinado,";
		   	 } else {
			   	 $sql .= "CASE WHEN ter.terid IS NULL THEN 
			   	 	'<center>Não criado</center>' 
			   	 ELSE 
			   	 	'<center><input type=checkbox '||CASE WHEN (SELECT COUNT(poaid) FROM cte.processoobraanexo WHERE proid=p.proid)>0 THEN 
			   	 		'disabled' 
			   	 	ELSE 
			   	 		'' 
			   	 	END||' name=termo value='||ter.terid||' onclick=assinarTermo(this); '||CASE WHEN ter.terassinado=TRUE THEN
			   	 		'checked' 
			   	 	ELSE 
			   	 		'' 
			   	 	END ||'></center>' 
			   	 END as assinado,";
		   	 }
		   	if(!$_REQUEST['exportarexcel']){
			 	$sql .= "CASE WHEN ter.terassinado=TRUE THEN '<center><img onclick=\"enviarAnexo2('||p.proid||');\" style=\"cursor:pointer;\" src=\"../imagens/reject.png\"></center>' ELSE '<center>Termo não assinado</center>' END as anexopublicado,";
		   	}
	   	    $sql .= "(SELECT COUNT(eo.preid) FROM cte.empenhoobra eo INNER JOIN cte.empenho emp ON emp.empid=eo.empid WHERE emp.empnumeroprocesso=p.pronumeroprocesso) as qtdobrempenhadas
		FROM 
			cte.processoobra p 
		LEFT JOIN cte.resolucao r ON r.resid=p.resid
		INNER JOIN territorios.municipio m ON m.muncod=p.muncod 
		LEFT JOIN cte.termocompromissopac ter ON ter.proid = p.proid AND ter.terstatus='A' 
		WHERE 1=1
	".(($where)?"AND ".implode(" AND ", $where):"")."";

if($_REQUEST['exportarexcel']) {
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatDocente".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($sql,$cabecalho,100000,5,'N','100%',$par2);
	exit;
} else {
	$db->monta_lista($sql,$cabecalho,100,5,'N','center','N','formprocesso');
}
?>
<script>
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url) {
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(690,400);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	function displayStaticMessage(messageContent,cssClass) {
		messageObj.setHtmlContent(messageContent);
		messageObj.setSize(600,150);
		messageObj.setCssClassMessageBox(cssClass);
		messageObj.setSource(false);	// no html source since we want to use a static message here.
		messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
		messageObj.display();
	}
	function closeMessage() {
		messageObj.close();	
	}
</script>
