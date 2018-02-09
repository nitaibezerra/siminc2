<?php

$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];

$ptrid = $_SESSION["emenda"]["ptridAnalise"];

if( $_REQUEST['requisicao'] == 'mostraXML' ){
	$sql = "select l.logrequest, l.logresponse from emenda.logerroswsdl l where l.logid = '{$_REQUEST['logid']}' and l.logtipo = 'enviaProposta'";
	$arXML = $db->pegaLinha($sql);
	
	if( $_REQUEST['tipo'] == 'E' ){
		$xml = $arXML['logrequest'];
	} else {
		$xml = $arXML['logresponse'];
	}
	
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 100%">
		<tr>
			<th style="text-align: center;">XML</th>
		</tr>
		<tr>
			<td style="font-size: 12px"><pre><?=simec_htmlentities($xml); ?></pre></td>
		</tr>
	</table>
	<?
	exit();
}

include_once APPRAIZ . 'includes/workflow.php';
include  APPRAIZ."includes/cabecalho.inc";

$boReformulacao = pegaPaiPTA( $_SESSION["emenda"]["ptridAnalise"] );
$boReformulacao = ( !empty($boReformulacao) ? true : false );

$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptridAnalise"] );

montaAbasAnalise( $abacod_tela, $url, '' , $estadoAtual, $boReformulacao);

$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );


monta_titulo( 'Envio de Proposta SICONV', $subtitulo_modulo );

if ( $ptrid ){
	
	echo cabecalhoPlanoTrabalho($ptrid, true);
	$docid = criarDocumento( $ptrid );
	$existePendencias = pegaConclusaoAnaliseDados( $ptrid, 'T' );
	
	montaVisualizarPTA( $ptrid, true, $boReformulacao );
	
	$habilAnaliseTecnica = true;
	
	$arSiconv = $db->pegaLinha("select ptrpropostasiconv, ptrpropostasiconvano, ptrnumprocessoempenho, entiddirigente from emenda.planotrabalho where ptrid = $ptrid");
	$ptrpropostasiconv 		= $arSiconv['ptrpropostasiconv'];
	$ptrpropostasiconvano 	= $arSiconv['ptrpropostasiconvano'];
	$ptrnumprocessoempenho 	= $arSiconv['ptrnumprocessoempenho'];
	$entiddirigente 		= $arSiconv['entiddirigente'];
	
	$boExec 	= $db->pegaUm("select count(e.exfid) from emenda.execucaofinanceira e where ptrid = $ptrid and e.ptres is not null and e.plicod is not null");
	$boAnalise 	= $db->pegaUm("select count(a.anaid) from emenda.analise a 
								where a.ptrid = $ptrid 
									and a.anadataconclusao is not null 
								    and a.anatipo = 'T'
								    and a.analote = (SELECT max(analote) 
									    				FROM emenda.analise 
									                    WHERE anatipo = 'T' 
									                    	and ptrid = a.ptrid)");
}
$msg = '';
if( $boExec < 1 ){
	$msg .= 'É necessário informar os dados referente ao plano interno, entre na aba "Informações Gerais" é informe os dados.<br>';
}
if( $boAnalise < 1 ){
	$msg .= 'É necessário informar o parecer técnico, entre na aba "Análise Técnica" é informe o parecer.<br>';
}
if( empty($entiddirigente) ){
	$msg .= 'É necessário atualizar os dados da entidade, clique sobre o nome do orgão ou da entidade para que a mesma possa ser atualizada.<br>';
}
?>
<style>
	.tabelaSiconv{ 
		width: 100%; 
	}
</style>
<script type="text/javascript" src="js/emenda.js"></script>
<table class="tabela" style="width: 100%" cellspacing="1" cellpadding="2" align="center">
<tr>
	<td>
	<table class="tabela" style="width: 100%" cellspacing="1" cellpadding="2">
		<tr>
			<th colspan="2">Dados da Proposta SICONV</th>
		</tr>
		<tr>
			<td class="subtitulodireita" width="20%"><b>Número do Processo:</b></td>
			<td>
			<?
			$ptrnumprocessoempenho = (($ptrnumprocessoempenho)?substr($ptrnumprocessoempenho,0,5)."." .substr($ptrnumprocessoempenho,5,6)."/".substr($ptrnumprocessoempenho,11,4)."-".substr($ptrnumprocessoempenho,15,2):"");
			echo campo_texto( 'ptrnumprocessoempenho', 'S', "$boSolicitado", '', 30, 50, '#####.######/####-##', '', '', '', 0, 'id="ptrnumprocessoempenho"' );
			?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="20%"><b>Nº da Proposta:</b></td>
			<td>
				<?if( empty($ptrpropostasiconv) ){ 
					if( $msg != '' ){ 
						echo '<span style="color: red;"><b>'.$msg.'</b></span><br>
								<input type="button" value="Enviar Proposta SICONV" name="btnEnviarProposta" id="btnEnviarProposta" disabled="disabled">';
					}else{ ?>
						<input type="button" value="Enviar Proposta SICONV" name="btnEnviarProposta" id="btnEnviarProposta" onclick="enviarPropostaSICONV('<?php echo $ptrid; ?>');">
					<?} ?>
				<?} else {
					echo $ptrpropostasiconv.'/'.$ptrpropostasiconvano;
				} ?>
			</td>
		</tr>
		<tr>
			<th colspan="2">Dados do Envio ao SICONV</th>
		</tr>
		<tr>
			<td colspan="2">
			<? 
			$arCPF = array('05646593638', '72571659120');
			if( in_array($_SESSION['usucpf'], $arCPF) ){
				$acao = "'<center>
			    				<img src=\"/imagens/send.png\" border=0 title=\"XML de Envio\" style=\"cursor:pointer;\"\" onclick=\"mostraXML('|| l.logid ||', \'E\');\">&nbsp;
			    				<img src=\"/imagens/reject2.gif\" border=0 title=\"XML de Retorno\" style=\"cursor:pointer;\" onclick=\"mostraXML('|| l.logid ||', \'R\');\">
			    			</center>' as acao,";
				$cabecalho 		= array("Envio/Retorno", "Retorno","CPF","Nome","Data do Envio");
			} else {
				$cabecalho 		= array("Retorno","CPF","Nome","Data do Envio");
			}
				$sql = "SELECT 
						  $acao
						  l.logresponse,
						  public.formata_cpf_cnpj(l.usucpf) as cpf,
			              u.usunome,
						  to_char(l.logdatasolicitacao, 'DD/MM/YYYY HH24:MI:SS') as data
						FROM 
						  emenda.logerroswsdl  l
			              inner join seguranca.usuario u on u.usucpf = l.usucpf
			              WHERE l.ptrid = $ptrid
			              	and l.logtipo = 'enviaProposta' 
			              order by l.logid desc";
				
				
				$param = array('classTable'=>'tabelaSiconv');
				$db->monta_lista($sql, $cabecalho, 100, 10, 'N', 'center', '', '', '','', '', $param);
				?>
			</td>
		</tr>
	</table>
	</td>
	<td width="05%" valign="top" align="center">
			<?php
				wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $ptrid , 'tipo' => "T" ) );	
			?>
	</td>
</tr>
</table>

<div id="dialog_recurso" title="Visualizar XML" style="display: none;" >
	<div style="padding:5px;text-align:justify;" id="mostraRetornoRecurso"></div>
</div>

<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script src="../includes/prototype.js"></script>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<script type="text/javascript">
jQuery.noConflict();

messageObj = new DHTML_modalMessage();	// We only create one object of this class
messageObj.setShadowOffset(5);	// Large shadow

function displayMessage(url){
	messageObj.setSource(url);
	messageObj.setCssClassMessageBox(false);
	messageObj.setSize(560,345);
	messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
	messageObj.display();
}

function closeMessage(){
	messageObj.close();	
}

function enviarPropostaSICONV(ptrid){
	var msg  = '';
		
	var ptrnumprocessoempenho = document.getElementById('ptrnumprocessoempenho').value;
	
	if(ptrnumprocessoempenho.length != 20){
		msg += 'É necessário digitar o número do processo de empenho\n';
	}		
	
	if(msg){
		alert(msg);
		return false;			
	}
	displayMessage('?modulo=principal/popUpSenhaFNDE&acao=A&especie_empenho=enviarproposta&ptrid='+ptrid);						
}

function mostraXML(logid, tipo){
	
	jQuery.ajax({
		type: "POST",
		url: window.location.href,
		data: "requisicao=mostraXML&logid="+logid+"&tipo="+tipo,
		async: false,
		success: function(msg){
			if( tipo == 'E' ){
				jQuery("#mostraRetornoRecurso").css('height', '500px'); 
			} else {
				jQuery("#mostraRetornoRecurso").css('height', '200px'); 
			}
			jQuery("#mostraRetornoRecurso").css('overflow', 'auto');
			
			jQuery( "#dialog_recurso" ).show();
			jQuery( "#mostraRetornoRecurso" ).html(msg);
			jQuery( '#dialog_recurso' ).dialog({
					resizable: false,
					width: 1000,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
						'Selecionar': function() {
							jQuery( this ).dialog( 'close' );
						},
						'Fechar': function() {
							jQuery( this ).dialog( 'close' );
						}
					}
			});
		}
	});
}

function abreEspecificacao(espid){
	window.open( 'emenda.php?modulo=principal/cadastroEspecificacoes&acao=A&popup=true&espid='+espid, 'especificacao', 'width=700,height=350,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
}
</script>