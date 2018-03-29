<?php
/**
 * Classe de mapeamento da entidade monitora.pi_enquadramentodespesa
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 23-11-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Monitora_Model_Pi_enquadramentodespesa
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Monitora_Model_PiEnquadramentoDespesa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'monitora.pi_enquadramentodespesa';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'eqdid',
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
        'eqdid' => null,
        'eqdcod' => null,
        'eqddsc' => null,
        'eqdano' => null,
        'eqdstatus' => null,
        'mpneid' => null,
        'eqdstreduzido' => null,
        'eqdrp' => null,
    );

    public function recuperarTodos($stCampos = '*', $arClausulas = null, $stOrdem = null, array $opcoes = array()) {
        // -- Retornar a query ao invés dos dados
        $opcoes['query'] = isset($opcoes['query']) ? $opcoes['query'] : false;
        // -- Faz join com as tabelas mapeadas nas foreing keys
        $opcoes['join'] = isset($opcoes['join']) ? $opcoes['join'] : false;

        $sql = "SELECT DISTINCT $stCampos
                FROM $this->stNomeTabela t1
                  LEFT JOIN planacomorc.resultadoprimario as rp ON ( t1.eqdrp = rp.rpcod )
                ";

        $contagemTabelas = 2;
        if ($opcoes['join']) {
            if (empty($this->arChaveEstrangeira)) {
                throw new Exception("Não existe nenhuma chave estranjeira definida para a tabela {$this->stNomeTabela}.");
            }
            $sql .= $this->gerarJoin($contagemTabelas, $opcoes['join']);
        }
        if ($opcoes['leftjoin']) {
            if (empty($this->arChaveEstrangeira)) {
                throw new Exception("Não existe nenhuma chave estranjeira definida para a tabela {$this->stNomeTabela}.");
            }
            $sql .= $this->gerarJoin($contagemTabelas, $opcoes['leftjoin'], 'LEFT');
        }

        $sql .= $arClausulas ? " WHERE " . implode(" AND ", $arClausulas) : "";
        $sql .= $stOrdem ? " ORDER BY $stOrdem" : "";

        if ($opcoes['query']) {
            return $sql;
        }

        $resultado = $this->carregar($sql);
        return $resultado ? $resultado : array();
    }

    public function recuperarRP($eqdid){
      $sql = 'SELECT irpcod FROM monitora.enquadramentorp WHERE eqdid = '. (int)$eqdid;
      $dados = $this->carregar($sql);
      $dados = $dados ? $dados : [];
      $listaResultado = array();
      foreach ($dados as $key => $value) {
        $listaResultado[] = $value['irpcod'];
      }
      return $listaResultado;
    }

    public function recuperarTodosDetail(stdClass $param){
      $where = NULL;
        if($param->exercicio){
          $where = "\n AND eqdano = '". (int)$param->exercicio . "'";
        }

        $sql = "
          SELECT DISTINCT
          *,
          REPLACE(REPLACE(REPLACE(ARRAY(
      			SELECT DISTINCT
      				' ' || erp.irpcod || ' - ' ||rp.irpdsc || ' '
      			FROM monitora.enquadramentorp erp
              JOIN public.identresultprimario AS rp on erp.irpcod = rp.irpcod
      			WHERE
      				erp.eqdid = pi_enquadramentodespesa.eqdid
            )::TEXT, '{', ''), '}', ''), '\"', '') AS lista_rp
          FROM monitora.pi_enquadramentodespesa
          WHERE
            eqdstatus = 'A'
            $where
          ORDER BY
            eqddsc
        ";
        $lista = $this->carregar($sql);
        // implement here cast de array
        return $lista;
    }
}//end Class
?>
