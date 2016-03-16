<?php

ob_start();

// --------------- Dependências
//include_once APPRAIZ . 'sase/classes/Mapas.class.inc';
include_once APPRAIZ . 'sase/classes/QuestoesPontuaisPar.class.inc';
include APPRAIZ . 'includes/cabecalho.inc';

header("Pragma: public");
header("Expires: 0");
header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
header("Content-Type: application/force-download");
header("Content-Type: application/octet-stream");
header("Content-Type: application/download");
header("Content-Disposition: attachment;filename=relatorio.xls");
header("Content-Transfer-Encoding: binary ");

global $db;

$sql = "
	SELECT
        iu.muncod as ibge,
        iu.mun_estuf AS estuf,
        m.mundescricao AS municipio,
        p.pertitulo AS pergunta,
        ip.itptitulo AS resposta,
        esd.esddsc AS estadoatual         
	FROM
	    par.parquestionario qq
	INNER JOIN questionario.questionarioresposta qr USING (qrpid)
	INNER JOIN par.instrumentounidade iu ON iu.inuid = qq.inuid
	INNER JOIN workflow.documento d ON d.docid = iu.docid
	INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
	LEFT JOIN territorios.municipio m ON m.muncod = iu.muncod
	INNER JOIN questionario.questionario q ON q.queid = qr.queid
	INNER JOIN questionario.pergunta p ON p.queid = q.queid AND p.perid IN ( 1473, 1478, 1483, 1535 ) 
	INNER JOIN questionario.itempergunta ip ON ip.perid = p.perid
	INNER JOIN questionario.resposta r ON r.perid = p.perid AND r.itpid = ip.itpid AND r.qrpid = qr.qrpid
	WHERE
	    iu.itrid = 2
	ORDER BY
	    estuf, municipio, pergunta,resposta
";
$relatorio = $db->carregar( $sql );

ob_clean();

echo "<table>";

// cabecalho
echo "
	<tr>
		<td>IBGE</td>
		<td>Estado</td>
		<td>Municipio</td>
		<td>Pergunta</td>
		<td>Resposta</td>
		<td>Estado Atual</td>
	</tr>
";

// linhas do relatorio
foreach ($relatorio as $chave => $linha)
	echo "
		<tr>
			<td>" . $linha['ibge'] . "</td>
			<td>" . $linha['estuf'] . "</td>
			<td>" . $linha['municipio'] . "</td>
			<td>" . $linha['pergunta'] . "</td>
			<td>" . $linha['resposta'] . "</td>
			<td>" . $linha['estadoatual'] . "</td>
		</tr>
	";

echo "</table>";