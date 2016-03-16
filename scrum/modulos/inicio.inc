<?
//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";

$controller = new Controller_Postit();
if ($controller->isPerfil(PERFIL_GESTORPROGRAMA)) {
    $tab = 'tabSprint';
} else {
    $tab = 'tabKanban';
}
// perfil consulta 1154
//ver($_SESSION, d);

if($controller->isPerfil(PERFIL_CONSULTA)){
    $consulta = true;
} else {
    $consulta = false;
}

?>
<link rel="stylesheet" type="text/css" href="../scrum/css/scrum.css"/>

<?php // montaComboPrograma($_POST['prgid']);   ?>

<br>
<div class="row">
    <div class="col-md-12">
<!--        <div class="form-group">
                <legend>Projetos Ágeis (SCRUM)</legend>
            </div>-->
        <ul class="nav nav-tabs" id='tabsScrum'>
            <li class="active"><a id="tabKanban" href="#kanban">Início</a></li>
            <li ><a id="tabResumoHora" href="#resumohora">Resumo Horas</a></li>
            <li><a id="tabSprint" href="#sprint">Ciclos</a></li>
            <?php if(!$consulta): ?>
            <li ><a id="tabPostit" href="#postit">Entregáveis</a></li>
            <li ><a id="tabStory" href="#story">Estórias</a></li>
            <li ><a id="tabProject" href="#subprogram">Subprojeto</a></li>
            <li ><a id="tabProgram" href="#program">Projetos</a></li>
            <?php endif ?>
        </ul>
        <div class="tab-content">
            <div class="tab-pane fade in active" id="kanban"></div>
            <div class="tab-pane fade in active" id="resumohora"></div>
            <div class="tab-pane fade" id="sprint"></div>
            <?php if(!$consulta): ?>
            <div class="tab-pane fade" id="postit">...</div>
            <div class="tab-pane fade" id="story">...</div>
            <div class="tab-pane fade" id="subprogram">...</div>
            <div class="tab-pane fade" id="program">...</div>

            <?php endif ?>
        </div>
    </div>
</div>
<script lang="javascript">
    $(document).ready(function() {
        getContentTab($('#<?php echo $tab ?>'));
    });

    $('#tabsScrum a').click(function(e) {
        getContentTab($(this));
    });

    /**
     *
     */
    function getContentTab(element)
    {
        $('#kanban').empty();
        $('#resumohora').empty();
        $('#sprint').empty();
        $('#postit').empty();
        $('#team').empty();
        $('#story').empty();
        $('#subprogram').empty();
        $('#program').empty();

        var href = $(element).attr('href');
        var controller = href.replace('#', '');
        var data = {controller: controller, action: 'default'};

        $.post(window.location.href, data, function(html) {
            $(href).html(html);
//            $(e).tab('show');
        });
//        e.preventDefault();
        $(element).tab('show');
    }
</script>