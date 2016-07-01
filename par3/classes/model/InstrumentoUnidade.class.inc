<?php

/**
 * Classe de mapeamento da entidade par3.instrumentounidade
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 21-09-2015
 * @link     no link
 */

/**
 * Par_Model_Instrumentounidade
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 21-09-2015
 * @link     no link
 */
class Par3_Model_InstrumentoUnidade extends Modelo
{

    /**
     * Nome da tabela especificada
     *
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.instrumentounidade';

    /**
     * Chave primaria.
     *
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('inuid');

    /**
     * Chaves estrangeiras.
     *
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array(
            'tabela' => 'usuario',
            'pk' => 'usucpf',
        ),
        'muncod, mun_estuf' => array(
            'tabela' => 'territorios.municipio',
            'pk' => 'muncod, estuf',
        ),
        'itrid' => array(
            'tabela' => 'par3.instrumento',
            'pk' => 'itrid',
        ),
        'estuf' => array(
            'tabela' => 'territorios.estado',
            'pk' => 'estuf',
        )
    );

    /**
     * Atributos
     *
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'inuid' => null,
        'itrid' => null,
        'estuf' => null,
        'muncod' => null,
        'mun_estuf' => null,
        'inudescricao' => null,
        'usucpf' => null,
        'inudata' => null,
        'inucnpj' => null,
        'qrpid' => null,
    );


    /**
     * Função prepararFiltro
     * - monta o filtro SQL para lista de intstrumentos unidade.
     *
     * @return retorna um array de filtros.
     *
     */
    public function prepararFiltro()
    {
        $where = array();
        
            if ($_POST['itrid'] != '' && !is_array($_POST['itrid'])) {
            $where[] = "itrid = {$_POST['itrid']}";
        }
        
        if ($_POST['itrid'] != '' && is_array($_POST['itrid'])) {
        	$itrids = implode(',', $_POST['itrid']);
        	$where[] = "itrid IN ({$itrids})";
        }
        
        if ($_POST['mundescricao'] != '') {
            $where[] = "UPPER(public.removeacento(inudescricao)) ILIKE UPPER(public.removeacento('%{$_POST['mundescricao']}%'))";
        }

        if ($_POST['estuf'] != '') {
            $where[] = "estuf = '{$_POST['estuf']}'";
        }

        return $where;

    }//end prepararFiltro()


    /**
     * Função pegarSQLLista
     * - monta SQL para lista de intstrumentos unidade.
     *
     * @return retorna a query.
     *
     */
    public function pegarSQLLista()
    {
        $where = self::prepararFiltro();

        $sql = 'SELECT
    	           inuid,
    	           inudescricao
    	       FROM
    	           par3.instrumentounidade
    	       WHERE
    	           ' . implode(' AND ', $where) . '
    	       ORDER BY
    	           2';

        return $sql;

    }//end pegarSQLLista()


    /**
     * Função recuperarInuidPar
     * - recupera inuid do PAR antigo.
     *
     * @return integer $inuid.
     *
     */
    public function recuperarInuidPar($inuid)
    {
        $instrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);

        $where = " AND estuf = '".$instrumentoUnidade->estuf."'";

        if ($instrumentoUnidade->itrid === '2') $where = " AND muncod = '".$instrumentoUnidade->muncod."'";

        $sql = 'SELECT
    	           inuid
    	       FROM
    	           par.instrumentounidade
    	       WHERE
    	           itrid = '.$instrumentoUnidade->itrid.$where;

        return $instrumentoUnidade->pegaUm($sql);

    }//end recuperarInuidPar()


    /**
     * Função retornarQrpid
     * - recupera retorna qrpid.
     *
     * @return integer $qrpid.
     *
     */
    public function retornarQrpidQuestoesEstrategicas()
    {
        if (!$this->qrpid) {
            $sql = "INSERT INTO questionario.questionarioresposta(queid, qrptitulo, qrpdata)
                    VALUES(108, 'Reposta Questões Estratégicas - inuid: {$this->inuid}', now())
                    RETURNING qrpid;";

            $this->qrpid = $this->pegaUm($sql);
            $this->salvar();
            $this->commit();
        }

        return $this->qrpid;

    }//end retornarQrpid()



}//end class

?>