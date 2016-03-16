<?php
/* Tratamento de segurança */
include APPRAIZ . "rehuf/classes/Certificado.class.inc";

if (validaVariaveisSistema ()) {
	echo "<script>
			alert('Problemas nas variáveis do sistema.');
			window.location='?modulo=inicio&acao=A';
		  </script>";
	exit ();
}
$permissoes = verificaPerfilRehuf ();
validaAcessoHospital ( $permissoes ['verhospitais'], $_SESSION ['rehuf_var'] ['entid'] );
// Validando os acessos as funções autorizadas
switch ($_REQUEST ['acao']) {
	case 'A' :
		$_FUNCOESAUTORIZADAS = array (
				'adicionarCertificado' => true
		);
		break;
}

if($_REQUEST['requisicao'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicao']();
}


if ($_REQUEST ['requisicaoAjax'] && $_REQUEST ['classe']) {
	$n = new $_REQUEST ['classe'] ();
	$n->$_REQUEST ['requisicaoAjax'] ();
	exit ();
}
extract($_POST);
/* FIM - Tratamento de segurança */

echo '<br>';

/* montando cabeçalho */
monta_titulo ( "REHUF", "Programa Nacional de Re-estruturação dos Hospitais Universitários Federais" );
monta_cabecalho_rehuf ( $_SESSION ['rehuf_var'] ['entid'] );

/* Tela de gerenciamento dos dados especificos */
// Buscando os dados especificos do HOSPITAL
$dadosestruturaunidade = $db->pegaLinha ( "SELECT * FROM rehuf.estruturaunidade WHERE entid = '" . $_SESSION ['rehuf_var'] ['entid'] . "'" );
$esutipo = $dadosestruturaunidade ['esutipo'];
$esuareacontruida = number_format ( $dadosestruturaunidade ['esuareacontruida'], 2, ',', '.' );
$esucontratualizado = (($dadosestruturaunidade ['esucontratualizado'] == "t") ? "sim" : "nao");
$esucertiticado = (($dadosestruturaunidade ['esucertiticado'] == "t") ? "sim" : "nao");
$esugestao = $dadosestruturaunidade ['esugestao'];
$esudataadereebserh = formata_data ( $dadosestruturaunidade ['esudataadereebserh'] );

if($_REQUEST['tipo'] == "1"){
	$titulo = "Adicionar Certificado - Hospital de Ensino";
} elseif ($_REQUEST['tipo'] == "2"){
	$titulo = "Adicionar Certificado - Hospital Amigo da Criança";
} elseif ($_REQUEST['tipo'] == "3"){
	$titulo = "Adicionar Acreditação/Certificação de Qualidade";
} else {
	$titulo = "Adicionar Outros Certificados/Certificações";
}
?>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<script type="text/javascript">
	function salvarCertificado(tipo){
		$("#requisicao").val("adicionarCertificado");
		$("#classe").val("Certificado");
		$("#formulario").submit();
		alert("Operação realizada com sucesso.");
		opener.location.reload();
	}

	function filtraTR(id){
		if(id == 'Accreditation Canada International' || id == 'Compromisso com a Qualidade Hospitalar (CQH)' || id == 'Joint Commission International' || id == 'Outras'){
			escondeTR();
			$('#tr_mes').show();
			$('#tr_obs').show();
		} else if(id == 'ISO 9001:2008'){
			escondeTR();
			$('#tr_area').show();
			$('#tr_mes').show();
			$('#tr_obs').show();
		} else if(id == 'Organização Nacional de Acreditação'){
			escondeTR();
			$('#tr_mes').show();
			$('#tr_nivel').show();
			$('#tr_obs').show();
		} else {
			escondeTR();
		}
	}

	function escondeTR(){
		$('#tr_mes').hide();
		$('#tr_area').hide();
		$('#tr_nivel').hide();
		$('#tr_obs').hide();
	}
</script>

<form id="formulario" name='formulario' action='' method='post'>
	<input type="hidden" name="classe" id="classe" value="Certificado"/>
	<input type="hidden" name="requisicao" id="requisicao" value=""/>
	<input type="hidden" name="tipo" id="tipo" value="<?php echo $_REQUEST['tipo']?>"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1"
		align="center">
		<tr>
			<td>
				<center>
					<label class="TituloTela" style="color: #000000;"><?php echo $titulo?></label>
				</center>
			</td>
		</tr>
		<?php if ($_REQUEST['tipo'] == "1"):?>
			<tr height="100px">
	        	<td class="SubTituloDireita" >Portaria de Homologação</td>
				<td><?php echo campo_texto('cerdsc', 'N', 'S', '', 50, 255, '', '', 'left', '', 0, 'id="cerdsc"', ''); ?></td>
			</tr>
			<tr height="100px">
	        	<td class="SubTituloDireita" >Data de Publicação</td>
				<td><?php echo campo_data2('cerdata', 'N', 'S', '', 'S' )?></td>
			</tr>
			<tr height="100px">
	        	<td class="SubTituloDireita" >Observação</td>
				<td><?php echo campo_textarea( "cerobs", "S", $habilitado, '', 80, 5, null); ?></td>
			</tr>
		<?php elseif ($_REQUEST['tipo'] == "2"):?>
			<tr height="100px">
	        	<td class="SubTituloDireita" >Portaria de Homologação</td>
				<td><?php echo campo_texto('cerdsc', 'N', 'S', '', 50, 255, '', '', 'left', '', 0, 'id="cerdsc"', ''); ?></td>
			</tr>
			<tr height="100px">
	        	<td class="SubTituloDireita" >Data de Publicação</td>
				<td><?php echo campo_data2('cerdata', 'N', 'S', '', 'S' )?></td>
			</tr>
			<tr height="100px">
	        	<td class="SubTituloDireita" >Observação</td>
				<td><?php echo campo_textarea( "cerobs", "S", $habilitado, '', 80, 5, null); ?></td>
			</tr>
		<?php elseif ($_REQUEST['tipo'] == "3"):?>
			<tr height="100px">
	        	<td class="SubTituloDireita" >Nome</td>
				<td>
					<?php 
						$certificados = new Certificado();
						$arrCertificados = $certificados->listaCertificados();
						$db->monta_combo("cerdsc", $arrCertificados, 'S', "Selecione...", 'filtraTR(this.value)', '', '', '', 'S', 'cerdsc');
					?>
				</td>
			</tr>
			<tr height="100px" style="display:none" id="tr_mes">
	        	<td class="SubTituloDireita" >Mês/Ano de Obtenção</td>
				<td><?php echo campo_data2('cerdata', 'N', 'S', '', 'S' )?></td>
			</tr>
			<tr height="100px" style="display:none" id="tr_area">
	        	<td class="SubTituloDireita" >Área Certificada</td>
				<td>
					<?php echo campo_texto('cerarea', 'N', 'S', '', 50, 255, '', '', 'left', '', 0, 'id="cerarea"', ''); ?>
				</td>
			</tr>
			<tr height="100px" style="display:none" id="tr_nivel">
	        	<td class="SubTituloDireita" >Nível</td>
				<td>
					<?php 
						$certificados = new Certificado();
						$arrNiveis = $certificados->listaNiveis();
						$db->monta_combo("cernivel", $arrNiveis, 'S', "Selecione...", '', '', '', '', 'S', 'cernivel');
					?>
				</td>
			</tr>
			<tr height="100px" style="display:none" id="tr_obs">
	        	<td class="SubTituloDireita" >Observação</td>
				<td><?php echo campo_textarea( "cerobs", "S", $habilitado, '', 80, 5, null); ?></td>
			</tr>
		<?php else:?>
			<tr height="100px">
	        	<td class="SubTituloDireita" >Nome</td>
				<td><?php echo campo_texto('cerdsc', 'N', 'S', '', 50, 255, '', '', 'left', '', 0, 'id="cerdsc"', ''); ?></td>
			</tr>
			<tr height="100px">
	        	<td class="SubTituloDireita" >Mês/Ano de Obtenção</td>
				<td><?php echo campo_data2('cerdata', 'N', 'S', '', 'S' )?></td>
			</tr>
			<tr height="100px">
	        	<td class="SubTituloDireita" >Observação</td>
				<td><?php echo campo_textarea( "cerobs", "S", $habilitado, '', 80, 5, null); ?></td>
			</tr>
		<?php endif;?>
		<tr>
			<td>
				<input type="button" value="Adicionar" onclick="salvarCertificado(<?php echo $_REQUEST['tipo'];?>)" />
				<input type='button' id='btnsubmit' value='Fechar' onclick="javascript:self.close()">
			</td>
		</tr>
	</table>
</form>