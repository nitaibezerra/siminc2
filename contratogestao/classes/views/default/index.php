<div class="modal fade" id="dialogo_cadastrar_contrato" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
    <div class="modal-dialog-large">
        <div class="modal-content" id="form_contrato"></div>
    </div>
</div>

<div class="modal fade" id="dialogo_cadastrar_contrato_item" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content" id="form_contrato_item"></div>
    </div>
</div>

<div class="modal fade" id="dialogo_fator_avaliado" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
    <div class="modal-dialog-large">
        <div class="modal-content" id="form_fator_avaliado"></div>
    </div>
</div>

<div class="row">
    <div class="col-lg-12" id="div_msg_ordenacao" style="display:none">
        <div class="alert alert-dismissable alert-success">
            <strong>Sucesso! </strong><span id="msg_retorno"></span>
        </div>
    </div>
</div>

<ul id="tab_gestao_contrato" class="nav nav-tabs" role="tablist">
    <li class="active"><a href="#conteudo_principal">Principal</a></li>
    <li class="disabled"><a href="#fator_avaliado_conteudo">Fator Avaliado</a></li> 
</ul>

<br>
<div class="tab-content" id="myTabContent">
    <div id="conteudo_principal" class="tab-pane fade active in">

        <div class="row">
            <div class="col-lg-12">
                <?php if ($this->perfilUsuario->possuiAcesso()): ?>
                    <button type="button" class="btn btn-primary btn-xs adicionarContrato"><span class="glyphicon glyphicon-plus"></span> novo contrato</button> |
                <?php endif; ?>

<!--                <button type="button" class="btn btn-link btn-xs" style="display:none"><span class="glyphicon glyphicon-download-alt"></span> excel</button>-->

<!--                <button type="button" class="btn btn-link btn-xs" id="exibir_todos">-->
<!--                    <span class="glyphicon glyphicon-resize-full"></span> expandir-->
<!--                </button>-->
<!--                |-->
<!--                <button type="button" class="btn btn-link btn-xs" id="esconder_todos">-->
<!--                    <span class="glyphicon glyphicon-resize-small"></span> esconder-->
<!--                </button>-->
            </div>
        </div>

        <hr>

        <div id="container-listar">
            <?php include_once 'listar.php'; ?>
        </div>
    </div>
    <div id="fator_avaliado_conteudo" class="tab-pane"></div>
</div>
