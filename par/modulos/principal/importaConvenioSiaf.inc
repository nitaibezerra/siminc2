<?php
include_once APPRAIZ.'par/classes/importaConvenioSiaf.class.php';

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$boImportaConvenio = new importaConvenio();
set_time_limit(0);
// pega o endereço do diretório atual
//getcwd();
if($_POST['carrega'] == "insere"){
	if( $_FILES["arquivo"]['tmp_name'] ){
		$boImportaConvenio->importar( $_FILES["arquivo"]['tmp_name'] );
		echo '<script type="text/javascript"> alert("Operação realizada com sucesso!");</script>';
	} else {
		echo '<script type="text/javascript"> alert("Você deve escolher um arquivo.!");</script>';
	}
}
if($_POST['carrega'] == "update"){
	$sql = "UPDATE par.processopar p SET prpnumeroconveniosiafi = ds.it_nu_convenio
			FROM par.dadosconveniosiafi ds 
				inner join par.processopar pp ON pp.prpnumeroconveniofnde = substring(ds.it_nu_original, 1,6) and substring(ds.it_nu_original, 8,11) = prpanoconveniofnde::text
					and pp.prpnumeroconveniofnde is not null
					-- AND (pp.prpnumeroconveniosiafi is null or pp.prpnumeroconveniosiafi = '')
			WHERE
				p.prpnumeroconveniofnde = pp.prpnumeroconveniofnde
				and p.prpanoconveniofnde = pp.prpanoconveniofnde
				AND p.prpid = pp.prpid";
	
	$db->executar($sql);
	
	$sql = "update par.dadosconveniosiafi set dscstatus = 'I', dscdataexclusao = now() where dscstatus = 'A'";
	$db->executar($sql);
	
	$db->commit();
	$db->sucesso('principal/importaConvenioSiaf');
}
monta_titulo( 'Importa Convênio SIAFI', '' );

?>
<html>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<body marginwidth="0" marginheight="0" bgcolor="#ffffff" leftmargin="0" topmargin="0">
	<form id="formulario" name="formulario" action="#" method="post" enctype="multipart/form-data" >
		<input type="hidden" name="carrega" id="carrega" value="1">
				
		<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="SubTituloDireita" style="width:19.5%;">Arquivos:</td>
				<td><input type="file" name="arquivo" id="arquivo" value="" size="100"></td>
			</tr>
			<tr>
				<td colspan="2" style="text-align: center;">
					<input type="button" name="btnCarregar" id="btnCarregar" value="Carregar dados do Arquivo" onclick="carregaArquivo();">
					&nbsp;&nbsp;&nbsp;<input type="button" name="btnUpdate" id="btnUpdate" value="Atualizar N° convênio no <?php echo SIGLA_SISTEMA; ?>" onclick="atualizarConvenio();">
				</td>
			</tr>
			<tr>
				<td colspan="2" style="text-align: center;"><div id="lista"></div></td>
			</tr>
		</table>
	</form>
	<?
	$boImportaConvenio->carregaRegistroConvenio();
	?>
</body>
<script type="text/javascript">
	function carregaArquivo(){
		/*var myajax = new Ajax.Request('importaConvenioSiaf.php', {
			        method:     'post',
			        parameters: '&carrega=true',
			        asynchronous: false,
			        onComplete: function (res){
						$('lista').innerHTML = res.responseText;
			        }
			  });*/
			  $('carrega').value = "insere";
			  $('formulario').submit();
	}
	
	function atualizarConvenio(){
		$('carrega').value = "update";
		$('formulario').submit();
	} 
</script>
</html>