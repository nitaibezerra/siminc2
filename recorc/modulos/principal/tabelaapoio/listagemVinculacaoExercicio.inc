<?php

$where = array ();

	$where [] = "vie.exercicio = '{$_SESSION['exercicio']}' and nrc.nrcano = '{$_SESSION['exercicio']}'";

	if (chaveTemValor ( $_REQUEST, 'prfid' )) {
		$where [] .= "vie.prfid = '{$_REQUEST['prfid']}'";
	}
	if (chaveTemValor ( $_REQUEST, 'unicod' )) {
		$where [] .= "uni.unicod = '{$_REQUEST['unicod']}'";
	}
	if (chaveTemValor ( $_REQUEST, 'foncod' )) {
		$where [] .= " fon.foncod = '{$_REQUEST['foncod']}'";
	}
	if (chaveTemValor ( $_REQUEST, 'nrccod' )) {
		$where [] .= " nrc.nrccod = '{$_REQUEST['nrccod']}'";
	}

if (! empty ( $where )) {
	$where = 'WHERE ' . implode ( ' AND ', $where );
} else {
	$where = '';
}

$sql = <<<DML
 SELECT DISTINCT
                vie.vieid as codVinculacao,
				vie.exercicio as exercicio,
				per.prfdsc as periodo,
                uni.unicod || ' - ' || unidsc     AS unidade,
                fon.foncod || ' - ' || fon.fondsc AS fonte,
                nrc.nrccod || ' - ' || nrc.nrcdsc AS natureza
            FROM
                recorc.vinculacaoexercicio vie
            LEFT JOIN
                recorc.captacao cap
            ON
                vie.unicod = cap.unicod
            AND vie.foncod = cap.foncod
            AND vie.nrccod = cap.nrccod
            AND vie.exercicio = cap.exercicio
            LEFT JOIN
            recorc.periodoreferencia per
            ON
            vie.prfid = per.prfid
            JOIN
                public.unidade uni
            ON
                uni.unicod::INTEGER = vie.unicod::INTEGER
            JOIN
                public.fonterecurso fon
            ON
                fon.foncod::INTEGER = vie.foncod::INTEGER
            JOIN
                public.naturezareceita nrc
            ON
                nrc.nrccod::INTEGER = vie.nrccod::INTEGER
            {$where}
            ORDER BY
                uni.unicod || ' - ' || unidsc    ,
                fon.foncod || ' - ' || fon.fondsc
DML;

$listagem = new Simec_Listagem();
$listagem->setCabecalho(array(
'Exercício',
		'Período',
		'Unidade Orçamentária',
		'Fonte',
		'Natureza de Receita' ));
$listagem->setQuery($sql);
$listagem->setAcoes(array(
    'edit' => 'editVinculacaoExercicio',
    'delete' => 'deleteVinculacaoExercico'
));
$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
$listagem->addCallbackDeCampo(array('unidade', 'fonte', 'natureza' ), 'alinhaParaEsquerda');
if (false === $listagem->render()):
    ?>
    <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
        Nenhum registro encontrado
    </div>
<?php endif; ?>

