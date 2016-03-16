<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
//include_once "/var/www/simec/global/config.inc";
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
 
session_start();

if(!$_SESSION['usucpf']) $_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$lotnumportaria 	= $_REQUEST['portaria'];
$lotdataportaria 	= $_REQUEST['data'];
$lote			 	= $_REQUEST['lote'];


$sql = "SELECT distinct 
			tm.estuf, 
		    tm.muncod, 
		    tm.mundescricao,
		    p.pinid,
		    case when p.pinanoseguinte = 'S' then p.pinperiodorepasse2 else p.pinperiodorepasse end as pinperiodorepasse,
		    p.lotid,
		    lp.lotdsc
		FROM
			proinfantil.proinfantil p
		    inner join proinfantil.loteproinfancia lp on lp.lotid = p.lotid
		    inner join obras2.obras oi on oi.obrid = p.obrid and oi.obrstatus = 'A'
		    inner join entidade.entidade AS ee ON oi.entid = ee.entid
		    inner join entidade.endereco AS ed ON oi.endid = ed.endid
		    inner join territorios.municipio AS tm ON ed.muncod = tm.muncod
		WHERE p.lotid = $lote
		order by tm.estuf, tm.mundescricao";
$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

alterarLote($arrDados, $lotnumportaria, $lotdataportaria, $lote);

function alterarLote( $arrDados, $lotnumportaria = '', $lotdataportaria = '', $lote = '' ){	
	global $db;	
	
	$dataPortaria = explode('/', $lotdataportaria);
	$dia = $dataPortaria[0];
	$mes = $dataPortaria[1];
	$ano = $dataPortaria[2];
	$mes = mes_extenso($mes);
	
	$texto = '<p style="text-align: justify;"><strong>PORTARIA N&ordm;&nbsp;&nbsp;'.$lotnumportaria.', &nbsp;&nbsp;&nbsp;&nbsp;DE&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dia.'&nbsp;&nbsp;&nbsp;&nbsp; DE &nbsp;&nbsp;&nbsp;&nbsp;'.$mes.'&nbsp;&nbsp;&nbsp;&nbsp;DE '.$ano.'.</strong></p>
<p style="text-align: justify;">&nbsp;</p>
<p style="padding-left: 300px; text-align: justify;">Autoriza o Fundo Nacional de Desenvolvimento da Educa&ccedil;&atilde;o &ndash; FNDE a realizar a transfer&ecirc;ncia de recursos financeiros aos munic&iacute;pios e ao Distrito Federal para a manuten&ccedil;&atilde;o de novas matr&iacute;culas em novos estabelecimentos p&uacute;blicos de educa&ccedil;&atilde;o infantil, constru&iacute;dos com recursos de programas federais, conforme Resolu&ccedil;&atilde;o CD/FNDE n&ordm; 15 de 16 de maio de 2013.</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;"><strong>O SECRET&Aacute;RIO DE EDUCA&Ccedil;&Atilde;O B&Aacute;SICA</strong>, no uso das atribui&ccedil;&otilde;es, resolve:</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">Art. 1&ordm; Divulgar os munic&iacute;pios e o Distrito Federal que est&atilde;o aptos a receber o pagamento do recurso de apoio &agrave; manuten&ccedil;&atilde;o de novas matr&iacute;culas em novos estabelecimentos p&uacute;blicos de educa&ccedil;&atilde;o infantil, constru&iacute;dos com recursos de programas federais, que estejam em plena atividade e com matr&iacute;culas que ainda n&atilde;o tenham sido contempladas com recursos do Fundo de Manuten&ccedil;&atilde;o e Desenvolvimento da Educa&ccedil;&atilde;o B&aacute;sica e de Valoriza&ccedil;&atilde;o dos Profissionais da Educa&ccedil;&atilde;o (Fundeb), de que trata a Lei n&ordm; 12.499 de 29 de setembro de 2011, e conforme informa&ccedil;&otilde;es declaradas pelos munic&iacute;pios e o Distrito Federal no SIMEC &ndash; M&oacute;dulo E.I. Manuten&ccedil;&atilde;o &ndash; Unidades do Proinf&acirc;ncia.</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">Art. 2&ordm; Autorizar o FNDE/MEC a realizar a transfer&ecirc;ncia de recursos financeiros aos munic&iacute;pios e Distrito Federal para manuten&ccedil;&atilde;o de novas matr&iacute;culas em novos estabelecimentos p&uacute;blicos de educa&ccedil;&atilde;o infantil, conforme destinat&aacute;rios e valores constantes da listagem anexa.</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">Art. 3&ordm; Esta Portaria entra em vigor na data de sua publica&ccedil;&atilde;o</p>
<p>&nbsp;</p>
<p>&nbsp;</p>
<p style="text-align: center;"><strong>MANUEL FERNANDO PALÁCIOS DA CUNHA E MELO</strong></p>
<p style="text-align: center;">Secret&aacute;rio da Educa&ccedil;&atilde;o B&aacute;sica</p>';
				
		$html = $texto.'
		<p style="page-break-before:always"><!-- pagebreak --></p>
		<table align="center" class="listagem" border="1" width="100%" cellSpacing="1" cellPadding=3 >
			<tr>
				<th colspan="8" style="text-align: center;">ANEXO</th>
			</tr>
			<tr>
				<th rowspan="2" width="05%"><b>UF</b></th>
				<th rowspan="2" width="25%" style="text-align: center;"><b>Municípios</b></th>
				<th rowspan="2" width="05%" style="text-align: center;"><b>Código IBGE</b></th>
				<th colspan="4" width="60%" style="text-align: justify;"><b>Quantidade de novas matrículas, declaradas pelos Municípios e o Distrito Federal, em novos estabelecimentos públicos de educação infantil, construídos com recursos de programas
	federais e que estão em plena atividade</b></th>
				<th rowspan="2" width="05%" style="text-align: center;"><b>Valor do Repasse</b></th>
			</tr>
			<tr>
				<th style="text-align: center;"><b>Creche Parcial</b></th>
				<th style="text-align: center;"><b>Creche Integral</b></th>
				<th style="text-align: center;"><b>Pré-Escola Parcial</b></th>
				<th style="text-align: center;"><b>Pré-Escola Integral</b></th>
			</tr>';
			
		$totMatricula = 0;
		$arrLote = array();
		$arrMuncod = array();
		$muncodAnterior	= '';
		$valorTotalGeral = 0;
		
		/*$sql = "delete from proinfantil.loteminutaproinfantil where lotid = $lote";
		$db->executar($sql);*/
	
		foreach ($arrDados as $key => $v) {
			$muncodAtual 	= $v['muncod'];
			$muncodProximo 	= $arrDados[$key+1]['muncod'];
			
			$sql = "SELECT r.resdsc FROM proinfantil.questionario q
						INNER JOIN questionario.resposta r on r.qrpid = q.qrpid
					WHERE r.perid = 1587  and q.pinid = {$v['pinid']}";
		    $dataini = $db->pegaUm($sql);
			$data_atendimento = formata_data_sql($dataini);
			$vlPeriodoMes = 0;
			$totMatricula = (int)0;
			$valorTotal = (int)0;
			
			$qtdChecParc = $db->pegaUm("SELECT coalesce(sum(mds.alaquantidade), 0) FROM proinfantil.mdsalunoatendidopbf mds where mds.titid in (2) and mds.pinid = {$v['pinid']} and mds.timid  = 3");
			if( $qtdChecParc > 0 ){
				$totMatricula =(int)$qtdChecParc;
				$sql = "SELECT vaavalor as valor FROM proinfantil.valoraluno 
					WHERE vaastatus = 'A' AND vaatipo = 'P' 
					AND timid = 3 AND ('{$data_atendimento}' BETWEEN vaadatainicial AND vaadatafinal)";
				$vlPeriodo = $db->pegaUm($sql);
				$vlPeriodoMes = $vlPeriodo ? $vlPeriodo / 12 : 0;				
				$valorTotal = ($totMatricula * ($vlPeriodoMes ? $vlPeriodoMes : 0) * $v['pinperiodorepasse']);
			}
			$valorTotalGeral+=(float)$valorTotal;
			$valorTotal = (int)0;
			
			$qtdChecInte = $db->pegaUm("SELECT coalesce(sum(mds.alaquantidade), 0) FROM proinfantil.mdsalunoatendidopbf mds where mds.titid in (1) and mds.pinid = {$v['pinid']} and mds.timid  = 3");
			if( $qtdChecInte > 0 ){
				$totMatricula =(int)$qtdChecInte;
				$sql = "SELECT vaavalor as valor FROM proinfantil.valoraluno 
					WHERE vaastatus = 'A' AND vaatipo = 'I' 
					AND timid = 3 AND ('{$data_atendimento}' BETWEEN vaadatainicial AND vaadatafinal)";
				$vlPeriodo = $db->pegaUm($sql);
				$vlPeriodoMes = $vlPeriodo ? $vlPeriodo / 12 : 0;			
				$valorTotal = ($totMatricula * ($vlPeriodoMes ? $vlPeriodoMes : 0) * $v['pinperiodorepasse']);
			}
			$valorTotalGeral+=(float)$valorTotal;
			$valorTotal = (int)0;
			
			$qtdPreParc = $db->pegaUm("SELECT coalesce(sum(mds.alaquantidade), 0) FROM proinfantil.mdsalunoatendidopbf mds where mds.titid in (2) and mds.pinid = {$v['pinid']} and mds.timid  = 1");
			if( $qtdPreParc > 0 ){
				$totMatricula =(int)$qtdPreParc;
				$sql = "SELECT vaavalor as valor FROM proinfantil.valoraluno 
					WHERE vaastatus = 'A' AND vaatipo = 'P' 
					AND timid = 1 AND ('{$data_atendimento}' BETWEEN vaadatainicial AND vaadatafinal)";
				$vlPeriodo = $db->pegaUm($sql);
				$vlPeriodoMes = $vlPeriodo ? $vlPeriodo / 12 : 0;				
				$valorTotal+= ($totMatricula * ($vlPeriodoMes ? $vlPeriodoMes : 0) * $v['pinperiodorepasse']);
			}
			$valorTotalGeral+=(float)$valorTotal;
			$valorTotal = (int)0;
			
			$qtdPreInte = $db->pegaUm("SELECT coalesce(sum(mds.alaquantidade), 0) FROM proinfantil.mdsalunoatendidopbf mds where mds.titid in (1) and mds.pinid = {$v['pinid']} and mds.timid  = 1");
			if( $qtdPreInte > 0 ){
				$totMatricula =(int)$qtdPreInte;
				$sql = "SELECT vaavalor as valor FROM proinfantil.valoraluno 
					WHERE vaastatus = 'A' AND vaatipo = 'I' 
					AND timid = 1 AND ('{$data_atendimento}' BETWEEN vaadatainicial AND vaadatafinal)";
				$vlPeriodo = $db->pegaUm($sql);
				$vlPeriodoMes = $vlPeriodo ? $vlPeriodo / 12 : 0;				
				$valorTotal+= ($totMatricula * ($vlPeriodoMes ? $vlPeriodoMes : 0) * $v['pinperiodorepasse']);
			}
			$valorTotalGeral+=(float)$valorTotal;
			$valorTotal = (int)0;
			
			$qtdChecParcTot += $qtdChecParc;
			$qtdChecInteTot += $qtdChecInte;
			$qtdPreParcTot 	+= $qtdPreParc;
			$qtdPreInteTot 	+= $qtdPreInte;
			
			if( $muncodProximo != $muncodAtual ){
				
				$arrLote[]= array(
								'municipio' 	=> $v['mundescricao'],
								'ibge' 			=> $v['muncod'],
								'estuf' 		=> $v['estuf'],
								'qtdChecParc'	=> (int)$qtdChecParcTot,
								'qtdChecInte' 	=> (int)$qtdChecInteTot,
								'qtdPreParc' 	=> (int)$qtdPreParcTot,
								'qtdPreInte' 	=> (int)$qtdPreInteTot,
								'valorTotal' 	=> (float)$valorTotalGeral,
							);
				$qtdChecParcTot 	= 0;
				$qtdChecInteTot 	= 0;
				$qtdPreParcTot 		= 0;
				$qtdPreInteTot		= 0;
				$valorTotalGeral 	= 0;
			}
			$totMatricula = (int)0;
		}
	//ver($arrLote,d);
	
	 foreach ($arrLote as $v) {
		$valorTotal = ($v['valorTotal'] ? number_format($v['valorTotal'],2,",",".") : '0,00');
			
		$html.='
		<tr>
			<td>'.$v['estuf'].'</td>
			<td style="text-align: left;">'.$v['municipio'].'</td>
			<td style="text-align: center;">'.$v['ibge'].'</td>
			<td style="text-align: center;">'.(int)$v['qtdChecParc'].'</td>
			<td style="text-align: center;">'.(int)$v['qtdChecInte'].'</td>
			<td style="text-align: center;">'.(int)$v['qtdPreParc'].'</td>
			<td style="text-align: center;">'.(int)$v['qtdPreInte'].'</td>
			<td style="text-align: right;">'.$valorTotal.'</td>
		</tr>';
		
		$valorTotal = str_replace(".","", $valorTotal);
		$valorTotal = str_replace(",",".", $valorTotal);
		
		/* $sql = "INSERT INTO proinfantil.loteminutaproinfantil(lotid, estuf, muncod, crecheparcial, crecheintegral, preescolaparcial, preescolaintegral, valorrepasse) 
				VALUES ({$lote}, '{$v['estuf']}', '{$v['ibge']}', ".(int)$v['qtdChecParc'].", ".(int)$v['qtdChecInte'].", ".(int)$v['qtdPreParc'].", ".(int)$v['qtdPreInte'].", ".$valorTotal.")";
		$db->executar($sql);
		$db->commit(); */
	} 
	$html.= '</table>';
	
	include_once APPRAIZ . "includes/classes/RequestHttp.class.inc";
	ob_clean();
		
	$nomeArquivo 		= 'minuta_repasse_'.date('Y-m-d').'_lote_'.$lote;
	$diretorio		 	= APPRAIZ . 'arquivos/proinfantil/minutaproinfantil';
	$diretorioArquivo 	= APPRAIZ . 'arquivos/proinfantil/minutaproinfantil/'.$nomeArquivo.'.pdf';
	
	if( !is_dir($diretorio) ){
		mkdir($diretorio, 0777);
	}
	
	$http = new RequestHttp();
	$html = utf8_encode($html);
	$response = $http->toPdf( $html );

	$fp = fopen($diretorioArquivo, "w");
	if ($fp) {
	  stream_set_write_buffer($fp, 0);
	  fwrite($fp, $response);
	  fclose($fp);
	}
	
	$sql = "INSERT INTO public.arquivo (arqnome, arqextensao, arqdescricao, arqtipo, arqtamanho, arqdata, arqhora, usucpf, sisid, arqstatus)
			VALUES( '".$nomeArquivo."',
					'pdf',
					'".$nomeArquivo."',
					'application/pdf',
					'".filesize($diretorioArquivo)."',
					'".date('Y-m-d')."',
					'".date('H:i:s')."',
					'".$_SESSION["usucpf"]."',
					{$_SESSION['sisid']},
					'A') RETURNING arqid";
	
	$arqid = $db->pegaUm($sql);
	
	$sql = "UPDATE proinfantil.loteproinfancia SET
				lotnumportaria = {$lotnumportaria},
				lotminutaportaria = '{$textoSQL}',
				lotdataportaria = '".formata_data_sql($lotdataportaria)."',
				arqid = $arqid
			WHERE lotid = $lote";

	//$sql = "UPDATE proinfantil.loteproinfancia SET arqid = $arqid, lotdsc = 'Lote: '||lotid WHERE lotid = $lote";
	$db->executar($sql);
	$db->commit();
		
	echo "alert('Lote criado com sucesso.');";
}
?>