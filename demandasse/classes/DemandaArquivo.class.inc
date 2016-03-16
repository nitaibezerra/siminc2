<?php
	
class DemandaArquivo extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandasse.demandaarquivo";

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
									  );

    public function formularioArquivos($complementoDisabled = '') {

        $dmdid = $this->dmdid ? $this->dmdid : 0;
        $sql = "select dmaid, dmadsc from demandasse.demandaarquivo where dmdid = {$dmdid} order by dmaid desc";
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
                    <legend>Arquivos (<?php echo count($dados); ?>)</legend>
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
                $listagem->addAcao('edit', 'editarArquivo');
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
                            jQuery("#div_listagem_arquivo").load('/demandasse/demandasse.php?modulo=principal/demandasformulario&acao=A&action=form_arquivo&dmdid='+$('#dmdid').val());
                        }
                    }

                    jQuery("#form-arquivo").ajaxForm(options).submit();
                });
            });
        </script>
    <?php
    }
}