<?php
// empreendimento || obra || orgao
verificaSessao( 'obra' );

$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];

switch ( $_POST['op'] ){
	case 'alterar':
		$_SESSION['obras2']['sfndeid'] = $_POST['sfndeid'];
		header('Location: ?modulo=principal/cadSupervisaoFNDE&acao=E');
		die;
	case 'apagar':
		$supervisaoFnde = new SupervisaoFNDE( $_POST['sfndeid'] );
		$supervisaoFnde->sfndestatus = 'I';
		$supervisaoFnde->salvar();
		
		$db->commit();
		die("<script>
				alert('Operação realizada com sucesso!');
				location.href = '?modulo=principal/listaSupervisaoFNDE&acao=A';
			 </script>");
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);

echo cabecalhoObra($obrid);
monta_titulo( 'Lista de Supervisão FNDE', '' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<div style="height: 300px; overflow: auto;"> 
<?php
$supervisaoFnde = new SupervisaoFNDE();
$param   		= array();
$param['empid'] = $empid;
$sql = $supervisaoFnde->listaSql( $param );
$cabecalho = array( "Ação",
                    "Situação da Supervisão",
                    "Data da Supervisão",
                    "Data de Inclusão",
                    "Responsável",
                    "Cargo do Responsável",
                    "Inserido Por",
					"Unidade em Funcionamento?");
$db->monta_lista($sql,$cabecalho,20,5,'N','center',$par2, "formulario");
?>
</div>
<form id="formSupervisaoFnde" action="" method="post">
	<center>
	<input type="hidden" name="op" id="op">
	<input type="hidden" name="sfndeid" id="sfndeid">
	<div style="width: 95%; height: 23px; background: #C0C0C0;">
		<input type="button" value="Nova Supervisão" onclick="novaSupervisaoFNDE();" style="float: left; margin-left: 30px;">
	</div>
	</center>
</form>

<?php
        $objObras = new Obras($obrid);
        $blockEdicao = $objObras->verificaObraVinculada();
        if($blockEdicao){
            echo '<script type="text/javascript">';
            echo " setTimeout(bloqueiaForm('formSupervisaoFnde1'), 500);
                   function bloqueiaForm(idForm){
                      jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                      jQuery('#gerarXls').attr('disabled', false);
                   }
                 ";
            echo '</script>';
        }
?>

<script type="text/javascript">
<!--
function novaSupervisaoFNDE(){
	location.href = '?modulo=principal/cadSupervisaoFNDE&acao=A';
}

function alterarSupFNDE( sfndeid ){
	$('#op').val('alterar');
	$('#sfndeid').val(sfndeid);
	$('#formSupervisaoFnde').submit();
}

function excluirSupFNDE( sfndeid ){
	if ( confirm('Deseja apagar esta supervisão?') ){
		$('#op').val('apagar');
		$('#sfndeid').val(sfndeid);
		$('#formSupervisaoFnde').submit();	
	}
}
//-->
</script>