<?php
/**
 * Sistema Simec
 * Setor responsável: SPO-MEC
 * Desenvolvedor: Equipe Consultores Simec
 * Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
 * Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima Barbosa (e-mail: renandelima@gmail.com)
 * Módulo: matriz.inc
 * Finalidade: Manutenção na tabela matriz
 */
/**
 * Classe de tratamento de queries.
 * @see DML
 */
require_once(APPRAIZ . 'includes/library/simec/DB/DML.php');

/**
 * Atualiza a relação da tabela matriz com a tabela fonterecurso.
 * Os códigos de fontes que não estão presentes no segundo
 * parâmetro são removidos da relação, exceto se a fonte não
 * mencionada for uma unidade fixa (ver documentação de
 * pegar_fontes_fixas()).
 *
 * @param integer $mtrid
 * @param integer $foncods
 */
function atualiza_fonterecursomatriz($mtrid, $foncods) {
    global $db;
    $mtrid = (integer) $mtrid;
    if (!$mtrid) {
        return;
    }
    $sql_remocao = "delete from elabrev.fonterecursomatriz where mtrid = " . $mtrid;
    $db->executar($sql_remocao);
    if ($foncods[0] == '') {
        $foncods = array();
    }
    // captura fontes fixas (olhar documentação de pegar_fontes_fixas())
    $foncods_fixos = array_keys(pegar_fontes_fixas($mtrid));
    foreach ($foncods_fixos as $foncod_fixo) {
        if (!in_array($foncod_fixo, $foncods)) {
            array_push($foncods, $foncod_fixo);
        }
    }
    // FIM captura fontes fixas
    foreach ((array) $foncods as $foncod) {
        $sql_insercao = "insert into elabrev.fonterecursomatriz (mtrid, foncod) values (" . $mtrid . ", '" . $foncod . "')";
        $db->executar($sql_insercao);
    }
}

/**
 * Atualiza a relação da tabela matriz com a tabela ação.
 * Os códigos de fontes que não estão presentes no segundo
 * parâmetro são removidos da relação. Caso a coluna tenha
 * registros relacionados à ela na tabela despesaacao,
 * nenhuma ação é realizada.
 *
 * @param integer $mtrid
 * @param integer $acacods
 */
function atualiza_acaomatriz($mtrid, $acacods) {
    global $db;
    $mtrid = (integer) $mtrid;
    if (!$mtrid) {
        return;
    }

    $sql_remocao = "delete from elabrev.acaomatriz where mtrid = " . $mtrid;
    $db->executar($sql_remocao);

    if ($acacods[0] == '') {
        $acacods = array();
    }
    foreach ((array) $acacods as $acacod) {
        $sql_insercao = "insert into elabrev.acaomatriz (mtrid, acacod) values (" . $mtrid . ", '" . $acacod . "')";
        $db->executar($sql_insercao);
    }
}

/**
 * Atualiza a lista de planos orçamentários associados a uma coluna.
 *
 * @param integer $mtrid ID da coluna à qual serão associados os POs.
 * @param array $planosorcamentarios Lista de planos orçamentários que serão associados à coluna.
 */
function atualiza_planosorcamentarios($mtrid, $planosorcamentarios) {
    global $db;

    // -- Validando o mtrid
    if (!$mtrid) {
        return;
    }

    // -- Removendo as associações anteriores
    $dmlString = <<<DML
DELETE FROM proporc.matrizplanoorcamentario
   WHERE mtrid = :mtrid
DML;
    $dml = new Simec_DB_DML($dmlString);
    $dml->addParam('mtrid', $mtrid);
    $db->executar($dml);

    if (!is_array($planosorcamentarios) || $planosorcamentarios[0] == '') {
        $planosorcamentarios = array();
    }

    // -- Incluíndo os novos dados do PO à associação com a coluna
    $dmlString = <<<DML
INSERT INTO proporc.matrizplanoorcamentario(mtrid, unicod, prgcod, acacod, loccod, plocod)
SELECT :mtrid, pao.unicod, pao.prgcod, pao.acacod, pao.loccod, plo.plocodigo
  FROM elabrev.planoorcamentario plo
    INNER JOIN elabrev.ppaacao_orcamento pao USING(acaid)
  WHERE ploid = :ploid
DML;
    $dml->setString($dmlString)
            ->addParam('mtrid', $mtrid);
    foreach ($planosorcamentarios as $po) {
        $dml->addParam('ploid', $po);
        $db->executar($dml);
    }
}

/**
 * Atualiza a relação da tabela matriz com a tabela GND. Os
 * códigos GND que não estão presentes no segundo parâmetro
 * são removidos da relação. Caso a coluna tenha
 * registros relacionados à ela na tabela despesaacao,
 * nenhuma ação é realizada.
 *
 * @param integer $mtrid
 * @param integer $gndcods
 */
function atualiza_matrizgnd($mtrid, $gndcods) {
    global $db;
    $mtrid = (integer) $mtrid;
    if (!$mtrid) {
        return;
    }
    $sql_remocao = "delete from elabrev.matrizgnd where mtrid = " . $mtrid;
    $db->executar($sql_remocao);

    if ($gndcods[0] == '') {
        $gndcods = array();
    }
    foreach ((array) $gndcods as $gndcod) {
        $sql_insercao = "insert into elabrev.matrizgnd (mtrid, gndcod) values (" . $mtrid . ", '" . (integer) $gndcod . "')";
        $db->executar($sql_insercao);
    }
}

/**
 * Atualiza a relação da tabela matriz com a tabela unidade.
 * Os códigos de unidades que não estão presentes no segundo
 * parâmetro são removidos da relação, exceto se a unidade
 * não mencionada for uma unidade fixa (ver documentação de
 * pegar_unidades_fixas()).
 *
 * @param integer $mtrid
 * @param integer $uniids
 */
function atualiza_unidadematriz($mtrid, $uniids) {
    global $db;
    $mtrid = (integer) $mtrid;
    if (!$mtrid) {
        return;
    }
    if ($uniids[0] == '') {
        $uniids = array();
    }
    $sql_remocao = "delete from elabrev.unidadematriz where mtrid = " . $mtrid;
    $db->executar($sql_remocao);
    // captura unidades fixas (olhar documentação de pegar_unidades_fixas())
    $uniids_fixos = array_keys(pegar_unidades_fixas($mtrid));
    foreach ($uniids_fixos as $uniid_fixo) {
        if (!in_array($uniid_fixo, $uniids)) {
            array_push($uniids, $uniid_fixo);
        }
    }
    // FIM captura unidades fixas
    foreach ((array) $uniids as $uniid) {
        // o campo uniid não é chave primária da tabela unidade
        // por conta disso a chave primária é recuperada da tabela utilizando esse uniid, que é um campo único
        $sql_unidade = "select unicod, unitpocod from unidade where uniid = " . (integer) $uniid . " limit 1";
        $dados_unidade = $db->recuperar($sql_unidade);
        $unicod = addslashes($dados_unidade['unicod']);
        $unitpocod = addslashes($dados_unidade['unitpocod']);
        // realiza insercao da relacao
        $sql_insercao = "insert into elabrev.unidadematriz (mtrid, unicod, unitpocod) values (" . $mtrid . ", '" . $unicod . "', '" . $unitpocod . "')";
        $db->executar($sql_insercao);
    }
}

/**
 * Altera um registro na tabela elabrev.matriz
 *
 * O parâmtro $mtrid é o identificador utilizado da tabela.
 *
 * @param integer $mtrid
 * @param string $mtrdsc
 * @param string $mtrsigla
 * @param string $mtrano
 * @param integer $gpmid
 * @param boolean $gpmatriz
 * @return boolean
 */
function atualiza_matriz($mtrid, $mtrdsc, $mtrsigla, $mtrano, $gpmid, $mtrmatriz) {
    global $db;
    $mtrid = (integer) $mtrid;
    if (!$mtrid) {
        return;
    }
    $mtrmatriz = $mtrmatriz ? 'true' : 'false';
    $sql = "update elabrev.matriz set mtrdsc = '" . $mtrdsc . "', mtrsigla = '" . $mtrsigla . "', mtrano = '" . $mtrano . "', gpmid = " . (integer) $gpmid . ", mtrmatriz = " . $mtrmatriz . " where mtrid = " . $mtrid;
    return (boolean) $db->executar($sql);
}

/**
 * Insere um registro na tabela elabrev.matriz.
 *
 * @param string $mtrdsc
 * @param string $mtrsigla
 * @param string $mtrano
 * @param integer $gpmid
 * @param boolean $gpmatriz
 * @return integer
 */
function insere_matriz($mtrdsc, $mtrsigla, $mtrano, $gpmid, $mtrmatriz) {
    global $db;
    $mtrmatriz = $mtrmatriz ? 'true' : 'false';
    $sql = "insert into elabrev.matriz (mtrdsc, mtrsigla, mtrano, gpmid, mtrmatriz) values ('" . $mtrdsc . "', '" . $mtrsigla . "', '" . $mtrano . "', " . (integer) $gpmid . ", " . $mtrmatriz . ")";
    if (!$db->executar($sql)) {
        return 0;
    }
    return $db->pegaUm("select currval('elabrev.matriz_mtrid_seq'::regclass) as mtrid");
}

/**
 * Verifica se a matriz possui algum registro na tabela
 * despecaacao relacionada com ela.
 *
 * @param integer $mtrid
 * @return boolean
 */
function matriz_possui_despesa($mtrid) {
    global $db;
    $mtrid = (integer) $mtrid;
    $sql_auxiliar = "select count(*) from elabrev.despesaacao join elabrev.ppaacao_orcamento using (acaid) join elabrev.acaomatriz using (acacod) where acaomatriz.mtrid = " . $mtrid;
    return $db->pegaUm($sql_auxiliar);
}

/**
 * Consulta todos os POs associados a uma matriz/coluna.
 *
 * @param integer $mtrid
 * @return boolean|array
 */
function matriz_possui_planosorcamentarios($mtrid) {
    global $db;
    $dmlString = <<<DML
SELECT plo.ploid,
       mpo.unicod || '.' || mpo.prgcod  || '.' || mpo.acacod  || '.' ||  mpo.loccod AS programatica,
       plocodigo || '-' || plotitulo AS planoorcamentario,
       mpo.unicod
  FROM proporc.matrizplanoorcamentario mpo
    INNER JOIN elabrev.matriz mtr USING(mtrid)
    INNER JOIN elabrev.planoorcamentario plo
      ON (mpo.plocod = plo.plocodigo)
    INNER JOIN elabrev.ppaacao_orcamento pao
      ON (plo.acaid = pao.acaid
          AND (mtr.mtrano::int - 1)::varchar = pao.prgano
          AND mpo.unicod = pao.unicod
          AND mpo.prgcod = pao.prgcod
          AND mpo.acacod = pao.acacod
          AND mpo.loccod = pao.loccod)
  WHERE mtr.mtrid = :mtrid
DML;
    $dml = new Simec_DB_DML($dmlString);
    $dml->addParam('mtrid', (int) $mtrid);
    return $db->carregar($dml);
}

/**
 * Captura as fontes relacionadas à uma matriz que tiverem
 * valores cadastrados na tabela limitefonte. O array retornado
 * os índices são os foncod e os valores a descrição.
 *
 * @param integer $mtrid
 * @return string[]
 */
function pegar_fontes_fixas($mtrid) {
    global $db;
    $mtrid = (integer) $mtrid;
    if (!$mtrid) {
        return array();
    }
    $sql = "select fonterecurso.foncod as codigo, fonterecurso.foncod || ' - ' || fonterecurso.fondsc as descricao from elabrev.limitefonte join elabrev.montantematriz using (mtmid) join public.fonterecurso using (foncod) where mtrid = " . $mtrid . " group by fonterecurso.foncod, fonterecurso.fondsc";
    $fontes = $db->carregar($sql);
    $fontes_fixas = array();
    if ($fontes) {
        foreach ($fontes as $descricao) {
            $fontes_fixas[$descricao['codigo']] = $descricao['descricao'];
        }
    }
    return $fontes_fixas;
}

/**
 * Captura as unidades relacionadas à uma matriz que tiverem
 * valores cadastrados na tabela limitefonte. O array retornado
 * os índices são os uniid e os valores a descrição.
 *
 * @param integer $mtrid
 * @return string[]
 */
function pegar_unidades_fixas($mtrid) {
    global $db;
    $mtrid = (integer) $mtrid;
    if (!$mtrid) {
        return array();
    }
    $sql = "select unidade.uniid, unidade.unicod || ' - ' || unidade.unidsc as descricao from elabrev.limitefonte join elabrev.montantematriz using (mtmid) join unidade using (unicod,unitpocod) where mtrid = " . $mtrid . " group by unidade.uniid, unidade.unicod, unidsc";
    $unidades = $db->carregar($sql);
    $unidades_fixas = array();
    if ($unidades) {
        foreach ($unidades as $descricao) {
            $unidades_fixas[$descricao['uniid']] = $descricao['descricao'];
        }
    }
    return $unidades_fixas;
}

/**
 * Remove um registro da tabela matriz.
 *
 * Os relacionamentos (fonterecurso, gnd e unidade) também são removidos.
 *
 * @param unknown_type $mtrid
 * @return unknown
 */
function remover_matriz($mtrid) {
    global $db;
    $mtrid = (integer) $mtrid;
    atualiza_fonterecursomatriz($mtrid, array());
    atualiza_acaomatriz($mtrid, array());
    atualiza_matrizgnd($mtrid, array());
    atualiza_unidadematriz($mtrid, array());
    atualiza_subacao($mtrid, array());
    #subacao
    $sql = "delete from elabrev.matrizsubacao where mtrid = " . $mtrid;
    $db->executar($sql);
    #gnd
    $sql = "delete from elabrev.matrizgnd where mtrid = " . $mtrid;
    $db->executar($sql);
    #acao
    $sql = "delete from elabrev.acaomatriz where mtrid = " . $mtrid;
    $db->executar($sql);
    #fonte
    $sql = "delete from elabrev.matriz where mtrid = " . $mtrid;
    #po
    $sql = "DELETE FROM proporc.matrizplanoorcamentario WHERE mtrid = {$mtrid}";
    $db->executar($sql);
    return (boolean) $db->executar($sql);
}

function atualiza_subacao($mtrid, $sbaids = array()) {
    global $db;
    $mtrid = (integer) $mtrid;
    if (!$mtrid) {
        return;
    }
    if ($sbaids[0] == '') {
        return;
    }

    $sql_remocao = "delete from elabrev.matrizsubacao where mtrid = " . $mtrid;
    $db->executar($sql_remocao);
    foreach ((array) $sbaids as $sbaid) {
        $sql_insercao = "insert into elabrev.matrizsubacao (mtrid, sbaid) values (" . $mtrid . ", '" . (integer) $sbaid . "')";
        $db->executar($sql_insercao);
    }
}

#pegando o orgcod
$sql = "select orgcod from instituicao where ittstatus='A'";
$RS = $db->record_set($sql);
$res = $db->carrega_registro($RS, 0);
$_SESSION['ittorgao'] = trim($res['orgcod']);


// PRÉ-PROCESSAMENTO
// VARIÁVEIS
// campos da entidade matriz
$mtrid = (integer) $_REQUEST['mtrid'];
$mtrdsc = (string) $_REQUEST['mtrdsc'];
$mtrsigla = (string) $_REQUEST['mtrsigla'];
//$mtrano = (string) $_REQUEST['mtrano'];
$mtrano = $_SESSION['exercicio'] + 1;
$gpmid = (integer) $_REQUEST['gpmid'];
$mtrmatriz = (boolean) $_REQUEST['mtrmatriz'];

// variáveis gerais
$modulo = $_REQUEST['modulo'];
$act = trim($_REQUEST['act']); // indica se o formulario foi submetido
//$titulo_modulo = $act == 'alterar' ? 'Alterar Coluna' : 'Incluir Coluna';
$titulo_modulo = $_REQUEST['acao'] == 'A' ? 'Alterar Coluna' : 'Incluir Coluna';
$sql_combo_grupo_matriz = "select gpmid as codigo, gpmdsc as descricao from elabrev.grupomatriz";
$cabecalho_matrizes = array('Ações', 'Descrição', 'Sigla', 'Execução');
$sql_matrizes = "select '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar Coluna\" onclick=\"altera_coluna('||mtrid||')\">&nbsp;&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir Coluna\" onclick=\"excluir_coluna('||mtrid||')\">' as acao, mtrdsc, mtrsigla, mtrano from elabrev.matriz where mtrano = '" . ($_SESSION['exercicio'] + 1) . "'";
$sql_fonte = "select foncod as codigo, foncod || ' - ' || fondsc as descricao from public.fonterecurso where fonstatus = 'A' order by foncod";
$sql_gnd = "select gndcod as codigo, gndcod || ' - ' || gnddsc as descricao from gnd where gndstatus = 'A' order by codigo";
$sql_acao = <<<DML
SELECT DISTINCT acacod AS codigo,
                acacod || ' - ' ||  acadsc AS descricao
  FROM elabrev.ppaacao_orcamento
  WHERE prgano = '{$_SESSION['exercicio']}'
    AND acasnrap = 'f'
  ORDER BY acacod
DML;
$sql_unidade = <<<DML
SELECT uniid AS codigo,
       unicod || ' - ' || unidsc AS descricao
  FROM unidade
  WHERE unistatus = 'A'
    AND (uni.orgcod = '26000' OR uni.unicod IN('74902', '73107'))
    AND orgcod = '{$_SESSION['ittorgao']}'
  ORDER BY unicod
DML;
$sql_subacao = <<<DML
SELECT sbaid AS codigo,
       sbacod || ' - ' || sbatitulo AS descricao
  FROM monitora.pi_subacao
  WHERE sbastatus = 'A'
    AND sbaano = '{$_SESSION['exercicio']}'
    AND sbasituacao = 'A'
    AND pieid IS NOT NULL
  ORDER BY sbacod
DML;
$mensagem_alert = '';
// FIM VARIÁVEIS
// MANIPULA REQUISICAO
// verifica se formulario foi submetido para exibição
if ($act == 'exibir') {
    // sobreescreve as variáveis declaradas como campos da entidade matriz
    $sql_matriz = "select mtrid, mtrdsc, mtrsigla, mtrano, gpmid, mtrmatriz from elabrev.matriz where mtrid = " . $mtrid;
    extract((array) $db->recuperar($sql_matriz));
    $mtrmatriz = $mtrmatriz == 't';
    $act = 'alterar'; // ação se transforma em alteração
// verifica se formulario foi submetido para alteração/inserção/exclusão
} elseif ($act == 'alterar' || $act == 'inserir' || $act == 'excluir') {
    if ($act == 'alterar') {
        // realiza alteração
        $sucesso = atualiza_matriz($mtrid, $mtrdsc, $mtrsigla, $mtrano, $gpmid, $mtrmatriz);
    } elseif ($act == 'inserir') {
        // realiza inserção
        // caso a inserção ocorra com sucesso o $mtrid é definido e definido e $executa_acao é verdadeiro
        // caso contrário o $mtrid fica zero e $executa_acao é falso
        $sucesso = (boolean) $mtrid = insere_matriz($mtrdsc, $mtrsigla, $mtrano, $gpmid, $mtrmatriz);
    } else {
        if (count(pegar_fontes_fixas($mtrid)) || count(pegar_unidades_fixas($mtmid))) {
            $mensagem_alert = 'Não é possível remover a coluna, pois ela já possui valores distribuídos.';
        } else {
            $sucesso = remover_matriz($mtrid);
        }
    }

    if ($sucesso) {
        atualiza_fonterecursomatriz($mtrid, $_REQUEST['fonte']);
        atualiza_acaomatriz($mtrid, $_REQUEST['acacod']);
        atualiza_matrizgnd($mtrid, $_REQUEST['gnd']);
        atualiza_unidadematriz($mtrid, $_REQUEST['unidade']);
        atualiza_subacao($mtrid, $_REQUEST['subacao']);
        atualiza_planosorcamentarios($mtrid, $_POST['planoorcamentario']);

        $db->commit();
        $db->sucesso('principal/propostaorcamentaria/configuracao/matriz', '&act=exibir&mtrid=' . $_REQUEST['mtrid']);
        exit();
    }
    // reinicia dados da entidade matriz
    $mtrid = 0;
    $mtrdsc = '';
    $mtrsigla = '';
    $mtrano = '';
    $gpmid = '';
    $mtrmatriz = '';
    $act = 'inserir';
} elseif ('listarUOs' == $act) { // -- Listagem de POs para associação na matriz/coluna
    require_once(APPRAIZ . 'www/planacomorc/_funcoesVinculacaoOrcamentaria.php');

    $dml = <<<DML
SELECT DISTINCT aca.unicod AS codigo,
                uni.unidsc AS descricao
  FROM monitora.acao aca
    INNER JOIN public.unidade uni USING(unicod)
  WHERE aca.prgano = '{$_SESSION['exercicio']}'
  ORDER BY aca.unicod
DML;
    if ($data = $db->carregar($dml)) {
        echo <<<HTML
<div class="step1" style="height:300px;overflow:auto">
HTML;
        echo inputSearch('unicodSearch');
        echo <<<HTML
    <table class="tabela unicodSearch" border="1" style="border-collapse:collapse" bgcolor="#f5f5f5"
           cellSpacing="4" cellPadding="4" align="center">
        <tr style="background-color:#000;color:#fff;">
            <th><input type="checkbox" class="superCheck uo" data-target="uoCheck" onclick="marcadesmarcatodosUO()" /></th>
            <th>Código</th>
            <th>Descrição</th>
        </tr>
HTML;
        foreach ($data as $unidade) {
            echo <<<HTML
        <tr>
            <td><input class="uoCheck" name="uo[]" value="{$unidade['codigo']}" id="uo{$unidade['codigo']}" type="checkbox" /></td>
            <td>{$unidade['codigo']}</td>
            <td>{$unidade['descricao']}</td>
        </tr>
HTML;
        }
        echo <<<HTML
    </table>
</div>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center" style="margin-top:7px">
    <tr>
        <td colspan="3" style="text-align:right"><input type="button" value="OK" class="btnAcao {$_GET['selecionaUo']}" /></td>
    </tr>
</table>
<div class="step2" style="display:none" />
HTML;
    } else {
        echo '<center>Nenhum item encontrado.</center>';
    }
    die();
} elseif ('listarPOs' == $act) {
    /**
     * Classe de tratamento de parâmetros da query.
     * @see DML
     */
    require_once(APPRAIZ . 'includes/library/simec/DB/DML.php');
    require_once(APPRAIZ . 'www/planacomorc/_funcoesVinculacaoOrcamentaria.php');

    $unicod = explode(',', $_REQUEST['unicod']);
    $query = <<<DML
SELECT ploid AS id,
       unicod || '.' || prgcod || '.' || acacod || '.' || loccod AS programatica,
       plocodigo || ' - ' || plotitulo AS planoorcamentario
  FROM elabrev.planoorcamentario plo
    INNER JOIN elabrev.ppaacao_orcamento pao USING(acaid)
  WHERE prgano = :prgano
    AND unicod = :unicod
  ORDER BY unicod, acacod, prgcod, plocodigo, plotitulo
DML;
    $dml = new Simec_DB_DML($query);
    $dml->addParam('prgano', $_SESSION['exercicio'], true)
            ->addParam('unicod', $unicod);

    if (!$dadospo = $db->carregar($dml)) {
        echo '<p>Nenhum PO encontrado para as UOs selecionadas.';
        die();
    }

    // -- Tabela com os POs retornados
    echo <<<HTML
<div class="step1" style="height:240px;overflow:auto">
HTML;
    echo inputSearch('unicodSearch');
    echo <<<HTML
    <table class="tabela unicodSearch" border="1" style="border-collapse:collapse" bgcolor="#f5f5f5"
           cellSpacing="4" cellPadding="4" align="center">
        <tr style="background-color:#000;color:#fff;">
            <th><input type="checkbox" class="superCheck po" data-target="poCheck" onclick="marcadesmarcatodosPO(this)" /></th>
            <th>Programática</th>
            <th>PO</th>
        </tr>
HTML;
    foreach ($dadospo as $po) {
        echo <<<HTML
        <tr>
            <td>
                <input class="poCheck" name="po[]" value="{$po['id']}" id="po{$po['id']}"
                       type="checkbox" data-programatica="{$po['programatica']}" data-po="{$po['planoorcamentario']}" />
            </td>
            <td>{$po['programatica']}</td>
            <td>{$po['planoorcamentario']}</td>
        </tr>
HTML;
    }
    echo <<<HTML
    </table>
</div>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center" style="margin-top:7px">
    <tr>
        <td colspan="3" style="text-align:right"><input type="button" value="Confirmar" class="btnSalvarPO" /></td>
    </tr>
</table>
HTML;
    die();
} else { // formulário não submetido
    $act = 'inserir';
}

// FIM MANIPULA REQUISICAO
$possui_limitefonte = false;
$unidades_fixas = array(); // códigos das unidades que não podem ser removidas
$fontes_fixas = array(); // códigos das fontes que não podem ser removidas
if ($mtrid) {
    $unidades_fixas = array_keys(pegar_unidades_fixas($mtrid));
    $fontes_fixas = array_keys(pegar_fontes_fixas($mtrid));
    $possui_limitefonte = count($unidades_fixas) || count($fontes_fixas);
    $possui_despesa = matriz_possui_despesa($mtrid);
    $possui_pos = matriz_possui_planosorcamentarios($mtrid);
}
// FIM PRÉ-PROCESSAMENTO

include APPRAIZ . "includes/cabecalho.inc";
?>
<br/>
<? $db->cria_aba($abacod_tela, $url, ''); ?>
<? monta_titulo($titulo_modulo, 'Administração de Colunas'); ?>
<script type="text/javascript" src="js/_funcoes.js"></script>
<script type="text/javascript" src="/planacomorc/js/tabelasApoio.js"></script>
<link rel="stylesheet" type="text/css" href="http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.9/themes/base/jquery-ui.css" />
<style type="text/css">
    .marcado{background-color:lightgreen}
    .remover{display:none}
</style>
<script type="text/javascript">
    /**
     * Impementa modal Dialog
     * dependencia: Jquery UI
     */
    var openModal = function(title, act)
    {
        var $modal = $1_11(".modal-dialog").css({width: ($1_11(window).width() * 0.8), height: "430px", display: "none"}),
                positionX = ($1_11(window).width() / 2) - ($modal.width() / 2),
                positionY = 80;

        $modal.dialog({
            position: [positionX, positionY],
            height: $modal.height(),
            width: $modal.width(),
            title: title,
            modal: true,
            open: function(e, ui) {
                callAjax(window.location.href + '&act=' + act, function(response) {
                    $1_11(".modal-dialog").html(response);
                });
            }
        });

        // -- Corrigindo o layout da modal, que aparecia todo bugado
        $1_11('.ui-widget-overlay')
                .css('height', $1_11(window).height() + 'px')
                .css('width', $1_11(window).width() + 'px');
        $1_11('.ui-dialog-titlebar-close span').css('margin', '-8px 0 0 -8px');
    };

    var abrirSelecaoPO = function(e) {
        // -- Abre popup
        $1_11('.modal-dialog').html(
                $1_11('<img/>', {border: 0, alt: 'Aguarde o carregamento...', src: '../imagens/wait.gif'})
                );

        openModal('Selecione o(s) Plano(s) Orçamentário(os)', 'listarUOs');
    };

    $1_11(document).ready(function() {
        $1_11('img.vincPO').click(abrirSelecaoPO);
        $1_11('#planoorcamentario').dblclick(abrirSelecaoPO);

    });

    jq.expr[':'].contains = function(a, i, m) {
        return jq(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
    };

    jq("input.unicodSearch").live("keyup", function() {
        jq('table.unicodSearch tbody tr td').removeClass('marcado');
        jq('table.unicodSearch tbody tr').removeClass('remover');
        var stringPesquisa = jQuery(this).val();
        if (stringPesquisa) {
            jq('table.unicodSearch tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
            jq('table.unicodSearch tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
        }
    });
    jq('#planoorcamentario').live('keydown', function(e) {
        combo_popup_remove_selecionados(e, 'planoorcamentario');
    });

    jq('input.btnAcao').live('click', function() {
        var $btnacao = jq(this);
        if ('none' === jq('.step1').css('display')) {
            jq('.step1').show();
            jq('.step2').hide().html('');
            $btnacao.attr('value', 'ok');
        } else {
            var uos = [];
            jq('input.uoCheck:checked').each(function() {
                uos.push(jq(this).val());
            });

            // -- requisicao dos POs
            callAjax(
                    window.location.href + '&act=listarPOs&unicod=' + uos.join(','),
                    function(response) {
                        jq('.step1').hide();
                        jq('.step2').show().html(response);
                        $btnacao.attr('value', '<<< voltar');
                    }
            );
        }
    });

    // -- Seleção de POs na popup
    marcaPO = function(elem) {
        var $chkbox = jq(elem.target);
        // -- Se o checkbox for marcado, adiciona-o à lista de seleção
        if ($chkbox.attr('checked')) {
            // -- Oculta o placeholder, se ele ainda existir
            jq('#planoorcamentario option[value=""]').remove();
            var option = new Option(
                $chkbox.attr('data-programatica') + ': ' + $chkbox.attr('data-po'),
                $chkbox.val()
            );
            jq(option).attr('id', 'po_' + $chkbox.val());
            jq('#planoorcamentario').append(option);
        } else { // -- Se o checkbox for desmarcado, a opção deve ser removida da lista
            jq('#po_' + $chkbox.val()).remove();
            if (1 === jq('#planoorcamentario option').size()) {
                var optPlaceholder = new Option('Duplo clique para selecionar da lista', '');
                jq('#planoorcamentario').append(optPlaceholder);
                jq('.placeholder').css('display', 'block');
            }
        }
    };

    jq('input.poCheck').live('click', marcaPO);

    // -- Bind de ação para fechar a popup direto da seleção de POs
    jq(".btnSalvarPO").live('click', function() {
        $1_11(".modal-dialog").dialog("close");
    });
</script>
<form method="post" name="formulario">
    <input type="hidden" name="modulo" value="<?= $modulo ?>">
    <input type="hidden" name="mtrid" value="<?= $mtrid ?>">
    <input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>">
    <input type="hidden" name="act" value="<?= $act ?>">
    <!-- FORMULÁRIO DE INSERÇÃO / EDIÇÃO -->
    <table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
        <tr>
            <!-- EXERCICIO -->
            <td align='right' class="SubTituloDireita">Exercício:</td>
            <td><?= campo_texto('mtrano', 'N', 'N', '', 4, 4, '####', ''); ?></td>
        </tr>
        <tr>
            <!-- DESCRICAO -->
            <td align='right' class="SubTituloDireita">Descrição:</td>
            <td><?= campo_texto('mtrdsc', 'S', 'S', '', 50, 200, '', ''); ?></td>
        </tr>
        <tr>
            <!-- SIGLA -->
            <td align='right' class="SubTituloDireita">Sigla:</td>
            <td><?= campo_texto('mtrsigla', 'S', 'S', '', 7, 7, '', ''); ?></td>
        </tr>
        <tr>
            <!-- GRUPO MATRIZ -->
            <td align='right' class="SubTituloDireita">Grupo Coluna:</td>
            <td><? $db->monta_combo('gpmid', $sql_combo_grupo_matriz, 'S', '', '', ''); ?></td>
        </tr>
        <tr>
            <!-- GRUPO MATRIZ -->
            <td align='right' class="SubTituloDireita">Fontes de Recurso:</td>
            <td>
                <? $sql_auxiliar = "select f.foncod as codigo, f.foncod || ' - ' || f.fondsc as descricao from elabrev.fonterecursomatriz fm inner join public.fonterecurso f on fm.foncod = f.foncod where fm.mtrid = " . $mtrid . $where_fontes_fixas; ?>
                <? $fonte = $db->carregar($sql_auxiliar); ?>
                <? combo_popup('fonte', $sql_fonte, 'Selecione a(s) Fonte(s) de Recurso', '600x700', 0, $fontes_fixas, 'Não é possível remover fonte, pois ela já possui valor de limite distribuido por essa coluna.'); ?>
            </td>
        </tr>
        <tr>
            <!-- GRUPO MATRIZ -->
            <td align='right' class="SubTituloDireita">GND:</td>
            <td>
                <? $sql_auxiliar = "select g.gndcod as codigo, g.gndcod || ' - ' || g.gnddsc as descricao from elabrev.matrizgnd mg inner join gnd g on mg.gndcod = g.gndcod where mg.mtrid = " . $mtrid; ?>
                <? $gnd = $db->carregar($sql_auxiliar); ?>
                <? $sql_auxiliar = "select g.gndcod from elabrev.matrizgnd mg inner join gnd g on mg.gndcod = g.gndcod where mg.mtrid = " . $mtrid; ?>
                <? $gnds = $db->carregar($sql_auxiliar); ?>
                <? $gnds_fixos = array(); ?>
                <? if ($gnds) : ?>
                    <? foreach ($gnds as $gnd_fixo) : ?>
                        <? array_push($gnds_fixos, $gnd_fixo['gndcod']); ?>
                    <? endforeach; ?>
                <? endif; ?>
                <? combo_popup('gnd', $sql_gnd, 'Selecione o(s) GND', '400x400', 0, $gnds_fixos, 'Não é possível remover gnd, pois a coluna já possui valor de limite distribuido.'); ?>
            </td>
        </tr>
        <tr>
            <!-- AÇÃO -->
            <td align='right' class="SubTituloDireita">Ações Relacionadas:</td>
            <td>
                <?php
                $sql_auxiliar = <<<DML
SELECT DISTINCT a.acacod AS codigo,
                a.acacod || ' - ' || a2.acadsc AS descricao
  FROM elabrev.matrizgnd mg
    INNER JOIN elabrev.acaomatriz a ON mg.mtrid = a.mtrid
    INNER JOIN elabrev.ppaacao_orcamento a2 ON a.acacod = a2.acacod AND acasnrap = 'f'
  WHERE a2.prgano = '{$_SESSION['exercicio']}'
    AND mg.mtrid = {$mtrid}
DML;
                $acacod = $db->carregar($sql_auxiliar);
                $sql_auxiliar = <<<DML
SELECT DISTINCT a.acacod
  FROM elabrev.matrizgnd mg
    INNER JOIN elabrev.acaomatriz a ON mg.mtrid = a.mtrid
    INNER JOIN monitora.acao a2 ON a.acacod = a2.acacod AND acasnrap = 'f'
  WHERE a2.prgano = '{$_SESSION['exercicio']}'
    AND mg.mtrid = {$mtrid}
DML;
                $acoes = $db->carregar($sql_auxiliar);
                $acoes_fixas = array();
                if ($acoes) {
                    foreach ($acoes as $acao_fixa) {
                        array_push($acoes_fixas, $acao_fixa['acacod']);
                    }
                }
                combo_popup('acacod', $sql_acao, 'Selecione a(s) Ação(ões)', '400x400', 0, $acoes_fixas, 'Não é possível remover ação relacionada, pois a coluna já possui valor de limite distribuido.');
                ?>
            </td>
        </tr>
        <tr>
            <!-- GRUPO MATRIZ -->
            <td align='right' class="SubTituloDireita">Unidades Orçamentárias:</td>
            <td>
                <?
                $sql_auxiliar = "select u.uniid as codigo, u.unicod || ' - ' || u.unidsc as descricao from elabrev.unidadematriz um inner join unidade u on um.unicod = u.unicod and um.unitpocod = u.unitpocod where um.mtrid = " . $mtrid . $where_unidades_fixas;
                $unidade = $db->carregar($sql_auxiliar);

                $sql_unidade = "select u.uniid as codigo, u.unicod || ' - ' || u.unidsc as descricao, guu.guoid
                                from unidade u
                                    left join elabrev.grupouo_unidade guu on guu.unicod = u.unicod
                                where u.unitpocod = 'U'
                                and u.unistatus = 'A'
                                and u.orgcod = '" . ((integer) $_SESSION['ittorgao']) . "'
                                order by u.unicod";

                combo_popup_uo('unidade', $sql_unidade, 'Selecione a(s) Unidade(s) Orçamentárias', '500x700', 0, $unidades_fixas, 'Não é possível remover unidade, pois ela já possui valor de limite atribuido à ela por essa coluna.');
                ?>
            </td>
        </tr>
        <tr>
            <!-- SUBACAO -->
            <td align='right' class="SubTituloDireita">Subação:</td>
            <td>
                <?php
                // -- subações selecionados
                $sql_auxiliar = <<<DML
SELECT pi.sbaid AS codigo,
       pi.sbacod || ' - ' || pi.sbatitulo AS descricao
  FROM monitora.pi_subacao AS pi
    INNER JOIN elabrev.matrizsubacao AS ma ON pi.sbaid = ma.sbaid
  WHERE sbaano = '{$_SESSION['exercicio']}'
    AND ma.mtrid = {$mtrid}{$where_unidades_fixas}
DML;
                $subacao = $db->carregar($sql_auxiliar);
                $selecione = 'Selecione a(s) Subação(ões)';
                $alert = 'Não é possível remover unidade, pois ela já possui valor de limite atribuido à ela por essa coluna.';
                combo_popup('subacao', $sql_subacao, $selecione, '400x500', 0, $unidades_fixas, $alert);
                unset($selecione, $alert);
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita">Plano Orçamentário:</td>
            <td>
                <select id="planoorcamentario" class="CampoEstilo vincPO" style="width:400px"
                        name="planoorcamentario[]" size="10" multiple="multiple" tipo="combo_popup" maximo="0" style="overflow:auto">
                            <?php
                            // -- POs selecionados
                            if ($possui_pos) {
                                foreach ($possui_pos as $po) {
                                    echo <<<HTML
                    <option value="{$po['ploid']}">{$po['programatica']}: {$po['planoorcamentario']}</option>
HTML;
                                }
                            } else {
                                echo <<<HTML
                    <option value="">Duplo clique para selecionar da lista</option>
HTML;
                            }
                            ?>
                </select>
                <img align="absmiddle" style="cursor:pointer" src="../imagens/pop_p.gif" class="vincPO" />
            </td>
        </tr>
        <tr style="display:none">
            <!-- DISTRIBUICAO -->
            <td align='right' class="SubTituloDireita">Matriz de Distribuição:</td>
            <td>
                <input <?= $possui_limitefonte ? ' disabled="disabled" ' : '' ?> type="radio" name="mtrmatriz_check" value="1" id="mtrmatriz_1" <?= $mtrmatriz ? 'checked="checked"' : '' ?>>
                <label for="mtrmatriz_1">Sim</label>
                <input <?= $possui_limitefonte ? ' disabled="disabled" ' : '' ?> type="radio" name="mtrmatriz_check" value="0" id="mtrmatriz_0" <?= $mtrmatriz ? '' : 'checked="checked"' ?>>
                <label for="mtrmatriz_0">Não</label>
                <input type="hidden" name="mtrmatriz" value="<?= mtrmatriz ? '1' : '0' ?>" />
            </td>
        </tr>
        <tr bgcolor="#cccccc">
            <td>&nbsp;</td>
            <td>
                <input type="button" name="bsubmeter" value="Salvar" class="botao" onclick="validar_cadastro();"/>
            </td>
        </tr>
    </table>
    <!-- FIM FORMULÁRIO DE INSERÇÃO / EDIÇÃO -->
</form>
<div class="modal-dialog" style="left:0px;display:none">
    <img src="../imagens/wait.gif" border="0" alt="Aguarde o carregamento..." />
</div>
<!-- LISTAGEM -->
<? $db->monta_lista($sql_matrizes, $cabecalho_matrizes, 200, 20, '', '', ''); ?>
<!-- FIM LISTAGEM -->
<script language="javascript">

<!--

    /**
     * Indica se a coluna atual possui algum limitefonte ou não.
     *
     * @var integer
     */
    var possui_limitefonte = <?= $possui_limitefonte ? 'true' : 'false' ?>;

    /**
     * Quantidade de fontes que possuem limite definido para a coluna em questão.
     *
     * @far integer
     */
    var quantidade_fontes_fixas = <?= count($fontes_fixas) ?>;

    /**
     * Indica se a coluna atual possui despesa ou não.
     *
     * @var integer
     */
    var possui_despesa = <?= $possui_despesa ? 'true' : 'false' ?>

    /**
     * Prepara formulário para alteração.
     *
     * @param integer mtrid
     * @return void
     */
    function altera_coluna(mtrid)
    {
        document.location.href = '?modulo=principal/propostaorcamentaria/configuracao/matriz&acao=A&act=exibir&mtrid=' + mtrid;
    }

    /**
     * Prepara formulário para remoção.
     *
     * @param integer mtrid
     * @return void
     */
    function excluir_coluna(mtrid)
    {
        if (window.confirm("Confirma a exclusão da coluna selecionada (" + mtrid + ") em Colunas?")) {
            if (window.confirm('Esta ação NÃO PODERÁ SER DESFEITA. Confirma?')) {
                document.location.href = '?modulo=principal/propostaorcamentaria/configuracao/matriz&acao=I&act=excluir&mtrid=' + mtrid;
            }
        }
        return;
    }

    /**
     * Verifica se formulário este correto e o submete.
     *
     * @return void
     */
    function validar_cadastro()
    {
        prepara_formulario();
        var formulario = document.formulario;
        if (!validaBranco(formulario.mtrano, 'Exercício'))
            return;
        if (!validaBranco(formulario.mtrdsc, 'Descrição'))
            return;
        if (!validaBranco(formulario.gpmid, 'Grupo Coluna'))
            return;
        if (!possui_limitefonte) {
            if (!validaBranco(formulario.fonte, 'Fontes de Recurso'))
                return;
            if (!validaBranco(formulario.unidade, 'Unidades Orçamentárias'))
                return;
        }
        if (!possui_despesa) {
            if (!validaBranco(formulario.gnd, 'GND'))
                return;
        }
        // verifica quantidade de fontes de acordo com o campo mtrmatriz
        // caso seja matriz só pode haver uma fonte selecionada
        if (formulario.mtrmatriz_check[0].checked == '1' && formulario.fonte.options.length != 1) {
            alert('Para matrizes de distribuição só pode haver uma fonte de recurso.');
            return;
        }
        formulario.mtrmatriz.value = formulario.mtrmatriz_check[0].checked ? '1' : '0';
        formulario.submit();
    }

<? if ($mensagem_alert != '') : ?>
        alert('<?= str_replace("'", "\\'", $mensagem_alert) ?>');
<? endif; ?>

-->

</script>
<script>
// Gato para marcar e desmarcar todos os POs
function marcadesmarcatodosUO() {
        jQuery(".uoCheck").click();
}

function marcadesmarcatodosPO(elem)
{
    if (jQuery(elem).attr('checked')) {
        jQuery('.poCheck').each(function(){
            jQuery(this).attr('checked', true);
            marcaPO({target:this});
        });
    } else {
        jQuery('.poCheck').each(function(){
            jQuery(this).attr('checked', false);
            marcaPO({target:this});
        });
    }
}
</script>