<?php
/**
 * Saída em tela do relatório gerencial geral
 */
ini_set("memory_limit", "5120M");
set_time_limit(30000);

// salva os POST na tabela
if ($_REQUEST['salvar'] == 1) {
    $existe_rel = 0;
    $sql = sprintf(
        "select prtid from public.parametros_tela where prtdsc = '%s'",
        $_REQUEST['titulo']
    );
    $existe_rel = $db->pegaUm($sql);
    if ($existe_rel > 0) {
        $sql = sprintf(
            "UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d WHERE prtid = %d",
            $_REQUEST['titulo'],
            addslashes(addslashes(serialize($_REQUEST))),
            $_SESSION['usucpf'],
            $_SESSION['mnuid'],
            $existe_rel
        );
        $db->executar($sql);
        $db->commit();
    } else {
        $sql = sprintf(
            "INSERT INTO public.parametros_tela(prtdsc, prtobj, prtpublico, usucpf, mnuid) VALUES('%s', '%s', %s, '%s', %d)",
            $_REQUEST['titulo'],
            addslashes(addslashes(serialize($_REQUEST))),
            'FALSE',
            $_SESSION['usucpf'],
            $_SESSION['mnuid']
        );
        $db->executar($sql);
        $db->commit();
    }
    ?>
    <script type="text/javascript">
        alert('Operação realizada com sucesso!');
        location.href = 'minc.php?modulo=relatorio/gerenciais/geral&acao=A';
    </script>
    <?php
    die;
}

//coloca periodo e valor do ponto no titulo
if (!$_REQUEST['titulo']) {
    $_REQUEST['titulo'] = "Relatório Geral de Planejamento Orçamentário";
}

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

$sql = <<<DML
SELECT DISTINCT e.entcodent,
                e.entnome,
                CASE WHEN e.tpcid = 1 THEN 'Estadual'
                     ELSE 'Municipal'
                  END AS tipo,
                ext.extdescricao AS eixo,
                m.estuf,
                m.mundescricao,
                CASE WHEN est.esdid IS NOT NULL THEN est.esddsc
                     ELSE 'Não Iniciado'
                  END AS situacao,
                CASE WHEN maedu.mcemodalidadeensino = 'M' THEN 'Médio'
                     ELSE 'Fundamental'
                  END AS mcemodalidadeensino,
                CASE WHEN 'A' = maedu.mceclassificacaoescola THEN 'Aberta'
                     WHEN 'R' = maedu.mceclassificacaoescola THEN 'Rural'
                     WHEN 'U' = maedu.mceclassificacaoescola THEN 'Urbana'
                  END AS classificacao
  FROM minc.mcemaiscultura maedu
    INNER JOIN entidade.entidade e ON e.entid = maedu.entid
    INNER JOIN entidade.endereco endi ON endi.entid = e.entid
    LEFT JOIN territorios.municipio m ON m.muncod = endi.muncod
    LEFT JOIN workflow.documento d ON d.docid = maedu.docid
    LEFT JOIN workflow.estadodocumento est ON est.esdid = d.esdid
    JOIN (SELECT *
            FROM minc.mceplanotrabalho a
              JOIN minc.mceplanoeixo b ON a.ptrid = b.ptrid
              INNER JOIN minc.mceeixotematico met USING(extid)) ext
      ON (ext.mceid = maedu.mceid __W_EIXO__)
  WHERE maedu.mceanoreferencia = '{$_SESSION['exercicio']}'
    AND maedu.mcestatus = 'A'
    __WHERE__
DML;

$arConfig = array(
    array('filtro' => 'situacao', 'campo' => 'est.esdid'),
    array('filtro' => 'tipo', 'campo' => 'e.tpcid'),
    array('filtro' => 'mcemodalidadeensino', 'campo' => 'maedu.mcemodalidadeensino', 'quote' => true),
    array('filtro' => 'classificacao', 'campo' => 'maedu.mceclassificacaoescola', 'quote' => true),
    array('filtro' => 'eixo', 'campo' => 'extid', 'placeholder' => '__W_EIXO__'),
);

monta_sql($sql, $arConfig);
$dados = $db->carregar($sql);

if (1 == $_REQUEST['pesquisa']) { // -- Relatório em tela
    $agrup = monta_agp();
    $col = monta_colunas();
    $r = new montaRelatorio();
    $r->setAgrupador($agrup, $dados);
    $r->setMonstrarTolizadorNivel(true);
    $r->setTolizadorLinha(false);
    $r->setColuna($col);
    $r->setBrasao(true);
    if($_REQUEST['tiporel'] == '2'){
        $r->setEspandir(false);
    }else{
        $r->setEspandir(true);
    }
?>
<html>
    <head>
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    </head>
    <body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
        <!--  Monta o Relatório -->
        <? echo $r->getRelatorio(); ?>
    </body>
</html>
<?php
} else { // -- Relatório XLS

    $arCabecalho = array();
    $colXls = array();

    // -- Montando o cabeçalho do XLS - adicionando as colunas escolhidas ao cabeçalho
    foreach (retornaColunasELabels() as $column => $label) {
        array_push($arCabecalho, $label);
        array_push($colXls, $column);
    }

    header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
    header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
    header("Pragma: no-cache");
    header("Content-type: application/xls; name=SIMEC_MINC_RelatorioGeral_".date("Ymdhis").".xls");
    header("Content-Disposition: attachment; filename=SIMEC_MINC_RelatorioGeral_".date("Ymdhis").".xls");
    header("Content-Description: MID Gera excel");

    $db->monta_lista_tabulado($dados, $arCabecalho, 100000, 5, 'N', '100%', '');
    die;
}
