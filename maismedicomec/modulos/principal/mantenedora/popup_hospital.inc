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
$hspid = $_SESSION['maismedicomec']['hspid'];
$mntid = $_SESSION['maismedicomec']['mntid'];


if ($hspid != '')
    $dados = buscarDadosHospital($hspid);
$dadosMant = buscarDadosMantenedora($mntid);

// ver($dados,d);

$sql = " SELECT * FROM maismedicomec.mantida WHERE mtdid = {$dados['mtdid']} ";
$dadosMantida = $db->carregar( $sql );
// ver($dadosMantida,d);
?>

<div>


    <div>Inscrição Finalizada com Sucesso!! </div><br/>

    <div>Hospital: <?php echo $dados['hsprazaosocial']; ?></div>
    <div> Nº da Inscrição: <?php echo $dados['hspid']; ?>/<?php echo date("Y", strtotime($dados['hspdtinclusao'])); ?>    </div>
    <div  style="margin-bottom: 20px;">Data de inclusão: <?php echo date("d/m/Y", strtotime($dados['hspdtinclusao'])); ?> </div>
    <!-- <div> Mantenedora: <?php echo $dadosMant['mntrazaosocial'] ?>  </div>
    <div style="margin-bottom: 20px;"> Código:<?php echo $dadosMant['mntid'] ?></div> -->

    <div> Mantida: <?php echo $dadosMantida[0]['mtddsc'] ?>  </div>
    <div style="margin-bottom: 20px;"> Código:<?php echo $dadosMantida[0]['mtdid'] ?></div>

    <input type="button" id="imprimir" value="Imprimir" onclick="(this).style.display = 'none';javascript:window.print();">

</div>
