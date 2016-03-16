<?php

if ($_GET['ctrid']) {
    $_SESSION['ctrid'] = $_GET['ctrid'];
}

if ($_REQUEST ['req'] == 'popup') {
	include APPRAIZ . 'contratos/modulos/principal/popupIncluirOb.inc';
	exit ();
}
if ($_REQUEST ['req'] == 'verificaNumeroFatura') {
	if ($_POST ['ftcid']) :
		$sql = "SELECT
					ftcid
				FROM
					contratos.faturacontrato
				WHERE
					TRIM(ftcnumero) ILIKE TRIM('{$_POST['ftcnumero']}') AND
					ctrid = {$_SESSION['ctrid']} AND
					ftcid != '{$_POST['ftcid']}'";
		
		$ftcid = $db->pegaUm ( $sql );
	
	
	endif;
	die ( $ftcid );
}

// verifica sessão da pagina $_SESSION['ctrid']
verificaSessaoPagina ();

// verifica saldo
if ($_POST) {
	
	$somaValorOb = 0;
	if ($_POST ['obfvalor']) {
		foreach ( $_POST ['obfvalor'] as $v ) {
			if ($v) {
				$somaValorOb = $somaValorOb + str_replace ( array (
						".",
						"," 
				), array (
						"",
						"." 
				), $v );
			}
		}
	}
	
	$saldoContrato = pegaSaldoContrato ( $_SESSION ['ctrid'], $_POST ['ftcid'] );
	
	if ($_POST ['ftcvalor']) {
		$valorNota = str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $_POST ['ftcvalor'] );
		
		if ($valorNota > $saldoContrato) {
			echo '<script>
						alert("O valor (R$ ' . number_format ( $valorNota, 2, ',', '.' ) . ') não pode ser maior que o saldo do contrato (R$ ' . number_format ( $saldoContrato, 2, ',', '.' ) . ')!");
						history.back();
					  </script>';
			die ();
		}
	}
	
	if ($_POST ['obfvalor']) {
		
		// Conversão necessário devido à problema de comparação no FLOAT
		$somaValorOb = ( float ) number_format ( $somaValorOb, 2, '.', '' );
		$saldoContrato = ( float ) number_format ( $saldoContrato, 2, '.', '' );
		
		if ($somaValorOb > $saldoContrato) {
			echo '<script>
						alert("A soma dos valores das OBs (R$ ' . number_format ( $somaValorOb, 2, ',', '.' ) . ') não podem ser maior que o saldo do contrato (R$ ' . number_format ( $saldoContrato, 2, ',', '.' ) . ')!");
						history.back();
					  </script>';
			die ();
		}
	}
}

include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "contratos/classes/FaturaContrato.class.inc";
include_once APPRAIZ . "contratos/classes/EmpenhoContrato.class.inc";
include_once APPRAIZ . "contratos/classes/AnexoFatura.class.inc";
include_once APPRAIZ . "contratos/classes/OrdemBancariaFatura.class.inc";

if ($_REQUEST ['requisicao'] && $_REQUEST ['classe']) {
// 	dbg($_POST, d);
	$n = new $_REQUEST ['classe'] ();
	$n->$_REQUEST ['requisicao'] ();
	
	$db->commit ();
}

// Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
echo '<br>';
$db->cria_aba ( $abacod_tela, $url, '' );
monta_titulo ( $titulo_modulo, obrigatorio () . ' Indica campo obrigatório' );
montaCabecalhoContrato ( $_SESSION ['ctrid'] );
$sql = "SELECT
	ret.*
	FROM
	contratos.ctretencaocontrato ret
	natural join contratos.ctcontrato ct
	WHERE
ctrid = {$_SESSION['ctrid']}";

$retencao = $db->pegaLinha ( $sql );
if ($_GET ['ftcid']) {
	$nota = new FaturaContrato ( $_GET ['ftcid'] );
	$arrDados = $nota->getDados ();
	
	if ($arrDados) {
		extract ( $arrDados );
		$ftcdataemissao = $ftcdataemissao ? formata_data ( $ftcdataemissao ) : "";
        $ftcdatalimite = $ftcdatalimite ? formata_data ( $ftcdatalimite ) : "";
		$somaRetido = $retir + $retcsll + $retcofins + $retpasep + $retoutro;
		$ftctotalfatura = number_format ( $ftcvalor - $ftcglosa, 2, ',', '.' );
		
		// se a soma for maior que 0, calcular as porcentagem de cada tributo, senão busca porcentagem default
		
		if ($somaRetido > 0) {
			$retencao ['retir'] = ($retir / ($ftcvalor - $ftcglosa)) * 100;
			$retencao ['retcsll'] = ($retcsll / ($ftcvalor - $ftcglosa)) * 100;
			$retencao ['retcofins'] = ($retcofins / ($ftcvalor - $ftcglosa)) * 100;
			$retencao ['retpasep'] = ($retpasep / ($ftcvalor - $ftcglosa)) * 100;
			$retencao ['retoutro'] = ($retoutro / ($ftcvalor - $ftcglosa)) * 100;
			$retencao ["totalperc"] = $retencao ['retir'] + $retencao ['retcsll'] + $retencao ['retcofins'] + $retencao ['retpasep'] + $retencao ['retoutro'];
			$ftctotalapagar = number_format ( ($ftcvalor - $ftcglosa) - $somaRetido, 2, ',', '.' );
		} else {
			$retencao ["totalperc"] = $ftcaliqoutro + $retencao ["retir"] + $retencao ["retcsll"] + $retencao ["retcofins"] + $retencao ["retpasep"];
			$retoutro = ($ftcvalor - $ftcglosa) * ($ftcaliqoutro / 100);
			$retir = ($ftcvalor - $ftcglosa) * ($retencao ["retir"] / 100);
			$retcsll = ($ftcvalor - $ftcglosa) * ($retencao ["retcsll"] / 100);
			$retcofins = ($ftcvalor - $ftcglosa) * ($retencao ["retcofins"] / 100);
			$retpasep = ($ftcvalor - $ftcglosa) * ($retencao ["retpasep"] / 100);
			$retoutro = ($retoutro != null) ? $retoutro : 0.00;
			$retir = ($retir != null) ? $retir : 0.00;
			$retcsll = ($retcsll != null) ? $retcsll : 0.00;
			$retcofins = ($retcofins != null) ? $retcofins : 0.00;
			$retpasep = ($retpasep != null) ? $retpasep : 0.00;
			$somaRetido = $retoutro + $retir + $retcsll + $retcofins + $retpasep;
			$ftctotalapagar = number_format ( ($ftcvalor - $ftcglosa) - $somaRetido, 2, ',', '.' );
			$diferenca = (($ftcvalor - $ftcglosa) - $somaRetido) - $totalFaturas;
			$ftctotalfatura = number_format ( ($ftcvalor - $ftcglosa), 2, ',', '.' );
		}
		$ftcglosa = $ftcglosa ? $ftcglosa : 0;
		$ftcglosa = $ftcglosa ? number_format ( $ftcglosa, 2, ',', '.' ) : "";		
		$ftcvalor = $ftcvalor ? number_format ( $ftcvalor, 2, ',', '.' ) : "";
// 		$retoutro = $retoutro ? number_format ( $retoutro, 2, ',', '.' ) : "";
		
		$ob = new OrdemBancariaFatura ();
		$arrOrdem = $ob->getOrdemBancaria ( $ftcid );
		if ($arrOrdem)
			foreach ( $arrOrdem as $ordem ) {
				$totalFaturas += $ordem ["obfvalor"];
			}
	} else {
		// tratar dados qnd é a primeira vez que o usuario entra apos selecionar a aliquicota
	}
}

if ($_SESSION ['administrativo'] ['contratos'] ['notafiscal'] ['errosalvar']) {
	if ($_POST) {
		extract ( $_POST );
	}
	unset ( $_SESSION ['administrativo'] ['contratos'] ['notafiscal'] ['errosalvar'] );
}

// $habilitado = true;
// $somenteLeitura = 'S';

if ($docid) {
	require_once APPRAIZ . 'includes/workflow.php';
	$arrDoc = wf_pegarDocumento ( $docid );
	if ($arrDoc ['esdid'] == ESTADO_WK_FATURAMENTO_AGUARDANDO_PAGAMENTO) {
		$habilitado = false;
		$habilitadoOb = true;
	} elseif ($arrDoc ['esdid'] == ESTADO_WK_FATURAMENTO_PAGO) {
		$habilitado = false;
		$habilitadoOb = false;
	} else {
		$habilitado = true;
		$habilitadoOb = false;
	}
}

$acesso 		= verificaPermissaoTelaUsuario();
$desabilitado   = $acesso['desabilitado'];
$habilitado     = ($desabilitado == true ? false : true);
$somenteLeitura = $acesso['leitura'];

$perfis = arrayPerfil ();
$arPerfisBloqEdicao = array (
		PERFIL_EQUIPE_TECNICA_UNIDADE,
		PERFIL_CONSULTA_UNIDADE,
		PERFIL_CONSULTA_GERAL,
		PERFIL_TRIAGEM 
);

$arDif = array_diff ( $perfis, $arPerfisBloqEdicao );
// Bloqueia os campos caso o usuáro só tenha os perfis restritos
if (count ( $arDif ) == 0) {
// 	$habilitado = false;
// 	$somenteLeitura = 'N';
	$habilitadoOb = false;
}

// $arInt = array_intersect ( $perfis, $arPerfisBloqEdicao );

// // MÉTODO PARA A VERIFICAR SE O CONTRATO PERTENCE À UNIDADE DO USUARIO E SE O USUARIO PODERÁ EDITAR NESTA TELA
// if ($_SESSION ['ctrid'])
// 	$verifica = verificaResponsabilidade ( $_SESSION ['ctrid'], $perfis, array (
// 			PERFIL_GESTOR_FINANCEIRO_UNIDADE,
// 			PERFIL_TRIAGEM 
// 	) );
// 	// verifica se nao esta no bloqueado e se caso nao esteja bloq ele tem direito a editar
// if (count ( $arInt ) != 0 || (count ( $arInt ) == 0 && $verifica == false)) {
// 	$habilitado = false;
// 	$somenteLeitura = 'N';
// }
// $arrDoc['esdid'] == ESTADO_WK_FATURAMENTO_AGUARDANDO_PAGAMENTO

if ($ftcid) {
	$cnpj = $db->pegaUm ( "select distinct entnumcpfcnpj from entidade.entidade where entid =
					    (select entidcontratada from contratos.ctcontrato where ctrid = 
					     (select ctrid from contratos.faturacontrato where ftcid = {$ftcid}))" );
}

if ($_SESSION ['ctrid']) {
	$hspid = $db->pegaUm ( "SELECT 
						h.hspid 
					   FROM contratos.hospital h 
					   INNER JOIN 
						contratos.ctcontrato co ON co.hspid = h.hspid 
					   WHERE 
						co.ctrid='" . $_SESSION ['ctrid'] . "'" );
}

if ($_REQUEST ['req'] == 'popupimpressao' && $_REQUEST ['acao'] != 'I') {
	imprimirContrato ( $_SESSION ['ctrid'] );
	exit ();
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript"
	src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link
	href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"
	type="text/css" rel="stylesheet"></link>
<style>
.link {
	cursor: pointer
}
</style>
<script>

function verificaNumFatura(){
	var dataPost = {req		  : 'verificaNumeroFatura',
					ftcid 	  : $('#ftcid').val(),
					ftcnumero : $('#ftcnumero').val()};
	$.post( "?modulo=principal/addNotaContratos&acao=A", dataPost, function( data ) {
		data = trim(data);
		if ( data != '' ){
			alert('Este número de fatura (' + $('#ftcnumero').val() + ') já existe para o contrato.');
			$('#ftcnumero').val('');
			$('#ftcnumero').focus();
		}
	});
}

	function verificaTotalOb()
	{
		var obCalc = new Calculo();
		
		var total_valor = 0;
		var total 		= $("#ftcvalor").val();
		total 			= converteStringEmNumero(total);

		$.each( $("[name='obfvalor[]']"), function( chave, campo ) {
			var valor = $(campo).val();
			if(valor){
				total_valor = obCalc.operacao(total_valor, valor, '+');
//				total_valor += converteStringEmNumero(valor);
			}
		});

		total_valor = (total_valor ? total_valor : '0,00');
		
		$("#td_total_ob").html( mascaraglobal('[.###],##',total_valor) );		
				
		if( obCalc.comparar(total_valor, total, '=') == false ){
			$("#td_obs_ob").html("A soma das OBs é diferente do valor da nota fiscal.");
		}else{
			$("#td_obs_ob").html("");
		}
		recalcularOrdemBancaria();	
			
		/*	
		if(!verificaNumeroInteiro(total_valor)){
			$("#td_total_ob").html(mascaraglobal('[.###],##',total_valor));
		}else{
			$("#td_total_ob").html(mascaraglobal('[.###]',total_valor)+",00");
		}
		if(total_valor != total){
			$("#td_obs_ob").html("*Valor total das OBs não confere.");
		}else{
			$("#td_obs_ob").html("");
		}
		*/
	}
	function justificativaGlosa(){
		var obCalc = new Calculo();
		
//		if( ($('#ftcglosa').val() != '' && $('#ftcglosa').val() != '0,00') || obCalc. ){
		if( obCalc.comparar($('#ftcglosa').val(), '0,00', '>') ){
			$("#justificativaGlosa").show();
			$("#ftcjustificativaglosa").focus();
		}else{
			$("#justificativaGlosa").hide();
			$("#ftcjustificativaglosa").val('');
		}
	}
	function verificaNumeroInteiro(n) {
	   return ((typeof n==='number')&&(n%1===0));
	}
	
    function recalcularOrdemBancaria(){
           	var valor =  $("#td_total_ob").text();  
               	$('#span_ftcinformado').text(valor);
               	
           	var total_contrato =  $("#ftctotalapagar").val();
               	total_contrato =  formata_valor_banco(total_contrato);
               	
           	var total_valor =     $('#td_total_ob').text().trim();
               	total_valor =    formata_valor_banco(total_valor);
               	
           	var diferenca = 0;
           	
			if(parseFloat(total_valor) > parseFloat(total_contrato)){
               diferenca =   parseFloat(total_valor) - parseFloat(total_contrato);
               $("#diferencaFatura").val(diferenca);
               $("#span_diferenca_ftcvalor").css('color','red');
               $("#span_diferenca_ftcvalor").text('- '+formata_valor_moeda(diferenca));
            }else{
               diferenca =   parseFloat(total_contrato) - parseFloat(total_valor);  
               $("#diferencaFatura").val(diferenca);
               $("#span_diferenca_ftcvalor").css('color','blue'); 
               $("#span_diferenca_ftcvalor").text(formata_valor_moeda(diferenca)); 
            }
			//alert(diferenca);
			$('#tr_erro_calculo').show();
            
           	var vl_total_contrato =  $("#vl_total_contrato").text();
               	vl_total_contrato = formata_valor_banco(vl_total_contrato);
               	
			if(parseFloat(total_valor) > parseFloat(vl_total_contrato)){ 
				$('#tr_erro_calculo_vlcontrato').show();
               	$('#valor_total_nf').text( $("#td_total_ob").text());
               	$('#valor_total_contrato').text( $("#vl_total_contrato").text());
			}else{
              	$('#tr_erro_calculo_vlcontrato').hide();   
			}  
	}
        
        
	function salvarNotaFiscal()
	{
		var obCalc = new Calculo();

		//if($("#ftcglosa").val()!='' && $("#ftcjustificativaglosa").val()=='' ){
/*		
		if( obCalc.comparar($('#ftcglosa').val(), '0,00', '>') && $("#ftcjustificativaglosa").val()=='' ){
			alert('A justificativa da glosa deve ser preenchida.');
            return false;
		}
*/

        if ( $("#rettotalperc").length ){
            if( obCalc.comparar($('#rettotalperc').val(), '100,00', '>') ){
                alert('O valor de retenção não pode exceder o valor da fatura');
                return false;
            }
        }

		var erroInput = 0;
        $('input[type="text"][name^="obfnumero"]').each(function(i,val){
                   
			var numero 	= $(this).val();
            var valor 	= $('input[name^="obfvalor"]').eq(i).val();
			//var empenho = $('select[name^="epsid"]').eq(i).val();

			if(numero != '' || valor != ''){
				if(numero == '' || valor == '' ){
	            	erroInput = 1;
	                if(valor == ''){
	                	$('input[name^="obfvalor"]').eq(i).focus(); 
					}else{
	                	$(this).focus();
					}
				}
			}

			
		});



		if(erroInput != 0){
        	alert('Número ou valor da ordem bancária não preenchido.');
            return false;
		}
		               
        var vl_ob = $('#td_total_ob').text();

        if(vl_ob == ''){
        	vl_ob = '0,00';
		}
               
        var vl_nota = $('#ftcvalor').val();
            vl_ob 	= formata_valor_banco(vl_ob);
            vl_nota = formata_valor_banco(vl_nota);
               
		if(parseFloat(vl_ob) > parseFloat(vl_nota)){
        	alert('A soma dos valores das ordens bancárias não pode ser superior ao valor da nota fiscal.');
            return false;
		}

		var erro = 0;
		<?php  if($arrDoc['esdid'] == ESTADO_WK_FATURAMENTO_AGUARDANDO_PAGAMENTO):?>
		<?php //if( $ftcid ):?>
			var total_valor = 0;
			var total 	= $("#ftcvalor").val();
//			total = converteStringEmNumero(total);
			
			$.each( $("[name='obfvalor[]']"), function( chave, campo ) {
				var valor = $(campo).val();
				if(valor){
					total_valor = obCalc.operacao(total_valor, valor, '+');
//					total_valor += converteStringEmNumero(valor);
				}
			});
			
//			if(total_valor != total){
/*			if( obCalc.comparar(total_valor, total, '=') == false ){
				alert('O valor do somatório das ordens bancárias (' + mascaraglobal('[.###],##', new String(total_valor)) + ') deve ser igual ao valor da fatura (' + mascaraglobal('[.###],##', new String(total)) + ').');
				erro = 1;
				return false;
			} 
*/    
		<?php else:?>

		 	var nomeform 		= 'formulario';
			var submeterForm 	= true;
			var campos 			= new Array();
			var tiposDeCampos 	= new Array();
			
			//campos[0] 			= "epsid";
			campos[0]			= "ftcnumero"; 
			campos[1]			= "ftcdataemissao";		
			campos[2]			= "ftcvalor";
							 
			//tiposDeCampos[0] 	= "texto";
			tiposDeCampos[0] 	= "texto";
			tiposDeCampos[1] 	= "data";
			tiposDeCampos[2] 	= "valor";

			
			if( obCalc.comparar($('#ftcglosa').val(), '0,00', '>') && $("#ftcjustificativaglosa").val()=='' ){
				campos[3]			= "ftcjustificativaglosa";
				tiposDeCampos[3] 	= "textarea";
			}

        if ( $("#ftcaliqoutro").length ){
			if( obCalc.comparar($('#ftcaliqoutro').val(), '0,00', '>') ){
				campos[4]			= "ftcjustificativaretencao";
				tiposDeCampos[4] 	= "textarea";
			}
        }

			$valida = validaForm(nomeform, campos, tiposDeCampos, submeterForm );

			return false;
			
/*			
			if(!$("#numempenho").val())
			{
				alert('Favor informar o Número de Empenho.');
				erro = 1;
				$("#numempenho").focus();
				return false;
			}
			if(!$("#ftcnumero").val())
			{
				alert('Favor informar o Número de Fatura.');
				erro = 1;
				$("#ftcnumero").focus();
				return false;
			}
			if(!$("[name='ftcdataemissao']").val())
			{
				alert('Favor informar a Data de Emissão.');
				erro = 1;
				$("[name='ftcdataemissao']").focus();
				return false;
			}
			if(!$("#ftcvalor").val())
			{
				alert('Favor informar o Valor.');
				erro = 1;
				$("#ftcvalor").focus();
				return false;
			}
*/			
		<?php endif;?>
		
		if(erro == 0)
		{
			$("#formulario").submit();
		}
		
	}

	function converteStringEmNumero(total)
	{
		total = total.replace(".", "");
		total = total.replace(".", "");
		total = total.replace(".", "");
		total = total.replace(".", "");
		total = total.replace(".", "");
		total = total.replace(".", "");
		total = total.replace(",", ".");
		total = total*1;
		return total;
	}

	function addNovoAnexo(obj)
	{
		//tbody_anexo
		//var tr = $(obj).parent().parent();
		//var html = "<tr><td class='SubTituloDireita' ></td><td><input type=\"text\" class=\"normal\"  onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\"  maxlength=\"255\" size=\"41\" name=\"arqdescricao[]\" style=\"text-align:left;\"> <input type='file' name='arquivo[]' /> <img src='../imagens/excluir.gif' onclick=\"$(this).parent().parent().remove()\" class='link' /></td></tr>";
		//$(tr).closest('tr').after(html);
		
		var tr_clone = $('#tr_anexo').clone();
		$('td:eq(3)', tr_clone).html('');
		$('td:eq(1) input', tr_clone).val('');
		$('td:eq(2) input', tr_clone).val('');

		tr_clone.insertAfter($('#tb_anexo > tbody > tr:last'));
	}

	function downloadAnexo(arqid)
	{
		window.location = 'contratos.php?modulo=principal/addNotaContratos&acao=A&requisicao=downloadAnexo&classe=AnexoFatura&arqid='+arqid;
	}
	
	function excluirAnexo(anfid,arqid,ftcid)
	{
		if(confirm("Deseja realmente excluir este anexo?")){
			window.location = 'contratos.php?modulo=principal/addNotaContratos&acao=A&requisicao=excluirAnexo&classe=AnexoFatura&arqid='+arqid+'&anfid='+anfid+'&ftcid='+ftcid;
		}
	}
	function addOrdemBancaria(obj)
	{
		var tr = $(obj).parent().parent();
		var img_obr = " <img border=\"0\" title=\"Indica campo obrigatório.\" src=\"../imagens/obrig.gif\">";
		var html = "<tr><td><img src='../imagens/excluir.gif' title='Excluir ordem bancária' onclick=\"$(this).parent().parent().remove()\" class='link' /></td><td><input type=\"text\" class=\"normal\"  onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\"  maxlength=\"12\" size=\"12\" name=\"obfnumero[]\" style=\"text-align:left;\">"+img_obr+"</td><td><input type=\"text\" class=\"normal\"  onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal('[.###],##',this.value);\" onchange=\"verificaTotalOb()\" maxlength=\"40\" size=\"12\" name=\"obfvalor[]\" style=\"text-align:left;\">"+img_obr+"</td></tr>";
		$(tr).closest('tr').before(html);
	}

	//incluir_ob_select
	function popupIncluirOb(cnpj, hspid/*, epsid*/)
	{
		var ftcid = <?php echo $ftcid ? $ftcid : '0'; ?>;
		var popUp = window.open('/contratos/contratos.php?modulo=principal/addNotaContratos&acao=A&req=popup&cnpj='+cnpj+'&ftcid='+ftcid+'&hspid='+hspid, 'popupOb', 'height=500,width=700,scrollbars=yes,top=50,left=200');
//		var popUp = window.open('/contratos/contratos.php?modulo=principal/addNotaContratos&acao=A&req=popup&cnpj='+cnpj+'&ftcid='+ftcid+'&hspid='+hspid+'&empenho='+epsid, 'popupOb', 'height=500,width=700,scrollbars=yes,top=50,left=200');
		popUp.focus();
	}

	
$(document).ready(function(){
    $('#span_ftcvalor').text($('#ftctotalapagar').val());
    var contOrdem = $('#tb_numob > tbody > tr > td > img.link').length;
    if(contOrdem > 1){
      $('#tr_erro_calculo').show(); 
      recalcularOrdemBancaria();
    }
    
    $('.obsiafi').click(function(){
       recalcularOrdemBancaria();  
    });
    $('#ftcglosa').keyup( function(){
		calcularetencao();
    	calcularTotalFatura();
		calculaTotalAPagar()
     });
    $('#ftcvalor').keyup( function(){
		calcularetencao();
    	calcularTotalFatura();
		calculaTotalAPagar();
     });
    $('#ftcaliqoutro').keyup( function(){
     	var glosa = formata_valor_banco($('#ftcglosa').val());
 		var valor = formata_valor_banco($('#ftcvalor').val());
 		var retoutroperc = formata_valor_banco($('#ftcaliqoutro').val());
 		var outroretido = (valor - glosa)*(retoutroperc/100);
		$('#retoutro').val(formata_valor_moeda(outroretido));       
		calculaPorcentagemTotal();
		calculaRetencaoTotal();
		calculaTotalAPagar();

     });
    $('#retirperc').keyup( function(){
     	var glosa = formata_valor_banco($('#ftcglosa').val());
 		var valor = formata_valor_banco($('#ftcvalor').val());
 		var retirperc = formata_valor_banco($('#retirperc').val());
 		var irretido = (valor - glosa)*(retirperc/100);
		$('#retir').val(formata_valor_moeda(irretido));
 		
		calculaPorcentagemTotal();
		calculaRetencaoTotal();
		calculaTotalAPagar();

     });
    $('#retcsllperc').keyup( function(){
     	var glosa = formata_valor_banco($('#ftcglosa').val());
 		var valor = formata_valor_banco($('#ftcvalor').val());
 		var retcsllperc = formata_valor_banco($('#retcsllperc').val());
 		var csllretido = (valor - glosa)*(retcsllperc/100);
		$('#retcsll').val(formata_valor_moeda(csllretido));
		calculaPorcentagemTotal();
		calculaRetencaoTotal();
		calculaTotalAPagar();

     });
    $('#retcofinsperc').keyup( function(){
     	var glosa = formata_valor_banco($('#ftcglosa').val());
 		var valor = formata_valor_banco($('#ftcvalor').val());
 		var retcofinsperc = formata_valor_banco($('#retcofinsperc').val());
 		var cofinsretido = (valor - glosa)*(retcofinsperc/100);
		$('#retcofins').val(formata_valor_moeda(cofinsretido));
		calculaPorcentagemTotal();
		calculaRetencaoTotal();
		calculaTotalAPagar();

     });
    $('#retpasepperc').keyup( function(){
     	var glosa = formata_valor_banco($('#ftcglosa').val());
 		var valor = formata_valor_banco($('#ftcvalor').val());
 		var retpasepperc = formata_valor_banco($('#retpasepperc').val());
 		var pasepretido = (valor - glosa)*(retpasepperc/100);
		$('#retpasep').val(formata_valor_moeda(pasepretido));
		calculaPorcentagemTotal();
		calculaRetencaoTotal();
		calculaTotalAPagar();

     });
    $('#ftcaliqoutro').blur( function(){
		if($('#ftcaliqoutro').val()!=''){
			$("#justificativaRetencao").show();
			$("#ftcjustificativaretencao").focus();
		}else{
			$("#justificativaRetencao").hide();
			$("#ftcjustificativaretencao").val('');
		}
    });
    $('#retoutro').keyup( function(){
 		var outros = formata_valor_banco($('#retoutro').val());
 		calcularPorcentagemTributo(outros,'ftcaliqoutro');
		calculaPorcentagemTotal();
		calculaRetencaoTotal();
		calculaTotalAPagar();
     });
    $('#retir').keyup( function(){
 		var valor = formata_valor_banco($('#retir').val());
 		calcularPorcentagemTributo(valor,'retirperc');
		calculaPorcentagemTotal();
		calculaRetencaoTotal();
		calculaTotalAPagar();
     });
    $('#retcsll').keyup( function(){
 		var valor = formata_valor_banco($('#retcsll').val());
 		calcularPorcentagemTributo(valor,'retcsllperc');
		calculaPorcentagemTotal();
		calculaRetencaoTotal();
		calculaTotalAPagar();
     });
    $('#retcofins').keyup( function(){
 		var valor = formata_valor_banco($('#retcofins').val());
 		calcularPorcentagemTributo(valor,'retcofinsperc');
		calculaPorcentagemTotal();
		calculaRetencaoTotal();
		calculaTotalAPagar();
     });
    $('#retpasep').keyup( function(){
 		var valor = formata_valor_banco($('#retpasep').val());
 		calcularPorcentagemTributo(valor,'retpasepperc');
		calculaPorcentagemTotal();
		calculaRetencaoTotal();
		calculaTotalAPagar();
     });
    $('#retoutro').blur( function(){
		if($('#retoutro').val()!='' && $('#retoutro').val()!='0,00'){
			$("#justificativaRetencao").show();
			$("#ftcjustificativaretencao").focus();
		}else{
			$("#justificativaRetencao").hide();
			$("#ftcjustificativaretencao").val('');
		}
    });
 });
 function calcularPorcentagemTributo(valorCampo,campo)
 {
	 //alert(campo+": "+valorCampo);
  	var glosa = formata_valor_banco($('#ftcglosa').val());
	var valor = formata_valor_banco($('#ftcvalor').val());

	var porcentagem=(valorCampo/(valor-glosa)*100);
 	$("#"+campo).val(formata_valor_moeda(porcentagem));
 }
 function calculaTotalAPagar(){
 	var glosa = formata_valor_banco($('#ftcglosa').val());
	var valor = formata_valor_banco($('#ftcvalor').val());
	var retencao = formata_valor_banco($('#rettotal').val());
	if(retencao){
		var total = (valor - glosa)-retencao;
	}else{
		var total = (valor - glosa);
	}
	$('#ftctotalapagar').val(formata_valor_moeda(total));
 }
 function calculaRetencaoTotal(){
 	var ir = formata_valor_banco($('#retir').val());
	var csll = formata_valor_banco($('#retcsll').val());
	var cofins = formata_valor_banco($('#retcofins').val());
	var pasep = formata_valor_banco($('#retpasep').val());
	var outro = formata_valor_banco($('#retoutro').val());
	var totalretencao = ir + csll + cofins + pasep + outro;
	$('#rettotal').val(formata_valor_moeda(totalretencao));
 }
 function calculaPorcentagemTotal(){
		var percent = formata_valor_banco($('#ftcaliqoutro').val());
		var percir = formata_valor_banco($('#retirperc').val());
		var perccsll = formata_valor_banco($('#retcsllperc').val());
		var perccofins = formata_valor_banco($('#retcofinsperc').val());
		var percpasep = formata_valor_banco($('#retpasepperc').val());
		var percentualAplicar = percent+percir+perccsll+perccofins+percpasep;
		//alert(percir+' '+perccsll);
		$('#rettotalperc').val(formata_valor_moeda(percentualAplicar));
 }
 function calcularetencao(){
  	var bruto = formata_valor_banco($('#ftcvalor').val()) - formata_valor_banco($('#ftcglosa').val());
	var outros = formata_valor_banco($('#retoutro').val());
	var percoutro=(outros/(bruto*100));
//	$('#ftcaliqoutro').val(formata_valor_moeda(percoutro));

	var percir = formata_valor_banco($('#retirperc').val());
	var perccsll = formata_valor_banco($('#retcsllperc').val());
	var perccofins = formata_valor_banco($('#retcofinsperc').val());
	var percpasep = formata_valor_banco($('#retpasepperc').val());
 	var percentualAplicar = percoutro+percir+perccsll+perccofins+percpasep;
 	$('#retir').val(formata_valor_moeda(bruto*(percir/100)));
 	$('#retcsll').val(formata_valor_moeda(bruto*(perccsll/100)));
 	$('#retcofins').val(formata_valor_moeda(bruto*(perccofins/100)));
 	$('#retpasep').val(formata_valor_moeda(bruto*(percpasep/100)));
 	var retoutrosper = formata_valor_banco($('#ftcaliqoutro').val());
 	//alert(retoutrosper);
 	$('#retoutro').val(formata_valor_moeda(bruto*(retoutrosper/100)));
	var ir = formata_valor_banco($('#retir').val());
	var csll = formata_valor_banco($('#retcsll').val());
	var cofins = formata_valor_banco($('#retcofins').val());
	var pasep = formata_valor_banco($('#retpasep').val());
	var outraretencao = formata_valor_banco($('#retoutro').val());
	var retencao = ir + csll + cofins + pasep + outraretencao;
	$('#rettotal').val(formata_valor_moeda(retencao));
  	
 }
 function calcularTotalFatura(){
		var glosa = formata_valor_banco($('#ftcglosa').val());
		var valor = formata_valor_banco($('#ftcvalor').val());
		var retencao = formata_valor_banco($('#rettotal').val());
		var total = (valor - glosa)-retencao;
		var fatura = (valor - glosa);
		$('#ftctotalapagar').val(formata_valor_moeda(total));
		$('#totalfatura').val(formata_valor_moeda(fatura));
 }
 function popupRetencao(id)
 {
 	var popUp = window.open('/contratos/contratos.php?modulo=principal/popupRetencao&acao=A&id='+id, 'popupImpressao', 'height=350,width=400,scrollbars=yes,top=50,left=200');
 	popUp.focus();
 }
 
 function PrintElem(elem)
 {
     Popup($(elem).html());
 }
 function Popup(data) 
 {
     var mywindow = window.open('', 'Despacho - impressão', 'height=800,width=600');
     mywindow.document.write('<html><head><title>my div</title>');
     mywindow.document.write('<style>table.bordasimples { border-collapse: collapse; } table.bordasimples tr td { border: 1px solid #ccc;}#myDiv, p { padding-left: 40px;}</style>');
     mywindow.document.write('</head><body >');
     mywindow.document.write(data);
     mywindow.document.write('</body></html>');

     mywindow.print();
     mywindow.close();

     return true;
 }
</script>
<form name=formulario id=formulario method=post	enctype="multipart/form-data">
	<input type="hidden" name="requisicao" id="requisicao"
		value="salvarNotaFiscal"> <input type="hidden" name="classe"
		id="classe" value="FaturaContrato"> <input type="hidden" name="ftcid"
		id="ftcid" value="<?php echo $ftcid ?>"> <input type="hidden"
		name="docid" id="docid" value="<?php echo $docid ?>"> <input
		type="hidden" name="diferencaFatura" id="diferencaFatura"
		value="<?php echo $diferenca ?>">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"
		align="center">
		<!-- 	
	    <tr>
	        <td class="SubTituloDireita" width="25%" >
	        	Nota de Empenho:
	        </td>
	        <td>
	        	<?php
										// $empenho = new EmpenhoContrato();
										// $arrDados = $db->carregar($empenho->getSqlComboEmpenhos($_SESSION['ctrid']));
										// if(count($arrDados) > 1){
										// $arrAtributos = false;
										// $arrAtributos['name'] = "epsid";
										// $arrAtributos['id'] = "epsid";
										// $arrAtributos['sql'] = $arrDados;
										// $arrAtributos['obrigatorio'] = true;
										// $arrAtributos['habilitado'] = $habilitado;
										// $arrAtributos['value'] = $epcid;
										// $db->monta_combo($arrAtributos);
										// }else{
										// if($arrDados[0]) {
										// $epsid = $arrDados[0]['codigo'];
										// echo $arrDados[0]['descricao'];
										// echo "<input type='hidden' name='epsid' value='".$arrDados[0]['codigo']."' id='epsid' />";
										// } else {
										// echo "Não existe nota de empenho cadastrada";
										// echo "<input type='hidden' name='epsid' value='' id='epsid' title='Nota de Empenho' />";
										// }
										// }
										?>
	        </td>
	        <td rowspan="6" align="right" valign="top" width="80">
	       		<?php
										// if($docid){
										// // Barra de estado WORKFLOW
										// wf_desenhaBarraNavegacao($docid, array('ftcid'=>$ftcid));
										// }
										?>
	        </td>      
	    </tr>
-->
		<tr>
			<td class="SubTituloDireita" width="25%">Número da Fatura:</td>
			<td>
	        		<?php
//                     $habilitado = true;

											$arrAtributos = false;
											$arrAtributos ['name'] = "ftcnumero";
											$arrAtributos ['id'] = "ftcnumero";
											$arrAtributos ['title'] = "Número da Fatura";
											$arrAtributos ['obrigatorio'] = true;
											$arrAtributos ['habilitado'] = $habilitado;
											$arrAtributos ['size'] = 15;
											$arrAtributos ['maxsize'] = 200;
											$arrAtributos ['align'] = "left";
											$arrAtributos ['value'] = $ftcnumero;
											$arrAtributos ['onchange'] = "verificaNumFatura();";
											$arrAtributos ['mascara'] = "[#]";
											echo campo_texto ( $arrAtributos )?>
	        </td>
			<td rowspan="6" align="right" valign="top" width="80">
	       		<?php
										if ($docid) {
											// Barra de estado WORKFLOW
											wf_desenhaBarraNavegacao ( $docid, array (
                                                    'ctrid' => $_SESSION['ctrid'],
                                                    'nf' => $ftcnumero,
													'ftcid' => $ftcid,
													'docid' => $docid,
													'diferenca' => $diferenca 
											) );
										}
										?>
	        </td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Descrição:</td>
			<td>
	        	<?php
										$arrAtributos = false;
										$arrAtributos ['name'] = "ftcdescricao";
										$arrAtributos ['id'] = "ftcdescricao";
										$arrAtributos ['title'] = "Descrição";
										$arrAtributos ['obrigatorio'] = false;
										$arrAtributos ['habilitado'] = $habilitado;
										$arrAtributos ['cols'] = 56;
										$arrAtributos ['rows'] = 5;
										$arrAtributos ['maxsize'] = 500;
										$arrAtributos ['value'] = $ftcdescricao;
										echo campo_textarea ( $arrAtributos )?>
	        </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de Emissão:</td>
			<td>
	        	<?php echo campo_data2("ftcdataemissao","S",($habilitado ? "S" : "N"),"Data de Emissão","N")?>
	        </td>
		</tr>
        <tr>
            <td class="SubTituloDireita">Data limite para pagamento:</td>
            <td>
                <?php echo campo_data2("ftcdatalimite","N",($habilitado ? "S" : "N"),"Data de Emissão","N")?>
            </td>
        </tr>
		<tr>
			<td class="SubTituloDireita">Valor:</td>
			<td>
	        	<?php
										$arrAtributos = false;
										$arrAtributos ['name'] = "ftcvalor";
										$arrAtributos ['id'] = "ftcvalor";
										$arrAtributos ['title'] = "Valor";
										$arrAtributos ['obrigatorio'] = true;
										$arrAtributos ['habilitado'] = $habilitado;
										$arrAtributos ['size'] = 15;
										$arrAtributos ['maxsize'] = 20;
										$arrAtributos ['mascara'] = "[.###],##";
										$arrAtributos ['align'] = "right";
										$arrAtributos ['value'] = $ftcvalor;
										$arrAtributos ['evtkeyup'] = 'verificaTotalOb()';
										echo campo_texto ( $arrAtributos )?>
	        </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Glosa:</td>
			<td>
	        	<?php
										$ftcglosa = ($ftcglosa ? $ftcglosa : '');
										$arrAtributos = false;
										$arrAtributos ['name'] = "ftcglosa";
										$arrAtributos ['id'] = "ftcglosa";
										$arrAtributos ['title'] = "Glosa";
										$arrAtributos ['obrigatorio'] = false;
										$arrAtributos ['habilitado'] = $habilitado;
										$arrAtributos ['size'] = 15;
										$arrAtributos ['maxsize'] = 20;
										$arrAtributos ['mascara'] = "[.###],##";
										$arrAtributos ['align'] = "right";
										$arrAtributos ['value'] = $ftcglosa;
										$arrAtributos ['evtblur'] = 'justificativaGlosa();';
										echo campo_texto ( $arrAtributos )?>
	        </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Valor Total da Fatura:</td>
			<td><input type="text" id="totalfatura" name="totalfatura"
				style="text-align: right;" size="16" readonly
				value="<?php echo $ftctotalfatura;?>" /></td>
		</tr>
		<tr id=justificativaGlosa <?php if($ftcglosa>0): else:?>
			style="display: none;" <?php endif;?>>
			<td class="SubTituloDireita" valign="top">Justificativa da Glosa:</td>
			<td>
	        	<?php
										$arrAtributos = false;
										$arrAtributos ['name'] = "ftcjustificativaglosa";
										$arrAtributos ['id'] = "ftcjustificativaglosa";
										$arrAtributos ['title'] = "Justificativa da Glosa";
										$arrAtributos ['obrigatorio'] = true;
										$arrAtributos ['habilitado'] = $habilitado;
										$arrAtributos ['cols'] = 56;
										$arrAtributos ['rows'] = 5;
										$arrAtributos ['maxsize'] = 500;
										$arrAtributos ['value'] = $ftcjustificativaglosa;
										echo campo_textarea ( $arrAtributos );
										
										?>
	        </td>
		</tr>
<?php

if ($retencao ["retid"]) {
// 	if (in_array ( PERFIL_TRIAGEM, $perfis ))
// 		$habilitado = true;
	?>
	    <tr>
			<td class="SubTituloDireita" rowspan=2>Retenção:</td>
			<td><a onclick="popupRetencao(<?php echo $retencao["retid"]?>)"><?php echo $retencao["retcod"]." - ".$retencao["retres"];?></a></td>
		</tr>
		<tr>
			<td>
				<table class="tabela" bgcolor="#f5f5f5" cellspacing="1"
					cellpadding="0" style="width: 720px">
					<thead>
						<tr>
							<th>IR</th>
							<th>CSLL</th>
							<th>COFINS</th>
							<th>PIS/PASEP</th>
							<th>Outros</th>
							<th>Total</th>
						</tr>
					</thead>
					<tbody>
						<tr>
							<td>
					        	<?php
	$arrAtributos = false;
	$arrAtributos ['name'] = "retirperc";
	$arrAtributos ['id'] = "retirperc";
	$arrAtributos ['title'] = "ir Percentual";
	$arrAtributos ['obrigatorio'] = false;
	$arrAtributos ['habilitado'] = $habilitado;
	$arrAtributos ['size'] = 6;
	$arrAtributos ['maxsize'] = 6;
	$arrAtributos ['mascara'] = "[.###],##";
	$arrAtributos ['align'] = "right";
	$arrAtributos ['value'] = number_format ( $retencao ["retir"], 2, ",", "." );
	
	echo campo_texto ( $arrAtributos )?>%
					        </td>
							<td>
					        	<?php
	$arrAtributos = false;
	$arrAtributos ['name'] = "retcsllperc";
	$arrAtributos ['id'] = "retcsllperc";
	$arrAtributos ['title'] = "csll Percentual";
	$arrAtributos ['obrigatorio'] = false;
	$arrAtributos ['habilitado'] = $habilitado;
	$arrAtributos ['size'] = 6;
	$arrAtributos ['maxsize'] = 6;
	$arrAtributos ['mascara'] = "[.###],##";
	$arrAtributos ['align'] = "right";
	$arrAtributos ['value'] = number_format ( $retencao ["retcsll"], 2, ",", "." );
	
	echo campo_texto ( $arrAtributos )?>%
					        </td>
							<td>
					        	<?php
	$arrAtributos = false;
	$arrAtributos ['name'] = "retcofinsperc";
	$arrAtributos ['id'] = "retcofinsperc";
	$arrAtributos ['title'] = "cofins Percentual";
	$arrAtributos ['obrigatorio'] = false;
	$arrAtributos ['habilitado'] = $habilitado;
	$arrAtributos ['size'] = 6;
	$arrAtributos ['maxsize'] = 6;
	$arrAtributos ['mascara'] = "[.###],##";
	$arrAtributos ['align'] = "right";
	$arrAtributos ['value'] = number_format ( $retencao ["retcofins"], 2, ",", "." );
	
	echo campo_texto ( $arrAtributos )?>%
					        </td>
							<td>
					        	<?php
	$arrAtributos = false;
	$arrAtributos ['name'] = "retpasepperc";
	$arrAtributos ['id'] = "retpasepperc";
	$arrAtributos ['title'] = "pasep Percentual";
	$arrAtributos ['obrigatorio'] = false;
	$arrAtributos ['habilitado'] = $habilitado;
	$arrAtributos ['size'] = 6;
	$arrAtributos ['maxsize'] = 6;
	$arrAtributos ['mascara'] = "[.###],##";
	$arrAtributos ['align'] = "right";
	$arrAtributos ['value'] = number_format ( $retencao ["retpasep"], 2, ",", "." );
	
	echo campo_texto ( $arrAtributos )?>%
					        </td>
							<td>
					        	<?php
	$arrAtributos = false;
	$arrAtributos ['name'] = "ftcaliqoutro";
	$arrAtributos ['id'] = "ftcaliqoutro";
	$arrAtributos ['title'] = "Outro Percentual";
	$arrAtributos ['obrigatorio'] = false;
	$arrAtributos ['habilitado'] = $habilitado;
	$arrAtributos ['size'] = 6;
	$arrAtributos ['maxsize'] = 6;
	$arrAtributos ['mascara'] = "[.###],##";
	$arrAtributos ['align'] = "right";
	$arrAtributos ['value'] = number_format ( $retencao ["retoutro"], 2, ",", "." );
	
	echo campo_texto ( $arrAtributos )?>%					        
					        
					        </td>
							<td><input type="text" id="rettotalperc" name="rettotalperc"
								style="text-align: right;" size="6" readonly
								value="<?php echo number_format($retencao["totalperc"],2,",",".");?>" />%
							</td>
						</tr>
						<tr>
							<td>
					        	<?php
	$arrAtributos = false;
	$arrAtributos ['name'] = "retir";
	$arrAtributos ['id'] = "retir";
	$arrAtributos ['title'] = "IR";
	$arrAtributos ['obrigatorio'] = false;
	$arrAtributos ['habilitado'] = $habilitado;
	$arrAtributos ['size'] = 10;
	$arrAtributos ['maxsize'] = 10;
	$arrAtributos ['mascara'] = "[.###],##";
	$arrAtributos ['align'] = "right";
	$arrAtributos ['value'] = ($retir) ? number_format ( ($retir), 2, ",", "." ) : "0,00";
	echo campo_texto ( $arrAtributos )?>	
					        </td>
							<td>
					        	<?php
	$arrAtributos = false;
	$arrAtributos ['name'] = "retcsll";
	$arrAtributos ['id'] = "retcsll";
	$arrAtributos ['title'] = "CSLL";
	$arrAtributos ['obrigatorio'] = false;
	$arrAtributos ['habilitado'] = $habilitado;
	$arrAtributos ['size'] = 10;
	$arrAtributos ['maxsize'] = 10;
	$arrAtributos ['mascara'] = "[.###],##";
	$arrAtributos ['align'] = "right";
	$arrAtributos ['value'] = ($retcsll) ? number_format ( ($retcsll), 2, ",", "." ) : "0,00";
	echo campo_texto ( $arrAtributos )?>	
					        </td>
							<td>
					        
					        	<?php
	$arrAtributos = false;
	$arrAtributos ['name'] = "retcofins";
	$arrAtributos ['id'] = "retcofins";
	$arrAtributos ['title'] = "COFINS";
	$arrAtributos ['obrigatorio'] = false;
	$arrAtributos ['habilitado'] = $habilitado;
	$arrAtributos ['size'] = 10;
	$arrAtributos ['maxsize'] = 10;
	$arrAtributos ['mascara'] = "[.###],##";
	$arrAtributos ['align'] = "right";
	$arrAtributos ['value'] = ($retcofins) ? number_format ( ($retcofins), 2, ",", "." ) : "0,00";
	echo campo_texto ( $arrAtributos )?>	
					        </td>
							<td>
					        
					        	<?php
	$arrAtributos = false;
	$arrAtributos ['name'] = "retpasep";
	$arrAtributos ['id'] = "retpasep";
	$arrAtributos ['title'] = "PIS/PASEP";
	$arrAtributos ['obrigatorio'] = false;
	$arrAtributos ['habilitado'] = $habilitado;
	$arrAtributos ['size'] = 10;
	$arrAtributos ['maxsize'] = 10;
	$arrAtributos ['mascara'] = "[.###],##";
	$arrAtributos ['align'] = "right";
	$arrAtributos ['value'] = ($retpasep) ? number_format ( ($retpasep), 2, ",", "." ) : "0,00";
	echo campo_texto ( $arrAtributos )?>	
					        </td>
							<td>
					        	<?php
	$arrAtributos = false;
	$arrAtributos ['name'] = "retoutro";
	$arrAtributos ['id'] = "retoutro";
	$arrAtributos ['title'] = "Outro Valor";
	$arrAtributos ['obrigatorio'] = false;
	$arrAtributos ['habilitado'] = $habilitado;
	$arrAtributos ['size'] = 10;
	$arrAtributos ['maxsize'] = 10;
	$arrAtributos ['mascara'] = "[.###],##";
	$arrAtributos ['align'] = "right";
	$arrAtributos ['value'] = ($retoutro) ? number_format ( ($retoutro), 2, ",", "." ) : "0,00";
	
	echo campo_texto ( $arrAtributos )?>
		        				</td>
							<td><input type="text" id="rettotal" name="rettotal"
								style="text-align: right;" size="10" readonly
								value=<?php echo ($somaRetido)?'"'.number_format($somaRetido,2,",",".").'"':"0,00";?>>
							</td>
						</tr>
					</tbody>
				</table>
			
			<td>
		
		</tr>
		<tr id=justificativaRetencao
			<?php if($ftcjustificativaretencao!=''): else:?>
			style="display: none;" <?php endif;?>>
			<td class="SubTituloDireita" valign="top">Justificativa da Outra
				Retenção:</td>
			<td>
	        	<?php
	$arrAtributos = false;
	$arrAtributos ['name'] = "ftcjustificativaretencao";
	$arrAtributos ['id'] = "ftcjustificativaretencao";
	$arrAtributos ['title'] = "Justificativa da Retenção";
	$arrAtributos ['obrigatorio'] = true;
	$arrAtributos ['habilitado'] = $habilitado;
	$arrAtributos ['cols'] = 56;
	$arrAtributos ['rows'] = 5;
	$arrAtributos ['maxsize'] = 500;
	$arrAtributos ['value'] = $ftcjustificativaretencao;
	echo campo_textarea ( $arrAtributos );
	
	?>
	        </td>
		</tr>
<?php
} else {
	?>
<tr>
			<td class="SubTituloDireita">Retenção:</td>
			<td>Nenhuma Alíquota de Retenção Selecionada.</td>
		</tr>
<?php } ?>
	    <tr>
			<td class="SubTituloDireita">Valor a Pagar ao Fornecedor:</td>
			<td><input type="text" id="ftctotalapagar" name="ftctotalapagar"
				style="text-align: right;" size="16" readonly
				value="<?php echo $ftctotalapagar?>" /></td>
		</tr>

	    <?php
					$perfis = arrayPerfil ();
					
					if (($cnpj && $hspid /*&& $epsid*/ )) : // &&
					                                        // (in_array(PERFIL_SUPER_USUARIO, $perfis ) || in_array(PERFIL_GESTOR_FINANCEIRO_UNIDADE, $perfis ) || in_array(PERFIL_PERFIL_ADMINISTRADOR, $perfis ) || in_array(PERFIL_GESTOR_UNIDADE, $perfis ))):
						
						?>
	    	<tr>
			<td class="SubTituloDireita" valign="top">Ordem Bancária</td>
			<td>
				<table id="tb_numob" class="tabela" bgcolor="#f5f5f5"
					cellspacing="1" cellpadding="3" style="width: 520px">
					<thead>
						<tr>
							<th style="width: 50px">Ação</th>
							<th style="width: 200px">Número</th>
							<th style="width: 150px">Valor</th>
							<th style="width: 150px">Empenho</th>
						</tr>
					
					
					<tbody>
		        		<?php if( $arrDoc['esdid'] == ESTADO_WK_FATURAMENTO_AGUARDANDO_PAGAMENTO ):?>
		        		<?php //if($arrDoc['esdid'] == ESTADO_WK_FATURAMENTO_AGUARDANDO_PAGAMENTO ? true : $habilitado):?>
			        		<tr>
							<td align="center"><img src="../imagens/consultar.gif"
								title="Pesquisar ordens bancárias" align="absmiddle"
								class="link"
								onclick="popupIncluirOb('<?php echo $cnpj; ?>','<?php echo $hspid; ?>')" />
								<!-- <img src="../imagens/consultar.gif" align="absmiddle" class="link" onclick="popupIncluirOb('<?php // echo $cnpj; ?>','<?php // echo $hspid; ?>','<?php //echo $epsid; ?>')" /> -->
							</td>
							<td valign="top">
			        				<?php
							$arrAtributos = false;
							$arrAtributos ['name'] = "obfnumero[]";
							$arrAtributos ['obrigatorio'] = true;
							$arrAtributos ['habilitado'] = $habilitadoOb;
							// $arrAtributos['habilitado'] = ($arrDoc['esdid'] == ESTADO_WK_FATURAMENTO_AGUARDANDO_PAGAMENTO ? true : $habilitado);
							$arrAtributos ['size'] = 13;
							$arrAtributos ['maxsize'] = 12;
							$arrAtributos ['align'] = "left";
							echo campo_texto ( $arrAtributos )?>
			        				<input type="hidden" name="obfdatatransacao[]"
								value="<?php echo date('Y/m/d'); ?>" />
							</td>
							<td align="center" valign="top">
			        				<?php
							$arrAtributos = false;
							$arrAtributos ['name'] = "obfvalor[]";
							$arrAtributos ['obrigatorio'] = true;
							$arrAtributos ['habilitado'] = $habilitadoOb;
							// $arrAtributos['habilitado'] = ($arrDoc['esdid'] == ESTADO_WK_FATURAMENTO_AGUARDANDO_PAGAMENTO ? true : $habilitado);
							$arrAtributos ['size'] = 12;
							$arrAtributos ['maxsize'] = 40;
							$arrAtributos ['mascara'] = "[.###],##";
							$arrAtributos ['align'] = "right";
							$arrAtributos ['onchange'] = "verificaTotalOb()";
							$arrAtributos ['evtkeyup'] = "verificaTotalOb()";
							echo campo_texto ( $arrAtributos )?>
			        				<input type="hidden" name="obfsiafi[]" value="false" />
							</td>
							<td align="center" valign="top" nowrap="nowrap">
			        				<?php
							$sql = "SELECT
												e.epsid as codigo,  
									   			s.nu_empenho as descricao
											FROM
												contratos.empenhovinculocontrato e
											INNER JOIN contratos.empenho_siafi s ON s.epsid = e.epsid
											WHERE
												e.ctrid = {$_SESSION['ctrid']}
											ORDER BY
												1";
							
							$db->monta_combo ( 'epsid[]', $sql, ($habilitadoOb ? 'S' : 'N'), "Selecione...", '', '', '', '140', 'S' );
							?>
			        			</td>
						</tr>
			        	<?php endif; ?>
		        		<?php if($arrOrdem): ?>
		        			<?php foreach($arrOrdem as $ordem):?>
		        				<?php if($ordem['obfsiafi'] == 't'): ?>
		        					<tr
							id="<?php echo trim($ordem['obfnumero']); ?>_<?php echo $ordem['epsid']; ?>"
							class="obsiafi">
							<td align="center">
					        				<?php // if($arrDoc['esdid'] == ESTADO_WK_FATURAMENTO_AGUARDANDO_PAGAMENTO ? true : $habilitado):?>
					        				<?php if( $habilitadoOb ):?>
					        					<img src="../imagens/excluir.gif"
								title="Excluir ordem bancária"
								onclick="$(this).parent().parent().remove();verificaTotalOb()"
								class="link" />
					        				<?php endif; ?>	
				        				</td>
							<td>
				        					<?php echo $ordem['hspid'].$ordem['unicod'].$ordem['obfnumero']; ?>
				        					<input type="hidden" name="obfnumero[]"
								value="<?php echo $ordem['obfnumero']; ?>" /> <input
								type="hidden" name="obfdatatransacao[]"
								value="<?php echo $ordem['obfdatatransacao']; ?>" />
							</td>
							<td align="center">
					        				<?php echo formata_valor($ordem['obfvalor']); ?>
					        				<input type="hidden" name="obfvalor[]"
								onchange="verificaTotalOb()"
								value="<?php echo formata_valor($ordem['obfvalor']); ?>" /> <input
								type="hidden" name="obfsiafi[]" value="true" />
											<?php $total_ob += $ordem['obfvalor']; ?>
				        				</td>
							<td>
				        					<?php
									echo $ordem ['nu_empenho'];
									?>
				        					<input type="hidden" name="epsid[]"
								value="<?php echo $ordem['epsid']; ?>" />
							</td>
						</tr>
		        				<?php else: ?>
			        				<tr>
							<td align="center">
					        				<?php //if($arrDoc['esdid'] == ESTADO_WK_FATURAMENTO_AGUARDANDO_PAGAMENTO ? true : $habilitado):?>
					        				<?php if( $habilitadoOb ):?>
					        					<img src="../imagens/excluir.gif"
								title="Excluir ordem bancária"
								onclick="$(this).parent().parent().remove();verificaTotalOb()"
								class="link" />
							</td>
					        				<?php endif; ?>
					        			<td>
					        				<?php
									$arrAtributos = false;
									$arrAtributos ['name'] = "obfnumero[]";
									$arrAtributos ['obrigatorio'] = true;
									// $arrAtributos['habilitado'] = ($arrDoc['esdid'] == ESTADO_WK_FATURAMENTO_AGUARDANDO_PAGAMENTO ? true : $habilitado);
									$arrAtributos ['habilitado'] = $habilitadoOb;
									$arrAtributos ['size'] = 13;
									$arrAtributos ['maxsize'] = 12;
									$arrAtributos ['align'] = "left";
									$arrAtributos ['value'] = $ordem ['hspid'] . $ordem ['unicod'] . $ordem ['obfnumero'];
									echo campo_texto ( $arrAtributos )?>
					        				<input type="hidden" name="obfdatatransacao[]"
								value="<?php echo $ordem['obfdatatransacao']; ?>" />
							</td>
							<td align="center">
					        				<?php
									$arrAtributos = false;
									$arrAtributos ['name'] = "obfvalor[]";
									$arrAtributos ['obrigatorio'] = true;
									$arrAtributos ['habilitado'] = $habilitadoOb;
									// $arrAtributos['habilitado'] = ($arrDoc['esdid'] == ESTADO_WK_FATURAMENTO_AGUARDANDO_PAGAMENTO ? true : $habilitado);
									$arrAtributos ['size'] = 12;
									$arrAtributos ['maxsize'] = 40;
									$arrAtributos ['mascara'] = "[.###],##";
									$arrAtributos ['align'] = "right";
									$arrAtributos ['evtkeyup'] = "verificaTotalOb()";
									$arrAtributos ['onchange'] = "verificaTotalOb()";
									$arrAtributos ['value'] = $ordem ['obfvalor'] ? number_format ( $ordem ['obfvalor'], 2, ',', '.' ) : "";
									$total_ob += $ordem ['obfvalor'];
									echo campo_texto ( $arrAtributos )?>
					        				<input type="hidden" name="obfsiafi[]" value="false" />
							</td>
							<td>
				        					<?php
									echo $ordem ['nu_empenho'];
									?>
				        					<input type="hidden" name="epsid[]"
								value="<?php echo $ordem['epsid']; ?>" />
							</td>
						</tr>
				        		<?php endif; ?>
		        			<?php endforeach;?>
		        		<?php endif;?>
		        		</tbody>
					<tfoot>
						<!-- 
						<tr>
							<td colspan="3" align="right">
							<?php if($arrDoc['esdid'] == ESTADO_WK_FATURAMENTO_AGUARDANDO_PAGAMENTO ? true : $habilitado):?>
							<a class="link" onclick="addOrdemBancaria(this)"><img src="../imagens/gif_inclui.gif" align="absmiddle" /> Inserir nova Ordem Bancária</a>
							<?php endif; ?>
							</td>
						</tr>
						 -->
						<tr>
							<td><b>Total</b></td>
							<td>&nbsp;</td>
							<td id="td_total_ob" align="center" style="font-weight: bold;">
		        				<?php echo $total_ob ? number_format($total_ob,2,',','.') : ""?>
		        			</td>
						</tr>
						<tr>
							<td colspan="4">
								<table class="tabela" id="tr_erro_calculo" bgcolor="#f5f5f5"
									cellspacing="1"
									style="width: 520px; margin: 0 0 0 0; display: none;">
									<thead>
										<tr>
											<td id="td_obs_ob" colspan="5"
												style="font-weight: bold; color: red; font-size: xx-small;">
												<b><?php echo $ftcvalor != number_format($total_ob, 2, ',', '.') && $habilitadoOb ? "A soma das OBs é diferente do valor da <b>nota fiscal</b>." : "" ?></b>
											</td>
										</tr>
									</thead>
									<tbody>
										<tr id="tr_erro_calculo_nota">
											<td style="width: 120px;"><b>Valor da nota fiscal:</b></td>
											<td><b><span id="span_ftcvalor"></span></b></td>
											<td>&nbsp;</td>
											<td style="width: 90px;"><b>Diferença:</b></td>
											<td><b><span id="span_diferenca_ftcvalor"></b></td>
										</tr>
										<tr id="tr_erro_calculo_informado">
											<td><b>Valor Informado:</b></td>
											<td><b><span id="span_ftcinformado"><?php echo $total_ob ? number_format($total_ob, 2, ',', '.') : "" ?></span></b></td>
											<td colspan="3">&nbsp;</td>
										</tr>
									</tbody>
								</table>
							</td>
						</tr>
					</tfoot>
				</table>
		        	<?php
						$arrAtributos = false;
						$arrAtributos ['name'] = "orbid";
						$arrAtributos ['id'] = "orbid";
						$empenho = new EmpenhoContrato ();
						$arrAtributos ['sql'] = $empenho->getSqlComboOrdemPagamento ( $epcid );
						$arrAtributos ['obrigatorio'] = true;
						$arrAtributos ['habilitado'] = $habilitadoOb;
						$arrAtributos ['value'] = $orbid;
						// $db->monta_combo($arrAtributos);
						?>
		        </td>
		</tr>
		<tr id="tr_erro_calculo_vlcontrato" style="display: none;">
			<td class="SubTituloDireita" valign="top">Alerta</td>
			<td align="left">
				<table class="tabela" id="table_erro_calculo_vlcontrato"
					bgcolor="#f5f5f5" cellspacing="1"
					style="padding: 3px; height: auto; width: 500px; border-color: red;">
					<tr>
						<td colspan="2"><b> O valor somado das notas fiscais não deve ser
								superior ao valor do contrato.</b></td>
					</tr>
					<tr>
						<td style="width: 160px;"><b>Valor do contrato:</b></td>
						<td><b><span id="valor_total_contrato">44.556,00</span></b></td>
					</tr>
					<tr>
						<td style="width: 160px;"><b>Valor total das notas fiscais:</b></td>
						<td><b><span id="valor_total_nf">50.622,00</b></td>
					</tr>
				</table>
			</td>
		</tr>            
	    <?php endif; ?>
	    	<tr>
			<td class="SubTituloDireita" valign="top">Anexos:</td>
			<td align="left">
	    <?php if($habilitado):?>
	    			<table id="tb_anexo" class="tabela" class="tabela"
					bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"
					style="width: 85%">
					<thead>
						<tr>
							<th>Ação</th>
							<th>Anexo</th>
							<th colspan="2">Descrição</th>

						</tr>
					</thead>
					<tbody>
						<tr id="tr_anexo">
							<td align="center"><img src='../imagens/excluir.gif'
								onclick="if($('#tb_anexo tbody tr').length > 1){ $(this).parent().parent().remove() } else { alert('Não é possível excluir mais linhas'); }"
								class='link' /></td>
							<td><input type="file" name="arquivo[]" /></td>
							<td>
					        	<?php
						$arrAtributos = false;
						$arrAtributos ['name'] = "arqdescricao[]";
						$arrAtributos ['obrigatorio'] = false;
						$arrAtributos ['habilitado'] = $habilitado;
						$arrAtributos ['size'] = 40;
						$arrAtributos ['maxsize'] = 255;
						$arrAtributos ['align'] = "left";
						echo campo_texto ( $arrAtributos )?>
						        </td>
							<td align="right"><img src="../imagens/gif_inclui.gif"
								onclick="addNovoAnexo(this)" class="link" /> &nbsp;<span
								onclick="addNovoAnexo(this)" style="cursor: pointer;">Adicionar
									anexo</span></td>
						</tr>
					</tbody>
				</table>
		<?php endif;?>					
		
<!-- 		
					<table class="tabela" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width:95%" >
						<tr>
							<th class="SubTituloCentro">
							Lista de Anexos
							</th>
						</tr>
						<tr>
							<td class="CampoCentroMenor">
							Clique na imagem <img src="../imagens/anexo.gif"> para fazer download.
							</td>
						</tr>
					</table>					
 -->				
					<?php
					if ($ftcid) {
						?>
 					<fieldset style="width: 80%; background: #FFFFFF;">
					<legend>
						Lista de anexos <font size="1">(Clique na imagem <img
							src="../imagens/anexo.gif"> para fazer download.)
						</font>
					</legend>	
					<?php
						$anexo = new AnexoFatura ();
						$anexo->listaAnexo ( $ftcid, $habilitado );
						?>
					</fieldset>
					<?php
					}
					?>
				</td>
		</tr>
		<tr style="background-color: #cccccc">
			<td align='right'></td>
			<td colspan="2">
	        	<?php if( ( ($habilitado || $habilitadoOb) && (in_array(PERFIL_TRIAGEM, $perfis ) ||in_array(PERFIL_SUPER_USUARIO, $perfis ) || in_array(PERFIL_GESTOR_FINANCEIRO_UNIDADE, $perfis ) || in_array(PERFIL_GESTOR_UNIDADE, $perfis ) || in_array(PERFIL_PERFIL_ADMINISTRADOR, $perfis ) || in_array(PERFIL_FISCAL_CONTRATO, $perfis )))):?>
	        		<input type="button" name="btn_buscar" value="Salvar"
				onclick="salvarNotaFiscal()">
	        	<?php endif;?>
	        	<input type="button" name="btn_novo" value="Voltar"
				onclick="window.location='?modulo=principal/execucaoFinanceiraContratos&acao=A'">
	        	<?php if ($ftcid): ?>
	        	<div style="float: right" class="notprint">
	        	<?php 
				/*
				 * Gestor Financeiro da unidade: 354
				 * Gestor da unidade: 1118
				 */
				$array =  pegaPerfilGeral($_SESSION['ucpf'], $_SESSION['sisid']); 
				if(in_array(PERFIL_GESTOR_FINANCEIRO_UNIDADE, $array) || in_array(PERFIL_GESTOR_UNIDADE, $array) || in_array(PERFIL_SUPER_USUARIO, $array) ){
					echo '<input type="button" value="Imprimir Despacho" onclick="javascript: PrintElem(\'#mydiv\');" />';
				} 
			?>
					
				</div>
	        	<?php endif;?>
	        </td>
		</tr>
	</table>
</form>
<br />
<?php if($_SESSION['administrativo']['contratos']['notafiscal']['alert']): ?>
<script>
	alert('<?php echo $_SESSION['administrativo']['contratos']['notafiscal']['alert'] ?>');
	<?php unset($_SESSION['administrativo']['contratos']['notafiscal']['alert'])?>
</script>
<?php 
	endif;
 
// Obtem dados adicionais do contrato.
$sql = "SELECT
		ent.entnome as contratada,
		tpc.tpcdsc || ' Nº ' ||  ctr.ctrnum || ' / ' || ctr.ctrano as numcontrato,
		mod.moddsc as moddsc,
		ctr.ctrobj as ctrobj,
		case
		when ctrvlrtotal is not null then ctrvlrtotal
		else ctrvlrinicial
		end as valor_total,
		(
		SELECT
		SUM(a.total)
		FROM (
		SELECT
		obs.valor AS total
		FROM
		contratos.ctcontrato c
		JOIN entidade.entidade e ON e.entid = c.entidcontratada
		JOIN contratos.hospital h ON h.hspid = c.hspid AND
		h.hspstatus = 'A'
		JOIN contratos.hospitalug hu ON hu.hspid = h.hspid
		JOIN contratos.empenhovinculocontrato ec ON ec.ctrid = c.ctrid
		JOIN contratos.empenho_siafi es ON es.epsid = ec.epsid AND
		TRIM(es.ungcod) = TRIM(hu.ungcod) AND
		TRIM(es.co_favorecido) = TRIM(e.entnumcpfcnpj)
		JOIN contratos.ob_siafi obs ON TRIM(obs.empenho) = TRIM(es.nu_empenho) AND
		TRIM(obs.unidade) = TRIM(hu.ungcod) AND
		TRIM(obs.it_co_credor) = TRIM(e.entnumcpfcnpj)
		WHERE
		c.ctrid = {$_SESSION['ctrid']} AND
		obs.ob NOT IN (
		SELECT
		obf.obfnumero
		FROM
		contratos.faturacontrato fc
		JOIN contratos.ordembancariafatura obf ON obf.ftcid = fc.ftcid
		WHERE
		fc.ftcstatus = 'A' AND
		fc.ctrid = {$_SESSION['ctrid']}
		)
		UNION ALL
		SELECT
		SUM(obfvalor) AS total
		FROM contratos.faturacontrato ftc
		JOIN contratos.ordembancariafatura obf ON ftc.ftcid = obf.ftcid
		WHERE ftc.ftcstatus = 'A'
		AND ftc.ctrid = ctr.ctrid
		) AS a
		) AS valor_executado,
		(
		SELECT SUM(ftcglosa) AS total FROM contratos.faturacontrato ftc WHERE ftc.ftcstatus = 'A'
		AND ftc.ctrid = ctr.ctrid
		)as glosa,
		(select
		sum(tdavlr) as total
		from contratos.cttermoaditivo adi
		where adi.tdastatus = 'A'
		and adi.ctrid = ctr.ctrid) as valor_aditivo,
		hspabrev || ' - '|| hspdsc as contratante
		FROM contratos.ctcontrato ctr
		left join contratos.ctmodalidadecontrato mod on ctr.modid = mod.modid
		left join contratos.cttipocontrato tpc on ctr.tpcid = tpc.tpcid
		left join entidade.entidade ent on ent.entid = ctr.entidcontratada
		left join contratos.hospital h on h.hspid = ctr.hspid
		WHERE ctr.ctrid = {$_SESSION['ctrid']}";
$dados = $db->pegaLinha($sql);
?>
<style rel="stylesheet" type="text/css">
	@media print { .notprint { display: none }}
	@media screen {.notscreen { display: none }}
</style>
<div id="mydiv" class="notscreen" style="font-size: 14px;">
			<!-- Topo Imagem superior + título-->
			<div>
				<div align="center"><img src="/imagens/logo-ebserh-contraste.png" width="150"></div>
				<div style="font-weight: bold;">Despacho nº:<span style="margin-left: 100px;"> CO/DAF/EBSERH/MEC</span></div>
				<div align="right"><?php echo 'Brasília, '. date('d/m/Y');?></div>
			</div>

			<!-- Informações Iniciais -->
			<div>
				<table border='0'>
					<tr>
						<td>Processo:</td>
						<td><?php echo $dados['numcontrato'];?></td>
					</tr>
					<tr>
						<td>Favorecidos:</td>
						<td><?php echo $dados['contratada'];?></td>
					</tr>
					<tr>
						<td>CNPJ:</td>
						<td><?php echo $cnpj;?></td>
					</tr>
					<tr>
						<td>Nota Fiscal (Fatura):</td>
						<td><?php echo $ftcnumero;?></td>
					</tr>
					<tr>
						<td>Valor total:</td>
						<td><?php echo 'R$ '.$ftctotalfatura;?></td>
					</tr>
				</table>
			</div>
			<br>
			<!-- Texto corpo -->
			<div align="center">Senhor Diretor Administrativo Financeiro,</div>
			<ul style="list-style-type: decimal; text-align: justify;">
				<li>Trata-se do pagamento à Empresa <?php echo '<b>'.$dados['contratada'].'</b>';?>, conforme
					documento emitido em nome da Empresa <?php echo '<b>'.$dados['contratante'].'</b>';?> - Nota Fiscal -
					<?php echo '<b>'.$ftcnumero.'</b>'?>, referente a <?php echo '<b>'.$ftcdescricao.'</b>';?>, conforme especificado
					nos comprovantes anexos e de acordo com o demonstrativo abaixo: <!-- Tabela demonstrativo -->
					<table class="bordasimples" align='center'>
						<thead>
							<tr style="background: #ccc;">
								<td>Seq.</td>
								<td>Nº da Nota fiscal / Fatura</td>
								<td>Valor Bruto <br> (a)</td>
								<td>Retenção Tributária <br>(b)</td>
								<td>Glosa <br>(c)</td>
								<td>Valor líquido Devido <br>(d = a-b-c)</td>
							</tr>
						</thead>
						<tbody>
							<tr>
								<td><?php echo ++$seq;?></td>
								<td><?php echo $ftcnumero;?></td>
								<td><?php echo 'R$ '.$ftctotalfatura;?></td>
								<td><?php echo ($somaRetido) ? 
												'R$ '. number_format($somaRetido,2,",",".") : 
												"R$ 0,00";?>
								</td>
								<td><?php echo 'R$ ' . number_format($dados['glosa'], 2, ',', '.' );?></td>
								<td><?php  
								$retido 	= 	str_replace( '.','', 
												($somaRetido)? 
												number_format($somaRetido,2,",",".") : "0,00" );
								$vlrGlosa   = 	$dados['glosa'];
								$vlrTotal	=	 str_replace('.','', $ftctotalfatura);
								// A - B - C
								$calculo 	= str_replace('-','',$retido - $vlrGlosa - $vlrTotal); 
								echo 'R$'. formata_valor($calculo);
								?></td>
							</tr>
						</tbody>
						<tfoot>
							<tr>
								<td colspan="2">Total</td>
								<td><?php echo 'R$ '.$ftctotalfatura;?></td>
								<td><?php echo ($somaRetido) ? 
												'R$ '. number_format($somaRetido,2,",",".") : 
												"R$ 0,00"; ?>
								</td>
								<td><?php echo 'R$ ' . number_format($dados['glosa'], 2, ',', '.' );?></td>
								<td><?php echo 'R$'. formata_valor($calculo); ?></td>
							</tr>
						</tfoot>
					</table>
				</li>
				<li>Ressaltamos que ao efetuar o atesto na fatura apresentada para
					pagamento, a fiscalização registra que os serviços, constantes da
					nota fiscal, foram devidamente prestados.</li>
				<li>De acordo com o Art. 42 do Decreto nº 93.872/86 e no art. 64 da
					Lei 4.320/67 sugerimos que o presente expediente seja submetido à
					apreciação do Senhor <b>Presidente da Empresa</b>, objetivando a
					necessária ordenação do pagamento da despesa.</li>
			</ul>
			
			<?php 
			// meses
			$mess = array("Jan" => "janeiro", "Feb" => "fevereiro", "Mar" => "marco", "Apr" => "abril", "May" => "maio", "Jun" => "junho", "Jul" => "julho", "Aug" => "agosto", "Nov" => "novembro", "Sep" => "setembro", "Oct" => "outubro", "Dec" => "dezembro");
			?>
			<br>
			<!-- Assinaturas -->
			<p align="center">
				<b>Marco Antônio Alves Côrrea</b> <br>Coordenador de Orçamento
			</p>
			<p align="left">
				De acordo,<br> À apreciação do Senhor Presidente,<br> Em <span style="margin-left:30px;">de</span>  
				<?php echo $mess[date('M')];?> de <?php echo date('Y');?>.</span>
			</p>
			<p align="center">
				<b>Walmir Gomes de Sousa</b> <br>Diretor da Diretoria Administrativa
				Financeira
			</p>
			<p align="left">
				De acordo,<br> Autorizo o pagamento da despesa.<br> Em <span style="margin-left:30px;">de</span>  
				<?php echo $mess[date('M')];?> de <?php echo date('Y');?>.</span>
			</p>
			<p align="center">
				<b>José Rubens Rebelatto</b> <br>Presidente da Empresa Brasileira de
				Serviços Hospitalares
			</p>
			
</div>
<script type='text/javascript'>
 function PrintElem(elem)
 {
     Popup($(elem).html());
 }
 function Popup(data) 
 {
     var mywindow = window.open('', 'Despacho',  'height=800,width=600');
     mywindow.document.write('<html><head><title>Nota Fiscal nº <?php echo $ftcnumero;?></title>');
     mywindow.document.write("<style rel=\'stylesheet\' type=\'text/css\'>table.bordasimples { border-collapse: collapse; font-size: 14px; } table.bordasimples tr td{ border: 1px solid #ccc;} #myDiv{ font-family: arial, verdana, sans-serif;} #myDiv, p { padding-left: 40px;} </style>' ");
     mywindow.document.write('<body style=\'margin-top: -25px; font-size: 14px; line-height: 18px;\'>');
     mywindow.document.write(data);
     mywindow.document.write('</body></html>');

     mywindow.print();
     mywindow.close();

     return true;
 }
</script>