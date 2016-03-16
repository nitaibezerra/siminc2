<?
// executando requisição
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

/* Carregando o projeto PROFE/INEP, testando os possíveis erros */
if($_REQUEST['anpid']) {
	$sql = "SELECT anpdata, anpdscsituacao FROM profeinep.andamentoprocesso WHERE anpid='".$_REQUEST['anpid']."'";
	$andamento = $db->pegaLinha($sql);
	if($andamento) {
		$requisicao = 'atualizarandamento';
		$anpdata = $andamento['anpdata'];
		$anpdscsituacao = $andamento['anpdscsituacao'];
	} else {
		direcionar('?modulo=principal/andamento&acao=A','Não existe o andamento solicitado. Entre em contato com a equipe técnica.');
	}
} else {
	$requisicao = 'inserirandamento';
}
echo "<br/>";
// monatando abas
echo montarAbasArray(monta_abas_processo($_SESSION['profeinep_var']['tprid']),$_SERVER['REQUEST_URI']);
// montando cabeçalho
monta_cabecalho_profeinep($_SESSION['profeinep_var']['prcid']);

// verifica se muda a cor da aba de documento
$boMudaCorAba = verificaSeMudaCor($_SESSION['profeinep_var']['prcid']);
?>
<script src="./js/profeinep.js"></script>
<script src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

	$(document).ready(function(){
	
		// Serve para mudar a cor da aba documentos, quando existe número gerado e não tem arquivo anexado
		var boMudaCorAba = '<?php echo $boMudaCorAba;?>';
		if(!boMudaCorAba){
			if($('.tbl_conteudo').find('table:first').find('table:first').find('td:first').next().next().next().next().next().next().next().next().next().text() != 'Documentos'){
				$('.tbl_conteudo').find('table:first').find('table:first').find('td:first').next().next().next().next().next().next().next().next().next().next().next().css('color','red').css('font-weight','bold');
			} else {
				$('.tbl_conteudo').find('table:first').find('table:first').find('td:first').next().next().next().next().next().next().next().next().next().css('color','red').css('font-weight','bold');
			}
			
		}
	});
	
function validarandamento(obj) {
	if(document.getElementById('anpdata').value == '') {
		alert('Data de inclusão é obrigatório.');
		return false;
	}
	obj.disabled=true;
	document.getElementById('formulario').submit();
}
</script>
<form name="formulario" id="formulario" method="post" onsubmit="return validarandamento();">
<input type="hidden" name="requisicao" value="<? echo $requisicao; ?>"/>
<? if($_REQUEST['anpid']) { ?>
<input type="hidden" name="anpid" value="<? echo $_REQUEST['anpid']; ?>">
<? } ?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan='2'>Andamento do Processo</td>
</tr>
<tr>
	<td class="SubTituloDireita">Data inclusão :</td>
	<td><? echo campo_data( 'anpdata', 'N', 'S', '', 'S' ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Descrição :</td>
	<td><? echo campo_textarea( 'anpdscsituacao', 'S', 'S', '', '70', '4', '255'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Responsável :</td>
	<td><? echo $_SESSION['usunome']; ?></td>
</tr>
<tr>
	<td  bgcolor="#CCCCCC" colspan="2" align="center">
		<input type="button" value="Salvar" onclick="validarandamento(this);">
		<input type="button" value="Novo" onclick="window.location='?modulo=principal/andamento&acao=A';">
	</td>
</tr>	
</table>
</form>
<?
$sql = " SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\" style=\"cursor:pointer\" onclick=\"window.location=\'?modulo=principal/andamento&acao=A&anpid='|| anp.anpid ||'\';\"> <img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer\" onclick=\"Excluir(\'?modulo=principal/andamento&acao=A&requisicao=removerandamento&anpid='|| anp.anpid ||'\',\'Deseja realmente excluir este andamento?\');\"></center>' as acao,
				to_char(anp.anpdata, 'dd/mm/yyyy'), 
				ent.usunome 
		 FROM profeinep.andamentoprocesso as anp
		 LEFT JOIN seguranca.usuario as ent ON anp.usucpf = ent.usucpf  
		 WHERE anp.prcid='".$_SESSION['profeinep_var']['prcid']."'";
$cabecalho = array("Ações","Data inclusão","Responsável");
$db->monta_lista_simples($sql, $cabecalho, 50, 10, 'N', '', '' );
?>
