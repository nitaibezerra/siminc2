<?php
/**
 * Classe de mapeamento da entidade proporc.ploafinanceiro.
 *
 * $Id: Ploafinanceiro.php 101146 2015-08-11 21:22:58Z maykelbraz $
 */

/**
 * Mapeamento da entidade proporc.ploafinanceiro.
 *
 * @see Modelo
 */
class Proporc_Model_Ploafinanceiro extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'proporc.ploafinanceiro';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'plfid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'dpaid' => array('tabela' => 'despesaacao', 'pk' => 'dpaid'),
        'usucpf' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'plfid' => null,
        'dpaid' => null,
        'mtrid' => null,
        'usucpf' => null,
        'plfvalor' => null,
        'plfinclusao' => null,
    );

    public function contarDetalhamentoGrupo($gdpid)
    {
        $gdpid = !empty($gdpid)?$gdpid:$this->gdpid;
        if (empty($gdpid)) {
            throw new Exception('É preciso informar o ID do grupo para consultar seus detalhamentos.');
        }

        $sql = <<<DML
SELECT COUNT(1) AS "numFinanceiros"
  FROM proporc.grupodespesa gdp
    INNER JOIN proporc.despesa dsp USING(gdpid)
    INNER JOIN proporc.ploafinanceiro plf ON(dsp.dspid = plf.mtrid)
  WHERE gdp.gdpid = :gdpid
DML;
        $dml = new Simec_DB_DML($sql);
        $dml->addParam('gdpid', $gdpid);

        return $this->carregar($dml);
    }
}
