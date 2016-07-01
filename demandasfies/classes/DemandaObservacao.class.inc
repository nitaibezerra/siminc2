<?php

class DemandaObservacao extends Modelo
{

    const K_TIPO_GERAL                      = 'GE';
    const K_TIPO_SINTESE_AUTOR              = 'SA';
    const K_TIPO_DETERMINACAO_JUDICIAL      = 'DJ';
    const K_TIPO_QUESTIONAMENTOS_DIRIMIDOS  = 'QD';
    const K_TIPO_QUESTIONAMENTOS_AREAS      = 'AR';
    const K_TIPO_ENTREGAS                   = 'ET';

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandasfies.demandaobservacao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("dmoid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'dmoid' => null,
        'dmdid' => null,
        'dmotexto' => null,
        'usucpfinclusao' => null,
        'dmodtinclusao' => null,
        'usucpfalteracao' => null,
        'dmodtalteracao' => null,
        'dmostatus' => null,
        'usucpfinativacao' => null,
        'dmodtinativacao' => null,
        'dmotipo' => null,
        'dmeid' => null,
        'esdid' => null,
    );


    public function formularioObservacoes($complementoDisabled = '')
    {

        $dmdid = $this->dmdid ? $this->dmdid : 0;
        $sql = "select dmoid, substr(dmotexto, 0, 100) as dmotexto, usunome, u.usucpf, usucpfinclusao
                from demandasfies.demandaobservacao dob
                    INNER join seguranca.usuario u on dob.usucpfinclusao = u.usucpf
                where dmdid = {$dmdid}
                and dmostatus = 'A'
                and dmotipo = 'GE'
                order by dmoid desc";
        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : array();
        ?>
        <div class="well">
            <?php if (!$complementoDisabled){ ?>
            <form id="form-observacao" method="post" class="form-horizontal">
                <input name="dmoid" type="hidden" value="<?php echo $this->dmoid; ?>">
                <input name="dmdid" type="hidden" value="<?php echo $this->dmdid; ?>">
                <input name="dmotipo" type="hidden" value="GE">
                <input name="action" type="hidden" value="salvar_observacao">
                <?php } ?>
                <fieldset>
                    <legend>Observações <span style="color: red">(<?php echo count($dados); ?>)</span></legend>
                    <div class="form-group">
                        <label for="dmdtitulo" class="col-lg-2 col-md-2 control-label">Observação:</label>

                        <div class="col-lg-10 col-md-10 ">
                            <textarea class="form-control" rows="3" name="dmotexto"
                                      id="dmotexto"><?php echo $this->dmotexto; ?></textarea>
                        </div>
                    </div>
                </fieldset>
                <div>
                    <button title="Salvar" id="btn-salvar-observacao" class="btn btn-success" type="button"><span
                            class="glyphicon glyphicon-thumbs-up"></span> Salvar
                    </button>
                </div>
                <?php if (!$complementoDisabled){ ?>
            </form>
        <?php } ?>

            <div style="margin-top: 20px; background: #ffffff !important;">
                <?php
                $listagem = new Simec_Listagem();
                $listagem->setCabecalho(array('Observação', 'Responsável'));
                $listagem->addCallbackDeCampo(array('dmotexto', 'usunome'), 'alinhaParaEsquerda');

                $listagem->addAcao('view', 'detalharObservacao');
                $listagem->addAcao('edit', 'editarObservacao');
                $listagem->addAcao('delete', 'inativarObservacao');

                $listagem->setAcaoComoCondicional('edit',array(array('campo' => 'usucpfinclusao', 'operacao' => 'igual', 'valor' => $_SESSION['usucpf'])));
                $listagem->setAcaoComoCondicional('delete',array(array('campo' => 'usucpfinclusao', 'operacao' => 'igual', 'valor' => $_SESSION['usucpf'])));
                $listagem->esconderColunas (array('usucpfinclusao'));

                $listagem->setQuery($sql);
                $listagem->render();
                ?>
            </div>

        </div>

        <script type="text/javascript">
            $(function () {
                $('#btn-salvar-observacao').click(function () {
                    if (!$('#dmotexto').val()) {
                        alert('Favor preencher o campo de texto.');
                        return false
                    }

                    options = {
                        success: function () {
                            jQuery("#div_listagem_observacao").load('/demandasfies/demandasfies.php?modulo=principal/demandasformulario&acao=A&action=form_observacao&dmdid=' + $('#dmdid').val());
                        }
                    }

                    jQuery("#form-observacao").ajaxForm(options).submit();
                });
            });

            function editarObservacao(dmoid) {
                window.location = 'demandasfies.php?modulo=principal/demandasformulario&acao=A&dmoid=' + dmoid + '&dmdid=' + '<?php echo $this->dmdid; ?>#div_listagem_observacao';
            }

            function inativarObservacao(dmoid) {
                if (confirm('Deseja realmente excluir o registro?')) {
                    window.location = 'demandasfies.php?modulo=principal/demandasformulario&acao=A&action=inativar_observacao&dmoid=' + dmoid + '&dmdid=' + '<?php echo $this->dmdid; ?>';
                }
            }
        </script>
    <?php
    }

    public function formularioSinteses($tipo = 'GE', $titulo = 'Observações')
    {

        $dmdid = $this->dmdid ? $this->dmdid : 0;
        $sql = "select dmoid, substr(dmotexto, 0, 100) as dmotexto, to_char(dob.dmodtinclusao, 'DD/MM/YYYY HH24:mi:ss'), usunome, esddsc, usucpfinclusao
                from demandasfies.demandaobservacao dob
                    INNER join seguranca.usuario u on dob.usucpfinclusao = u.usucpf
                    LEFT  join workflow.estadodocumento ed on ed.esdid = dob.esdid
                where dmdid = {$dmdid}
                and dmostatus = 'A'
                and dmotipo = '{$tipo}'
                order by dmoid desc";

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : array();
        ?>
        <div class="well">

            <form id="form-observacao" method="post" class="form-horizontal">
                <input name="dmoid" type="hidden" value="<?php echo $this->dmoid; ?>">
                <input name="dmdid" type="hidden" value="<?php echo $this->dmdid; ?>">
                <input name="dmotipo" type="hidden" value="<?php echo $tipo; ?>">
                <input name="action" type="hidden" value="salvar_observacao">
                <fieldset>
                    <legend><?php echo $titulo; ?> <span style="color: red">(<?php echo count($dados); ?>)</span></legend>
                    <div class="form-group">

                        <div class="col-lg-12 col-md-12 ">
                            <textarea class="form-control" rows="5" name="dmotexto" id="dmotexto"><?php echo $this->dmotexto; ?></textarea>
                        </div>
                    </div>
                </fieldset>
                <div>
                    <button title="Salvar" id="btn-salvar-observacao" class="btn btn-success" type="submit"><span
                            class="glyphicon glyphicon-thumbs-up"></span> Salvar
                    </button>
                </div>
            </form>

            <div style="margin-top: 20px; background: #ffffff !important;">
                <?php
                $listagem = new Simec_Listagem();

                if(self::K_TIPO_QUESTIONAMENTOS_AREAS == $tipo){
                    $listagem->setCabecalho(array('Texto', 'Dt. Inclusão', 'Responsável', 'Área'));
                } else {
                    $listagem->setCabecalho(array('Texto', 'Dt. Inclusão', 'Responsável'));
                    $listagem->esconderColunas(array('esddsc'));
                }

                $listagem->addCallbackDeCampo(array('dmotexto', 'usunome'), 'alinhaParaEsquerda');
                $listagem->addAcao('view', 'detalharObservacao');

                $listagem->addAcao('edit', 'editarObservacao');
                $listagem->addAcao('delete', 'inativarObservacao');

                $listagem->setAcaoComoCondicional('edit',array(array('campo' => 'usucpfinclusao', 'operacao' => 'igual', 'valor' => $_SESSION['usucpf'])));
                $listagem->setAcaoComoCondicional('delete',array(array('campo' => 'usucpfinclusao', 'operacao' => 'igual', 'valor' => $_SESSION['usucpf'])));
                $listagem->esconderColunas (array('usucpfinclusao'));

                $listagem->setQuery($sql);
                $listagem->render();
                ?>
            </div>

        </div>

        <script type="text/javascript">
            $(function () {
//                $('#btn-salvar-observacao').click(function () {
//                    if (!$('#dmotexto').val()) {
//                        alert('Favor preencher o campo de texto.');
//                        return false
//                    }
//                });
            });

            function editarObservacao(dmoid) {
                window.location = 'demandasfies.php?modulo=principal/demandasformulario&acao=A&dmdid=<?php echo $this->dmdid; ?>&aba=sinteses&dmoid=' + dmoid;
            }

            function inativarObservacao(dmoid) {
                if (confirm('Deseja realmente excluir o registro?')) {
                    window.location = 'demandasfies.php?modulo=principal/demandasformulario&acao=A&dmdid=<?php echo $this->dmdid; ?>&aba=sinteses&dmoid=' + dmoid +'&action=inativar_observacao';
                }
            }
        </script>
    <?php
    }

    public function ultimaSintese($tipo = 'GE', $titulo = 'Observações')
    {
        $dmdid = $this->dmdid ? $this->dmdid : 0;
        $sql = "select dmotexto, usunome, to_char(dmo.dmodtinclusao, 'DD/MM/YYYY HH:24:mi:ss') as data, esddsc
                from demandasfies.demandaobservacao dmo
                    INNER join seguranca.usuario u on dmo.usucpfinclusao = u.usucpf
                    LEFT  join workflow.estadodocumento ed on ed.esdid = dmo.esdid
                where dmostatus = 'A'
                and dmdid = $dmdid
                and dmotipo = '{$tipo}'
                order by dmo.dmodtinclusao desc
                limit 1";

        $dados = $this->pegaLinha($sql);
        $dados = $dados ? $dados : array();
        ?>
        <p style="color: red;"><?php echo $titulo; ?><?php echo $dados['esddsc'] ? " <strong>({$dados['esddsc']})</strong>" : '' ?></p>
        <p>Por: <strong><?php echo $dados['usunome']; ?></strong></p>
        <p>Em: <strong><?php echo $dados['data']; ?></strong></p>

        <p style="margin-top: 10px; margin-bottom: 50px;">
            <?php echo nl2br($dados['dmotexto']); ?>
        </p>

    <?php
    }

    public function formularioEntregas()
    {
        $tipo = self::K_TIPO_ENTREGAS;

        $dmdid = $this->dmdid ? $this->dmdid : 0;
        $dmeid = $this->dmeid ? $this->dmeid : 0;
        $sql = "select dmoid, substr(dmotexto, 0, 100) as dmotexto, to_char(dob.dmodtinclusao, 'DD/MM/YYYY HH24:mi:ss'), usunome, usucpfinclusao
                from demandasfies.demandaobservacao dob
                    INNER join seguranca.usuario u on dob.usucpfinclusao = u.usucpf
                where dmdid = {$dmdid}
                and dmostatus = 'A'
                and dmotipo = '{$tipo}'
                and dmeid = {$dmeid}
                order by dmoid desc";

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : array();
        ?>
        <div class="well">

            <form id="form-observacao" method="post" class="form-horizontal">
                <input name="dmoid" type="hidden" value="<?php echo $this->dmoid; ?>">
                <input name="dmdid" type="hidden" value="<?php echo $this->dmdid; ?>">
                <input name="dmeid" type="hidden" value="<?php echo $this->dmeid; ?>">
                <input name="dmotipo" type="hidden" value="<?php echo $tipo; ?>">
                <input name="action" type="hidden" value="salvar_observacao">
                <fieldset>
                    <legend>Textos <span style="color: red">(<?php echo count($dados); ?>)</span></legend>
                    <div class="form-group">

                        <div class="col-lg-12 col-md-12 ">
                            <textarea class="form-control" rows="5" name="dmotexto" id="dmotexto"><?php echo $this->dmotexto; ?></textarea>
                        </div>
                    </div>
                </fieldset>
                <div>
                    <button title="Salvar" id="btn-salvar-observacao" class="btn btn-success" type="submit"><span
                            class="glyphicon glyphicon-thumbs-up"></span> Salvar
                    </button>
                </div>
            </form>

            <div style="margin-top: 20px; background: #ffffff !important;">
                <?php
                $listagem = new Simec_Listagem();
                $listagem->setCabecalho(array('Texto', 'Dt. Inclusão', 'Responsável'));
                $listagem->addCallbackDeCampo(array('dmotexto', 'usunome'), 'alinhaParaEsquerda');
                $listagem->addAcao('view', 'detalharObservacao');
                $listagem->addAcao('edit', 'editarObservacao');
                $listagem->addAcao('delete', 'inativarObservacao');
				$listagem->setAcaoComoCondicional('delete',array(array('campo' => 'usucpfinclusao', 'operacao' => 'igual', 'valor' => $_SESSION['usucpf'])));
				$listagem->esconderColunas (array('usucpfinclusao'));
                $listagem->setQuery($sql);
                $listagem->render();
                ?>
            </div>

        </div>

        <script type="text/javascript">
            $(function () {
//                $('#btn-salvar-observacao').click(function () {
//                    if (!$('#dmotexto').val()) {
//                        alert('Favor preencher o campo de texto.');
//                        return false
//                    }
//                });
            });

            function editarObservacao(dmoid) {
                window.location = 'demandasfies.php?modulo=principal/demandasformulario&acao=A&dmdid=<?php echo $this->dmdid; ?>&aba=entregas&dmoid=' + dmoid + '&dmeid=<?php echo $this->dmeid; ?>';
            }

            function inativarObservacao(dmoid) {
                if (confirm('Deseja realmente excluir o registro?')) {
                    window.location = 'demandasfies.php?modulo=principal/demandasformulario&acao=A&dmdid=<?php echo $this->dmdid; ?>&aba=entregas&dmoid=' + dmoid + '&dmeid=<?php echo $this->dmeid; ?>&action=inativar_observacao';
                }
            }
        </script>
    <?php
    }
}