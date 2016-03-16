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

function salvarObservacoesEvento() {

	if(jQuery('#evtobservacoes').val()=='') {
		alert('Preencha a observação');
		return false;
	}
	
	jQuery('#formulario').submit();
}

function excluirObservacoesEvento(evbid) {
	var conf = confirm('Deseja excluir esta observação?');
	if(conf) {
		window.location='atendimento.php?modulo=principal/eventoeventoanexo&acao=A&requisicao=excluirObservacoesEvento&evbid='+evbid;
	}
}

</script>
<?

echo montarAbasArray(carregarAbasEvento(), "/atendimento/atendimento.php?modulo=principal/eventoobservacoes&acao=A");

monta_titulo( 'Gerenciamento de eventos', 'Fazer uploads/downloads do arquivos referentes ao evento' ); 
cabecalhoAgenda(); 

$permissao = permissoesPerfil();
?>
<form method="post" id="formulario" name="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="inserirObservacoesEvento">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
	  <td class="SubTituloDireita">Observações:</td>
	  <td><? echo campo_textarea( 'evbobservacoes', 'S', (($permissao['adicionarevento'])?'S':'N'), '', '70', '4', '2000'); ?></td>
	</tr>
	<? if($permissao['adicionarevento']) : ?>
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button" name="salvar" value="Salvar" onclick="salvarObservacoesEvento();"></td>
	</tr>
	<? endif; ?>

</table>
</form>
<?
$sql = "SELECT '".(($permissao['adicionarevento'])?"<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirObservacoesEvento('||obs.evbid||')\">":"")."' as acao, 
		obs.evbobservacoes, 
		usu.usunome, 
		to_char(evbdatains,'dd/mm/YYYY HH24:MI') as evbdatains  
		FROM atendimento.eventoobservacoes obs 
		INNER JOIN seguranca.usuario usu ON usu.usucpf = obs.usucpf 
		WHERE evtid='".$_SESSION['atendimento_var']['evtid']."' AND obs.evbstatus='A'";

$cabecalho = array("&nbsp;", "Observação", "Inserido por","Data/Hora");
$db->monta_lista($sql,$cabecalho,50,8,'N','center',$par2,"");

?>