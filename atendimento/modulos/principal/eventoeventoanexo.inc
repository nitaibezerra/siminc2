<?
 /*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
   Mdulo:inicio.inc
   Finalidade: permitir abrir a página inicial
    */

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

if(!$_SESSION['atendimento_var']['evtid']) die("<script>alert('Evento não encontrado. Selecione um evento');window.location='atendimento.php?modulo=principal/evento&acao=A';</script>");
if(!$_SESSION['atendimento_var']['agdid']) die("<script>alert('Selecione uma agenda');window.location='atendimento.php?modulo=principal/agenda&acao=A';</script>");

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>

function salvarAnexoEvento() {

	if(jQuery('#arquivo').val()=='') {
		alert('Selecione um arquivo');
		return false;
	}
	if(jQuery('#taeid').val()=='') {
		alert('Selecione o tipo de documento');
		return false;
	}
	
	jQuery('#formulario').submit();
}

function downloadAnexoEvento(arqid) {
	window.location='atendimento.php?modulo=principal/eventoeventoanexo&acao=A&requisicao=downloadAnexoEvento&arqid='+arqid;
}

function excluirAnexoEvento(aneid) {
	var conf = confirm('Deseja excluir este anexo?');
	if(conf) {
		window.location='atendimento.php?modulo=principal/eventoeventoanexo&acao=A&requisicao=excluirAnexoEvento&aneid='+aneid;
	}
}


</script>
<?

echo montarAbasArray(carregarAbasEvento(), "/atendimento/atendimento.php?modulo=principal/eventoeventoanexo&acao=A");

monta_titulo( 'Gerenciamento de eventos', 'Fazer uploads/downloads do arquivos referentes ao evento' ); 
cabecalhoAgenda(); 

$permissao = permissoesPerfil();

?>
<form method="post" id="formulario" name="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="inserirAnexoEvento">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
	  <td class="SubTituloDireita">Arquivo:</td>
	  <td><input type="file" name="arquivo" id="arquivo" <?=((!$permissao['adicionarevento'])?"disabled":"") ?>> <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/></td>
	</tr>
	<tr>
	  <td class="SubTituloDireita">Tipo:</td>
 	  <td>
	  <?php
	  $sql = "select taeid as codigo, taedescricao as descricao from atendimento.tipoanexoevento order by taedescricao asc";
	  $db->monta_combo( 'taeid', $sql, (($permissao['adicionarevento'])?'S':'N'), 'Selecione', '', '', '', 200, 'S', 'taeid');
	  ?>
	  </td>
	</tr>
	<tr>
	  <td class="SubTituloDireita">Descrição:</td>
	  <td><? echo campo_textarea( 'anedescricao', 'N', (($permissao['adicionarevento'])?'S':'N'), '', '80', '5', '100'); ?></td>
	</tr>
	<? if($permissao['adicionarevento']) : ?>
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button" name="salvar" value="Salvar" onclick="salvarAnexoEvento();"></td>
	</tr>
	<? endif; ?>
</table>
</form>
<?
$sql = "SELECT '".(($permissao['adicionarevento'])?"<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirAnexoEvento('||aev.aneid||')\">":"")." <img src=../imagens/anexo.gif style=cursor:pointer; onclick=\"downloadAnexoEvento('||aev.arqid||')\">' as acao, 
		aev.anedescricao, 
		tae.taedescricao 
		FROM atendimento.anexoevento aev 
		LEFT JOIN atendimento.tipoanexoevento tae ON aev.taeid = tae.taeid 
		WHERE evtid='".$_SESSION['atendimento_var']['evtid']."' AND aev.anestatus='A'";

$cabecalho = array("&nbsp;", "Descrição", "Tipo");
$db->monta_lista($sql,$cabecalho,50,8,'N','center',$par2,"");

?>