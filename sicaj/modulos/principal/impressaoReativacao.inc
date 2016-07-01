<?php

/**
 * verifica se existe o ID do pedido
 */
if (!isset($_GET['id'])) {
    header('location:sicaj.php?modulo=principal/listar&acao=A');
    die();
}
/*
 * Gerando a Chave do Documento
 */
require_once APPRAIZ . 'www/validacaodocumento/gerarchave.php';

/* Verificando se o documento já foi gerado com Chave */
$sql = sprintf("select vldid from sicaj.pedidocdovalidado where pdcid = %d and pcvstatus = 'A' and tipo = 'R'", (int) $_GET['id']);

$vldid = $db->pegaUm($sql);

if ($vldid) {
    baixarDocumentoValidado($vldid);
} else {
    $stmt = sprintf("
        select
            pdcid,
            pdccodacaojudicial,
            pdcvalorexecpassado,
            CASE WHEN pdc.docid IS NOT NULL
                THEN (
                    SELECT TO_CHAR(htddata, 'DD/MM/YYYY')
                    FROM workflow.historicodocumento
                    WHERE docid = (SELECT docid FROM sicaj.pedidocdo WHERE pdcid = pdc.pdcid)
                    ORDER BY htddata DESC LIMIT 1
                    )
                ELSE
                    '-'
            END AS datastatus
        from sicaj.pedidocdo pdc
        where pdcid = %d", (int) $_GET['id']);

    $row = $db->pegaLinha($stmt);

    /**
     * verifica se retornou a row do banco
     */
    if (!$row) {
        header('location:sicaj.php?modulo=principal/listar&acao=A');
        die();
    }

    $data = $row['datastatus'] != '-' ? explode('/',$row['datastatus']) : explode('/',date('d/m/Y'));
    $dataPorExtenso = $data[0] . " de " . pegaMesAno((int) $data[1]) . " de " . $data[2];
    $row['pdcvalorexecpassado'] = number_format($row['pdcvalorexecpassado'], 2, ',', '.');

    $key = md5('brasao');
    $pathImg = APPRAIZ . '/www/imagens/brasao.JPG';
    $brasao = baixarImagem($key, $pathImg);

    $html = <<<htmldesaida
        <div align="center">
            <img src="data:image/jpg;base64,$brasao" />
            <h4>
                MINISTÉRIO DA EDUCAÇÃO<br />
                SECRETARIA EXECUTIVA<br />
                SUBSECRETARIA DE PLANEJAMENTO E ORÇAMENTO
            </h4>
            <p>Esplanada dos Ministérios Bloco "L" - Anexo I - 1º Andar<br /> (61) 2022-8801 - E-mail: spo@mec.gov.br</p>
        </div>

        <p align="right">Brasília,  {$dataPorExtenso}.</p>
        <p>Assunto: <strong>Manifestação de disponibilidade orçamentária.</strong></p>
        <p>Senhor Dirigente de Recursos Humanos.</p>
        <p style="text-align:justify;">
            1.	Com vistas ao cumprimento do Art. 4°, inciso II, da Portaria MP n° 17, de 6 de fevereiro de 2001, este órgão
                setorial informa que, visando o pagamento da decisão judicial de código SICAJ n° {$row['pdccodacaojudicial']}, as despesas decorrentes
                dos respectivos ajustes financeiros, cujo valor referente a exercícios anteriores é de R$ {$row['pdcvalorexecpassado']},
                correrão à conta das dotações constantes da Lei Orçamentária Anual do corrente exercício e suas respectivas
                suplementações relativas às despesas de Pessoal e Encargos Sociais consignadas nessa unidade orçamentária.
        </p>
        <p align="right">
            {{simec_chave}}
        </p>
htmldesaida;

    $vldid = gerarDocumentoValidado($html);

    $sql = "INSERT INTO sicaj.pedidocdovalidado (pdcid, vldid, pcvstatus, tipo) VALUES ('{$_GET['id']}', '{$vldid}', 'A', 'R')";
    $db->executar($sql);
    $db->commit();

    baixarDocumentoValidado($vldid);
}
?>
