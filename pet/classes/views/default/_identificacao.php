<div class="panel with-nav-tabs panel-primary">
    <div class="panel-heading">
        <ul class="nav nav-tabs" id="tab_grupo">
            <li class="active">
                <a data-toggle="tab" href="#tab1InformacaoBasica"><span class="glyphicon glyphicon-certificate" aria-hidden="true"></span>
                    Informações Básicas</a></li>
            <li>
                <a data-toggle="tab" href="#tab2Descricao"><span class="glyphicon glyphicon-align-justify" aria-hidden="true"></span>
                    Descrições</a></li>
            <li>
                <a data-toggle="tab" href="#tab3Colegiado"><span class="glyphicon glyphicon-book" aria-hidden="true"></span>
                    Colegiado(s)</a></li>
        </ul>
    </div>

    <div class="panel-body">
        <div class="tab-content">
            <div id="tab1InformacaoBasica" class="tab-pane fade active in">
                <?php require_once('_informacao_basica.php'); ?>
            </div>

            <div id="tab2Descricao" class="tab-pane fade">
                <?php require_once('_descricoes.php'); ?>
            </div>

            <div id="tab3Colegiado" class="tab-pane fade">
                <?php require_once('_colegiado.php'); ?>
            </div>
        </div>
    </div>
</div>