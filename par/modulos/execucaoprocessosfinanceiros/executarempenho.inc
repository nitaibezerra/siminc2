<?php
// ------------- INCLUDES ------------- //
include APPRAIZ . "par/modulos/execucaoprocessosfinanceiros/classes/processos.class.inc";
include APPRAIZ . "par/modulos/execucaoprocessosfinanceiros/classes/empenhos.class.inc";

// ------------- DECLARAÇÃO DE VARIAVEIS ------------- //
$empid	 	= $_REQUEST['empid'] 	? $_REQUEST['empid'] 	: 0;
$processo 	= $_REQUEST['processo'] ? $_REQUEST['processo'] : die("<script> alert('Erro ao tentar identificar o processo!'); window.close();</script>");

$obProcesso = new Processos();
$obProcesso->carregarPorId($processo);

// ------------- REQUISIÇÕES AJAX E FUNÇÕES ------------- //
if($_REQUEST['requisicao'] == 'listaPassivelEmpenho') {
	
	
	$obEmpenho = new Empenho();
	$obEmpenho->carregarPorId($empid);

	$obProcesso->controleDeTela(get_class($obEmpenho));
	$obEmpenho->listaDadosPassiveisDeEmpenho($obProcesso);
	exit();
}

if($_REQUEST['requisicao'] == 'lista100empenhado') {
	
	$obEmpenho = new Empenho();
	$obEmpenho->carregarPorId($empid);

	$obProcesso->controleDeTela(get_class($obEmpenho));
	$obEmpenho->listaDados100ProcentoEmpenhadas($obProcesso);
	exit();
}
?>
<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    <script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	</head>
	<body>
		<?php monta_titulo( $titulo_modulo, '' ); ?>
		<? $obProcesso->montaCabecalhoEmpenho(); ?>
		<form name="formempenho" id="formempenho" method="post" action="" >
			<input type="hidden" id="empid" name="empid" value="<?php echo $empid; ?>">
			<input type="hidden" id="processo" name="processo" value="<?php echo $processo; ?>">
			<input type="hidden" id="sbdidH" name="sbdidH" value="">
			
			<table id="total" align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
				<tr>
					<td colspan="2">
						<div id="divListaPassivelDeEmpenho">Carregando...</div>
					</td>
				</tr>
				<tr>
					<td class="subtitulodireita" width="50%"><b>Valor total de empenho:</b></td>
					<td><input type="text" id="id_total" name="name_total" size="20" class="normal" style="text-align:right;" readonly="readonly" value="0,00"></td>
				</tr>
				<tr>
					<td colspan="2" class="subtituloCentro">
						<?php 
							$perfil = pegaArrayPerfil($_SESSION['usucpf']);
							
							if(	in_array(PAR_PERFIL_EMPENHADOR, $perfil) ||
								in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
								in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || 
								in_array(PAR_PERFIL_COORDENADOR_GERAL ,$perfil)
							){
								$disabled = '';
							}else{
								$disabled = 'disabled="disabled"';
							}
						?>
						<input type="button" name="solicitar" <?php echo $disabled; ?> id="solicitar" value="Solicitar empenho" onclick="document.getElementById('div_auth').style.display='block'" />
						
						<?php if( in_array( PAR_PERFIL_SUPER_USUARIO, pegaArrayPerfil($_SESSION['usucpf']) ) ){ ?>
							<input type=button id=visualizar name=visualizar  value=Visualizar XML onclick=visEmp(); />
						<?php } ?>
					</td>
				</tr>
				<tr>
					<td colspan="2">
						<div id="divLista100porcentoEmpenhado" style="overflow-x: auto; overflow-y: auto; width:100%; height:300px;">Carregando...</div>
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>
<script type="text/javascript">
    $(document).ready(function() {
    	var processo = $("#processo").val();
		carregarListaSubacaoAEmpenhar(processo);
		carregarListaSubacao100Empenhado(processo);
	});
	
	function carregarListaSubacaoAEmpenhar(processo) {
		$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=execucaoprocessosfinanceiros/executarempenho&acao=A",
	   		data: "requisicao=listaPassivelEmpenho&processo="+processo,
	   		async: false,
	   		success: function(msg){
	   			document.getElementById('divListaPassivelDeEmpenho').innerHTML = msg;
	   		}
		});
		
	}
	
	function carregarListaSubacao100Empenhado(processo) {
		$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=execucaoprocessosfinanceiros/executarempenho&acao=A",
	   		data: "requisicao=lista100empenhado&processo="+processo,
	   		async: false,
	   		success: function(msg){
	   			document.getElementById('divLista100porcentoEmpenhado').innerHTML = msg;
	   		}
		});
		
	}
	
	function marcarChk(obj) {
		var linha = obj.parentNode.parentNode;
	
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
		
		//var tabela = obj.parentNode.parentNode.parentNode;
		calcularTotal();
		
		//faço o AJAX para poder carregar o combo da fonte de recurso
		/*$.ajax({
			type: "POST",
			url: "par.php?modulo=principal/solicitacaoEmpenhoPar&acao=A",
			data: "requisicao=carregarFonteRecurso&sbdid="+obj.value,
			async: false,
			success: function(msg){
				document.getElementById('fonteRecursoSPAN').innerHTML=msg;
		}
		});

		//faço o AJAX para poder carregar o combo do Plano Interno
		$.ajax({
			type: "POST",
			url: "par.php?modulo=principal/solicitacaoEmpenhoPar&acao=A",
			data: "requisicao=carregarPlanoInterno&sbdid="+obj.value,
			async: false,
			success: function(msg){
				document.getElementById('planointernoSPAN').innerHTML=msg;
		}
		});
		
		filtraPTRES( document.getElementById('planointerno').value );*/
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
				
				//var valorEmpenhado = linha.cells[4].childNodes[2].value;
				/*if( obj.id.substr( 0,6 ) == 'id_vlr' ){ //veio do valor
					var total = obj.value;
					var total_mac = mascaraglobal('###.###.###.###,##',replaceAll(total,'.',''));
					var total_for = parseFloat(replaceAll(replaceAll(total, '.', ''), ',', '.'));
					var percent = total_for*100/valor;
					linha.cells[6].childNodes[0].value = percent;
					linha.cells[6].childNodes[1].value = total_mac;
				} else { // veio da porcentagem
					var total = valor*obj.value/100;
					var total_mac = mascaraglobal('###.###.###.###,##',replaceAll(total.toFixed(2),'.',''));
					//linha.cells[7].innerHTML = total_mac;
					linha.cells[7].childNodes[0].value = total_mac;
					linha.cells[6].childNodes[1].value = total_mac;
				}
				//linha.cells[5].childNodes[1].value = total;
				var tabela = obj.parentNode.parentNode.parentNode;*/
			
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
		var valorInformado 	= linha.cells[7].childNodes[0].value;
		
		if( valorInformado > valorSubacao ){
			alert('O valor informado para empenho ultrapassa 100% do valor da Subação.');
			linha.cells[7].childNodes[0].value = 0;
		}
		
	}
    </script>
