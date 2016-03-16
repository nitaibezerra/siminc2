<?php
/**
 * $Id: CargaOrcamentaria.class.inc 82151 2014-07-01 12:56:52Z maykelbraz $
 */

  /**
   *
   */
  include_once APPRAIZ . 'planacomorc/classes/WSQualitativo.class.inc';
  include_once APPRAIZ . 'planacomorc/classes/SiopQualitativo.class.inc';
  include_once APPRAIZ . 'planacomorc/modulos/sistema/comunica/funcoesImportacao.inc';

/*
 * Dependências qualitativas de carca
 * 1-QC-Tabelas apoio) tipo inclusao
 * 1-QC-Tabelas apoio) macro desafio
 * 1-QC-Tabelas apoio) tipo programa
 * 1-QC-Tabelas apoio) funcao
 * 1-QC-Tabelas apoio) esfera
 * 1-QC-Tabelas apoio) produto
 * 1-QC-Tabelas apoio) unidade de medida
 * 1-QC-Tabelas apoio) tipo acao
 * 2-QC-Tabelas apoio) subfuncao <= funcao
 * 1-QC-Programação Completa) orgao <= orgão
 * 2-QC-Programação Completa) localizador <= tipo inclusao
 * 2-QC-Programação Completa) programa <= macro desafio / tipo programa / orgao
 * 2-QC-Programação Completa) objetivo <= orgao / programa
 * 3-QC-Programação Completa) iniciativa <= objetivo / orgao / programa
 * 4-QC-Programação Completa) acao <= iniciativa / esfera / subfuncao / tipo acao / objetivo
 * -------------------------------------------------------------
 * 5-QC-Programação Completa) plano orcamentario <= acao / unidade de medida
 * -------------------------------------------------------------
 * Q: Qualitativo
 * I: Carrega isolado
 * C: Carrega em conjunto
 * Tabelas apoio: WSQualitativo -> obterTabelas de Apoio
 * Programação Completa: WSQualitativo -> obterProgramacaoCompleta
 */
class CargaOrcamentaria {

    const TABELAS_APOIO = 1;
    const PROGRAMACAO_COMPLETA = 2;


    /**
     * Usuário e senha utilizado no sipo.
     * @var array
     */
    private $_paramsSiop;

    private $_obterTabelasApoio = array(
        'params' => array('exercicio' => null),
        'retorno' => array(
            'retornarTiposInclusao' => array(),
            'retornarMacroDesafios' => array(),
            'retornarTiposPrograma' => array(),
            'retornarFuncoes' => array(),
            'retornarEsferas' => array(),
            'retornarProdutos' => array(),
            'retornarUnidadesMedida' => array(),
            'retornarTiposAcao' => array(),
            'retornarSubFuncoes' => array()
            ));

    private $_obterProgramacaoCompleta = array(
        'params' => array('exercicio' => null),
        'retorno' => array(
            'retornarOrgaos' => array(),
            'retornarLocalizadores' => array(),
            'retornarProgramas' => array(),
            'retornarObjetivos' => array(),
            'retornarIniciativas' => array(),
            'retornarAcoes' => array(),
            'retornarPlanosOrcamentarios' => array()
        ));

    private $_dependencias = array(
        'obterTabelasApoioResponse' => array(
            'tiposInclusaoDTO' => array(
                'tabela' => 'planacomorc.tipo_inclusao',
                'unique' => array('codigoTipoInclusao' => 'id_tipo_inclusao'),
                'restricao' => array('snAtivo', '=', 1),
                'cols' => array('codigoTipoInclusao' => array('map' => 'id_tipo_inclusao', 'tipo' => 'int'),
                    'dataHoraAlteracao' => array('map' => 'instante_alteracao', 'tipo' => 'timestampwot'),
                    'descricao' => array('map' => 'descricao', 'tipo' => 'text'))),
            'macroDesafiosDTO' => array(
                'tabela' => 'planacomorc.macro_desafio',
                'unique' => array('codigoMacroDesafio' => 'id_macro_desafio'),
                'restricao' => null,
                'cols' => array('codigoMacroDesafio' => array('map' => 'id_macro_desafio', 'tipo' => 'int'),
                    'descricao' => array('map' => 'descricao', 'tipo' => 'text'),
                    'titulo' => array('map' => 'titulo', 'tipo' => 'text'))),
            'tiposProgramaDTO' => array(
                'tabela' => 'planacomorc.tipo_programa',
                'unique' => array('codigoTipoPrograma' => 'id_tipo_programa'),
                'restricao' => array('snAtivo', '=', 1),
                'cols' => array('codigoTipoPrograma' => array('map' => 'id_tipo_programa', 'tipo' => 'int'),
                    'descricao' => array('map' => 'descricao', 'tipo' => 'text'))),
            'funcoesDTO' => array(
                'tabela' => 'planacomorc.funcao',
                'unique' => array('codigoFuncao' => 'codigo'),
                'restricao' => array('snAtivo', '=', 1),
                'cols' => array('descricao' => array('map' => 'descricao', 'tipo' => 'text'),
                    'descricaoAbreviada' => array('map' => 'descricao_abreviada', 'tipo' => 'text'),
                    'dataHoraAlteracao' => array('map' => 'instante_alteracao', 'tipo' => 'timestampwot'),
                    'codigoFuncao' => array('map' => 'codigo', 'tipo' => 'text'))),
            'esferasDTO' => array(
                'tabela' => 'planacomorc.esfera',
                'unique' => array('codigoEsfera' => 'codigo'),
                'restricao' => array('snAtivo', '=', 1),
                'cols' => array('codigoEsfera' => array('map' => 'codigo', 'tipo' => 'text'),
                    'dataHoraAlteracao' => array('map' => 'instante_alteracao', 'tipo' => 'timestampwot'),
                    'descricao' => array('map' => 'descricao', 'tipo' => 'text'),
                    'descricaoAbreviada' => array('map' => 'descricao_abreviada', 'tipo' => 'text'))),
            'produtosDTO' => array(
                'tabela' => 'planacomorc.produto',
                'unique' => array('codigoProduto' => 'codigo'),
                'restricao' => array('snAtivo', '=', 1),
                'cols' => array('dataHoraAlteracao' => array('map' => 'instante_alteracao', 'tipo' => 'timestampwot'),
                    'descricao' => array('map' => 'descricao', 'tipo' => 'text'),
                    'codigoProduto' => array('map' => 'codigo', 'tipo' => 'text'))),
            'unidadesMedidaDTO' => array(
                'tabela' => 'planacomorc.unidade_medida',
                'unique' => array('codigoUnidadeMedida' => 'codigo'),
                'restricao' => array('snAtivo', '=', 1),
                'cols' => array(
                    'codigoUnidadeMedida' => array('map' => 'codigo', 'tipo' => 'text'),
                    'dataHoraAlteracao' => array('map' => 'instante_alteracao', 'tipo' => 'timestampwot'),
                    'descricao' => array('map' => 'descricao', 'tipo' => 'text'))),
            'tiposAcaoDTO' =>  array(
                'tabela' => 'planacomorc.tipo_acao',
                'unique' => array('codigoTipoAcao' => 'id_tipo_acao'),
                'restricao' => array('snAtivo', '=', 1),
                'cols' => array('codigoTipoAcao' => array('map' => 'id_tipo_acao', 'tipo' => 'int'),
                    'descricao' => array('map' => 'descricao', 'tipo' => 'text'))),
            'subFuncoesDTO' => array(
                'dependencia' => array('funcoesDTO' => 'id_funcao'),
                'tabela' => 'planacomorc.subfuncao',
                'unique' => array('codigoSubFuncao' => 'codigo'),
                'restricao' => array('snAtivo', '=', 1),
                'cols' => array('codigoSubFuncao' => array('map' => 'codigo', 'tipo' => 'text'),
                    //'codigoFuncao' => array('map' => 'funcao.codigo', 'tipo' => 'int'),
                    'dataHoraAlteracao' => array('map' => 'instante_alteracao', 'tipo' => 'timestampwot'),
                    'descricao' => array('map' => 'descricao', 'tipo' => 'text'),
                    'descricaoAbreviada' => array('map' => 'descricao_abreviada', 'tipo' => 'text')))),
        'obterProgramacaoCompletaResponse' => array(
            'orgaosDTO' => array(
                'dependencia' => array('orgaosDTO' => 'id_orgao'),
                'tabela' => 'planacomorc.orgao',
                'unique' => array('codigoOrgao' => 'codigo'),
                'restricao' => array('snAtivo', '=', 1),
                'cols' => array('codigoOrgao' => array('map' => 'codigo', 'tipo' => 'text'),
                    //'codigoOrgaoPai' => array('map' => '??', 'tipo' => 'int'),
                    'descricao' => array('map' => 'descricao', 'tipo' => 'text'),
                    'descricaoAbreviada' => array('map' => 'descricao_abreviada', 'tipo' => 'text'),
                    'orgaoId' => array('map' => 'siop_id', 'tipo' => 'int'),
                    'tipoOrgao' => array('map' => 'tipo', 'tipo' => 'text'))),
            'localizadoresDTO' => array(
                'dependencia' => array('tipoInclusaoDTO' => 'id_tipo_inclusao'),
                'tabela' => 'planacomorc.localizador',
                'unique' => array('codigoLocalizador' => 'codigo'),
                'cols' => array('anoInicio' => array('map' => 'ano_inicio', 'tipo' => 'int'),
                    'identificadorUnico' => array('map' => 'siop_id', 'tipo' => 'int'),
                    'anoTermino' => array('map' => 'ano_termino', 'tipo' => 'int'),
                    'codigoLocalizador' => array('map' => 'codigo', 'tipo' => 'text'),
                    'codigoTipoInclusao' => array('map' => 'id_tipo_inclusao', 'tipo' => 'int'),
                    'descricao' => array('map' => 'descricao', 'tipo' => 'text'),
                    'municipio' => array('map' => 'municipio', 'tipo' => 'text'),
                    'regiao' => array('map' => 'regiao', 'tipo' => 'text'),
                    'siglaRegiao' => array('map' => 'sigla_regiao', 'tipo' => 'text'),
                    'totalFinanceiro' => array('map' => 'total_financeiro', 'tipo' => 'int'),
                    'totalFisico' => array('map' => 'total_fisico', 'tipo' => 'int'),
                    'uf' => array('map' => 'uf', 'tipo' => 'text'))),
            'programasDTO' => array(
                'dependencia' => array(
                    'planacomorc.orgao' => array(
                        'ws' => 'codigoOrgao', 'db' => 'id_orgao', 'tipo' => 'text')),
                'tabela' => 'planacomorc.programa',
                'unique' => array('codigoPrograma' => 'codigo'),
                'cols' => array(
                    // -- 'codigoTipoPrograma' => array('map' => 'id_tipo_programa', 'tipo' => 'int'),
                    'codigoPrograma' => array('map' => 'codigo', 'tipo' => 'text'),
                    'estrategiaImplementacao' => array('map' => 'estrategia_implementacao', 'tipo' => 'text'),
                    'codigoMacroDesafio' => array('map' => 'id_macro_desafio', 'tipo' => 'int'),
                    'codigoOrgao' => array('map' => 'id_orgao', 'tipo' => 'sql',
                        'query' => "SELECT id_orgao FROM planacomorc.orgao WHERE codigo = '__VALOR__'"),
                    'horizonteTemporalContinuo' => array('map' => 'horizonte_temporal_continuo', 'tipo' => 'int'),
                    'identificadorUnico' => array('map' => 'siop_id', 'tipo' => 'int'),
                    'justificativa' => array('map' => 'justificativa', 'tipo' => 'text'),
                    'objetivo' => array('map' => 'objetivo', 'tipo' => 'text'),
                    'objetivoGoverno' => array('map' => 'objetivo_governo', 'tipo' => 'text'),
                    'objetivoSetorial' => array('map' => 'objetivo_setorial', 'tipo' => 'text'),
                    'problema' => array('map' => 'problema', 'tipo' => 'text'),
                    'publicoAlvo' => array('map' => 'publico_alvo', 'tipo' => 'text'),
                    'titulo' => array('map' => 'titulo', 'tipo' => 'text'),
                    'unidadeResponsavel' => array('map' => 'unidade_responsavel', 'tipo' => 'text'))),
            'objetivosDTO' => array(
                'dependencia' => array(
                    'planacomorc.orgao' => array(
                        'ws' => 'codigoOrgao', 'db' => 'id_orgao', 'tipo' => 'text'),
                    'planacomorc.programa' => array(
                        'ws' => 'codigoPrograma', 'db' => 'id_programa', 'tipo' => 'text')),
                'tabela' => 'planacomorc.objetivo',
                'unique' => array('codigoObjetivo' => 'codigo'),
                'cols' => array(
                    'codigoObjetivo' => array('map' => 'codigo', 'tipo' => 'text'),
                    'codigoOrgao' => array('map' => 'id_orgao', 'tipo' => 'sql',
                        'query' => "SELECT id_orgao FROM planacomorc.orgao WHERE codigo = '__VALOR__'"),
                    'codigoPrograma' => array('map' => 'id_programa', 'tipo' => 'sql',
                        'query' => "SELECT id_programa FROM planacomorc.programa WHERE codigo = '__VALOR__'"),
                    'enunciado' => array('map' => 'enunciado', 'tipo' => 'text'),
                    'identificadorUnico' => array('map' => 'siop_id', 'tipo' => 'int'))),
            'iniciativasDTO' => array(
                'dependencia' => array(
                    'planacomorc.orgao' => array(
                        'ws' => 'codigoOrgao', 'db' => 'id_orgao', 'tipo' => 'text'),
                    'planacomorc.objetivo' => array(
                        'ws' => 'codigoObjetivo', 'db' => 'id_objetivo', 'tipo' => 'text'),
                    'planacomorc.programa' => array(
                        'ws' => 'codigoPrograma', 'db' => 'id_programa', 'tipo' => 'text')),
                'tabela' => 'planacomorc.iniciativa',
                'unique' => array('codigoIniciativa' => 'codigo'),
                'cols' => array(
                    'codigoIniciativa' => array('map' => 'codigo', 'tipo' => 'text'),
                    'codigoObjetivo' => array('map' => 'id_objetivo', 'tipo' => 'sql',
                        'query' => "SELECT id_objetivo FROM planacomorc.objetivo WHERE codigo = '__VALOR__'"),
                    'codigoOrgao' => array('map' => 'id_orgao', 'tipo' => 'sql',
                        'query' => "SELECT id_orgao FROM planacomorc.orgao WHERE codigo = '__VALOR__'"),
                    'codigoPrograma' => array('map' => 'id_programa', 'tipo' => 'sql',
                        'query' => "SELECT id_programa FROM planacomorc.programa WHERE codigo = '__VALOR__'"),
                    'identificadorUnico' => array('map' => 'siop_id', 'tipo' => 'int'),
                    'titulo' => array('map' => 'titulo', 'tipo' => 'text'),
                    'snIndividualizada' => array('map' => 'individualizada', 'tipo' => 'text'))),
            'acoesDTO' => array(
                'dependencia' => array(
                    'planacomorc.iniciativa' => array(
                        'ws' => 'codigoIniciativa', 'db' => 'id_iniciativa', 'tipo' => 'text'),
                    'planacomorc.esfera' => array(
                        'ws' => 'codigoEsfera', 'db' => 'id_esfera', 'tipo' => 'text'),
                    'planacomorc.subfuncoes' => array(
                        'ws' => 'codigoSubFuncao', 'db' => 'id_subfuncao', 'tipo' => 'text'),
                    'planacomorc.objetivo' => array(
                        'ws' => 'codigoObjetivo', 'db' => 'id_objetivo', 'tipo' => 'text')),
                'tabela' => 'planacomorc.acao',
                'unique' => array('codigoAcao' => 'codigo'),
                'cols' => array(
                    'codigoAcao' => array('map' => 'codigo', 'tipo' => 'text'),
                    'codigoPrograma' => array('map' => 'id_programa', 'tipo' => 'sql',
                        'query' => "SELECT id_programa FROM planacomorc.programa WHERE codigo = '__VALOR__'"),
                    'codigoEsfera' => array('map' => 'id_esfera', 'tipo' => 'sql',
                        'query' => "SELECT id_esfera FROM planacomorc.esfera WHERE codigo = '__VALOR__'"),
                    'codigoSubFuncao' => array('map' => 'id_subfuncao', 'tipo' => 'sql',
                        'query' => "SELECT id_subfuncao FROM planacomorc.subfuncao WHERE codigo = '__VALOR__'"),
                    'codigoTipoAcao' => array('map' => 'id_tipo_acao', 'tipo' => 'int'),
                    'codigoObjetivo' => array('map' => 'id_objetivo', 'tipo' => 'sql',
                        'query' => "SELECT id_objetivo FROM planacomorc.objetivo WHERE codigo = '__VALOR__'"),
                    'codigoIniciativa' => array('map' => 'id_iniciativa', 'tipo' => 'sql',
                        'query' => "SELECT id_iniciativa FROM planacomorc.iniciativa WHERE codigo = '__VALOR__'"))),
            // -- As chamadas abaixo são casos especiais e são executadas por suas próprias funções, estão inclusas aqui
            // -- apenas para garantir a ordem de execução, pois devem ser executados antes dos planos orçamentários
            'dados_acao_exercicio' => 'especial',
            'acao_programatica' => 'especial',
            // -- Fim das chamadas especiais
            'planosOrcamentariosDTO' => array(
                'dependencia' => array(
                    'planacomorc.acao_programatica' => array(
                        'ws' => array('codigoAcao', 'exercicio'), 'db' => 'id_acao', 'tipo' => 'text'),
                    'planacomorc.unidade_medida' => array(
                        'ws' => 'codigoUnidadeMedida', 'db' => 'id_unidade_medida', 'tipo' => 'text')),
                'tabela' => 'planacomorc.plano_orcamentario',
                'unique' => array('identificadorUnico' => 'siop_id'),
                'cols' => array(
                    'planoOrcamentario' => array('map' => 'codigo', 'tipo' => 'text'),
                    'identificadorUnico' => array('map' => 'siop_id', 'tipo' => 'int'),
                    'titulo' => array('map' => 'titulo', 'tipo' => 'text'),
                    'detalhamento' => array('map' => 'detalhamento', 'tipo' => 'text'),
                    'codigoProduto' => array('map' => 'id_produto', 'tipo' => 'sql',
                        'query' => "SELECT id_produto FROM planacomorc.produto WHERE codigo = '__VALOR__'"),
                    'codigoUnidadeMedida' => array('map' => 'id_unidade_medida', 'tipo' => 'sql',
                        'query' => "SELECT id_unidade_medida FROM planacomorc.unidade_medida WHERE codigo = '__VALOR__'"),
                    'identificadorUnicoAcao' => array('map' => 'id_acao_programatica', 'tipo' => 'sql',
                        'query' =>
                        'SELECT id_acao_programatica FROM planacomorc.acao_programatica WHERE siop_id = __VALOR__ AND id_exercicio = __EXERCICIO__')))));

    public function __construct($paramsAuth) {
        $this->_paramsSiop = $paramsAuth;
    }

    /**
     * Retorna os parametros de envio ao webservice para definir o retorno dos dados.
     * @param string $qual tabelasApoio || programacaoCompleta
     * @return type
     */
    private function _getConfig($qual) {
        switch ($qual) {
            case self::TABELAS_APOIO:
                return array_fill_keys(
                        array_keys($this->_obterTabelasApoio['retorno']), true);
            case self::PROGRAMACAO_COMPLETA:
                return array_fill_keys(
                        array_keys($this->_obterProgramacaoCompleta['retorno']), true);
        }
    }

    public function obterTabelasDeApoio($documento) {
        if ('obterCargaOrcamentaria' == $documento) {
            $siopQualitativo = new SiopQualitativo(
                    array_merge($this->_paramsSiop, array('post' => $this->_getConfig(self::TABELAS_APOIO))));
            $retornoDTO = $siopQualitativo->obterTabelasApoio();
            $this->_processaRetornoDTO($retornoDTO);
        } else {
            $siopQualitativo = new SiopQualitativo(
                    array_merge($this->_paramsSiop, array('post' => $this->_getConfig(self::PROGRAMACAO_COMPLETA))));
            $retornoDTO = $siopQualitativo->obterProgramacaoCompleta();
            $this->_processaRetornoDTO($retornoDTO);
//            // -- Processamento da ação programática
//            $this->_upsertAcaoProgramatica($retornoDTO->return->acoesDTO);
//            // -- Processamento dos dados temporais da ação
//            $this->_upsertDadosAcaoExercicio($retornoDTO->return->acoesDTO);
        }
    }

    private function _processaRetornoDTO($retornoDTO) {// ver($retornoDTO, d);
        foreach ($this->_dependencias[get_class($retornoDTO)] as $DTOName => $DTOConfig) {
            switch ($DTOName) {
                case 'dados_acao_exercicio':
                    // -- Processamento da ação programática
                    $this->_upsertAcaoProgramatica($retornoDTO->return->acoesDTO);
                    break;
                case 'acao_programatica':
                    // -- Processamento dos dados temporais da ação
                    $this->_upsertDadosAcaoExercicio($retornoDTO->return->acoesDTO);
                    break;
                default:
                    if (!empty($retornoDTO->return->$DTOName)) {
                        $this->_upsert($retornoDTO->return->$DTOName, $DTOConfig);
                    }
            }

            // -- Reseta o timeout do apache
            set_time_limit(300000);
        }
    }

    private function _upsert($dadosDTO, $configDTO) { //ver($dadosDTO, d);
        global $db;

        $dadosArmazenados = $this->_carregaDoBanco($configDTO);
        $colsConfig = $configDTO['cols'];
        $unique = $configDTO['unique'];
        $pk = array_shift(array_keys($unique));
        $pkt = current($unique);
        $codigosInseridos = array();
        foreach ($dadosDTO as $dadoDTO) {
            // -- Validando se existe restrição de importação. Ex: snativo = 1
            $pularRegistro = false;
            if (isset($configDTO['restricao'])) {
                switch ($configDTO['restricao'][1]) {
                    default:
                        if ($configDTO['restricao'][2] != $dadoDTO->$configDTO['restricao'][0]) {
                            $pularRegistro = true;
                        }
                }
            }
            // -- Validando repetições
            if (in_array($dadoDTO->$pk, $codigosInseridos)) {
                $pularRegistro = true;
            }

            if ($pularRegistro) { continue; }

            // -- Inserindo ou atualizando o registro
            $sql = '';
            if (empty($dadosArmazenados)
                    // -- O código não estava presente na busca inicial na base
                    || !in_array(array($pkt => $dadoDTO->$pk), $dadosArmazenados)) {
                $campos = $valores = array();
                foreach ($colsConfig as $key => $config) {
                    $campos[] = $config['map'];
                    switch ($config['tipo']) {
                        case 'text':
                            $dadoDTO->$key = str_replace("'", "''", $dadoDTO->$key);
                            $valores[] = "'{$dadoDTO->$key}'"; break;
                        case 'timestampwot':
                            if (empty($dadoDTO->$key)) { $valores[] = "null";
                            } else { $valores[] = "'{$dadoDTO->$key}'"; }
                            break;
                        case 'sql':
                            if (!is_null($dadoDTO->$key)) {
                                $valores[] = '(' . str_replace(
                                        array('__VALOR__', '__EXERCICIO__'),
                                        array($dadoDTO->$key, $_SESSION['exercicio']), $config['query']) . ')';
                            } else {
                                $valores[] = 'NULL';
                            }
                            break;
                        default:
                            if (!is_null($dadoDTO->$key)) { $valores[] = $dadoDTO->$key;
                            } else { $valores[] = 'NULL'; }
                    }
                }
                $sql = "INSERT INTO {$configDTO['tabela']}(" . implode(', ', $campos) . ') VALUES (' . implode(', ', $valores) . ')';
                $codigosInseridos[] = $dadoDTO->$pk;
            } else {
                $campoValor = array();
                foreach ($colsConfig as $key => $config) {
                    if ($pk != $key) { // -- ignorando a atualização do campo de chave primária
                        switch ($config['tipo']) {
                            case 'text':
                                $dadoDTO->$key = str_replace("'", "''", $dadoDTO->$key);
                                $campoValor[] = "{$config['map']} = '{$dadoDTO->$key}'"; break;
                            case 'timestampwot':
                                if (empty($dadoDTO->$key)) { $valores[] = "null";
                                } else { $valores[] = "'{$dadoDTO->$key}'"; }
                                break;
                            case 'sql':
                                if (!is_null($dadoDTO->$key)) {
                                    $campoValor[] = "{$config['map']} = (" . str_replace(
                                        array('__VALOR__', '__EXERCICIO__'),
                                        array($dadoDTO->$key, $_SESSION['exercicio']), $config['query']) . ')';
                                } else {
                                    $campoValor[] = "{$config['map']} = NULL";
                                }
                                break;
                            default:
                                if (!is_null($dadoDTO->$key)) { $campoValor[] = "{$config['map']} = {$dadoDTO->$key}";
                                } else { $campoValor[] = "{$config['map']} = NULL"; }
                        }
                    }
                }
                $sql = "UPDATE {$configDTO['tabela']} SET " . implode(', ', $campoValor) . " WHERE {$unique[$pk]} = '{$dadoDTO->$pk}'";
            }
            $db->executar($sql);
        }
        $db->commit();
        if ('planacomorc.plano_orcamentario' == $configDTO['tabela'] || 'planacomorc.acao_programatica' == $configDTO['tabela']) {
            //exit('---');
        }
    }

    private function _upsertAcaoProgramatica($acoesDTO) {
        global $db;

        $sql = <<<DML
SELECT aca.codigo AS "codigoAcao",
       exe.exercicio,
       org.codigo AS "codigoOrgao"
  FROM planacomorc.acao_programatica apr
    INNER JOIN planacomorc.acao aca USING(id_acao)
    INNER JOIN planacomorc.exercicio exe USING(id_exercicio)
    LEFT JOIN planacomorc.orgao org USING(id_orgao)
DML;
        $dadosArmazenados = $db->carregar($sql);
        // -- Unique: acao - orgao - exercicio
        foreach ($acoesDTO as $acaoDTO) {
            if (empty($dadosArmazenados) || !in_array(
                    array('codigoAcao' => $acaoDTO->codigoAcao,
                        'exercicio' => (string)$acaoDTO->exercicio,
                        'codigoOrgao' => $acaoDTO->codigoOrgao), $dadosArmazenados)) {
                $sql = <<<DML
INSERT INTO planacomorc.acao_programatica(id_acao, id_exercicio, id_orgao, siop_id)
  VALUES ((SELECT id_acao FROM planacomorc.acao WHERE codigo = '{$acaoDTO->codigoAcao}'),
          {$acaoDTO->exercicio},
          (SELECT id_orgao FROM planacomorc.orgao WHERE codigo = '{$acaoDTO->codigoOrgao}' LIMIT 1),
          {$acaoDTO->identificadorUnico})
DML;
                $db->executar($sql);
            }
        }
        $db->commit();
    }

    private function _upsertDadosAcaoExercicio($acoesDTO) {
        global $db;

        $sql = <<<DML
SELECT aca.codigo AS "codigoAcao",
       exe.exercicio
  FROM planacomorc.dados_acao_exercicio dae
    INNER JOIN planacomorc.acao aca USING(id_acao)
    INNER JOIN planacomorc.exercicio exe USING(id_exercicio)
DML;
        $dadosArmazenados = $db->carregar($sql);
        // -- Unique: acao - exercicio
        $codigosInseridos = array(); $i = 1;
        foreach ($acoesDTO as $acaoDTO) {
            if (empty($dadosArmazenados) || !in_array(
                    array('codigoAcao' => (string)$acaoDTO->codigoAcao,
                        'exercicio' => (string)$acaoDTO->exercicio), $dadosArmazenados)) {

                if (!in_array((string)$acaoDTO->codigoAcao . (string)$acaoDTO->exercicio, $codigosInseridos)) {

                    $codigosInseridos[] = (string)$acaoDTO->codigoAcao . (string)$acaoDTO->exercicio;
                    if (empty($acaoDTO->codigoAcao)) { $acaoDTO->codigoAcao = 'NULL';
                    } else { $acaoDTO->codigoAcao = "'{$acaoDTO->codigoAcao}'"; }
                    if (empty($acaoDTO->codigoProduto)) { $acaoDTO->codigoProduto = 'NULL';
                    } else { $acaoDTO->codigoProduto = "'{$acaoDTO->codigoProduto}'"; }
                    if (empty($acaoDTO->codigoUnidadeMedida)) { $acaoDTO->codigoUnidadeMedida = 'NULL';
                    } else { $acaoDTO->codigoUnidadeMedida = "'{$acaoDTO->codigoUnidadeMedida}'"; }

                    $acaoDTO->titulo = str_replace("'", "''", $acaoDTO->titulo);
                    $acaoDTO->descricao = str_replace("'", "''", $acaoDTO->descricao);
                    $acaoDTO->especificacaoProduto = str_replace("'", "''", $acaoDTO->especificacaoProduto);
                    $acaoDTO->finalidade = str_replace("'", "''", $acaoDTO->finalidade);
                    $acaoDTO->detalhamentoImplementacao = str_replace("'", "''", $acaoDTO->detalhamentoImplementacao);

                    $sql = <<<DML
INSERT INTO planacomorc.dados_acao_exercicio(id_acao, id_exercicio, id_produto, id_unidade_medida,
    titulo, descricao, detalhamento_implementacao, especificacao_produto, finalidade)
  VALUES((SELECT id_acao FROM planacomorc.acao WHERE codigo = {$acaoDTO->codigoAcao}),
         (SELECT id_exercicio FROM planacomorc.exercicio WHERE exercicio = {$acaoDTO->exercicio}),
         (SELECT id_produto FROM planacomorc.produto WHERE codigo = {$acaoDTO->codigoProduto}),
         (SELECT id_unidade_medida FROM planacomorc.unidade_medida WHERE codigo = {$acaoDTO->codigoUnidadeMedida}),
         '{$acaoDTO->titulo}', '{$acaoDTO->descricao}', '{$acaoDTO->detalhamentoImplementacao}',
         '{$acaoDTO->especificacaoProduto}', '{$acaoDTO->finalidade}')
DML;
//                    ver($sql, d);
                    $db->executar($sql);
                }
            }
        }
        $db->commit();
    }

    private function _carregaDoBanco($configTabela) {
        global $db;

        $campo = implode(', ', $configTabela['unique']);
        $sql = "SELECT {$campo} FROM {$configTabela['tabela']}";
        return $db->carregar($sql);
    }
}

?>


















