<?php
/**
 * Classe de mapeamento da entidade siconv.propostaws
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-11-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Siconv_Model_Propostaws
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Siconv_Model_PropostaWs extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'siconv.propostaws';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'id',
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
        'id' => null,
        'ano' => null,
        'atribuicaorespanalise' => null,
        'bensservicows' => null,
        'capacidadetecnica' => null,
        'contabancariaws' => null,
        'cronogramadesembolsows' => null,
        'cronogramafisicows' => null,
        'datapropostaws' => null,
        'fimexecucao' => null,
        'idhash' => null,
        'inicioexecucao' => null,
        'justificativa' => null,
        'membroparticipantews' => null,
        'modalidadepropostaws' => null,
        'objetoconvenio' => null,
        'orgaoconcedentews' => null,
        'orgaoexecutorws' => null,
        'parecerplanotrabalho' => null,
        'parecerproposta' => null,
        'proponentews' => null,
        'propostaparticipemembrows' => null,
        'propostaprogramaws' => null,
        'repasseexerciciofuturopropostaws' => null,
        'sequencial' => null,
        'situacaoenviocontratorepassews' => null,
        'situacaolegado' => null,
        'situacaoprojetobasicows' => null,
        'situacaopropostaws' => null,
        'valorcontrapartida' => null,
        'valorcontrapartidabensservicos' => null,
        'valorcontrapartidafinanceira' => null,
        'valorglobal' => null,
        'valorrepasse' => null,
        'datacarga' => null,
        'valor' => null,
        'emeid' => null,
        'numeroconvenio' => null,
    );

}//end Class
?>