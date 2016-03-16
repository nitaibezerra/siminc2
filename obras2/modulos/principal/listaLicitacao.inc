<?php
$arOrgid = verificaAcessoEmOrgid();
if ( !in_array( $_SESSION['obras2']['orgid'], $arOrgid ) ){
	$_SESSION['obras2']['orgid'] = '';
}

// empreendimento || obra || orgao
verificaSessao( 'orgao' );

$_SESSION['obras2']['orgid'] = 3; //$_REQUEST['orgid'] ? $_REQUEST['orgid'] : $_SESSION['obras2']['orgid'];
$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : current( $arOrgid ));
$orgid 						 = $_SESSION['obras2']['orgid'];

//$_SESSION['obras2']['orgid'] = $_REQUEST['orgid'] ? $_REQUEST['orgid'] : $_SESSION['obras2']['orgid'];
//$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : ORGID_EDUCACAO_SUPERIOR);
//$orgid 						 = $_SESSION['obras2']['orgid'];

// Limpa-se o obrid, pois na edição da licitação, caso haja obrid subentende-se que provém da edição da licitação pela OBRA.
// Logo retornará para obra após ser salvo. 
unset( $_SESSION['obras2']['obrid'] );

switch ( $_POST['op'] ){
	case 'apagar':
		if ( $_POST['licid'] ){
			$licitacao = new Licitacao( $_POST['licid'] );
			$licitacao->licstatus = 'I';
			$licitacao->salvar();
			
			$obraLic = new ObraLicitacao();
			$obraLic->apagaPorLicitacao( $_POST['licid'] );
			
			$contrato = new Contrato();
			$contrato->apagaPorLicitacao( $_POST['licid'] );
			
			$obraContrato = new ObrasContrato();
			$obraContrato->apagaPorLicitacao( $_POST['licid'] );
			
			$db->commit();
		}
		die('<script>
				alert(\'Operação realizada com sucesso!\');
				location.href=\'?modulo=principal/listaLicitacao&acao=A\';
			 </script>');
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

$arAba = getArAba('listalicitacao');
echo montarAbasArray($arAba, "?modulo=principal/listaLicitacao&acao=A&orgid=" . $orgid);

monta_titulo($titulo_modulo, '');
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
function alterarLic( licid ){
	window.location = '?modulo=principal/cadLicitacao&acao=E&licid=' + licid;
}

function excluirLic( licid ){
	if ( confirm('Deseja apagar esta licitação?') ){
		var form = $('<form>').attr('method', 'post')
							  .attr('action', '')
							  .append( 
							   			$('<input>').attr('name', 'licid')
							   						.attr('type', 'hidden')
							   						.attr('value', licid)
							   		   )
							  .append( 
							   			$('<input>').attr('name', 'op')
							   						.attr('type', 'hidden')
							   						.attr('value', 'apagar')
							   		   )
		$(document.body).append( form );
		form.submit();

	}			   
}
</script>
<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td>
			<a title="Adicionar Nova Licitacao" href="?modulo=principal/cadLicitacao&acao=A">[Nova Licitação]</a>
		</td>
	</tr>
</table>
<?php
$licitacao 		 = new Licitacao();
$filtro 		 = $_POST;
$filtro['orgid'] = $orgid;

$cabecalho = array("Ação","Número da Licitação", "Modalidade");
$db->monta_lista($licitacao->listaSql( $filtro ),$cabecalho,100,5,'N','center',$par2, "formulario");
?>
