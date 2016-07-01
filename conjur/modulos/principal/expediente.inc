<?php
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
}
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
echo "<br/>";

/* Tratamento de segurança */
$permissoes = verificaPerfilConjur();
/* FIM - Tratamento de segurança */


echo montarAbasArray(monta_abas_processo($_SESSION['conjur_var']['tprid']),$_SERVER['REQUEST_URI']);
monta_cabecalho_conjur($_SESSION['conjur_var']['prcid']);

// verifica se muda a cor da aba de documento
$boMudaCorAba = verificaSeMudaCor($_SESSION['conjur_var']['prcid']);
?>
<script src="./js/conjur.js"></script>
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
	
</script>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
<tr>
	<td colspan="2" class="SubTituloCentro">Pareceres</td>
</tr>
<tr>
	<td class="SubTituloEsquerda">Expedientes</td>
</tr>
<tr>
	<td>
	<?
	$sql = "SELECT	'<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\" style=\"cursor:pointer\" onclick=\"window.location=\'?modulo=principal/expediente_lancamento&acao=A&expid='|| exp.expid ||'\';\"> <img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"Excluir(\'?modulo=principal/expediente&acao=A&requisicao=removerexpediente&expid='|| exp.expid ||'\',\'Deseja realmente exluir este expediente?\')\"></center>' as acao,						
   					to_char(expdtinclusao, 'dd/mm/yyyy') as expdtinclusao,
					usu.usunome,
					tpe.tpedsc
			FROM conjur.expediente exp 
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = exp.usucpf  
			LEFT JOIN conjur.tipoexpediente tpe ON tpe.tpeid = exp.tpeid
			WHERE expstatus='A' AND prcid='".$_SESSION['conjur_var']['prcid']."'";
	 
	$cabecalho = array( "Ação", 
						"Data de Inclusão",
						"Responsável",
						"Tipo");
	$db->monta_lista_simples($sql, $cabecalho, 50, 10, 'N', '100%', '' );
	?>
	</td>
</tr>
<tr style="background-color: #cccccc">
	<td>&nbsp;
	<? if($permissoes['gravar']) { ?>
	<input type="button" value="Entrar com Novo Expediente" onclick="this.disabled=true;window.location='?modulo=principal/expediente_lancamento&acao=A';"></td>
	<? } ?>
</tr>									
</table>
