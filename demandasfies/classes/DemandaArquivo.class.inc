<?php
	
class DemandaArquivo extends Modelo{

    const K_TIPO_GERAL    = 'GE';
    const K_TIPO_ENTREGAS = 'ET';

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandasfies.demandaarquivo";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("dmaid");

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'dmaid' => null, 
									  	'dmdid' => null, 
									  	'arqid' => null, 
									  	'dmadsc' => null, 
									  	'usucpfinclusao' => null, 
									  	'dmadtinclusao' => null, 
									  	'usucpfalteracao' => null, 
									  	'dmadtalteracao' => null, 
									  	'dmastatus' => null, 
									  	'usucpfinativacao' => null, 
									  	'dmadtinativacao' => null, 
									  	'dmeid' => null,
									  	'dmetipo' => null,
									  );

    public function formularioArquivos($complementoDisabled = '') {

        $dmdid = $this->dmdid ? $this->dmdid : 0;
        $sql = "select dmaid, dmadsc, usucpfinclusao from demandasfies.demandaarquivo where dmdid = {$dmdid} and dmastatus = 'A' and dmatipo = '" . self::K_TIPO_GERAL . "'  order by dmaid desc";
        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : array();
//        ver($dados, d);
        ?>
        <div class="well">
            <?php if (!$complementoDisabled){ ?>
            <form id="form-arquivo" method="post" class="form-horizontal" enctype="multipart/form-data">
                <input name="dmaid" type="hidden" value="<?php echo $this->dmaid; ?>" >
                <input name="dmdid" type="hidden" value="<?php echo $this->dmdid; ?>" >
                <input name="action" type="hidden" value="salvar_arquivo" >
                <?php } ?>
                <fieldset>
                    <legend>Arquivos <span style="color: red">(<?php echo count($dados); ?>)</span></legend>
                    <div class="form-group">
                        <label for="dmadsc" class="col-lg-2 col-md-2 control-label">Arquivo:</label>
                        <div class="col-lg-10 col-md-10 ">
                            <input type="hidden" name="arqid" id="arqid" value="<?php echo $this->arqid; ?>">
                            <input type="file" class="btn btn-primary start" name="file" id="file" title="Selecionar arquivo" />
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="dmadsc" class="col-lg-2 col-md-2 control-label">Descrição:</label>
                        <div class="col-lg-10 col-md-10 ">
                            <input id="dmadsc" name="dmadsc" type="text" class="form-control" placeholder="" required="required"  value="<?php echo $this->dmadsc; ?>" <?php echo $complementoDisabled; ?>>
                        </div>
                    </div>
                </fieldset>
                <div>
                    <button title="Salvar" id="btn-salvar-arquivo" class="btn btn-success" type="button"><span class="glyphicon glyphicon-thumbs-up"></span> Salvar</button>
                </div>
                <?php if (!$complementoDisabled){ ?>
            </form>
        <?php } ?>

            <div style="margin-top: 20px; background: #ffffff !important;">
                <?php
                $listagem = new Simec_Listagem();
                $listagem->setCabecalho(array('Arquivo'));
                $listagem->addAcao('download', 'baixarArquivo');
                $listagem->addAcao('delete', 'inativarArquivo');

                $listagem->setAcaoComoCondicional('delete',array(array('campo' => 'usucpfinclusao', 'operacao' => 'igual', 'valor' => $_SESSION['usucpf'])));
                $listagem->esconderColunas (array('usucpfinclusao'));

                $listagem->setQuery($sql);
                $listagem->render();
                ?>
            </div>

        </div>

        <script type="text/javascript">
            $(function(){
                $('#btn-salvar-arquivo').click(function(){
                    if(!$('#dmadsc').val()){
                        alert('Favor preencher o campo de descrição.');
                        return false
                    }

                    options = {
                        success : function() {
                            jQuery("#div_listagem_arquivo").load('/demandasfies/demandasfies.php?modulo=principal/demandasformulario&acao=A&action=form_arquivo&dmdid='+$('#dmdid').val());
                        }
                    }

                    jQuery("#form-arquivo").ajaxForm(options).submit();
                });
            });

            function baixarArquivo(dmaid)
            {
                window.location = 'demandasfies.php?modulo=principal/demandasformulario&acao=A&action=download_arquivo&dmaid=' + dmaid + '&dmdid=' + '<?php echo $this->dmdid; ?>';
            }

            function inativarArquivo(dmaid)
            {
                if (confirm('Deseja realmente excluir o registro?')) {
                    window.location = 'demandasfies.php?modulo=principal/demandasformulario&acao=A&action=inativar_arquivo&dmaid=' + dmaid + '&dmdid=' + '<?php echo $this->dmdid; ?>';
                }
            }
        </script>
    <?php
    }

    public function formularioEntregas() {

        $tipo = self::K_TIPO_ENTREGAS;

        $dmdid = $this->dmdid ? $this->dmdid : 0;
        $dmeid = $this->dmeid ? $this->dmeid : 0;
        $sql = "select dmaid, dmadsc, to_char(da.dmadtinclusao, 'DD/MM/YYYY HH24:mi:ss'), usunome, usucpfinclusao
                from demandasfies.demandaarquivo da
                    INNER join seguranca.usuario u on da.usucpfinclusao = u.usucpf
                where dmdid = {$dmdid}
                and dmatipo = '{$tipo}'
                and dmeid = {$dmeid}
                and dmastatus = 'A'
                order by dmaid desc";
        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : array();
//        ver($dados, $sql, d);
        ?>
        <div class="well">
            <form id="form-arquivo" method="post" class="form-horizontal" enctype="multipart/form-data">
                <input name="dmaid"   type="hidden" value="<?php echo $this->dmaid; ?>" >
                <input name="dmdid"   type="hidden" value="<?php echo $this->dmdid; ?>" >
                <input name="dmeid"   type="hidden" value="<?php echo $this->dmeid; ?>" >
                <input name="dmatipo" type="hidden" value="<?php echo $tipo; ?>">
                <input name="action"  type="hidden" value="salvar_arquivo" >
                <fieldset>
                    <legend>Arquivos <span style="color: red">(<?php echo count($dados); ?>)</span></legend>
                    <div class="form-group">
                        <label for="dmadsc" class="col-lg-2 control-label">Arquivo:</label>
                        <div class="col-lg-10">
                            <input type="hidden" name="arqid" id="arqid" value="<?php echo $this->arqid; ?>">
                            <input type="file" class="btn btn-primary start" name="file" id="file" title="Selecionar arquivo" />
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="dmadsc" class="col-lg-2 control-label">Descrição:</label>
                        <div class="col-lg-10">
                            <input id="dmadsc" name="dmadsc" type="text" class="form-control" placeholder="" required="required"  value="<?php echo $this->dmadsc; ?>">
                        </div>
                    </div>
                </fieldset>
                <div>
                    <button title="Salvar" id="btn-salvar-arquivo" class="btn btn-success" type="submit"><span class="glyphicon glyphicon-thumbs-up"></span> Salvar</button>
                </div>
            </form>

            <div style="margin-top: 20px; background: #ffffff !important;">
                <?php
                $listagem = new Simec_Listagem();
                $listagem->setCabecalho(array('Arquivo', 'Dt. Inclusão', 'Responsável'));
                $listagem->addCallbackDeCampo(array('dmadsc', 'usunome'), 'alinhaParaEsquerda');
                $listagem->addAcao('download', 'baixarArquivo');
                $listagem->addAcao('delete', 'inativarArquivo');

                $listagem->setAcaoComoCondicional('delete',array(array('campo' => 'usucpfinclusao', 'operacao' => 'igual', 'valor' => $_SESSION['usucpf'])));
                $listagem->esconderColunas (array('usucpfinclusao'));

                $listagem->setQuery($sql);
                $listagem->render();
                ?>
            </div>

        </div>

        <script type="text/javascript">
            $(function(){
                $('#btn-salvar-arquivo').click(function(){
                    if(!$('#dmadsc').val()){
                        alert('Favor preencher o campo de descrição.');
                        return false
                    }
                });
            });

            function baixarArquivo(dmaid)
            {
                window.location = 'demandasfies.php?modulo=principal/demandasformulario&acao=A&action=download_arquivo&dmaid=' + dmaid + '&dmdid=' + '<?php echo $this->dmdid; ?>';
            }

            function inativarArquivo(dmaid)
            {
                if (confirm('Deseja realmente excluir o registro?')) {
                    window.location = 'demandasfies.php?modulo=principal/demandasformulario&acao=A&dmdid=<?php echo $this->dmdid; ?>&aba=entregas&dmaid=' + dmaid + '&dmeid=<?php echo $this->dmeid; ?>&action=inativar_arquivo';
                }
            }
        </script>
    <?php
    }
}