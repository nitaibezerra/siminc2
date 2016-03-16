<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once "_funcoes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

//$arrPtas = array(2624, 2924, 2897, 2882, 3062, 2580, 2987, 2650, 2832, 2532, 2649, 2745, 2514, 2847, 2778, 3178, 3102, 3184, 2645, 2828, 2828, 3021, 2982, 2962, 2821, 2815, 3201, 3191, 3192, 3198, 3232, 2782, 2638, 3231, 3231, 2969, 3170, 2751, 2884, 3213, 2579, 2958, 2834, 2801, 3166, 2781, 2781, 2611, 2629, 2581, 2583, 3261, 2584, 2949, 2793, 3234, 3257, 2603, 2802, 3293, 2743, 2743, 3039, 2995, 3193);
$sql = "SELECT cnpj, uf, razao_social, proposta_sapent, pta, n_convenio_siconv, limite, programa, situacao, valor_concedente, valor_empenho,
  			valor_proponente, data, processo, nota_empenho, local_atual, data_recebimento, r, ano 
  		FROM emenda.cargaempenho
  		WHERE 
  			nota_empenho <> ''
  			--and pta = '3231'
		order by pta";

$arrDados = $db->carregar( $sql );
//ver($arrDados,d);
foreach ($arrDados as $dados) {
	
	if( is_numeric($dados['pta']) && !empty($dados['nota_empenho']) ){
		$ptrid = $db->pegaUm("select ptrid from emenda.planotrabalho where ptrcod = {$dados['pta']}");
		
		$convenioFNDE = ($dados['n_convenio_siconv'] ? "'".$dados['n_convenio_siconv']."'" : 'null');
		$anoConvenioFNDE = ($dados['ano'] ? "'".$dados['ano']."'" : 'null');
		$data_recebimento = ($dados['data_recebimento'] ? "'".formata_data_sql($dados['data_recebimento'])."'" : 'null');
		$notaEmpenho = ($dados['nota_empenho'] ? "'".$dados['nota_empenho']."'": 'null');
		$seqEmpenho = ($dados['nota_empenho'] ? substr(trim($dados['nota_empenho']), 6) : 'null');
		
		$dados['processo'] = str_replace(".","", $dados['processo']);
		$dados['processo'] = str_replace("/","", $dados['processo']);
		$dados['processo'] = str_replace("-","", $dados['processo']);
		
		$processo = ($dados['processo'] ? "'".$dados['processo']."'": 'null');
		$sql = "UPDATE emenda.planotrabalho SET ptrnumconvenio = {$convenioFNDE}, ptranoconvenio = {$anoConvenioFNDE}, ptrnumprocessoempenho = {$processo} WHERE ptrid = $ptrid";
		$db->executar( $sql );
		
		$sql = "select e.exfid, e.exfvalor, vede.emeid, e.plicod, e.ptres,
					(CASE WHEN vede.gndcod = '3' THEN '3.3.'||vede.mapcod||'.41'
	                 	   WHEN vede.gndcod = '4' THEN '4.4.'||vede.mapcod||'.42'
	                  END) as naturezadesp 
				from emenda.execucaofinanceira e
					inner join emenda.ptemendadetalheentidade pde on pde.pedid = e.pedid
				    inner join emenda.v_emendadetalheentidade vede on vede.edeid = pde.edeid 
				where e.ptrid = $ptrid
					and e.exfvalor = ".retiraPontos($dados['valor_concedente'])." 
					and e.exfstatus = 'A'";
		$arrExec = $db->carregar($sql);
		$arrExec = $arrExec ? $arrExec : array();
				
		foreach ($arrExec as $exec) {
			$sql = "UPDATE emenda.execucaofinanceira SET 
				  exfnumsolempenho = {$seqEmpenho},
				  exfnumempenhooriginal = {$notaEmpenho},
				  exfcodfontesiafi = '0112000000',
				  exfespecieempenho = '01',
				  semid = 4,
				  exfvalor = ".retiraPontos($dados['valor_empenho']).",
				  exfdataalteracao = now(),
				  exfanooriginal = '2012',
				  exfverifsiafi = false,
				  exfverifcadin = false,
				  exfdatainclusao = now(),
				  exfcarga = 'S'
				WHERE
					exfid = {$exec['exfid']}";
			$db->executar( $sql );
			
			$propostaSiconv = ($dados['proposta_sapent'] ? "'".$dados['proposta_sapent']."/2012'" : 'null');
			
			$db->pegaUm("DELETE FROM emenda.sicempenhohistorico WHERE ptrid = $ptrid and exfid = {$exec['exfid']}");
			
			$sql = "INSERT INTO emenda.sicempenhohistorico(ptrid, exfid, anconvenio, anexercicio, cocentrogestaoaprov, coespecieempenho, cofonterecursoaprov,
  						cogestaoemitente, conaturezadespesaaprov, coplanointernoaprov, coprogramafnde, coptresaprov, cosituacaodocsiafi, cotipodocumento,
  						counidadegestoraemitente, dsusernamemovimento, dtmovimento, nucgcfavorecido, nuconvenio, nuconveniosiconv, nuempenhooriginal,
  						nuempenhosiafi, nuidsistema, nuprocesso, nupropostasiconv, nuseqdocsiafi, nuseqmovne, vlempenho) 
					VALUES ($ptrid, {$exec['exfid']}, '{$dados['ano']}', '{$dados['ano']}', '61500000000', '01', '0112000000',
  						'15253', '{$exec['naturezadesp']}', '{$exec['plicod']}', '03', '{$exec['ptres']}', null, null, 
  						'153173', null, {$data_recebimento}, '{$dados['cnpj']}', {$convenioFNDE}, {$convenioFNDE}, {$notaEmpenho}, 
  						null, '02', {$processo}, {$propostaSiconv}, null, {$seqEmpenho}, ".retiraPontos($dados['valor_empenho'])." )";
  			$db->executar($sql);
			
  			$db->pegaUm("DELETE FROM emenda.execfinanceirahistorico WHERE exfid = {$exec['exfid']}");
  			  			
			$sql = "INSERT INTO emenda.execfinanceirahistorico(usucpf, exfid, semid, exfvalor, exfespecieempenho, efhiddataalteracao, exfverifsiafi, exfverifcadin) 
					VALUES ('{$_SESSION['usucpforigem']}', {$exec['exfid']}, 4, ".retiraPontos($dados['valor_concedente']).", '01', now(), false, false)";
			$db->executar( $sql );
			
			$total = $db->pegaUm("SELECT count(speid) FROM emenda.siconvtpaemenda WHERE specodigosiconv = {$propostaSiconv} and ptrid = {$ptrid} and emeid = '{$exec['emeid']}'");
	        if( $total == 0 ){
	        	$sql = "INSERT INTO emenda.siconvtpaemenda(specodigosiconv, ptrid, emeid) 
						VALUES ( {$propostaSiconv}, '{$ptrid}', '{$exec['emeid']}')";
				$db->executar( $sql );
	        }
		}
		$db->commit();
	}
}

?>