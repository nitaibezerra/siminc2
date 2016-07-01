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


monta_titulo( $titulo_modulo, '&nbsp;' );

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

$sql = "SELECT 
			res.resdescricao as resolucao,
			ter.terid as termopac,
			pro.pronumeroprocesso as numeroprocesso,
			mun.estuf as uf,
			mun.muncod as municipio,
			pro.proid,
			pro.protipo
		FROM par.processoobra pro
		INNER JOIN par.resolucao res ON res.resid = pro.resid
		LEFT JOIN par.termocompromissopac ter ON ter.proid = pro.proid AND pro.muncod = ter.muncod AND ter.terstatus='A'
		INNER JOIN territorios.municipio mun ON mun.muncod = pro.muncod
		where pro.prostatus = 'A'
		";

$registros = $db->carregar($sql);

if($registros[0]) {
	
	echo "<p align=center><input type=button name=xls value=XLS onclick=\"window.location='par.php?modulo=relatorio/relatoriopublicacao&acao=A&exibirxls=1';\"></p>";
	
	echo "<table class=\"tabela\" align=\"center\" bgcolor=\"#f5f5f5\" cellspacing=\"1\" cellpadding=\"3\">";
	
	echo "<tr>";
	
	$arCabecalho = array("Nº da Resolução",
						 "Nº do Termo PAC",
						 "Nº do Processo",
						 "UF","Nome da Prefeitura",
						 "Nº do CNPJ",
						 "Relação de Obras Aprovadas",
						 "Valor da Obra",
						 "Fonte de Recurso",
						 "Natureza de Despesa",
						 "Nº da Nota de Empenho",
						 "Valor da Nota de Empenho",
						 "Nome do Prefeito",
						 "Nº do CPF do Prefeito",
						 "Nº do RG do Prefeito",
						 "Tipo de Obra");
	
	echo "<td class=\"SubTituloCentro\">Nº da Resolução</td>
		  <td class=\"SubTituloCentro\">Nº do Termo PAC</td>
		  <td class=\"SubTituloCentro\">Nº do Processo</td>
		  <td class=\"SubTituloCentro\">UF</td>
		  <td class=\"SubTituloCentro\">Nome da Prefeitura</td>
		  <td class=\"SubTituloCentro\">Nº do CNPJ</td>
		  <td class=\"SubTituloCentro\">Relação de Obras Aprovadas</td>
		  <td class=\"SubTituloCentro\">Valor da Obra</td>
		  <td class=\"SubTituloCentro\">Fonte de Recurso</td>
		  <td class=\"SubTituloCentro\">Natureza de Despesa</td>
		  <td class=\"SubTituloCentro\">Nº da Nota de Empenho</td>
		  <td class=\"SubTituloCentro\">Valor da Nota de Empenho</td>
		  <td class=\"SubTituloCentro\">Nome do Prefeito</td>
		  <td class=\"SubTituloCentro\">Nº do CPF do Prefeito</td>
		  <td class=\"SubTituloCentro\">Nº do RG do Prefeito</td>
		  <td class=\"SubTituloCentro\">Tipo de Obra</td>";
	
	echo "</tr>";
	
	foreach($registros as $reg) {
		$dadospref = $db->pegaLinha("SELECT
										ent1.entnome as prefeito, 
										ent1.entnumcpfcnpj as cpf_prefeito, 
										ent1.entnumrg as rg_prefeito, 
										ent1.entorgaoexpedidor as orgaoep_prefeito, 
										ent2.entnome as prefeitura, 
										ent2.entnumcpfcnpj as cnpj_prefeitura
									FROM
										par.entidade ent1
									INNER JOIN par.entidade ent2 ON ent1.inuid = ent2.inuid AND ent2.entstatus = 'A' AND ent2.dutid = ".DUTID_PREFEITURA."
									WHERE
										ent1.entstatus = 'A' AND
										ent1.dutid = ".DUTID_PREFEITO." AND
										ent2.muncod='".$reg['municipio']."'");
		/*
		$dadospref = $db->pegaLinha("select ent1.entnome as prefeito, 
												  ent1.entnumcpfcnpj as cpf_prefeito, 
												  ent1.entnumrg as rg_prefeito, 
												  ent1.entorgaoexpedidor as orgaoep_prefeito, 
												  ent2.entnome as prefeitura, 
												  ent2.entnumcpfcnpj as cnpj_prefeitura 
										   from entidade.entidade ent1 
										   inner join entidade.funcaoentidade fen1 on fen1.entid = ent1.entid and fen1.funid=2 
										   inner join entidade.funentassoc fea ON fea.fueid = fen1.fueid 
										   inner join entidade.entidade ent2 on ent2.entid = fea.entid 
										   inner join entidade.funcaoentidade fen2 ON fen2.entid = ent2.entid and fen2.funid=1
										   inner join entidade.endereco ende on ende.entid = ent2.entid 
										   where ende.muncod='".$reg['municipio']."'");
		*/
		$obrasempenho = $db->carregar("select UPPER(pre.predescricao) as predescricao, 
											  pre.prevalorobra,
											  pre.preid,
											  UPPER(pre.prelogradouro) as prelogradouro,
											  pto.ptodescricao
									   from par.empenho emp 
								  	   inner join par.empenhoobra emo on emo.empid = emp.empid and eobstatus = 'A' and empstatus = 'A'
								  	   inner join obras.preobra pre on pre.preid = emo.preid AND pre.docid IS NOT NULL 
								  	   inner join obras.pretipoobra pto on pto.ptoid = pre.ptoid  
								  	   where emp.empnumeroprocesso='".$reg['numeroprocesso']."'");
		
		$dadosempenho = $db->pegaLinha("select emp.empfonterecurso,
										       emp.empcodigonatdespesa,
										       emp.empnumero,
										       vve.vrlempenhocancelado as empvalorempenho 
									   from par.empenho emp 
											inner join par.v_vrlempenhocancelado vve on vve.empid = emp.empid and empstatus = 'A'
								  	   where emp.empnumeroprocesso='".$reg['numeroprocesso']."'");
		
		
		$termopac = $db->carregarColuna("select 'PAC2'||lpad(terid::text,5,'0')||'/'||to_char(terdatainclusao,'YYYY') as numerotermo from par.termocompromissopac ter
								   		 where ter.proid = '".$reg['proid']."' AND 
								   		 ter.muncod = '".$reg['municipio']."' AND 
								   		 ter.terstatus='A'");
		
		unset($predescricao, $totalValorObra);
		
		$numeroobra = 1;
		if($obrasempenho[0]) {
			foreach($obrasempenho as $ob) {
				$predescricao[] = $numeroobra.") ".$ob['preid']." - ".$ob['predescricao']." ".$ob['prelogradouro']."/".$ob['ptodescricao']." - R$ ".number_format($ob['prevalorobra'],2,",",".");
				$totalValorObra += $ob['prevalorobra'];
				$numeroobra++;
			}
		}
		
		echo "<tr>";
		
		$arDados[] = array('resolucao'=>$reg['resolucao'],
						   'termopac'=>(($termopac)?implode(", ",$termopac):"Sem termo"),
						   'numeroprocesso'=>mascaraglobal($reg['numeroprocesso'],'#####.######/####-##'),
						   'uf'=>$reg['uf'],
						   'prefeitura'=>$dadospref['prefeitura'],
						   'cnpj_prefeitura'=>mascaraglobal($dadospref['cnpj_prefeitura'],'##.###.###/####-##'),
						   'predescricao'=>(($predescricao)?implode(" ",$predescricao):"Não existem"),
						   'totalValorObra'=>number_format($totalValorObra,2,",","."),
						   'empfonterecurso'=>$dadosempenho['empfonterecurso'],
						   'empcodigonatdespesa'=>$dadosempenho['empcodigonatdespesa'],
						   'empnumero'=>$dadosempenho['empnumero'],
						   'empvalorempenho'=>number_format($dadosempenho['empvalorempenho'],2,",","."),
						   'prefeito'=>$dadospref['prefeito'],
						   'cpf_prefeito'=>mascaraglobal($dadospref['cpf_prefeito'],'###.###.###-##'),
						   'rg_prefeito'=>$dadospref['rg_prefeito']." ".$dadospref['orgaoep_prefeito'],
						   'protipo'=>(($reg['protipo']=="P")?"Proinfância":"Quadra")
		);
		
		echo "<td>".$reg['resolucao']."</td>
			  <td>".(($termopac)?implode(", ",$termopac):"Sem termo")."</td>
			  <td>".mascaraglobal($reg['numeroprocesso'],'#####.######/####-##')."</td>
			  <td>".$reg['uf']."</td>
			  <td>".$dadospref['prefeitura']."</td>
			  <td>".mascaraglobal($dadospref['cnpj_prefeitura'],'##.###.###/####-##')."</td>
			  <td>".(($predescricao)?implode(" ",$predescricao):"Não existem")."</td>
			  <td>".number_format($totalValorObra,2,",",".")."</td>
			  <td>".$dadosempenho['empfonterecurso']."</td>
			  <td>".$dadosempenho['empcodigonatdespesa']."</td>
			  <td>".$dadosempenho['empnumero']."</td>
			  <td>".number_format($dadosempenho['empvalorempenho'],2,",",".")."</td>
			  <td>".$dadospref['prefeito']."</td>
			  <td>".mascaraglobal($dadospref['cpf_prefeito'],'###.###.###-##')."</td>
			  <td>".$dadospref['rg_prefeito']." ".$dadospref['orgaoep_prefeito']."</td>
			  <td>".(($reg['protipo']=="P")?"Proinfância":"Quadra")."</td>
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
}



?>