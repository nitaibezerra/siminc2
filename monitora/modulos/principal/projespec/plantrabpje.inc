<?
/*
Sistema Simec
Setor responsavel: SPO-MEC
Desenvolvedor: Equipe Consultores Simec
Analista: Gilberto Arruda Cerqueira Xavier
Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Fabrício Mendonça(fabriciomendonca@gmail.com)
Modificacoes: Thiago Mata thiago_mata@yahoo.com.br 9 de novembro de 2006
Módulo:plantrabpje.inc
Finalidade: permitir o controle de cadastro de atividades num plano de trabalho de projeto especial
*/

//dbg( var_export( $_REQUEST ) , 1 );
$pjeid = $_REQUEST['id'];
if (! is_numeric($pjeid)) $pjeid=$_REQUEST['id'];
if ($_REQUEST['id'])  $_SESSION['pjeid']= $pjeid;

require_once( 'plantrabpje_funcoes.inc' );

$_SESSION[ 'showForm' ] = false;
$_SESSION[ 'coordpje' ] = false;
$pjeid=$_SESSION['pjeid'];

if ($_REQUEST['abrirarvore'])
{
  $parametro = "&abrirarvore=1" ;
}
else $parametro = "&abrirarvore=0" ;

$modulo = $_REQUEST['modulo'];//

if (! $_SESSION['pjeid'])
{
	?>
	<html>
	<body>
		<script>
			alert( 'Escolha primeiro o Projeto Especial!' );
			history.back();
		</script>
	</body>
	</html>
	<?
	$db->close();
	exit();

}


if ($_REQUEST['act'] == 'inserir')
{
	inserirPlanoTrabalho( $ptotipo, $ptoordem ,$ptoordem2 , $ungabrev,$modulo,$parametro);
}
if ($_REQUEST['act']=='alterar')
{
	alterarPlanoTrabalho( $ptotipo, $ptoordem , $ptoordem2, $ungabrev,$modulo,$parametro);
}
if ( $_REQUEST[ 'arrCod' ] )
{
	atualizarDatasdasAtividades( $ptotipo , $ptoordem , $ptoordem2,$modulo);
}
if ($_REQUEST['act']=='aprov')
{
	aprovarAtividade( $modulo , $ptotipo , $ptoordem , $ptoordem2,$modulo);
}
if ($_REQUEST['act']=='aprovalote')
{
	aprovarLotedeAtividades( $modulo , $pjeid,$modulo );
}
if ($_REQUEST['act']=='retorno')
{
	retornarAtividadeParaEdicao( $modulo , $ptotipo , $ptoordem , $ptoordem2,$modulo);
}
if ($_POST['exclui'])
{
	excluirAtividade( $modulo , $_REQUEST['exclui']);
}
chamaCabecalho();
include_once( APPRAIZ."includes/cabecalho.inc" );
//require_once( 'plantrabpje_apresentacao.inc' );
require_once( 'plantrabpje_apresentacao.inc' );
?>
