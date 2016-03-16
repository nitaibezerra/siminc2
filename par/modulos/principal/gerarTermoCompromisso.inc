<?php
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if( $_POST['requisicao'] == 'gerarTermo' ){
	$obMacro = new AlteraMacroDocumento($_POST['muncod']);
	$obMacro->montaTermo( $_POST );
}

/*if($_REQUEST["submetido"]) {
	if($_REQUEST['mdoid'] != "") {
		
		$sql = "SELECT mdoconteudo FROM par.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = ".$_REQUEST['mdoid'];
		$imitexto = $db->pegaUm($sql);
		
		if($imitexto){
			$obMacro = new AlteraMacroDocumento($preid);
			$obMacro->alteraMacrosDocumento( $imitexto, $preid );
		}else{
			$imitexto = "Não existem informações sobre esta minuta.";
		}
	}
	if($_REQUEST['pmcid'] != "" && !$_REQUEST['mdoid']) {
		$imitexto = $ptminutaconvenio["pmctexto"];
	}
} else {
	if($ptminutaconvenio) {
		$imitexto = $ptminutaconvenio["pmctexto"];
	}
}

$imitexto = str_replace('"', "'", $imitexto);*/

$obPreObraControle = new PreObraControle();

$arObras = array();
if($_POST['pesquisa'] && !empty($_POST['municipio']) ){
	$post = $_POST;
	$arObras = $obPreObraControle->recuperarListaObras($post);
}
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Geração Termo de Compromisso - PAC', '');
$arCabecalho = array("Ações","Nome da obra","Tipo da obra","Município","UF","Situação","Usuário", "Data da Situação", "Analista");
?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="95%">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form method="post" name="formulario" id="formulario">
					<input type="hidden" value="true" id="bogeratermo" name="bogeratermo">
					<input type="hidden" value="" id="requisicao" name="requisicao">
					<table border="0" cellpadding="3" cellspacing="0" width="95%">
						<tr>
							<td valign="bottom">
								Estado
								<br/>
								<?php
									$estuf = $_POST['estuf'];
									$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
									$db->monta_combo("estuf", $sql, 'S', 'Todas as Unidades Federais', 'montaComboMunicipios', '' );
								?>
							</td>
						</tr>
						<tr>
							<?if( $_POST['estuf'] ){ ?>
							<td valign="bottom" align="left">
								Município
								<br/>
								<?php
									$municipio = $_POST['municipio'];
									$sql = "select e.muncod as codigo, e.mundescricao as descricao from territorios.municipio e where e.estuf = '".$_POST['estuf']."' order by e.mundescricao asc";
									$db->monta_combo("municipio", $sql, 'S', 'Selecione um Municípios', '', '', '', '', '', 'municipio' ); 
								?>
							</td>
							<?} ?>
						</tr>
						<tr>
							<td colspan="2">
							Tipo de Obra<br>
								<?php 

								$sql = "select ptoid as codigo, ptodescricao as descricao from obras.pretipoobra order by ptodescricao desc";
								$db-> monta_checkbox('ptoid[]', $sql, $_POST['ptoid']);
								?>															
							</td>
						</tr>
						<tr>
							<td>
								<div style="float: left;">
									<input type="button" name="pesquisar" value="Pesquisar" onclick="pesquisarObras();" />
									<input type="hidden" name="pesquisa" value="1"/>
								</div>
							</td>
						</tr>
						<tr>
							<td>&nbsp;</td>
						</tr>
						<tr>
							<td colspan="10">
							<table width="100%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">
								<thead>
								<tr>
									<?foreach ($arCabecalho as $cabecalho) {?>
									<td valign="top" bgcolor="" align="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
										style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" 
										class="title"><strong><?=$cabecalho; ?></strong></td>
									<?} ?>
								</tr>
								</thead>
								<tbody>
								<?
								if( !empty($arObras) ){
									$disabilita = '';
									foreach ($arObras as $key => $v) {
										$key % 2 ? $cor = "" : $cor = "#FFFFFF";
										$muncod = $v['muncod'];
									?>
										<tr bgcolor="<?=$cor;?>" onmouseout="this.bgColor='<?=$cor;?>';" onmouseover="this.bgColor='#ffffcc';">
											<td title="Ações"><?=$v['acao']; ?></td>
											<td title="Nome da obra"><?=$v['predescricao']; ?></td>
											<td title="Tipo da obra"><?=$v['ptodescricao']; ?></td>
											<td title="Município"><?=$v['mundescricao']; ?></td>
											<td title="UF"><?=$v['estuf']; ?></td>
											<td title="Situação"><?=$v['esddsc']; ?></td>
											<td title="Usuário"><?=$v['usunome']; ?></td>
											<td title="Data da Situação"><?=$v['htddata']; ?></td>
											<td title="Analista"><?=$v['nomeanalista']; ?></td>
										</tr>
									<?}
									echo '<tr><td colspan="10"><b>Total de Registros: '.sizeof($arObras).'</b>
												<input type="hidden" value="'.$muncod.'" id="muncod" name="muncod"></td>
										 </tr>';
								} else {
									$disabilita = 'disabled="disabled"';?>
									<tr>
										<td bgcolor="#FFFFFF" align="center" style="color: rgb(204, 0, 0);" colspan="10">Não foram encontrados Registros.</td>
									</tr>
								<?} ?>
								</tbody>
							</table>
							</td>
						</tr>
						<tr>
							<td>&nbsp;</td>
						</tr>
						<tr>
							<td colspan="2" id="combo_objeto">
								Informe Nº da Resolução<br>
								<?php
								$sql = "SELECT 
										  resid as codigo,
										  resnumero ||' - '||to_char(resdatapublicacao, 'DD/MM/YYYY') as descricao
										FROM 
										  par.resolucao
										WHERE resstatus = 'A'";
								
								/*combo_popup('resid', $sql, '', '400x400', 0, array(), '', 'S', false, false, 05, 350 );
								echo obrigatorio();*/
								$db->monta_combo("resid", $sql, 'S', 'Selecione...', '', '', '', '450', '', 'resid', '', '', '' );
								?>
							</td>
						</tr>
						<tr>
							<td><input type="button" name="btnGerar" <?=$disabilita; ?> id="btnGerar" value="Gerar Termo" onclick="gerarTemo();"></td>
						</tr>
					</table>
				</form>
			</td>
		</tr>
	</tbody>
</table>
<script type="text/javascript">
	function montaComboMunicipios( estuf ){
		document.getElementById('formulario').submit();
	}
	function pesquisarObras(){
		if( document.getElementById('municipio').value == '' ){
			alert('O campo "Município" é de preenchimento obrigatório!');
			document.getElementById('municipio').focus();
			return false;
		}
		
		document.getElementById('formulario').submit();
	}
	function gerarTemo(){
		var form = document.getElementById( 'formulario' );
		
		//selectAllOptions( document.getElementById( 'resid' ) );
		var mensagem  = 'Os seguintes campos devem ser preenchidos: \n\n';
		var validacao = true;
		var resid	  = document.getElementById("resid");

		//var j = resid.options.length;
		/*if( resid.options[0].value == '' ){
			mensagem += 'Nº da Resolução \n';
			validacao = false;	
		}*/
		
		if( resid.value == '' ){
			mensagem += 'Nº da Resolução \n';
			validacao = false;	
		}
		
		var preid = document.getElementsByName("preid[]");
		var boSeleciona = false;	
		for(var i=0; i<preid.length; i++) {
			if( preid[i].checked == true ){
				boSeleciona = true;
			}
		}
		
		if( !boSeleciona ){
			mensagem += 'Selecionar uma Obra \n';
			validacao = false;
		}
		
		if ( !validacao ){
			alert( mensagem );
			return validacao;
		}else{
			document.getElementById('requisicao').value = 'gerarTermo';
			form.submit();
		}
	}
	
	function onOffCampo( campo ){
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		
		if ( div_on.style.display == 'none' ){
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}else{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}
</script>
<?
//$arCabecalho = array("Ações","Nome da obra","Tipo da obra","Município","UF","Situação","Usuário", "Data da Situação", "Analista");
//$db->monta_lista_array($arObras, $arCabecalho, 20, 10, 'N', '');
/*?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<div id="erro"></div>
</body>
<script type="text/javascript">
var btSalvar	= document.getElementById("bt_salvar");
var btCancelar	= document.getElementById("bt_cancelar");
var form		= document.getElementById("formulario");

document.getElementById('texto').value = document.getElementById('imitexto').value;

tinyMCE.init({
		// General options
		mode : "textareas",
		theme : "advanced",
		language: "pt",
		editor_selector : "emendatinymce",
		plugins : "safari,pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template,wordcount",

		// Theme options
		theme_advanced_buttons1 : "bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,|,formatselect,fontselect,fontsizeselect",
		theme_advanced_buttons2 : "cut,copy,paste,pastetext,pasteword,|,search,replace,|,bullist,numlist,|,outdent,indent,blockquote,|,undo,redo,|,link,unlink,anchor,image,cleanup,help,code,|,insertdate,inserttime,preview,|,forecolor,backcolor",
		theme_advanced_buttons3 : "tablecontrols,|,hr,removeformat,visualaid,|,sub,sup,|,charmap,emotions,iespell,media,advhr,|,print,|,ltr,rtl,|,fullscreen",
		theme_advanced_buttons4 : "insertlayer,moveforward,movebackward,absolute,|,styleprops,|,cite,abbr,acronym,del,ins,attribs,|,visualchars,nonbreaking,template,pagebreak",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		theme_advanced_statusbar_location : "",
		theme_advanced_resizing : true,

		// Example content CSS (should be your site CSS)
		content_css : "css/content.css",

		// Drop lists for link/image/media/template dialogs
		template_external_list_url : "lists/template_list.js",
		external_link_list_url : "lists/link_list.js",
		external_image_list_url : "lists/image_list.js",
		media_external_list_url : "lists/media_list.js",

		// Replace values for the template plugin
		template_replace_values : {
			username : "Some User",
			staffid : "991234"
		}
	});

function salvarTermoCompromisso() {
	$('bt_salvar').disabled 	= true;
	$('bt_cancelar').disabled 	= true;
	document.getElementById("submetido").value = "1";
	document.getElementById("salvar1").value = "1";
	form.submit();
}

function carregaMinutaIniciativa() {
	var mdoid = document.getElementById('modelo').value;
	document.getElementById("submetido").value = "1";
	document.getElementById("mdoid").value = mdoid;
	$('bt_salvar').disabled 	= true;
	$('bt_cancelar').disabled 	= true;
	if( mdoid ){
		form.submit();
	}
}
</script>
*/
?>