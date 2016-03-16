<?php
/**
 * Classe de mapeamento da entidade par3.questoesestrategicas_escolas_Gremio
 *
 * @category Class
 * @package  A1
 * @author   EDUARDO DUNICE NETO <eduardo.neto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 20-10-2015
 * @link     no link
 */



/**
 * Par3_Model_QuestoEsestrategicasEscolasGremio
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Par3_Model_QuestoEsestrategicasEscolasGremio extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.questoesestrategicas_escolas_gremio';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'qeeid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'qrpid' => array('tabela' => 'questionario.questionarioresposta', 'pk' => 'qrpid'),
        'perid' => array('tabela' => 'questionario.pergunta', 'pk' => 'perid'),
        'entid' => array('tabela' => 'entidade.entidade', 'pk' => 'entid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'qeeid' => null,
        'qrpid' => null,
        'perid' => null,
        'entid' => null,
    );

    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'qeeid' => array(  'Digits'  ),
            'qrpid' => array(  'Digits'  ),
            'perid' => array(  'Digits'  ),
            'entid' => array(  'Digits'  ),
        );
    }//end getCamposValidacao($dados)


    /**
     * Função pegarSQLLista
     * - monta SQL lista de escolas com Gremio
     *
     */
    public function pegarSQLLista($arrPost)
    {
        $where = self::montarFiltroSQL($arrPost);

        $sql = "SELECT DISTINCT
                    ent.entid as id,
                    qee.entid as entid,
                    mun.mundescricao    AS descricaoMunicipio,
                    ent.entcodent       AS codigoescola,
                    entnome           AS nomeescola
                FROM
                    entidade.entidade ent
                INNER JOIN entidade.endereco                    ede ON ent.entid     = ede.entid
                INNER JOIN territorios.municipio                mun ON mun.muncod    = ede.muncod
                INNER JOIN territorios.estado                   est ON est.estuf     = ede.estuf
                LEFT JOIN entidade.entidadedetalhe              etd ON ent.entcodent = etd.entcodent
                LEFT JOIN par3.questoesestrategicas_escolas_gremio  qee ON qee.entid     = ent.entid AND qee.qrpid = {$arrPost['qrpid']} AND perid = {$arrPost['perid']}
                LEFT JOIN questionario.questionarioresposta     qrp ON qrp.qrpid     = qee.qrpid
                LEFT JOIN par3.instrumentounidade               inu ON inu.qrpid     = qrp.qrpid
                WHERE
                    ".implode(' AND ', $where)."
                ORDER BY
                    mun.mundescricao, entnome";

        return $sql;

    }//end pegarSQLLista()


   /**
     * Função montarFiltroSQL
     * - montar filtro da SQL
     *
     */
    public function montarFiltroSQL($arrPost)
    {
        $where = array('ent.entcodent IS NOT NULL', "ent.entstatus = 'A'");

        $modelUnidade = new Par3_Model_InstrumentoUnidade($arrPost['inuid']);

        if ($modelUnidade->itrid === '2') {
            $where[] = "ede.muncod = '{$modelUnidade->muncod}'";
            $where[] = "tpcid = 3";
        } else {
            $where[] = "ede.estuf = '{$modelUnidade->estuf}'";
            $where[] = "tpcid = 1";
        }

        return $where;

    }//end montarFiltroSQL()


   /**
     * Função retornarQtdEscolas
     * - montar filtro da SQL
     *
     */
    public function retornarQtdEscolas($qrpid, $perid)
    {
        $sql = "SELECT count(DISTINCT entid) FROM {$this->stNomeTabela}
                WHERE qrpid = $qrpid AND perid = $perid";

        $qtd = $this->pegaUm($sql);

        return $qtd>0 ? $qtd : 0;

    }//end retornarQtdEscolas()


   /**
     * Função excluirPorEntid
     * - excluir escola
     *
     */
    public function excluirPorEntid($arrPost)
    {
        $sql = "DELETE FROM {$this->stNomeTabela}
                WHERE
                    qrpid = {$arrPost['qrpid']}
                    AND perid = {$arrPost['perid']}
                    AND entid = {$arrPost['entid']}";

        $qtd = $this->executar($sql);

    }//end excluirPorEntid()



}//end Class
?>