<?php
$c = new CursosEdital();


$_SESSION['academico']['cdtid'] = $_REQUEST['cdtid'] ? $_REQUEST['cdtid'] : $_SESSION['academico']['cdtid'];
$cdtid = $_SESSION['academico']['cdtid'];

$_SESSION['academico']['entidcampus'] = $_REQUEST['entidcampus'] ?  $_REQUEST['entidcampus'] : $_SESSION['academico']['entidcampus']; 

switch ($_REQUEST['evento']){
	case 'listaExecCursos':
		header('Content-Type: text/html; charset=ISO-8859-15');
		if ($_REQUEST['listadetalhe']){
			$param['listadetalhe'] = true;
		}
		
		$c->listaExecCursos($_REQUEST, $param);
		exit;
	break;	
	case 'excluirExec':
		$retorno = $c->manterExecucaocurso(array("excstatus" => "I"), array("excid" => $_REQUEST['excid']));
		$retorno ? 
			$db->sucesso($_REQUEST['modulo']) 
		: 
			redir('', 'Não foi possível excluir o registro.\nTente novamente!');
		exit;
	break;	
	case 'supertitle':
		header('Content-Type: text/html; charset=ISO-8859-15');
		$htm = $c->exibiDadosEditalCurso( $_REQUEST['edtid'] );
		die($htm);
	break;	
}


include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// Monta as abas
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( 'Execução do Curso', '' );
echo $c->cabecalhoCurso($cdtid);

?>
<script language="JavaScript" src="./geral/js/academico.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
<script type="text/javascript" src="../includes/remedial.js"></script>
<script type="text/javascript" src="../includes/superTitle.js"></script>

<center>
<div id="listaExecCurso" style="width:95%; height: 270px; overflow: auto; border:1px solid #ccc; border-top: 0px;"> 
<?
// Lista de execucao de cursos cursos
$c->listaExecCursos();
?>
</div>
<div class="divButton" style="background: #ccc; text-align: left; width:95%; border: 1px solid #CCC; border-top: 0px;">
	<input type="button" value="Inserir Oferta" onclick="janela('?modulo=principal/cursosevagas/cadCursoOferta&acao=A', 600, 480, 'cadCurso');">&nbsp;&nbsp;
	<input type="button" value="Voltar" onclick="location.href='?modulo=principal/cursosevagas/listaCurso&acao=A';">
</div>
</center>