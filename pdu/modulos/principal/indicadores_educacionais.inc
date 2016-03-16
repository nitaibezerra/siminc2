<?
/*
  Sistema Simec
  Setor responsvel: MEC
  Desenvolvedor: Equipe Consultores Simec
  Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
  Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
  Mdulo:inicio.inc
  Finalidade: permitir abrir a página inicial
   */

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";

$intid = $_POST['id'];
if($_POST['id']){
    $intid = $_POST['id'];

    $sql = "SELECT * FROM pdu.instituicao WHERE intid = {$intid}";
    $instituicao = $db->pegaLinha($sql);
    $_SESSION['instituicao'] = $instituicao;

} else if(!$_SESSION['instituicao']['intid']) {
    echo 'Falta selecionar a instituicao';
    exit;
}

global $clsOracle;
//$clsGrafico = new Grafico(NULL, false);


?>

<div class="row">
    <div class="col-lg-12">
        <div class="page-header">
            <h2 id="forms">
<!--                Instituição-->
<!--                <small>-->
                    <?php echo $_SESSION['instituicao']['intdscrazaosocial'] ?>
<!--                </small>-->
            </h2>
        </div>
    </div>
</div>
<div class="row">
    <div class="col-md-12" style="padding-bottom: 20px;">
        <ul class="nav nav-tabs" id="tabsIndicadoresEducacionais">
            <li class="active"><a id="tabCensoEducacional2012" href="#censoEducacional2012" data-toggle="tab">Censo Educacional 2012</a></li>
            <li><a id="tabCensoEducacional2013" href="#censoEducacional2013" data-toggle="tab">Censo Educacional 2013</a></li>
            <li><a id="tabIde" href="#ide" data-toggle="tab">Indicadores Demográfico e Educacionais - IDE</a></li>
            <li><a id="tabIgc" href="#igc" data-toggle="tab">Indice Geral dos Cursos - IGC</a></li>
        </ul>
    </div>
    <!-- Tab panes -->
    <div class="tab-content">
        <div class="tab-pane active" id="censoEducacional2012">...</div>
        <div class="tab-pane active" id="censoEducacional2013">...</div>
        <div class="tab-pane" id="ide">...</div>
        <div class="tab-pane" id="igc">...</div>
    </div>
</div>
<?php $tab = 'tabCensoEducacional2012' ?>
<script lang="javascript">
    $(document).ready(function() {
        getContentTab($('#<?php echo $tab ?>'));
    });

    $('#tabsIndicadoresEducacionais a').click(function(e) {
        getContentTab($(this));
    });

    /**
     *
     */
    function getContentTab(element)
    {
        $('#censoEducacional2012').empty();
        $('#censoEducacional2013').empty();
        $('#ide').empty();
        $('#igc').empty();

        var href = $(element).attr('href');
        var action = href.replace('#', '');

        var data = {controller: 'Indicadoreseducacionais', action: action};

        $.post(window.location.href, data, function(html) {
            $(href).html(html);
//            $(e).tab('show');
        });
//        e.preventDefault();
        $(element).tab('show');
    }
</script>

