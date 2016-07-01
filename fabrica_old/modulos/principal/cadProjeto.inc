<?php
if($_REQUEST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicaoAjax']();
	exit;
}

include APPRAIZ."fabrica/classes/Projeto.class.inc";

$prjid = $_REQUEST['prjid'];

if($_REQUEST['requisicao']){
	$projeto = new Projeto($prjid);
	$projeto->popularDadosObjeto($_REQUEST);
	
	if($_POST['prjid'] && $_POST['prjid'] != "")
		$projeto->$_REQUEST['requisicao']();
	else
		$prjid = $projeto->$_REQUEST['requisicao']();
		
	if($_POST['prjproduto'][0]){
		deletarProdutosProjeto($prjid);
		inserirProdutosProjeto($prjid,$_POST['prjproduto']);
		unset($_POST['prjproduto']);
	}
	
	if($_POST['arrModulo']){
		inserirModulosProjeto($prjid,$_POST['arrModulo']);
		unset($_POST['arrModulo']);
	}
		
	$projeto->commit();
	
	if($_FILES['arqname']['tmp_name']){
		include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
		
		$campos	= array(
				"tapid"			=> $_POST['tapid'],
				"prjid"			=> $prjid ,
				"anpdsc"		=> "'".$_POST['arqdsc']."'",
				"anpstatus"		=> "'A'",
				"anpdtinclusao"	=> "now()"
				);	
				
		$file = new FilesSimec("anexoprojeto", $campos ,"fabrica");
		$file->setUpload($_POST['arqdsc']);
	}
	
	$msg = "Operação Realizada com Sucesso!";
}

if($prjid){
	$projeto = new Projeto($prjid);
	$arrDados = $projeto->getDados();
	extract($arrDados);
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$titulo = "Cadastro de Projetos";
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
	
	function salvarProjeto()
	{
		if(validaFormulario()){
			selectAllOptions( document.getElementById('prjproduto') );
			$('#form_cad_projeto').submit();
		}
	}
	
	function validaFormulario()
	{
		var erro = 0;
		$("[class~=obrigatorio]").each(function() { 
			if((this.name == "tapid" || this.name == "arqdsc") && !$("[name=arqname]").val()){
			
			}else{
				if(!this.value){
					erro = 1;
					alert('Favor preencher todos os campos obrigatórios!');
					this.focus();
					return false;
				}
			}
		});
		if(erro == 0){
			return true;
		}
	}
	
	function excluirAnexo(prjid,arqid){
		if(confirm('Deseja realmente excluir o anexo?')){
			$('#td_lista_anexos').html(carregando());
			$.ajax({
			  url: 'fabrica.php?modulo=principal/cadProjeto&acao=A&requisicaoAjax=excluirAnexoProjeto&arqid=' + arqid + '&prjid=' + prjid,
			  success: function(data) {
			    $('#td_lista_anexos').html(data);
			    alert('Anexo excluído com sucesso!');
			  }
			});
		}
	}
	
	function download(arqid)
	{
		window.location.href = 'fabrica.php?modulo=principal/cadProjeto&acao=A&requisicaoAjax=downwloadArquivo&arqid=' + arqid;
	}
	
	function carregando()
	{
		return "<center><img src=\"../imagens/wait.gif\" class=\"middle\" />Aguarde...Carregando!</center>";
	}
	
	function excluirModulo(mdpid,num)
	{
		if(confirm('Deseja realmente excluir o módulo?')){
			$.ajax({
			  url: 'fabrica.php?modulo=principal/cadProjeto&acao=A&requisicaoAjax=deletarModulosProjeto&mdpid=' + mdpid,
			  success: function(data) {
			    if(data){
			    	alert(data);
			    }else{
			    	$('#tr_modulo_' + num).remove();
			    	alert('Módulo excluído com sucesso!');
			    }
			  }
			});
		}
	}
	
	function addModulo()
	{
		 if(!$('[name=novo_modulo]').val()){
		 	alert('Favor informar o nomedo Módulo!');
		 	$('[name=novo_modulo]').focus();
		 }else{
		 	
		 	var num = $('[class=modulos]').size();
		 	
		 	if(num != 1 && $('#tr_modulo_' + num) && !$('#tr_modulo_' + num)){
		 		num = num -1;
		 	}
		 	
		 	$('#tr_modulo_' + num).after('<tr class="modulos" id="tr_modulo_' + ((num*1)+1) + '" ><td class="SubtituloDireita" >Módulo ' + ((num*1)+1) + '</td><td><input type="text" class=" normal" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="' + $('[name=novo_modulo]').val() + '" maxlength="120" size="41" name="arrModulo[0' + ((num*1)+1) + '\]"> <img src="../imagens/excluir.gif" class="middle link" title="Excluir Módulo" onclick="excluirElemento(\'tr_modulo_' + ((num*1)+1) + '\')"></td></tr>');
		 	num = (num*1) + 1;
		 	$('[name=hdn_num_modulos]').val( num );
		 	$('[name=novo_modulo]').val("")
		 	
		 }
		 
	}
	
	function excluirElemento(id)
	{
		$('#' + id).remove();
		
	}
	
</script>
<form name="form_cad_projeto" id="form_cad_projeto" enctype="multipart/form-data" method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubtituloDireita">Contrato:</td>
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
			<td class="SubtituloDireita" >Nome do Projeto:</td>
			<td>
				<input type="hidden" name="requisicao" value="salvar" />
				<input type="hidden" name="prjid" value="<?=$prjid?>" />
				<?=campo_texto("prjnome","S","S","Nome do Projeto","60","120","","") ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Sigla:</td>
			<td><?=campo_texto("prjsigla","S","S","Sigla do Projeto","40","120","","") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Descrição:</td>
			<td><?=campo_textarea("prjdesc","S","S","Descrição do Projeto","80","3","2000") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Escopo:</td>
			<td><?=campo_textarea("prjescopo","S","S","Escopo do Projeto","80","3",false) ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Observações:</td>
			<td><?=campo_textarea("prjobs","S","S","Observações do Projeto","80","3",false) ?></td>
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
			
			$prjproduto = recuperarProdutosProjeto($prjid);
			
			combo_popup("prjproduto",$sql,"Selecione o(s) Produtos(s)");
			?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Unidade:</td>
			<td>
				<?php $sql = "	select
									ungcod as codigo,
									ungdsc as descricao
								from
									public.unidadegestora
								where
									ungstatus = 'A'
								order by
									ungdsc"; ?>
				<?=$db->monta_combo("ungcod",$sql,"S","Selecione...","","","","","S") ?>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="TituloTabela">Módulos do Projeto</td>
		</tr>
		<tr id="tr_modulo_0" >
			<td class="SubtituloDireita">Adicionar Módulo:</td>
			<td><?=campo_texto("novo_modulo","N","S","Adicionar Módulo","40","120","","") ?> <img src="../imagens/gif_inclui.gif" class="middle link" title="Adicionar Módulo" onclick="addModulo()"></td>
		</tr>
		<?php $projeto ? $projeto->showModulosProjeto($prjid) : "" ?>
		<tr>
			<td colspan="2" class="TituloTabela">Anexar Arquivos</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Arquivo:</td>
			<td><input type="file" name="arqname" value=""  /></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Tipo:</td>
			<td>
				<?php $sql = "	select
									tapid as codigo,
									tapdsc as descricao
								from
									fabrica.tipoanexoprojeto
								where
									tapstatus = 'A'
								order by
									tapdsc"; ?>
				<?=$db->monta_combo("tapid",$sql,"S","Selecione...","","","","","S") ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Descrição:</td>
			<td><?=campo_texto("arqdsc","S","S","Descrição do Anexo","40","120","","") ?></td>
		</tr>
		<tr>
			<td colspan="2" class="TituloTabela center">
				<input type="button" onclick="salvarProjeto()" value="Salvar" >
				<input type="button" onclick="limparCampos()" value="Limpar Campos" >
			</td>
		</tr>
		<?php if($projeto): ?>
		<tr>
			<td colspan="2" class="TituloTabela">Anexos</td>
		</tr>
		<tr>
			<td id="td_lista_anexos" colspan="2"><?php $projeto->showAnexosProjeto($prjid) ?></td>
		</tr>
		<?php endif; ?>
	</table>
</form>
<?php if($msg): ?>
<script>
	alert('<?php echo $msg ?>');
</script>
<?php endif; ?>