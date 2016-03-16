<?php
require_once APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . "academico/classes/TermoCooperacao.class.inc";

unset($_SESSION['academico']['tmcid']);

if($_GET['tmcidExcluir']){
	$obTermoCooperacao = new TermoCooperacao();
	$obTermoCooperacao->excluir($_GET['tmcidExcluir']);
	$obTermoCooperacao->commit();
	unset($_GET['tmcidExcluir']);
	unset($obTermoCooperacao);
	$db->sucesso("principal/listaTermoCooperacao");
	die;
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
<!--

function alterarTermoCooperacao(id){
	if(id){
		window.location.href = "academico.php?modulo=principal/cadTermoCooperacao&acao=A&tmcid="+id;
	}
}

function excluirTermoCooperacao(id){
	if(confirm('Deseja excluir registro')){
		window.location.href = "academico.php?modulo=principal/listaTermoCooperacao&acao=A&tmcidExcluir="+id;
		return true;
	} else {
		return false;
	}
}

function abreCumprimento(id){
	if(id){
		window.location.href = 'academico.php?modulo=principal/cadTermoCooperacao&acao=A&id='+id;
	}
}

//-->
</script>
<?php 
echo '<br/>';
monta_titulo( $titulo_modulo, '' );
?>
<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td class="SubTituloEsquerda" valign="bottom">
				<span style="cursor:pointer" onclick="window.location.href = 'academico.php?modulo=principal/cadTermoCooperacao&acao=A'"><img src="/imagens/gif_inclui.gif" align="absmiddle" style="border: none" /> Novo Termo Cooperação</span>
			</td>
		</tr>
	</tbody>
</table>
<!--<form method="post" name="formulario" id="formulario">
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
         		<input type="submit" name="btnPesquisar" id="btnPesquisar" value="Pesquisar" /> <input type="button" value="Incluir Novo" onclick="window.location.href='academico.php?modulo=principal/cadTermoCooperacao&acao=A';" />	
			</td>
     	</tr>
	</tbody>
</table>
</form>
--><?php 
$where = array();
/*
if($_POST['pesquisa']){
	extract($_POST);
	
	if( $tmcid ){
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
	
}*/

$sql = "
select
	'<center>
		<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"alterarTermoCooperacao(\''|| tc.tmcid ||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\"></a>
		<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"excluirTermoCooperacao(\''|| tc.tmcid ||'\')\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a>
	</center>' as acao,
		ep.entnome as nomeprop,
		erp.entnome as nomerepprop,
		ec.entnome as nomeconc,
		erc.entnome as nomerepconc,
		to_char(tc.tmcdtinclusao, 'DD/MM/YYYY') as tmcdtinclusao 
	FROM academico.termocooperacao tc
		left join entidade.entidade ep on tc.entidproponente = ep.entid
		left join entidade.entidade erp on tc.entidrepprop = erp.entid
		left join entidade.entidade ec on tc.entidconcedente = ec.entid
		left join entidade.entidade erc on tc.entidrepconc = erc.entid
	WHERE tmcstatus = 'A'
";
//ver($sql);
$cabecalho = array("Ação","Nome Proponente","Nome Representante Proponente", "Nome Concedente","Nome Representante Concedente","Data Inclusão");
//$alinhamento   = array( 'center', 'center', 'left' );
//$tamanho   = array( '5%', '7%', '5%', '5%', '10%', '10%', '10%' );
$db->monta_lista($sql,$cabecalho,50,5,'N','center','', '', $tamanho, $alinhamento);
?>