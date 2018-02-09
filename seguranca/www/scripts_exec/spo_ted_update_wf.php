<?php
set_time_limit(0);

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$_REQUEST['baselogin']  = 'simec_espelho_producao';

// carrega as funções gerais
require_once BASE_PATH_SIMEC . '/global/config.inc';

require_once APPRAIZ . 'includes/classes_simec.inc';
require_once APPRAIZ . 'includes/funcoes.inc';
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '72324414104';
$_SESSION['usucpf'] = '72324414104';
$_SESSION['sisid'] = 2;

$db = new cls_banco();

//Adiciona novo passo para Departamento Juridico do Proponente
$strSQL = "
    INSERT INTO workflow.estadodocumento(
        tpdid,
        esdordem,
        esddsc
    )VALUES(
        97,
        2,
        'Em analise pelo Departamento Jurídico do Proponente'
    ) returning esdid;
";

if ($esdid = $db->pegaUm($strSQL)) {

    $strSQL = "
        INSERT INTO
            workflow.acaoestadodoc (
            esdidorigem,
            esdiddestino,
            aeddscrealizar,
            aedstatus,
            aeddscrealizada,
            esdsncomentario,
            aedcondicao,
            aedobs,
            aedvisivel,
            aedicone,
            aedordem,
            aedpreacao,
            aedposacao,
            aedcodicaonegativa,
            aeddatainicio,
            aeddatafim
        )VALUES(
            631,
            {$esdid},
            'Enviar para análise do departamento jurídico do Proponente',
            'A',
            'Em analise pelo Departamento Jurídico do Proponente',
            FALSE,
            '',
            '',
            true,
            '1.png',
            0,
            '',
            '',
            TRUE,
            NULL,
            NULL
        )
        returning
            workflow.acaoestadodoc.aedid;
    ";

    if ($aedid = $db->pegaUm($strSQL)) {
        $strSQL = "
            INSERT INTO workflow.estadodocumentoperfil (
                pflcod,
                aedid
            )VALUES(
                23,
                {$aedid}
            )
        ";

        $db->pegaUm($strSQL);
        $db->commit();
    }
}

//Adiciona novo passo para Departamento Juridico do Concedente
$strSQL = "
    INSERT INTO workflow.estadodocumento(
        tpdid,
        esdordem,
        esddsc
    )VALUES(
        97,
        6,
        'Em análise pelo Departamento Jurídico do Concedente'
    ) returning esdid
";

if ($esdid = $db->pegaUm($strSQL)) {

    $strSQL = "
        INSERT INTO
            workflow.acaoestadodoc (
            esdidorigem,
            esdiddestino,
            aeddscrealizar,
            aedstatus,
            aeddscrealizada,
            esdsncomentario,
            aedcondicao,
            aedobs,
            aedvisivel,
            aedicone,
            aedordem,
            aedpreacao,
            aedposacao,
            aedcodicaonegativa,
            aeddatainicio,
            aeddatafim
        )VALUES(
            642,
            {$esdid},
            'Enviar para análise do departamento jurídico do Concedente',
            'A',
            'Em analise pelo Departamento Jurídico do Concedente',
            FALSE,
            '',
            '',
            true,
            '1.png',
            0,
            '',
            '',
            TRUE,
            NULL,
            NULL
        )
        returning
            workflow.acaoestadodoc.aedid;
    ";

    if ($aedid = $db->pegaUm($strSQL)) {
        $strSQL = "
            INSERT INTO workflow.estadodocumentoperfil (
                pflcod,
                aedid
            )VALUES(
                23,
                {$aedid}
            )
        ";

        $db->pegaUm($strSQL);
        $db->commit();
    }
}

//Adicionando novo passo RCO em aprovação pela Diretoria
$strSQL = "
    INSERT INTO workflow.estadodocumento(
        tpdid,
        esdordem,
        esddsc
    )VALUES(
        97,
        16,
        'Relatório de cumprimento do objeto aguardando aprovação da Diretoria'
    ) returning esdid
";

if ($esdid = $db->pegaUm($strSQL)) {

    $strSQL = "
        INSERT INTO
            workflow.acaoestadodoc (
            esdidorigem,
            esdiddestino,
            aeddscrealizar,
            aedstatus,
            aeddscrealizada,
            esdsncomentario,
            aedcondicao,
            aedobs,
            aedvisivel,
            aedicone,
            aedordem,
            aedpreacao,
            aedposacao,
            aedcodicaonegativa,
            aeddatainicio,
            aeddatafim
        )VALUES(
            656,
            {$esdid},
            'Enviar relatório de cumprimento do objeto para aprovação da Diretoria',
            'A',
            'Relatório de cumprimento do objeto aguardando aprovação da Diretoria',
            FALSE,
            '',
            '',
            true,
            '1.png',
            0,
            '',
            '',
            TRUE,
            NULL,
            NULL
        )
        returning
            workflow.acaoestadodoc.aedid
    ";

    if ($aedid = $db->pegaUm($strSQL)) {

        $strSQL = "
            INSERT INTO
				workflow.estadodocumentoperfil (
				pflcod,
				aedid
			) VALUES (
				866,
				{$aedid}
			),(
				23,
				{$aedid}
			)
        ";

        $db->executar($strSQL);
        $db->commit();
    }
}

//Exclui a ação de arquivar termo do estado RCO em aprovação pela Coordenação
$strSQL = "
    UPDATE
        workflow.acaoestadodoc
    SET
        aedstatus = 'I'
    WHERE
        aedid = 1654
";
$db->executar($strSQL);
$db->commit();

//Adicionando novo passo RCO em aprovação pelo Secretário
$strSQL = "
    INSERT INTO workflow.estadodocumento(
        tpdid,
        esdordem,
        esddsc
    )VALUES(
        97,
        17,
        'Relatório de cumprimento do objeto aguardando aprovação do Secretário'
    ) returning esdid
";

$strSQLAnt = "
    select esdid from workflow.estadodocumento where tpdid = 97
    and esddsc ilike 'Relatório de cumprimento do objeto aguardando aprovação da Diretoria'
";

$esdid = $db->pegaUm($strSQL);
$esdidorigem = $db->pegaUm($strSQLAnt);

if ($esdid && $esdidorigem) {

    $strSQL = "
        INSERT INTO
            workflow.acaoestadodoc (
            esdidorigem,
            esdiddestino,
            aeddscrealizar,
            aedstatus,
            aeddscrealizada,
            esdsncomentario,
            aedcondicao,
            aedobs,
            aedvisivel,
            aedicone,
            aedordem,
            aedpreacao,
            aedposacao,
            aedcodicaonegativa,
            aeddatainicio,
            aeddatafim
        )VALUES(
            {$esdidorigem},
            {$esdid},
            'Enviar relatório de cumprimento do objeto para aprovação do Secretário',
            'A',
            'Relatório de cumprimento do objeto aguardando aprovação do Secretário',
            FALSE,
            '',
            '',
            true,
            '1.png',
            0,
            '',
            '',
            TRUE,
            NULL,
            NULL
        )
        returning
            workflow.acaoestadodoc.aedid
    ";

    if ($aedid = $db->pegaUm($strSQL)) {

        $strSQL = "
            INSERT INTO
				workflow.estadodocumentoperfil (
				pflcod,
				aedid
			) VALUES (
				860,
				{$aedid}
			),(
				23,
				{$aedid}
			)
        ";

        $db->executar($strSQL);
        $db->commit();
    }
}

//Adicionando novo passo para RCO em aprovação pela DIGAP
$strSQL = "
    INSERT INTO workflow.estadodocumento(
        tpdid,
        esdordem,
        esddsc
    )VALUES(
        97,
        19,
        'Relatório de cumprimento do objeto aguardando aprovação da DIGAP FNDE'
    ) returning esdid
";

$strSQLAnt = "
    select esdid from workflow.estadodocumento where tpdid = 97
    and esddsc ilike 'Relatório de cumprimento do objeto aguardando aprovação do Secretário'
";

$esdid = $db->pegaUm($strSQL);
$esdidorigem = $db->pegaUm($strSQLAnt);

if ($esdid && $esdidorigem) {

    $strSQL = "
        INSERT INTO
            workflow.acaoestadodoc (
            esdidorigem,
            esdiddestino,
            aeddscrealizar,
            aedstatus,
            aeddscrealizada,
            esdsncomentario,
            aedcondicao,
            aedobs,
            aedvisivel,
            aedicone,
            aedordem,
            aedpreacao,
            aedposacao,
            aedcodicaonegativa,
            aeddatainicio,
            aeddatafim
        ) VALUES (
            {$esdidorigem},
            {$esdid},
            'Enviar relatório de cumprimento do objeto para aprovação da DIGAP FNDE',
            'A',
            'Relatório de cumprimento do objeto em aprovação da DIGAP FNDE',
            FALSE,
            '',
            '',
            true,
            '1.png',
            0,
            '',
            '',
            TRUE,
            NULL,
            NULL
        )
        returning
            workflow.acaoestadodoc.aedid
    ";

    if ($aedid = $db->pegaUm($strSQL)) {

        $strSQL = "
            INSERT INTO
				workflow.estadodocumentoperfil (
				pflcod,
				aedid
			) VALUES (
				865,
				{$aedid}
			),(
				23,
				{$aedid}
			)
        ";

        $db->executar($strSQL);
        $db->commit();
    }
}

$strSQL = "
    INSERT INTO workflow.estadodocumento(
        tpdid,
        esdordem,
        esddsc
    )VALUES(
        97,
        18,
        'Relatório de cumprimento do objeto aguardando aprovação do Representante Legal do Concedente'
    ) returning esdid
";

$strSQLAnt = "
    select esdid from workflow.estadodocumento where tpdid = 97
    and esddsc ilike 'Relatório de cumprimento do objeto aguardando aprovação da DIGAP FNDE'
";

$esdid = $db->pegaUm($strSQL);
$esdidorigem = $db->pegaUm($strSQLAnt);

if ($esdid && $esdidorigem) {

    $strSQL = "
        INSERT INTO
            workflow.acaoestadodoc (
            esdidorigem,
            esdiddestino,
            aeddscrealizar,
            aedstatus,
            aeddscrealizada,
            esdsncomentario,
            aedcondicao,
            aedobs,
            aedvisivel,
            aedicone,
            aedordem,
            aedpreacao,
            aedposacao,
            aedcodicaonegativa,
            aeddatainicio,
            aeddatafim
        )VALUES(
            {$esdidorigem},
            {$esdid},
            'Em aprovação pelo Representante Legal do Concedente',
            'A',
            'Aprovação pelo Representante Legal do Concedente',
            FALSE,
            '',
            '',
            true,
            '1.png',
            0,
            '',
            '',
            TRUE,
            NULL,
            NULL
        )
        returning
            workflow.acaoestadodoc.aedid
    ";

    if ($aedid = $db->pegaUm($strSQL)) {

        $strSQL = "
                INSERT INTO
                    workflow.estadodocumentoperfil (
                    pflcod,
                    aedid
                ) VALUES (
                    871,
                    {$aedid}
                ),(
                    23,
                    {$aedid}
                )
            ";

        $db->executar($strSQL);
        $db->commit();
    }
}

//Adicionando acão para o Representante Legal Concedente Finalizar o Termo
$strSQLAnt = "
    select esdid from workflow.estadodocumento where tpdid = 97
    and esddsc ilike 'Relatório de cumprimento do objeto aguardando aprovação do Representante Legal do Concedente'
";

$esdidorigem = $db->pegaUm($strSQLAnt);

if ($esdidorigem) {

    $strSQL = "
        INSERT INTO
            workflow.acaoestadodoc (
            esdidorigem,
            esdiddestino,
            aeddscrealizar,
            aedstatus,
            aeddscrealizada,
            esdsncomentario,
            aedcondicao,
            aedobs,
            aedvisivel,
            aedicone,
            aedordem,
            aedpreacao,
            aedposacao,
            aedcodicaonegativa,
            aeddatainicio,
            aeddatafim
        ) VALUES (
            {$esdidorigem},
            640,
            'Finalizar Termo',
            'A',
            'Termo Finalizado',
            FALSE,
            '',
            '',
            true,
            '1.png',
            0,
            '',
            '',
            TRUE,
            NULL,
            NULL
        )
        returning
            workflow.acaoestadodoc.aedid
    ";

    if ($aedid = $db->pegaUm($strSQL)) {

        $strSQL = "
            INSERT INTO
                workflow.estadodocumentoperfil (
                pflcod,
                aedid
            ) VALUES (
                865,
                {$aedid}
            ),(
                23,
                {$aedid}
            )
        ";

        $db->executar($strSQL);
        $db->commit();
    }
}


$strSQLAnt = "
    select esdid from workflow.estadodocumento where tpdid = 97
    and esddsc ilike 'Relatório de cumprimento do objeto aguardando aprovação da Diretoria'
";

$strSQLPos = "
    select esdid from workflow.estadodocumento where tpdid = 97
    and esddsc ilike 'Relatório de cumprimento do objeto aguardando aprovação do Representante Legal do Concedente'
";


$esdidorigem = $db->pegaUm($strSQLAnt);
$esdiddestino = $db->pegaUm($strSQLPos);
//ver($esdidorigem, $esdiddestino, d);

if ($esdidorigem && $esdiddestino) {

    $strSQL = "
        INSERT INTO
            workflow.acaoestadodoc (
            esdidorigem,
            esdiddestino,
            aeddscrealizar,
            aedstatus,
            aeddscrealizada,
            esdsncomentario,
            aedcondicao,
            aedobs,
            aedvisivel,
            aedicone,
            aedordem,
            aedpreacao,
            aedposacao,
            aedcodicaonegativa,
            aeddatainicio,
            aeddatafim
        ) VALUES (
            {$esdidorigem},
            {$esdiddestino},
            'Enviar para aprovação do Representante Legal do Concedente',
            'A',
            'Aprovado pelo Representante Legal do Concedente',
            FALSE,
            '',
            '',
            true,
            '1.png',
            0,
            '',
            '',
            TRUE,
            NULL,
            NULL
        )
        returning
            workflow.acaoestadodoc.aedid
    ";

    //ver($strSQL);
    $aedid = $db->pegaUm($strSQL);

    if ($aedid) {

        $strSQL = "
            INSERT INTO
                workflow.estadodocumentoperfil (
                pflcod,
                aedid
            ) VALUES (
                860,
                {$aedid}
            ),(
                23,
                {$aedid}
            )
        ";

        //ver($strSQL, d);
        $db->executar($strSQL);
        $db->commit();
    }
}

$db->close();

die('complete!!');