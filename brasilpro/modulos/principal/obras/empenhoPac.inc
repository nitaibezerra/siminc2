<?php

if($_REQUEST['requisicao']){	
	$_REQUEST['requisicao']($_POST);
	die;
}

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '', array(8667) );
monta_titulo( $titulo_modulo, '&nbsp;' );

?>
<style>
	#pesquisas thead tr td
	{
		background-color: #F7F7F7;
	}

	#pesquisas thead tr td.textosTabela
	{
		width: 200px;
	}
	#pesquisas thead tr td.camposTabela
	{
		width: 300px;
	}
	#pesquisas tbody tr td
	{
		background-color: #F7F7F7;
	}
	
	#pesquisas tbody tr td.SubTituloDireita
	{
		FONT-SIZE: 8pt;
    	COLOR: black;
    	FONT-FAMILY: Arial, Verdana;
    	TEXT-ALIGN: right;
		BACKGROUND-COLOR: #dcdcdc;
	}
	
	#pesquisas tbody tr td input.botao
	{
		border: 1px solid #757575;
		background-color: #DCDCDC;
		font-size: 11px;
		font-family: arial;
		font-weight: bold;
	}
	
	
	
	
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/brasilproobras.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script type="text/javascript">
	//jQuery.noConflict();
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
			$sql = "SELECT pts.ppsid as codigo, substring(pts.ppsdsc, 0, 50) || ' - ' || fe.frmdsc  AS descricao 
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
			Todos: <input <?php echo $_REQUEST['tipoExecucao'] == "TTE" ? "checked='checked'" : "" ?> type="radio" name="tipoExecucao" value="TTE" >
		</td> 
	</tr>
	<tr>
		<td class="subtitulodireita">Validação Gestor:</td>
		<td colspan="3">
			Sim: <input <?php echo $_REQUEST['dopusucpfvalidacaogestor'] == "t" ? "checked='checked'" : "" ?>  type="radio" value="t" id="dopusucpfvalidacaogestor" name="dopusucpfvalidacaogestor" />
			Não: <input <?php echo $_REQUEST['dopusucpfvalidacaogestor'] == "f" ? "checked='checked'" : "" ?>  type="radio" value="f" id="dopusucpfvalidacaogestor" name="dopusucpfvalidacaogestor" />
			Todos: <input <?php echo $_REQUEST['dopusucpfvalidacaogestor'] == "TVG" ? "checked='checked'" : "" ?> type="radio" name="dopusucpfvalidacaogestor" value="TVG" >
		</td>
	</tr>		
	<tr>
		<td class="subtitulodireita">Validação FNDE:</td>
		<td colspan="3">
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
		
		if($_REQUEST['numeroprocesso']) {
			$where[] = "p.prpnumeroprocesso = '".$_REQUEST['numeroprocesso']."'";
		}
		if($_REQUEST['municipio']) {
			$where[] = "removeacento(m.mundescricao) ILIKE removeacento('%".$_REQUEST['municipio']."%')";
		}
		
		if($_REQUEST['estuf']) {
			$where[] = "m.estuf='".$_REQUEST['estuf']."'";
		}
		
		if($_REQUEST['frmid']) {
			//$where[] = "s.frmid = '".$_REQUEST['frmid']."'";
		}
		
		if($_REQUEST['ppsid']) {
			//$where[] = "pts.ppsid = '".$_REQUEST['ppsid']."'";
		}
		
		if($_REQUEST['empenhado'] == "1") {
			$where[] = "(SELECT sum(ep2.empvalorempenho) FROM cte.empenho ep2 WHERE ep2.empnumeroprocesso=p.prpnumeroprocesso) > 0";
		}
		if($_REQUEST['empenhado'] == "2") {
			$where[] = "(SELECT sum(ep2.empvalorempenho) FROM cte.empenho ep2 WHERE ep2.empnumeroprocesso=p.prpnumeroprocesso) = 0";
		}

		if($_REQUEST['tipoExecucao'] == "C") {
			$where[] = "p.prptipoexecucao = 'C'";
		}
		
		if($_REQUEST['tipoExecucao'] == "T") {
			$where[] = "p.prptipoexecucao = 'T'";
		}
		
		if($_REQUEST['dopusucpfvalidacaofnde'] == 't') {
			$where[] = "dp.dopusucpfvalidacaofnde IS NOT NULL ";
		}
		if($_REQUEST['dopusucpfvalidacaofnde'] == 'f') {
			$where[] = "dp.dopusucpfvalidacaofnde IS NULL ";
		}
		if($_REQUEST['dopusucpfvalidacaogestor'] == 't') {
			$where[] = "dp.dopusucpfvalidacaogestor IS NOT NULL ";
			if($_REQUEST['tipoExecucao'] == "T") {
				$where[] = "mdoid = 3"; // Termo
			}else if ($_REQUEST['tipoExecucao'] == "C"){
				$where[] = "mdoid = 2"; // Convênio
			}else if ($_REQUEST['tipoExecucao'] == "TTE"){
				$where[] = "mdoid in (2,3)"; // Convênio
			}
			
		}
		if($_REQUEST['dopusucpfvalidacaogestor'] == 'f') {
			$where[] = "dp.dopusucpfvalidacaogestor IS NULL ";
			if($_REQUEST['tipoExecucao'] == "T") {
				$where[] = "mdoid = 3"; // Termo
			}else if ($_REQUEST['tipoExecucao'] == "C"){
				$where[] = "mdoid = 2"; // Convênio
			}else if ($_REQUEST['tipoExecucao'] == "TTE"){
				$where[] = "mdoid in (2,3)"; // Convênio
			}
			
		}	
	}

	//$cabecalho = array("&nbsp;","&nbsp;","Nº Processo", "Município","UF", "Valor empenhado(R$)","Tipo");
	/*
	$sql = "SELECT DISTINCT
				'<center><img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarListaEmpenhoPar(\''||p.prpnumeroprocesso||'\', this);\"></center>' as mais,
			   	'<center><img src=../imagens/money.gif style=cursor:pointer; onclick=\"window.open(\'brasilpro.php?modulo=principal/obras/solicitacaoEmpenhoPac&acao=A&processo=' || p.prpnumeroprocesso || '&prpid='||p.prpid||'&inuid='||p.inuid||'\',\'Empenho\',\'scrollbars=yes,height=500,width=800,status=no,toolbar=no,menubar=no,location=no\');\"></center>' as acoes, 
			   	p.prpnumeroprocesso, 
			   	m.mundescricao, 
			   	m.estuf, 
			 --  (SELECT sum(ep.empvalorempenho) FROM cte.empenho ep WHERE ep.empnumeroprocesso=p.prpnumeroprocesso) as valorempenhado,
			 0 as valorempenhado,
			   -- (SELECT COUNT(emp.sbaid) FROM cte.empenho eo INNER JOIN cte.empenhosubacao emp ON emp.empid=eo.empid WHERE eo.empnumeroprocesso=p.prpnumeroprocesso) as qtdsubacaoempenhada
			   CASE WHEN p.prptipoexecucao = 'C' THEN 'Convênio' ELSE 'Termo' END AS tipo
		FROM cte.instrumentounidade iu
		INNER JOIN cte.pontuacao 		po   on po.inuid   = iu.inuid
		INNER JOIN cte.acao 			a   on a.ptoid   = po.ptoid
		INNER JOIN cte.subacao			s   on s.aciid   = a.aciid 
		INNER JOIN cte.subacaodetalhe sd ON sd.sbaid = s.sbaid AND sd.sbdano = date_part('year', now())
		INNER JOIN cte.processobrasilpro p ON p.inuid = iu.inuid
		LEFT JOIN cte.empenhosubacao ep ON ep.sbaid = s.sbaid AND sd.sbdano = sd.sbdano
		LEFT JOIN cte.proposicaosubacao  pts on pts.frmid = s.frmid
		LEFT JOIN territorios.municipio m ON m.muncod = iu.muncod
		".(($where)?"WHERE ".implode(" AND ", $where):"") 
		." and p.prptipoexecucao = 'C' ORDER BY m.estuf, m.mundescricao";
	dbg($sql);
	*/
	
	$cabecalho = array("&nbsp;","&nbsp;","Nº Processo", "Nº Documenta", "Município","UF", "Valor empenhado(R$)","QTD subações","Tipo");
	
	$consultaDocumenta  = "'<a href=\"#\" onclick=\"tramitar(\''||p.prpdocumenta||'\');\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" title=\"Consultar histórico do documento no sistema Documenta do FNDE\" border=\"0\"></a>'";
	$insereDocumenta  = "'<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"cadastraDocumenta(\''||p.prpnumeroprocesso||'\', \'empenhoPar\');\" title=\"Consultar histórico do documento no sistema Documenta do FNDE\" border=\"0\">'";
	
	/*$insereDocumenta 	= "'<input type=\"text\" class=\" normal\" title=\"\" onkeypress=\"return somenteNumeros(event);\" id=\"prpdocumenta_'||p.prpid||'\" 
							onblur=\"MouseBlur(this); onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" 
							onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'#######/####-#\',this.value);\" value=\"\" maxlength=\"30\" size=\"20\" name=\"prpdocumenta_'||p.prpid||'\" style=\"\">
							<img src=\"../imagens/gif_inclui.gif\" style=\"cursor:pointer;\" onclick=\"cadastraDocumenta();\" title=\"Salvar Documenta\" border=\"0\">'";*/
	
	$sql = "SELECT DISTINCT
				'<center><img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarListaEmpenho(\''||p.prpnumeroprocesso||'\', this);\"></center>' as mais,
			   	'<center>'||
					case when p.prpdocumenta is not null then $consultaDocumenta
						else $insereDocumenta
					end || '&nbsp;<img src=../imagens/money.gif style=cursor:pointer; onclick=\"window.open(\'brasilpro.php?modulo=principal/obras/solicitacaoEmpenhoPac&acao=A&processo=' || p.prpnumeroprocesso || '&prpid='||p.prpid||'&inuid='||p.inuid||'\',\'Empenho\',\'scrollbars=yes,height=500,width=800,status=no,toolbar=no,menubar=no,location=no\');\"> ' || 
			   	' </center>' as acoes, 
			   	p.prpnumeroprocesso,
			   	p.prpdocumenta, 
			   	m.mundescricao, 
			   	m.estuf, 
			   (SELECT sum(ep.empvalorempenho) FROM cte.empenho ep WHERE ep.empnumeroprocesso=p.prpnumeroprocesso) as valorempenhado,
			   (SELECT COUNT(emp.sbaid) 
			   		FROM cte.empenho eo 
			   		INNER JOIN cte.empenhosubacao emp ON emp.empid = eo.empid 
			   		WHERE eo.empnumeroprocesso = p.prpnumeroprocesso) as qtdsubacaoempenhada,
			   CASE WHEN p.prptipoexecucao = 'C' THEN 'Convênio' ELSE 'Termo' END AS tipo
		FROM cte.instrumentounidade iu
		INNER JOIN cte.processobrasilpro p ON p.inuid = iu.inuid
		LEFT JOIN territorios.municipio m ON m.muncod = iu.muncod
		LEFT JOIN cte.documentobrasilpro dp ON dp.prpid = p.prpid AND dopstatus = 'A'
		".(($where)?"WHERE ".implode(" AND ", $where):""). 
		"ORDER BY tipo, m.estuf, m.mundescricao";
//dbg(simec_htmlentities($sql));die;
	$db->monta_lista($sql,$cabecalho,100,5,'N','center','N','formprocesso');
	
	$sql = "SELECT DISTINCT
			    sum(e.empvalorempenho) as valorempenhado
		FROM cte.instrumentounidade iu
		INNER JOIN cte.processobrasilpro p ON p.inuid = iu.inuid
		INNER JOIN cte.empenho e ON e.empnumeroprocesso=p.prpnumeroprocesso";
	
	$valotTotal = $db->pegaUm($sql);
	
$wsusuario = WEB_SERVICE_SIOP_USUARIO;
$wssenha = WEB_SERVICE_SIOP_SENHA;

$largura 	= "250px";
$altura 	= "120px";
$id 		= "div_auth";	
?>
<table id="pesquisas" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td class="subTituloCentro">
	Total Empenhado: <?php echo "R$". number_format($valotTotal, 2, ',', '.');?>
		</td>
	</tr>
</table>

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
			<td><?php echo campo_texto("ws_usuario_consulta","S","S","Usuário","18","","","","","","","id='ws_usuario_consulta'") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Senha:</td>
			<td>
				<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="" size="19" id="ws_senha_consulta" name="ws_senha_consulta">
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
<?php
$largura = "250px";
$altura = "120px";
$id = "div_auth_cancela";
?>
<style>
	.popup_alerta_cancela{
			width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>
<div id="<?php echo $id ?>" class="popup_alerta_cancela <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td width="30%" class="SubtituloDireita">Usuário:</td>
				<td><?php echo campo_texto("ws_usuario_cancela","S","S","Usuário","18","","","","","","","id='ws_usuario_cancela'") ?></td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Senha:</td>
				<td>
					<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="" size="19" id="ws_senha_cancela" name="ws_senha_cancela">
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			<tr>
				<td align="center" bgcolor="#D5D5D5" colspan="2">
					<input type="hidden" name="ws_empid" id="ws_empid" value="" />
					<input type="hidden" name="ws_processo" id="ws_processo" value="" />
					<input type="button" name="btn_enviar" onclick="cancelarEmpenhoWS()" value="ok" />
					<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
				</td>
			</tr>
		</table>
	</div>
</div>

<?php 
function listaEmpenhoProcesso($dados) {
	global $db;
	if($dados['empnumeroprocesso']) {
		$where[] = "empnumeroprocesso='".$dados['empnumeroprocesso']."'";
	} elseif($_SESSION['brasilpro_var']['proid']) {
		$where[] = "proid='".$_SESSION['brasilpro_var']['proid']."'";
	}
	$where[] = "funid = 1";

	$sql = "SELECT '<img align=absmiddle src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarHistoricoEmpenho(\''||e.empid||'\', this);\">' as mais,
				   CASE WHEN e.empsituacao!='CANCELADO' THEN '<img src=../imagens/refresh2.gif style=cursor:pointer; onclick=consultarEmpenho('||e.empid||',\'' || trim(e.empnumeroprocesso) || '\');>' ELSE '&nbsp;' END as acao_consultar,
				   CASE WHEN e.empsituacao!='CANCELADO' THEN '<img src=../imagens/excluir.gif align=absmiddle style=cursor:pointer; onclick=cancelarEmpenho('||e.empid||',\'' || trim(e.empnumeroprocesso) || '\');>' ELSE '&nbsp;' END as acao_cancelar,
				   e.empcnpj, en.entnome, e.empprotocolo, e.empnumero, e.empvalorempenho, u.usunome, e.empsituacao FROM cte.empenho e
			LEFT JOIN cte.processoobra p ON trim(e.empnumeroprocesso) = trim(p.pronumeroprocesso)
			LEFT JOIN seguranca.usuario u ON u.usucpf=e.usucpf
			LEFT JOIN entidade.entidade en ON en.entnumcpfcnpj=e.empcnpj
			LEFT JOIN entidade.funcaoentidade fun ON fun.entid=en.entid
			".(($where)?"WHERE ".implode(" AND ", $where):"");

	$cabecalho = array("&nbsp;","&nbsp;","&nbsp;","CNPJ","Entidade","Nº protocolo","Nº empenho","Valor empenho(R$)","Usuário criação","Situação empenho");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);
}
?>