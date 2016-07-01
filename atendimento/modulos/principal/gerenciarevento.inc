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

if(isset($_POST['buscar'])){
	buscarPessoasCPF($_POST['cpf'],$_POST['tipo']);
	exit;
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	die();
}
if($_REQUEST['requisicaoAjax']) {
	$_REQUEST['requisicaoAjax']($_REQUEST);
	die();
}

if(!$_REQUEST['evtid']) die("<script>alert('Evento não encontrado');window.location='atendimento.php?modulo=principal/evento&acao=A';</script>");
if(!$_SESSION['atendimento_var']['agdid']) die("<script>alert('Selecione uma agenda');window.location='atendimento.php?modulo=principal/agenda&acao=A';</script>");

if($_REQUEST['evtid']) $_SESSION['atendimento_var']['evtid'] = $_REQUEST['evtid']; 

$sql = "SELECT * FROM atendimento.evento WHERE evtid='".$_REQUEST['evtid']."'";
$evento = $db->pegaLinha($sql);
extract($evento);

$arrEvtdataini = explode(" ",$evtdataini);
$evtdataini = $arrEvtdataini[0];
$evtdatainihora = substr($arrEvtdataini[1],0,5);

$arrEvtdatafim = explode(" ",$evtdatafim);
$evtdatafim = $arrEvtdatafim[0];
$evtdatafimhora = substr($arrEvtdatafim[1],0,5);


//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

?>
<!-- (INÍCIO) BIBLIOTECAS - PARA USO DOS COMPONENTES (CALENDÁRIO E SLIDER) -->

<script	language="javascript" type="text/javascript" src="../includes/blendtrans.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/slider/slider.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<!-- (INÍCIO) BIBLIOTECAS - PARA USO DOS COMPONENTES (BOX) -->
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />
<?
$sql = "SELECT * FROM atendimento.tipopessoa WHERE tppstatus='A' ORDER BY tppid";
$tipopessoa = $db->carregar($sql);
?>
<script>
jQuery(document).ready(function() {
	<? if($tipopessoa[0]) :
	foreach($tipopessoa as $tp) : ?>
	carregarPessoasPorTipo('<?=$tp['tppid'] ?>');
	<? endforeach;
	endif; ?>
	habilitarAutoComplete();	
	carregarEncaminhamentos();
});

function cat( val ){
	return true;
}

function carregarMunicipiosAjax( id ){
	var pevid = jQuery('#'+id).attr('id');
	pevid = pevid.substring(6,100);
	jQuery.ajax({
   		type: "POST",
   		url: window.location,
   		data: "requisicaoAjax=carregarMunicipiosAjax&uf="+jQuery('#'+id).val()+"&pevid="+pevid,
   		async: false,
   		success: function(html){
   			jQuery('#muncod_'+pevid).parent().html(html);
   		}
 	});
}

function carregarPessoasPorTipo(tppid) {
	jQuery('#tipopessoas_'+tppid).html('Carregando...');
	jQuery.ajax({
   		type: "POST",
   		url: "atendimento.php?modulo=principal/gerenciarevento&acao=A",
   		data: "requisicaoAjax=listarPessoasPorTipo&evtid=<?=$_REQUEST['evtid'] ?>&tppid="+tppid,
   		async: false,
   		success: function(html){jQuery('#tipopessoas_'+tppid).html(html);}
 		});
}

function carregarEncaminhamentos() {
	jQuery.ajax({
   		type: "POST",
   		url: "atendimento.php?modulo=principal/gerenciarevento&acao=A",
   		data: "requisicaoAjax=carregarEncaminhamentos&evtid=<?=$_REQUEST['evtid'] ?>",
   		async: false,
   		success: function(html){jQuery('#td_encaminhamentosporpessoa').html(html);}
 		});
}

function salvarParticipantes(tppid) {
	jQuery.ajax({
   		type: "POST",
   		url: "atendimento.php?modulo=principal/gerenciarevento&acao=A",
   		data: "requisicaoAjax=salvarParticipantes&"+jQuery("#tipopessoas_"+tppid).find('[name!="requisicaoAjax"]').serialize(),
   		async: false,
   		success: function(html){ alert("Participante inserido com sucesso!"); }
 	});
}

function removerPessoaEvento(pevid, tppid) {
	var conf = confirm('Deseja excluir participante?');
	if(conf) {
		jQuery.ajax({
	   		type: "POST",
	   		url: "atendimento.php?modulo=principal/gerenciarevento&acao=A",
	   		data: "requisicaoAjax=excluirPessoaEvento&pevid="+pevid,
	   		async: false,
	   		success: function(html){carregarPessoasPorTipo(tppid);}
	 		});
	}
}

function removerEncaminhamento(ecmid) {
	var conf = confirm('Deseja excluir encaminhamento?');
	if(conf) {
		jQuery.ajax({
	   		type: "POST",
	   		url: "atendimento.php?modulo=principal/gerenciarevento&acao=A",
	   		data: "requisicaoAjax=excluirEncaminhamento&ecmid="+ecmid,
	   		async: false,
	   		success: function(html){carregarEncaminhamentos();}
	 		});
	}
}

function inserirEncaminhamentoPorPessoa(pevid) {
	var ecmdescricao = prompt('Digite Encaminhamento:');
	var ecmprazo = prompt('Digite o Prazo Encaminhamento (DD/MM/YYYY):');
	if( ecmdescricao != '' && ecmprazo != '' ) {
		alert(ecmdescricao+' - '+ecmprazo);
		jQuery.ajax({
	   		type: "POST",
	   		url: "atendimento.php?modulo=principal/gerenciarevento&acao=A",
	   		data: "requisicaoAjax=inserirEncaminhamentoPorPessoa&pevid="+pevid+"&ecmdescricao="+ecmdescricao+"&ecmprazo="+ecmprazo,
	   		async: false,
	   		success: function(html){
	   			alert('Encaminhamento inserido com sucesso.');
				if( html == 'data' ){
					alert('Data inválida');
				}
				carregarEncaminhamentos()
	   		}
	 	});
	}
}

function habilitarAutoComplete() {
	jQuery("[id^='pesnome_']:enabled").each(function(i,j,a) {
		jQuery(j).autocomplete("atendimento.php?modulo=principal/gerenciarevento&acao=A&requisicaoAjax=buscarPessoas", {
		  	cacheLength:10,
			width: 440,
			scrollHeight: 220,
			selectFirst: true,
			autoFill: false
		}).result(function(event, data, formatted) {
			var linha_nome = j.parentNode.parentNode.parentNode;
			linha_nome.cells[0].childNodes[0].childNodes[1].value=data[0];		
   			linha_nome.cells[3].childNodes[0].childNodes[1].value=data[1];
   			linha_nome.cells[4].childNodes[0].childNodes[1].value=data[2];
   			linha_nome.cells[1].childNodes[0].childNodes[1].value=data[3];
   			linha_nome.cells[2].childNodes[0].childNodes[1].value=data[4];
   			linha_nome.cells[5].childNodes[0].childNodes[1].value=data[5];
	    });
    });
}

function buscarDadosCPF(id,tp) {
	jQuery.post("atendimento.php?modulo=principal/gerenciarevento&acao=A&requisicaoAjax=buscarPessoasCPF",{buscar:'buscar',cpf:jQuery("#pescpf_"+id).val(),tipo:tp},
		function(data){
			var obj = jQuery.parseJSON(data);
			if(obj){
				jQuery("#pescpf_"+id).val(obj[0]);	
				jQuery("#pesemail_"+id).val(obj[1]);	
				jQuery("#pestelefone_"+id).val(obj[2]);	
				jQuery("#pesnome_"+id).val(obj[3]);	
				jQuery("#pesid_"+id).val(obj[4]);	
				jQuery("#pevcargo_"+id).val(obj[5]);
				if(tp == 2 || tp == 3){
					jQuery("#estuf_"+id).val(obj[6]);
					jQuery("#muncod_"+id).val(obj[7]);
					jQuery("#pevorgao_"+id).val(obj[8]);
				}
			}
	}); 
}

function salvarEvento() {
	if(jQuery('#evtassunto').val()=='') {
		alert('Preencha um assunto');
		return false;
	}
	if(jQuery('#evtdataini').val()=='') {
		alert('Preencha data de início');
		return false;
	}
	if(jQuery('#evtdatafim').val()=='') {
		alert('Preencha data de fim');
		return false;
	}	
	jQuery('#requisicao').val('atualizarEvento');
	jQuery('#formulario').submit();
}

function inserirPessoaPorTipo(tppid) {
	//salvarParticipantes(tppid);
	jQuery.ajax({
   		type: "POST",
   		url: "atendimento.php?modulo=principal/gerenciarevento&acao=A",
   		data: "requisicaoAjax=inserirPessoaPorTipo&evtid=<?=$_REQUEST['evtid'] ?>&tppid="+tppid,
   		async: false,
   		success: function(html){ } });
	carregarPessoasPorTipo(tppid);
	habilitarAutoComplete();
}


function abrirAnexoEncaminhamento(pecid) {
	window.open('atendimento.php?modulo=principal/eventoeventoanexo&acao=A&requisicao=telaInserirAnexoEncaminhamento&pecid='+pecid,'Observações','scrollbars=no,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}
</script>
<?

echo montarAbasArray(carregarAbasEvento(), "/atendimento/atendimento.php?modulo=principal/gerenciarevento&acao=A&evtid=".$_REQUEST['evtid']);

monta_titulo( 'Gerenciamento de eventos', 'Preencha as informações referentes aos eventos' ); 
cabecalhoAgenda();

$permissao = permissoesPerfil();

?>
<form method="post" id="formulario" name="formulario">
<input type="hidden" name="evtid" value="<?=$_REQUEST['evtid'] ?>">
<input type="hidden" name="requisicao" id="requisicao" value="atualizarEvento">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
	  <td class="SubTituloDireita">Assunto:</td>
	  <td><? echo campo_texto('evtassunto', "S", (($permissao['adicionarevento'])?"S":"N"), "Assunto", 67, 150, "", "", '', '', 0, 'id="evtassunto"' ); ?></td>
	</tr>
	<tr>
	  <td class="SubTituloDireita">Data de início:</td>
	  <td><? echo campo_data2('evtdataini','N', (($permissao['adicionarevento'])?"S":"N"), 'Data de início', 'S' ); ?> Hora : <? echo campo_texto('evtdatainihora', "S", (($permissao['adicionarevento'])?"S":"N"), "Hora inicío", 6, 5, "##:##", "", '', '', 0, 'id="evtdatainihora"' ); ?></td>
	</tr>
	<tr>
	  <td class="SubTituloDireita">Data de fim:</td>
	  <td><? echo campo_data2('evtdatafim','N', (($permissao['adicionarevento'])?"S":"N"), 'Data fim', 'S' ); ?> Hora : <? echo campo_texto('evtdatafimhora', "S", (($permissao['adicionarevento'])?"S":"N"), "Hora fim", 6, 5, "##:##", "", '', '', 0, 'id="evtdatafimhora"' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Pauta:</td>
		<td><? echo campo_textarea( 'evtpauta', 'S', (($permissao['adicionarevento'])?"S":"N"), '', '70', '4', '5000'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Programa:</td>
		<td>
			<?php
				$sql_combo = "
					SELECT preid AS codigo,
					       predescricao AS descricao
					FROM atendimento.programaevento
					ORDER BY predescricao
				";
				$db->monta_combo('preid', $sql_combo, 'S', 'Selecione', '', '', '', '200', 'S', 'preid');
			?>
		</td>
	</tr>
	<?
	if($tipopessoa[0]) :
	foreach($tipopessoa as $tp) :
	?>
	<tr>
	  <td class="SubTituloEsquerda" colspan="2"><b><?=$tp['tppdescricao'] ?></b> <? if($permissao['adicionarevento']) : ?><img src="../imagens/gif_inclui.gif" border="0" align="absmiddle" style="cursor:pointer;" onclick="inserirPessoaPorTipo('<?=$tp['tppid'] ?>');"><? endif; ?></td>
	</tr>
	<tr>
	  <td colspan="2" id="tipopessoas_<?=$tp['tppid'] ?>"></td>
	</tr>
	<tr>
		<td align="center" colspan="2"><input id="salvar" type="button" class="botao" value="Salvar" name="salvar" onclick="salvarParticipantes(<?=$tp['tppid'];?>);"></td>
	</tr>
	<?
	endforeach;
	endif;
	?>		
	<tr>
		<td class="SubTituloDireita">Resumo:</td>
		<td><? echo campo_textarea( 'evtobservacoes', 'S', (($permissao['adicionarevento'])?"S":"N"), '', '70', '4', '2000'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Encaminhamentos Gerais</td>
		<td><? echo campo_textarea( 'evtencaminhamentos', 'S', (($permissao['adicionarevento'])?"S":"N"), '', '70', '4', '2000'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">Encaminhamentos Por Pessoa</td>
	</tr>
	<tr>
		<td id="td_encaminhamentosporpessoa" colspan="2"></td>
	</tr>
	<? if($permissao['adicionarevento']) : ?>
	<tr>
		<td class="SubTituloCentro" colspan="2">
			<input type="button" name="salvar" value="Salvar" onclick="salvarEvento();"> 
			<input type="button" name="voltar" value="Voltar" onclick="window.location='atendimento.php?modulo=principal/evento&acao=A';"></td>
	</tr>
	<? endif; ?>
</table>
</form>

