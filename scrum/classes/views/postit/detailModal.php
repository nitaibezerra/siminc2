<div class="modal-dialog-large">
<div class="modal-content">
<div class="modal-header">
    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
    <h4 class="modal-title">Detalhe</h4>
</div>
<div class="modal-body">

<div class="row">
    <div class="col-lg-6">
        <!--        <legend><strong>Demanda--><?php //if($this->entityDemanda['dmdid']['value']) echo ' - ' . $this->entityDemanda['dmdid']['value']; ?><!--</strong></legend>-->

        <h1>
            <!--            --><?php //ver($this->entityDemanda, d); ?>
            <!--        <small>Demanda--><?php //if($this->entityDemanda['dmdid']['value']) echo ' - ' . $this->entityDemanda['dmdid']['value']; ?><!--</small>-->
        </h1>
        <!---->
        <!--        <h3>--><?php //echo $this->entity['entdsc']['value'] ?><!--</h3>-->
        <!--        <div class="bs-callout bs-callout-info">-->
        <!--            <h4>Alternate elements</h4>-->
        <!--            <p>Feel free to use <code>&lt;b&gt;</code> and <code>&lt;i&gt;</code> in HTML5. <code>&lt;b&gt;</code> is meant to highlight words or phrases without conveying additional importance while <code>&lt;i&gt;</code> is mostly for voice, technical terms, etc.</p>-->
        <!--        </div>-->
        <!--        <dl>-->
        <!--            <dt>Descrição</dt>-->
        <!--            <dd>--><?php //echo $this->entity['entdsc']['value'] ?><!--</dd>-->
        <!--        </dl>-->
        <h3><?php echo $this->entity['entdsc']['value'] ?></h3>
    </div>
    <div class="col-lg-6">
        <div class="well">
            <form id="form-entregavel" name="form-entregavel" method="post" class="form-horizontal">
                <!--<input type="hidden" name="action" value="save"/>-->
                <input type="hidden" name="entid" value="<?php echo $this->entity['entid']['value'] ?>"/>
                <input type="hidden" name="qtdMaxHoraSprint" value="<?php echo $this->qtdMaxHoraSprint ?>"/>
                <fieldset>
                    <div class="form-group text-center">
                        <legend>Entregável</legend>
                    </div>
                    <div class="form-group">
                        <label for="inputEmail" class="col-lg-4 control-label" for="Programa">Responsável</label>
                        <div class="col-lg-8">
                            <select name="usucpfresp" id="usucpfresp" class="chosen-select form-control" data-placeholder="Selecione" disabled="disabled">
                                <option value=""></option>
                                <?php foreach ($this->equipe as $responsavel): ?>
                                    <option <?php if ($this->entity['usucpfresp']['value'] == $responsavel['usucpf']) echo 'selected="selected"' ?> value="<?php echo $responsavel['usucpf'] ?>"><?php echo $responsavel['usunome'] ?></option>
                                <?php endforeach ?>
                            </select>
                            <!--<input name="usucpfresp" id="usucpfresp" type="text" min="1" max="100" class="form-control" value="<?php echo $this->entity['usucpfresp']['value'] ?>">-->
                            <?php // echo campo_texto('enthrsexec', 'N' , 'S', 'Descrição' , 25, '30', '###' , '' , '', '', '', 'id="enthrsexec" required="required" class="form-control" style="width=100%;"')  ?>
                        </div>
                        <!--<p class="help-block">Selecione um programa.</p>-->
                    </div>
                    <div class="form-group">
                        <label for="inputEmail" class="col-lg-4 control-label" for="Programa">Solicitante</label>
                        <div class="col-lg-8">
                            <input name="usucpfsol" id="usucpfsol" type="text" min="1" max="100" disabled="disabled"  class="form-control" value="<?php echo $this->solicitante['usunome'] ?>">
                            <?php // echo campo_texto('enthrsexec', 'N' , 'S', 'Descrição' , 25, '30', '###' , '' , '', '', '', 'id="enthrsexec" required="required" class="form-control" style="width=100%;"')  ?>
                        </div>
                        <!--<p class="help-block">Selecione um programa.</p>-->
                    </div>
                    <div class="form-group has-warning">
                        <label for="inputEmail" class="col-lg-4 control-label" for="Programa">Horas de execução</label>
                        <div class="col-lg-8">
                            <input disabled="disabled" name="enthrsexec" id="enthrsexec" type="number" min="1" max="100" required="required" placeholder="000" class="form-control" value="<?php echo $this->entity['enthrsexec']['value'] ?>">
                            <?php // echo campo_texto('enthrsexec', 'N' , 'S', 'Descrição' , 25, '30', '###' , '' , '', '', '', 'id="enthrsexec" required="required" class="form-control" style="width=100%;"')  ?>
<!--                            <p class="help-block">Quantidade de horas para este entregável.</p>-->
                        </div>
                    </div>
                </fieldset>
            </form>
        </div>
<!--    </div>-->
<!--    <div class="col-lg-6">-->
        <div class="well">
            <form id="form-demanda" name="form-demanda" method="post" class="form-horizontal">
                <fieldset>
                    <div class="form-group text-center">
                        <legend>Demanda<?php if($this->entityDemanda['dmdid']['value']) echo ' - ' . $this->entityDemanda['dmdid']['value']; ?></legend>
                    </div>
                    <div class="form-group">
                        <label for="inputEmail" class="col-lg-4 control-label" for="Programa">Prioridade da demanda</label>
                        <div class="col-lg-8">
                            <select name="priid" id="priid" class="form-control" disabled="disabled">
                                <option value=""> Selecione </option>
                                <?php foreach ($this->prioridades as $prioridade): ?>
                                    <option <?php if ($this->entityDemanda['priid']['value'] == $prioridade['priid']) echo 'selected="selected"' ?> value="<?php echo $prioridade['priid'] ?>"><?php echo $prioridade['pridsc'] ?></option>
                                <?php endforeach ?>
                            </select>
                            <!--<input type="text" class="form-control" id="demandante" placeholder="Demandante" value="<?php echo $this->solicitante['usunome'] ?>" disabled="disabled">-->
                        </div>
                        <!--<p class="help-block">Selecione um programa.</p>-->
                    </div>
                    <div class="form-group">
                        <label for="inputEmail" class="col-lg-4 control-label" for="dmddatainiprevatendimento">Previsão de início do atendimento</label>
                        <div class="col-lg-4">
                            <input  disabled="disabled" name="dmddatainiprevatendimento" type="text" class="form-control" id="dmddatainiprevatendimento" placeholder="00/00/0000" maxlength="10" value="<?php echo $this->entityDemanda['dmddatainiprevatendimento']['value'] ?>" data-format="dd/MM/yyyy hh:mm:ss" >
                        </div>
                        <div class="col-lg-4">
                            <input  disabled="disabled" name="hiniatendimento" type="time" class="form-control" id="hiniatendimento" placeholder="00:00" value="<?php echo $this->entityDemanda['hiniatendimento']['value'] ?>" >
                        </div>
                        <!--<p class="help-block">Selecione um programa.</p>-->
                    </div>
                    <div class="form-group">
                        <label for="inputEmail" class="col-lg-4 control-label" for="Programa">Previsão de término do atendimento</label>
                        <div class="col-lg-4">
                            <input disabled="disabled" name="dmddatafimprevatendimento" type="text" class="form-control" id="dmddatafimprevatendimento" placeholder="00/00/0000" maxlength="10" value="<?php echo $this->entityDemanda['dmddatafimprevatendimento']['value'] ?>" data-format="dd/MM/yyyy hh:mm:ss">
                        </div>
                        <div class="col-lg-4">
                            <input disabled="disabled" name="hfimatendimento" type="time" class="form-control" id="hfimatendimento" placeholder="00:00" value="<?php echo $this->entityDemanda['hfimatendimento']['value'] ?>" >
                        </div>
                        <!--<p class="help-block">Selecione um programa.</p>-->
                    </div>
                </fieldset>
            </form>
        </div>
    </div>
</div>

<div class="modal-footer">
    <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
</div>
</div><!-- /.modal-content -->
</div><!-- /.modal-dialog -->
<script lang="javascript">
</script>