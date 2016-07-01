<?php
if( $_POST['ajaxUnidade'] && $_POST['ajaxProcesso']){
	 $_SESSION['unidade'] = $_POST['ajaxUnidade'];
	 $_SESSION['copid']   = $_POST['ajaxProcesso'];
	 die;
}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>'; 
$db->cria_aba( $abacod_tela, $url, '' );
montaCabecalhoProcesso($_SESSION['copid'], false);
monta_titulo( "Lista de Unidades Participantes", '' );
$sql = "select distinct
			'<center><a href=\"javascript: selecionaUnidade('|| u.usgid ||', '|| p.copid ||');\"><img align=\"absmiddle\" src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\"></a>
			<img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"removerAdesao('|| u.usgid ||', '|| p.copid ||');\" title=\"Excluir Adesão\"/></center>' as acao,
			u.usgcod as cod,
			u.usggestaouasg,
			u.usgdsc as unidade,
			ed.esddsc,
			case when dec.decid is not null then '<center><img src=\"../imagens/ico_html.gif\" width=20 height=20 border=0 title=\"Visualizar Declaração\" style=\"cursor:pointer;\"  onclick=\"window.open(\'?modulo=principal/visualizarDeclaracao&acao=A&decid='|| dec.decid ||'\',\'declaracao\',\'toolbar=no,location=no,status=no,menubar=no,scrollbars=yes,resizable=no,width=800,height=600\');\"></center>'
			else '<center><img src=\"../imagens/ico_html_gray.gif\" width=20 height=20 border=0 title=\"Sem Declaração\"></center>'
			end as declaracao
		FROM evento.uasg u
			INNER JOIN evento.coadesao a on u.usgid = a.usgid
			INNER JOIN evento.coprocesso p on a.copid = p.copid
			LEFT JOIN workflow.documento d on d.docid = a.docid
			LEFT JOIN workflow.estadodocumento ed on d.esdid = ed.esdid 
			LEFT JOIN evento.declaracao dec on u.usgid = dec.usgid and dec.decstatus = 'A' AND dec.copid = p.copid
			LEFT JOIN evento.couasgnivelcontratacao nc on nc.usgid = u.usgid AND p.conid = nc.conid
		WHERE p.copid = {$_SESSION['copid']} ";
$cabecalho = array("Ação","Código UASG","Código Gestão","Unidade", "Situação", "Declaração");
$db->monta_lista($sql,$cabecalho,200,10,'N','center','');
?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" >
function selecionaUnidade( id, copid ){
	var req = new Ajax.Request(window.location.href, {
		        method:     'post',
		        parameters: '&ajaxUnidade='+id+'&ajaxProcesso='+copid,							         
		        onComplete: function (res) {
					window.open('evento.php?modulo=principal/cadCompraInfra&acao=A&boExibeCabecalho=N','','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=1000,height=500');
				}
		});
}

function removerAdesao( id, copid ) {
 
	if ( confirm( 'Deseja excluir a UASG do Processo?' ) ) {
		window.location.href = 'evento.php?modulo=principal/unidadesParticipantes&acao=A&remover='+id+copid;
	}
}

</script>