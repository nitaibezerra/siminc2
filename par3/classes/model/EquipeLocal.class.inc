<?php
/**
 * Classe de modelo do EquipeLocal
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-10-2015
 * @link     no link
 */


/**
 * Par3_Model_EquipeLocal
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-10-2015
 * @link     no link
 */
class Par3_Model_EquipeLocal extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.equipelocal';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'eloid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'ecaid' => array('tabela' => 'par3.equipelocal_cargo', 'pk' => 'ecaid'),
        'entid' => array('tabela' => 'par3.instrumentounidade_entidade', 'pk' => 'entid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'eloid' => null,
        'entid' => null,
        'ecaid' => null,
        'ecadsc' => null,
        'elostatus' => null,
        'elodtcriacao' => null,
        'elodtexclusao' => null,
    );


    /**
     * Função pegarSQLListaEquipeLocal
     * - intaiva Equipe Local.
     *
     * @return void.
     *
     */
    public function pegarSQLLista($arrPost)
    {
        $where = self::montarFiltroLista($arrPost);

        $where[] = "ent.inuid = {$arrPost['inuid']}";
        $where[] = "ent.tenid = ".Par3_Model_InstrumentoUnidadeEntidade::EQUIPE;

        $sql = "SELECT
                	elo.eloid as id,
                	ent.entcpf,
                	ent.entnome,
                	eca.ecadsc,
                	ese.esedsc,
                	ent.entemail
                FROM par3.equipelocal elo
                INNER JOIN par3.equipelocal_cargo eca ON eca.ecaid = elo.ecaid
                INNER JOIN par3.equipelocal_seguimento ese ON ese.eseid = eca.eseid
                INNER JOIN par3.instrumentounidade_entidade ent ON ent.entid = elo.entid
                WHERE
                    ".implode(' AND ', $where);

        return $sql;
    }//end pegarSQLListaEquipeLocal()


    /**
     * Função montarFiltroEquipeLocal
     * - intaiva Equipe Local.
     *
     * @return void.
     *
     */
    public function montarFiltroLista($arrPost)
    {
        $where = array();
        if ($arrPost['elostatus'] != '') $where[] = "elo.elostatus = '{$arrPost['elostatus']}'";

        return $where;
    }//end pegarSQLListaEquipeLocal()


    /**
     * Função carregaArrayEquipe
     * - monta array equipe.
     *
     * @return void.
     *
     */
    public function carregaArrayEquipe($arrPost)
    {
        $sql = self::pegarSQLLista($arrPost);

        $arrDados = self::carregar($sql);

        return $arrDados;
    }//end carregaArrayEquipe()

}