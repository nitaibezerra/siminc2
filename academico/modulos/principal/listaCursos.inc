<?php
$autoriazacaoconcursos = new autoriazacaoconcursos();
$curso = new cursos();

unset($_SESSION["academico"]["curid"]);

if ($_REQUEST['listaCursosAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	$curso->listaCursosAjax($_REQUEST['modalidade'], $_REQUEST['entid']);
	exit;
}
if ($_REQUEST['excluiCursosAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	$curso->excluiCursosAjax($_REQUEST['curid']);
	exit;
}

require_once APPRAIZ . "includes/classes/entidades.class.inc";
require_once APPRAIZ . "includes/cabecalho.inc";

echo '<br/>';

$_SESSION['academico']['entidcampus'] = $_REQUEST['entid'] ? $_REQUEST['entid'] : $_SESSION['academico']['entidcampus'];  
$entidcampus = $_SESSION['academico']['entidcampus'] ? $_SESSION['academico']['entidcampus'] : $_REQUEST['entid']; 

$existecampus = academico_existecampus( $entidcampus );

if( !$existecampus ){
	echo "<script>
			alert('O Campus informado não existe!');
			history.back(-1);
		  </script>";
	die;
}
// Busca a unidade pai do Campus
$autoriazacaoconcursos = new autoriazacaoconcursos();
$_SESSION['academico']['entid'] = $autoriazacaoconcursos->buscaentidade($entidcampus);

if($entidcampus){
	$_SESSION['academico']['entidadenivel'] = 'campus';
}

$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Lista de Cursos", "");

// mensagem de bloqueio
$bloqueado = academico_mensagem_bloqueio($_SESSION['academico']['orgid']);

// cria o cabeçalho padrão do módulo
echo $autoriazacaoconcursos->cabecalho_entidade($entidcampus);

?>
<body>

<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="formCurso" name="formCurso" action="" method="post" enctype="multipart/form-data" >
	<input type="hidden" value="<?=$entidcampus ?>" name="hidEntidade" id="hidEntidade">
	<input type="hidden" value="<?=$tipocurso ?>" name="hidTipoCurso" id="hidTipoCurso">
	
	<table id="tblCurso" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td colspan="2">&nbsp;</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 250px;"><b>Informe a Modalidade:</b></td>
			<td><label for="modalidade1"><input type="radio" name="modalidade" id="modalidade1" value="1" checked="checked" onclick="listaCursos();"> Cursos de Graduação</label>
				<label for="modalidade7"><input type="radio" name="modalidade" id="modalidade7" value="7" onclick="listaCursos();"> Cursos de Pós-Graduação</label></td>
		</tr>
	</table>
</form>
<div id="lista" style="display: ''"></div>
<table id="tblCurso" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<th colspan="2" style="text-align: left;"><input style="cursor: pointer;" type="button" name="btnNovo" value="Inserir Curso" onclick="novoCurso();"></th>
	</tr>
</table>

<script type="text/javascript">
	listaCursos();
	
	function listaCursos(){
		var req = new Ajax.Request('academico.php?modulo=principal/listaCursos&acao=A', {
			        method:     'post',
			        parameters: '&listaCursosAjax=true&modalidade='+validaCheck()+
			        					'&entid='+$('hidEntidade').value,
			        asynchronous: false,
			        onComplete: function (res){	
						$('lista').innerHTML = res.responseText;
						$('hidTipoCurso').value = validaCheck();
			        }
			  });
	}
	function validaCheck(){
		var f = formCurso;
		var check = "";
		
		for(i=0; i<f.length; i++){
			if(f.elements[i].type == "radio"){
				if(f.elements[i].checked){
					check = f.elements[i].value;
				}
			}
		} 
		return check;
	}
	function novoCurso(){
		window.location.href = 'academico.php?modulo=principal/cadastroCursos&acao=A&tipoCurso='+$('hidTipoCurso').value;
	}
	function alterarCurso(curid){
		window.location.href = 'academico.php?modulo=principal/cadastroCursos&acao=A&tipoCurso='+$('hidTipoCurso').value+'&curid='+curid;
	}
	function excluiCursos(curid){
		if(confirm("Tem certeza que deseja excluir este registro?") ){	
			var req = new Ajax.Request('academico.php?modulo=principal/listaCursos&acao=A', {
				        method:     'post',
				        parameters: '&excluiCursosAjax=true&curid='+curid,
				        asynchronous: false,
				        onComplete: function (res){	
	
							if(res.responseText == 1){
								alert('Operação realizada com sucesso!');
								listaCursos();
							}else{
								alert('Operação não realizada!');
							}
				        }
				  });
		}
	}
</script>
</body>