<?php
header('content-type: text/html; charset=ISO-8859-1');
?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<link rel="stylesheet" type="text/css" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css"></link>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>


<script type="text/javascript">

   

    function fecharPopup() {
        window.close();
    }

    function imprimir(this) {

    }


</script> 

<?php
$mnmid = $_SESSION['maismedicomec']['mnmid'];
$mntid = $_SESSION['maismedicomec']['mntid'];


if ($mnmid != ''){
	if ($_REQUEST['tipo'] == '2'){
    	$dados = buscarDadosMantidaCandidata($mnmid);
	}else{
		$dados = buscarDadosMantida($mnmid);
	}
}
$dadosMant = buscarDadosMantenedora($mntid)
?>

<div>

	<?php if ($_REQUEST['tipo'] == '2'){?>
		<div>Nova Mantida</div>
	<?}else{?>
		<div>Mantida Indicada</div>
	<?}?>
    <div>Inscrição Finalizada com Sucesso!! </div>
	<br>
    <div>Mantida: <?php echo $dados['mtddsc']; ?></div>
    <?php if($dados['mantida_muncod']){?>
    <div>Município: <?php echo $db->pegaUm("select mundescricao || ' / ' || estuf as mun from territorios.municipio where muncod = '{$dados['mantida_muncod']}'"); ?> </div>
    <?}?>
    <div> Nº da Inscrição: <?php echo $dados['mnmid']; ?>/<?php echo date("Y", strtotime($dados['mtddtinclusao'])); ?>    </div>
    <div  style="margin-bottom: 20px;">Data de inclusão: <?php echo date("d/m/Y", strtotime($dados['mtddtinclusao'])); ?>  </div>
    <div> Mantenedora: <?php echo $dadosMant['mntrazaosocial'] ?>  </div>
    <div style="margin-bottom: 20px;"> Código:<?php echo $dadosMant['mntid'] ?></div>


    <input type="button" id="imprimir" value="Imprimir" onclick="(this).style.display = 'none';javascript:window.print();">

</div>
