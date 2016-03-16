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

    $sql = "SELECT intid , intdscrazaosocial FROM pdu.instituicao WHERE intid = {$intid}";
    $instituicao = $db->pegaLinha($sql);
    $_SESSION['instituicao'] = $instituicao;

} else if(!$_SESSION['instituicao']['intid']) {
    echo 'Falta selecionar a instituicao';
    exit;
}
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
        <ul class="nav nav-tabs" id="tabsInstituicao">
            <li class="active"><a id="tabInstituicao" href="#instituicao" data-toggle="tab">Instituição</a></li>
            <li><a id="tabDirigente" href="#dirigente" data-toggle="tab">Dirigentes</a></li>
            <li><a id="tabCampus" href="#campus" data-toggle="tab">Campus</a></li>
            <li><a id="tabCampus" href="#indicadoreseducacionais" data-toggle="tab">Indicadores acadêmicos</a></li>
            <li><a id="tabCampus" href="#diagnostico" data-toggle="tab">Diagnóstico</a></li>
        </ul>
    </div>
    <!-- Tab panes -->
    <div class="tab-content">
        <div class="tab-pane active" id="instituicao">...</div>
        <div class="tab-pane active" id="dirigente">...</div>
        <div class="tab-pane" id="campus">...</div>
        <div class="tab-pane" id="indicadoreseducacionais">...</div>
        <div class="tab-pane" id="diagnostico">...</div>
    </div>
</div>
<?php $tab = 'tabInstituicao' ?>
<script lang="javascript">
    $(document).ready(function() {
        getContentTab($('#<?php echo $tab ?>'));
    });

    $('#tabsInstituicao a').click(function(e) {
        getContentTab($(this));
    });

    /**
     *
     */
    function getContentTab(element)
    {
//        $('#kanban').empty();
//        $('#sprint').empty();
//        $('#postit').empty();
//        $('#team').empty();
        $('#instituicao').empty();
        $('#dirigente').empty();
        $('#campus').empty();
        $('#indicadoreseducacionais').empty();
        $('#diagnostico').empty();

        var href = $(element).attr('href');
        var controller = href.replace('#', '');

        var data = {controller: controller, action: 'index'};

        $.post(window.location.href, data, function(html) {
            $(href).html(html);
//            $(e).tab('show');
        });
//        e.preventDefault();
        $(element).tab('show');
    }
</script>

