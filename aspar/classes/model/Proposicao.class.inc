<?php
/**
 * Classe de mapeamento da entidade aspar.proposicao.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade aspar.proposicao.
 *
 * @see Modelo
 */
class Aspar_Model_Proposicao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'aspar.proposicao';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'prpid'
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'prpid' => null,
        'posid' => null,
        'tipid' => null,
        'refid' => null,
        'tprid' => null,
        'usucpf' => null,
        'priid' => null,
        'prptextoementa' => null,
        'prpdtsolicitacao' => null,
        'prpano' => null,
        'prpnumerosenado' => null,
        'prpnumerocamara' => null,
        'prpdtparecer' => null,
        'prpdscparecer' => null,
        'prpdtinclusao' => null,
        'prpprazo' => null,
        'prpstatus' => null,
    );

    public function gravar()
    {
        global $url;
        $this->popularDadosObjeto();
        $this->usucpf = $_SESSION['usucpforigem'];

        try{
            $sucesso = $this->salvar();
            $this->commit();
        } catch (Simec_Db_Exception $e) {
            $url .= '&prpid=' . $this->prpid;
            simec_redirecionar($url, 'error');
        }

        $url .= '&prpid=' . $this->prpid;
        if($sucesso){
            simec_redirecionar($url, 'success');
        }
        simec_redirecionar($url, 'error');
    }

}