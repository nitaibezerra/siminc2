<?php
// mensagem de bloqueio
$bloqueado = academico_mensagem_bloqueio($_SESSION['academico']['orgid']);
$entid	   = $_SESSION['academico']['entid'];

$autoriazacaoconcursos = new autoriazacaoconcursos();
$c = new CursosEdital();

switch ($_REQUEST['evento']){
	case 'listaEquivalencia':
		header('Content-Type: text/html; charset=ISO-8859-15');
		$param = array(
						'tipoLista' => $_REQUEST['tipoLista']
					   );
		$c->listaEquivalencia(null , $param );
		exit;
	break;	
	case 'excluirEquiv':
		$retorno = $c->deletarGrupoequivalencia($_REQUEST['greid']);
		$retorno ? 
			$db->sucesso($_REQUEST['modulo']) 
		: 
			redir('', 'Não foi possível excluir o registro.\nTente novamente!');
		exit;
	break;	
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$_SESSION["academico"]["tipocurso"]  = $_REQUEST["tpcid"] ? $_REQUEST["tpcid"] : 1;

// Monta as abas
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( 'Lista Equivalência de Cursos', '' );
// cria o cabeçalho padrão do módulo
echo $autoriazacaoconcursos->cabecalho_entidade($entid);
?>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="JavaScript" src="./geral/js/academico.js"></script>

<br/>

<?php 

echo acaMenusTipoCurso( $_REQUEST["acao"], $_SESSION["academico"]["tipocurso"], "listaEquivalencia" );

 

// Lista de cursos
$c->listaEquivalencia();

?>


<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr bgcolor="#D0D0D0">
		<td>
			<input type="button" value="Inserir Equivalência" onclick="redireciona('?modulo=principal/cursosevagas/cadEquivalencia&acao=A');">
		</td>
	</tr>
</table>

<script>
function listaEquivalencia(tipoLista){
	$('#listaEquivalencia').html('<center><img src="/imagens/carregando.gif"><br>Carregando...</center>');
	$('#listaEquivalencia').load('?modulo=principal/cursosevagas/listaEquivalencia&acao=A&evento=listaEquivalencia', {'tipoLista' : tipoLista});
}
</script>