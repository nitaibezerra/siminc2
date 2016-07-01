<?
include  APPRAIZ."par/classes/Habilita.class.inc";
include '_funcoes_empenho_par_obras.php';

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

if($_REQUEST['inuid']) {
	$_SESSION['par_var']['inuid'] = $_REQUEST['inuid'];
}else{
	alert('Entidade não encontrada!');
}


if($_REQUEST['proid']) {
	$proid = $_REQUEST['proid'];
	//$_SESSION['par_var']['proid']  = $_REQUEST['proid'];
	$_SESSION['par_var']['esfera'] = $db->pegaUm("SELECT CASE WHEN muncod IS NOT NULL THEN 'municipal' ELSE 'estadual' END FROM par.processoobraspar WHERE prostatus = 'A' and proid='".$_REQUEST['proid']."'"); 
}else{
	alert("Não foi possível encontrar os dados do Processo!");
	die("<script>window.close();</script>");
	
}
$processo = $_REQUEST['processo'];

$retornoEmpenhoSigef = verificaEmpenhoSigef( $processo, 'OBRAS' );

$retorno = verificaEmpenhoValorDivergente($processo, '', 'OBRA');
/*
if(!$_SESSION['par_var']['proid']) die("<script>
											alert('Processo não encontrado');
											window.close();
										</script>");

*/
// Monta tarja vermelha com aviso de pendência de obras
montarAvisoCabecalho(null, null, null, $_REQUEST['inuid']);

function carregarPtresPorPI( $dados ){
	global $db;
	
	$sql = "select distinct
								pliptres as codigo,
								pliptres as descricao
							from par.planointerno
							where plinumplanointerno = '{$dados['plicod']}'";
	$ptres = $db->carregar($sql);
	$html = '<div style="float: left;">
             <div style="margin-top: 10px;" class="comboBusca">
             <select data-placeholder="Escolha PTRES" id="ptres" name="ptres" class="chosen-select3" style="width:200px;" tabindex="2">
             ';
              if($ptres && is_array($ptres)){
              	foreach ($ptres as $ptre){
                	$html .= '<option value="' .$ptre['codigo']. '">'.$ptre['codigo'].'</option>';
              	}
              }
              $html .= '</select>
                        </div>
                    </div>'; 
           
	echo $html;
	die();
	//$db->monta_combo( "ptres", $sql, 'S', 'Selecione...', 'filtraFonteRecurso',"","","","N","","", $ptres);
}

?>
<html>
<head>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<style>

	.vizualisa_obra{
		cursor:pointer;
	}
	
 	.comboBusca a.chosen-single{
            -webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box;
            background-color: #FFFFFF !important;
            border: 1px solid #AAAAAA !important;
            border-radius: 5px !important;
            box-shadow: 0 0 3px #FFFFFF inset, 0 1px 1px rgba(0, 0, 0, 0.1) !important;
            color: #444444 !important;
            display: block !important;
            height: 30px !important;
            line-height: 29px !important;
            overflow: hidden !important;
            padding: 0 0 0 8px !important;
            position: relative !important;
            text-align: center !important;
            text-decoration: none !important;
            white-space: nowrap !important;

            /*background-color: red !important;*/
            /*border: 1px solid #AAAAAA !important;*/
            /*color: #444444 !important;*/
            /*text-align: center !important;*/
    }
</style>
</head>
<body>
<?php
monta_titulo( $titulo_modulo, '&nbsp;' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>
<script language="JavaScript" src="../estrutura/js/funcoes.js"></script>
<script src="../library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
<script src="../library/chosen-1.0.0/docsupport/prism.js" type="text/javascript"></script>
 <link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen" >
<script type="text/javascript" src="./js/par.js"></script>
<script type="text/javascript" src="../includes/JQuery/maskMoney.js" ></script>

<script>
$(document).ready(function() {
	carregaTelaEmpenhoDivergente();
	jQuery("input.maskMoney").maskMoney({showSymbol:true, symbol:"", decimal:",", thousands:".", allowNegative: true});
	
	carregarListaObraPar(<?=$proid; ?>);
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

	jQuery('.vizualisa_obra').live('click', function(){
		var preid = jQuery(this).attr('preid');
		var sbaid = jQuery(this).attr('sbaid');
		var sobano = jQuery(this).attr('sobano');
		window.open('par.php?modulo=principal/subacaoObras&acao=A&preid='+preid+'&sbaid='+sbaid+'&ano='+sobano, 
					'preobra', 
					"height=400,width=800,scrollbars=yes,top=0,left=0" );
	});
});

function carregarDadosObra(sbaid, preid, ano){
	window.open('par.php?modulo=principal/subacaoObras&acao=A&preid='+preid+'&sbaid='+sbaid+'&ano='+ano, 
			'preobra', 
			"height=400,width=800,scrollbars=yes,top=0,left=0" );
}

function reduzirEmpenho(empid, processo, especie) {
	window.open('par.php?modulo=principal/reduzirEmpenhoObrasPar&acao=A&empid='+empid+'&processo='+processo+'&especie='+especie, 
				'reduzirEmpenhoObrasPar', 
				"height=400,width=800,scrollbars=yes,top=0,left=0" );
}	

function visEmp(proid){
	var dadosobras = $('#formpreobras').serialize();
	var filtrosobras = $('#formulario').serialize();
	
	$.ajax({
		type: "POST",
		url: "par.php?modulo=principal/solicitacaoEmpenhoObrasPar&acao=A",
		data: "requisicao=solicitarEmpenho&tipo=visualiza&"+filtrosobras+'&'+dadosobras+'&proid='+proid,
		async: false,
		success: function(msg){
			document.getElementById('visXML').style.display='block';
			document.getElementById('visXMLDadosEmpenho').innerHTML=msg;
	}
	});
}

function consultarObra(sbaid, ano){
	url = "par.php?modulo=principal/subacao&acao=A&sbaid=" + sbaid + "&ano=" + ano + "&inuid=<?=$_REQUEST['inuid'] ?>";
	janela(url,800,600,"Subação")
}

function marcarChkObrasParEmp(obj) {
	var preid = obj.value;

	if(obj.checked) {
		document.getElementById('id_'+preid).className="norma percempenhol";
		document.getElementById('id_'+preid).readOnly=false;
		document.getElementById('id_vlr_'+preid).className="normal vrlaempenhar valorjaempenhado";
		document.getElementById('id_vlr_'+preid).readOnly=false;
		
		var valorObra = parseFloat(document.getElementById('vlrobra_'+preid).value); //valor da obra
		var valorEmpenhado = parseFloat(document.getElementById('vlr_empenhado_'+preid).value); //valor do empenho
		
		var percent = (((valorObra-valorEmpenhado)*100)/valorObra);
		document.getElementById('id_'+preid).value = percent;
		
		//document.getElementById('id_vlr_'+preid).focus();
		//obj.focus();
	} else {
		document.getElementById('id_'+preid).className="disabled percempenho";
		document.getElementById('id_'+preid).readOnly=true;
		document.getElementById('id_'+preid).value = '0,00';
		
		document.getElementById('id_vlr_'+preid).className="disabled valorjaempenhado";
		document.getElementById('id_vlr_'+preid).readOnly=true;
		document.getElementById('id_vlr_'+preid).value = '0,00';
	}
	
	var percPago = document.getElementById('porcentagem_'+preid).value;
	
	if( percPago > 0 && document.getElementById('chk_'+preid).checked == true ){
		var valorRestante = 100 - percPago;
		if( document.getElementById('id_'+preid).value == '' ){
			document.getElementById('id_'+preid).value = mascaraglobal('##,##',replaceAll(valorRestante,'.',''));
		}
		calculaEmpenhoObraPar(preid, 'p');
	} else {	
		calculaEmpenhoObraPar(preid, 'p');
	}
	
	//faço o AJAX para poder carregar o combo do Plano Interno
	$.ajax({
		type: "POST",
		url: "par.php?modulo=principal/solicitacaoEmpenhoObrasPar&acao=A",
		data: "requisicao=carregarPlanoInterno&preid="+preid,
		async: false,
		success: function(msg){
			document.getElementById('planointernoSPAN').innerHTML=msg;
		}
	});
	
	filtraPTRESObrasPar( document.getElementById('planointerno').value, preid );
}

function calculaEmpenhoObraPar(preid, tipo) {
	
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
				//obj.value = mascaraglobal('###.###.###.###,##',replaceAll(valor.toFixed(2),'.',''));
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
				//obj.value = mascaraglobal('###.###.###.###,##',replaceAll(valor.toFixed(2),'.',''));
				return false;
			}
			percent = replaceAll(percent, ',', '.');
			
			var total = valorObra*percent/100;
			var total_mac = mascaraglobal('###.###.###.###,##',replaceAll(total.toFixed(2),'.',''));
			
			document.getElementById('id_vlr_'+preid).value = total_mac;
		}
	}
	
	calcularTotalObrasPar();
}

function calcularTotalObrasPar() {
	
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
		
		if( (parseFloat(valorEmpenhado) + parseFloat(valorAempenhar)).toFixed(2) > parseFloat(valorObra) ){
			//document.getElementById('id_'+preid).value = (100 - parseFloat(document.getElementById('porcentagem_'+preid).value) );
			
			alert( "O valor do empenho não pode ultrapassar o valor da obra." ); 
			document.getElementById('id_'+preid).value = '0,00';
			document.getElementById('id_vlr_'+preid).value = '0,00';
			//calculaEmpenhoObraPar(preid, 'p');
			
		} else {		
			totalAEmepnhar = parseFloat(totalAEmepnhar) + parseFloat(valorAempenhar);
		}
	});
	
	document.getElementById('id_total').value = mascaraglobal('###.###.###.###,##', parseFloat(totalAEmepnhar).toFixed(2));
}

function solicitarEmpenhoObrasPar() {

	var form = document.getElementById('formpreobras');
	
	var usuario = document.getElementById('wsusuario').value;
	var senha = document.getElementById('wssenha').value;
	var proid = document.getElementById('proid').value;
	
	var marcado=false;
	for(var i=0;i<form.elements.length;i++) {
		if(form.elements[i].type) {
			if(form.elements[i].type == "checkbox" && form.elements[i].checked == true) {
				marcado = true;
			}
		}
	}
	if(!marcado) {
		alert('Nenhuma obra selecionada');
		return false;
	}
	
	if(!usuario) {
		alert('Favor informar o usuário!');
		return false;
	}
	
	if(!senha) {
		alert('Favor informar a senha!');
		return false;
	}
	var erroVrl = false;
	$('[name="chk[]"]:checked').each(function(){
		var preid = $(this).val();
		
		var valorAempenhar = document.getElementById('id_vlr_'+preid).value;
		var percent = document.getElementById('id_'+preid).value;
		
		if( valorAempenhar == '' || valorAempenhar == '0' || valorAempenhar == '0,00' || valorAempenhar == '0.00' || valorAempenhar == '00' ){
			alert('O Valor a Empenhar não pode ser vazio!');
			document.getElementById('id_vlr_'+preid).focus();
			erroVrl = true;
			return false;
		}
		
		if( percent == '' || percent == '0' || percent == '0,00' || percent == '0.00' || percent == '00' ){
			alert('O % Empenho não pode ser vazio!');
			document.getElementById('id_vlr_'+preid).focus();
			erroVrl = true;
			return false;
		}
	});
	
	if( erroVrl == false ){
		divCarregando();
		
		var dadosobras = $('#formpreobras').serialize();
		var filtrosobras = $('#formulario').serialize();
		
		$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/solicitacaoEmpenhoObrasPar&acao=A",
	   		data: "wsusuario=" + usuario + "&wssenha=" + senha + "&requisicao=executarEmpenho&proid="+proid+"&"+filtrosobras+'&'+dadosobras,
	   		async: false,
	   		success: function(msg){
	   			alert(msg);
	   		}
		});
		
		carregarListaObraPar(<?=$proid; ?>);
		carregarListaEmpenhoProcesso('<? echo $processo ?>');
	
		divCarregado();
	}	
}

function telaLogin( action ) {
	
	jQuery('input[type="text"], input[type="password"]').css('font-size', '14px');
	
	jQuery( "#div_login" ).show();
	jQuery( '#div_login' ).dialog({
		resizable: false,
		width: 500,
		modal: true,
		show: { effect: 'drop', direction: "up" },
		buttons: {
			'Ok': function() {
				if( action == 'solicitar' ){
					solicitarEmpenhoObrasPar();
				}
				if( action == 'consultar' ){
					consultarEmpenhoObrasParWS();
				}
				if( action == 'cancelar' ){
					cancelarEmpenhoObrasParWS();
				}
				jQuery( this ).dialog( 'close' );
			},
			'Fechar': function() {
				jQuery( this ).dialog( 'close' );
			}
		}
	});
}

</script>
<? cabecalhoSolicitacaoEmpenho($_REQUEST['proid']);
exibirHistoricoSigef($processo);

?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
<tr>
<td id="listapreobra">Carregando...</td>
</tr>
</table>

<form name="formulario" id="formulario">
	<input type="hidden" id="sbdidH" name="sbdidH" value="">
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td class="SubTituloDireita"><b>Valor total de empenho:</b></td>
		<td><input type="text" id="id_total" name="name_total" size="30" class="normal" style="text-align:right;" readonly="readonly" value="0,00"></td>
	</tr>	
	<?php
		$sql = "    SELECT 	DISTINCT
										plinumplanointerno as codigo,
										plinumplanointerno as descricao
								FROM
									par.planointerno
								-- WHERE pliano = '2013'
								";
		$PI = $db->carregar($sql);
		
		$arrayPI = array(
						array('codigo'=>'RFP01M411DP', 'descricao'=>'RFP01M411DP'),
						array('codigo'=>'RFP01M421DP', 'descricao'=>'RFP01M421DP'),
						array('codigo'=>'RFP01M571DP', 'descricao'=>'RFP01M571DP'),
						array('codigo'=>'RFP01M431DP', 'descricao'=>'RFP01M431DP'),
						array('codigo'=>'RFP01M951DP', 'descricao'=>'RFP01M951DP'),
						array('codigo'=>'RFP01M401DP', 'descricao'=>'RFP01M401DP'),
						array('codigo'=>'RFP01M381DP', 'descricao'=>'RFP01M381DP'),
						array('codigo'=>'RFP01M321DP', 'descricao'=>'RFP01M321DP'),
						array('codigo'=>'RFD01B992DP', 'descricao'=>'RFD01B992DP'),
						array('codigo'=>'RFD01M331DP', 'descricao'=>'RFD01M331DP'),
						array('codigo'=>'NFP01P412DP', 'descricao'=>'NFP01P412DP'),
						array('codigo'=>'GFF34B0101N', 'descricao'=>'GFF34B0101N'),
						array('codigo'=>'GFF34B416DN', 'descricao'=>'GFF34B416DN'),
						array('codigo'=>'GFF34B417DA', 'descricao'=>'GFF34B417DA'),
						array('codigo'=>'LFP01P410DN', 'descricao'=>'LFP01P410DN')
						);
		
		$arrayDadosPIFinal = array_merge($PI, $arrayPI);
	?>
	
	<tr>
		<td class="SubTituloDireita">Plano Interno:</td>
		<td>
			 <div style="float: left;">
                        <div style="margin-top: 10px;" class="comboBusca">
                            <select data-placeholder="Escolha um módulo um PI" id="planointerno" name="planointerno" class="chosen-select" style="width:200px;" tabindex="2" onchange="javascipt:pegaPI(this.value);">
                                <?php
                                if($arrayDadosPIFinal && is_array($PI)){
                                    foreach ($arrayDadosPIFinal as $PIs): ?>
                                        <option value="<?php echo $PIs['codigo']; ?>"><?php echo $PIs['codigo']; ?></option>
                                    <?php endforeach;
                                } ?>
                            </select>
                        </div>
                    </div>
              <input type="hidden" id="pi" name="pi" value="">
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><b>PTRES:</b></td>
		<td>
			<span id="ptresSPAN"> Selecione um PI.</span>
		</td>
	</tr>	
	<?php
		$sql = "SELECT distinct
						fonte as codigo, 
						fonte || ' - ' || dscfonte as descricao 
					FROM 
						financeiro.empenhopar ep
					order by fonte";
		$fontes = $db->carregar($sql);
	?>
	<tr>
		<td class="SubTituloDireita" width='30%'><b>Fonte de Recurso:</b></td>
		<td>
	 		<div style="float: left;">
                        <div style="margin-top: 10px;" class="comboBusca">
                            <select data-placeholder="Escolha uma Fonte de Recurso" id="fonte" name="fonte" class="chosen-select2" style="width:200px;" tabindex="2" onchange="javascipt:pegafonte(this.value);">
                                <?php
                                if($fontes && is_array($fontes)){
                                    foreach ($fontes as $fonte): ?>
                                        <option value="<?php echo $fonte['codigo']; ?>"><?php echo $fonte['codigo']; ?></option>
                                    <?php endforeach;
                                } ?>
                            </select>
                        </div>
                    </div>
		</td>
	</tr>
	<tr>
				<td class="SubTituloDireita"><b>Centro de Gestão:</b></td>
				<td>
					<select name='gestaosolicitacao' id="gestaosolicitacao" class="CampoEstilo" style="width: auto">
						<option value="61400000000" selected="selected">61400000000</option>
						<option value="61500000000" >61500000000</option>
						<option value="61700000000">61700000000</option>
						<option value="69500000000">69500000000</option>
						
					</select>
				</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><b>Natureza de Despesa:</b></td>
		<td>
				<select  name='naturezadespesasolicitacao' id="naturezadespesasolicitacao" class="CampoEstilo" style="width: auto">
					<option value="44404200" selected="selected">Capital Município - 44404200</option>
					<option value="33404100" >Custeio Município - 33404100</option>
					<option value="44304200">Capital Estados - 44304200</option>
					<option value="33304100">Custeio Estados - 33304100</option>
				</select>	
		</td>
	</tr>

	<tr>
		<td colspan="2" class="SubTituloDireita" style="text-align: center;">
		<?php 	
			$perfil = pegaPerfilGeral();
			//regras de acesso passada por Thiago em 24/05/2012
			if( in_array(PAR_PERFIL_PAGADOR, $perfil) || 
				in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
				in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || 
				in_array(PAR_PERFIL_COORDENADOR_GERAL ,$perfil)
			){	
		?>		
			<input type="button" name="solicitar" value="Solicitar empenho" onclick="telaLogin('solicitar');" />
		<?php }?>
		<?php if( in_array( PAR_PERFIL_SUPER_USUARIO, pegaArrayPerfil($_SESSION['usucpf']) ) ){ ?>
			<input type=button id=visualizar name=visualizar  value=Visualizar XML onclick=visEmp(<?=$proid; ?>); />
		<?php } ?>
		</td>
	</tr>
</table>
</form>
<table  align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td class="subtitulodireita" width="20%" style="font-weight: bold" valign="top">Legenda:
			<input type="hidden" name="processo" id="processo" value="<?php echo $_REQUEST['processo']?>" />
			<input type="hidden" name="proid" id="proid" value="<?=$_REQUEST['proid'] ?>">
			<input type="hidden" name="wsempid" id="wsempid" value="" />
			<input type="hidden" name="ws_especie" id="ws_especie" value="" />
			<input type="hidden" name="tiposistema" id="tiposistema" value="OBRA" />
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
<style>
	.popup_alerta{
			width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;display:none}
	.popup_alerta2{
			width:90%;height:90%;position:absolute;z-index:0;top:50%;left:30%;margin-top:-250;margin-left:-250;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>
<?php /*?>
<div id="<?php echo $id ?>" class="popup_alerta <?php echo $classeCSS ?>" >
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
				<input type="button" name="btn_enviar" onclick="solicitarEmpenhoObrasPar()" value="ok" />
				<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
			</td>
		</tr>
		</table>
	</div>
</div>
<?php */ ?>
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
<?php /*
$largura = "250px";
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
					<input type="button" name="btn_enviar" onclick="consultarEmpenhoObrasParWS()" value="ok" />
					<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
				</td>
			</tr>
		</table>
	</div>
</div>

<div id="debug"></div>
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
					<input type="button" name="btn_enviar" onclick="cancelarEmpenhoObrasParWS()" value="ok" />
					<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
				</td>
			</tr>
		</table>
	</div>
</div>
<?php */?>
<div id="div_dialog" title="Histórico de Empenho" style="display: none; text-align: center;">
	<div style="padding:5px;text-align:justify;" id="mostra_dialog"></div>
</div>

<div id="div_divergente" title="Empenho Divergentes" style="display: none; text-align: center;">
	<div style="padding:5px;text-align:justify;" id="mostra_divergente"></div>
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

	<script type="text/javascript">
                    var jq = jQuery.noConflict();
                    jq('.chosen-select').chosen({allow_single_deselect:true});
                    jq('.chosen-select2').chosen({allow_single_deselect:true});
                    jq('.chosen-select3').chosen({allow_single_deselect:true});
                    
                    function pegaPI(system)
            {
            			//jq('#pi').val(system);
            			$.ajax({
							type: "POST",
							url: "par.php?modulo=principal/solicitacaoEmpenhoObrasPar&acao=A",
							data: "requisicao=carregarPtresPorPI&plicod="+system,
							success: function(msg){
							
								document.getElementById("ptresSPAN").innerHTML = msg;
							}
						});
                
            }
            
                     function pegafonte(system)
            {
            			//jq('#pi').val(system);
                
            }
            
            
            
     </script>	