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
$sql = sprintf("select vldid from sicaj.pedidocdovalidado where pdcid = %d and pcvstatus = 'A' and tipo = 'C'", (int) $_GET['id']);

$vldid = $db->pegaUm($sql);

if ($vldid) {
    baixarDocumentoValidado($vldid);
} else {
    $stmt = sprintf("select pdcid,
    						numprocessoadm,
    						pdcnumprocessojudicial,
    						pdcjuizodacao,
    						pdccodobjeto,
    						pdccodacaojudicial,
    						pdcnumbeneficioacao,
    						TO_CHAR(pdcdatainicio, 'DD/MM/YYYY') as pdcdatainicio,
    						pdcvalorcdo,
    						pdcvalorexecatual,
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

    $total_cdo = ($row['pdcvalorexecatual'] + $row['pdcvalorexecpassado']);

    $data = $row['datastatus'] != '-' ? explode('/',$row['datastatus']) : explode('/',date('d/m/Y'));
    $dataPorExtenso = $data[0] . " de " . pegaMesAno((int) $data[1]) . " de " . $data[2];
    $row['pdcvalorexecatual'] = number_format($row['pdcvalorexecatual'], 2, ',', '.');
    $row['pdcvalorexecpassado'] = number_format($row['pdcvalorexecpassado'], 2, ',', '.');
    $total_cdo = number_format($total_cdo, 2, ',', '.');

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

		    <p style="text-align:justify;">1.	Para fins de cumprimento do Art. 4º, inciso II, da Portaria MP nº 17, de 6 de fevereiro de 2001,
		        segue em anexo cópia da tela SICAJ onde este órgão setorial manifestou a existência de previsão orçamentária
		        para as despesas decorrentes do efetivo cumprimento da decisão judicial em epígrafe, nos termos do Art. 5º
		        da referida Portaria, relativo a seguinte ação judicial:</p>

		    <ul>
		        <li>Número do processo administrativo: {$row['numprocessoadm']}</li>
		        <li>Número do processo judicial: {$row['pdcnumprocessojudicial']}</li>
		        <li>Juizo da ação: {$row['pdcjuizodacao']}</li>
		        <li>Código do objeto cadastrado no SICAJ: {$row['pdccodobjeto']}</li>
		        <li>Ação cadastrada no SICAJ: {$row['pdccodacaojudicial']}</li>
		        <li>Numero de Benefíciário da ação: {$row['pdcnumbeneficioacao']}</li>
		        <li>Data do ìnicio da eficácia temporal: {$row['pdcdatainicio']}</li>
		    </ul>

		    <p style="text-align:justify;">2.	As despesas correrão à conta das dotações constantes da Lei Orçamentária Anual, e suas respectivas suplementações,
		        relativas às despesas de Pessoal e Encargos Sociais consignadas nessa unidade orçamentária, e suas respectivas suplementações.</p>
		    <p>3.   Valores</p>

		    <ul>
		        <li>Valor no Exercício Atual: R$ {$row['pdcvalorexecatual']} </li>
		        <li>Valor no Exercício Anterior: R$ {$row['pdcvalorexecpassado']}</li>
		        <li>Valor total do CDO: R$ {$total_cdo}</li>
		    </ul>

    		<p align="right">
		        {{simec_chave}}
    		</p>
htmldesaida;

    $vldid = gerarDocumentoValidado($html);

    $sql = "INSERT INTO sicaj.pedidocdovalidado (pdcid, vldid, pcvstatus) VALUES ('{$_GET['id']}', '{$vldid}', 'A')";
    $db->executar($sql);
    $db->commit();

    baixarDocumentoValidado($vldid);
}
?>
