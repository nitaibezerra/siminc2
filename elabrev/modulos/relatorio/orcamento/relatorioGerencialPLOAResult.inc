<?php
/**
 * Relatório gerencial da PLOA
 * Exibe o detalhamento dos valores declarados nas propostas orçamentárias.
 * @package ProgramacaoOrcamentaria
 * @subpackage RelatorioOrcamento
 */

// -- coloca periodo e valor do ponto no titulo
if (!$_REQUEST['titulo']) {
    $_REQUEST['titulo'] = "Relatório Gerencial da PLOA";
}

// --  Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

$relatorio = new montaRelatorio();
$relatorio->setAgrupador(
        monta_agrupador_ger_ploa(),
        $db->carregar(
                monta_sql_ger_ploa()
        )
);
$relatorio->setColuna(
        monta_coluna_ger_ploa()
);
$relatorio->setBrasao(true);
if (2 == $_REQUEST['tiporel']) {
    $relatorio->setEspandir(false);
} else {
    $relatorio->setEspandir(true);
}
$relatorio->setTotNivel(true);
$relatorio->setMonstrarTolizadorNivel(true);
?>
<html>
    <head>
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    </head>
    <body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
        <!--  Monta o Relatório -->
        <? echo $relatorio->getRelatorio(); ?>
    </body>
</html>
<?php
function monta_sql_ger_ploa()
{
    $where = array();
    // -- Filtros dos relatórios
    $where[] = queryFiltro('ppoid', 'd.ppoid', false); // -- Proposta Orçamentária
    $where[] = queryFiltro('esfcod', 'a.esfcod'); // -- Esfera
    $where[] = queryFiltro('prgcod', 'a.prgid'); // -- Programa
    $where[] = queryFiltro('acacod', 'a.acaid'); // -- Ação
    $where[] = queryFiltro('unicod', 'a.unicod'); // -- Unidade Orçamentária
    $where[] = queryFiltro('loccod', 'a.loccod'); // -- Localizador
    $where[] = queryFiltro('funcod', 'a.funcod'); // -- Função
    $where[] = queryFiltro('sfucod', 'a.sfucod'); // -- Subfunção
    $where[] = queryFiltro('ndpcod', 'n.ndpid'); // -- Natureza da despesa

    $where = array_filter($where);
    if ($where) {
        $where = ' WHERE ' . implode(' AND ', $where);
    } else {
        $where = '';
    }

    $sql = <<<DML
SELECT distinct a.prgcod,
                a.acacod,
                a.unicod,
                a.loccod,
                a.acadsc,
                a.funcod,
                a.sfucod,
                po.plocodigo,
                po.plotitulo,
                COALESCE(sbacod, '-') AS sbacod,
                a.sacdsc,
                COALESCE(TRIM(a.acadscprosof), '-') AS acadscprosof,
                COALESCE(a.acadscunmsof, '-') AS acadscunmsof,
                a.acaqtdefisico,
                n.ndpcod,
                COALESCE(SUBSTR(e.esfdsc, 1, 1), '-') AS esfdsc,
                d.iducod,
                d.foncod,
                d.dpavalor::numeric AS valor,
                d.dpavalorexpansao::numeric AS expansao,
                d.dpavalor::numeric + d.dpavalorexpansao::numeric AS valortotal
  FROM elabrev.despesaacao d
    INNER JOIN elabrev.ppaacao_orcamento a ON a.acaid = d.acaid
    INNER JOIN public.naturezadespesa n ON n.ndpid = d.ndpid
    INNER JOIN public.esfera e ON e.esfcod = a.esfcod
    LEFT JOIN elabrev.planoorcamentario po ON po.ploid = d.ploid
    LEFT JOIN elabrev.subacao sa ON sa.sbaid = d.sbaid
  {$where}
  ORDER BY a.unicod
DML;

    return $sql;
}

function monta_agrupador_ger_ploa()
{
    global $listaAtributos;

    $agrupador = $_REQUEST['agrupador'];
    $agpRelatorio = array(
        'agrupador' => array(),
        'agrupadoColuna' => array_keys($listaAtributos)
    );
    if ($agrupador) {
        foreach ($agrupador as $agp) {
            array_push(
                    $agpRelatorio['agrupador'],
                    array(
                        'campo' => $agp,
                        'label' => $listaAtributos[$agp]
                    )
            );
        }
    }

    $arrAgrupadoresFixos = array(
        'unicod',
        'prgcod',
        'acacod',
        'loccod',
    );
    
    foreach ($arrAgrupadoresFixos as $agpFixo) {
        array_push(
                $agpRelatorio['agrupador'],
                array(
                    'campo' => $agpFixo,
                    'label' => $listaAtributos[$agpFixo]
                )
        );
    }
    return $agpRelatorio;
}

function monta_coluna_ger_ploa()
{
    global $listaAtributos;
    if (!is_array($_REQUEST['coluna'])) {
        $_REQUEST['coluna'] = array();
    }
    $arrColunas = array_merge(array(
        'unicod',
        'prgcod',
        'acacod',
        'loccod',
        'funcod',
        'sfucod',
        'plocodigo',
        'sbacod',
        'acadscprosof',
        'acadscunmsof',
        'acaqtdefisico',
        'ndpcod',
        'esfdsc',
        'iducod',
        'foncod',
        'valor',
        'expansao',
        'valortotal'
    ), $_REQUEST['coluna']);

    $coluna = array();
    foreach (array_keys($listaAtributos) as $col) {
        if (in_array($col, $arrColunas)) {
            $type = 'string';
            switch ($col) {
                case 'valor': // -- no-break
                case 'expansao': // -- no-break
                case 'valortotal': // -- no-break
                case 'acaqtdefisico': // -- no-break
                    $type = 'numeric';
                    break;
            }
            // -- Adicionando as colunas fixas e selecionadas ao array de colunas do relatório
            array_push(
                    $coluna,
                    array(
                        'campo' => $col,
                        'label' => $listaAtributos[$col],
                        'type' => $type
                    )
            );
        }
    }

    return $coluna;
}