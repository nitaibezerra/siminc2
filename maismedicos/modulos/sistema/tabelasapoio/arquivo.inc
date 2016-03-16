<?php 

if($_REQUEST['requisicao'] == 'mudaOrdem'){
	
// 	echo '<pre>';
// 	var_dump($_POST);
	
	$ordemAtual = $_POST['ordem'];
	
	
	
	$boAltera = $db->pegaUm("select count(*) from maismedicos.arquivo where arqstatus = 'A' and catid = {$_POST['catid']}");
	
	if($ordemAtual>$boAltera || $ordemAtual==0){
		$rsAltera = $db->carregar("select arqid, arqordem from maismedicos.arquivo where arqstatus = 'A' and catid = {$_POST['catid']} order by arqordem asc, arqid asc");
		if($rsAltera){
			$sqlCorrige = '';
			foreach($rsAltera as $key => $value){				
				$sqlCorrige .= "update maismedicos.arquivo set arqordem = ".($key+1)." where arqid = {$value['arqid']};";
				if($value['arqid']==$_POST['arqid']) $_POST['ordem'] = $key+1;
			}
			if($sqlCorrige){
				$db->executar($sqlCorrige);
				$db->commit();
			}
		}
	}
	
	if($_POST['operacao']=='subir'){
		$ordem1 = $_POST['ordem']+1;
	}else{
		$ordem1 = $_POST['ordem']-1;
	}
	
	$ordem1 = $ordem1<=0 ? 1 : $ordem1;

	if($boAltera>1 && ($boAltera>=$ordem1)){
		
		$sql = "update maismedicos.arquivo set arqordem = {$_POST['ordem']} where arqordem = {$ordem1} and catid = {$_POST['catid']};
				update maismedicos.arquivo set arqordem = {$ordem1} where arqid = {$_POST['arqid']};";
		
		$db->executar($sql);
		if($db->commit()){
			die('1');
		}
		
	}else{
		
		$categoria = $db->pegaUm("select catdsc from maismedicos.categoria where catid = {$_REQUEST['catid']}");
		
		if($boAltera==1){
		
			die('Não foi possível mudar a posição do arquivo, existe somente um registro para a categoria '.$categoria.'!');
			
		}elseif($boAltera<=$ordem1){
			
			die('Este arquivo está na última posição para a categoria '.$categoria.'!');
			
		}elseif($ordem1==1){
			
			die('Este arquivo está na primeira posição para a categoria '.$categoria.'!');
		}
	}
	
	die('Não foi possível mudar a posição do arquivo, tente novamente!');
}

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$campos	= array('catid' 	=> $_REQUEST['catid'],
				'arqdsc'	=> "'".$_REQUEST['arqdsc']."'",
				'arqordem'	=> "(select max(coalesce(arqordem,0))+1 from maismedicos.arquivo where catid = {$_REQUEST['catid']})");

$file = new FilesSimec("arquivo", $campos, 'maismedicos');
if(is_file($_FILES["arquivo"]["tmp_name"])){
	$arquivoSalvo = $file->setUpload($_FILES["arquivo"]["name"], "arquivo");
	$db->sucesso('sistema/tabelasapoio/arquivo');
}

if($_REQUEST['requisicao'] == 'downloadArquivo'){
	$file->getDownloadArquivo($_REQUEST['arqid']);
}

if($_REQUEST['requisicao'] == 'deletaArquivo'){
	$file->setRemoveUpload($_REQUEST['arqid']);
	$db->sucesso('sistema/tabelasapoio/arquivo');
}

if($_REQUEST['requisicao'] == 'gerenciarCategorias'){
	include_once APPRAIZ.'maismedicos/modulos/principal/gerenciarCategorias.inc';
	die;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
print '<br/>';

$titulo2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';
monta_titulo( 'Uploads', $titulo2 );
$db->cria_aba($abacod_tela, $url, $parametros);

$habilitado = 'S';

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>

function salvarArquivo()
{
	var catid = $('[name=catid]'),
	   arqdsc = $('[name=arqdsc]'),
	  arquivo = $('[name=arquivo]');

	if(catid.val() == ''){
		alert('Escolha uma categoria!');
		catid.focus();
		return false;
	} 	
	if(arqdsc.val() == ''){
		alert('Informe a descrição do arquivo!');
		arqdsc.focus();
		return false;
	} 	
	if(arquivo.val() == ''){
		alert('O campo arquivo é obrigatório!');
		arquivo.focus();
		return false;
	} 	

	divCarregando();
	$('#formcadastro').submit();
}

function baixarArquivo( arqid )
{
	document.location.href = '/maismedicos/maismedicos.php?modulo=sistema/tabelasapoio/arquivo&acao=A&requisicao=downloadArquivo&arqid='+arqid;
}

function excluirArquivo( arqid )
{
	if(confirm('Deseja excluir o arquivo?')){
		divCarregando();
		document.location.href = '/maismedicos/maismedicos.php?modulo=sistema/tabelasapoio/arquivo&acao=A&requisicao=deletaArquivo&arqid='+arqid;
	}
}

function gerenciarCategorias()
{	
	var popUp = window.open('/maismedicos/maismedicos.php?modulo=sistema/tabelasapoio/link&acao=A&requisicao=gerenciarCategorias&grupo=arquivo', 'popupCategorias', 'height=400,width=600,scrollbars=yes,top=50,left=200');
	popUp.focus();
}

function mudaOrdem(arqid, ordem, operacao, catid)
{
	$.ajax({
		url		: '/maismedicos/maismedicos.php?modulo=sistema/tabelasapoio/arquivo&acao=A',
		type	: 'post',
		data	: 'requisicao=mudaOrdem&arqid='+arqid+'&operacao='+operacao+'&ordem='+ordem+'&catid='+catid,
		success	: function(e){
			if(e=='1'){
				location.reload();
			}else{
				alert(e);
			}
		}
	});
}

function pesquisar(opr)
{
	
	var params = '';
	
	if(opr=='pesquisa'){
		params += '&catid='+$('#formpesquisa select[name=catid]').val();
		params += '&arqnome='+$('#formpesquisa input[name=arqnome]').val();
		params += '&arqdsc='+$('#formpesquisa input[name=arqdsc]').val();
		params += '&usunome='+$('#formpesquisa input[name=usunome]').val();
	}	
	params += '&requisicao='+$('#formpesquisa input[name=requisicao]').val(	);

	window.location.href = '/maismedicos/maismedicos.php?modulo=sistema/tabelasapoio/arquivo&acao=A'+params;
	
}

function limpar()
{

	$('#formpesquisa select[name=catid]').val('');
	$('#formpesquisa input[name=arqnome]').val('');
	$('#formpesquisa input[name=arqdsc]').val('');
	$('#formpesquisa input[name=usunome]').val('');	
	
}

$(function(){

	$('.mudaform').click(function(){
		
		if(this.id=='tabcadastro'){
			$('#formcadastro').show();
			$('#formpesquisa').hide();
			$('#lipesquisa').removeClass('active');
			$('#licadastro').addClass('active');
		}else{
			$('#formcadastro').hide();
			$('#formpesquisa').show();
			$('#lipesquisa').addClass('active');
			$('#licadastro').removeClass('active');
		}
		
	});	
	
});

</script>

<div class="row">
	<br />
	<div class="col-md-12">
		<ul class="nav nav-tabs">                    
			<li class="active" id="licadastro"><a id="tabcadastro" class="mudaform" href="javascript:void(0)">Cadastrar</a></li>                
			<li id="lipesquisa"><a id="tabpesquisa" class="mudaform" href="javascript:void(0)">Pesquisar</a></li>                
		</ul>
	</div>
</div>

<form name="formcadastro" id="formcadastro" method="post" action="" enctype="multipart/form-data">
	<input type="hidden" name="requisicao" id="requisicao" value="" />
		
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >	
		<thead>
			<tr>
				<th>Categoria</th>
				<th>Descrição</th>
				<th>Arquivo</th>
				<th>&nbsp;</th>
			</tr>
		</thead>
		<tbody>	
			<tr>
				<td align="center">
				
					<table>
						<tr>
							<td>
								<?php
								$sqlCat = "select
												catid as codigo,
												catdsc as descricao
											from maismedicos.categoria
											where catgrupo = 'arquivo'
											order by catdsc";
								$db->monta_combo('catid', $sqlCat, 'S', 'Selecione...', '', '', '', 300, 'S'); 
								?>
							</td>
							<td valign="top"><img src="../imagens/edit_on.gif" style="cursor:pointer;margin-top:8px;" alt="Gerenciar categorias" title="Gerenciar categorias" onclick="gerenciarCategorias()" /></td>
						</tr>
					</table>
					
				</td>
				<td><?php echo campo_textarea('arqdsc', 'S', $habilitado, '', 74, 2, 255); ?></td>		
				<td align="center"><input type="file" name="arquivo" <?php echo $habilitado!='S' ? 'disabled' : '' ?>/></td>
				<td align="center">
					<!-- <img src="../imagens/incluir_p.gif" title="Adicionar arquivo" alt="Adicionar arquivo" style="cursor:pointer" onclick="salvarArquivo()"/> -->
					<input onclick="salvarArquivo()" type="button" value="Incluir" />
				</td>
			</tr>
		</tbody>
	</table>	
</form>
<?php 
if($_REQUEST['requisicao'] == 'pesquisar'){

	extract($_GET);

	if($catid){
		$arWhere[] = "anx.catid = {$catid}";
	}
	if($arqnome){
		$arWhere[] = " arq.arqnome || '' || arq.arqextensao ilike '%{$arqnome}%' ";
	}
	if($arqdsc){
		$arWhere[] = " anx.arqdsc ilike '%{$arqdsc}%' ";
	}
	if($usunome){
		$arWhere[] = " usu.usunome ilike '%{$usunome}%' ";
	}

}
?>
<form name="formpesquisa" id="formpesquisa" method="post" action="" style="display:none;">
	<input type="hidden" name="requisicao" id="requisicao_pesquisa" value="pesquisar" />
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td class="subtitulodireita">Categoria</td>
			<td><?php $db->monta_combo('catid', $sqlCat, $habilitado, 'Selecione...', '', '', '', '', 'N'); ?></td>
		</tr>	
		<tr>
			<td class="subtitulodireita">Nome do arquivo</td>
			<td><?php echo campo_texto('arqnome', 'N', $habilitado, '', 25, 255, '', '');  ?></td>	
		</tr>	
		<tr>
			<td class="subtitulodireita">Descrição</td>
			<td><?php echo campo_texto('arqdsc', 'N', $habilitado, '', 25, 255, '', '');  ?></td>	
		</tr>	
		<tr>
			<td class="subtitulodireita">Responsável</td>
			<td><?php echo campo_texto('usunome', 'N', $habilitado, '', 25, 255, '', '');  ?></td>	
		</tr>
		<tr>
			<td class="subtitulodireita">&nbsp;</td>
			<td>
				<input type="button" value="Pesquisar" onclick="pesquisar('pesquisa')" />
				<input type="button" value="Limpar" onclick="limpar()" />
				<input type="button" value="Ver todos" onclick="pesquisar('todos')" />
			</td>	
		</tr>	
	</table>
</form>
<?php 
	
$sqlLista = "select 
				'<img src=\"../imagens/lupa_grafico.gif\" style=\"cursor:pointer\" onclick=\"baixarArquivo(' || arq.arqid || ')\" title=\"Download\" alt=\"Download\" />
				 <img src=\"../imagens/exclui_p.gif\" style=\"cursor:pointer\" onclick=\"excluirArquivo(' || arq.arqid || ')\" title=\"Excluir\" alt=\"Excluir\" />
				<img src=\"../imagens/indicador-verde2.png\" alt=\"Posição atual: ' || coalesce(anx.arqordem,0) || '\" title=\"Posição atual: ' || coalesce(anx.arqordem,0) || '\" onclick=\"mudaOrdem(' || anx.arqid || ',' || coalesce(anx.arqordem,0) || ', \'subir\', ' || anx.catid || ')\" style=\"cursor:pointer\"/>		 
				<img src=\"../imagens/indicador-verde2_para_baixo.png\" alt=\"Posição atual: ' || coalesce(anx.arqordem,0) || '\" title=\"Posição atual: ' || coalesce(anx.arqordem,0) || '\" onclick=\"mudaOrdem(' || anx.arqid || ',' || coalesce(anx.arqordem,0) || ', \'descer\', ' || anx.catid || ')\" style=\"cursor:pointer\"/>' as acao,
				anx.arqordem,
				cat.catdsc,
				arq.arqnome || '.' || arq.arqextensao as arquivo,				
				anx.arqdsc as descricao,
				round( (arq.arqtamanho::numeric/1024)  , 2 ) || 'kb' as tamanho,
				arq.arqtipo as tipo,
				usu.usunome as responsavel					
			from maismedicos.arquivo anx
			join public.arquivo arq on arq.arqid = anx.arqid
			join seguranca.usuario usu on usu.usucpf = arq.usucpf
			join maismedicos.categoria cat on cat.catid = anx.catid
			".(is_array($arWhere) ? ' where '.implode(' and ', $arWhere) : '')."
			order by cat.catdsc, anx.arqordem desc";

$arCabecalho = array('Ação', 'Ordem', 'Categoria', 'Arquivo', 'Descrição', 'Tamanho', 'Tipo', 'Responsável');
$db->monta_lista($sqlLista, $arCabecalho, 25, 10, '', '', '', '', array('80px'));

?>
<?php if($_REQUEST['requisicao'] == 'pesquisar'): ?>
	<script>
		$(function(){
			$('#formcadastro').hide();
			$('#formpesquisa').show();
			$('#lipesquisa').addClass('active');
			$('#licadastro').removeClass('active');
		});
	</script>
<?php endif; ?>