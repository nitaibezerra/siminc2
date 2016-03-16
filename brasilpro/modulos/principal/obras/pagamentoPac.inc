<?php
if($_REQUEST['verificaDocumentacao']){
		
	$sql = "SELECT
				prptipoexecucao 
			FROM 
				cte.processobrasilpro 
			WHERE 
				prpid = ".$_POST['prpid'];
				
	$prptipoexecucao = $db->pegaUm( $sql );
	
	$erros = array();
	
	if( $prptipoexecucao == 'T' ){
		$sql = "SELECT 
					prpid, sum(qtd1) as cadastradas, sum(qtd2) as validadas
				FROM (
						(
							SELECT 
								prp.prpid, count(prp.dopid) as qtd1, 0 as qtd2
							FROM 
								cte.documentobrasilpro prp
							INNER JOIN cte.modelosdocumentos md ON md.mdoid = prp.mdoid
							WHERE
								md.tpdcod = 102
								AND prp.dopstatus = 'A'
							GROUP BY 
								prp.prpid
							
						) UNION ALL (
						
							SELECT 
								prp.prpid, 0 as qtd1, count(prp.dopid) as qtd2
							FROM 
								cte.documentobrasilpro prp
							INNER JOIN cte.modelosdocumentos md ON md.mdoid = prp.mdoid
							WHERE 
								prp.dopusucpfvalidacaogestor IS NOT NULL
								AND md.tpdcod = 102
								AND prp.dopstatus = 'A'
							GROUP BY
								prp.prpid
						)
				) as f
				WHERE
					prpid = ".$_POST['prpid']."
				GROUP BY
					prpid";
		
		$dado = $db->pegaLinha( $sql );
		
		if( $dado['cadastradas'] != $dado['validadas'] || $dado['cadastradas'] == 0 ){
			$erro = "Pagamento bloqueado para os processos sem\nTermo de Compromisso validado pelo gestor.";
		}
		
	} else {
		$sql = "SELECT 
					prp.dopid
				FROM 
					cte.documentobrasilpro prp
				INNER JOIN cte.modelosdocumentos md ON md.mdoid = prp.mdoid
				WHERE
					md.tpdcod = 16
					AND prp.dopstatus = 'A'
					AND prpid = ".$_POST['prpid'];
		
		$dadoConv = $db->pegaUm( $sql );
		
		if( $dadoConv['dopid'] == '' ){
			$erros[] = "Convênio";
		} 

		$sql = "SELECT 
					prp.dopid
				FROM 
					cte.documentobrasilpro prp
				INNER JOIN cte.modelosdocumentos md ON md.mdoid = prp.mdoid
				WHERE
					md.tpdcod = 101
					AND prp.dopstatus = 'A'
					AND prpid = ".$_POST['prpid'];
		
		$dadoPar = $db->pegaUm( $sql );
		
		if( $dadoPar['dopid'] == '' ){
			$erros[] = "Parecer";
		} 
		if( is_array( $erros ) ){
			if( $erros[0] ){
				$erro = "Pagamento bloqueado por falta do(s) documento(s) de " . implode(" e ", $erros) . ".";
			}
		}
	}
	
	if( $erro ){
		echo $erro;
	} else {
		echo "valida";
	}
	
	exit;
}
//include '_funcoes_empenho.php';

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
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="./js/brasilproobras.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>

<script type="text/javascript">
	
	jQuery.noConflict();

	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url){
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(560,140);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	
	function closeMessage(){
		messageObj.close();	
	}
</script>
<script>
function verificaDocumentos( prpnumeroprocesso, prpid, tipo ){
	var myajax = new Ajax.Request('brasilpro.php?modulo=principal/obras/pagamentoPac&acao=A', {
		        method:     'post',
		        parameters: '&verificaDocumentacao=true&prpid='+prpid+'&prpnumeroprocesso='+prpnumeroprocesso,
		        asynchronous: false,
		        onComplete: function (res){
		        	if( res.responseText == 'valida' ){
						window.open("brasilpro.php?modulo=principal/obras/solicitacaoPagamentoPac&acao=A&processo="+prpnumeroprocesso+"&prpid="+prpid,"Empenho","scrollbars=yes,height=500,width=800,status=no,toolbar=no,menubar=no,location=no");
					} else {
						alert(res.responseText);
					}
		        }
		  });
}
function carregarListaEmpenhoPagamentoPar(pro, obj) {
	var tabela = obj.parentNode.parentNode.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode.parentNode;

	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex+1);
		var col0 = nlinha.insertCell(0);
		col0.innerHTML = "&nbsp;";
		var col1 = nlinha.insertCell(1);
		col1.id="listapagamentoprocesso_"+pro;
		col1.colSpan=8;
		carregarListaPagamentoEmpenhoPar('', pro);
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex+1);	
	}
}

function carregarPagamento(empid, obj) {
	
	var linha = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	
	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncol = nlinha.insertCell(0);
		ncol.colSpan=8;
		ncol.innerHTML="Carregando...";
		jQuery.ajax({
			type: "POST",
			url: "brasilpro.php?modulo=principal/obras/solicitacaoPagamentoPac&acao=A",
			data: "requisicao=listaPagamento&empid="+empid,
			async: false,
			success: function(msg){
			ncol.innerHTML=msg;
		}
		});
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}
	
}
</script>
<form method="post" name="formulario" id="formulario">
<table id="pesquisas" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<thead>
		<tr>
			<td class="textosTabela"></td>
			<td class="camposTabela"></td>
			<td class="textosTabela"></td>
			<td></td>
		</tr>
	</thead>
	<tbody>
	<tr>
		<td class="SubTituloDireita">Número de Processo:</td>
		<td colspan="3">
		<?php
			$filtro = simec_htmlentities( $_POST['numeroprocesso'] );
			$numeroprocesso = $filtro;
			echo campo_texto( 'numeroprocesso', 'N', 'S', '', 50, 200, '', ''); 
		?>
		</td>
		
	</tr>
	<tr>
		<td class="SubTituloDireita" >Município:</td>
		<td colspan="3">
		<?php 
			$filtro = simec_htmlentities( $_POST['municipio'] );
			$municipio = $filtro;
			echo campo_texto( 'municipio', 'N', 'S', '', 50, 200, '', ''); 
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Estado:</td>
		<td colspan="3">
		<?php
			$estuf = $_POST['estuf'];
			$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
			$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Forma de Execução:</td>
		<td colspan="3">
		<?php
			$frmid = $_POST['frmid'];
			$sql = "SELECT frmid as codigo , frmdsc as descricao FROM cte.formaexecucao WHERE frmid in (6, 13) ORDER BY frmdsc ASC";
			$db->monta_combo( "frmid", $sql, 'S', 'Forma de Execução', '', '' );
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Tipo de Subação:</td>
		<td colspan="3">
		<?php
			$ppsid = $_POST['ppsid'];
			$sql = "SELECT pts.ppsid as codigo, pts.ppsdsc || ' - ' || fe.frmdsc  AS descricao 
					FROM cte.proposicaosubacao pts
					INNER JOIN cte.formaexecucao fe ON fe.frmid = pts.frmid
					WHERE fe.frmid in (6, 13)
					ORDER BY pts.ppsdsc, fe.frmdsc";
			$db->monta_combo( "ppsid", $sql, 'S', 'Tipo de subação - Forma de Execução', '', '' );
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Empenho Solicitado:</td>
		<td colspan="3">
			Sim: <input <?php echo $_REQUEST['empenhado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="1" >
			Não: <input <?php echo $_REQUEST['empenhado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="2" >
		</td> 
	</tr>
	<tr>
		<td class="SubTituloDireita" >Tipo Execução:</td>
		<td colspan="3">
			Convênio: <input <?php echo $_REQUEST['tipoExecucao'] == "C" ? "checked='checked'" : "" ?> type="radio" name="tipoExecucao" value="C" >
			Termo: <input <?php echo $_REQUEST['tipoExecucao'] == "T" ? "checked='checked'" : "" ?> type="radio" name="tipoExecucao" value="T" >
		</td> 
	</tr>
	<tr>
		<td class="subtitulodireita">Validação Gestor:</td>
		<td>
			Sim: <input <?php echo $_REQUEST['dopusucpfvalidacaogestor'] == "t" ? "checked='checked'" : "" ?>  type="radio" value="t" id="dopusucpfvalidacaogestor" name="dopusucpfvalidacaogestor" />
			Não: <input <?php echo $_REQUEST['dopusucpfvalidacaogestor'] == "f" ? "checked='checked'" : "" ?>  type="radio" value="f" id="dopusucpfvalidacaogestor" name="dopusucpfvalidacaogestor" />
		</td>
	</tr>		
	<tr>
		<td class="subtitulodireita">Validação FNDE:</td>
		<td>
			Sim: <input <?php echo $_REQUEST['dopusucpfvalidacaofnde'] == "t" ? "checked='checked'" : "" ?> type="radio" value="t" id="dopusucpfvalidacaofnde" name="dopusucpfvalidacaofnde" />
			Não: <input <?php echo $_REQUEST['dopusucpfvalidacaofnde'] == "f" ? "checked='checked'" : "" ?> type="radio" value="f" id="dopusucpfvalidacaofnde" name="dopusucpfvalidacaofnde"  />
		</td>
	</tr>	
	<tr>
		<td></td>
		<td colspan="3">
			<input class="botao" type="submit" name="pesquisar" value="Pesquisar" />
			<input class="botao" type="button" name="todos" value="Ver todos" onclick="window.location='brasilpro.php?modulo=principal/obras/empenhoPac&acao=A';" />
		</td>
	</tr>
	</tbody>
</table>
</form>
<?php

if($_REQUEST['pesquisar']) {
	
	if($_REQUEST['municipio']) {
		$where[] = "removeacento(m.mundescricao) ILIKE removeacento('%".$_REQUEST['municipio']."%')";
	}
	
	if($_REQUEST['estuf']) {
		$where[] = "m.estuf='".$_REQUEST['estuf']."'";
	}
	if($_REQUEST['empenhado'] == "1") {
		$where[] = "(SELECT sum(ep2.empvalorempenho) FROM cte.empenho ep2 WHERE ep2.empnumeroprocesso=p.prpnumeroprocesso) > 0";
	}
	if($_REQUEST['empenhado'] == "2") {
		$where[] = "(SELECT sum(ep2.empvalorempenho) FROM cte.empenho ep2 WHERE ep2.empnumeroprocesso=p.prpnumeroprocesso) = 0";
	}
	if($_REQUEST['pagamento'] == "1") {
		$where[] = "p.prpnumeroprocesso IN ( SELECT em.empnumeroprocesso FROM cte.empenho em INNER JOIN cte.pagamento pg ON em.empid=pg.empid )";
	}
	if($_REQUEST['pagamento'] == "2") {
		$where[] = "p.prpnumeroprocesso NOT IN ( SELECT em.empnumeroprocesso FROM cte.empenho em INNER JOIN cte.pagamento pg ON em.empid=pg.empid )";
	}
	if($_REQUEST['pagamento'] == "3") {
		$where2[] = "valorpagamento > 0";
	}
	
	if($_REQUEST['tipoExecucao'] == "C") {
		$where[] = "p.prptipoexecucao = 'C'";
	}
	
	if($_REQUEST['tipoExecucao'] == "T") {
		$where[] = "p.prptipoexecucao = 'T'";
	}
	
	if($_REQUEST['tipoobra'] == "P") {
		$where[] = "protipo = 'P'";
	}
	if($_REQUEST['tipoobra'] == "Q") {
		$where[] = "protipo <> 'P'";
	}
	if($_REQUEST['resid']) {
		$where[] = "r.resid = '".$_REQUEST['resid']."'";
	}
	if($_REQUEST['dopusucpfvalidacaofnde'] == 't') {
		$where[] = "dp.dopusucpfvalidacaofnde IS NOT NULL ";
	}
	if($_REQUEST['dopusucpfvalidacaofnde'] == 'f') {
		$where[] = "dp.dopusucpfvalidacaofnde IS NULL ";
	}
	if($_REQUEST['dopusucpfvalidacaogestor'] == 't') {
		$where[] = "dp.dopusucpfvalidacaogestor IS NOT NULL ";
	}
	if($_REQUEST['dopusucpfvalidacaogestor'] == 'f') {
		$where[] = "dp.dopusucpfvalidacaogestor IS NULL ";
	}
}
if($_REQUEST['exportarexcel']){
	$cabecalho = array("Nº Processo", "Nº Documenta", "Município", "UF", "Valor empenhado(R$)", "Valor pagamento(R$)");
} else {
	$cabecalho = array("&nbsp;", "&nbsp;", "Nº Processo", "Nº Documenta", "Município", "UF", "Valor empenhado(R$)", "Valor pagamento(R$)", "Tipo");
}

$consultaDocumenta  = "'<a href=\"#\" onclick=\"tramitar(\''||p.prpdocumenta||'\');\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" title=\"Consultar histórico do documento no sistema Documenta do FNDE\" border=\"0\"></a>'";
$insereDocumenta  = "'<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"cadastraDocumenta(\''||p.prpnumeroprocesso||'\', \'pagamentoPar\');\" title=\"Consultar histórico do documento no sistema Documenta do FNDE\" border=\"0\">'";

$sql = "SELECT * FROM (
		SELECT DISTINCT ";
		if(!$_REQUEST['exportarexcel']){
			$sql .=	"'<center><img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarListaEmpenhoPagamentoPar(\''||p.prpnumeroprocesso||'\', this);\"></center>' as mais,
			   		-- '<center><img src=../imagens/money.gif style=cursor:pointer; onclick=\"window.open(\'brasilpro.php?modulo=principal/obras/solicitacaoPagamentoPac&acao=A&processo=' || p.prpnumeroprocesso || '&prpid='||p.prpid||'\',\'Empenho\',\'scrollbars=yes,height=500,width=800,status=no,toolbar=no,menubar=no,location=no\');\"></center>' as acoes,
			   		 '<center>'||
					case when p.prpdocumenta is not null then $consultaDocumenta
						else $insereDocumenta
					end || '&nbsp;<img src=../imagens/money.gif style=cursor:pointer; onclick=\"verificaDocumentos( \'' || p.prpnumeroprocesso || '\', ' || p.prpid || ', \'Empenho\');\"></center>' as acoes,";
		} 
			$sql .=	" p.prpnumeroprocesso, p.prpdocumenta, m.mundescricao, m.estuf, 
			(SELECT COALESCE(sum(ep.empvalorempenho),0.00) FROM cte.empenho ep WHERE ep.empnumeroprocesso=p.prpnumeroprocesso) as valorempenhado,
			(SELECT COALESCE(sum(pg.pagvalorparcela),0.00) FROM cte.pagamento pg INNER JOIN cte.empenho em ON em.empid=pg.empid 
			    WHERE em.empnumeroprocesso=p.prpnumeroprocesso AND pg.pagstatus='A') as valorpagamento,
			CASE WHEN p.prptipoexecucao = 'C' THEN 'Convênio' ELSE 'Termo' END as tipo
		 FROM cte.processobrasilpro p
		 INNER JOIN cte.instrumentounidade iu on iu.inuid = p.inuid
		 INNER JOIN cte.empenho e ON e.empnumeroprocesso = p.prpnumeroprocesso
		 INNER JOIN territorios.municipio m ON m.muncod= iu.muncod 
		 LEFT JOIN cte.documentobrasilpro dp ON dp.prpid = p.prpid AND dopstatus = 'A'
		".(($where)?"WHERE ".implode(" AND ", $where):"")." ) foo 
		".(($where2)?"WHERE ".implode(" AND ", $where2):"");
//ver($sql);die;
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
	//dbg($sql);
	$db->monta_lista($sql,$cabecalho,100,5,'N','center','N','formprocesso');
}
?>
<?php
$largura = "250px";
$altura = "120px";
$id = "div_auth_consulta";
?>
<style>
	.popup_alerta{
			width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>
<div id="<?php echo $id ?>" class="popup_alerta <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td width="30%" class="SubtituloDireita">Usuário:</td>
			<td><?php echo campo_texto("ws_usuario_consulta","S","S","Usuário","22","","","","","","","id='ws_usuario_consulta'") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Senha:</td>
			<td>
				<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="" size="23" id="ws_senha_consulta" name="ws_senha_consulta">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#D5D5D5" colspan="2">
				<input type="hidden" name="ws_empid" id="ws_empid" value="" />
				<input type="hidden" name="ws_processo" id="ws_processo" value="" />
				<input type="button" name="btn_enviar" onclick="consultarEmpenhoWS()" value="ok" />
				<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
			</td>
		</tr>
		</table>
	</div>
</div>