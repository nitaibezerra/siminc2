<?php
/**
 * Arquivo com funções compartilhadas pelos sistemas SPO.
 * $Id: funcoesspo.php 98435 2015-06-10 13:50:26Z maykelbraz $
 */

/**
 * Arquivo com a definição dos componentes usados pela SPO.
 * @see funcoesspo_componentes.php
 */
require_once(dirname(__FILE__) . '/funcoesspo_componentes.php');

/**
 * Monta uma combo de unidades orçamentárias com o nome dados[unicod] e id unicod.
 * @param mixed $valor O valor do item que deve vir selecionado na combo.
 * @param array $whereAdicional Array com clausulas where adicionais para a query.
 * @param array $opcoesAdicionais Array com opções adicionais para a combo de seleção.
 */
function inputComboUnicod($valor, array $whereAdicional = array(), array $opcoesAdicionais = array())
{
    $query = "
SELECT uni.unicod AS codigo,
       uni.unicod || ' - ' || uni.unidsc AS descricao
  FROM public.unidade uni
  WHERE uni.unistatus = 'A'
    AND (uni.orgcod = '". CODIGO_ORGAO_SISTEMA. "' OR uni.unicod IN('74902', '73107'))
    %s
  ORDER BY uni.unicod
";

    // -- Se houver um $whereAdicional, o incluí na query
    $substituto = '';
    if (!empty($whereAdicional)) {
        $substituto = ' AND ' . implode(' AND ', $whereAdicional);
    }
    $stmt = sprintf($query, $substituto);

    inputCombo(
        'dados[unicod]',
        $stmt,
        $valor,
        'unicod',
        $opcoesAdicionais
    );
}

/**
 * Monta uma combo de tipo de recurso com o nome dados[trccod] e id trccod.
 * @param mixed $valor O valor do item que deve vir selecionado na combo.
 * @param array $whereAdicional Array com clausulas where adicionais para a query.
 * @param array $opcoesAdicionais Array com opções adicionais para a combo de seleção.
 */
function inputComboTipoRecurso($valor, array $whereAdicional = array(), array $opcoesAdicionais = array())
{

    $query = <<<DML
SELECT tprcod AS codigo,
       tprcod || ' - ' || tprdsc AS descricao
  FROM spo.tiporecurso
DML;

    // -- Se houver um $whereAdicional, o incluí na query
    $substituto = '';
    if (!empty($whereAdicional)) {
        $substituto = ' AND ' . implode(' AND ', $whereAdicional);
    }
    $stmt = sprintf($query, $substituto);

    inputCombo(
        'dados[stccod]',
        $stmt,
        $valor,
        'stccod',
        $opcoesAdicionais
    );
}

/**
 * Monta uma combo de grupo de natureza de despesas com o nome dados[ctgcod] e id ctgcod.
 * @param mixed $valor O valor do item que deve vir selecionado na combo.
 * @param array $whereAdicional Array com clausulas where adicionais para a query.
 * @param array $opcoesAdicionais Array com opções adicionais para a combo de seleção.
 */
function inputComboGND($valor, array $whereAdicional = array(), array $opcoesAdicionais = array())
{
    $query = <<<DML
        SELECT gndcod AS codigo, gndcod || ' - ' ||gnddsc AS descricao FROM spo.gruponaturezadespesa
DML;

    // -- Se houver um $whereAdicional, o incluí na query
    $substituto = '';
    if (!empty($whereAdicional)) {
        $substituto = ' AND ' . implode(' AND ', $whereAdicional);
    }
    $stmt = sprintf($query, $substituto);

    inputCombo(
        'dados[ctgcod]',
        $stmt,
        $valor,
        'ctgcod',
        $opcoesAdicionais
    );
}

/**
 * Monta uma combo de elemento de despesas com o nome dados[elmcod] e id elmcod.
 * @param mixed $valor O valor do item que deve vir selecionado na combo.
 * @param array $whereAdicional Array com clausulas where adicionais para a query.
 * @param array $opcoesAdicionais Array com opções adicionais para a combo de seleção.
 */
function inputComboElemento($valor, array $whereAdicional = array(), array $opcoesAdicionais = array())
{
    $query = <<<DML
        SELECT eldcod AS codigo, eldcod || ' - ' || elddsc AS descricao FROM spo.elementodespesa
DML;

    // -- Se houver um $whereAdicional, o incluí na query
    $substituto = '';
    if (!empty($whereAdicional)) {
        $substituto = ' AND ' . implode(' AND ', $whereAdicional);
    }
    $stmt = sprintf($query, $substituto);

    inputCombo(
        'dados[elmcod]',
        $stmt,
        $valor,
        'elmcod',
        $opcoesAdicionais
    );
}


/**
 * Monta uma combo de unidades gestoras com o nome dados[ungcod] e id ungcod.
 * @param mixed $valor O valor do item que deve vir selecionado na combo.
 * @param array $whereAdicional Array com clausulas where adicionais para a query.
 * @param array $opcoesAdicionais Array com opções adicionais para a combo de seleção.
 */
function inputComboUngcod($valor, array $whereAdicional = array(), array $opcoesAdicionais = array())
{
    $query = <<<DML
SELECT ung.ungcod AS codigo,
       ung.ungcod || ' - ' || ung.ungdsc AS descricao
  FROM public.unidadegestora ung
    WHERE ung.ungstatus= 'A'
      %s
  ORDER BY ung.unicod
DML;

    // -- Se houver um $whereAdicional, o incluí na query
    $substituto = '';
    if (!empty($whereAdicional)) {
        $substituto = ' AND ' . implode(' AND ', $whereAdicional);
    }
    $stmt = sprintf($query, $substituto);
    //ver($stmt);

    inputCombo(
        'dados[ungcod]',
        $stmt,
        $valor,
        'ungcod',
        $opcoesAdicionais
    );
}

/**
 * Monta uma combo de fontes detalhadas do siafi com o nome dados[fdsid] e id fdsid.
 * @param mixed $valor O valor do item que deve vir selecionado na combo.
 * @param array $whereAdicional Array com clausulas where adicionais para a query.
 * @param array $opcoesAdicionais Array com opções adicionais para a combo de seleção.
 */
function inputComboFonteDetalhada($valor, $ano, array $whereAdicional = array(), array $opcoesAdicionais = array())
{
    $query = <<<DML
SELECT fds.fdscod AS codigo,
       fds.fdscod || ' - ' || fds.fdsdsc AS descricao
  FROM spo.fontedetalhadasiafi fds
  WHERE fds.fdsano = '%d'
    %s
  ORDER BY fds.fdscod
DML;

    // -- Se houver um $whereAdicional, o inclui na query
    $substituto = '';
    if (!empty($whereAdicional)) {
        $substituto = ' AND ' . implode(' AND ', $whereAdicional);
    }
    $stmt = sprintf($query, $ano, $substituto);
    //ver($stmt);

    inputCombo(
        'dados[fdsid]',
        $stmt,
        $valor,
        'fdsid',
        $opcoesAdicionais
    );
}

/**
 * Monta uma combo de vonculação de pagamentos com o nome dados[vincod] e id vincod.
 * @param mixed $valor O valor do item que deve vir selecionado na combo.
 * @param array $whereAdicional Array com clausulas where adicionais para a query.
 * @param array $opcoesAdicionais Array com opções adicionais para a combo de seleção.
 */
function inputComboVinculacaoPagamento($valor, $ano, array $whereAdicional = array(), array $opcoesAdicionais = array())
{
    $query = <<<DML
    SELECT vp.vnpcod AS codigo, vp.vnpcod || ' - ' || vp.vnpdsc AS descricao
FROM spo.vinculacaopagamento vp
    WHERE vp.vnpano = '%d'
    %s
ORDER BY vp.vnpcod
DML;

    // -- Se houver um $whereAdicional, o inclui na query
    $substituto = '';
    if (!empty($whereAdicional)) {
        $substituto = ' AND ' . implode(' AND ', $whereAdicional);
    }
    $stmt = sprintf($query, $ano, $substituto);
    //ver($stmt, d);

    inputCombo(
        'dados[vincod]',
        $stmt,
        $valor,
        'vincod',
        $opcoesAdicionais
    );
}

/**
 * Verifica se tem dados na sessão para utilização dentro da funcionalidade.
 * Os dados retornados são usados em funções de processamento, ou para preencher formulários.
 *
 * Obs: Ao chamar esta função, sempre preceda a atribuição com o operador &.
 * $dados = &dadosDaSessao('gestao'); Com isso, mudanças feitas em $dados serão refletidas na sessão e
 * vice e versa.
 *
 * @param string $identificador Identifica uma funcionalidade do sistema, para agrupamento dos dados.
 * @return array
 */
function &dadosDaSessao($identificador)
{
    if (!isset($_SESSION[$_SESSION['sisdiretorio']])
        || !isset($_SESSION[$_SESSION['sisdiretorio']][$identificador])
        || !isset($_SESSION[$_SESSION['sisdiretorio']][$identificador]['dados'])) {
        $_SESSION[$_SESSION['sisdiretorio']][$identificador]['dados'] = array();
    }

    return $_SESSION[$_SESSION['sisdiretorio']][$identificador]['dados'];
}

/**
 * Formata um valor numérico no formato de reais, sem o R$.
 * @param mixed $valor Valor para ser formatado.
 * @return String
 */
function mascaraMoeda($valor, $align = true)
{
    $valor = number_format($valor, 2, ',', '.');
    if (false !== strpos($valor, '-')) {
        $valor = '<span style="color:red"><b>' . $valor . '</b></span>';
    }
    if ($align) {
        $valor = "<p style=\"text-align:right!important\">{$valor}</p>";
    }
    return $valor;
}

/**
 * Alinha o texto para a esquerda
 * @param mixed $valor Valor para ser formatado.
 * @return String
 */
function alinhaParaEsquerda($valor)
{
    $valor = "<p style=\"text-align: left !important;\">$valor</p>";
    return $valor;
}
function alinharEsquerda($valor)
{
    return alinhaParaEsquerda($valor);
}

function mascaraNumero($valor)
{
    $valor = number_format($valor, 0, ',', '.');
    if (false !== strpos($valor, '-')) {
        $valor = '<span style="color:red"><b>' . $valor . '</b></span>';
    }

    return $valor;
}

function removeMascaraMoeda($valor)
{
    return str_replace(array('.', ','), array('', '.'), $valor);
}

/**
 * Retorna uma consulta SQL deve ser utilizada em um cláusula WHERE com IN. Ao ser executada<br />
 * retorna uma lista com as responsábilidades cadastradas pelo usuário.
 *
 * @param string $usucpf Número do CPF do usuário no sistema.
 * @param int $perfil Número do perfil para consulta de responsábilidades.
 * @param string $camporesponsabilidade Nome do campo que armazena a responsabilidade em $schema.usuarioresponsabilidades.
 * @param string|null $schema Nome do schema da tabela usuarioresponsabilidade, usa o da sessão se estiver nulo.
 * @param bool $asArray Indica que os dados devem ser retornados, ao invés de retornar uma query.
 * @return string
 */
function pegaResposabilidade($usucpf, $perfil, $camporesponsabilidade, $schema = null, $asArray = false)
{
    if (is_null($schema)) {
        $schema = $_SESSION['sisdiretorio'];
    }

    $camporesponsabilidade = str_replace("'", '', $camporesponsabilidade);
    $schema = str_replace("'", '', $schema);

    $query = <<<DML
SELECT rpu.{$camporesponsabilidade}
  FROM {$schema}.usuarioresponsabilidade rpu
  WHERE rpu.usucpf = '%s'
    AND rpu.pflcod = %d
    AND rpu.rpustatus = 'A'
DML;
    $stmt = sprintf($query, $usucpf, $perfil);

    if ($asArray) {
        global $db;
        $dadosdb = $db->carregar($stmt);
        foreach ($dadosdb as &$resp) {
            $resp = current($resp);
        }

        return $dadosdb;
    }

    return $stmt;
}

function gravacaoDesabilitada($motivo = '', $tamanho = 6, $offset = 3, $alinhamento = 'center')
{
    $texto = '<span class="glyphicon glyphicon-exclamation-sign"></span> Gravação desabilitada.';
    if (!empty($motivo)) {
        $texto .= '<br /><b>Motivo</b>: ' . $motivo;
    }

    echo <<<HTML
<div class="alert alert-danger col-md-{$tamanho} col-md-offset-{$offset} text-{$alinhamento}" role="alert">$texto</div>
<br style="clear:both" />
HTML;
}

function incluirDocid($campo, $tabela, $valorid, $tpdoc, $descricao = '', $esquema = '')
{
    /**
     * Funções de gestão do workflow.
     * @see worflow.php
     */
    require_once(APPRAIZ . 'includes/workflow.php');

    global $db;

    if (empty($esquema)) {
        $esquema = $_SESSION['sisdiretorio'];
    }

    $sql = <<<DML
SELECT docid
  FROM {$esquema}.{$tabela}
  WHERE {$campo} = :{$campo}
DML;
    $dml = new Simec_DB_DML($sql);
    $dml->addParam($campo, $valorid);

    if ($docid = $db->pegaUm($dml)) {
        return $docid;
    }

    // -- Criando um novo docid
    $docid = wf_cadastrarDocumento($tpdoc, "{$descricao} - {$valorid}");

    // -- Atualizando o docid na tabela
    $sql = <<<DML
UPDATE {$esquema}.{$tabela}
  SET docid = :docid
  WHERE {$campo} = :{$campo}
DML;
    $dml->setString($sql)
        ->addParam('docid', $docid)
        ->addParam($campo, $valorid);

    $db->executar($dml);
    $db->commit();

    return $docid;
}

