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

if($_REQUEST['agdid']) {
	$_SESSION['atendimento_var'] = $db->pegaLinha("SELECT agdid, agddescricao FROM atendimento.agenda WHERE agdid='".$_REQUEST['agdid']."'");
}

if(!$_SESSION['atendimento_var']['agdid']) die("<script>alert('Selecione uma agenda');window.location='atendimento.php?modulo=principal/agenda&acao=A';</script>");

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

monta_titulo( 'Eventos', 'Selecione um evento' );
cabecalhoAgenda();

?>
<!-- (INÍCIO) BIBLIOTECAS - PARA USO DOS COMPONENTES (CALENDÁRIO E SLIDER) -->
<script	language="javascript" type="text/javascript" src="../includes/blendtrans.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/slider/slider.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
jQuery(document).ready(function() {
	listarEventos();
});

function listarEventos() {
	jQuery('#listaeventos').html('Carregando...');
	jQuery.ajax({
   		type: "POST",
   		url: "atendimento.php?modulo=principal/agenda&acao=A",
   		data: "requisicao=listarEventos<?=(($_REQUEST['pesnome'])?"&pesnome=".$_REQUEST['pesnome']:"") ?><?=(($_REQUEST['evtdatafim'])?"&evtdatafim=".$_REQUEST['evtdatafim']:"") ?><?=(($_REQUEST['evtdataini'])?"&evtdataini=".$_REQUEST['evtdataini']:"") ?><?=(($_REQUEST['evtassunto'])?"&evtassunto=".$_REQUEST['evtassunto']:"") ?>",
   		async: false,
   		success: function(html){jQuery('#listaeventos').html(html);}
 		});
}

function excluirEvento(evtid){
	if( confirm('Tem certeza que deseja excluir o evento?') ){
		window.location='atendimento.php?modulo=principal/evento&acao=A&requisicao=excluirEvento&evtid='+evtid;
	} else {
		return false;
	}
}


function inserirAgenda(agddescricao) {
	jQuery.ajax({
   		type: "POST",
   		url: "atendimento.php?modulo=principal/agenda&acao=A",
   		data: "requisicao=inserirAgenda&agddescricao="+agddescricao,
   		async: false,
   		success: function(html){}
 		});
}

function inserirEvento(evtassunto) {
	var evtid='';
	jQuery.ajax({
   		type: "POST",
   		url: "atendimento.php?modulo=principal/evento&acao=A",
   		data: "requisicao=inserirEvento&evtassunto="+evtassunto,
   		async: false,
   		success: function(html){evtid=html;}
 		});
 	return evtid;
}

function adicionarEvento() {
	evtassunto = prompt('Assunto do evento:');
	if(evtassunto) {
		var evtid = inserirEvento(evtassunto);
		window.location='atendimento.php?modulo=principal/gerenciarevento&acao=A&evtid='+evtid;
	}
}

function buscarEventos() {
	if(document.getElementById('evtdataini').value!='') {
		if(!validaData(document.getElementById('evtdataini'))) {
			alert('Data deve ser no formato ##/##/####');
			return false;
		}
	}

	document.getElementById('form').submit();
}

</script>
<form name="form" id="form"  method="post" action="" >
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
	  <td class="SubTituloDireita">Assunto:</td>
	  <td><?php echo campo_texto("evtassunto","N","S","Assunto","50","250","","","","","","","",$_REQUEST['evtassunto'])?></td>
	</tr>
	<tr>
	  <td class="SubTituloDireita">Data:</td>
	  <td><? echo campo_data2('evtdataini','N', 'S', 'Data de início', 'S' ); ?> até <? echo campo_data2('evtdatafim','N', 'S', 'Data de fim', 'S' ); ?></td>
	</tr>
	<tr>
	  <td class="SubTituloDireita">Participantes:</td>
	  <td><?php echo campo_texto("pesnome","N","S","Data","50","250","","","","","","","",$_REQUEST['pesnome'])?></td>
	</tr>
	<tr>
	  <td colspan="2" class="SubTituloCentro"><input type="button" name="buscar" value="Buscar" onclick="buscarEventos();"> <input type="button" name="todos" value="Ver todos" onclick="window.location='atendimento.php?modulo=principal/evento&acao=A';"></td>
	</tr>
</table>
</form>
<?

$permissao = permissoesPerfil();

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<? if($permissao['adicionarevento']): ?>
	<tr>
	  <td class="SubTituloDireita"><a style="cursor:pointer;" onclick="adicionarEvento();"><img src="../imagens/gif_inclui.gif" border="0" align="absmiddle"> Adicionar Evento</a></td>
	</tr>
<? endif; ?>
	<tr>
	  <td id="listaeventos"></td>
	</tr>
</table>
