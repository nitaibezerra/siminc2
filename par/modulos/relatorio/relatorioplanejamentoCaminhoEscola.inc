<?php

include APPRAIZ . "includes/cabecalho.inc";
include APPRAIZ . 'includes/Agrupador.php';
echo'<br>';

function mascaraglobal($value, $mask) {
	$casasdec = explode(",", $mask);
	// Se possui casas decimais
	if($casasdec[1])
		$value = sprintf("%01.".strlen($casasdec[1])."f", $value);

	$value = str_replace(array("."),array(""),$value);
	if(strlen($mask)>0) {
		$masklen = -1;
		$valuelen = -1;
		while($masklen>=-strlen($mask)) {
			if(-strlen($value)<=$valuelen) {
				if(substr($mask,$masklen,1) == "#") {
						$valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
						$valuelen--;
				} else {
					if(trim(substr($value,$valuelen,1)) != "") {
						$valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
					}
				}
			}
			$masklen--;
		}
	}
	return $valueformatado;
}
?>
<script>
function atualiza( val ){
	if( val != '' ){
		document.getElementById('execucao').submit();
	}
}
</script>
<?php 

monta_titulo( $titulo_modulo, '&nbsp;' );

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

echo "<form method=\"POST\" id=\"execucao\"><p align=center> Tipo de execução:<br>";
$sql = "(SELECT
			'C' as codigo,
			'Convênio' as descricao)
		UNION ALL
		(SELECT
			'T' as codigo,
			'Termo' as descricao)";
$db->monta_combo('prptipoexecucao',$sql,'S', 'Selecione...','atualiza(this.value)','','','','','','',$_REQUEST['prptipoexecucao']);
echo "</p></form>";

if($_REQUEST['prptipoexecucao']){

$sql = "(SELECT 
			p.muncod, 
			m.mundescricao as municipio, 
			ent.entnumcpfcnpj as cnpj, 
			sd.sbdplanointerno as planointerno,
			m.estuf,  
			p.prpnumeroprocesso, 
			e.empnumero as notaempenhoempenho, 
			e.empnumeroconvenio::text ||'/'|| e.empanoconvenio::text as numerodoconvenio,
			trim(to_char(SUM(sd.sbdquantidade),'999G999G999G999G999')) as quantidade,
			'R$ ' || trim(to_char(SUM(valor_total_empenhado),'999G999G999G999G999D99')) as valorempenhado, 
			'R$ ' || trim(to_char(SUM(pagvalorparcela),'999G999G999G999G999D99')) as valorpago,
			p.prpbanco as banco,
			p.prpagencia as agencia, 
			p.nu_conta_corrente as contacorrente
		FROM par.processopar  p
		INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
		INNER JOIN territorios.municipio m ON m.muncod = iu.muncod
		INNER JOIN par.subacaodetalhe sd ON p.prpid = sd.prpid
		INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
		INNER JOIN par.empenhosubacao es ON es.sbaid = s.sbaid and eobstatus = 'A'
		INNER JOIN par.empenho e ON e.empid = es.empid and empstatus = 'A'
		LEFT JOIN par.pagamento pa ON pa.empid = e.empid AND pagstatus = 'A' AND pagsituacaopagamento = '2 - EFETIVADO'
		INNER JOIN entidade.endereco ende ON iu.muncod = ende.muncod
		INNER JOIN entidade.entidade ent ON ende.entid = ent.entid
		INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid AND fen.funid=1
		WHERE s.prgid in (52,81,51,50,83) AND prptipoexecucao = '".$_REQUEST['prptipoexecucao']."'
		GROUP BY p.muncod, m.mundescricao, p.prpnumeroprocesso, e.empnumero, m.estuf, numerodoconvenio, sd.sbdquantidade, cnpj, planointerno,
		p.prpbanco,
			p.prpagencia, 
			p.nu_conta_corrente
		ORDER BY m.estuf, m.mundescricao, prpnumeroprocesso)
		UNION ALL
		(
		SELECT
			' <b>Total:</b> ' as muncod, 
			'<b>' || count(municipio)::varchar || '</b>' as municipio, 
			' - ' as cnpj, 
			' - ' as planointerno,
			' - ' as estuf,  
			' - ' as prpnumeroprocesso, 
			' - ' as notaempenhoempenho, 
			' - ' as numerodoconvenio,
			'<b> ' || trim(to_char(SUM(quantidade),'999G999G999G999G999')) || '</b>' as quantidade,
			'<b> R$' || trim(to_char(SUM(valorempenhado),'999G999G999G999G999D99')) || '</b>' as valorempenhado, 
			'<b> R$ ' || trim(to_char(SUM(valorpago),'999G999G999G999G999D99')) || '</b>' as valorpago,
			'' as banco,
			'' as agencia, 
			'' as contacorrente
		FROM (
			SELECT 
				m.mundescricao as municipio,
				sd.sbdquantidade as quantidade,
				SUM(valor_total_empenhado) as valorempenhado, 
				SUM(pagvalorparcela) as valorpago,
				p.prpbanco as banco,
			p.prpagencia as agencia, 
			p.nu_conta_corrente as contacorrente
			FROM par.processopar  p
			INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
			INNER JOIN territorios.municipio m ON m.muncod = iu.muncod
			INNER JOIN par.subacaodetalhe sd ON p.prpid = sd.prpid
			INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
			INNER JOIN par.empenhosubacao es ON es.sbaid = s.sbaid and eobstatus = 'A'
			INNER JOIN par.empenho e ON e.empid = es.empid and empstatus = 'A'
			LEFT JOIN par.pagamento pa ON pa.empid = e.empid AND pagstatus = 'A' AND pagsituacaopagamento = '2 - EFETIVADO'
			WHERE s.prgid in (52,81,51,50,83) AND prptipoexecucao = '".$_REQUEST['prptipoexecucao']."'
			GROUP BY municipio,sd.sbdquantidade, p.prpbanco,
			p.prpagencia, 
			p.nu_conta_corrente
			) as foo
		)";

$registros = $db->carregar($sql);

if($registros[0]) {
	
	echo "<p align=center><input type=button name=xls value=Excel onclick=\"window.location='par.php?modulo=relatorio/relatorioplanejamentoCaminhoEscola&acao=A&exibirxls=1&prptipoexecucao=".$_REQUEST['prptipoexecucao']."';\"></p>";
	
	echo "<table class=\"tabela\" align=\"center\" bgcolor=\"#f5f5f5\" cellspacing=\"1\" cellpadding=\"3\">";
	
	echo "<tr>";
	
										
	
	
	$arCabecalho = array("Código IBGE",
						 "Município",
						 "CNPJ",
						 "Plano Interno",
						 "UF",
						 "Processo",
						 "Nota empenho",
						 "Convênio",
						 "Número de Veículos",
						 "Valor empenhado",
						 "Valor Pago",
						 "Banco",
						 "Agência",
						 "Conta Corrente"
	);
	
	echo "<td class=\"SubTituloCentro\" width=\"5%\" style=\"font-size:13px;\">Código IBGE</td>
		  <td class=\"SubTituloCentro\" width=\"15%\" style=\"font-size:13px;\">Município</td>
		  <td class=\"SubTituloCentro\" width=\"10%\"  style=\"font-size:13px;\">CNPJ</td>
		  <td class=\"SubTituloCentro\" width=\"10%\" style=\"font-size:13px;\">Plano Interno</td>
		  <td class=\"SubTituloCentro\" width=\"5%\"  style=\"font-size:13px;\">UF</td>
		  <td class=\"SubTituloCentro\" width=\"10%\" style=\"font-size:13px;\">Processo</td>
		  <td class=\"SubTituloCentro\" width=\"10%\" style=\"font-size:13px;\">Nota empenho</td>
		  <td class=\"SubTituloCentro\" width=\"10%\" style=\"font-size:13px;\">Convênio</td>
		  <td class=\"SubTituloCentro\" width=\"5%\" style=\"font-size:13px;\">Número de Veículos</td>
		  <td class=\"SubTituloCentro\" width=\"10%\" style=\"font-size:13px;\">Valor empenhado</td>
		  <td class=\"SubTituloCentro\" width=\"10%\" style=\"font-size:13px;\">Valor Pago</td>
		  <td class=\"SubTituloCentro\" width=\"10%\" style=\"font-size:13px;\">Banco</td>
		  <td class=\"SubTituloCentro\" width=\"10%\" style=\"font-size:13px;\">Agência</td>
		  <td class=\"SubTituloCentro\" width=\"10%\" style=\"font-size:13px;\">Conta Corrente</td>
		  ";
	
	echo "</tr>";
	
	foreach($registros as $k => $reg) {
		
		$cor = '';
		if($k % 2 == 0){
			$cor = 'background-color: white;';
		}
		
		echo "<tr style=\"".$cor."\">";
		
		
		$arDados[] = array('muncod'=>$reg['muncod'],
						   'municipio'=>$reg['municipio'],
						   'cnpj'=>$reg['cnpj'],
						   'planointerno'=>$reg['planointerno'],
						   'estuf'=>$reg['estuf'],
						   'prpnumeroprocesso'=>$reg['prpnumeroprocesso'],
						   'notaempenhoempenho'=>$reg['notaempenhoempenho'],
						   'numerodoconvenio'=>$reg['numerodoconvenio'],
						   'quantidade'=>$reg['quantidade'],
						   'valorempenhado'=>$reg['valorempenhado'],
						   'valorpago'=>$reg['valorpago'],
							'banco'=>$reg['banco'],
							'agencia'=>$reg['agencia'],
							'contacorrente'=>$reg['contacorrente']
		
		);

		
		
		
		echo "<td align=\"center\">".$reg['muncod']."</td>
			  <td>".$reg['municipio']."</td>
			  <td align=\"center\">".$reg['cnpj']."</td>
			  <td align=\"center\">".$reg['planointerno']."</td>
			  <td align=\"center\">".$reg['estuf']."</td>
			  <td align=\"center\">".$reg['prpnumeroprocesso']."</td>
			  <td align=\"center\">".$reg['notaempenhoempenho']."</td>
			  <td align=\"center\">".$reg['numerodoconvenio']."</td>
			  <td align=\"center\">".$reg['quantidade']."</td>
			  <td style=\"color:blue\" align=\"right\" >".$reg['valorempenhado']."</td>
			  <td style=\"color:blue\" align=\"right\" >".$reg['valorpago']."</td>
			  <td align=\"center\">".$reg['banco']."</td>
			  <td align=\"center\">".$reg['agencia']."</td>
			  <td align=\"center\">".$reg['contacorrente']."</td>
			  ";

		
		echo "</tr>";
		
	}
	
	echo "</table>";
}

if($_REQUEST['exibirxls']) {
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatDocente".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados,$arCabecalho,100000,5,'N','100%',$par2);
	exit;
}

}

?>