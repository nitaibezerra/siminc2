<?
// REMOVER
if ( $_REQUEST['coiidRemover'] ) 
{
	$sql = "update evento.coitem SET coistatus = 'I' WHERE coiid = ".$_REQUEST['coiidRemover'];
	$db->executar( $sql );
	$db->commit();
	$db->sucesso('principal/pesquisaItens');
	unset($_REQUEST['coiidRemover']); 
}

if($_REQUEST['coidsc']){
	$coidsc = $_REQUEST['coidsc'];
	$coidscbusca =  str_replace(" ","%",$coidsc);
	$codicao = " AND i.coicodsiasg || i.coidsc ilike '%".$coidscbusca."%' ";
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );
$sql = "SELECT DISTINCT
 			'<div align=\"center\"><img
				align=\"absmiddle\"
				src=\"/imagens/alterar.gif\"
				style=\"cursor: pointer\"
				onclick=\"javascript: selecionarItem('|| i.coiid ||' );\"
				title=\"Selecionar Processo\"
			 > 
				<img
					align=\"absmiddle\"
					src=\"/imagens/excluir.gif\"
					style=\"cursor: pointer\"
					onclick=\"removerItem( '|| i.coiid ||' );\"
					title=\"Excluir Processo\"
				/> </div>' as coiid,
			i.coicodsiasg, i.coidsc,
			um.umedescricao, 
			coalesce( i.coivlrreferenciamin, 0 ) / 1 as coivlrreferenciamin,
			coalesce( i.coivlrreferenciamax, 0 ) / 1 as coivlrreferenciamax
	    FROM evento.coitem i
			LEFT JOIN evento.unidademedida um on i.umeid = um.umeid
			WHERE i.coistatus = 'A' $codicao "; 
?>
<script src="../includes/prototype.js"></script>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form action="" method="POST" name="formulario">
					<input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>"/>
					<div style="float: left;">
						<table border="0" cellpadding="3" cellspacing="0">
							<tr>
								<td valign="bottom">
									Descrição do Item:
									<br/> 
									<?= campo_texto( 'coidsc', 'N', 'S', '', 50, 200, '', '' ); ?>
								</td>
							</tr>
							
						</table>
					<div style="float: left;">
						<br />
							<input type="button" name="" value="Pesquisar" onclick="return validaForm();"/>
							<input type="button" name="" value="Ver Todos" onclick="window.location.href='evento.php?modulo=principal/pesquisaItens&acao=A';"/>
						</div>	
					</div>
				</form>
			</td>
		</tr>
	</tbody>
</table>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:none;">
	<tr>
		<td style="padding: 10px;">
			<span
				style="cursor: pointer"
				onclick="cadastrarItem();"
				title="Novo Item">
				<img
					align="absmiddle"
					src="/imagens/gif_inclui.gif"
				/>
			Cadastrar Item
			</span>
			</td>
		</tr>
	</table>
<?php
	$cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "Cód. SIASG", "Descrição", "Unidade de Medida", "Valor Min.", "Valor Máx." ); 
	$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '');
?>
<script type="text/javascript"><!--
	
function removerItem( coiid ) {
 
	if ( confirm( 'Deseja excluir o Item?' ) ) {
		window.location.href = 'evento.php?modulo=principal/pesquisaItens&acao=A&coiidRemover='+coiid;
	}
}

function selecionarItem( coiid )
{   
	window.location.href = 'evento.php?modulo=principal/cadItem&acao=A&coiid='+coiid; 

}

function cadastrarItem(){ 
	window.location.href = 'evento.php?modulo=principal/cadItem&acao=I';
}

function validaForm()
{ 
  	if(document.formulario.coidsc.value == ''){
  		alert('Digite algum critério para pesquisar.');
  		document.formulario.coidsc.focus();
  		return false;
  	}
  	document.formulario.submit(); 
}
</script>