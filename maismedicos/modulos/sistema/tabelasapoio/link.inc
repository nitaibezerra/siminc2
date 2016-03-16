<?php 

if($_REQUEST['requisicao'] == 'mudaOrdem'){

	$ordemAtual = $_POST['ordem'];

	$totalPosicoes = $db->pegaUm("select count(*) from maismedicos.link where lnkstatus = 'A' and catid = {$_POST['catid']}");
	
	// Corrige posicoes
	if($ordemAtual>$totalPosicoes || $ordemAtual==0){
		$rsAltera = $db->carregar("select lnkid, lnkordem from maismedicos.link where lnkstatus = 'A' and catid = {$_POST['catid']} order by lnkordem asc, lnkid asc");
		if($rsAltera){
			$sqlCorrige = '';
			foreach($rsAltera as $key => $value){
				$sqlCorrige .= "update maismedicos.link set lnkordem = ".($key+1)." where lnkid = {$value['lnkid']};";
				if($value['lnkid']==$_POST['lnkid']) $ordemAtual = $key+1;
			}
			if($sqlCorrige){
				$db->executar($sqlCorrige);
				$db->commit();
			}
		}
	}
	
	// Atualiza a posição
	if($_POST['operacao']=='subir'){
		$ordem1 = $ordemAtual+1;
	}else{
		$ordem1 = $ordemAtual-1;
	}	
	$ordem1 = $ordem1<=0 ? 1 : ($totalPosicoes<$ordem1) ? $totalPosicoes : $ordem1;

	if($totalPosicoes>1 && ($totalPosicoes>=$ordem1) || $_POST['ordem']==0){

		$sql = "update maismedicos.link set lnkordem = {$ordemAtual} where lnkordem = {$ordem1} and catid = {$_POST['catid']};
				update maismedicos.link set lnkordem = {$ordem1} where lnkid = {$_POST['lnkid']};";

		$db->executar($sql);
		if($db->commit()){
			die('1');
		}

	}else{

		$categoria = $db->pegaUm("select catdsc from maismedicos.categoria where catid = {$_REQUEST['catid']}");

		if($totalPosicoes==1){

			die('Não foi possível mudar a posição do arquivo, existe somente um registro para a categoria '.$categoria.'!');
			
		}elseif($totalPosicoes==$ordemAtual && $_POST['operacao']=='subir'){
			
			die('Este arquivo está na última posição para a categoria '.$categoria.'!');
			
		}elseif($ordemAtual==1 && $_POST['operacao']=='descer'){
			
			die('Este arquivo está na primeira posição para a categoria '.$categoria.'!');
		}
	}

	die('Não foi possível mudar a posição do arquivo, tente novamente!');
}

if($_REQUEST['requisicao'] == 'pegaLink'){
	$sql = "select * from maismedicos.link where lnkid = {$_REQUEST['lnkid']}";
	$rs = $db->pegaLinha($sql);
	$rs['lnktitulo'] = utf8_encode($rs['lnktitulo']);
	$rs['lnksubtitulo'] = utf8_encode($rs['lnksubtitulo']);
	$rs['lnkdsc'] = utf8_encode($rs['lnkdsc']);
	echo simec_json_encode($rs);
	die;
}

if($_REQUEST['requisicao'] == 'deletaLink'){
	if($_REQUEST['lnkid']){
// 		$sql = "delete from maismedicos.link where lnkid = {$_REQUEST['lnkid']}";
		$sql = "update maismedicos.link set lnkstatus = 'I' where lnkid = {$_REQUEST['lnkid']}";
		$db->executar($sql);
		if($db->commit()){
			$db->sucesso('sistema/tabelasapoio/link');
		}
	}
}

if($_REQUEST['requisicao'] == 'salvarLink'){
	
	if($_REQUEST['lnkid']){
	
		$sql = "update maismedicos.link set
					catid 		 = {$_REQUEST['catid']},
					lnktitulo 	 = '{$_REQUEST['lnktitulo']}',
					lnksubtitulo = '{$_REQUEST['lnksubtitulo']}',
					lnkdsc 		 = '{$_REQUEST['lnkdsc']}',
					lnkurl 		 = '{$_REQUEST['lnkurl']}',
					usucpf 		 = '{$_SESSION['usucpf']}'
				where 
					lnkid = {$_REQUEST['lnkid']}";
		
	}else{
		
		$sql = "insert into maismedicos.link 
					(catid, lnktitulo, lnksubtitulo, lnkdsc, lnkurl, usucpf)
				values
					({$_REQUEST['catid']}, '{$_REQUEST['lnktitulo']}', '{$_REQUEST['lnksubtitulo']}', '{$_REQUEST['lnkdsc']}', '{$_REQUEST['lnkurl']}', '{$_SESSION['usucpf']}');";
		
	}
	
	$db->executar($sql);
	if($db->commit()){
		$db->sucesso('sistema/tabelasapoio/link');
	}
}

if($_REQUEST['requisicao'] == 'gerenciarCategorias'){
	include_once APPRAIZ.'maismedicos/modulos/principal/gerenciarCategorias.inc';
	die;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
print '<br/>';

$titulo2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';
monta_titulo( 'Links', $titulo2 );
$db->cria_aba($abacod_tela, $url, $parametros);

$habilitado = 'S';

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>

function salvarArquivo()
{
	var catid = $('[name=catid]'),
	lnktitulo = $('[name=lnktitulo]'),
	   lnkurl = $('[name=lnkurl]'),
	   lnkdsc = $('[name=lnkdsc]');

	if(catid.val() == ''){
		alert('Escolha uma categoria!');
		catid.focus();
		return false;
	} 	
	if(lnktitulo.val() == ''){
		alert('O campo título é obrigatório!');
		lnktitulo.focus();
		return false;
	} 	
	if(lnkdsc.val() == ''){
		alert('O campo descrição é obrigatório!');
		lnkdsc.focus();
		return false;
	} 	
	if(lnkurl.val() == ''){
		alert('O campo link (url) é obrigatório!');
		lnkurl.focus();
		return false;
	} 	

	$('#formcadastro').submit();
}

function excluirLink( lnkid )
{
	if(confirm('Deseja excluir o link?')){
		document.location.href = '/maismedicos/maismedicos.php?modulo=sistema/tabelasapoio/link&acao=A&requisicao=deletaLink&lnkid='+lnkid;
	}
}

function editarLink( lnkid )
{
	$.ajax({
		url		: '/maismedicos/maismedicos.php?modulo=sistema/tabelasapoio/link&acao=A',
		type	: 'post',
		dataType: "json",
		data	: 'requisicao=pegaLink&lnkid='+lnkid,
		success	: function(e){
			$('[name=catid]').val(e.catid);
			$('[name=lnktitulo]').val(e.lnktitulo);
			$('[name=lnksubtitulo]').val(e.lnksubtitulo);
			$('[name=lnkdsc]').val(e.lnkdsc);
			$('[name=lnkurl]').val(e.lnkurl);
			$('[name=lnkid]').val(e.lnkid);
			$('#bt_salvar').val('Alterar');
		}
	});
}

function gerenciarCategorias()
{	
	var popUp = window.open('/maismedicos/maismedicos.php?modulo=sistema/tabelasapoio/link&acao=A&requisicao=gerenciarCategorias&grupo=link', 'popupCategorias', 'height=400,width=600,scrollbars=yes,top=50,left=200');
	popUp.focus();
}


function pesquisar(opr)
{
	
	var params = '';
	
	if(opr=='pesquisa'){
		params += '&catid='+$('#formpesquisa select[name=catid]').val();
		params += '&lnktitulo='+$('#formpesquisa input[name=lnktitulo]').val();
		params += '&lnksubtitulo='+$('#formpesquisa input[name=lnksubtitulo]').val();
		params += '&lnkdsc='+$('#formpesquisa input[name=lnkdsc]').val();
		params += '&lnkurl='+$('#formpesquisa input[name=lnkurl]').val();
		params += '&usunome='+$('#formpesquisa input[name=usunome]').val();
	}	
	params += '&requisicao='+$('#formpesquisa input[name=requisicao]').val(	);

	window.location.href = '/maismedicos/maismedicos.php?modulo=sistema/tabelasapoio/link&acao=A'+params;
	
}

function limpar()
{

	$('#formpesquisa select[name=catid]').val('');
	$('#formpesquisa input[name=lnktitulo]').val('');
	$('#formpesquisa input[name=lnksubtitulo]').val('');
	$('#formpesquisa input[name=lnkdsc]').val('');
	$('#formpesquisa input[name=lnkurl]').val('');	
	$('#formpesquisa input[name=usunome]').val('');	

}

function mudaOrdem(lnkid, ordem, operacao, catid)
{
	$.ajax({
		url		: '/maismedicos/maismedicos.php?modulo=sistema/tabelasapoio/link&acao=A',
		type	: 'post',
		data	: 'requisicao=mudaOrdem&lnkid='+lnkid+'&operacao='+operacao+'&ordem='+ordem+'&catid='+catid,
		success	: function(e){
			if(e=='1'){
				location.reload();
			}else{
				alert(e);
				location.reload();
			}
		}
	});
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
	<input type="hidden" name="requisicao" id="requisicao" value="salvarLink" />		
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >	
		<thead>
			<tr>
				<th>Categoria</th>
				<th>Título</th>
				<th>Subtitulo</th>
				<th>Descrição</th>
				<th>
					Link (URL)&nbsp;
					<img src="../imagens/ajuda.png" height="15" alt="A URL deve conter o cabeçalho http:// exemplo: http://www.google.com/" title="A URL deve conter o cabeçalho http:// exemplo: http://www.google.com/" style="cursor:pointer;"/>
				</th>
				<th>&nbsp;</th>
			</tr>
		</thead>
		<tbody>	
			<tr>
				<td align="center">
					<input type="hidden" name="lnkid" value="" />
					<table>
						<tr>
							<td>
								<?php
								$sqlCat = "select
												catid as codigo,
												catdsc as descricao
											from maismedicos.categoria
											where catgrupo = 'link'
											order by catdsc";
								$db->monta_combo('catid', $sqlCat, 'S', 'Selecione...', '', '', '', 200, 'S'); 
								?>
							</td>
							<td valign="top"><img src="../imagens/edit_on.gif" style="cursor:pointer;margin-top:8px;" alt="Gerenciar categorias" title="Gerenciar categorias" onclick="gerenciarCategorias()" /></td>
						</tr>
					</table>
					
					
				</td>
				<td align="center"><?php echo campo_texto('lnktitulo', 'S', $habilitado, '', 25, 255, '', '');  ?></td>	
				<td align="center"><?php echo campo_texto('lnksubtitulo', 'N', $habilitado, '', 25, 255, '', '');  ?></td>	
				<td><?php echo campo_textarea('lnkdsc', 'S', $habilitado, '', 55, 2, 500); ?></td>		
				<td align="center"><?php echo campo_texto('lnkurl', 'S', $habilitado, '', 35, 500, '', '');  ?></td>
				<td align="center">
					<input onclick="salvarArquivo()" type="button" value="Incluir" id="bt_salvar" />
				</td>
			</tr>
		</tbody>
	</table>	
</form>

<?php 
if($_REQUEST['requisicao'] == 'pesquisar'){

	extract($_GET);

	if($catid){
		$arWhere[] = "lnk.catid = {$catid}";
	}
	if($lnktitulo){
		$arWhere[] = " lnk.lnktitulo ilike '%{$lnktitulo}%' ";
	}
	if($lnksubtitulo){
		$arWhere[] = " lnk.lnktitulo ilike '%{$lnksubtitulo}%' ";
	}
	if($lnkdsc){
		$arWhere[] = " lnk.lnkdsc ilike '%{$lnkdsc}%' ";
	}
	if($lnkurl){
		$arWhere[] = " lnk.lnkurl ilike '%{$lnkurl}%' ";
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
			<td class="subtitulodireita">Título</td>
			<td><?php echo campo_texto('lnktitulo', 'N', $habilitado, '', 25, 255, '', '');  ?></td>	
		</tr>	
		<tr>
			<td class="subtitulodireita">Subtitulo</td>
			<td><?php echo campo_texto('lnksubtitulo', 'N', $habilitado, '', 25, 255, '', '');  ?></td>	
		</tr>	
		<tr>
			<td class="subtitulodireita">Descrição</td>
			<td><?php echo campo_texto('lnkdsc', 'N', $habilitado, '', 25, 255, '', '');  ?></td>	
		</tr>
		<tr>
			<td class="subtitulodireita">Link (URL)</td>
			<td><?php echo campo_texto('lnkurl', 'N', $habilitado, '', 25, 255, '', '');  ?></td>	
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
				'<img src=\"../imagens/editar_nome.gif\" style=\"cursor:pointer\" onclick=\"editarLink(' || lnk.lnkid || ')\" title=\"Editar\" alt=\"Editar\" />
				 <img src=\"../imagens/exclui_p.gif\" style=\"cursor:pointer\" onclick=\"excluirLink(' || lnk.lnkid || ')\" title=\"Excluir\" alt=\"Excluir\" />
				 <img src=\"../imagens/indicador-verde2.png\" alt=\"Posição atual: ' || coalesce(lnk.lnkordem,0) || '\" title=\"Posição atual: ' || coalesce(lnk.lnkordem,0) || '\" onclick=\"mudaOrdem(' || lnk.lnkid || ',' || coalesce(lnk.lnkordem,0) || ', \'subir\', ' || lnk.catid || ')\" style=\"cursor:pointer\"/>		 
				 <img src=\"../imagens/indicador-verde2_para_baixo.png\" alt=\"Posição atual: ' || coalesce(lnk.lnkordem,0) || '\" title=\"Posição atual: ' || coalesce(lnk.lnkordem,0) || '\" onclick=\"mudaOrdem(' || lnk.lnkid || ',' || coalesce(lnk.lnkordem,0) || ', \'descer\', ' || lnk.catid || ')\" style=\"cursor:pointer\"/>' as acao,
				coalesce(lnk.lnkordem,0) as lnkordem,
				cat.catdsc,
				lnk.lnktitulo,
				lnk.lnksubtitulo,
				lnk.lnkdsc,
				'<a href=\"' || lnk.lnkurl || '\" target=\"_blank\" \><img src=\"../imagens/globo_terrestre.png\" alt=\"' || lnk.lnkurl || '\" title=\"' || lnk.lnkurl || '\" style=\"cursor:pointer;border:0;\" border:0; /></a>' as url,
				usu.usunome as responsavel					
			from maismedicos.link lnk	
			left join maismedicos.categoria cat on cat.catid = lnk.catid
			left join seguranca.usuario usu on usu.usucpf = lnk.usucpf
			where lnk.lnkstatus = 'A' 
			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
			order by cat.catordem, lnk.lnkordem desc";

$arCabecalho = array('Ação', 'Ordem', 'Categoria', 'Titulo', 'Subtitulo', 'Descrição', 'Link (URL)', 'Responsável');
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