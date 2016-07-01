<?
 /*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Programador: Fernando Bagno (fernandobagno@gmail.com)
 */

if ( $_REQUEST["carga"] ){
	
	$esdid = $_REQUEST["carga"];
	$ies->montaListaInstituicoes( $esdid, $filtro );
	die;
	
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

print "<br/>";
monta_titulo( "Programa IES MEC / BNDES", "" );

if ( iesPossuiPerfil( IES_CONSULTAGERAL ) || iesPossuiPerfil( IES_VALIDACAOSESU ) || $db->testa_superuser() || iesPossuiPerfil( IES_ADMINISTRADOR ) || iesPossuiPerfil( IES_COMISSAOAVALIADORA ) ){
	$ies->montaInicioValidacao();	
}else{
	
	$_SESSION["ies"]["iesid"] = iesVerificaPermissaoIes( $_SESSION["usucpf"] );
	$ies->montaInicioCadastrador( $_SESSION["ies"]["iesid"] );
}

?>
<script language="JavaScript" src="./geral/js/ies.js"></script>

<script>
var params;

function desabilitarConteudo( id ){
	
	var url = 'ies.php?modulo=inicio&acao=C&carga='+id;
	if ( document.getElementById('img'+id).name == '-' ) {
		url = url + '&subAcao=retirarCarga';
		var myAjax = new Ajax.Request(
			url,
			{
				method: 'post',
				asynchronous: false
			});
	}
	
}
</script>