<?php
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
            "INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid ) VALUES ( '%s', '%s', %s, '%s', %d )",
            $_REQUEST['titulo'],
            addslashes(addslashes(serialize($_REQUEST))),
            'FALSE',
            $_SESSION['usucpf'],
            $_SESSION['mnuid']
        );
        $db->executar( $sql );
        $db->commit();
    }
    ?>
    <script type="text/javascript">
        alert('Operação realizada com sucesso!');
        location.href = '?modulo=relatorio/termoCooperacao/relGeral&acao=A';
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

$sql = monta_sql();
$dados = $db->carregar($sql);

$agrup = monta_agp();
$col = monta_coluna();
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados);
$r->setTolizadorLinha(false);
$r->setColuna($col);
$r->setTotNivel(false);
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

function monta_agp()
{
    $agrupador = $_REQUEST['agrupador'];
    $agp = array(
        'agrupador' => array(),
        'agrupadoColuna' => array(
            'descricao',
            'unidadegestorap',
            'unidadegestorac',
            'resp_proponente',
            'resp_concedente',
            'esddsc',
            'coodsc',
            'ptres_desc',
            'plicod',
            'ntddsc',
            'valor',
        )
    );

    $arLabels = retornaColunasELabels();

    foreach ($agrupador as $grp) {
        switch ($grp) {
            case 'descricao':
                array_push($agp['agrupador'], array('campo' => 'descricao', 'label' => 'Descrição'));
                continue;
            case 'unidadegestorap':
                array_push($agp['agrupador'], array('campo' => 'unidadegestorap', 'label' => 'UG do Proponente'));
                continue;
            case 'unidadegestorac':
                array_push($agp['agrupador'], array('campo' => 'unidadegestorac', 'label' => 'UG do Cedente'));
                continue;
            case 'resp_proponente':
                array_push($agp['agrupador'], array('campo' => 'resp_proponente', 'label' => 'Representante do Proponente'));
                continue;
            case 'resp_concedente':
                array_push($agp['agrupador'], array('campo' => 'resp_concedente', 'label' => 'Representante do Cedente'));
                continue;
            case 'esddsc':
                array_push($agp['agrupador'], array('campo' => 'esddsc', 'label' => 'Estado da Documentação'));
                continue;
            case 'coodsc':
                array_push($agp['agrupador'], array('campo' => 'coodsc', 'label' => 'Coordenação'));
                continue;
            case 'ntddsc':
                array_push($agp['agrupador'], array('campo' => 'ntddsc', 'label' => $arLabels['ntddsc']));
                continue;
            case 'ptres_desc':
                array_push($agp['agrupador'], array('campo' => 'ptres_desc', 'label' => $arLabels['ptres_desc']));
                continue;
            case 'plicod':
                array_push($agp['agrupador'], array('campo' => 'plicod', 'label' => $arLabels['plicod']));
                continue;
            case 'valor':
                array_push($agp['agrupador'], array('campo' => 'valor', 'label' => $arLabels['valor']));
                continue;
        }
    }
    array_push(
            $agp['agrupador'],
            array(
                "campo" => "descricao",
                "label" => "Descrição"
            )
    );

    array_push(
            $agp['agrupador'],
            array(
                "campo" => "proid",
                "label" => "Previsão Orçamentária"
            )
    );
    return $agp;
}
