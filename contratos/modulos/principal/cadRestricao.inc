<?php
include_once APPRAIZ."contratos/classes/FaseRestricao.class.inc";
include_once APPRAIZ."contratos/classes/TipoRestricao.class.inc";
include_once APPRAIZ."contratos/classes/Restricao.class.inc";

$restricao = new Restricao($_GET['rstid']);
switch ( $_POST['requisicao'] ){
	case 'salvar':
		$arDado = $_POST;
		$arDado['usucpfsuperacao'] = ($_POST['rstsituacao'] == "true" ? $_SESSION['usucpf'] : '');
		
		$arDado['ctrid'] = $_SESSION['ctrid'];

		$arDado['usucpf'] = $_SESSION['usucpf'];
		
		$arCamposNulo = array();
		if ( empty($arDado['usucpfsuperacao']) ){
			$arDado['usucpfsuperacao'] = null;
			$arCamposNulo[]  = 'usucpfsuperacao'; 	
		}
		if ( empty($arDado['rstsituacao']) ){
			$arDado['rstsituacao'] = null;
			$arCamposNulo[]  = 'rstsituacao'; 	
		}
		if ( empty($arDado['rstdtsuperacao']) ){
			$arDado['rstdtsuperacao'] = null;
			$arCamposNulo[]  = 'rstdtsuperacao'; 	
		}
		if ( empty($arDado['rstdtprevisaoregularizacao']) ){
			$arDado['rstdtprevisaoregularizacao'] = null;
			$arCamposNulo[]  = 'rstdtprevisaoregularizacao'; 	
		}
		if ( empty($arDado['rstdscprovidencia']) ){
			$arDado['rstdscprovidencia'] = null;
			$arCamposNulo[]  = 'rstdscprovidencia'; 	
		}
		$arDado['rstdtprevisaoregularizacao'] = $arDado['rstdtprevisaoregularizacao'] ? formata_data_sql($arDado['rstdtprevisaoregularizacao']):null;
		$arDado['rstdtsuperacao'] = $arDado['rstdtsuperacao'] ? formata_data_sql($arDado['rstdtsuperacao']):null;
		
		$restricao->popularDadosObjeto( $arDado )
				  ->salvar(true, true, $arCamposNulo);
		$db->commit();				  		   

		die("<script>
				alert('Operação realizada com sucesso!'); 
				window.opener.location.replace( window.opener.location ); 
				window.close();
			 </script>");
}
$faseRestricao = new FaseRestricao();
$tipoRestricao = new TipoRestricao();


extract( $restricao->getDados() );
$rstdtsuperacao = formata_data($rstdtsuperacao);

// Por padrão os campos vem habilitados
$desabilitado   = false;
$somenteLeitura = 'S';
$verifica = true;

$perfis 			= arrayPerfil();
$arPerfisBloqEdicao = array(PERFIL_CONSULTA_UNIDADE,
		PERFIL_CONSULTA_GERAL);

$arInt = array_intersect($perfis, $arPerfisBloqEdicao);

//MÉTODO PARA A VERIFICAR SE O CONTRATO PERTENCE À UNIDADE DO USUARIO E SE O USUARIO PODERÁ EDITAR NESTA TELA
if($_SESSION['ctrid'])
	$verifica = verificaResponsabilidade($_SESSION['ctrid'],$perfis,array(PERFIL_GESTOR_FINANCEIRO_UNIDADE,PERFIL_EQUIPE_TECNICA_UNIDADE));
//verifica se nao esta no bloqueado e se caso nao esteja bloq ele tem direito a editar
if( count($arInt) != 0 || (count($arInt) == 0 && $verifica==false)) {
	$desabilitado 	= true;
	$somenteLeitura = 'N';
}
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script language="JavaScript">
	//Editor de textos
	tinyMCE.init({
		mode : "textareas",
		theme : "advanced",
		plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
		theme_advanced_buttons1 : "undo,redo,separator,bold,italic,underline,separator,justifyleft,justifycenter,justifyright, justifyfull",
		theme_advanced_buttons2 : "",
		theme_advanced_buttons3 : "",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
		language : "pt_br",
		entity_encoding : "raw"
		});
</script>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php
monta_titulo( 'Restrições e Providências', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
?>
<form method="post" id="formulario" name="formulario" onsubmit="return validar(<?php echo $_REQUEST["ctrid"]; ?>);" action="">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">

		<tr>
			<td class="SubTituloDireita" align="right">Tipo de Restrição:</td>
			<td valign="top">
			<?php
				$db->monta_combo( 'tprid', $tipoRestricao->listaCombo(), $somenteLeitura, 'Selecione', '', '', '', '', 'S', 'tprid');   
			?>
			</td>
		</tr>
	    <tr>
	        <td align='right' class="SubTituloDireita">Restrição:</td>
	        <td>
	        <?php $rstdsc = $restricao->rstdsc; ?>
			<?=campo_textarea("rstdsc",'N',$somenteLeitura,'',70,8,'');?>
		    </td>
	    </tr>
	    <tr>
	        <td align='right' class="SubTituloDireita">Previsão da Providência:</td>
	        <td valign="top">
	        <?php $rstdtprevisaoregularizacao = formata_data($restricao->rstdtprevisaoregularizacao); ?>
		    <?= campo_data2( 'rstdtprevisaoregularizacao', 'N', $somenteLeitura, '', 'N' ); ?>
		    </td>
	    </tr>    
	    <tr>
	        <td align='right' class="SubTituloDireita">Providência:</td>
	        <td>
	        <?php $rstdscprovidencia = $restricao->rstdscprovidencia; ?>
			<?=campo_textarea("rstdscprovidencia", 'N', $somenteLeitura, '', 70, 8, '');?>
		    </td>
	    </tr>
	    
	    <?php 
	    if ($_REQUEST["rstid"]):
	    ?>
	    <tr>
	    	<td align='right' class="SubTituloDireita">Restrição superada?</td>
	    	<td colspan="2">
	    		<?php 
	    		if ($rstsituacao == "t"):
	    		?>
	    		<input type="radio" name="rstsituacao" id="rstsituacao" value="true" checked> Sim
	    		<input type="radio" name="rstsituacao" id="rstsituacao" value="false" onclick="document.formulario.rstdtsuperacao.value = '';"> Não
	    		<?php 
	    		else: 
	    		?>
	    		<input type="radio" name="rstsituacao" id="rstsituacao" value="true"> Sim
	    		<input type="radio" name="rstsituacao" id="rstsituacao" value="false" onclick="document.formulario.rstdtsuperacao.value = '';" checked> Não
	    		<?php 
	    		endif; 
	    		?>
	    	 	&nbsp; Se <b>sim</b>, entre com a data: 
	    	 	<?= campo_data2( 'rstdtsuperacao', 'N', 'S', '', 'N' ); ?>
	    	 	<input type="hidden" name="rstid" id="rstid" value="<? echo $_REQUEST["rstid"]; ?>">
	    	 </td>
	    </tr>
		<?php
	    endif;
	    ?>
		
		<tr bgcolor="#C0C0C0">
			<td>&nbsp;</td>
			<td>
				<input type="hidden" name="requisicao" value="salvar"/>
				<input type="hidden" name="subimete" value='2'/>
				<?php if(!$desabilitado){ ?>
					<input type='submit' class='botao' name='Salvar' value='Salvar'/>
				<?php } ?>
				<input type='button' class='botao' value='Voltar' id='btFechar' name='btFechar' onclick='window.opener.location.replace(window.opener.location); window.close();'/>
			</td>			
		</tr>        		
	</table>
</form>

<script type="text/javascript">
function validar(rstid){
	
	var mensagem           = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
	var validacao          = true;
	var tprid              = document.getElementById("tprid").value;
	var rstdsc	           = document.getElementById("rstdsc");
	var rstdscprovidencia = document.getElementById("rstdscprovidencia");

	if (rstid){
		var rstdtsuperacao = document.formulario.rstdtsuperacao.value;
		var rstsituacao	   = document.getElementById("rstsituacao");
	}
	
	rstdsc.value = tinyMCE.getContent("rstdsc");
	var rstdsc_l = rstdsc.value.length;
	
	rstdscprovidencia.value = tinyMCE.getContent("rstdscprovidencia");
	var rstdscprovidencia_l = rstdscprovidencia.value.length;
	
	if (tprid == ""){
		mensagem += 'Tipo de Restrição \n';
		validacao = false;
	}
	
	if (rstdsc.value == ""){
		mensagem += 'Restrição \n';
		validacao = false;
	}
	
	if (rstdsc_l > 500){
		alert("O limite de 500 caracteres foi ultrapassado");
		return false;
	}
	
	if ( rstdscprovidencia_l > 500 ){
		alert('O campo Providência deve ter no máximo 500 caracteres!');
		return false;
	}

	if (document.formulario.rstdtprevisaoregularizacao.value != ""){
		if (!validaData(document.formulario.rstdtprevisaoregularizacao)){
			alert("A data de previsão de regularização informada é inválida");
			document.formulario.rstdtprevisaoregularizacao.focus();
			return false;
		}
	}	
	
	if (rstid){
		if (rstdtsuperacao == "" && rstsituacao.checked == true){
			alert("É necessário informar a data da superação.");
			return false;
		} 
		
		if (rstdtsuperacao != "" && rstsituacao.checked == true){
			if (!validaData(document.formulario.rstdtsuperacao)){
				alert("A data de superação informada é inválida");
				document.formulario.rstdtsuperacao.focus();
				return false;
			}
		}
	}	
	
	if (!validacao){
		alert(mensagem);
	} 
	
	return validacao; 
}

</script>

</body>