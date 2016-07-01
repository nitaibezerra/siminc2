<?
if ($_POST['ajax'] == "consultaProcesso"){
	$sql = "SELECT 
				prcnumsidoc 
			FROM 
				conjur.processoconjur 
			WHERE
				prcnumsidoc = '" . trim($_POST['prcnumsidoc']) . "'"; 
	$prcnumsidoc = $db->pegaUm( $sql );
	
	die($prcnumsidoc);
}

if($_REQUEST['req'] == 'ACsidoc'){
	ob_clean();
	$sql = "SELECT prcnumsidoc FROM conjur.processoconjur"; 
	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();
	 
	$q = strtolower($_GET["q"]);
	if ($q === '') return;
	
	foreach ($arDados as $key=>$value){
		if (strpos(strtolower($key), $q) !== false) {
			$prcnumsidoc = trim($value['prcnumsidoc']);
			echo "{$prcnumsidoc}|{$prcnumsidoc}\n";			
		}
	}
	
	die;

}

if($_REQUEST['req'] == 'ACprcnomeinteressado'){
	ob_clean();
	$sql = "SELECT prcnomeinteressado FROM conjur.processoconjur"; 
	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();
	 
	$q = strtolower($_GET["q"]);
	if ($q === '') return;
	foreach ($arDados as $key=>$value){
		if (strpos(strtolower($value['prcnomeinteressado']), $q) !== false) {
			$prcnomeinteressado = $value['prcnomeinteressado'];
			echo "{$prcnomeinteressado}|{$prcnomeinteressado}\n";			
		}
	}
	
	die;

}

if($_REQUEST['req'] == 'ACtasdsc'){
	ob_clean();
	$sql = "SELECT tasdsc FROM conjur.tipoassunto WHERE tasstatus = 'A' ORDER BY tasdsc"; 
	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();
	 
	$q = strtolower($_GET["q"]);
	if ($q === '') return;
	foreach ($arDados as $key=>$value){
		if (strpos(strtolower($value['tasdsc']), $q) !== false) {
			$tasdsc = $value['tasdsc'];
			echo "{$tasdsc}|{$tasdsc}\n";			
		}
	}
	
	die;

}

if($_REQUEST['req'] == 'ACprodsc'){
	ob_clean();
	
	if( empty($_REQUEST['unpid']) ) die;

	$sql = "SELECT prodsc FROM conjur.procedencia WHERE unpid = '".$_REQUEST['unpid']."' AND prodtstatus = 'A' ORDER BY	prodsc"; 
	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();
	 
	$q = strtolower($_GET["q"]);
	if ($q === '') return;
	foreach ($arDados as $key=>$value){
		if (strpos(strtolower($value['prodsc']), $q) !== false) {
			$prodsc = $value['prodsc'];
			echo "{$prodsc}|{$prodsc}\n";			
		}
	}
	
	die;

}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
}
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
?>
<!--<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>-->
<script type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="./js/conjur.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />

<script type="text/javascript">
	function exibirOcultarNumeracaoUnica(value) {
		var visibilidade = determinarVisibilidadeProcesso(value);
		document.getElementById('linhaNumeracaoUnicaJudicial').style.visibility = visibilidade;
		document.getElementById('linhaNumeracaoUnicaJudicialAntigo').style.visibility = visibilidade;
	}	
	
	function determinarVisibilidadeProcesso(valorSelecionado) {
		if( valorSelecionado==2 ) { // 2, conforme ID na base de dados
			return "visible";
		} else {
			reinicializarNumeroProcessoJudicial();
			return "collapse";
		}
	}

	function reinicializarNumeroProcessoJudicial() {
		document.formulario.prcnumeroprocjudicial.value = "";
		document.formulario.prcnumeroprocjudantigo.value = "";
	}		
	
	function conjurJs_digitoVerificador() {
		var campoNumeroProcessoSidoc    = document.getElementById('prcnumsidoc');
		
		if(!conferirDigitoVerificadorModulo11(campoNumeroProcessoSidoc)){
			document.getElementById('botaosubmeter').disabled=false;
			return false;
		} 
	} 	

	jQuery.noConflict();

	jQuery(document).ready(function() {

		jQuery('#prodsc').removeClass('normal ac_input');
		jQuery('#prodsc').addClass('disabled ac_input');
		
		jQuery('#prcnumsidoc').autocomplete("conjur.php?modulo=principal/cadastrarprocesso&acao=A&req=ACsidoc", {
		  	cacheLength:10,
			width: 440,
			scrollHeight: 220,
			selectFirst: true,
			autoFill: true
		});
		
		jQuery('#prcnomeinteressado').autocomplete("conjur.php?modulo=principal/cadastrarprocesso&acao=A&req=ACprcnomeinteressado", {
		  	cacheLength:10,
			width: 440,
			scrollHeight: 220,
			selectFirst: true,
			autoFill: true
		});
		
		jQuery('#tasdsc').autocomplete("conjur.php?modulo=principal/cadastrarprocesso&acao=A&req=ACtasdsc", {
		  	cacheLength:10,
			width: 440,
			scrollHeight: 220,
			selectFirst: true,
			autoFill: true
		});

		jQuery('#prodsc').autocomplete("conjur.php?modulo=principal/cadastrarprocesso&acao=A&req=ACprodsc", {
			extraParams: 
			{
		       unpid: function() { return jQuery("#unpid").val(); }
			},
		  	cacheLength:10,
			width: 440,
			scrollHeight: 220,
			selectFirst: true,
			autoFill: true
		});

		jQuery('#unpid').change(function(){
//			url = "conjur.php?modulo=inicio&acao=C&req=ACprodsc&unpid="+jQuery(this).val();
//			alert(url);
			if(jQuery(this).val()!='')
			{
				if(jQuery('#prodsc').hasClass('disabled'))
				{
					jQuery('#prodsc').removeClass('disabled ac_input');
					jQuery('#prodsc').addClass('normal ac_input');
				}
			}
			else
			{
				jQuery('#prodsc').removeClass('normal ac_input');
				jQuery('#prodsc').addClass('disabled ac_input');
				
			}
		});
	});
	
	function mudaCampoNumero()
	{
		jQuery('#tdNumeroProcesso').html('Número do Processo '+ jQuery('#tipoNumeracao').val());
	}
	
</script>
<? 
//$sql = " SELECT prcnumsidoc FROM conjur.processoconjur "; 
//$processosidoc = $db->carregar($sql);

?>
<form name="formulario" id="formulario" method="post" onclick="return ">
<input type="hidden" name="requisicao" value="inserirprocessoconjur">
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	<tr>
		<td bgcolor="#CCCCCC" colspan="2"><b>Cadastro de Processo</b></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo de processo :</td>
		<td><?
		$sqlTipoProcesso = "SELECT tprid as codigo, tprdsc as descricao 
							FROM conjur.tipoprocesso 
							WHERE tprstatus = 'A' 
							ORDER BY tprdsc";
		$db->monta_combo('tprid', $sqlTipoProcesso, 'S', 'Selecione...', 'exibirOcultarNumeracaoUnica', '', '', 150, 'S', 'tprid');
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo de numeração SIDOC/EMEC :</td>
		<td>
			<select id="tipoNumeracao" name="tipoNumeracao" onchange='mudaCampoNumero()'>
				<option value='SIDOC'>SIDOC</option>
				<option value='EMEC'>EMEC</option>
			</select>
		</td>
	</tr>
	<tr>
		<td id='tdNumeroProcesso' class="SubTituloDireita" style="width:20%;">Número do Processo SIDOC :</td>
		<td><?
			$pcjnumerosidoc = $_REQUEST['prcnumsidoc']; 
			// retirada validacao do numero sidoc/emec -> echo campo_texto('prcnumsidoc', 'S', 'S', '', 35, 25, '#################', '', 'left', '', 0, 'id="prcnumsidoc"','','','numProcesso(this);');
			echo campo_texto('prcnumsidoc', 'S', 'S', '', 35, 25, '#################', '', 'left', '', 0, 'id="prcnumsidoc"','','','');
			?>
		</td>
	</tr>

	<tr id="linhaNumeracaoUnicaJudicial" style="visibility: collapse;">
		<td class="SubTituloDireita" style="width:20%;">Numeração Única Judicial :</td>
		<td><? echo campo_texto('prcnumeroprocjudicial', 'N', 'S', '', 30, 25, '#######-##.####.#.##.####', '', 'right', '', 0, ' style="text-align:right;" '); ?></td>
	</tr>
	
	<tr id="linhaNumeracaoUnicaJudicialAntigo" style="visibility: collapse;">
		<td class="SubTituloDireita" style="width:20%;">Numeração Judicial Antiga :</td>
		<td><? echo campo_texto('prcnumeroprocjudantigo', 'N', 'S', '', 30, 25, '####-##-##-######-#', '', 'right', '', 0, ' style="text-align:right;" '); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Interessado :</td>
		<td><? echo campo_texto('prcnomeinteressado', 'S', 'S', '', 50, 255, '', '', 'left', '', 0, 'id="prcnomeinteressado"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data de Entrada do Processo :</td>
		<td><? echo campo_data('prcdtentrada', 'S', 'S', '', 'S'); ?></td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita">Tipo de Procedência :</td>
		<td><?
			//$unpid = $_REQUEST['unpid'];
			$sqlTipoProcedencia = "SELECT unpid as codigo,	unpdsc as descricao 
						   FROM conjur.unidadeprocedencia
						   WHERE undpstatus = 'A'
						   ORDER BY	unpdsc";
			$db->monta_combo('unpid', $sqlTipoProcedencia, 'S', 'Selecione...', '', '', '', 300, 'S','unpid');
			?></td>
	</tr>	
	
	<tr>
		<td align='right' class="SubTituloDireita">Procedência:</td>
		<td>
<!--			<div id="proid_on" style="display:none;">	-->
				<?
				$prodsc = $_REQUEST['prodsc'];
				echo campo_texto('prodsc', 'S', 'S', '', 50, 255, '', '', 'left', '', 0, 'id="prodsc"');
				?>			
<!--			</div>-->
<!--			<div id="proid_off" style="color:#909090;">Selecione um Tipo de Procedência.</div>-->
		</td>
	</tr>
	
	<!--<tr>
		<td class="SubTituloDireita">Procedência :</td>
		<td><?
			$unicod = $_REQUEST['unicod'];
			$sqlProcedencia = " SELECT 	 unicod || '&' || unitpocod as codigo, unidsc as descricao 
						   		FROM 	 public.unidade
						   		WHERE 	 orgcod = '".ORGCOD."' AND unistatus = 'A'
						   		ORDER BY unidsc";
			$db->monta_combo('unicod', $sqlProcedencia, 'S', 'Selecione...', '', '', '', 300, 'N','unicod');
			?></td>
	</tr>
	-->
	<!--<tr>
		<td class="SubTituloDireita">Tema :</td>
		<td><?
//		$tasid = $_REQUEST['tasid'];
//		$sqlAssunto = "SELECT 	tasid as codigo, tasdsc as descricao
//					   FROM		conjur.tipoassunto
//					   WHERE 	tasstatus = 'A' 
//					   ORDER BY tasdsc";
//		$db->monta_combo('tasid', $sqlAssunto, 'S', 'Selecione...', '', '', '', 300, 'N', 'tasid');
		/*$tasdsc = $_REQUEST['tasdsc'];
		echo campo_texto('tasdsc', 'S', 'S', '', 50, 255, '', '', 'left', '', 0, 'id="tasdsc"');*/
		?></td>
	</tr>-->
	<tr>
		<td class="SubTituloDireita">Urgente :</td>
		<td>
			<input type="radio" name="tipid" value="1"/> Sim
			<input type="radio" name="tipid" value="2" checked="checked"/> Não
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita">Relevante :</td>
		<td>
			<input type="radio" name="prcprioritario" id="prcprioritario" value="sim" /> Sim
			<input type="radio" name="prcprioritario" id="prcprioritario" value="nao" checked="checked"/> Não
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
		</td>
	</tr>			
	
	<tr>
		<td class="SubTituloDireita">Assunto :</td>
		<td><? echo campo_textarea('prcdesc', 'N', 'S', '', 100, 7, 500); ?></td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="2"><b>Partes no Processo</b></td>
	</tr>
	<tr>
		<td colspan="2">
			<table id="tabela_interessado" width="100%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
				<tr>
				<td valign="top" align="center" class="title" style="width:80px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nome</strong></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Tipo Pessoal</strong></td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td>
			<input type="button" value="Inserir Partes" style="cursor:pointer" onclick="abreJanelaCadastroInteressados();">
		</td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="2"><b>Expressão Chave</b></td>
	</tr>
	<tr>
		<td colspan="2">
			<table id="tabela_expressao" width="100%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
				<tr>
					<td valign="top" align="center" class="title" style="width:80px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
					<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Expressão Chave</strong></td>
				</tr>
				<tr>
					<td valign="top" align="center" class="title" style="width:80px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><img style="cursor:pointer;" src="../imagens/gif_inclui.gif"  onclick="cadastrarExpressao();"></td>
					<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><input type="text" class="normal" id="expressao_chave" size="30" maxlength="20" value="<?=$_REQUEST["expressao_chave"]?>"></td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Anexar Processos</td>
	</tr>
	<tr>
		<td colspan="2">
			<table id="tabela_vincprocessos" width="100%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
			<tr>
				<td valign="top" align="center" class="title" style="width:80px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ações</strong></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nº do processo</strong></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Tipo</strong></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Interessado</strong></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data Entrada</strong></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Situação Conjur</strong></td>
			</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td colspan="2"><input type="button" value="Anexar processo" onclick="abreJanelaProcessosVinculados();"></td>
	</tr>
	<tr>
		<td bgcolor="#CCCCCC" colspan="2" align="center">
			<input type="button" value="Salvar" id="botaosubmeter" onclick="return submeteFormCadastroProcesso();">
			<input type="button" value="Voltar" style="cursor:pointer" onclick="this.disabled=true;window.location='?modulo=inicio&acao=C';">
		</td>
	</tr>
</table>
</form>
<script>
function numProcesso(obj) {
	var prcnumsidoc = obj.value;	
	
	if (jQuery('#tipoNumeracao').val() != 'SIDOC')
	{
		if (prcnumsidoc.length != 9)
		{
			alert('Número EMEC inválido');
			obj.focus();
			return false;
		}
		else
		{
			var ano = prcnumsidoc.substring(0,4);
			var hoje = new Date();
			
			if (ano < 1900 || ano > hoje.getFullYear())
			{
				alert('número EMEC inválido');
				obj.focus()
				return false;
			}
			else
			{
				return true;
			}
		}
	}
		
	divCarregando();
	
	
	var url = "?modulo=principal/cadastrarprocesso&acao=A";
	new Ajax.Request(url, {
			  method	   : 'post',
			  asynchronous : false,
			  parameters   : '&ajax=consultaProcesso&prcnumsidoc=' + prcnumsidoc,
			  onSuccess    : function(retorno) {
				if(retorno.responseText != ''){
					alert('O Processo já está cadastrado!');
					obj.value = '';
				}else{
					return conferirDigitoVerificadorModulo11(obj);
				}
			  } 	
			});
	divCarregado();
	return;
}
</script>
