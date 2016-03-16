<?php
/**
 * Classe de mapeamento da entidade sase.pne
 *
 * @category Class
 * @package  A1
 * @author   EDUARDO DUNICE NETO <eduardo.neto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 14-10-2015
 * @link     no link
 */



/**
 * Par3_Model_Pne
 *
 * @category Class
 * @package  A1
 * @author   EDUARDO DUNICE NETO <eduardo.neto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Sase_Model_Pne extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sase.pne';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'pneid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'subid' => array('tabela' => 'sase.submeta', 'pk' => 'subid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pneid' => null,
        'subid' => null,
        'estuf' => null,
        'pnegama' => null,
        'pnevalor' => null,
        'pneconvergencia' => null,
        'pneano' => null,
        'pnetaxa' => null,
        'pnesalario' => null,
        'muncod' => null,
        'pnetipo' => null,
        'regcod' => null,
        'mescod' => null,
        'pnetipometa' => null,
        'pnevalormeta' => null,
        'pnesemvalor' => null,
        'pnepossuiplanoremvigente' => null,
        'pneplanorefcaput' => null,
        'pneanoprevisto' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'pneid' => array(  'Digits'  ),
            'subid' => array(  'Digits'  ),
            'estuf' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 2))  ),
            'pnegama' => array( 'allowEmpty' => true ),
            'pnevalor' => array( 'allowEmpty' => true ),
            'pneconvergencia' => array( 'allowEmpty' => true ),
            'pneano' => array( 'allowEmpty' => true, 'Digits'  ),
            'pnetaxa' => array( 'allowEmpty' => true ),
            'pnesalario' => array( 'allowEmpty' => true ),
            'muncod' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 7))  ),
            'pnetipo' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))  ),
            'regcod' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 2))  ),
            'mescod' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 4))  ),
            'pnetipometa' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))  ),
            'pnevalormeta' => array( 'allowEmpty' => true ),
            'pnesemvalor' => array( 'allowEmpty' => true ),
            'pnepossuiplanoremvigente' => array( 'allowEmpty' => true ),
            'pneplanorefcaput' => array( 'allowEmpty' => true ),
            'pneanoprevisto' => array( 'allowEmpty' => true, 'Digits'  ),
        );
    }//end getCamposValidacao($dados)


    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function gravar()
    {
        global $url;
        $this->popularDadosObjeto();
        $url .= '&pneid=' . $this->pneid;

        try{
            $sucesso = $this->salvar();
            $this->commit();
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }

        if($sucesso){
            simec_redirecionar($url, 'success');
        }
        simec_redirecionar($url, 'error');
    }//end gravar()


   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function excluir()
    {
        global $url;
       // $url = 'aspar.php?modulo=principal/proposicao/index&acao=A';
        try{
            $this->excluir();
            $this->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }//end excluir()


   /**
     * Função carregarPneUnidade
     * - carrega os dados PNE da Unidade
     *
     * @return object $objPne
     **/
    public function carregarPneUnidade($campo, $valor, $ano, $subid)
    {
        $sql = "SELECT pneid FROM sase.pne
                WHERE $campo = '$valor' AND pneano = '$ano' AND subid = $subid";

        $pneid = self::pegaUm($sql);

        return new Sase_Model_Pne($pneid);
    }//end carregarPneUnidade()



}//end Class
?>