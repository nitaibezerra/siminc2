<?php
/**
 * Classe de mapeamento da entidade painel.seriehistorica
 *
 * @category Class
 * @package  A1
 * @author   MARCUS VINÍCIUS LOUVEM ROCHA <marcus.rocha@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-10-2015
 * @link     no link
 */


/**
 * Painel_Model_Seriehistorica
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Painel_Model_Seriehistorica extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'painel.seriehistorica';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'sehid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'regid' => array('tabela' => 'painel.regionalizacao', 'pk' => 'regid'),
        'indid' => array('tabela' => 'painel.indicador', 'pk' => 'indid'),
        'dpeid' => array('tabela' => 'painel.detalheperiodicidade', 'pk' => 'dpeid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'sehid' => null,
        'indid' => null,
        'sehvalor' => null,
        'sehstatus' => null,
        'sehqtde' => null,
        'dpeid' => null,
        'sehdtcoleta' => null,
        'regid' => null,
        'sehbloqueado' => null,
        'dmiid' => null,
    );


    public function __construct()
    {
        $GLOBALS['nome_bd'] = '';
        $GLOBALS['servidor_bd'] = '';
        $GLOBALS['porta_bd'] = '5432';
        $GLOBALS['usuario_db'] = '';
        $GLOBALS['senha_bd'] = '';

    }


    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'sehid' => array('Digits'),
            'indid' => array('Digits'),
            'sehvalor' => array('allowEmpty' => true),
            'sehstatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
            'sehqtde' => array(),
            'dpeid' => array('allowEmpty' => true, 'Digits'),
            'sehdtcoleta' => array('allowEmpty' => true),
            'regid' => array('allowEmpty' => true, 'Digits'),
            'sehbloqueado' => array('allowEmpty' => true),
            'dmiid' => array('allowEmpty' => true, 'Digits'),
        );
    }//end getCamposValidacao($dados)

    public function carregarGraficoIndicador($indicador)
    {
        $sql = "SELECT tid1.tiddsc AS tipo, SUM(dsh.dshqtde::INTEGER) AS total
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                WHERE sh.indid IN ({$indicador})
                AND dpe.dpeanoref = '2015'
                AND sh.sehstatus <> 'I'
                AND dsh.dshuf = 'BA'
                AND dsh.dshcodmunicipio = '2909307'
                GROUP BY tipo
                ORDER BY tipo limit 15";
        return $this->carregar($sql);
    }


    public function carregarTabelaIndicadoresVinculados($indicador1 = null, $indicador2 = null)
    {

        $sql = "SELECT tipo, escola, SUM(totalequipamento) AS totalequipamento, SUM(totaldemanda) AS totaldemanda,dshcod
                    FROM (
                        SELECT
                            'Equipamento' AS detalhe,
                            tid1.tiddsc AS tipo,
                            esc.escdsc AS escola,
                            SUM(dsh.dshqtde::INTEGER) AS totalequipamento,
                            0 AS totaldemanda, dsh.dshcod
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                        INNER JOIN painel.escola esc ON esc.esccodinep = dsh.dshcod
                        WHERE sh.indid IN (3205)
                        AND dpe.dpeanoref = '2015'
                        AND sh.sehstatus <> 'I'
                        --AND dsh.dshcod = '29331978' --Código Escola
                        --AND dsh.dshuf = 'BA' --UF
                        AND dsh.dshcodmunicipio = '2909307' --Município
                        GROUP BY detalhe, tipo, escola, dsh.dshcod
                    UNION ALL
                        SELECT
                            'Demanda' AS detalhe,
                            tid1.tiddsc AS tipo,
                            esc.escdsc AS escola,
                            0 AS totalequipamento,
                            SUM(dsh.dshqtde::INTEGER) AS totaldemanda,dsh.dshcod
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                        INNER JOIN painel.escola esc ON esc.esccodinep = dsh.dshcod
                        WHERE sh.indid IN (3272)
                        AND dpe.dpeanoref = '2015'
                        AND sh.sehstatus <> 'I'
                        --AND dsh.dshcod = '29331978' --Código Escola
                        --AND dsh.dshuf = 'BA' --UF
                        AND dsh.dshcodmunicipio = '2909307' --Município
                        GROUP BY detalhe, tipo, escola, dsh.dshcod
                    ) AS FOO
                    GROUP BY tipo, escola, dshcod
                    ORDER BY tipo, escola
                    ";

        return $this->carregar($sql);
    }

    public function detalhamentoIndicadorfunction($indid)
    {
        $sql = "select
				ind.indnome,
				exo.exodsc,
				sec.secdsc,
				aca.acadsc,
				aca.acaid,
				ind.indobjetivo,
				ind.indcumulativo,
				ind.regid,
				unm.unmdesc,
				ind.indformula,
				ind.indtermos,
				ind.indfontetermo,
				ind.indobservacao,
				ind.indvispadrao,
				per.perdsc,
				est.estdsc,
				col.coldsc,
				reg.regdescricao,
				ume.umedesc
			from
				painel.indicador ind
			left join
				painel.eixo exo ON exo.exoid = ind.exoid
			left join
				painel.secretaria sec ON sec.secid = ind.secid
			left join
				painel.acao aca ON aca.acaid = ind.acaid
			left join
				painel.unidademedicao unm ON unm.unmid = ind.unmid
			left join
				painel.periodicidade per ON per.perid = ind.perid
			left join
				painel.unidademeta ume on ind.umeid = ume.umeid
			left join
				painel.estilo est ON est.estid = ind.estid
			left join
				painel.coleta col ON col.colid = ind.colid
			left join
				painel.regionalizacao reg ON reg.regid = ind.regid
			where
				ind.indid = {$indid}
			limit 1";
        return $this->carregar($sql);

    }


}//end Class
?>
