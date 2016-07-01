<?php

set_time_limit(0);
ini_set("memory_limit", "1024M");

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
            "INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid ) VALUES ( '%s', '%s', %s, '%s', %d )",
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
        location.href = '?modulo=<?= $modulo ?>&acao=A';
    </script>
    <?
    die;
}

// Inclui componente de relatórios
include APPRAIZ . 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

include "funcoes_relatorio_infra.inc";

$sql = query_relatorio_infra();
$agrupador = monta_agp_relatorio_infra();
$coluna = monta_coluna_relatorio_infra();
$dados = $db->carregar($sql);

// Filtro de 'Valor do Contrato divergente do Cronograma'


$rel->setAgrupador($agrupador, $dados);
$rel->setColuna($coluna);
$rel->setTotNivel(true);
$rel->setTotalizador(false);

//dbg($rel->getAgrupar(), 1);


?>
<html>
<head>
    <title> Simec - Sistema Integrado de Monitoramento do Ministério da Educação </title>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
</head>
<body>
<center>
    <!--  Cabeçalho Brasão -->
    <?php echo monta_cabecalho_relatorio('95'); ?>
</center>

<!--  Monta o Relatório -->
<?php echo $rel->getRelatorio(); ?>


</body>
</html>