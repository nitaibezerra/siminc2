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
$mtdid = $_SESSION['maismedicomec']['mtdid'];
$mntid = $_SESSION['maismedicomec']['mntid'];


if ($mtdid != '')
    $dados = buscarDadosMantidaCandidata($mtdid);
$dadosMant = buscarDadosMantenedora($mntid)
?>

<div>


    <div>Inscrição Finalizada com Sucesso!! </div>

    <div>Mantida: <?php echo $dados['mtddsc']; ?></div>
    <div> Nº da Inscrição: <?php echo $dados['mtdid']; ?>/<?php echo date("Y", strtotime($dados['mtddtinclusao'])); ?>    </div>
    <div  style="margin-bottom: 20px;">Data de inclusão: <?php echo date("d/m/Y", strtotime($dados['mtddtinclusao'])); ?>  </div>
    <div> Mantenedora: <?php echo $dadosMant['mntrazaosocial'] ?>  </div>
    <div style="margin-bottom: 20px;"> Código:<?php echo $dadosMant['mntid'] ?></div>


    <input type="button" id="imprimir" value="Imprimir" onclick="(this).style.display = 'none';javascript:window.print();">

</div>
