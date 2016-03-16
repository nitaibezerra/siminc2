<?php

// ------------- DECLARAÇÃO DE VARIAVEIS ------------- //
$empid	 	= $_REQUEST['empid'] 	? $_REQUEST['empid'] 	: 0;
if( $_REQUEST['proid'] ){
	$processo 	= $_REQUEST['proid'];
	$_SESSION['execucaofinanceira']['proid'] = $_REQUEST['proid']; 
} else {
	$processo = $_SESSION['execucaofinanceira']['proid'];
}
if( empty($processo) ) die("<script> alert('Erro ao tentar identificar o processo!'); window.location.href = 'execucaofinanceira.php?modulo=principal/listaDeProcessos&acao=A';</script>");

$obProcesso = new Processo();
$obProcesso->carregarPorId($processo);
$obTipoProc = new TipoProcesso($obProcesso->tipid);
$obEmpenho = new Empenho($obProcesso);

// ------------- REQUISIÇÕES AJAX E FUNÇÕES ------------- //

if( $_POST['requisicao'] == 'mostraInformacoesEmpenho' ){
	header('content-type: text/html; charset=ISO-8859-1');
	$obEmpenho->mostraInformacoesEmpenho( $_POST['empid'] );
	exit();
}

if( $_POST['requisicao'] == 'carregarFonteRecurso' ){
	echo carregarFonteRecurso($_POST);
	exit();
}

if( $_POST['requisicao'] == 'carregarPlanoInterno' ){
	echo carregarPlanoInterno($_POST);
	exit();
}

if( $_POST['requisicao'] == 'carregarPtres' ){
	echo carregarPtres($_POST);
	exit();
}

include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, $parametros);

/*$valorEmpenhado = $db->pegaUm("SELECT sum(empvalorempenhado) FROM execucaofinanceira.empenho where proid = $processo");
$valorProjeto = $db->pegaUm("SELECT sum(pc.prcvalortotal) FROM execucaofinanceira.processocomposicao pc where proid = $processo");*/

?>
<?php monta_titulo( $titulo_modulo, '' ); ?>
<? $obProcesso->montaCabecalhoProcesso(); ?>
<style>	
	.popup_alerta
	{
		width:<?php echo $largura ?>;
		height:<?php echo $altura ?>;
		position:absolute;
		z-index:0;
		top:50%;
		left:50%;
		margin-top:-<?php echo $altura/2 ?>;
		margin-left:-<?php echo $largura/2 ?>;
		border:solid 2px black;
		background-color:#FFFFFF;
		display:none
	}
	
	label { cursor: pointer; }
	.ui-dialog-titlebar{
	    		text-align: center;
	    		}
</style>
<script language="javascript" type="text/javascript" src="js/modal.popup.js"></script>
<link rel='stylesheet' type='text/css' href='../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css'/>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<div class="demo">
	<div id="dialog-confirm" title="Resposta da solicitação de Empenho:"></div>
</div>
<form name="formempenho" id="formempenho" method="post" action="" >
	<table id="total" align="center" border="0" width="95%" class="tabela" cellpadding="1" cellspacing="1">
		<tr>
			<td><br>
				<table id="tb_empenho" align="center" border="0" style="width: 100%" class="tabela" cellpadding="3" cellspacing="2">
					<tr>
						<th colspan="2">Solicitar Empenho</th>
					</tr>
					<tr>
						<td style="text-align: center;">
						<?
						$obEmpenho->listaDadosPassiveisDeEmpenho();
						?>
						<table id="total" align="center" border="0" style="width: 100%" class="tabela" cellpadding="3" cellspacing="1">
					<tr>
						<td class="subtitulodireita"><b>Plano Interno:</b></td>
						<td>
							<span id="planointernoSPAN">
							<?php
							$sql = "SELECT 	DISTINCT
											plinumplanointerno as codigo,
											plinumplanointerno as descricao
									FROM
										par.planointerno
									WHERE 1 = 2";
							$db->monta_combo( "planointerno", $sql, 'S', 'Selecione', '', '' );
							?>
							</span>
						</td>
					</tr>
					<tr>
						<td class="subtitulodireita"><b>PTRES:</b></td>
						<td>
							<span id="ptresSPAN">
							<?php
							$sql = "select 
										pliptres as codigo,
										pliptres as descricao
									from par.planointerno
									where 1 = 2";
							$db->monta_combo( "ptres", $sql, 'S', 'Selecione', '', '' );
							?>
							</span>
						</td>
					</tr>
					<tr>
						<td class="subtitulodireita"><b>Fonte de Recurso:</b></td>
						<td>
							<span id="fonteRecursoSPAN">
							<?php
							$sql = "SELECT fonte as codigo, fonte as descricao FROM financeiro.empenhopar WHERE ptres = '0'";
							$db->monta_combo( "fonte", $sql, 'S', 'Selecione', '', '' );
							?>
							</span>
						</td>
					</tr>
					<tr>
						<td class="subtitulodireita"><b>Centro de Gestão:</b></td>
						<td>
								<select  name='gestaosolicitacao' id="gestaosolicitacao">
									<option value="61400000000" selected="selected">61400000000</option>
									<option value="61500000000" >61500000000</option>
									<option value="61700000000">61700000000</option>
								</select>
						</td>
					</tr>
					<?php 
						if( $obProcesso->proesfera == 'E' ){ ?>
						<tr>
							<td class="subtitulodireita"><b>Natureza de Despesa:</b></td>
							<td>
									<select  name='naturezadespesasolicitacao' id="naturezadespesasolicitacao">
										<option value="44304200">Capital Estados - 44304200</option>
										<option value="33304100">Custeio Estados - 33304100</option>
									</select>	
							</td>
						</tr>
					<?php } else { ?>
						<tr>
							<td class="subtitulodireita"><b>Natureza de Despesa:</b></td>
							<td>
									<select  name='naturezadespesasolicitacao' id="naturezadespesasolicitacao">
										<option value="44404200" selected="selected">Capital Município - 44404200</option>
										<option value="33404100" >Custeio Município - 33404100</option>
									</select>	
							</td>
						</tr>
					<?php } ?>
					<tr>
						<td>
							<input name='gestaosolicitacao' id="gestaosolicitacao" value="61400000000"  type="hidden"  />
						</td>
					</tr>
					<tr>
						<td class="subtitulodireita" width="50%"><b>Valor total de empenho:</b></td>
						<td><input type="text" id="id_total" name="name_total" size="20" class="normal" style="text-align:right;" readonly="readonly" value="0,00"></td>
					</tr>
					<tr>
						<td colspan="2" class="subtituloCentro">
							<input type="button" name="solicitar" <?php echo $disabled; ?> id="solicitar" value="Solicitar empenho">
							<input type="button" id="visualizar" name="visualizar" value="Visualizar XML" onclick="visualizaEmpenho();">
						</td>
					</tr>
				</table>
						
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td style="height: 5px"></td>
		</tr>
		<tr>
			<td>
				<table id="tb_pagamento" align="center" border="0" style="width: 100%" class="tabela" cellpadding="3" cellspacing="2">
					<tr>
						<th colspan="2">Pagamento</th>
					</tr>
					<tr>
						<td colspan="2">
							<?
							$obEmpenho->carregarListaEmpenhosRealizados();
							?>
						</td>
					</tr>
				</table>
			</td>
		</tr>
	</table>
</form>
<?
$wsusuario = 'wsmec';
$wssenha = 'Ch0c014t3';

$largura 	= "270px";
$altura 	= "150px";
$id 		= "div_auth";
?>
<?
/*$usuario = 'juliov';  
$senha   = 'vasco011';*/

$usuario = 'MECTIAGOT';
$senha   = 'M3135689';
?>
<div class="demo">
	<div id="dialog-confirm" title="Resposta da solicitação de Empenho:"></div>
</div>

<div id="dialog-aut" title="Autenticação" style="display:none;">
	<style>
		
	</style>
	<span class="ui-icon ui-icon-alert" style="float:left; margin:0 7px 5px 0;"></span>
	<div id="dialog-content-aut">
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td width="30%" class="ui-button ui-widget" style="text-align: right; font-size: 1.5em; color: #333333; font-weight: bold;">Usuário:</td>
				<td>					
					<input type="text" class="ui-button ui-widget ui-state-default ui-corner-all ui-button-text-only" title="Usuário" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
						onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="<?=$usuario; ?>" size="23" id="wsusuario" name="wsusuario">
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			<tr>
				<td style="text-align: right; font-size: 12px; color: #696969; font-weight: bold;">Senha:</td>
				<td>
					<input type="password" class="ui-button ui-widget ui-state-default ui-corner-all ui-button-text-only" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
						onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="<?=$senha; ?>" size="23" id="wssenha" name="wssenha">
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
		</table>
	</div>
	<div id="resposta" style="color: red; font-size: 0.9em"></div>
</div>

<div id="visXML" title="XML de Empenho" class="popup_alerta2" style="display: none" >
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td id="visXMLDadosEmpenho"></td>
		</tr>
		</table>
	</div>
</div>

<script type="text/javascript">

    $(document).ready(function() {
            
        $('#solicitar').click(function(e) {	
        	$('.ui-icon').hide();				
			$( '#dialog:ui-dialog' ).dialog( 'destroy' );
			$( '#dialog-aut' ).dialog({
				resizable: false,
				width: 300,
				modal: true,
				show: { effect: 'drop', direction: "up" },
				buttons: {
					'Ok': function() {
						$( this ).dialog( 'close' );
						solicitaEmpenho();
					},
					'Cancel': function() {
						$( this ).dialog( 'close' );
						//window.location.reload();
					}
				}
			});
       });
       
       $('#div_dadosaempenhar').css('height', $(window).height() - 350);
        
	});
	
	function visualizaEmpenho(){
		var dados = $('#formempenho').serialize();
		
		$.ajax({
			type: "POST",
			url: "execucaofinanceira.php?modulo=principal/executarEmpenho&acao=A",
			data: "requisicao=solicitarEmpenho&tipo=visualiza&"+dados,
			async: false,
			success: function(msg){
				$( "#visXMLDadosEmpenho" ).html(msg);
				$( '#visXML' ).dialog({
						resizable: false,
						width: 800,
						modal: true,
						show: { effect: 'drop', direction: "up" },
						buttons: {
							'Ok': function() {
								$( this ).dialog( 'close' );
							}
						}
				});
			}
		});
	}
	
	function solicitaEmpenho(){
		var usuario = $('#wsusuario').val();
		var senha = $('#wssenha').val();
		
		if( usuario == '' ){
			$('#resposta').html('Usuário está em branco!');
			return false;
		}
		var boSelect = false;
		$("#formempenho input").each(function(){
			if( $(this).attr('type') == 'checkbox'){
				if($(this).attr('checked') == true){
					boSelect = true;
				}
			}
		});
		
		if($('[name="fonte"]').val() == ''){
			alert('O campo "Fonte de Recurso" é obrigatório!');
			$('[name="fonte"]').focus();
			return false;
		}
		if($('[name="gestaosolicitacao"]').val() == ''){
			alert('O campo "Centro de Gestão" é obrigatório!');
			$('[name="gestaosolicitacao"]').focus();
			return false;
		}
		if($('[name="planointerno"]').val() == ''){
			alert('O campo "Plano Interno" é obrigatório!');
			$('[name="planointerno"]').focus();
			return false;
		}
		if($('[name="ptres"]').val() == ''){
			alert('O campo "PTRES" é obrigatório!');
			$('[name="ptres"]').focus();
			return false;
		}
		if($('[name="naturezadespesasolicitacao"]').val() == ''){
			alert('O campo "Natureza de Despesa" é obrigatório!');
			$('[name="naturezadespesasolicitacao"]').focus();
			return false;
		}
		
		if( boSelect == false ){
			alert('Selecione pelo menos um item para emepnhar!');
			return false;
		} else {
		
			var dadosForm = $('#formempenho').serialize();	        
	        $.ajax({
		   		type: "POST",
		   		url: "execucaofinanceira.php?modulo=principal/executarEmpenho&acao=A",
		   		data: "wsusuario=" + usuario + "&wssenha=" + senha + "&requisicao=solicitarEmpenho&"+dadosForm,
		   		async: false,
		   		success: function(msg){
		   			
		   			$( "#dialog-confirm" ).html(msg);	
					$( "#dialog-confirm" ).dialog({
						resizable: true,
						//height:500,
						width:500,
						modal: true,
						show: { effect: 'drop', direction: "up" },
						buttons: {
							"OK": function() {
								$( this ).dialog( "close" );
								window.location.reload();
								
							}
							
						}
					});
		   		}
			});
		}
	}
	
	function mostrarDadosPTA(ptrid){
		window.open("execucaofinanceira.php?modulo=principal/vizualizarPTA&acao=A&ptrid="+ptrid,"Visualiza Dados",'scrollbars=yes,height=500,width=900,status=no,toolbar=no,menubar=no,location=no');
	}
		
	function marcarChk(obj){
		var piatual = '';
		var piproximo = '';	
		var boPiDiferente = false;
		$('[name="chk[]"]').each(function(){
			
			if( $(this).attr('checked') == true ){
				piproximo = $('#pi_'+$(this).val()).val();
				if( piatual == '' ){
					piatual = $('#pi_'+$(this).val()).val();
				}
				if(piatual != piproximo){
					alert('PI diferente');
					$(obj).attr('checked', false);
					boPiDiferente = true;
					return false;
				}
			}
		});
		
		if( !boPiDiferente ){
			var check = $('[name="chk[]"]:checked').val();		
			document.getElementById('sbdidH').value = obj.value;
			
			if(obj.checked) {
				document.getElementById('id_'+obj.value).className="normal";
				document.getElementById('id_'+obj.value).readOnly=false;
				document.getElementById('id_vlr_'+obj.value).className="normal";
				document.getElementById('id_vlr_'+obj.value).readOnly=false;
			} else {
				document.getElementById('id_'+obj.value).className="disabled";
				document.getElementById('id_'+obj.value).readOnly=true;
				document.getElementById('id_vlr_'+obj.value).className="disabled";
				document.getElementById('id_vlr_'+obj.value).readOnly=true;
			}
			var sbdidH = '';
			if( Number(check) ){
				sbdidH = obj.value;
			} else {
				sbdidH = '0';
			}
			
			//var tabela = obj.parentNode.parentNode.parentNode;
			calcularTotal();
			
			//faço o AJAX para poder carregar o combo da fonte de recurso
			$.ajax({
				type: "POST",
				url: "execucaofinanceira.php?modulo=principal/executarEmpenho&acao=A",
				data: "requisicao=carregarFonteRecurso&sbdid="+sbdidH,
				async: false,
				success: function(msg){
					document.getElementById('fonteRecursoSPAN').innerHTML=msg;
			}
			});
	
			//faço o AJAX para poder carregar o combo do Plano Interno
			$.ajax({
				type: "POST",
				url: "execucaofinanceira.php?modulo=principal/executarEmpenho&acao=A",
				data: "requisicao=carregarPlanoInterno&sbdid="+sbdidH,
				async: false,
				success: function(msg){
					document.getElementById('planointernoSPAN').innerHTML=msg;
			}
			});
			
			filtraPTRES( document.getElementById('planointerno').value );
		}
	}
		
	function filtraPTRES(plicod) {
		sbdid = document.getElementById('sbdidH').value;
		$.ajax({
			type: "POST",
			url: "execucaofinanceira.php?modulo=principal/executarEmpenho&acao=A",
			data: "requisicao=carregarPtres&plicod="+plicod+"&sbdid="+sbdid,
			success: function(msg){
				document.getElementById("ptresSPAN").innerHTML = msg;
			}
		});
	}
	
	function calcularTotal() {
		var codigo = '';
		var vlEmpenhoTotal = 0;
		$("#formempenho input").each(function(){
			if( $(this).attr('type') == 'checkbox'){
				if( $(this).attr('checked') == true ){
					codigo = $(this).val();
					var vlEmpenho = $('#id_vlr_'+codigo).val();
					vlEmpenho = replaceAll(vlEmpenho, '.', '');
					vlEmpenho = replaceAll(vlEmpenho, ',', '.');
					vlEmpenhoTotal = parseFloat(vlEmpenhoTotal) + parseFloat(vlEmpenho)
				}
			}
		});
		
		document.getElementById('id_total').value= mascaraglobal('###.###.###.###,##',vlEmpenhoTotal.toFixed(2));
	}
	
	function calculaEmpenho(obj) {
	
		var arSbdid = $(obj).attr('id').split('_');
		
		if( obj.id.substr( 0,6 ) == 'id_vlr' ){ //veio do valor
			var total = obj.value;
			var codigo = arSbdid[2];
			
			var valor = $('#id_'+codigo).val();
			total_mac = total;
			total = replaceAll(total, '.', '');
			total = replaceAll(total, ',', '.');
			
			var total_for = parseFloat(total);			
			var percent = total_for*100/valor;
			
			$('#id_'+codigo).val(percent);
			$('#id_vlr_'+codigo).val(total_mac);					
		} else {
			
			if( $(obj).val() <= 100 ){
				var codigo = arSbdid[1];				
				var vlEmpenho = $('#vlrTotal_'+codigo).val();
				vlEmpenho = replaceAll(vlEmpenho, '.', '');
				vlEmpenho = replaceAll(vlEmpenho, ',', '.');
				
				var total = parseFloat(vlEmpenho)*($(obj).val()/100);
				var total_mac = mascaraglobal('###.###.###.###,##',total.toFixed(2));
				$('#id_vlr_'+codigo).val(total_mac);			
			} else {
				var vlEmpenho = $('#vlrTotal_'+codigo).val();
				$('#id_vlr_'+codigo).val(vlEmpenho);
				$(obj).val(100);
			}
		}
		calcularTotal();	
	}
	
	function verificaPreenchimentoPorcentagem(obj){
	//alert(obj);
	// 0 - &nbsp; 1- Nº do Empenho 2- Subação 3 - Valor da Subação 4 - % Empenhado 5 - Valor Empenhado 6 - % a Empenhar 7 - Valor a Empenhar
		/*var linha 			= obj.parentNode.parentNode;
		var valorSubacao 	= linha.cells[3].childNodes[0].value;
		var valorInformado 	= linha.cells[7].childNodes[0].value;*/
		var arSbdid = $(obj).attr('id').split('_');
		var valorSubacao 	= $('#id_'+arSbdid[1]).val();
		var valorInformado 	= $('#id_vlr_'+arSbdid[1]).val();
		var valorTotSub		= $('#vlrTotal_'+arSbdid[1]).val();
		var porcentagem		= $('#vlr_'+arSbdid[1]).val();
		valorTotSub 		= mascaraglobal('###.###.###.###,##',valorTotSub)
		
		if( valorInformado > valorSubacao ){
			alert('O valor informado para empenho ultrapassa 100% do valor da Subação.');
			$('#id_vlr_'+arSbdid[1]).val(valorTotSub);
			$('#id_'+arSbdid[1]).val(porcentagem);
		}
		
	}
	
	function carregarDadosEmpenho(idImg, empid){
		var img 	 = $('#'+idImg );
		var tr_nome = 'listaDadosEmpenho_'+ empid;
		var td_nome  = 'trV_'+ empid;
		
		if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == ''){
			$('#'+td_nome).html('Carregando...');
			img.attr('src', '../imagens/menos.gif');
			mostraInformacoesEmpenho(empid, td_nome);
		}
		if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
			$('#'+tr_nome).css('display', '');
			img.attr('src', '../imagens/menos.gif');
		} else {
			$('#'+tr_nome).css('display', 'none');
			img.attr('src', '/imagens/mais.gif');
		}
	}
	
	function mostraInformacoesEmpenho(empid, td_nome){
		$.ajax({
	   		type: "POST",
	   		url: "execucaofinanceira.php?modulo=principal/empenhoPagamento&acao=A",
	   		data: "requisicao=mostraInformacoesEmpenho&empid="+empid,
	   		async: false,
	   		success: function(msg){
	   			$('#'+td_nome).html(msg);
	   		}
		});
	}
	
	function carregarListaEmpenhosRealizados(processo) {
		$.ajax({
	   		type: "POST",
	   		url: "execucaofinanceira.php?modulo=principal/empenhoPagamento&acao=A",
	   		data: "requisicao=carregarListaEmpenhosRealizados&processo="+processo,
	   		async: false,
	   		success: function(msg){
	   			document.getElementById('divListaEmpenho').innerHTML = msg;
	   		}
		});
	}
 </script>
