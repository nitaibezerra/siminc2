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

jQuery(document).ready(function() {
	carregarVinculo('E');
	carregarVinculo('M');
	carregarVinculo('P');
});

function carregarVinculo(tipo) {
	jQuery.ajax({
   		type: "POST",
   		url: "atendimento.php?modulo=principal/eventovinculos&acao=A",
   		data: "requisicao=carregarVinculo&tipo="+tipo,
   		async: false,
   		success: function(html){jQuery('#div_'+tipo).html(html);}
 		});
}


function excluirVinculo(evvid) {
	var conf = confirm('Deseja excluir este vínculo?');
	if(conf) {
		window.location='atendimento.php?modulo=principal/eventovinculos&acao=A&requisicao=excluirVinculo&evvid='+evvid;
	}
}

function adicionarVinculo(tipo) {
	window.open('atendimento.php?modulo=principal/eventovinculos&acao=A&requisicao=telaInserirVinculo&tipo='+tipo,'Observações','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

</script>
<?

echo montarAbasArray(carregarAbasEvento(), "/atendimento/atendimento.php?modulo=principal/eventovinculos&acao=A");

monta_titulo( 'Gerenciamento de vínculos', 'Vincular Municípios, Estados e Empresas relacionadas com o evento' ); 
cabecalhoAgenda(); 

$permissao = permissoesPerfil();

?>
<form method="post" id="formulario" name="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="inserirAnexoEvento">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
	  <td class="SubTituloDireita" width="20%">UF:</td>
	  <td>
	  <? if($permissao['adicionarevento']) : ?>
	  <p><img src="../imagens/gif_inclui.gif" border="0" align="absmiddle" style="cursor:pointer;" onclick="adicionarVinculo('E');"> Adicionar UFs</p>
	  <? endif; ?>
	  <div id="div_E"></div>
	  </td>
	</tr>
	<tr>
	  <td class="SubTituloDireita" width="20%">Municípios:</td>
	  <td>
	  <? if($permissao['adicionarevento']) : ?>
	  <p><img src="../imagens/gif_inclui.gif" border="0" align="absmiddle" style="cursor:pointer;" onclick="adicionarVinculo('M');"> Adicionar Municípios</p>
	  <? endif; ?>
	  <div id="div_M"></div>
	  </td>
	</tr>
	<tr>
	  <td class="SubTituloDireita" width="20%">Empresas:</td>
	  <td>
	  <? if($permissao['adicionarevento']) : ?>
	  <p><img src="../imagens/gif_inclui.gif" border="0" align="absmiddle" style="cursor:pointer;" onclick="adicionarVinculo('P');"> Adicionar Empresas</p>
	  <? endif; ?>
	  <div id="div_P"></div>
	  </td>
	</tr>
</table>
</form>