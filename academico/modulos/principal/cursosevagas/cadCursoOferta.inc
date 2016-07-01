<?php 
$c = new CursosEdital();

$cdtid = 	   $_REQUEST['cdtid'];
$entid = 	   $_SESSION['academico']['entid'];
$entidCampus = $_REQUEST['entidCampus'];
$excid = 	   $_REQUEST['excid'];

if( $_REQUEST['salva'] ){
	$edtid = $_REQUEST['edital'];
	$cdtid = $_REQUEST['cdtid'];
	$ano   = $_REQUEST['ano'];
	
	$sql = "SELECT edtid FROM academico.editalexecucao WHERE cdtid = ".$cdtid." AND edeano = '".$ano."'";

	if( $db->pegaUm( $sql ) ){
		$sql = "UPDATE
					academico.editalexecucao
				SET
					edtid={$edtid}
				WHERE
				 	cdtid={$cdtid} AND
				 	edeano = '".$ano."'";
		$db->executar( $sql );
	} else {
		$sql = "INSERT INTO
					academico.editalexecucao ( edtid, cdtid, edeano )
				VALUES
					( {$edtid}, {$cdtid}, '{$ano}' )";
		$db->executar($sql);
			
	}
	if($db->commit()){			
		$db->sucesso('principal/cursosevagas/cadCursoOferta', '&cdtid='.$cdtid);
	} else {
		$db->insucesso('Falha na operação');
	}
	die;
}

switch ($_REQUEST['evento']){
	case 'listaCadExecCurso2010':
		header('Content-Type: text/html; charset=ISO-8859-15');
		$c->listaCadExecCurso2010($_REQUEST);
		die;
	break;
	case 'supertitle':
		header('Content-Type: text/html; charset=ISO-8859-15');
		$htm = $c->exibiDadosEditalCurso( $_REQUEST['edtid'] );
		die($htm);
	break;
}

$disabled 	   = 'S';
$displayButton = "display: none;"; 
if ($excid){
	$filtro['excid'] = $excid;
	$filtro['cdtid'] = $cdtid;
	
	$dados = $c->carregaExecucaocurso($filtro);
	extract($dados);
	$ano 	  		= $excanoexecucao;
	$disabled 		= 'N'; 
	$disabledButton = 'disabled="disabled"'; 
	$displayButton  = null;
	 
}

?>
<html>
	<head>
		<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
		<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
 
		<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<script language="JavaScript" src="./geral/js/academico.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		
		<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
		<script type="text/javascript" src="../includes/remedial.js"></script>
		<script type="text/javascript" src="../includes/superTitle.js"></script>
				
		<script type="text/javascript">
		d = document;
		
		function salvarCurso(){
			var txt = '';
			var preenchido = false;
			var retorno = true;
			
			edital = $("#editalCurso:checked").val();
			cdtid  = $("#cdtid").val();
			ano    = $("#ano").val();
			
			divCarregando();
			jQuery.ajax({
			   type: "POST",
			   url: "academico.php?modulo=principal/cursosevagas/cadCursoOferta&acao=A",
			   data: "&salva=true&edital="+edital+"&cdtid="+cdtid+"&ano="+ano,
			   success: function(res){
			   		alert('Dados salvos com sucesso!');
					divCarregado();
					window.close();
			   }
			 });
		}

		function filtraEditais(ano, cdtid){
				if (ano == ''){
					alert('Selecione o ano para efetuar a busca!');
					return false;
				}
				lista = $('#listaExecEditais');
				$(lista).html('<center><img src="/imagens/carregando.gif"><br>Carregando...</center>');
				$(lista).load('?modulo=principal/cursosevagas/cadCursoOferta&acao=A', {'evento' : 'listaCadExecCurso2010', 'edtano' : ano, 'cdtid' : cdtid}, function (res){
					if (res.indexOf('Não foram encontrados Registros') < 0){ 
						$('#salvar_fechar').show();
						$('#salvar').show();
					}else{
						$('#salvar_fechar').hide();
						$('#salvar').hide();
					}
					$('#excanoexecucao').val(ano);
				});
				
		}	

		</script>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
<?php
monta_titulo("Cadastro de Oferta do Curso<br><font size='2'>$tipoCurso</font>", obrigatorio() . ' Indica campo obrigatório' );
?>
<form method="POST"  name="formulario">
	<input type="hidden" name="evento" id="evento" value="manterExecCurso">
	<input type="hidden" name="excid" id="excid" value="<?=$excid?>">
	<input type="hidden" name="cdtid" id="cdtid" value="<?=$cdtid?>">
	<input type="hidden" name="ano" id="ano" value="<?=$_REQUEST['ano']?>">
	<input type="hidden" name="excanoexecucao" id="excanoexecucao" value="<?=$excanoexecucao ?>">

<center>
<div id="listaExecEditais" style="width:95%; height: 350px; overflow: auto; border:1px solid #ccc; border-top: 0px;"> 
<? 
	if (!$excid){
		echo '<font color="red">Escolha o ano para trazer seus respectivos editais</font>';
	}else{
		$c->listaCadExecCurso2010($filtro);
	}
?>
</div>

<div class="divButton" style="background: #ccc; text-align: center; width:95%; border: 1px solid #CCC; border-top: 0px;">  		
	<input name="salvar" id="salvar" type="button" value="Salvar/Fechar" onclick="salvarCurso()" style="<?=$displayButton ?>">
	&nbsp;&nbsp;
	<input type="button" value="Fechar" onclick="javascript:window.close();">		
</div>
</center>  
</form>      
</body>
</html>
<script>filtraEditais(<?=$_REQUEST['ano']?>, <?=$_REQUEST['cdtid']?>);</script>