<?

if($_POST['chkglosa']){
	
	$flag = false;
	foreach($_POST['chkglosa'] as $odsid=>$glosaid){
			
		$sql = "INSERT INTO fabrica.historicoglosadel(
			            glosaid, odsid, usucpf, hgddata)
			    VALUES ($glosaid, 
			    		$odsid,
			    		'".$_SESSION['usucpf']."', 
			    		now()
			    		)";
		$db->executar($sql);
		
		$sql = "UPDATE fabrica.ordemservico 
				SET glosaid = null 
				WHERE odsid = ".$odsid;
		$db->executar($sql);
		
		$flag = true;
	}
	
	if($flag) {
		$db->commit();
		die("<script>
				alert('Operação Efetuada com Sucesso.');
				location.href = 'fabrica.php?modulo=principal/listarGlosaOS&acao=A';
			</script>");
	}
	
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

echo '<br>';

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="./js/fabrica.js"></script>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script type="text/javascript">

function limparCampos(){
	$('input:not(:submit,:hidden,:button,[readonly=""],[readonly="readonly"]),textarea,select').val("");
}

function submeteFormulario()
{
	/*
	$('#usunomerequisitante option').each(function()
	{
		$(this).attr('selected',true);
	});
	$('#usufiscal option').each(function()
	{
		$(this).attr('selected',true);
	});
	*/
	if(confirm("<?=$_SESSION['usunome']?>,\n\nDeseja realmente apagar estas glosas?")){
		$('#form_listar').submit();
	}
}
</script>
<?php

$titulo_modulo = "Listar Glosa OS";
monta_titulo($titulo_modulo, '');



//recupera perfis de usuario
$pfls = arrayPerfil();

//permissao PERFIL_PREPOSTO
if(in_array(PERFIL_FISCAL_CONTRATO,$pfls) || in_array(PERFIL_SUPER_USUARIO,$pfls) || in_array(PERFIL_ADMINISTRADOR, $pfls)){
	
}
else{
	die("<script>
			alert('Seu perfil não possui acesso a esta tela.');
			history.back();
		</script>");
}


?>

<form name="formulario" id="form_listar" method="post" action="">

<?php

$sql = "SELECT
			'<center><input type=checkbox name=\"chkglosa['||os.odsid||']\" value=\"'||os.glosaid||'\"></center>' as acao, 
			os.odsid,
			os.scsid,
			glo.glosaqtdepf
		FROM fabrica.ordemservico os 
		LEFT JOIN fabrica.glosa glo ON glo.glosaid=os.glosaid
		WHERE os.glosaid is not null and odsdtprevinicio > '2013-01-01'
		order by 1";

$cabecalho     = array("Selecionar","Nº OS", "Nº SS", "Glosa (PF)");
$tamanho       = array("10%","10%","20%");
$alinhamento   = array("center","center","center");
$db->monta_lista_simples($sql,$cabecalho,500,5,'N','95%');

?>
<center>
<input type="button" value="Apagar Glosas" onclick="submeteFormulario();">
</center>

</form>

