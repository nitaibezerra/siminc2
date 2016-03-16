<?php
require_once APPRAIZ . 'includes/cabecalho.inc';

if($_GET['undidExcluir']){
	$sql = "SELECT picid FROM par.propostaitemcomposicao WHERE undid='".$_GET['undidExcluir']."'"; 
	$existe_pic = $db->pegaUm($sql);
	if(!$existe_pic) {
		
		$obUnidadeMedida = new UnidadeMedida();
		$obUnidadeMedida->excluir($_GET['undidExcluir']);
		$obUnidadeMedida->commit();
		unset($_GET['undidExcluir']);
		unset($obUnidadeMedida);
		$db->sucesso("principal/listaUnidadeMedida");
		die;
				
	} else {
		
		die("<script>
				alert('Não foi possivel excluir. Existem propostas de itens de composição cadastrados nesta unidade de medida');
				window.location='par.php?modulo=principal/listaUnidadeMedida&acao=A';
			 </script>");
		
	}

}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
<!--

function alterarUnidadeMedida(id){
	if(id){
		window.location.href = "par.php?modulo=principal/cadUnidadeMedida&acao=A&undid="+id;
	}
}

function excluirUnidadeMedida(id){
	if(confirm('Deseja excluir registro')){
		window.location.href = "par.php?modulo=principal/listaUnidadeMedida&acao=A&undidExcluir="+id;
		return true;
	} else {
		return false;
	}
}

//-->
</script>
<?php 
echo '<br/>';
monta_titulo( $titulo_modulo, '' );
?>
<form method="post" name="formulario" id="formulario">
<input type="hidden" id="pesquisa" name="pesquisa" value="1" />
<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td class="SubTituloDireita" valign="bottom">Número</td>
			<td>
				<?php 
					$undid = $_POST['undid']; 
					echo campo_texto( 'undid', 'N', 'S', '', 25, 200, '#######', '' ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="bottom">Descrição</td>
			<td>
				<?php 
					$unddsc = $_POST['unddsc']; 
					echo campo_texto( 'unddsc', 'N', 'S', '', 25, 200, '', '' ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >
					Tipo
			</td>
			<td>
				<?php
					$arTipo = array(
						array(
							'codigo'=>'E',
							'descricao'=>'Estadual'
						),
						array(
							'codigo'=>'M',
							'descricao'=>'Municipal'
						)
					);
					$undtipo = $obUnidadeMedida->undtipo;
					$db->monta_combo( "undtipo", $arTipo, 'S', 'Selecione...','', '', '', '100', 'N','undtipo');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="bottom">Grupo</td>
			<td>
				<?php 
					$sql = "SELECT 
								gumid as codigo,
								gumdsc as descricao
				            FROM 
				            	par.grupounidademedida
				            WHERE 
				            	gumstatus = 'A'";
					
					$db->monta_combo("gumid", $sql, "S", "Selecione...", '', '', '', '300', 'N','gumid');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="2" align="center" style="text-align:center"> 
         		<input type="submit" name="btnPesquisar" id="btnPesquisar" value="Pesquisar" /> <input type="button" value="Incluir Novo" onclick="window.location.href='par.php?modulo=principal/cadUnidadeMedida&acao=A';" />	
			</td>
     	</tr>
	</tbody>
</table>
</form>
<?php 
$where = array();

if($_POST['pesquisa']){
	extract($_POST);
	
	if( $undid ){
		array_push($where, " undid = '".$undid."' ");
	}
	
	if( $unddsc ){
		$unddscTemp = removeAcentos($unddsc);
		array_push($where, " public.removeacento(unddsc) ilike '%".$unddscTemp."%' ");
	}
	
	if( $undtipo ){
		array_push($where, " undtipo = '".$undtipo."' ");
	}
	
	if( $gumid ){
		array_push($where, " gum.gumid = '".$gumid."' ");
	}
	
}

$sql = "
select
	'<center>
		<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"alterarUnidadeMedida(\''|| undid ||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\"></a>
		<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"excluirUnidadeMedida(\''|| undid ||'\')\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a>
	</center>' as acao, undid,
	unddsc,
	case when undtipo = 'M' then 'Municipal'
	when undtipo = 'E' then 'Estadual'
	end as undtipo,
	gum.gumdsc
	from par.unidademedida um
	left join par.grupounidademedida gum on um.gumid = gum.gumid and gumstatus = 'A'
	" . ( is_array($where) && count($where) ? ' where ' . implode(' AND ', $where) : '' ) ."
";

$cabecalho 	 = array("Ação","Número","Descrição","Tipo","Grupo");
$alinhamento = array( 'center', 'center', 'left', 'center', 'center' );
$tamanho     = array( '5%', '15%', '50%', '15%', '20%' );
$db->monta_lista($sql,$cabecalho,50,5,'N','center','', '', $tamanho, $alinhamento);
?>