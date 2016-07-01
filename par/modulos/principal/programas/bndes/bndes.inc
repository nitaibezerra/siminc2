<?php

// Faz download de um dos arquivos solicitados
if( $_REQUEST["arquivo_login"] )
{
	// caminho do arquivo
	$path = APPRAIZ . "www/par/documentos/arquivos/";
	// recupera o nome e o tipo do arquivo
	switch($_REQUEST["arquivo_login"])
	{
		case 'formulario1':
			$file = "FRO_-_simplificada.xls";
			$type = "application/xls";
			break;
		case 'formulario2':
			$file = "ROBI_-_simplificada.doc";
			$type = "application/doc";
			break;
		case 'resumo1':
			$file = "PMAT_Automático_-_resumo.docx";
			$type = "application/doc";
			break;
		case 'resumo2':
			$file = "PMAT_-_resumo.docx";
			$type = "application/doc";
			break;
		case 'resumo3':
			$file = "PMAE_-_resumo.docx";
			$type = "application/doc";
			break;
		case 'texto1':
			$file = "PMAT_Automático_-_texto_orientador.docx";
			$type = "application/doc";
			break;
		case 'texto2':
			$file = "PMAT_-_texto_orientador.docx";
			$type = "application/doc";
			break;
		case 'texto3':
			$file = "PMAE_-_texto_orientador.docx";
			$type = "application/doc";
			break;
	}

	// caminho completo
	$file = $path . $file;
	//ver($file, d);
	// cabeçalho
	header("Content-type: $type");
	header("Content-Disposition: attachment;filename=$file");
	// mostra o download
	readfile($file);
	// destrói a variável do formulário
	unset($_REQUEST["formulario"]);
	exit;
}

include_once APPRAIZ . 'includes/cabecalho.inc';

echo '<br>';
monta_titulo( 'Programa BNDES', '' );

global $db;

if( $_SESSION['par']['itrid'] == 1 ){ // Estado
	$dado  = 'Estado';
	$dado2 = $_SESSION['par']['estuf'];
	$pop = '-';
	$populacao = 0;
} else { // Município
	$dado = 'Município';
	$sql = "SELECT mundescricao, munpopulacao FROM territorios.municipio WHERE muncod = '{$_SESSION['par']['muncod']}'";
	$mun = $db->pegaLinha( $sql );
	$dado2 = $mun['mundescricao'] .' - '. $_SESSION['par']['estuf'];
	$pop = $mun['munpopulacao'] . ' habitantes';
	$populacao = $mun['munpopulacao'];
}

if( $_POST['carregaAnexos'] ){
	$programa = '-';
	$forma = '-'; 
	$formulario = "-";
	$resumo = "-";
	$texto = "-";
	if( $_SESSION['par']['itrid'] == 2 ){ //Município
		if( $_POST['carregaAnexos'] == 1 ){
			if( $populacao < 150000 ){ // 1º caso
				$programa = "PMAT Automático";
				$forma = "Indireta e Automática"; 
				$formulario = "<a href=\"javascript:void(0);\" onclick=\"abreArquivo('formulario1');\">FRO <font color=\"red\">(Clique para Download)</font></a>";
				$resumo = "<a href=\"javascript:void(0);\" onclick=\"abreArquivo('resumo1');\">PMAT Automático <font color=\"red\">(Clique para Download)</font></a>";
				$texto = "<a href=\"javascript:void(0);\" onclick=\"abreArquivo('texto1');\">PMAT Automático <font color=\"red\">(Clique para Download)</font></a>";
			} else { // 3º caso
				if( $_POST['valoroperacao'] != 0 ){
					$programa = "PMAT";
					$forma = "Indireta"; 
					$formulario = "<a href=\"javascript:void(0);\" onclick=\"abreArquivo('formulario2');\">ROBI <font color=\"red\">(Clique para Download)</font></a>";
					$resumo = "<a href=\"javascript:void(0);\" onclick=\"abreArquivo('resumo2');\">PMAT <font color=\"red\">(Clique para Download)</font></a>";
					$texto = "<a href=\"javascript:void(0);\" onclick=\"abreArquivo('texto2');\">PMAT <font color=\"red\">(Clique para Download)</font></a>";
				}
			}
		} else {
			if( $populacao < 150000 ){ // 2º caso
				$programa = "PMAT";
				$forma = "Direta ou Indireta"; 
				$formulario = "<a href=\"javascript:void(0);\" onclick=\"abreArquivo('formulario2');\">ROBI <font color=\"red\">(Clique para Download)</font></a>";
				$resumo = "<a href=\"javascript:void(0);\" onclick=\"abreArquivo('resumo2');\">PMAT <font color=\"red\">(Clique para Download)</font></a>";
				$texto = "<a href=\"javascript:void(0);\" onclick=\"abreArquivo('texto2');\">PMAT <font color=\"red\">(Clique para Download)</font></a>";
			} else { // 4º caso
				$programa = "PMAT";
				$forma = "Direta ou Indireta"; 
				$formulario = "<a href=\"javascript:void(0);\" onclick=\"abreArquivo('formulario2');\">ROBI <font color=\"red\">(Clique para Download)</font></a>";
				$resumo = "<a href=\"javascript:void(0);\" onclick=\"abreArquivo('resumo2');\">PMAT <font color=\"red\">(Clique para Download)</font></a>";
				$texto = "<a href=\"javascript:void(0);\" onclick=\"abreArquivo('texto2');\">PMAT <font color=\"red\">(Clique para Download)</font></a>";
			}
		}
	} else { // Estado
		if( $_POST['carregaAnexos'] == 1 ){
			$programa = "PMAE";
			$forma = "Direta"; 
			$formulario = "<a href=\"javascript:void(0);\" onclick=\"abreArquivo('formulario2');\">ROBI <font color=\"red\">(Clique para Download)</font></a>";
			$resumo = "<a href=\"javascript:void(0);\" onclick=\"abreArquivo('resumo3');\">PMAE <font color=\"red\">(Clique para Download)</font></a>";
			$texto = "<a href=\"javascript:void(0);\" onclick=\"abreArquivo('texto3');\">PMAE <font color=\"red\">(Clique para Download)</font></a>";
		}
	}
}

?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script>

	var countModal = 1;

	function montaShowModal() {
		var campoTextArea = ''+
			'<p align=\'justify\'>Informamos que os cadastros Secretaria, Escola e Unidade Local Integrada estão abertos para preenchimento com os dados do PSE relativos ao ano/exercício de 2010.'+
			'<br /><br />Ao acessar o SIMEC/PSE, verifique na tela do monitor, na parte de cima à direita, que o sistema disponibiliza o ano/exercício do programa. Para acessar outros anos/exercícios do programa, basta trocar o ano antes de visualizar as informações.'+
			'<br /><br />Atenção! As Secretarias e Escolas dos Municípios da Portaria nº 2.931 de 4 de dezembro de 2008, terão até o dia 31 de dezembro de 2010, para registrar as informações relativas ao ano/exercício 2009.'+
			'<br /><br />Contamos com seu compromisso!'+
			'<br /><br />Gratos,'+
			'<br /><br />Equipe PSE.';
		var alertaDisplay = '<div class="titulo_box" >Prezado usuário do SIMEC/PSE, <br/ >'+campoTextArea+'</div><div class="links_box" ><center><input type="button" onclick=\'closeMessage(1); return false \' value="OK" />&nbsp;&nbsp;<input type="button" onclick=\'closeMessage(2); return false \' value="Cancelar" /></center>';
		displayStaticMessage(alertaDisplay,false);
		return false;
	}

	function displayStaticMessage(messageContent,cssClass) {
		messageObj = new DHTML_modalMessage();	// We only create one object of this class
		messageObj.setShadowOffset(5);	// Large shadow
		
		messageObj.setHtmlContent(messageContent);
		messageObj.setSize(520,300);
		messageObj.setCssClassMessageBox(cssClass);
		messageObj.setSource(false);	// no html source since we want to use a static message here.
		messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
		messageObj.display();
	}
	
	function closeMessage(alt){
		messageObj.close();
		if( alt == 2 ){
			window.location.href='par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';
		} else {	
			$("[name*='sessaomostramodal']").val('S');
			$("#form_bndes").submit();
		}
	}
	
	function verificaSeMostraShowModal(){
		var mostraShowModal = 'S';
		if(mostraShowModal){
			montaShowModal();	
		}
	}

	function enviarFormulario(){
		if( $("[name*='valoroperacao']").val() ){
			valor = $("[name*='valoroperacao']").val();
			valor = valor.replace(/\./gi, "");
			valor = valor.replace(/\,/gi, ".");
			if( $("[name*='itrid']").val() == 2 ){ //Município
				if( valor < 10000000.01 ){
					$("[name*='carregaAnexos']").val(1);
				}
				if( valor > 10000000.00 ){
					$("[name*='carregaAnexos']").val(2);
				}
			} else { //Estado
				if( valor > 1000000.00 ){
					$("[name*='carregaAnexos']").val(1);
				} else {
					$("[name*='carregaAnexos']").val(3);
				}
			}
		} else {
			alert( 'Preencha o valor de operação.' );
			return false;
		}
		$("#form_bndes").submit();
	}
	
	function abreArquivo(arq)
	{
		var form	= document.getElementById("form_bndes");
		var arquivo = document.getElementById("arquivo_login");

		arquivo.value = arq;
		form.submit();
	}
</script>
<form name="form_bndes" id="form_bndes" method="post" action="" >
<input type="hidden" name="carregaAnexos" id="carregaAnexos" value="">
<input type="hidden" name="itrid" id="itrid" value="<?=$_SESSION['par']['itrid'] ?>">
<input type="hidden" id="arquivo_login" name="arquivo_login" value="" />
<input type="hidden" id="sessaomostramodal" name="sessaomostramodal" value="" />
<?php

if( $_POST['sessaomostramodal'] ){
	$_SESSION['par']['bndes']['aceite'] = $_POST['sessaomostramodal'];
}

if( $_SESSION['par']['bndes']['aceite'] == 'S' ){
?>
	<table cellspacing="0" cellpadding="3" border="0" align="center" class="tabela">
		<tr>
			<td class="SubTituloDireita" width='20%'><?=$dado ?>:</td>
			<td><?=$dado2 ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">População:</td>
			<td><?=$pop ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Valor de Operação (R$):</td>
			<td><?php
				echo campo_texto('valoroperacao', 'N', 'S', 'valor operacao', 20, 50, '[.###],##', '', '', '', 0, '', '', $_POST['valoroperacao']);
			?></td>
		</tr>
		<?php
			if( $_POST['carregaAnexos'] ){
		?>
			<tr>
				<td bgcolor="#DCDCDC" colspan="2" align="center"><b>Anexos</b></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width='20%'>Programa BNDES:</td>
				<td><?=$programa ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Forma de Apoio:</td>
				<td><?=$forma ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width='20%'>Formulário:</td>
				<td><?=$formulario ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Resumo:</td>
				<td><?=$resumo ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Texto Orientador:</td>
				<td><?=$texto ?></td>
			</tr>
			<tr>
				<td bgcolor="#DCDCDC" colspan="2" align="center"><b>Sites Institucionais</b></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">PMAT Automático:</td>
				<td><a href="http://www.bndes.gov.br/SiteBNDES/bndes/bndes_pt/Institucional/Apoio_Financeiro/Programas_e_Fundos/PMAT/pmat_automatico.html" target="janela1">http://www.bndes.gov.br/SiteBNDES/bndes/bndes_pt/Institucional/Apoio_Financeiro/Programas_e_Fundos/PMAT/pmat_automatico.html</a></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">PMAT:</td>
				<td><a href="http://www.bndes.gov.br/SiteBNDES/bndes/bndes_pt/Institucional/Apoio_Financeiro/Produtos/FINEM/pmat.html" target="janela2">http://www.bndes.gov.br/SiteBNDES/bndes/bndes_pt/Institucional/Apoio_Financeiro/Produtos/FINEM/pmat.html</a></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">PMAE:</td>
				<td><a href="http://www.bndes.gov.br/SiteBNDES/bndes/bndes_pt/Institucional/Apoio_Financeiro/Produtos/FINEM/pmae.html" target="janela3">http://www.bndes.gov.br/SiteBNDES/bndes/bndes_pt/Institucional/Apoio_Financeiro/Produtos/FINEM/pmae.html</a></td>
			</tr>
		<?php
			} else {
		?>
		<tr>
			<td class="SubTituloDireita"></td>
			<td class="SubTituloEsquerda" >
				<input type="button" value="Enviar" name="btn_enviar" onclick="enviarFormulario()" />
			</td>
		</tr>
		<?php } ?>
	</table>
	</form>
<?php } else { ?>
<script>
	montaShowModal();
</script>
<?php } ?>