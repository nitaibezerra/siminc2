<?
require_once( 'pg_funcoes.inc' );

$_SESSION[ 'showForm' ] = false;
$_SESSION[ 'coordpje' ] = false;

print_r( $_REQUEST  );
exit();
//$pjeid					=	@$_SESSION[ 'pjeid' ];
$pjeid					=	@$_REQUEST[ 'pjeid' ];
$modulo					=	@$_REQUEST[ 'modulo' ];
$strAcao				=	@$_REQUEST[ 'act' ];

if( $_REQUEST[ 'arrCod' ] ) $strAcao = 'atualizaLote';
if( $_POST['exclui'] )		$strAcao = 'excluir';  

if( !$intPjeId )
{
	require_once( 'pg_apresentacao_erro.php' );
}
else
{
	switch( $strAcao )
	{
		case 'inserir':
		{
			inserirPlanoTrabalho( $ptotipo, $ptoordem ,$ptoordem2 , $ungabrev,$modulo );
			break;		
		}
		case 'alterar':
		{
			alterarPlanoTrabalho( $ptotipo, $ptoordem , $ptoordem2, $ungabrev,$modulo );
			break;		
		}
		case 'aprov':
		{
			aprovarAtividade( $modulo , $ptotipo , $ptoordem , $ptoordem2,$modulo );
			break;		
		}
		case 'aprovaLote':
		{
			aprovarLotedeAtividades( $modulo , $pjeid,$modulo );
			break;	
		}
		case 'atualizaLote':
		{
			atualizarDatasdasAtividades( $ptotipo , $ptoordem , $ptoordem2,$modulo );
			break;		
		}
		case 'retorno':
		{
			retornarAtividadeParaEdicao( $modulo , $ptotipo , $ptoordem , $ptoordem2,$modulo );
			break;		
		}
		case 'excluir':
		{
			excluirAtividade( $modulo , $ptotipo , $ptoordem , $ptoordem2,$modulo );
			break;
		}
		default:
		{
			throw new Exception( 'Acao desconhecida para Plano Gerencial' );
		}
	}
}

chamaCabecalho();
include_once( APPRAIZ."includes/cabecalho.inc" );
require_once( 'pg_apresentacao.inc' );
?>
