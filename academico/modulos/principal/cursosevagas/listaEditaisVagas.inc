<?php
$entid = $_SESSION['academico']['entid'] ? $_SESSION['academico']['entid'] : $_REQUEST['entidunidade'] ;
$c     = new CursosEdital();

switch ($_REQUEST['evento']){
	case 'excluir':
		$retorno = $c->manterEditalcurso(array("edtstatus" => "I"), array("edtid" => $_REQUEST['edtid']));
		$retorno ? 
			$db->sucesso($_REQUEST['modulo']) 
		: 
			redir('', 'Falha na operação!\nHouve alguma incompatibilidade nos dados enviados.');
		
	break;
	case 'listaEditais':
		header('Content-Type: text/html; charset=iso-8859-1');
		$c->listaEditalCurso();
		die;
	break;
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// Monta as abas
#$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( 'Lista de Editais de Oferta de Vagas', '' );

$_SESSION["academico"]["edttipo"]  = $_REQUEST["edttipo"] ? $_REQUEST["edttipo"] : "G";
?>
<div class='col-md-2' style='position:static;'>
    <?=cria_aba_2($abacod_tela, $url, $parametros);?>
</div>
<div class='col-md-9' style='position:static;'>
    <?
    // cria o cabeçalho padrão do módulo
    $autoriazacaoconcursos = new autoriazacaoconcursos();
    echo $autoriazacaoconcursos->cabecalho_entidade($entid);
print "<br/>";
print acaMenusTipoCursoEdital( $_REQUEST["acao"], $_SESSION["academico"]["edttipo"] );

?>

<script language="JavaScript" src="./geral/js/academico.js"></script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="subtitulocentro">Lista de Editais</td>
	</tr>
</table>

<?php 

$c->listaEditalCurso(); 

?> 	

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr bgcolor="#D0D0D0">
		<td>
			<input type="button" value="Inserir Edital" onclick="janela('?modulo=principal/cursosevagas/cadCursoVaga&acao=A', 600, 620, 'cadEditalVaga');">
		</td>
	</tr>
</table>
</div>