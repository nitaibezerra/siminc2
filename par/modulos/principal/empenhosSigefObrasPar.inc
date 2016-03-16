<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

require_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";

if( $_REQUEST['sequencial'] ){
	consultarEmpenhoSIGEF( $_REQUEST['sequencial'] );
	die;
}

/*$sql = "select distinct d.nu_seq_ne from par.empenhodivergentesigef d";
$arrNu_seq_ne = $db->carregarColuna($sql);

foreach ($arrNu_seq_ne as $nu_seq_ne) {
	consultarEmpenhoSIGEF($nu_seq_ne);
}
die;*/

/*
$arrSEQ = array(209480, 209504, 209493, 209481);
consultarEmpenhoSIGEF(5046180);
die;*/

if( $_REQUEST['requisicao'] == 'insereEmpenhoPar' ){
	$nu_seq_ne 			= $_POST['nu_seq_ne'];
	$numero_documento 	= $_POST['numero_documento'];
	$arrChk 			= $_POST['chk'];
	$arrPercent			= $_POST['percent'];
	$arrValorSub		= $_POST['valorsub'];
	
	$arrRetorno = consultarEmpenhoSIGEF( $nu_seq_ne );
	
	if( is_array($arrChk) ){
		foreach ($arrChk as $sbdid) {
			$percent 	= $arrPercent[$sbdid];
			$valorsub	= $arrValorSub[$sbdid];
		}		
	}
}

if( $_REQUEST['requisicao'] == 'mostraSubacaoEmpenhoPar' ){
	
	$nu_seq_ne 			= $_POST['nu_seq_ne'];
	$numero_documento 	= $_POST['numero_documento'];
	$processo			= $_POST['processo'];
	$nu_cnpj 			= $_POST['nu_cnpj'];	
	$valor_ne 			= $_POST['valor_ne'];
	$co_especie_empenho	= $_POST['co_especie_empenho'];
	
	$arEntidade = $db->pegaLinha("select i.iuenome, i.itrid, iu.muncod, iu.estuf 
								from par.instrumentounidadeentidade i 
									inner join par.instrumentounidade iu on iu.inuid = i.inuid
								where iuecnpj = '$nu_cnpj'");
	
	if( $arEntidade['itrid'] == 2 ){
		$descricao = $db->pegaUm("select mundescricao||'/'||estuf from territorios.municipio where muncod = '{$arEntidade['muncod']}'");
		$label = 'Município:';
	} else {
		$descricao = $db->pegaUm("select estdescricao||'/'||estuf from territorios.estado where estuf = '{$arEntidade['estuf']}'");
		$label = 'Estado:';
	}
	?>		
		<table class="tabela" style="width: 100%" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0" align="center">
			<tr>
				<td width="50%">
				<table width="100%" cellSpacing="1" border="0" cellPadding="3" align="center">
					<tr>
						<th colspan="4">Dados do Empenho SIGEF</th>
					</tr>
					<tr>
						<td class="subtitulodireita" width="25%">Entidade:</td>
						<td width="25%"><?=$arEntidade['iuenome'] ?></td>
						<td class="subtitulodireita" width="25%"><?=$label; ?></td>
						<td width="25%"><?=$descricao ?></td>
					</tr>
					<tr>
						<td class="subtitulodireita" width="25%">Processo:</td>
						<td width="25%"><?=$processo ?></td>
						<td class="subtitulodireita" width="25%">CNPJ:</td>
						<td width="25%"><?=$nu_cnpj ?></td>
					</tr>
					<tr>
						<td class="subtitulodireita" width="25%">Sequencial NE Atual:</td>
						<td width="25%"><?=$nu_seq_ne ?></td>
						<td class="subtitulodireita" width="25%">Nº do Empenho:</td>
						<td width="25%"><?=$numero_documento ?></td>
					</tr>
					<tr>
						<td class="subtitulodireita" width="25%">Valor do Empenho:</td>
						<td width="25%"><?=number_format($valor_ne, 2, ',', '.'); ?></td>
						<td class="subtitulodireita" width="25%">Especie:</td>
						<td width="25%"><?=$co_especie_empenho ?></td>
					</tr>
					<tr>
						<th colspan="4">Dados do Empenho PAR</th>
					</tr>
					<tr>
						<td colspan="4">
						<table class="tabela" style="width: 100%" cellSpacing="1" cellPadding="3" align="center">
						<tr>
							<td width="50%" valign="top">
							<?
								$sql = "select  
											e.empnumero, 
											e.empprotocolo, 
											e.empnumerooriginalpai,
											e.empcodigoespecie, 
											e.empsituacao, 
											vve.vrlempenhocancelado as empvalorempenho
										from par.empenho e 
											inner join par.v_vrlempenhocancelado vve on vve.empid = e.empid and empstatus = 'A'
										where 
											e.empnumeroprocesso = '{$processo}'
											and e.empcnpj = '{$nu_cnpj}'
											and empatualizadosigef = 'S'
											and empcodigoespecie not in ('03', '13', '02', '04')
										order by
											e.empprotocolo, e.empnumero";
								
								$cabecalho = array('Nº Empenho', 'Sequencial', 'Empenho Original', 'Especie', 'Situação', 'Valor');
								$db->monta_lista($sql, $cabecalho, 100000, 1, 'N', 'center', '', 'formempenho');
								?>
							</td>
						</tr>
						</table>
						</td>
					</tr>
					<tr>
						<th colspan="4">Dados da Subação PAR</th>
					</tr>
					<tr>
						<td colspan="4">
						<table class="tabela" style="width: 100%" cellSpacing="1" cellPadding="3" align="center">
						<tr>
							<td width="50%" valign="top">
							<?							
								$sql = "select
											acoes,
											codigo,
										    subacao,
										    sbdano,
										    valorsubacao,
										    '<input type=\"text\" id=\"percent[]\" disabled value=\"\" name=\"percent['||sbdid||']\" size=\"5\" maxlength=\"3\" onblur=\"this.value=mascaraglobal(\'[#]\',this.value); calculaEmpenho(\'P\', '||sbdid||');\" 
												onkeyup=\"this.value=mascaraglobal(\'[#]\',this.value); calculaEmpenho(\'P\', '||sbdid||');\" class=\"disabled\" onfocus=\"this.select();\">' as porcentagem,
										    '<input type=\"text\" id=\"valorsub[]\" disabled value=\"\" name=\"valorsub['||sbdid||']\" size=\"20\" onblur=\"this.value=mascaraglobal(\'[.###],##\',this.value); calculaEmpenho(\'V\', '||sbdid||');\" 
												onkeyup=\"this.value=mascaraglobal(\'[.###],##\',this.value); calculaEmpenho( \'V\', '||sbdid||');\" class=\"disabled\" onfocus=\"this.select();\">
												<input type=\"hidden\" id=\"valorsubacao\" name=\"valorsubacao['||sbdid||']\" value=\"'||valorsubacao||'\">
												' as valor
										from (
										SELECT
											'<center><input type=\"checkbox\" onclick=\"marcarChk(this, '||sd.sbdid||')\"; name=\"chk[]\" id=\"chk\" value=\"'||sd.sbdid||'\"></center>' as acoes,
										    d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || s.sbaordem||' ' as codigo,
										    s.sbadsc as subacao,
										    sd.sbdid,
										    sd.sbdano,
										    (SELECT cast(par.recuperaValorValidadosSubacaoPorAno(s.sbaid , sd.sbdano) as numeric(20,2) ) ) AS valorsubacao
										FROM 
										    par.processopar p
										    inner join par.processoparcomposicao pc on pc.prpid = p.prpid and pc.ppcstatus = 'A'
										    inner join par.subacaodetalhe sd on sd.sbdid = pc.sbdid
										    inner join par.subacao s on s.sbaid = sd.sbaid and s.sbastatus = 'A'
										    inner join par.acao a on a.aciid = s.aciid
										    inner join par.pontuacao pon on pon.ptoid = a.ptoid
										    inner join par.criterio c on c.crtid = pon.crtid
										    inner join par.indicador i on i.indid = c.indid
										    inner join par.area are on are.areid = i.areid
										    inner join par.dimensao d on d.dimid = are.dimid
										WHERE
										    p.prpnumeroprocesso = '$processo'
										    and p.prpcnpj = '$nu_cnpj'
										    and p.prpstatus = 'A'
										order by 
										    d.dimcod, are.arecod, i.indcod, s.sbaordem
										) as foo";
								//ver(simec_htmlentities($sql),d);
								$cabecalho = array('Ação', 'Localização', 'Subação', 'Ano', 'Valor da Subação', '% Empenhada', 'Valor Empenhado');
								$db->monta_lista($sql, $cabecalho, 100000, 1, 'N', 'center', '', 'formempenhopar');
								?>
							</td>
						</tr>
						</table>
						</td>
					</tr>
				</table>
				</td>
			</tr>
		</table>
	<?
	exit();
}
if( $_REQUEST['requisicao'] == 'mostraEmpenhoPar' ){
	
	$nu_seq_ne 			= $_POST['nu_seq_ne'];
	$numero_documento 	= $_POST['numero_documento'];
	$processo			= $_POST['processo'];
	$nu_cnpj 			= $_POST['nu_cnpj'];	
	$valor_ne 			= $_POST['valor_ne'];
	$co_especie_empenho	= $_POST['co_especie_empenho'];
	
	$arEntidade = $db->pegaLinha("select i.iuenome, i.itrid, iu.muncod, iu.estuf 
								from par.instrumentounidadeentidade i 
									inner join par.instrumentounidade iu on iu.inuid = i.inuid
								where iuecnpj = '$nu_cnpj'");
	
	if( $arEntidade['itrid'] == 2 ){
		$descricao = $db->pegaUm("select mundescricao||'/'||estuf from territorios.municipio where muncod = '{$arEntidade['muncod']}'");
		$label = 'Município:';
	} else {
		$descricao = $db->pegaUm("select estdescricao||'/'||estuf from territorios.estado where estuf = '{$arEntidade['estuf']}'");
		$label = 'Estado:';
	}
	?>		
		<table class="tabela" style="width: 100%" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0" align="center">
			<tr>
				<td width="50%">
				<table width="100%" cellSpacing="1" border="0" cellPadding="3" align="center">
					<tr>
						<th colspan="4">Dados do Empenho SIGEF</th>
					</tr>
					<tr>
						<td class="subtitulodireita" width="25%">Entidade:</td>
						<td width="25%"><?=$arEntidade['iuenome'] ?></td>
						<td class="subtitulodireita" width="25%"><?=$label; ?></td>
						<td width="25%"><?=$descricao ?></td>
					</tr>
					<tr>
						<td class="subtitulodireita" width="25%">Processo:</td>
						<td width="25%"><?=$processo ?></td>
						<td class="subtitulodireita" width="25%">CNPJ:</td>
						<td width="25%"><?=$nu_cnpj ?></td>
					</tr>
					<tr>
						<td class="subtitulodireita" width="25%">Sequencial NE Atual:</td>
						<td width="25%"><?=$nu_seq_ne ?></td>
						<td class="subtitulodireita" width="25%">Nº do Empenho:</td>
						<td width="25%"><?=$numero_documento ?></td>
					</tr>
					<tr>
						<td class="subtitulodireita" width="25%">Valor do Empenho:</td>
						<td width="25%"><?=number_format($valor_ne, 2, ',', '.'); ?></td>
						<td class="subtitulodireita" width="25%">Especie:</td>
						<td width="25%"><?=$co_especie_empenho ?></td>
					</tr>
					<tr>
						<th colspan="4">Dados do Empenho PAR</th>
					</tr>
					<tr>
						<td colspan="4">
						<table class="tabela" style="width: 100%" cellSpacing="1" cellPadding="3" align="center">
						<tr>
							<td width="50%" valign="top">
							<?
								$sql = "select
											'<center><input type=\"radio\" name=\"empprotocolo\" id=\"empprotocolo\" value=\"'||e.empprotocolo||'\"></center>' as acoes,  
											e.empnumero, 
											e.empprotocolo, 
											e.empcodigoespecie, 
											e.empsituacao, 
											vve.vrlempenhocancelado as empvalorempenho
										from par.empenho e 
											inner join par.v_vrlempenhocancelado vve on vve.empid = e.empid and empstatus = 'A'
										where 
											e.empnumeroprocesso = '{$_POST['processo']}'
											and e.empcnpj = '{$nu_cnpj}'
											and empcodigoespecie not in ('03', '13', '02', '04')
											and empatualizadosigef = 'N'
										order by
											e.empprotocolo, e.empnumero";
								
								$cabecalho = array('Ações', 'Nº Empenho', 'Sequencial', 'Especie', 'Situação', 'Valor');
								$db->monta_lista($sql, $cabecalho, 100000, 1, 'N', 'center', '', 'formempenho');
								?>
							</td>
						</tr>
						</table>
						</td>
					</tr>
				</table>
				</td>
			</tr>
		</table>
	<?
	exit();
}

if( $_REQUEST['requisicao'] == 'atualizarEmpenho' ){
	
	$nu_seq_ne 			= $_POST['nu_seq_ne'];
	$numero_documento 	= $_POST['numero_documento'];
	$empprotocolo	 	= $_POST['empprotocolo'];
	
	if( $nu_seq_ne ) $filtro = " and nu_seq_ne = '{$nu_seq_ne}' ";
	if( $numero_documento ) $filtro = " and numero_documento = '{$numero_documento}' ";
	
	$sql = "SELECT nu_seq_ne, nu_empenho_original, nu_seq_doc_siafi_original, an_exercicio_original, situacao_documento, data_documento, valor_ne, processo, nu_cnpj,
			  numero_documento, ds_problema, co_especie_empenho, valor_total_empenhado, valor_saldo_pagamento, co_programa_fnde, ano_convenio_original,
			  nu_convenio_original, unidade_gestora_responsavel, co_diretoria, status
		FROM 
			  par.empenhodivergentesigef
		where 
			1=1
			$filtro			
		order by processo";
	$arrEmpenho = $db->pegaLinha($sql);
	
	$empnumerooriginal 	= substr($numero_documento, 6);
	$empanooriginal 	= substr($numero_documento, 0, 4);
	
	$sql = "UPDATE par.empenho SET
				empnumerooriginal 			= '{$empnumerooriginal}',
  				empanooriginal 				= '{$empanooriginal}',
  				empcodigoespecie 			= '{$arrEmpenho['co_especie_empenho']}',
  				empsituacao 				= '{$arrEmpenho['situacao_documento']}',
  				empprotocolo 				= '{$arrEmpenho['nu_seq_ne']}',
  				empnumero 					= '{$arrEmpenho['numero_documento']}',
  				empvalorempenho 			= '{$arrEmpenho['valor_ne']}',
  				ds_problema 				= '{$arrEmpenho['ds_problema']}',
  				valor_total_empenhado 		= '{$arrEmpenho['valor_total_empenhado']}',
  				valor_saldo_pagamento 		= '{$arrEmpenho['valor_saldo_pagamento']}',
  				empdata 					= '{$arrEmpenho['data_documento']}',
  				co_diretoria 				= '{$arrEmpenho['co_diretoria']}',
  				nu_seq_doc_siafi_original	= '{$arrEmpenho['nu_seq_doc_siafi_original']}',
  				empnumerooriginalpai 		= '{$arrEmpenho['nu_empenho_original']}',
  				empatualizadosigef 			= 'S'
  			WHERE empprotocolo = $empprotocolo";
	ver($sql,d);
	
	$sql = "UPDATE par.empenhodivergentesigef SET status = 'A' WHERE processo = '{$arrEmpenho['processo']}' $filtro";
}

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );

/*$sql = "SELECT nu_seq_ne, nu_empenho_original, nu_seq_doc_siafi_original, an_exercicio_original, situacao_documento, data_documento, valor_ne, processo, nu_cnpj,
			  numero_documento, ds_problema, co_especie_empenho, valor_total_empenhado, valor_saldo_pagamento, co_programa_fnde, ano_convenio_original,
			  nu_convenio_original, unidade_gestora_responsavel, co_diretoria
		FROM 
			  par.empenhodivergentesigef
		--where processo in ('23400001168201277', '23400001486201238', '23400001598201299')
		order by processo";

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

$arrDados = array();
$arProcesso = array();
foreach ($arrDados as $proc) {
			
		if( $proc['numero_documento'] ){
			$filtro = " and empnumero = '{$proc['numero_documento']}'";
			$filtro1 = " and numero_documento = '{$proc['numero_documento']}'";
		} else {
			if( $proc['nu_seq_ne'] ){
				$filtro = " and empprotocolo = '{$proc['nu_seq_ne']}'";
				$filtro1 = " and nu_seq_ne = '{$proc['nu_seq_ne']}'";
			}
		}
		
		$sql = "SELECT empid, empcnpj, empnumerooriginal, empanooriginal, empnumeroprocesso, empcodigoespecie, empcodigopi, empcodigoesfera, empcodigoptres, empfonterecurso,
					empcodigonatdespesa, empcentrogestaosolic, empanoconvenio, empnumeroconvenio, empcodigoobs, empcodigotipo, empdescricao, empgestaoeminente, empunidgestoraeminente,
	  				empprogramafnde, empnumerosistema, empsituacao, usucpf, empprotocolo, empnumero, empvalorempenho, ds_problema, valor_total_empenhado, valor_saldo_pagamento,
	  				empdata, tp_especializacao, co_diretoria, empnumerooriginalpai, nu_seq_doc_siafi_original
				FROM par.empenho where empnumeroprocesso = '{$proc['processo']}' and empnumero = '{$proc['numero_documento']}'";
		$arEmpenho = $db->pegaLinha($sql);
		
		$arrPrograma = explode('-', $proc['co_programa_fnde']);
		
		$nu_empenho_original = (empty($proc['nu_empenho_original']) ? "'".substr($proc['numero_documento'], 6)."'" : "'".$proc['nu_empenho_original']."'");
		$an_exercicio_original = (empty($proc['an_exercicio_original']) ? "'".substr($proc['numero_documento'], 0, 4)."'" : "'".$proc['an_exercicio_original']."'");
				
		$nu_cnpj 						= ($proc['nu_cnpj'] ? "'".$proc['nu_cnpj']."'" : 'null');
		$nu_empenho_originalpai			= ($proc['nu_empenho_original'] ? "'".$proc['nu_empenho_original']."'" : 'null');
		$co_especie_empenho 			= ($proc['co_especie_empenho'] ? "'".$proc['co_especie_empenho']."'" : 'null');
		$ano_convenio_original 			= ($proc['ano_convenio_original'] ? "'".$proc['ano_convenio_original']."'" : 'null');
		$nu_convenio_original 			= ($proc['nu_convenio_original'] ? "'".$proc['nu_convenio_original']."'" : 'null');
		$unidade_gestora_responsavel	= ($proc['unidade_gestora_responsavel'] ? "'".$proc['unidade_gestora_responsavel']."'" : 'null');
		$situacao_documento 			= ($proc['situacao_documento'] ? "'".$proc['situacao_documento']."'" : 'null');
		$nu_seq_ne 						= ($proc['nu_seq_ne'] ? "'".$proc['nu_seq_ne']."'" : 'null');
		$numero_documento 				= ($proc['numero_documento'] ? "'".$proc['numero_documento']."'" : 'null');
		$valor_ne 						= ($proc['valor_ne'] ? "'".$proc['valor_ne']."'" : 'null');
		$ds_problema 					= ($proc['ds_problema'] ? "'".$proc['ds_problema']."'" : 'null');
		$valor_total_empenhado 			= ($proc['valor_total_empenhado'] ? "'".$proc['valor_total_empenhado']."'" : 'null');
		$valor_saldo_pagamento 			= ($proc['valor_saldo_pagamento'] ? "'".$proc['valor_saldo_pagamento']."'" : 'null');
		$data_documento 				= ($proc['data_documento'] ? "'".$proc['data_documento']."'" : 'null');
		$co_diretoria 					= ($proc['co_diretoria'] ? "'".$proc['co_diretoria']."'" : 'null');
		$nu_seq_doc_siafi_original 		= ($proc['nu_seq_doc_siafi_original'] ? "'".$proc['nu_seq_doc_siafi_original']."'" : 'null');
		
		if( $arEmpenho ){
			$sql = "UPDATE par.empenho SET 
						  empcnpj = ".$nu_cnpj.",
						  empnumerooriginal = ".$nu_empenho_original.",
						  empanooriginal = ".$an_exercicio_original.",
						  empcodigoespecie = ".$co_especie_empenho.",
						  empanoconvenio = ".$ano_convenio_original.",
						  empnumeroconvenio = ".$nu_convenio_original.",
						  empunidgestoraeminente = ".$unidade_gestora_responsavel.",
						  empsituacao = ".$situacao_documento.",
						  empprotocolo = ".$nu_seq_ne.",
						  empnumero = ".$numero_documento.",
						  --empvalorempenho = ".$valor_ne.",
						  ds_problema = ".$ds_problema.",
						  valor_total_empenhado = ".$valor_total_empenhado.",
						  valor_saldo_pagamento = ".$valor_saldo_pagamento.",
						  empdata = ".$data_documento.",
						  co_diretoria = ".$co_diretoria.",
						  empnumerooriginalpai = ".$nu_empenho_originalpai.",
						  nu_seq_doc_siafi_original = ".$nu_seq_doc_siafi_original.",
						  empatualizadosigef = 'S'
					WHERE 
						  empnumeroprocesso = '{$proc['processo']}'
						  $filtro";
						  
			$db->executar($sql);
		} else {
			if( trim($proc['co_especie_empenho']) == '03' ){
				
				$sql = "SELECT empid, empcnpj, empnumerooriginal, empanooriginal, empnumeroprocesso, empcodigoespecie, empcodigopi, empcodigoesfera, empcodigoptres, empfonterecurso,
							empcodigonatdespesa, empcentrogestaosolic, empanoconvenio, empnumeroconvenio, empcodigoobs, empcodigotipo, empdescricao, empgestaoeminente, empunidgestoraeminente,
			  				empprogramafnde, empnumerosistema, empsituacao, usucpf, empprotocolo, empnumero, empvalorempenho, ds_problema, valor_total_empenhado, valor_saldo_pagamento,
			  				empdata, tp_especializacao, co_diretoria, empnumerooriginalpai, nu_seq_doc_siafi_original
						FROM par.empenho where empnumeroprocesso = '{$proc['processo']}' and empcnpj = '{$proc['nu_cnpj']}'";
				$arrEmpenhoExi = $db->pegaLinha($sql);
				 
				$sql = "INSERT INTO par.empenho(empcnpj, empnumerooriginal, empanooriginal, empnumeroprocesso, empcodigoespecie, empanoconvenio, empnumeroconvenio,
							empunidgestoraeminente, empprogramafnde, empsituacao, usucpf, empprotocolo, empnumero, empvalorempenho, ds_problema, valor_total_empenhado,
  							valor_saldo_pagamento, empdata, co_diretoria, empnumerooriginalpai, nu_seq_doc_siafi_original, empcodigopi, empcodigoptres, empfonterecurso, 
  							empcodigonatdespesa, empcodigoesfera, empcentrogestaosolic, empcodigoobs, empcodigotipo, empdescricao, empgestaoeminente, empnumerosistema, empatualizadosigef) 
						VALUES ({$nu_cnpj}, {$nu_empenho_original}, {$an_exercicio_original}, '{$proc['processo']}', {$co_especie_empenho}, {$ano_convenio_original}, {$nu_convenio_original},
							{$unidade_gestora_responsavel}, '".trim($arrPrograma[0])."', {$situacao_documento}, '', {$nu_seq_ne}, {$numero_documento}, 
							{$valor_ne}, {$ds_problema}, {$valor_total_empenhado}, {$valor_saldo_pagamento}, {$data_documento}, {$co_diretoria}, {$nu_empenho_originalpai}, {$nu_seq_doc_siafi_original}, 
							'{$arrEmpenhoExi['empcodigopi']}', '{$arrEmpenhoExi['empcodigoptres']}', '{$arrEmpenhoExi['empfonterecurso']}', '{$arrEmpenhoExi['empcodigonatdespesa']}', 
							'{$arrEmpenhoExi['empcodigoesfera']}', '{$arrEmpenhoExi['empcentrogestaosolic']}', '{$arrEmpenhoExi['empcodigoobs']}', '{$arrEmpenhoExi['empcodigotipo']}',
							'{$arrEmpenhoExi['empdescricao']}', '{$arrEmpenhoExi['empgestaoeminente']}', '{$arrEmpenhoExi['empnumerosistema']}', 'S') returning empid";
				$empid = $db->pegaUm($sql);
				
				$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, empsituacao, co_especie_empenho, ds_problema, valor_total_empenhado, valor_saldo_pagamento)
		    			VALUES ('', $empid, NOW(), $situacao_documento, {$co_especie_empenho}, {$ds_problema}, {$valor_total_empenhado}, {$valor_saldo_pagamento})";
				$db->executar($sql);
			} else {
				$sql = "UPDATE par.empenhodivergentesigef SET status = 'I' WHERE processo = '{$proc['processo']}' $filtro1";
				$db->executar($sql);
			}
		}
	}
//}
//$db->commit();
*/

$acoesT = "'<center><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"confirmarEmpenho(\''||numero_documento||'\',\''|| nu_seq_ne ||'\',\''|| processo ||'\',\''|| nu_cnpj ||'\',\''|| valor_ne ||'\',\''|| co_especie_empenho ||'\');\" border=\"0\"></center>'";
$acoes = "'<center><img src=\"../imagens/gif_inclui.gif\" style=\"cursor:pointer;\" onclick=\"insereEmpenho(\''||numero_documento||'\',\''|| nu_seq_ne ||'\',\''|| processo ||'\',\''|| nu_cnpj ||'\',\''|| valor_ne ||'\',\''|| co_especie_empenho ||'\');\" border=\"0\"></center>'";
        		
        		$sql = "SELECT
        					case when (select count(empid) from par.empenho where empnumeroprocesso = processo and empcnpj = nu_cnpj and empatualizadosigef = 'N' and empstatus = 'A') > 0 then $acoesT
        						else $acoes 
        					end as acoes,
        					processo,
        					nu_cnpj, 
						    nu_seq_ne as sequencialne,
						    numero_documento as numeroempenho,
						    nu_empenho_original,
						    (select iuenome from par.instrumentounidadeentidade where iuecnpj = nu_cnpj and iuenome is not null  limit 1) as entidade,
						    situacao_documento as situacao,
						    co_especie_empenho as especie,
						    cast(valor_ne as numeric(20,2)) as valor
						FROM 
						    par.empenhodivergentesigef
						WHERE
							sistema = 'ObrasPAR'
	    				order by
	    					processo, 
	    					numero_documento,
	    					nu_seq_ne,
	    					valor_ne";

$arrProcesso = $db->carregar($sql);
$arrProcesso = $arrProcesso ? $arrProcesso : array();

$cabecalho = array("Ações", "Processo", "CNPJ", "Entidade", "Sequencial NE", "Nº Empenho", "Empenho Original", "Situação", "Especie", "Valor");

?>
<table width="95%" align="center" border="0" cellspacing="1" cellpadding="3"  class="listagem" bgcolor="#f5f5f5">
	<thead>
		<tr>
		<?foreach ($cabecalho as $cab){ ?>
			<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><?=$cab; ?></td>
		<?} ?>
		</tr>
	</thead>
	<tbody>
<?

if( $arrProcesso ){
$arrProc = array();
$cor = "";
foreach ($arrProcesso as $key => $v) {
	
	if( !in_array($v['processo'], $arrProc) ){
		$key % 2 ? $cor = "#dedfde" : $cor = "";
		
		$sql = "SELECT distinct
			  	count(processo)
			FROM 
			  	par.empenhodivergentesigef
			WHERE
				processo = '{$v['processo']}'
			  	and nu_cnpj = '{$v['nu_cnpj']}'";
		$total = $db->pegaUm($sql);
	}
		?>
	
		<tr bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
			<td valign="top" title="Ações"><?=$v['acoes'] ?></td>
		<?if( !in_array($v['processo'], $arrProc) ){ ?>
			<td align="right" style="color:rgb(0, 102, 204);" rowspan="<?=$total; ?>"><?=$v['processo'] ?></td>
			<td align="right" style="color:rgb(0, 102, 204);" rowspan="<?=$total; ?>"><?=$v['nu_cnpj'] ?></td>
			<td rowspan="<?=$total; ?>"><?=$v['entidade'] ?></td>
		<?} ?>
			<td align="right" valign="top" style="color:rgb(0, 102, 204);"><?=$v['sequencialne'] ?></td>
			<td valign="top"><?=$v['numeroempenho'] ?></td>
			<td valign="top"><?=$v['nu_empenho_original'] ?></td>
			<td valign="top"><?=$v['situacao'] ?></td>
			<td align="right" valign="top" style="color:rgb(0, 102, 204);"><?=$v['especie'] ?></td>
			<td align="right" valign="top" style="color:rgb(0, 102, 204);"><?=number_format($v['valor'], 2, ',', '.'); ?></td>
		</tr>
	<?
	array_push($arrProc, $v['processo']);
}
?>
</tbody>
	<tr >
		<td class="subtituloesquerda" colspan="10"><b>Total de Registros: <?=sizeof($arrProcesso) ?></b></td>
   </tr>
</table>
<?} ?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>

<div id="dialog_acoes" title="Empenhos do PAR" style="display: none; text-align: center;" >
	<div style="padding:5px;text-align:justify;" id="mostraRetorno"></div>
</div>
<script>

/*$(document).ready(function(){
	$('#valorsub[]').each(function(){
		$(this).val( mascaraglobal('[.###],##',this.value) );
	});
});*/

function calculaEmpenho(tipo, sbdid ){
	
	var valorsub		= $('[name="valorsub['+sbdid+']"]').val();
	var valorsubacao	= $('[name="valorsubacao['+sbdid+']"]').val();
	var percent 		= $('[name="percent['+sbdid+']"]').val();
	
	if( tipo == 'P' ){
		if( percent <= 100 ){
			var total = parseFloat(valorsubacao) * (percent/100);
			var total_mac = mascaraglobal('[.###],##',total.toFixed(2));
			$('[name="valorsub['+sbdid+']"]').val(total_mac);
		} else {
			$('[name="valorsub['+sbdid+']"]').val( mascaraglobal('[.###],##',valorsubacao) );
			$('[name="percent['+sbdid+']"]').val(100);
		}		
	} else {
		
		valorsub = replaceAll(valorsub, '.', '');
		valorsub = replaceAll(valorsub, ',', '.');
				
		var porcento = (parseFloat(valorsub) * 100) / parseFloat(valorsubacao);
		
		$('[name="percent['+sbdid+']"]').val( parseFloat(porcento) );
	}
}

function marcarChk(check, sbdid ){
	//percent valorsub
	if( check.checked == true ){
		$('[name="percent['+sbdid+']"]').removeClass('disabled');
		$('[name="percent['+sbdid+']"]').addClass('normal');
		$('[name="percent['+sbdid+']"]').attr('disabled', false);
		
		var valor = $('[name="valorsubacao['+sbdid+']"]').val();
		
		$('[name="valorsub['+sbdid+']"]').val( mascaraglobal('[.###],##', valor) );
		$('[name="percent['+sbdid+']"]').val( '100' );
		
		$('[name="valorsub['+sbdid+']"]').removeClass('disabled');
		$('[name="valorsub['+sbdid+']"]').addClass('normal');
		$('[name="valorsub['+sbdid+']"]').attr('disabled', false);
	} else {
		$('[name="percent['+sbdid+']"]').removeClass('normal');
		$('[name="percent['+sbdid+']"]').addClass('disabled');
		$('[name="percent['+sbdid+']"]').attr('disabled', true);
		
		$('[name="valorsub['+sbdid+']"]').val( '' );
		$('[name="percent['+sbdid+']"]').val( '' );
			
		$('[name="valorsub['+sbdid+']"]').removeClass('normal');
		$('[name="valorsub['+sbdid+']"]').addClass('disabled');
		$('[name="valorsub['+sbdid+']"]').attr('disabled', true);
	}
}

function confirmarEmpenho(numero_documento, nu_seq_ne, processo, nu_cnpj, valor_ne, co_especie_empenho){
	
	$.ajax({
		type: "POST",
		url: "par.php?modulo=principal/empenhosSigef&acao=A",
		data: "requisicao=mostraEmpenhoPar&nu_seq_ne="+nu_seq_ne+"&numero_documento="+numero_documento+"&processo="+processo+"&nu_cnpj="+nu_cnpj+"&valor_ne="+valor_ne+'&co_especie_empenho='+co_especie_empenho,
		async: false,
		success: function(msg){
			$( "#dialog_acoes" ).show();
			$( "#mostraRetorno" ).html(msg);
			$( '#dialog_acoes' ).dialog({
					resizable: false,
					width: 1000,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
						'Atualizar': function() {
							atualizarEmpenhoPAR(nu_seq_ne, numero_documento);
							$( this ).dialog( 'close' );
						},
						'Cancelar': function() {
							$( this ).dialog( 'close' );
						}
					}
			});
		}
	});
}

function insereEmpenho(numero_documento, nu_seq_ne, processo, nu_cnpj, valor_ne, co_especie_empenho){
	
	$.ajax({
		type: "POST",
		url: "par.php?modulo=principal/empenhosSigef&acao=A",
		data: "requisicao=mostraSubacaoEmpenhoPar&nu_seq_ne="+nu_seq_ne+"&numero_documento="+numero_documento+"&processo="+processo+"&nu_cnpj="+nu_cnpj+"&valor_ne="+valor_ne+'&co_especie_empenho='+co_especie_empenho,
		async: false,
		success: function(msg){
			$( "#dialog_acoes" ).show();
			$( "#mostraRetorno" ).html(msg);
			$( '#dialog_acoes' ).dialog({
					resizable: false,
					width: 1000,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
						'Inserir': function() {
							inserirEmpenhoPAR(nu_seq_ne, numero_documento);
							$( this ).dialog( 'close' );
						},
						'Cancelar': function() {
							$( this ).dialog( 'close' );
						}
					}
			});
		}
	});
}

function atualizarEmpenhoPAR(nu_seq_ne, numero_documento){
	var dados = $('#formempenho').serialize();
	
	$.ajax({
		type: "POST",
		url: "par.php?modulo=principal/empenhosSigef&acao=A",
		data: "requisicao=atualizarEmpenho&nu_seq_ne="+nu_seq_ne+'&numero_documento='+numero_documento+'&'+dados,
		async: false,
		success: function(msg){
			if(msg == 1){
				alert('Operação Realizada com Sucesso!');
			} else {
				alert('Falha na Operação!');
			}
			//window.location.href = window.location;
		}
	});
}

function inserirEmpenhoPAR(nu_seq_ne, numero_documento){
	var dados = $('#formempenhopar').serialize();
	
	$.ajax({
		type: "POST",
		url: "par.php?modulo=principal/empenhosSigef&acao=A",
		data: "requisicao=insereEmpenhoPar&nu_seq_ne="+nu_seq_ne+'&numero_documento='+numero_documento+'&'+dados,
		async: false,
		success: function(msg){
			console.log(msg);
			if(msg == 1){
				alert('Operação Realizada com Sucesso!');
			} else {
				alert('Falha na Operação!');
			}
			//window.location.href = window.location;
		}
	});
}
</script>
<?

function consultarEmpenhoSIGEF($nu_seq_ne) {
	$wsusuario 	= 'juliov';
	$wssenha	= 'jamudei1';
	
	$data_created = date("c");

	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$wsusuario</usuario>
			<senha>$wssenha</senha>
		</auth>
		<params>
        	<nu_seq_ne>$nu_seq_ne</nu_seq_ne>
		</params>
	</body>
</request>
XML;

	$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
	
	$xml = Fnde_Webservice_Client::CreateRequest()
			->setURL($urlWS)
			->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
			->execute();

	$xmlRetorno = $xml;

	//ver(simec_htmlentities($arqXml), simec_htmlentities($xml),d);
    $xml = simplexml_load_string( stripslashes($xml));
    
    $status 	= (string) $xml->body->row->status;
	$co_status	= substr( $status, 0, 1 );
	
    	ver($xml,d);
	if( trim($co_status) != 0 ){
	}
    //return $xml;
}
?>