<?php
include APPRAIZ."fabrica/classes/ContratoSistema.class.inc";

$ctsid = $_REQUEST['ctsid'];

if($_REQUEST['requisicao']){
	$cs = new ContratoSistema($ctsid);
	$cs->popularDadosObjeto($_REQUEST);
	
	if($_POST['ctsid'] && $_POST['ctsid'] != "")
		$cs->$_REQUEST['requisicao']();
	else
		$ctsid = $cs->$_REQUEST['requisicao']();
	
	if($_POST['prjproduto'][0]){
		$cs->deletarProdutos($ctsid);
		$cs->inserirProdutos($ctsid,$_POST['prjproduto']);
		unset($_POST['prjproduto']);
	}
		
	$cs->commit();
	
	$msg = "Operação Realizada com Sucesso!";
}

if($ctsid){
	$cs = new ContratoSistema($ctsid);
	$arrDados = $cs->getDados();
	extract($arrDados);
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
$titulo = "Cadastro Sistema / Contrato";
monta_titulo( $titulo, obrigatorio().'Indica campo obrigatório.' );

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<style>
	.TituloTabela{background-color: #C5C5C5;font-weight:bold}
	.center{text-align:center}
	.link{cursor: pointer;}
	.middle{vertical-align: middle;}
</style>
<script>
	function limparCampos()
	{
		$('input:not(:hidden,:button,[readonly=""],[readonly="readonly"]),textarea,select').val("");
	}
	
	function salvarSistema()
	{
		if(validaFormulario()){
			selectAllOptions( document.getElementById('prjproduto') );
			$('#form_cad_sistema').submit();
		}
	}
	
	function validaFormulario()
	{
		var erro = 0;
		$("[class~=obrigatorio]").each(function() { 
			if(!this.value){
				erro = 1;
				alert('Favor preencher todos os campos obrigatórios!');
				this.focus();
				return false;
			}
		});
		
		if(erro == 0){
			return true;
		}
	}	
</script>
<form name="form_cad_sistema" id="form_cad_sistema" method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubtituloDireita">Sistema:</td>
			<td>
				<?php $sql = "	select
									sidid as codigo,
									sidabrev ||' - '|| siddescricao as descricao
								from
									demandas.sistemadetalhe
								where
									sidstatus = 'A'
								order by
									2"; ?>
				<?=$db->monta_combo("sidid",$sql,"S","Selecione...","","","","","S") ?>
				<input type="hidden" name="requisicao" value="salvar" />
				<input type="hidden" name="ctsid" value="<?=$ctsid?>" />
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Contrato:</td>
			<td>
				<?php $sql = "	select
									ctrid as codigo,
									ctrnumero as descricao
								from
									fabrica.contrato
								where
									ctrstatus = 'A'
								order by
									ctrnumero"; ?>
				<?=$db->monta_combo("ctrid",$sql,"S","Selecione...","","","","","S") ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Produtos Esperados:</td>
			<td>
			<?php
			$sql = "select 
						prdid as codigo,
						prddsc as descricao
					from 
						fabrica.produto
					where
						prdstatus = 'A'
					order by
						prddsc desc"; 
			
			$cs ? $prjproduto = $cs->recuperarProdutos($ctsid) : "";
			
			combo_popup("prjproduto",$sql,"Selecione o(s) Produtos(s)");
			?>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="TituloTabela center">
				<input type="button" onclick="salvarSistema()" value="Salvar" >
				<input type="button" onclick="limparCampos()" value="Limpar Campos" >
			</td>
		</tr>
	</table>
</form>
<?php if($msg): ?>
<script>
	alert('<?php echo $msg ?>');
</script>
<?php endif; ?>