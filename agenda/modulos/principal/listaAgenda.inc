<?php

switch ( $_REQUEST['op'] ){
	case 'excluir':
		if ( $_REQUEST['agdid'] ){
			$msg 	= 'Operação realizada com sucesso!';
			$agenda = new Agenda( $_REQUEST['agdid'] );
			$agenda->popularDadosObjeto( array('agdstatus' => 'I') )
				   ->salvar();
			$db->commit();
		}else{
			$msg = 'Falha na execução da operação!';
		}	
			
		die('<script>
				alert(\'' . $msg . '\');
				location.href=\'?modulo=principal/listaAgenda&acao=A\';
			 </script>');
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Pesquisa de Agendas', '<img align="absmiddle" src="/imagens/principal.gif" title="Eventos da Agenda"> Ir para eventos da agenda');

$agenda = new Agenda();
$sql 	= $agenda->listaSQL( $_POST );

$cabecalho = array("Ação", "Nome");
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");
?>
<?php 
if ( verificaPerfil( array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR, PERFIL_GESTOR_MEC) ) ){
?>
<table class="tabela" align="center">
	<tr bgcolor="#CCCCCC">
	   <td>
	   	<input type="button" name="btalterar" value="Nova Agenda" onclick="location.href='?modulo=principal/cadastroAgenda&acao=A';" class="botao">
	   </td>
	</tr> 	
</table>
<?php 
}
?>
<script>
function alterarAgenda(agdid){
	location.href = '?modulo=principal/cadastroAgenda&acao=E&agdid=' + agdid;
}
function andamentoAgenda(agdid){
	location.href = '?modulo=relatorio/movimentoAgenda&acao=A&agdid=' + agdid;
}

function eventoAgenda(agdid){
	location.href = '?modulo=principal/listaEvento&acao=A&agdid=' + agdid;
}

function excluirAgenda(agdid){
	if ( confirm('Deseja apagar essa agenda?') ){
		location.href = '?modulo=principal/listaAgenda&acao=A&op=excluir&agdid=' + agdid;
	}
}
</script>