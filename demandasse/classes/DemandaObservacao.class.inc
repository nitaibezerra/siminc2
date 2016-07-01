<?php
	
class DemandaObservacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandasse.demandaobservacao";

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
    protected $arAtributos     = array(
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
									  );


    public function formularioObservacoes($complementoDisabled = '') {

        $dmdid = $this->dmdid ? $this->dmdid : 0;
        $sql = "select dmoid, dmotexto from demandasse.demandaobservacao where dmdid = {$dmdid} order by dmoid desc";
        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : array();
//        ver($dados, d);
        ?>
        <div class="well">
            <?php if (!$complementoDisabled){ ?>
            <form id="form-observacao" method="post" class="form-horizontal" >
                <input name="dmoid" type="hidden" value="<?php echo $this->dmoid; ?>" >
                <input name="dmdid" type="hidden" value="<?php echo $this->dmdid; ?>" >
                <input name="action" type="hidden" value="salvar_observacao" >
                <?php } ?>
                <fieldset>
                    <legend>Observações (<?php echo count($dados); ?>)</legend>
                    <div class="form-group">
                        <label for="dmdtitulo" class="col-lg-2 control-label">Observação:</label>
                        <div class="col-lg-10">
                            <textarea class="form-control" rows="3" name="dmotexto" id="dmotexto"><?php echo $this->dmotexto; ?></textarea>
                        </div>
                    </div>
                </fieldset>
                <div>
                    <button title="Salvar" id="btn-salvar-observacao" class="btn btn-success" type="button"><span class="glyphicon glyphicon-thumbs-up"></span> Salvar</button>
                </div>
            <?php if (!$complementoDisabled){ ?>
                </form>
            <?php } ?>

            <div style="margin-top: 20px; background: #ffffff !important;">
                <?php
                $listagem = new Simec_Listagem();
                $listagem->setCabecalho(array('Observação'));
                $listagem->addAcao('edit', 'editarObservacao');
                $listagem->setQuery($sql);
                $listagem->render();
                ?>
            </div>

        </div>

        <script type="text/javascript">
            $(function(){
                $('#btn-salvar-observacao').click(function(){
                    if(!$('#dmotexto').val()){
                        alert('Favor preencher o campo de texto.');
                        return false
                    }

                    options = {
                        success : function() {
                            jQuery("#div_listagem_observacao").load('/demandasse/demandasse.php?modulo=principal/demandasformulario&acao=A&action=form_observacao&dmdid='+$('#dmdid').val());
                        }
                    }

                    jQuery("#form-observacao").ajaxForm(options).submit();
                });
            });
        </script>
    <?php
    }
}