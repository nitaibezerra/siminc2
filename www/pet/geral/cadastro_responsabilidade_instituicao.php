<?php

include "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

$_SESSION['sisdiretorio'] = 'pet';
include_once APPRAIZ . 'includes/library/simec/Crud/Listing.php';
include_once APPRAIZ . 'includes/library/simec/Autoload.php';

$usuarioResponsabilidade = new Model_Usuarioresponsabilidade();

$iesid = $_POST["iesid"];
$usucpf = $_GET['usucpf'];
$pflcod = $_GET['pflcod'];

if (!empty($iesid)) {
    $usuarioResponsabilidade->atribuirResponsabilidade($usucpf, $pflcod, $iesid);
}
?>

<?php require_once "../_header.php"; ?>

<form name="formassocia" method="post">

    <div style="overflow:auto; width:496px; height:350px; border:2px solid #ececec; background-color: #ffffff;">
        <div class="panel panel-default">
            <div class="panel-heading">Lista de Instituições de Ensino Superior</div>
            <div class="panel-body">
                <?php $usuarioResponsabilidade->listaDados(); ?>
            </div>
        </div>
    </div>

    <hr>

    <div class="form-group">
        <div class="col-lg-12 text-center">
            <button type="submit" class="btn btn-primary"><span class="glyphicon glyphicon-pushpin" aria-hidden="true"></span> Vincular Responsabilidade</button>
        </div>
    </div>

</form>
<script>
    function abreconteudo(objeto){
        if (document.getElementById('img' + objeto).name == '+'){
            document.getElementById('img' + objeto).name = '-';
            document.getElementById('img' + objeto).src = document.getElementById('img' + objeto).src.replace('mais.gif', 'menos.gif');
            document.getElementById(objeto).style.visibility = "visible";
            document.getElementById(objeto).style.display = "";
        }else{
            document.getElementById('img' + objeto).name = '+';
            document.getElementById('img' + objeto).src = document.getElementById('img' + objeto).src.replace('menos.gif', 'mais.gif');
            document.getElementById(objeto).style.visibility = "hidden";
            document.getElementById(objeto).style.display = "none";
        }
    }
</script>