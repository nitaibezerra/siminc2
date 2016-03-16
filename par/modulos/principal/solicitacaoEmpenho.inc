<?
include '_funcoes_empenho.php';

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

if($_REQUEST['proid']) {
	$_SESSION['par_var']['proid']  = $_REQUEST['proid'];
	$_SESSION['par_var']['esfera'] = $db->pegaUm("SELECT CASE WHEN muncod IS NOT NULL THEN 'municipal' ELSE 'estadual' END FROM par.processoobra WHERE prostatus = 'A' and proid='".$_SESSION['par_var']['proid']."'"); 
}

$processo = $_REQUEST['processo'];

$retornoEmpenhoSigef = verificaEmpenhoSigef( $processo, 'PAC' );

$retorno = verificaEmpenhoValorDivergente($processo, '', 'PAC');

if(!$_SESSION['par_var']['proid']) die("<script>
											alert('Processo não encontrado');
											window.close();
										</script>");


// Monta tarja vermelha com aviso de pendência de obras
montarAvisoCabecalho(null, null, null, null, $_REQUEST['proid']);

?>
<html>
<head>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>
<body>
<?php
monta_titulo( $titulo_modulo, '&nbsp;' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<script language="JavaScript" src="../estrutura/js/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/maskMoney.js" ></script>
<style>

	.vizualisa_obra{
		cursor:pointer;
	}
	.ui-dialog-titlebar{
    	text-align: center;
    }
</style>

<script type="text/javascript" src="./js/par.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<link rel="stylesheet" href="../includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />

<script>
function mostraPendencia(){
	$( "#dialog_pendencia" ).show();
	$( '#dialog_pendencia' ).dialog({
			resizable: false,
			width: 900,
			height: 600,
			modal: true,
			show: { effect: 'drop', direction: "up" },
			buttons: {
				'Fechar': function() {
					$( this ).dialog( 'close' );
				}
			}
	});
}

function reduzirEmpenho(empid, processo, especie) {
	window.open('par.php?modulo=principal/reduzirEmpenhoPAC&acao=A&empid='+empid+'&processo='+processo+'&especie='+especie, 
			    'reduzirEmpenho', 
			    "height=400,width=800,scrollbars=yes,top=0,left=0" );
}	

$(document).ready(function() {

	jQuery('.vizualisa_obra').live('click', function(){
		var preid = jQuery(this).attr('preid');
		var muncod = jQuery(this).attr('muncod');
		window.open('par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=dados&preid='+preid+'&muncod='+muncod, 
					'preobra', 
					"height=400,width=800,scrollbars=yes,top=0,left=0" );
	});

	carregaTelaEmpenhoDivergente();

	jQuery("input.maskMoney").maskMoney({showSymbol:true, symbol:"", decimal:",", thousands:".", allowNegative: true});
	
	carregarListaPreObra();
	carregarListaEmpenhoProcesso( '<? echo $processo ?>' );

	jQuery('.focuscampo').focusin(function(){
		if( parseInt( jQuery(this).val() ) == '0' || parseInt( jQuery(this).val() ) == '00' || parseInt( jQuery(this).val() ) == '0,00' || parseInt( jQuery(this).val() ) == '0.00' ){
			jQuery(this).val('');
		}
	});

	jQuery('.focuscampo').focusout(function(){
		if( jQuery(this).val() == '' ){
			jQuery(this).val('0,00');
		}
	});
});

function abretodosPac(obj){
	$.each($('input[name="chk[]"]'), function(i,v){
		if($(obj).attr('checked')){
			$(v).attr('checked', true);
			marcarChkObrasPacEmp(v);
		} else {
			$(v).attr('checked', false);
			marcarChkObrasPacEmp(v);
		}
	});
}

function calculaPercentualTodosPac(obj){
	var valor = obj.value;
	$('[name="chk[]"]').each(function(){
		var preid = $(this).val();
		
		if( valor == '' ){
			$(this).attr('checked', false);
			document.getElementById('id_'+preid).className="disabled";
			document.getElementById('id_'+preid).readOnly=true;
			document.getElementById('id_'+preid).value = '0,00';
			
			document.getElementById('id_vlr_'+preid).className="disabled vrlaempenhar";
			document.getElementById('id_vlr_'+preid).readOnly=true;
			document.getElementById('id_vlr_'+preid).value = '0,00';
		} else {
			$(this).attr('checked', true);
			document.getElementById('id_'+preid).className="norma";
			document.getElementById('id_'+preid).readOnly=false;
			document.getElementById('id_vlr_'+preid).className="normal vrlaempenhar";
			document.getElementById('id_vlr_'+preid).readOnly=false;
			
			$('#id_'+preid).val(valor);
		}
		calculaEmpenhoObraPac(preid, 'p');
	});
}

function visEmp(){
	var dadosobras = $('#formpreobras').serialize();
	var filtrosobras = $('#formulario').serialize();
	
	$.ajax({
		type: "POST",
		url: "par.php?modulo=principal/solicitacaoEmpenho&acao=A",
		data: "requisicao=solicitarEmpenho&tipo=visualiza&"+filtrosobras+'&'+dadosobras,
		async: false,
		success: function(msg){
			document.getElementById('visXML').style.display='block';
			document.getElementById('visXMLDadosEmpenho').innerHTML=msg;
	}
	});
}

function marcarChkObrasPacEmp(obj) {
	var preid = obj.value;

	if(obj.checked) {
		document.getElementById('id_'+preid).className="norma";
		document.getElementById('id_'+preid).readOnly=false;
		document.getElementById('id_vlr_'+preid).className="normal vrlaempenhar";
		document.getElementById('id_vlr_'+preid).readOnly=false;
		
		var valorObra = parseFloat(document.getElementById('vlrobra_'+preid).value); //valor da obra
		var valorEmpenhado = parseFloat(document.getElementById('vlr_empenhado_'+preid).value); //valor do empenho
		
		var percent = (((valorObra-valorEmpenhado)*100)/valorObra);
		
		document.getElementById('id_'+preid).value = percent;
	} else {
		document.getElementById('id_'+preid).className="disabled";
		document.getElementById('id_'+preid).readOnly=true;
		document.getElementById('id_'+preid).value = '0,00';
		
		document.getElementById('id_vlr_'+preid).className="disabled vrlaempenhar";
		document.getElementById('id_vlr_'+preid).readOnly=true;
		document.getElementById('id_vlr_'+preid).value = '0,00';
	}
	calculaEmpenhoObraPac(preid, 'p');
}

function calculaEmpenhoObraPac(preid, tipo) {
	
	var valorObra = parseFloat(document.getElementById('vlrobra_'+preid).value); //valor da obra	
	var valorEmpenhado = parseFloat(document.getElementById('vlr_empenhado_'+preid).value); //valor do empenho	
	
	if( tipo == 'v' ){ //veio do valor
		if( document.getElementById('chk_'+preid).checked == true ){
			var valorAempenhar = document.getElementById('id_vlr_'+preid).value;
		
			if( valorAempenhar != '' ){
				valorAempenhar = replaceAll(valorAempenhar, '.', '');
				valorAempenhar = replaceAll(valorAempenhar, ',', '.');
				valorAempenhar = parseFloat(valorAempenhar);
			}
			if( valorAempenhar > valorObra ){
				alert( "O valor do empenho não pode ultrapassar o valor da obra." );
				document.getElementById('id_vlr_'+preid).value = '';
				return false;
			}
			
			if( valorAempenhar != '' ){
				var percent = valorAempenhar*100/valorObra;
				document.getElementById('id_'+preid).value = percent;				
				//document.getElementById('id_vlr_'+preid).value = mascaraglobal('###.###.###.###,##', valorAempenhar.toFixed(2));				
			} else {
				document.getElementById('id_'+preid).value = 0;
			}
		}
	} else { // veio da porcentagem
		if( document.getElementById('chk_'+preid).checked == true ){
			var percent = document.getElementById('id_'+preid).value;
			
			if( percent > 100 ){
				alert( "O valor do empenho não pode ultrapassar o valor da obra." );
				document.getElementById('id_vlr_'+preid).value = '';
				return false;
			}
			percent = replaceAll(percent, ',', '.');
			var total = ((parseFloat(valorObra) * parseFloat(percent))/100);
						
			//console.log(parseFloat(valorObra), parseFloat(percent), total);
			var total_mac = mascaraglobal('###.###.###.###,##',replaceAll(total.toFixed(2),'.',''));
			
			document.getElementById('id_vlr_'+preid).value = total_mac;
		}
	}
	calcularTotalObrasPac();
}

function calcularTotalObrasPac() {
	
	var totalAEmepnhar = 0;
	$('[name="chk[]"]:checked').each(function(){
		var preid = $(this).val();
		var valorObra = document.getElementById('vlrobra_'+preid).value; //valor da obra	
		var valorEmpenhado = document.getElementById('vlr_empenhado_'+preid).value; //valor do empenho
		var valorAempenhar = document.getElementById('id_vlr_'+preid).value; //valor do empenho
		
		if( valorAempenhar != '' ){
			valorAempenhar = replaceAll(valorAempenhar, '.', '');
			valorAempenhar = replaceAll(valorAempenhar, ',', '.');
			valorAempenhar = parseFloat(valorAempenhar);
		}
		//console.log( valorEmpenhado, valorAempenhar, valorObra )
		if( (parseFloat(valorEmpenhado) + parseFloat(valorAempenhar)).toFixed(2) > parseFloat(valorObra) ){
			if( ( (parseFloat(valorEmpenhado) + parseFloat(valorAempenhar)).toFixed(2) - parseFloat(valorObra) ).toFixed(2) == parseFloat(0.01) ){
				document.getElementById('id_vlr_'+preid).value = mascaraglobal('###.###.###.###,##', (parseFloat(valorAempenhar) - parseFloat(0.01)).toFixed(2));
			}else{				
				alert( "O valor do empenho não pode ultrapassar o valor da obra." );
				document.getElementById('id_'+preid).value = '0,00';
				document.getElementById('id_vlr_'+preid).value = '0,00'; 
			}
		} else {		
			totalAEmepnhar = parseFloat(totalAEmepnhar) + parseFloat(valorAempenhar);
		}
	});
	
	document.getElementById('id_total').value = mascaraglobal('###.###.###.###,##', parseFloat(totalAEmepnhar).toFixed(2));
}

function telaLogin( action ) {
	jQuery('input[type="text"], input[type="password"]').css('font-size', '14px');
	
	/* jQuery.ajax({
		type: "POST",
		url: window.location.href,
		async: false,
		success: function(msg){ */
			jQuery( "#div_login" ).show();
			//jQuery( "#mostra_dialog" ).html(msg);
			jQuery( '#div_login' ).dialog({
				resizable: false,
				width: 500,
				modal: true,
				show: { effect: 'drop', direction: "up" },
				buttons: {
					'Ok': function() {
						if( action == 'solicitar' ){
							solicitarEmpenho();
						}
						if( action == 'consultar' ){
							consultarEmpenhoWS();
						}
						if( action == 'cancelar' ){
							cancelarEmpenhoWS();
						}
						jQuery( this ).dialog( 'close' );
					},
					'Fechar': function() {
						jQuery( this ).dialog( 'close' );
					}
				}
			});
		/* }
	}); */
}

</script>
<? cabecalhoSolicitacaoEmpenho();
exibirHistoricoSigef($processo);
?>

<?php 


$arrDados = $db->pegaLinha("SELECT 
								   inu.inuid
							FROM par.processoobra p
							INNER JOIN territorios.municipio m ON m.muncod = p.muncod
							INNER JOIN par.instrumentounidade inu ON m.muncod = inu.muncod
							WHERE p.prostatus = 'A' and p.proid = {$_SESSION['par_var']['proid']}" );
							
$inuId = $arrDados['inuid'];

$today = date('Y-m-d');
if($inuId)
{
	$sqlRestricoes = 
		"SELECT 
			res.resdescricao,
			tpr.tprdescricao as tipo_restricao,
			CASE WHEN res.restemporestricao THEN
				to_char(resdatainicio, 'DD/MM/YYYY')
			ELSE
				'-'
			END
			as data_inicio,
			
			CASE WHEN res.restemporestricao THEN
				to_char(resdatafim, 'DD/MM/YYYY')
			ELSE
				'-'
			END
			as data_fim
		FROM
			par.restricaofnde res
		INNER JOIN
			par.tiporestricaofnde tpr ON res.tprid = tpr.tprid
		INNER JOIN
			par.instrumentounidaderestricaofnde iur ON res.resid = iur.resid
		WHERE
			res.resstatus = 'A'
		AND
			iur.inuid = {$inuId}
		AND
			CASE WHEN res.restemporestricao THEN
				CASE
					WHEN '{$today}' between resdatainicio and resdatafim THEN
						true
					ELSE
						false
					END
			ELSE
				true
			END
		";
	
	$arrRestricoes = $db->carregar($sqlRestricoes);
	
	$arrBloqueioObra = verificaBloqueioObras( $inuId );
	
	if( (count($arrRestricoes) && is_array($arrRestricoes)) || ($arrBloqueioObra['boBloqueia']) ) 
	{
	?>
<table id="listaRestricoes" align="center" border="0" style="border-color: #CCCCCC; border-right: 1px solid #CCCCCC;border-style: solid;border-width: 1px;   font-size: xx-small;text-decoration: none; width: 95%;" cellpadding="3" cellspacing="1">
			<thead>
				<tr>
					<td  colspan="4"> Restrições do município </td>
				</tr>

			</thead>
			<tbody>
			
				<?php
				if( count($arrRestricoes) && is_array($arrRestricoes))
				{
					foreach($arrRestricoes as $kRes => $vRes )
					{
						echo "
							<tr style=\"color:red;\">
								<td style=\"width:40%;\" > {$vRes['resdescricao']} </td>
							</tr>
							";	
					}
				}
					if( $arrBloqueioObra['boBloqueia'] ){
						
						echo "
							<tr style=\"color:red;\">
								<td style=\"width:40%;\" >";
						echo '<span style="color: red;">Existem pendências no Sistema '.$arrBloqueioObra['sistema'].'.</span><a href="#" onclick="mostraPendencia();";><span style="color: blue; cursor:pointer"> Clique aqui para verificar as pendências.</span></a>';
						echo " 	</td>
							 </tr>
								";	
						
					}
				?>
			</tbody>
			</table>
	<br>
<?php }
	}
?>


<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
<tr>
<td id="listapreobra">Carregando...</td>
</tr>
</table>
<form name="formulario" id="formulario">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td class="SubTituloDireita" width="40%">
			<b>Valor total de empenho:</b>
		</td>
		<td>
			<input type="hidden" name="proid" id="proid" value="<?=$_REQUEST['proid'] ?>">
			<input type="text" id="id_total" name="name_total" size="15" class="normal" style="text-align:right;" readonly="readonly" value="0,00">
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Plano Interno:</td>
		<td>
				<select name='planoInterno' id="planoInterno" class="CampoEstilo" style="width: auto">
					<option value="" selected="selected">Selecione</option>
					<option value="MEC00001" >MEC00001 - Proinfância</option>
					<option value="MEC00002" >MEC00002 - Quadra e Cobertura</option>
					<option value="RFF34I413DN">RFF34I413DN</option>
					<option value="RFF34I414DN">RFF34I414DN</option>
					<option value="RFF34B154DN">RFF34B154DN</option>
					<option value="RFF34B150DN">RFF34B150DN</option>
					<option value="RFF34B150DN">AFF34I609DN</option>
					<option value="RFF34B150DN">AFF34I604DN</option>
				</select>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Fonte de Recurso:</td>
		<td>
				<?php
				$sql = "SELECT e.frpid as codigo, e.frpcodigo||' - '||e.frptitulo || ' - ' || substring(frpfuncionalprogramatica, 9, 5) as descricao 
						FROM par.fonterecursopac e order by e.frptitulo";
				$db->monta_combo( "frpid", $sql, 'S', 'Selecione', '', '', '', '', 'S', 'frpid' );
				?>&nbsp;
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Centro de Gestão</td>
		<td>
				<select name='centroGestao' id="centroGestao" class="CampoEstilo" style="width: auto">
					<option value="69500000000" selected="selected" >69500000000</option>
					<option value="61700000000" >61700000000</option>
				</select>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >PTRES</td>
		<td>
				<select name='ptres' id="ptres" class="CampoEstilo" style="width: auto">
					<option value="061654" selected="selected" >061654</option>
					<option value="037825" >037825</option>
					<option value="037826" >037826</option>
					<option value="078983" >078983</option>
					<option value="078976" >078976</option>
					<option value="061655" >061655</option>
					<option value="087420" >087420</option>
					<option value="087417" >087417</option>
				</select>
		</td>
	</tr>
	
	<tr>
		<td colspan="2" class="SubTituloDireita" style="text-align: center;">
	<?php 
				$perfil = pegaPerfilGeral();
				if(	in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ||
					in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
					in_array(PAR_PERFIL_EMPENHADOR, $perfil)
				){
					$disabled = '';
				}else{
					$disabled = 'disabled="disabled"';
				}
			?>		
			<input type="button" name="solicitar" <?php echo $disabled; ?>value="Solicitar empenhos" onclick="telaLogin( 'solicitar' );" />
	<?php if( in_array( PAR_PERFIL_SUPER_USUARIO, pegaArrayPerfil($_SESSION['usucpf']) ) ){ ?>
			<input type=button id=visualizar name=visualizar  value=Visualizar XML onclick=visEmp(); />
	<?php } ?>
		</td>
	</tr>
</table>
</form>
<table  align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td class="subtitulodireita" width="20%" style="font-weight: bold" valign="top">Legenda: 
				<input type="hidden" name="processo" id="processo" value="<?php echo $_REQUEST['processo']?>" />
				<input type="hidden" name="wsempid" id="wsempid" value="" />
				<input type="hidden" name="proid" id="proid" value="<?php echo $_REQUEST['proid']?>" />
				<input type="hidden" name="ws_especie" id="ws_especie" value="" />
				<input type="hidden" name="tiposistema" id="tiposistema" value="PAC" />
				<input type="hidden" name="empdivergente" id="empdivergente" value="<?php echo $retorno[0]['processo']; ?>" />
				</td>
		<td valign="top" style="font-weight: bold" width="20%">&nbsp;<img src='../imagens/historico.png' title='Alterar'>&nbsp;-&nbsp;Histórico Situação de Empenho</td> 
		<td valign="top" style="font-weight: bold" width="20%">&nbsp;<img src='../imagens/refresh2.gif' title='Alterar'>&nbsp;-&nbsp;Atualizar Situação do Empenho</td>
		<td valign="top" style="font-weight: bold" width="20%">&nbsp;<img src='../imagens/money_ico.png' title='Excluir'>&nbsp;-&nbsp;Reduzir Valor Empenhado</td>
		<td valign="top" style="font-weight: bold" width="20%">&nbsp;<img src='../imagens/money_cancel.gif' title='Reduzir'>&nbsp;-&nbsp;Cancelar Nota de Empenho(NE)</td>				
	</tr>
</table>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td id="listaempenhoprocesso">Carregando...</td>
	</tr>
</table>
<?php
$largura = "250px";
$altura = "120px";
$id = "div_auth";
?>
<div id="debug"></div>
<style>
	#listaRestricoes thead tr td
	{
		background-color: #F7F7F7;
		font-weight: bold;
		font-size: 12px;
		font-family: arial;
	}
	.popup_alerta{
			width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;display:none}
	.popup_alerta2{
			width:90%;height:90%;position:absolute;z-index:0;top:50%;left:30%;margin-top:-250;margin-left:-250;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>
<!--  <div id="<?php echo $id ?>" class="popup_alerta <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<input type="hidden" id="proid" value="<?=$_REQUEST['proid'] ?>">
			<td width="30%" class="SubtituloDireita">Usuário:</td>
			<td><?php echo campo_texto("wsusuario","S","S","Usuário","22","","","","","","","id='wsusuario'") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Senha:</td>
			<td>
				<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="" size="23" id="wssenha" name="wssenha">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#D5D5D5" colspan="2">
				<input type="button" name="btn_enviar" onclick="solicitarEmpenho()" value="ok" />
				<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
			</td>
		</tr>
		</table>
	</div>
</div>-->
<div id="visXML" class="popup_alerta2 <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('visXML').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td width="10%" class="SubtituloDireita">Dados do Empenho:</td>
			<td id="visXMLDadosEmpenho"></td>
		</tr>
		</table>
	</div>
</div>
<?php
/*$largura = "250px";
$altura = "120px";
$id = "div_auth_consulta";
?>
<style>
	.popup_alerta_consulta{
			width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>
<div id="<?php echo $id ?>" class="popup_alerta_consulta <?php echo $classeCSS ?>" >
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
				<td><?php echo campo_texto("ws_usuario_cancela","S","S","Usuário","22","","","","","","","id='ws_usuario_cancela'") ?></td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Senha:</td>
				<td>
					<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="" size="23" id="ws_senha_cancela" name="ws_senha_cancela">
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
<? */ ?>
<div id="div_dialog" title="Histórico de Empenho" style="display: none; text-align: center;">
	<div style="padding:5px;text-align:justify;" id="mostra_dialog"></div>
</div>

<div id="div_login" title="Tela de Identificação" style="display: none; text-align: center;">
	<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td width="30%" class="SubtituloDireita">Usuário:</td>
			<td><?php echo campo_texto("wsusuario", "S", "S", "Usuário", "25","","","","","","", "id='wsusuario'") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Senha:</td>
			<td>
				<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" 
					onmouseover="MouseOver(this);" value="" size="26" id="wssenha" name="wssenha">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
	</table>
</div>

<div id="div_divergente" title="Empenho Divergentes" style="display: none; text-align: center;">
	<div style="padding:5px;text-align:justify;" id="mostra_divergente"></div>
</div>

<div id="dialog_pendencia" title="Pendências <?=$arrBloqueioObra['sistema']; ?>" style="display: none" >
	<div style="padding:5px;text-align:justify; font-size: 8pt; color: red;">
	<?
	if(is_array($arrBloqueioObra) && count($arrBloqueioObra))
	{
		foreach ($arrBloqueioObra['pendencia'] as $obra => $pendencia) {
			echo ($obra + 1).' - '.$pendencia;
		}	
	}
	?>
	</div>
</div>