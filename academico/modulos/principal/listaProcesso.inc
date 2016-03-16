<?php
require_once APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . "academico/classes/Processo.class.inc";

unset($_SESSION['academico']['prcid']);

if($_GET['prcidExcluir']){
	$obProcesso = new Processo();
	$obProcesso->excluir($_GET['prcidExcluir']);
	$obProcesso->commit();
	unset($_GET['prcidExcluir']);
	unset($obProcesso);
	$db->sucesso("principal/listaProcesso");
	die;
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
<!--

function alterarProcesso(id){
	if(id){
		window.location.href = "academico.php?modulo=principal/cumprimentoObjeto&acao=A&prcid="+id;
	}
}

function excluirProcesso(id){
	if(confirm('Deseja excluir registro')){
		window.location.href = "academico.php?modulo=principal/listaProcesso&acao=A&prcidExcluir="+id;
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
					$prcnumero = $_POST['prcnumero']; 
					echo campo_texto( 'prcnumero', 'N', 'S', '', 25, 200, '#######', '' ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="bottom">Unidade Orçamentária</td>
			<td>
				<?php 
					$unidsc = $_POST['unidsc']; 
					echo campo_texto( 'unidsc', 'N', 'S', '', 25, 200, '', '' ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="bottom">Unidade Gestora</td>
			<td>
				<?php 
					$ungdsc = $_POST['ungdsc']; 
					echo campo_texto( 'ungdsc', 'N', 'S', '', 25, 200, '', '' ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="2" align="center" style="text-align:center"> 
         		<input type="submit" name="btnPesquisar" id="btnPesquisar" value="Pesquisar" /> <input type="button" value="Incluir Novo" onclick="window.location.href='academico.php?modulo=principal/cumprimentoObjeto&acao=A';" />	
			</td>
     	</tr>
	</tbody>
</table>
</form>
<?php 
$where = array();

if($_POST['pesquisa']){
	extract($_POST);
	
	if( $prcid ){
		array_push($where, " p.prcnumero = '".$prcnumero."' ");
	}
	
	if( $unidsc ){
		$unidscTemp = removeAcentos($unidsc);
		array_push($where, " public.removeacento(unidsc) ilike '%".$unidscTemp."%' ");
	}
	
	if( $ungdsc ){
		$ungdscTemp = removeAcentos($ungdsc);
		array_push($where, " public.removeacento(ungdsc) ilike '%".$ungdscTemp."%' ");
	}
	
}

$sql = "
select
	'<center>
		<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"alterarProcesso(\''|| p.prcid ||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\"></a>
		<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"excluirProcesso(\''|| p.prcid ||'\')\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a>
	</center>' as acao,
		p.prcnumero,        
	    p.prndtinicio,
	    p.prndtfim,
		--p.unicodexecutor,
	    u.unidsc,
	    --p.ungcodgestor,
	    ug.ungdsc,
	    --p.prtid,
	    po.prtdsc
	from academico.processo p
	left join public.unidade u on p.unicodexecutor = u.unicod
	left join public.unidadegestora ug on p.ungcodgestor = ug.ungcod
	inner join academico.portaria po on p.prtid = po.prtid
	" . ( is_array($where) && count($where) ? ' where ' . implode(' AND ', $where) : '' ) ."
";
//ver($sql);
$cabecalho = array("Ação","Número","Dt. Inicio", "Dt. Fim","Unidade Orçamentária","Unidade Gestora","Portaria");
$alinhamento   = array( 'center', 'center', 'left' );
$tamanho   = array( '5%', '7%', '5%', '5%', '10%', '10%', '10%' );
$db->monta_lista($sql,$cabecalho,50,5,'N','center','', '', $tamanho, $alinhamento);
?>