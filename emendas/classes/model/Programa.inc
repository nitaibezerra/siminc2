<?php
/**
 * Classe de mapeamento da entidade emendas.programa
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 04-01-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Emendas_Model_Programa
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Emendas_Model_Programa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.programa';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'prgid',
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
        'prgid' => null,
        'prgtitulo' => null,
        'prgcodigo' => null,
        'idhash' => null,
        'prgstatus' => null,
    );

    public function atualizar($obWS, $filtro = null)
    {
        $retorno = $obWS->consultarPrograma($filtro);

        $prgid = $this->getPrograma($retorno);

        if($prgid){
            $this->carregarPorId((int)$prgid);
        } else {
            $this->prgid = null;
        }

        $this->idhash = $retorno->idHash;
        $this->prgcodigo = $retorno->codigo;
        $this->prgtitulo = $retorno->nome;
        $this->prgstatus = $retorno->status;

        $this->salvar();
    }

    public function getPrograma($programa)
    {
        $sql = "select prgid from emendas.programa where idhash = '{$programa->idHash}'";
        $prgid = $this->pegaUm($sql);
        
        if(!$prgid){
            $this->prgcodigo = $programa->codigo;
            $this->idhash = $programa->idHash;

            $this->salvar();
            $prgid = $this->prgid;
        }

        return $prgid;
    }

}//end Class
?>