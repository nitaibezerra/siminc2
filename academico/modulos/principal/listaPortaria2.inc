<?php
require_once APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . "academico/classes/Portaria.class.inc";

if($_GET['prtidExcluir']){
	$obPortaria = new Portaria();
	$obPortaria->excluir($_GET['prtidExcluir']);
	$obPortaria->commit();
	unset($_GET['prtidExcluir']);
	unset($obPortaria);
	$db->sucesso("principal/listaPortaria2");
	die;
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
<!--

function alterarPortaria(id){
	if(id){
		window.location.href = "academico.php?modulo=principal/cadastroPortaria&acao=A&prtid="+id;
	}
}

function excluirPortaria(id){
	if(confirm('Deseja excluir registro')){
		window.location.href = "academico.php?modulo=principal/listaPortaria2&acao=A&prtidExcluir="+id;
		return true;
	} else {
		return false;
	}
}

function abreCumprimento(id){
	if(id){
		window.location.href = 'academico.php?modulo=principal/cumprimentoObjeto&acao=A&id='+id;
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
					$prtid = $_POST['prtid']; 
					echo campo_texto( 'prtid', 'N', 'S', '', 25, 200, '#######', '' ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="bottom">Descrição</td>
			<td>
				<?php 
					$prtdsc = $_POST['prtdsc']; 
					echo campo_texto( 'prtdsc', 'N', 'S', '', 25, 200, '', '' ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="2" align="center" style="text-align:center"> 
         		<input type="submit" name="btnPesquisar" id="btnPesquisar" value="Pesquisar" /> <input type="button" value="Incluir Novo" onclick="window.location.href='academico.php?modulo=principal/cadastroPortaria&acao=A';" />	
			</td>
     	</tr>
	</tbody>
</table>
</form>
<?php 
$where = array();

if($_POST['pesquisa']){
	extract($_POST);
	
	if( $prtid ){
		array_push($where, " prtid = '".$prtid."' ");
	}
	
	if( $prtdsc ){
		$prtdscTemp = removeAcentos($prtdsc);
		array_push($where, " public.removeacento(prtdsc) ilike '%".$prtdscTemp."%' ");
	}
	
}

$sql = "
select
	'<center>
		<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"alterarPortaria(\''|| prtid ||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\"></a>
		<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"excluirPortaria(\''|| prtid ||'\')\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a>
	</center>' as acao, prtid,
	prtdsc
	from academico.portaria
	" . ( is_array($where) && count($where) ? ' where ' . implode(' AND ', $where) : '' ) ."
";

$cabecalho = array("Ação","Número","Descrição");
$alinhamento   = array( 'center', 'center', 'left' );
$tamanho   = array( '5%', '15%', '90%' );
$db->monta_lista($sql,$cabecalho,50,5,'N','center','', '', $tamanho, $alinhamento);
?>