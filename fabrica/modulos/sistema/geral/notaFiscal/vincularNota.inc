<?php
include APPRAIZ."fabrica/classes/NotaFiscalMemorando.class.inc";

$nfsid = $_REQUEST['nfsid'];

//verifica se o combo memorando será somente leitura 
//se for inclusão o combo fica habilitado, se for alteração combo fica desabilitado
if($nfsid){
	$comboMemorandoReadOnly = 'N';
	$disabled = 'disabled="disabled"';
}else{
	$comboMemorandoReadOnly = 'S';
	$disabled = '';
}

if($_REQUEST['requisicao']){
	$nf = new NotaFiscalMemorando($nfsid);
	$nf->popularDadosObjeto($_REQUEST);
	
	if($_POST['nfsid'] && $_POST['nfsid'] != ""){
		$nf->$_REQUEST['requisicao']();
	}
	else{
		if($nf->$_REQUEST['requisicao']() == false){
			$msg_erro = true;
		}
	}
	/*if($_POST['datacadastro']){
		//$nfm->atualizarDataCadastro($memoid,$_POST['datacadastro']);
		//unset($_POST['datacadastro']);
	}*/
	
	$nf->commit();
	// retorna mensagem de acordo com operação (salvar ou atualizar)
	
	
	if($msg_erro == true){
		$msg = 'Esta nota fiscal já possui um memorando vinculado';
		$retorno = true;
	}else{
		switch ($comboMemorandoReadOnly)
		{
			case('S'):
				$msg = 'Dados salvo com sucesso';
				break;
					
			case('N'):
				$msg = 'Dados alterados com sucesso';
				break;
					
			default:
				$msg = uft8_encode("Operação Realizada com Sucesso!");
				break;
		}
	}
}

if($nfsid){
	$nf = new NotaFiscalMemorando($nfsid);
	$arrDados = $nf->getDados();
	extract($arrDados);
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
$titulo = "Memorando / Notas Fiscais";
monta_titulo( $titulo, obrigatorio().'Indica campo obrigat&oacute;rio.' );

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

	function cancelarOperacao()
	{
		alert('Operação cancelada!');
		window.location.href = 'fabrica.php?modulo=sistema/geral/notaFiscal/listarVinculo&acao=A';
	}
	
	function salvarVinculo()
	{
		if(validaFormulario()){
			$('#form_cad_sistema').submit();
		}
	}
	
	function validaFormulario()
	{
		var erro = 0;
		$("[class~=obrigatorio]").each(function() { 
			if(!this.value){
				erro = 1;
				
				alert('Preencher os campos obrigatórios!');
								
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
			<td width="25%" class="SubtituloDireita">Nota Fiscal:</td>
			<td>
			<?php
				$nf ? $valorCampo = $nf->recuperarNfDescricao($nfsid) : "";
			?>
				<input class='CampoEstilo obrigatorio' type="text" name="nfsdsc" id="nfsdsc" value="<?=$valorCampo['nfsdsc']?>" <?=$disabled?>>
				<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />

				<input type="hidden" name="requisicao" value="salvar" />
				<?php
					if($nfsid){
						echo '<input type="hidden" name="nfsid" value="'.$nfsid.'" />';
					}
				?>	
				
				<input type="hidden" name="nfsstatus" value="A" /> <!-- <= retirar essa linha após remoção do campo nfsstatus do banco -->
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Memorando:</td>
			<td>
				<?php
				if(!$nfsid)
				{
					$nfsid = 0;
				}else{
					$memoid = $nf->recuperarValorSelecionadoCombo($nfsid);
				}
								
                $sql = "select
                		    memoid as codigo,
                			memonumero::varchar || '/' || to_char(memodata, 'YYYY')  as descricao
                		from
                		    fabrica.memorando
                		where
                		    nfsid is null 
                		    or nfsid = $nfsid
                        order by
                            memoid desc";?>
									
					
				<?=$db->monta_combo("memoid",$sql,"","Selecione...","","","","","S") ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Data do Cadastro:</td>
			<td>
			<?php
				if($nf){
					$nfsdatacadastro = $nf->recuperarDataCadastro($nfsid);
				}else{
					$nfsdatacadastro = date('d/m/Y');
				}

				if(!$nfsdatacadstro)
				{
					$nfsdatacadastro = date('d/m/Y');
				}
			?>
			<input type="text" name="nfsdatacadastro" id="nfsdatacadastro" value="<?=$nfsdatacadastro?>" disabled="disabled"/>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="TituloTabela center">
				<input type="button" onclick="salvarVinculo()" value="Salvar" >
				<input type="button" onclick="cancelarOperacao()" value="Cancelar" >
			</td>
		</tr>
	</table>
</form>
<?php if($msg): ?>
<script>
	alert('<?php echo $msg ?>');
<?php 
	if($msg_erro){
		echo "window.location.href = 'fabrica.php?modulo=sistema/geral/notaFiscal/vincularNota&acao=A';";
	}else{
		echo "window.location.href = 'fabrica.php?modulo=sistema/geral/notaFiscal/listarVinculo&acao=A';";
	}
	?>
</script>
<?php endif; ?>