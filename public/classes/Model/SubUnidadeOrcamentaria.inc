<?php
/**
 * Classe de mapeamento da entidade public.subunidadeorcamentaria
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 16-08-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Public_Model_Subunidadeorcamentaria
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Public_Model_SubUnidadeOrcamentaria extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.subunidadeorcamentaria';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'suoid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'unoid' => array('tabela' => 'unidadeorcamentaria', 'pk' => 'unoid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'suoid' => null,
        'unoid' => null,
        'suocod' => null,
        'suosigla' => null,
        'suonome' => null,
        'suostatus' => null,
        'suoflaglimiteliberado' => null,
        'suocodigopi' => null,
    );

    /**
     * Função recuperarTodos
     * Método usado para recuperar todos os registros do banco, seguinto parâmetros
     * @param string $stCampos - String contendo nomes dos campos a serem carregados
     * @param array $arClausulas - Array contendo dados da cláusula where
     * @param string $stOrdem - String contendo dados da cláusula ordey by
     * @param array $opcoes Opções adicionais do método.
     * @example - $obNomeObjeto->recuperarTodos( 'campo1, campo2', array( "id = 123" ), nome )
     * @example - $obNomeObjeto->recuperarTodos() --> Se não passar parâmetros reconhece como todos os registros do banco.
     * @access public
     * @author Orion Teles de Mesquita
     * @since 12/02/2009
     */
    public function recuperarTodos( $stCampos = '*', $arClausulas = null, $stOrdem = null, array $opcoes = array())
    {
        // -- Retornar a query ao invés dos dados
        $opcoes['query'] = isset($opcoes['query'])?$opcoes['query']:false;
        // -- Faz join com as tabelas mapeadas nas foreing keys
        $opcoes['join'] = isset($opcoes['join'])?$opcoes['join']:false;

        $sql = "SELECT DISTINCT $stCampos FROM public.vw_subunidadeorcamentaria t1";

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

        $sql .= $arClausulas ? " WHERE ". implode( " AND ", $arClausulas ) : "";
        $sql .= $stOrdem ? " ORDER BY $stOrdem" : "";

        if ($opcoes['query']) {
            return $sql;
        }

        $resultado = $this->carregar($sql);
        return $resultado ? $resultado : array();
    }

    /**
     * Monta consulta para retornar Sub-Unidades.
     *
     * @param stdClass $filtro
     * @return string
     */
    public static function queryCombo(stdClass $filtro)
    {
        $where = '';
        if($filtro->listaSubUnidadeUsuario && !empty(join_simec(',', $filtro->listaSubUnidadeUsuario))){
            $where .= "\n                AND suo.suocod::INTEGER IN(". join_simec(',', $filtro->listaSubUnidadeUsuario). ") ";
        }
        if($filtro->unicod){
            if(is_array($filtro->unicod)){
                if( !empty(join_simec(',', $filtro->unicod))){
                    $where .= "\n AND suo.unocod::INTEGER IN(". join_simec(',', $filtro->unicod). ") ";
                }
            } else {
                $where .= "\n AND suo.unocod = '". (int)$filtro->unicod. "' ";
            }
        }
        if($filtro->unofundo){
            $where .= "\n AND suo.unofundo = ". $filtro->unofundo;
        }

        $sql = "
            SELECT
                suo.suoid,
                suo.unoid,
                suo.suocod AS codigo,
                suo.suocod || ' - ' || suonome AS descricao
            FROM public.vw_subunidadeorcamentaria suo
            WHERE
                suo.suostatus = 'A'
                AND suo.prsano::INTEGER = ". (int)$filtro->exercicio. "
                $where
            ORDER BY
                suo.suonome
        ";
        // ver($sql,d);
        return $sql;
    }

    /**
     * Monta consulta para retornar Sub-Unidades.
     *
     * @param stdClass $filtro
     * @return string
     */
    public static function queryComboChaveSuoid(stdClass $filtro)
    {
        $where = '';
        if($filtro->listaSubUnidadeUsuario && !empty(join_simec(',', $filtro->listaSubUnidadeUsuario))){
            $where .= "\n AND suo.suocod::INTEGER IN(". join_simec(',', $filtro->listaSubUnidadeUsuario). ") ";
        }
        if($filtro->unicod){
            if(is_array($filtro->unicod)){
                if( !empty(join_simec(',', $filtro->unicod))){
                    $where .= "\n AND suo.unocod::INTEGER IN(". join_simec(',', $filtro->unicod). ") ";
                }
            } else {
                $where .= "\n AND suo.unocod = '". (int)$filtro->unicod. "' ";
            }
        }
        if($filtro->unofundo){
            $where .= "\n AND suo.unofundo = ". $filtro->unofundo;
        }

        $sql = "
            SELECT
                suo.suoid AS codigo,
                suo.suocod || ' - ' || suonome AS descricao
            FROM public.vw_subunidadeorcamentaria suo
            WHERE
                suo.suostatus = 'A'
                AND suo.prsano::INTEGER = ". (int)$filtro->exercicio. "
                $where
            ORDER BY
                suo.suonome
        ";
        return $sql;
    }

    /**
     * Função que retorna a lista de Unidade vinculadas ao tipo de perfil.
     *
     * @param null $usucpf
     * @param null $exercicio
     * @return array
     */
    public function recuperarPorUsuario($usucpf = null, $exercicio = null)
    {
        $perfis = pegaPerfilGeral();
        $usucpf = $usucpf ? $usucpf : $_SESSION['usucpf'];
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        # Se o perfil for de Unidade.
        if (in_array(PFL_UNIDADE, $perfis)) {

            $sql = "SELECT
                        suo.suoid,
                        suo.suosigla,
                        suo.suonome,
                        suo.unoid,
                        suo.unosigla,
                        suo.unonome
                    FROM public.vw_subunidadeorcamentaria suo
                        JOIN acompanhamento.usuarioresponsabilidade usr ON usr.ungcod = suo.suocod
                    WHERE
                        suo.prsano = '$exercicio'
                        AND suo.suostatus = 'A'
                        AND suo.unofundo = 'f'
                        AND usr.usucpf = '$usucpf'
                        AND usr.rpustatus = 'A'
                    ORDER BY suo.unosigla
            ";
        } else {
            $sql = "SELECT
                        suo.suoid,
                        suo.suosigla,
                        suo.suonome,
                        suo.unoid,
                        suo.unosigla,
                        suo.unonome
                    FROM
                        public.vw_subunidadeorcamentaria suo
                    WHERE
                        suo.prsano = '$exercicio'
                        AND suo.suostatus = 'A'
                        AND suo.unofundo = 'f'
                    ORDER BY suo.unosigla
            ";
    }
//ver($sql,d);
        $dados = $this->carregar($sql);

        $aUnidades = [];
        foreach($dados as $dado){
            $aUnidades[$dado['suoid']] = "{$dado['unosigla']} - {$dado['suonome']} ";
        }
        return $aUnidades;
    }

    public function recuperarSqlCombo($descricao = null, $where = [], $order = '') {
        $pk = $this->arChavePrimaria[0];

        $descricao = $descricao ? $descricao : ['unosigla', 'suonome'];

        $where[] = "suostatus = 'A'";
        $where[] = "prsano = '{$_SESSION['exercicio']}'";
        
        if ($descricao) {
            $descricao = implode(" || ' - ' || ", (array)$descricao);
        }

        $order = $order ? $order : $descricao;
        $where = count($where) ? implode("\n AND ", $where) : '';

        $sql = "
            SELECT
                {$pk} AS codigo,
                {$descricao} AS descricao
            FROM public.vw_subunidadeorcamentaria
            WHERE
                $where
            ORDER BY
                $order
        ";

        return $sql;
    }
    
    /**
     * Retorna SQL da consulta pra preencher o componente de combo referente à Subunidade.
     * 
     * @param stdClass $filtro
     * @return string
     */
    public function recuperarSqlComboUnidade(stdClass $filtro) {
        $sql = "
            SELECT DISTINCT
                suo.suoid AS codigo,
                suo.unosigla || ' - ' || suo.suonome AS descricao
            FROM public.vw_subunidadeorcamentaria suo
            WHERE
                suo.prsano = '". (int)$filtro->exercicio. "'
                AND suo.suostatus = 'A'
            ORDER BY
                2
        ";
        return $sql;
    }
    
    public function montarSqlBuscarRp(stdClass $parametros){
//        $filtro[] = "suo.suostatus = 'A'";
//        $filtro[] = "ptr.irpcod <> '6'";
//        $where = $this->montarFiltros($filtro);

        $sql = "
            SELECT
                suo.suoid,
                suo.unoid,
                suo.unocod,
                suo.unosigla,
                suo.unonome,
                suo.suosigla,
                suo.suocod,
                suo.suonome,
                ptr.ptrano,
                ptr.funcional,
                ptr.acatitulo,
                ptr.irpcod,
                ptr.plodsc,
                ptr.ptrid,
                lmu.lmuvlr,
                lmu.lmuid,
                psu.psuid,
                psu.ptrdotacaocapital ptrdotacao_capital,
                psu.ptrdotacaocusteio ptrdotacao_custeio,
                sum(COALESCE(pic.picvalorcapital, 0)) picvalorcapital,
                sum(COALESCE(pic.picvalorcusteio, 0)) picvalorcusteio,
                (
                    SELECT
                        COUNT(1)
                    FROM spo.ptressubunidade
                    WHERE
                        ptressubunidade.ptrid = ptr.ptrid
                ) AS compartilhada
            FROM public.vw_subunidadeorcamentaria suo
                JOIN spo.ptressubunidade psu on psu.suoid = suo.suoid
                JOIN monitora.vw_ptres ptr on ptr.ptrid = psu.ptrid AND ptr.ptrano = suo.prsano
                LEFT JOIN monitora.pi_planointernoptres ppt on ppt.ptrid = ptr.ptrid
                LEFT JOIN monitora.pi_planointerno pli ON(
                    pli.pliid = ppt.pliid
                    AND pli.ungcod = suo.suocod
                    AND pli.unicod = suo.unocod
                    AND plistatus = 'A')
                LEFT JOIN planacomorc.pi_complemento pic on pic.pliid = pli.pliid
                LEFT JOIN planacomorc.unidadegestora_limite lmu on lmu.ungcod = suo.suocod AND lmu.lmustatus = 'A' AND lmu.prsano = suo.prsano
            WHERE
                {$where}
            GROUP BY
                suo.suoid,
                suo.unoid,
                suo.unocod,
                suo.unosigla,
                suo.unonome,
                suo.suocod,
                suo.suonome,
                suo.suosigla,
                ptr.ptrano,
                ptr.funcional,
                ptr.acatitulo,
                ptr.irpcod,
                ptr.plodsc,
                ptr.ptrid,
                lmu.lmuvlr,
                lmu.lmuid,
                psu.psuid,
                ptrdotacao_capital,
                ptrdotacao_custeio
            ORDER BY
                suo.unonome,
                suo.suonome,
                ptr.funcional
        ";

        return $sql;
    }
    
    /**
     * Busca lista de RP.
     * @example array( 'suo.unocod' => UNIDADEORCAMENTARIA_SUOCOD_FNC, 'suo.unocod != '. UNIDADEORCAMENTARIA_SUOCOD_FNC, 'suo.unocod' => array(UNIDADEORCAMENTARIA_SUOCOD_FNC, 42101)
     *
     * @param stdClass $parametros
     * @return array
     */
    public function buscarRp(stdClass $parametros)
    {
        $sql = $this->montarSqlBuscarRp($parametros);
//ver($sql, d);
        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];
        $dadosAgrupados = [];
        foreach ($dados as $dado) {
            $dadosAgrupados[$dado['suocod']]['detalhe'][] = $dado;
            $dadosAgrupados[$dado['suocod']]['limite'] += $dado['rp_limite'];
            $dadosAgrupados[$dado['suocod']]['totalPlanejado'] = ($dadosAgrupados[$dado['suocod']]['planejadoCapital'] + $dadosAgrupados[$dado['suocod']]['planejadoCusteio']);
        }

        return $dadosAgrupados;
    }
    

}
