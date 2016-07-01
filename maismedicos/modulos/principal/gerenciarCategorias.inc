<?php 

if($_REQUEST['mudaOrdem'] == 'true'){

	$ordemAtual = $_POST['ordem'];

	$boAltera = $db->pegaUm("select count(*) from maismedicos.categoria where catstatus = 'A' and catgrupo = '{$_POST['grupo']}'");

	if($ordemAtual>$boAltera || $ordemAtual==0){
		$rsAltera = $db->carregar("select catid, catordem from maismedicos.categoria where catstatus = 'A' and catgrupo = '{$_POST['grupo']}' order by catordem asc, catid asc");
		if($rsAltera){
			$sqlCorrige = '';
			foreach($rsAltera as $key => $value){
				$sqlCorrige .= "update maismedicos.categoria set catordem = ".($key+1)." where catid = {$value['catid']};";
				if($value['catid']==$_POST['catid']) $_POST['ordem'] = $key+1;
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

		$sql = "update maismedicos.categoria set catordem = {$_POST['ordem']} where catordem = {$ordem1} and catgrupo = '{$_POST['grupo']}';
				update maismedicos.categoria set catordem = {$ordem1} where catid = {$_POST['catid']};";

		$db->executar($sql);
		if($db->commit()){
			die('1');
		}

	}else{

		if($boAltera==1){

			die('Não foi possível mudar a posição da categoria, existe somente um registro!');
			
		}elseif($boAltera<=$ordem1){
			
			die('Esta categoria já está na última posição!');
			
		}elseif($ordem1==1){
			
			die('Esta categoria já está na primeira posição!');
		}
	}

	die('Não foi possível mudar a posição da categoria, tente novamente!');
}

if($_REQUEST['pegaCategoria']){
	$sql = "select * from maismedicos.categoria where catid = {$_REQUEST['catid']}";
	$rs = $db->pegaLinha($sql);
	$rs['catdsc'] = utf8_encode($rs['catdsc']);
	echo simec_json_encode($rs);
	die;
}

if($_REQUEST['excluiCategoria']){
	
	$sql = "select sum(total) from (
				select count(*) as total from maismedicos.link where catid = {$_REQUEST['catid']}
				union all
				select count(*) as total from maismedicos.arquivo where catid = {$_REQUEST['catid']}
			) as foo";
	
	$rs = $db->pegaUm($sql);
	if($rs==0){
		$sql = "delete from maismedicos.categoria where catid = {$_REQUEST['catid']}";
		$db->executar($sql);
		if($db->commit()){
			die('1');
		}
	}else if($rs>0){
		die('Existem registros vinculados a esta categoria, não é possível exluí-la!');
	}
	
	die('Erro ao tentar excluir a categoria!');
}

if($_REQUEST['salvarCategoria']){
	
// 	ver($_POST, d);
	
	$biblioteca = $_REQUEST['biblioteca'] ? 'true' : 'null';
	$legislacao = $_REQUEST['legislacao'] ? 'true' : 'null';
	$links = $_REQUEST['links'] ? 'true' : 'null';
	$educacaopermanente = $_REQUEST['educacaopermanente'] ? 'true' : 'null';
	
	$ordem = $db->pegaUm("select coalesce(max(catordem),0) from maismedicos.categoria where catstatus = 'A' and catgrupo = '{$_REQUEST['catgrupo']}'");
	$ordem = $ordem<=0 ? 1 : $ordem+1; 
	
	if($_REQUEST['catid']){
		
		$sql = "update maismedicos.categoria set
					catdsc = '{$_REQUEST['catdsc']}',
					catgrupo = '{$_REQUEST['catgrupo']}',
					biblioteca = {$biblioteca},
					legislacao = {$legislacao},
					links = {$links},
					educacaopermanente = {$educacaopermanente}
				where catid = {$_REQUEST['catid']};";
		
	}else{
		
		$sql = "insert into maismedicos.categoria
					(catdsc, catgrupo, biblioteca, legislacao, catordem, links, educacaopermanente)
				values
					('{$_REQUEST['catdsc']}', '{$_REQUEST['catgrupo']}', {$biblioteca}, {$legislacao}, {$ordem}, {$links}, {$educacaopermanente}) returning catid;";
		
	}
	
	$catid = $db->pegaUm($sql);
	if($db->commit()){
		echo "<script type=\"text/javascript\" src=\"../includes/JQuery/jquery-1.4.2.js\"></script>
			  <script>
					//window.opener.location.reload();
					$('[name=catid]', window.opener.document).append('<option value={$catid}>{$_REQUEST['catdsc']}</option>');
					window.location.href = '{$_REQUEST['redirect']}'
			  </script>";
		
	}
}

?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>

	$(function(){
		
		$('#bt_salvar').click(function(){
			
			if($('[name=catdsc]').val()==''){
				alert('Informe o nome da categoria!');
				$('[name=catdsc]').focus();
				return false;
			}

			$('#formulario').submit();	
				
		});
		
	});

	function editarCategoria(catid)
	{
		$.ajax({
			url		: '/maismedicos/maismedicos.php?modulo=sistema/tabelasapoio/link&acao=A',
			type	: 'post',
			dataType: "json",
			data	: 'requisicao=gerenciarCategorias&pegaCategoria=true&catid='+catid,
			success	: function(e){

				if(e.biblioteca=='t'){
					$('[name=biblioteca]').attr('checked',true);
				}else{
					$('[name=biblioteca]').attr('checked',false);
				}
				if(e.legislacao=='t'){
					$('[name=legislacao]').attr('checked',true);
				}else{
					$('[name=legislacao]').attr('checked',false);
				}
				
				if(e.links=='t'){
					$('[name=links]').attr('checked',true);
				}else{
					$('[name=links]').attr('checked',false);
				}
				if(e.educacaopermanente=='t'){
					$('[name=educacaopermanente]').attr('checked',true);
				}else{
					$('[name=educacaopermanente]').attr('checked',false);
				}
				
				$('[name=catid]').val(e.catid);
				$('[name=catgrupo]').val(e.catgrupo);
				$('[name=catdsc]').val(e.catdsc);
				$('#bt_salvar').val('Alterar');
			}
		});
	}

	function excluirCategoria( catid )
	{
		if(confirm('Deseja excluir a categoria?')){
			$.ajax({
				url		: '/maismedicos/maismedicos.php?modulo=sistema/tabelasapoio/link&acao=A',
				type	: 'post',
				data	: 'requisicao=gerenciarCategorias&excluiCategoria=true&catid='+catid,
				success	: function(e){
					if(e=='1'){
						$("[name=catid] option[value='"+catid+"']", window.opener.document).remove();
						location.reload(); 
					}else{
						alert(e);
					}
				}
			});
		}
	}

	function mudaOrdem(catid, ordem, operacao, grupo)
	{

		$.ajax({
			url		: '/maismedicos/maismedicos.php?modulo=sistema/tabelasapoio/link&acao=A&',
			type	: 'post',
			data	: 'requisicao=gerenciarCategorias&mudaOrdem=true&catid='+catid+'&operacao='+operacao+'&ordem='+ordem+'&grupo='+grupo,
			success	: function(e){
				console.log(e);
				if(e=='1'){
					location.reload();
				}else{
					alert(e);
				}
			}
		});
	}
</script>
<?php 
$habilitado = 'S'; 
monta_titulo('Gerenciar Categorias', $_REQUEST['grupo']); 
?>

<form name="formulario" id="formulario" method="post" action="">
	<input type="hidden" name="salvarCategoria" value="true" />
	<input type="hidden" name="catid" value="" />
	<input type="hidden" name="redirect" value="<?php echo $_SERVER['REQUEST_URI']; ?>" />
	<input type="hidden" name="catgrupo" value="<?php echo $_REQUEST['grupo']; ?>" />
	<input type="hidden" name="requisicao" value="<?php echo $_REQUEST['requisicao']; ?>" />
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td class="subtitulodireita">Descrição</td>
			<td><?php echo campo_texto('catdsc', 'S', $habilitado, '', 35, 255, '', '');  ?></td>
		</tr>
		<?php if($_REQUEST['grupo']=='arquivo'): ?>
			<tr>
				<td class="subtitulodireita">Aba(s)</td>
				<td>
					<input type="checkbox" name="biblioteca" value="true" />&nbsp;Biblioteca&nbsp;
					<input type="checkbox" name="legislacao" value="true" />&nbsp;Legislação
				</td>
			</tr>
		<?php elseif($_REQUEST['grupo']=='link'): ?>
			<tr>
				<td class="subtitulodireita">Aba(s)</td>
				<td>
					<input type="checkbox" name="links" value="true" />&nbsp;Links importantes&nbsp;
					<input type="checkbox" name="educacaopermanente" value="true" />&nbsp;Educação permanente
				</td>
			</tr>
		<?php endif; ?>
		<tr>
			<td></td>
			<td>
			<input type="button" value="Inserir" id="bt_salvar" />
			<input type="button" value="Fechar" onclick="javascript:window.close()" />
			</td>
		</tr>
	</table>
</form>

<?php

$arCabecalho = array('Ação', 'Ordem', 'Código', 'Nome');

if($_REQUEST['grupo']=='arquivo'){

	array_push($arCabecalho, 'Legislação');
	array_push($arCabecalho, 'Biblioteca');
	
	$abasCampos = ",case when legislacao = true then '<img src=\"../imagens/check_p.gif\" />' end as legislacao,
					case when biblioteca = true then '<img src=\"../imagens/check_p.gif\" />' end as biblioteca";
	
}elseif($_REQUEST['grupo']=='link'){

	array_push($arCabecalho, 'Links');
	array_push($arCabecalho, 'Educação Permanente');
	
	$abasCampos = ",case when links = true then '<img src=\"../imagens/check_p.gif\" />' end as links,
					case when educacaopermanente = true then '<img src=\"../imagens/check_p.gif\" />' end as educacaopermanente";
}

$sql = "select 
			'<img src=\"../imagens/editar_nome.gif\" style=\"cursor:pointer\" onclick=\"editarCategoria(' || catid || ')\" title=\"Editar\" alt=\"Editar\" />
			 <img src=\"../imagens/exclui_p.gif\" style=\"cursor:pointer\" onclick=\"excluirCategoria(' || catid || ')\" title=\"Excluir\" alt=\"Excluir\" />
			 <img src=\"../imagens/indicador-verde2.png\" alt=\"Posição atual: ' || coalesce(catordem,0) || '\" title=\"Posição atual: ' || coalesce(catordem,0) || '\" onclick=\"mudaOrdem(' || catid || ',' || coalesce(catordem,0) || ', \'subir\', \'{$_REQUEST['grupo']}\')\" style=\"cursor:pointer\"/>		 
			 <img src=\"../imagens/indicador-verde2_para_baixo.png\" alt=\"Posição atual: ' || coalesce(catordem,0) || '\" title=\"Posição atual: ' || coalesce(catordem,0) || '\" onclick=\"mudaOrdem(' || catid || ',' || coalesce(catordem,0) || ', \'descer\', \'{$_REQUEST['grupo']}\')\" style=\"cursor:pointer\"/>' as acao,
			 coalesce(catordem,0) as ordem,
			catid,
			catdsc 
			{$abasCampos}
		from maismedicos.categoria 
		where catgrupo = '{$_REQUEST['grupo']}'
		order by catordem desc";


$db->monta_lista($sql, $arCabecalho, 10, 15, '', '', '', '', array('80px'));

?>