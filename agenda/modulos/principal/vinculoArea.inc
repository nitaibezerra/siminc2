<?php
/** ATENÇÃO **
 * &acao=A (Cadastro)
 * &acao=E (Edição)
 */

//if ( $_POST['op'] == 'salvar' ){
switch ($_POST['op']){
	case 'salvar':
		$eventoArea 	= new EventoArea( $_POST['evaid'] );
		$dados 			= $_POST;
		$dados['eveid'] = $_SESSION['agenda']['eveid'];
		$evaid 			= $eventoArea->popularDadosObjeto( $dados )
			   						 ->salvar();
		
		/*** Insere evento/area no workflow ***/
		pegaDocidEventoArea( $evaid );
		$db->commit();
		
		$_SESSION['agenda']['evaid'] = $evaid;
		die('<script>
				alert(\'Operação realizada com sucesso!\');
				location.href=\'?modulo=principal/vinculoArea&acao=E\';
			 </script>');
	case 'excluirParecer':
		if ( $_REQUEST['hseid'] ){
			$msg 	= 'Operação realizada com sucesso!';
			$histEncaminhamento = new HistEncaminhamento( $_REQUEST['hseid'] );
			$histEncaminhamento->popularDadosObjeto( array('hsestatus' => 'I') )
				   			   ->salvar();
			$db->commit();
		}else{
			$msg = 'Falha na execução da operação!';
		}	
			
		die('<script>
				alert(\'' . $msg . '\');
				location.href=\'?modulo=principal/vinculoArea&acao=E\';
			 </script>');
}

switch ($_REQUEST['ajax']){
	case 'dirigente':
		header('content-type: text/html; charset=ISO-8859-1');
		$aevid = $_POST['aevid'];
		if ( $aevid ){
			$dirigente = new DirigentesMec();
			$sql = $dirigente->listaSimples( array('aevid' => $aevid) );
			
			$cabecalho = array("Nome", "Cargo", "E-mail", "Telefone");
			$db->monta_lista_simples($sql,$cabecalho,100,5,'N','95%');
		}else{
			echo '<span>selecione uma área para visualizar seus dirigentes.</span>';	
		}
		exit;		
}

$agdid = $_SESSION['agenda']['agdid'];
$eveid = $_SESSION['agenda']['eveid'];

if ( $_GET['acao'] == 'E' ){
	if ( empty($_REQUEST['evaid']) && empty($_SESSION['agenda']['evaid']) ){
		die('<script>
				location.href=\'?modulo=principal/vinculoArea&acao=A\';
			 </script>');		
	}
	$evaid 						 = ( $_REQUEST['evaid'] ? $_REQUEST['evaid'] : $_SESSION['agenda']['evaid']);
	$_SESSION['agenda']['evaid'] = $evaid;
	
	$eventoArea = new EventoArea( $evaid );
	$dados  = $eventoArea->getDados();
	extract( $dados );	
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$arAba = abaEdicaoEvento('area');
echo montarAbasArray($arAba, "?modulo=principal/vinculoArea&acao=A");
monta_titulo( 'Área Vinculada', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.');


?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
<!--
function validarForm(){
	if ( $.trim( $('[name=aevid]').val() ) == '' ){
		alert('O preenchimento do campo "Área" é obrigatório.');
		return;
	}
	$('[name=op]').val('salvar');
	$('[name=formVinculo]').submit();
}

function cadastrarParecer(evaid, hseid){
	hseid = hseid || '';
	var url = (hseid ? '?modulo=principal/cadastroParecer&acao=E&hseid=' + hseid : '?modulo=principal/cadastroParecer&acao=P');
	janela(url,600,280,'popupcadastro');
}

function carregaDirigente(aevid){
					var td	= $('#td_lista_dirigente');
					if ( aevid != '' ){
						var url = location.href;
						$.ajax({
									  url  		 : url,
									  type 		 : 'post',
									  data 		 : {ajax  : 'dirigente', 
									  		  	    aevid : aevid},
									  dataType   : "html",
									  async		 : false,
									  beforeSend : function (){
									  	divCarregando();
										td.html('carregando...');
									  },
									  error 	 : function (){
									  	divCarregado();
									  },
									  success	 : function ( data ){
									  	td.html( data );
									  	divCarregado();
									  }
								});	
					}else{
						td.html('<span>selecione uma área para visualizar seus dirigentes.</span>');
					}			
}

$(document).ready(function (){
	var aevid = $('#aevid').val();
	if ( aevid ){
		carregaDirigente(aevid);
	}
});

//function alterarVinculo( evaid ){
//	$('#evaid').val( evaid );
//	$('#formVinculo').attr('action', '?modulo=principal/vinculoArea&acao=E')
//					 .submit();
//}

function excluirParecer( hseid ){
	if ( confirm('Deseja apagar esse parecer?') ){
		$('[name=op]').val('excluirParecer');
		$('[name=hseid]').val( hseid );
		$('[name=formVinculo]').submit();
	}	
}

//-->
</script>
<form name="formVinculo" id="formVinculo" method="post" action="">
<input name="op" id="op" value="" type="hidden">
<input name="evaid" id="evaid" value="<?php echo $evaid; ?>" type="hidden">
<input name="hseid" id="hseid" value="" type="hidden">
<?php 
if ( $_GET['acao'] == 'A' ){
	require_once('vinculoAreaCadastro.inc');	
}else{
	require_once('vinculoAreaEdicao.inc');	
}
?>
</form>

