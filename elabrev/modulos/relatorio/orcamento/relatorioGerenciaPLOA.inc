<?php
/**
 * Relatório gerencial da PLOA
 * Exibe o detalhamento dos valores declarados nas propostas orçamentárias.
 * @package ProgramacaoOrcamentaria
 * @subpackage RelatorioOrcamento
 */

header('content-type: text/html; charset=ISO-8859-1');

/**
 * Lista de atributos utilizadas como agrupadores, colunas e filtros.
 */
$listaAtributos = array(
    'unicod' => 'Unidade',
    'prgcod' => 'Programa',
    'acacod' => 'Ação',
    'acadsc' => 'Descrição da Ação',
    'loccod' => 'Localizador',
    'funcod' => 'Função',
    'sfucod' => 'Subfunção',
    'plocodigo' => 'Código PO',
    'plotitulo' => 'Descrição do PO',
    'sbacod' => 'Subação',
    'sacdsc' => 'Descrição da Subação',
    'acadscprosof' => 'acadscprosof',
    'acadscunmsof' => 'acadscunmsof',
    'ndpcod' => 'Natureza da Despesa',
    'esfdsc' => 'Esfera',
    'iducod' => 'iducod',
    'foncod' => 'Fonte',
    'acaqtdefisico' => 'Quantidade Física',
    'valor' => 'Valor',
    'expansao' => 'Limite Adicional',
    'valortotal' => 'Valor Total',
);

// executa e exibe a consulta solicitada pelo usuário
if (isset($_REQUEST['pesquisa']) == true && !$_REQUEST['carregar']) {
    if ($_REQUEST['prtid']) {
        $sql = sprintf("select prtobj from public.parametros_tela where prtid = {$_REQUEST['prtid']}");
        $itens = $db->pegaUm($sql);
        $dados = unserialize(stripslashes(stripslashes($itens)));
        $_REQUEST = $dados;
        unset($_REQUEST['salvar']);
    }
    switch ($_REQUEST['pesquisa']) {
        case '1':
            require_once(APPRAIZ . "elabrev/modulos/relatorio/orcamento/relatorioGerencialPLOAResult.inc");
            exit;
            // -- no break
//        case '2':
//            include "osxls_resultado.inc";
//            exit;
    }
}

/**
 * Query de consulta das esferas, com placeholder para substituição
 */
$qryEsfera = <<<DML
SELECT esf.esfcod AS codigo,
       esf.esfcod || ' - ' || esf.esfdsc AS descricao
  FROM public.esfera esf
  WHERE esf.esfstatus = 'A' __WHERE_ADICIONAL__
  ORDER BY esf.esfcod
DML;

/**
 * Query de consulta dos programas, com placeholder para substituição
 */
$qryPrograma = <<<DML
SELECT prg.prgid AS codigo,
       prg.prgcod || ' - ' || prg.prgdsc AS descricao
  FROM elabrev.ppaprograma_orcamento prg
  WHERE prg.prgano = '{$_SESSION['exercicio']}'
    AND prg.prgstatus = 'A' __WHERE_ADICIONAL__
  ORDER BY prg.prgcod
DML;

/**
 * Query de consulta das ações, com placeholder para substituição
 */
$qryAcao = <<<DML
SELECT DISTINCT aca.acaid AS codigo,
       aca.acacod || ' - ' || aca.acadsc AS descricao
  FROM elabrev.ppaacao_orcamento aca
  WHERE aca.prgano = '{$_SESSION['exercicio']}'
    AND aca.acastatus = 'A' __WHERE_ADICIONAL__
  ORDER BY aca.acacod || ' - ' || aca.acadsc
DML;

/**
 * Query de consulta das unidade, com placeholder para substituição
 */
$qryUnidade = <<<DML
SELECT uni.unicod AS codigo,
       uni.unicod || ' - ' || uni.unidsc AS descricao
  FROM public.unidade uni
  WHERE uni.unistatus = 'A'
    AND (uni.orgcod = '26000' OR uni.unicod like '74902') __WHERE_ADICIONAL__
  ORDER BY uni.unicod
DML;

/**
 * Query de consulta dos localizadores, com placeholder para substituição
 */
$qryLocalizador = <<<DML
select DISTINCT aca.loccod AS codigo,
                aca.loccod as descricao
  from elabrev.ppaacao_orcamento aca
  WHERE aca.prgano = '{$_SESSION['exercicio']}' __WHERE_ADICIONAL__
  ORDER BY descricao
DML;

/**
 * Query de consulta das funcoes, com placeholder para substituição
 */
$qryFuncao = <<<DML
SELECT funcod AS codigo,
       funcod || ' - ' || fundsc AS descricao
  FROM elabrev.ppafuncao fun
  WHERE fun.funstatus = 'A' __WHERE_ADICIONAL__
  ORDER BY funcod
DML;

/**
 * Query de consulta das subfuncoes, com placeholder para substituição
 */
$qrySubfuncao = <<<DML
SELECT sfucod AS codigo,
       sfucod || ' - ' || sfudsc AS descricao
  FROM elabrev.ppasubfuncao sfu
  WHERE sfu.sfustatus = 'A' __WHERE_ADICIONAL__
  ORDER BY sfucod
DML;

/**
 * Query de consulta das naturezas, com placeholder para substituição
 */
$qryNatureza = <<<DML
SELECT DISTINCT ndpid AS codigo,
                SUBSTR(ndpcod, 1, 6) || ' - ' || ndpdsc AS descricao
  FROM public.naturezadespesa
  WHERE ndpstatus = 'A'
    AND sbecod = '00'
    AND edpcod != '00'
    AND SUBSTR(ndpcod, 1, 2) NOT IN('31', '32', '46')
    AND (SUBSTR(ndpcod, 3, 2) IN('80', '90', '91') OR SUBSTR(ndpcod, 1, 6) IN ('335039')) __WHERE_ADICIONAL__
  ORDER BY 2
DML;

$qryPropostaOrcamento = <<<DML
SELECT ppoid AS codigo,
       ppoid || ' - ' || ppodsc || ' - ' || ppoano AS descricao
  FROM elabrev.propostaorcamento ppo
  WHERE ppo.ppostatus = 'A'
  ORDER BY ppoid DESC
DML;

/**
 * Prepara a query de elementos marcados para um filtro do tipo mostraComboPopup.
 * 
 * @param string $query Query de busca dos elementos do filtro, com o placeholder __WHERE_ADICIONAL__
 * @param string $rqtCampo O nome do campo no $_REQUEST
 * @param string $qryCampo O nome do campo na query, é usado para identificar os elementos que aparecerão marcados - corresponde a código
 * @param bool $quoteValores Indica se os valores devem ser colocados entre aspas, para strings
 * @return string
 */
function querySelecionados($query, $rqtCampo, $qryCampo, $quoteValores = true)
{
    $queryRetorno = '';
    if ($_REQUEST[$rqtCampo] && $_REQUEST[$rqtCampo][0] != '') {
        if ($quoteValores) {
            // -- Aplicando aspas aos elementos do array
            array_walk($_REQUEST[$rqtCampo], 'quote');
        }
        $queryRetorno = str_replace(
                '__WHERE_ADICIONAL__',
                " AND {$qryCampo} IN(" . implode(',', $_REQUEST[$rqtCampo]),
                $query);
    }
    return $queryRetorno;
}

/**
 * 
 * @param type $rqtCampo
 * @param type $qryCampo
 * @param type $quoteValores
 * @return type
 */
function queryfiltro($rqtCampo, $qryCampo, $quoteValores = true) {
    // -- Se não existir nenhum valor carregado no filtro, termina a execução
    if (empty($_REQUEST[$rqtCampo])) {
        return;
    }
    // -- Normalizando o campo no request como array para simplificação do tratamento
    if (!is_array($_REQUEST[$rqtCampo])) {
        $_REQUEST[$rqtCampo] = array($_REQUEST[$rqtCampo]);
    }
    // -- Processando os elementos do filtro
    if (!empty($_REQUEST[$rqtCampo][0])) {
        if ($quoteValores) {
            array_walk($_REQUEST[$rqtCampo], 'quote');
        }
        if (1 == count($_REQUEST[$rqtCampo])) { // -- Processando apenas um elemento para a query
            return "{$qryCampo} = {$_REQUEST[$rqtCampo][0]}";
        } else { // -- Processando vários elementos para a query
            return "{$qryCampo} IN(" . implode(', ', $_REQUEST[$rqtCampo]) . ")";
        }
    }
    return;
}

/**
 * @see cabecalho.inc
 */
include APPRAIZ . 'includes/cabecalho.inc';

/**
 * @see Agrupador
 */
include APPRAIZ . 'includes/Agrupador.php';

monta_titulo('Relatório Gerencial da PLOA', 'Selecione as opções desejadas');
?>
<script type="text/javascript" language="javascript" src="js/relatorio.js"></script>
<script type="text/javascript" language="javascript">
function exibeRelatorioGeral(tipo) {

    var formulario = document.formulario;
    var agrupador = document.getElementById('agrupador');
    var coluna = document.getElementById('coluna');
    var ppoid = document.getElementById('ppoid');

    // Tipo de relatorio
    formulario.pesquisa.value = '1';
    prepara_formulario();
    selectAllOptions( formulario.agrupador );
    selectAllOptions( coluna );
//    selectAllOptions( ordenarcampo );

    if (tipo == 'relatorio') {
        formulario.action = '<?php echo $hrefRedir; ?>';
        window.open('', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
        formulario.target = 'relatorio';
    } else {
        if (tipo == 'exibir') {
//            if (!agrupador.options.length) {
//                alert('Favor selecionar ao menos um item para agrupar o resultado!');
//                return false;
//            }
//            if (!coluna.options.length) {
//                alert('Favor selecionar ao menos uma coluna!');
//                return false;
//            }
            selectAllOptions(agrupador);
            selectAllOptions(coluna);
//            selectAllOptions(ordenarcampo);

            formulario.target = 'resultadoGeral';
            var janela = window.open('', 'resultadoGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
            janela.focus();
        } else if(tipo == 'salvar') {
            if (formulario.titulo.value == '') {
                alert( 'É necessário informar o título do relatório!' );
                formulario.titulo.focus();
                return;
            }
            var nomesExistentes = new Array();
            <?php
            $sqlNomesConsulta = "SELECT prtdsc FROM public.parametros_tela";
            $nomesExistentes = $db->carregar( $sqlNomesConsulta );
            if ($nomesExistentes){
                foreach ($nomesExistentes as $linhaNome) {
                    print "nomesExistentes[nomesExistentes.length] = '" . str_replace( "'", "\'", $linhaNome['prtdsc'] ) . "';";
                }
            }
            ?>
            var confirma = true;
            var i, j = nomesExistentes.length;
            for (i = 0; i < j; i++) {
                if (nomesExistentes[i] == formulario.titulo.value) {
                    confirma = confirm('Deseja alterar a consulta já existente?');
                    break;
                }
            }
            if (!confirma) {
                return;
            }
            formulario.target = '_self';
            formulario.action = 'elabrev.php?modulo=relatorio/termoCooperacao/relGeral&acao=A&salvar=1';
        }
    }
    formulario.submit();
}
</script>
<form action="" method="post" name="formulario" id="filtro">
    <input type="hidden" name="pesquisa" value="1" />

    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
<!--        <tr>
            <td class="SubTituloDireita">Título</td>
            <td>
                <?php echo campo_texto( 'titulo', 'N', $somenteLeitura, '', 65, 100, '', '', 'left', '', 0, 'id="titulo"'); ?>
            </td>
        </tr>-->
        <?php // -- Listagem de agrupadores ?>
        <tr>
            <td class="SubTituloDireita" width="15%">Agrupadores:</td>
            <td>
            <?php
            // -- Listagem de agrupadores
            $destino = empty($agrupadorAux)
                            ?array()
                            :$agrupadorAux;

            $origem = array(
                array('codigo' => 'plocodigo', 'descricao' => $listaAtributos['plocodigo']),
                array('codigo' => 'foncod', 'descricao' => $listaAtributos['foncod']),
                array('codigo' => 'sbacod', 'descricao' => $listaAtributos['sbacod']),
                array('codigo' => 'ndpcod', 'descricao' => $listaAtributos['ndpcod']),
            );
            $agrupador = new Agrupador('filtro', '');
            $agrupador->setOrigem('naoAgrupador', null, $origem);
            $agrupador->setDestino('agrupador', null, $destino);
            $agrupador->exibir();
            $origem = $destino = $agrupador = null;
            unset($origem, $destino, $agrupador);
            ?>
            </td>
        </tr>
        <?php // -- Colunas opcionais do relatório ?>
        <tr>
            <td class="SubTituloDireita" width="15%">Colunas:</td>
            <td>
            <?php
            // -- Listagem de agrupadores
            $destino = empty($colunaAux)
                            ?array()
                            :$colunaAux;
            $origem = array(
                array('codigo' => 'acadsc', 'descricao' => $listaAtributos['acadsc']),
                array('codigo' => 'plotitulo', 'descricao' => $listaAtributos['plotitulo']),
                array('codigo' => 'sacdsc', 'descricao' => $listaAtributos['sacdsc']),
            );
            $agrupador = new Agrupador('filtro', '');
            $agrupador->setOrigem('naoColuna', null, $origem);
            $agrupador->setDestino('coluna', null, $destino);
            $agrupador->exibir();
            $origem = $destino = $agrupador = null;
            unset($origem, $destino, $agrupador);
            ?>
            </td>
        </tr>
    </table>
    <?php // -- Controle de armazenamento de relatórios - skippado por enquanto ?>
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td class="SubTituloDireita" width="195" valign="top">Proposta Orçamentária</td>
            <td>
            <?php
            // -- Proposta orçamentária
            $db->monta_combo(
                    'ppoid',
                    $qryPropostaOrcamento,
                    'S',
                    null,
                    '',
                    '',
                    '',
                    '',
                    'S',
                    'ppoid',
                    '',
                    $_REQUEST['ppoid']
            );
            ?>
            </td>
        </tr>
    <?php
    // -- Filtros do relatório

    // -- Esfera
    mostrarComboPopup(
            $listaAtributos['esfdsc'],
            'esfcod',
            str_replace('__WHERE_ADICIONAL__', '', $qryEsfera),
            querySelecionados($qryEsfera, 'esfcod', 'esf.esfcod', $quoteValores = false),
            "Selecione a(s) {$listaAtributos['esfdsc']}(s)"
    );
    // -- Programa
    mostrarComboPopup(
            $listaAtributos['prgcod'],
            'prgcod',
            str_replace('__WHERE_ADICIONAL__', '', $qryPrograma),
            querySelecionados($qryPrograma, 'prgcod', 'prg.prgid', $quoteValores = false),
            "Selecione o(s) {$listaAtributos['prgcod']}(s)"
    );
    // -- Acao
    mostrarComboPopup(
            $listaAtributos['acacod'],
            'acacod',
            str_replace('__WHERE_ADICIONAL__', '', $qryAcao),
            querySelecionados($qryAcao, 'acacod', 'aca.acacod', $quoteValores = true),
            "Selecione a(s) {$listaAtributos['acacod']}(es)"
    );
    // -- Unidade
    mostrarComboPopup(
            $listaAtributos['unicod'],
            'unicod',
            str_replace('__WHERE_ADICIONAL__', '', $qryUnidade),
            querySelecionados($qryUnidade, 'unicod', 'uni.unicod', $quoteValores = true),
            "Selecione a(s) {$listaAtributos['unicod']}(s)"
    );
    // -- Localizador
    mostrarComboPopup(
            $listaAtributos['loccod'],
            'loccod',
            str_replace('__WHERE_ADICIONAL__', '', $qryLocalizador),
            querySelecionados($qryLocalizador, 'loccod', 'aca.loccod', $quoteValores = true),
            "Selecione o(s) {$listaAtributos['loccod']}(es)"
    );
    // -- Função
    mostrarComboPopup(
            $listaAtributos['funcod'],
            'funcod',
            str_replace('__WHERE_ADICIONAL__', '', $qryFuncao),
            querySelecionados($qryFuncao, 'funcod', 'fun.funcod', $quoteValores = true),
            "Selecione a(s) {$listaAtributos['funcod']}(ões)"
    );
    // -- Subfunção
    mostrarComboPopup(
            $listaAtributos['sfucod'],
            'sfucod',
            str_replace('__WHERE_ADICIONAL__', '', $qrySubfuncao),
            querySelecionados($qrySubfuncao, 'sfncod', 'sfu.sfucod', $quoteValores = true),
            "Selecione o(s) {$listaAtributos['sfncod']}(s)"
    );
    // -- Natureza
    mostrarComboPopup(
            $listaAtributos['ndpcod'],
            'ndpcod',
            str_replace('__WHERE_ADICIONAL__', '', $qryNatureza),
            querySelecionados($qryNatureza, 'ndpcod', 'ndp.ndpid', $quoteValores = true),
            "Selecione o(s) {$listaAtributos['ndpcod']}(s)"
    );
    ?>
        <tr>
            <td class="SubTituloDireita">Tipo Relatório:</td>
            <td>
                <input type="radio" name="tiporel" value="1" id="tiporel_1" <?php if($tiporel=='1') echo "checked"; ?>> 
                <label for="tiporel_1" style="cursor: pointer">Detalhado</label>
                &nbsp;&nbsp;&nbsp;
                <input type="radio" name="tiporel" value="2"id="tiporel_2" <?php if($tiporel=='2' || !$tiporel) echo "checked"; ?>>
                <label for="tiporel_2" style="cursor: pointer">Resumido</label>
            </td>
        </tr>
        <tr>
            <td bgcolor="#CCCCCC"></td>
            <td bgcolor="#CCCCCC">
                 <input type="button" value="Visualizar" onclick="exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
                  &nbsp;&nbsp;&nbsp;
                 <!--<input type="button" value="Visualizar XLS" onclick="exibeRelatorioGeralXLS();" style="cursor: pointer;"/>-->
                  &nbsp;&nbsp;&nbsp;
                 <!--<input type="button" value="Salvar Consulta" onclick="exibeRelatorioGeral('salvar');" style="cursor: pointer;"/>-->
            </td>
        </tr>
    </table>
</form>
