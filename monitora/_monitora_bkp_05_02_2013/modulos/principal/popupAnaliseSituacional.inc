<?php
if ( $_REQUEST['requisicao'] == 'mudarSituacao'){
	
	extract($_POST);
	
	$retorno = false;
	
	$sql = "";
	
	if ($tipo == "O")
	{
		$sql = "update monitora.analiseobjetivo set anosituacao = '{$situacao}', anoobs = '{$anobs}' where anoid = {$anid}";
	}
	elseif ($tipo == "M" )
	{
		$sql = "update monitora.analisemeta set anmsituacao = '{$situacao}', anmobs = '{$anobs}' where anmid = {$anid}";
	}
	elseif ($tipo == "I" )
	{
		$sql = "update monitora.analiseiniciativa set anisituacao = '{$situacao}', aniobs = '{$anobs}' where aniid = {$anid}";
	}
	
	$db->executar($sql);
	$db->commit();
	
	echo ("true");
	die;
}

global $db;

?>
<html>
  <head>
    <script type="text/javascript" src="/includes/prototype.js"></script>
    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    
    
  </head>
<form name="formulario" id="formulario" method="post">
<table  bgcolor="#f5f5f5" align="center" class="tabela" >
	<tr>
		<td class = "subtitulodireita" >
			<center> 
			<h3>Dados da análise</h3>
			</center>
		</td> 
	</tr>
	<tr>
		<td >
			<table cellpadding="0" border="0" width="98%" align="center" id="orcamento"  style="BORDER-RIGHT: #C9C9C9 1px solid; BORDER-TOP: #C9C9C9 1px solid; BORDER-LEFT: #C9C9C9 1px solid; BORDER-BOTTOM: #C9C9C9 1px solid;" >
				<tr>
					<td style="background-color: #C9C9C9;" align="right" >
						<b>Descrição</b>
						<input type="hidden" name="pliptres">
					</td>
					<td style="background-color: #C9C9C9;" align="center" nowrap>
						<?
						extract($_GET);
						
						//análise de objetivo
						if ($tipo == 'O')
						{
							$sql = "select anoid as anid, anodsc as andsc, anosituacao as ansituacao,  'O'  as tipo, anoobs as anobs from monitora.analiseobjetivo where anoid = " .$anid;
						}
						//análise de meta
						elseif ($tipo == 'M')
						{
							$sql = "select anmid as anid, anmdsc as andsc, anmsituacao as ansituacao,  'M'  as tipo, anmobs as anobs from monitora.analisemeta where anmid = " .$anid;
						}
						//análise de iniciativa
						elseif ($tipo == 'I')
						{
							$sql = "select aniid as anid, anidsc as andsc, anisituacao as ansituacao,  'I'  as tipo, aniobs as anobs from monitora.analiseiniciativa where aniid = " .$anid;
						} 
						
						$registros = $db -> pegaLinha($sql);
						
						
						echo ( $registros['andsc']); ?>
					</td>
				</tr>
				<tr>
					<td style="background-color: #C9C9C9;" align="right" >
						<b>Situação</b>
					</td>
					<td style="background-color: #C9C9C9;" align="center" nowrap>
						<?
						
						if ( $registros['ansituacao'] == "")
						{
							$registros['ansituacao'] = 'P';
						}
						$registros['ansituacao'] .= ':'  ;
						echo (	str_replace("S:", "Enviado para o SIOP", 
								str_replace("A:", "Aprovado pela SPO", 
								str_replace("R:", "Enviado para Revisão",  
								str_replace("H:", "Homologado pela SPO", 
								str_replace("E:", "Enviado para SPO", 
								str_replace("P:", "Pendente", $registros['ansituacao']))))))  ); 
						$registros['anosituacao'] = trim($registros['ansituacao'] , ':');
						?>
								
					</td>
				</tr>
				<tr>
					<td style="background-color: #C9C9C9;" align="right" >
						<b>observação</b>
					</td>
					<td style="background-color: #C9C9C9;" align="center" nowrap>
						<?echo ( $registros['anobs']); ?>
					</td>
				</tr>
				<tr>
					<td style="background-color: #C9C9C9;" align="right" >
						<b>Ação</b>
					</td>
					<td style="background-color: #C9C9C9;" align="center" nowrap>
						<?
						if($registros['anosituacao'] == 'P' ||  $registros['anosituacao'] == 'R' ){
						?>
							<input type="button" value="Enviar para SPO" onclick="mudarSituacao(<? echo($registros['anid']); ?>,  'E','<? echo($registros['tipo']); ?>');" style="cursor: pointer;"/>
						<?
						} 
						else if($registros['anosituacao'] == 'E' ){
						?>
							<input type="button" value="Homologar" onclick="mudarSituacao(<? echo($registros['anid']); ?>,  'H','<? echo($registros['tipo']); ?>');" style="cursor: pointer;"/>
							<input type="button" value="Enviar para revisão" onclick="montaShowModal(<? echo($registros['anid']); ?>,'<? echo($registros['tipo']); ?>');" style="cursor: pointer;"/>
						<?
						}
						else if($registros['anosituacao'] == 'H' ){
						?>
							<input type="button" value="Aprovar" onclick="mudarSituacao(<? echo($registros['anid']); ?>,  'A','<? echo($registros['tipo']); ?>');" style="cursor: pointer;"/>
						<?
						}
						else if ($registros['anosituacao'] == 'A' ){
						?>
							<input type="button" value="Enviar para SIOP" onclick="mudarSituacao(<? echo($registros['anid']); ?>,  'S','<? echo($registros['tipo']); ?>');" style="cursor: pointer;"/>
						<?
						}
						?>		
					</td>
				</tr>
			</table>
		</td>
	</tr>
</table>
</form>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script>

function mudarSituacao(anid , situacao, tipo){

	var anobs = '';
	
	if(situacao == 'R'){
		var anobs = $('anobs1').value;
		if(anobs == ''){
			alert('O campo da descrição é obrigatório');
			return false;
		} else {
			closeMessage();
		}
	} 
	
 	var url = window.location.href;
	var parametros = "requisicao=mudarSituacao&anid="+anid+"&situacao="+situacao+"&tipo="+tipo+"&anobs="+anobs;
	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(r) {
				if(r.responseText){
					alert('Dados gravados com Sucesso.');
					window.opener.document.formulario1.submit();
					window.close();
				}
			}
		}
	);
}

var countModal = 1;

function montaShowModal(anid, tipo) {
	var campoTextArea = '<form id="form" name="form"><div class="notprint">'+
			'<textarea class="txareaclsMouseOver" id="anobs'+countModal+'" name="anobs'+countModal+'" cols="80" rows="8" title="Mensagem" '+ 
				'onmouseover="MouseOver( this );" '+
				'onfocus="MouseClick( this );" '+
				'onmouseout="MouseOut( this );" '+
				'onblur="MouseBlur( this ); '+
				'textCounter( this.form.anobs'+countModal+', this.form.no_anobs, 500);" '+ 
				'style="width: 80ex;" '+
				'onkeydown="textCounter( this.form.anobs'+countModal+', this.form.no_anobs, 500 );" '+ 
				'onkeyup="textCounter( this.form.anobs'+countModal+', this.form.no_anobs, 500);">'+
			'</textarea><br> '+
			'<input readonly="readonly" style="border-left: 3px solid rgb(136, 136, 136); text-align: right; color: rgb(128, 128, 128);" '+ 
				'name="no_pihobs" size="6" maxlength="6" value="500" '+
				'class="CampoEstilo" type="text"> '+
			'<font size="1" color="red" face="Verdana"> máximo de caracteres</font> '+
		'</div><div id="print_anobs" class="notscreen" style="text-align: left;"></div>'+
		//checkBoxEmail+
		'</form>';
	var alertaDisplay = '<center><div class="titulo_box" >É necessário colocar observação.<br/ >'+campoTextArea+'</div><div class="links_box" ><br><input type="button" onclick="mudarSituacao('+anid+',  \'R\',\''+tipo+'\')" value="Gravar" /> <input type="button" onclick=\'closeMessage(); return false \' value="Cancelar" /></center>';
	displayStaticMessage(alertaDisplay,false);
	return false;
}

function displayStaticMessage(messageContent,cssClass) {
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	messageObj.setHtmlContent(messageContent);
	messageObj.setSize(420,215);
	messageObj.setCssClassMessageBox(cssClass);
	messageObj.setSource(false);	// no html source since we want to use a static message here.
	messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
	messageObj.display();
}

function closeMessage() {
	messageObj.close();	
}

</script>