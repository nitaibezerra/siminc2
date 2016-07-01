<?php

/**
 * Classe de controle da equipe local
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-10-2015
 * @link     no link
 */

/**
 * Par3_Controller_EquipeLocal
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-10-2015
 * @link     no link
 */
class Par3_Controller_EquipeLocal extends Controle
{
    /* Constantes da Equipe local
     * */
// 	const PREFEITURA          = 1;

    public function __construct()
    {
        parent::__construct();
    }


    /**
     * Função formEquipeLocal
     * - monta o formulario de equipe local
     *
     * @return string escreve a lista.
     *
     */
    public function formEquipeLocal($disabled = 'disabled', $objPessoaEquipe = null)
    {
        global $simec;

        echo '<input type="hidden" name="entid" value="'.$objPessoaEquipe->entid.'"/>';
        echo $simec->cpf('entcpf', 'CPF', $objPessoaEquipe->entcpf, array('class' => 'cpf', $disabled, 'data-pessoa' => true, 'data-pessoa-campos' => '{"entnome": "no_pessoa_rf"}', 'required'=>true));
        echo $simec->input('entnome', 'Nome', $objPessoaEquipe->entnome, array('maxlength' => '255', $disabled, 'required'));

        $sql = Par3_Model_EquipeLocalSeguimento::pegarSQLSelectCombo();
        $arrAttr = Array($disabled=>$disabled, 'required');
        echo $simec->select('eseid', 'Seguimento', $objPessoaEquipe->eseid, $sql, $arrAttr);

        $sql = Par3_Model_EquipeLocalCargo::pegarSQLSelectCombo(Array());
        $arrAttr = Array($disabled=>$disabled, 'required');
        echo $simec->select('ecaid', 'Cargo', $objPessoaEquipe->ecaid, $sql, $arrAttr);

        echo $simec->email('entemail', 'E-mail', $objPessoaEquipe->entemail, array('class' => 'email', $disabled, 'required'));
        echo "<script>
                $('#eseid').change(function(e) {
                    	e.preventDefault();
                    	var options = $(\"#ecaid\");
                    	$.ajax({
                		url: window.location.href,
                		data: {'req':'carregarCargos', 'eseid':$(this).val()},
                		method: 'post',
                		success: function (result) {
                			options.empty();
                			var result = JSON.parse(result);
                			$.each(result, function() {
                			    options.append(new Option(this.descricao, this.codigo));
                			});
                			options.focus();
                            $('#ecaid').trigger('chosen:updated');
                		}
                	});
                });
              </script>";
    }//end formEquipeLocal()


    /**
     * Função formNovoEquipeLocal
     * - monta o formulario de equipe local
     *
     * @return string escreve a lista.
     *
     */
    public function formNovoEquipeLocal($arrPost)
    {
        ob_clean();

	    echo '<div class="ibox-content">
	           <div class="ibox-title">
            	    <h3>Novo Integrantes</h3>
            	</div>
	            <form method="post" name="formEquipe" id="formEquipe" class="form form-horizontal">
                <input type="hidden" name="req" value="salvar"/>
                <input type="hidden" name="inuid" value="'.$arrPost['inuid'].'"/>
                <input type="hidden" name="tenid" value="'.Par3_Model_InstrumentoUnidadeEntidade::EQUIPE.'"/>';
	    self::formEquipeLocal($disabled, $obj);
	    echo '  </form>
            	<div class="ibox-footer">
            		<button type="button" class="btn btn-success salvar" '.$disabled.'><i class="fa fa-floppy-o"></i> Salvar</button>
            	</div>
	           </div>';
	    die();
    }//end formNovoEquipeLocal()


    /**
     * Função listaEquipe
     * - monta a lista de equipe.
     *
     * @return escreve a lista.
     *
     */
    public function listaEquipe($arrPost)
    {
        $sql = Par3_Model_EquipeLocal::pegarSQLLista($arrPost);

        $cabecalho = array('CPF', 'Nome', 'Função/Cargo', 'Segmento', 'E-mail');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->addCallbackDeCampo('entcpf','formatar_cpf'); // função php para formatar cpf
        $listagem->addAcao('delete', 'inativaIntegranteEquipe');

        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

    }//end listaEquipe()


    /**
     * Função salvar
     * - salvar novo membro da equipe.
     *
     * @return void.
     *
     */
    public function salvar($arrPost)
    {
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidadeEntidade();
        $modelEquipe             = new Par3_Model_EquipeLocal();

        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$arrPost['inuid']}&menu=equipe";

        try {
            $arrPost['entcpf'] = str_replace(Array('.', '-'), '', $arrPost['entcpf']);

            $modelInstrumentoUnidade->popularDadosObjeto($arrPost);
            $arrPost['entid'] = $modelInstrumentoUnidade->salvar();

            $modelEquipe
            ->popularDadosObjeto($arrPost)
            ->salvar();

            $modelEquipe->commit();

            simec_redirecionar($url, 'success');
        } catch(Exception $e) {
            $modelEquipe->rollback();
            simec_redirecionar($url, 'error');
        }

    }//end salvar()


    /**
     * Função invativar
     * - inativa membro da equipe.
     *
     * @return void.
     *
     */
    public function invativar($arrPost)
    {
        $modelEquipe = new Par3_Model_EquipeLocal($arrPost['eloid']);

        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$arrPost['inuid']}&menu=equipe";

        try {
            $modelEquipe->elostatus     = 'I';
            $modelEquipe->elodtexclusao = date('Y-m-d G:i:s');
            $modelEquipe->salvar();

            $modelEquipe->commit();

            simec_redirecionar($url, 'success');
            die();
        } catch(Exception $e) {
            $modelEquipe->rollback();
            simec_redirecionar($url, 'error');
            die();
        }

    }//end salvar()


}//end class

?>