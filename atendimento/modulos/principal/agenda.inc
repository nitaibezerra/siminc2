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


//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
jQuery(document).ready(function() {
	listarAgendas();
});

function listarAgendas() {
	jQuery('#listaagendas').html('Carregando...');
	jQuery.ajax({
   		type: "POST",
   		url: "atendimento.php?modulo=principal/agenda&acao=A",
   		data: "requisicao=listarAgendas",
   		async: false,
   		success: function(html){jQuery('#listaagendas').html(html);}
 		});
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

function atualizarAgenda(agddescricao, agdid) {
	jQuery.ajax({
   		type: "POST",
   		url: "atendimento.php?modulo=principal/agenda&acao=A",
   		data: "requisicao=atualizarAgenda&agddescricao="+agddescricao+"&agdid="+agdid,
   		async: false,
   		success: function(html){jQuery('#listaagendas').html(html);}
 		});
}

function adicionarAgenda() {
	agddescricao = prompt('Agenda:');
	if(agddescricao) {
		inserirAgenda(agddescricao);
		listarAgendas();
	}


}
function excluirAgenda(agdid){
	if( confirm('Tem certeza que deseja excluir a agenda?') ){
		window.location='atendimento.php?modulo=principal/evento&acao=A&requisicao=apagarAgenda&agdid='+agdid;
	} 	else {
		return false;
	}
}	


function editarAgenda(descricao,id) {
	agddescricao = prompt('Agenda:',descricao);
	
	if(agddescricao) {
		atualizarAgenda(agddescricao,id);
		listarAgendas();
	}

}
</script>

<? 	

monta_titulo( 'Agendas', 'Selecione a agenda' );

$permissao = permissoesPerfil();

?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<? if($permissao['adicionaragenda']) : ?>
	<tr>
	  <td class="SubTituloDireita"><a style="cursor:pointer;" onclick="adicionarAgenda();"><img src="../imagens/gif_inclui.gif" border="0" align="absmiddle"> Adicionar Agenda</a></td>
	</tr>
<? endif; ?>
	<tr>
	  <td id="listaagendas"></td>
	</tr>
</table>
