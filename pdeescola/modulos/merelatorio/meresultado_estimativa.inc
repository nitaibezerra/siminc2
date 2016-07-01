<?

ini_set( "memory_limit", "2048M" );
set_time_limit(0);

include APPRAIZ. 'includes/classes/relatorio.class.inc';

// Função para montar o array com os agrupadores
function monta_agp($tipo) {
	if($tipo == 'html') {
		$agp = array(
					"agrupador" => array(),
					"agrupadoColuna" => array(
												"memclassificacaoescola",
	 									   		"htddata",
												"nomeDiretor",
												"telefoneDiretor",
												"emailDiretor",
												"nomeCoordenador",
												"telefoneCoordenador",
												"emailCoordenador",
	 									   		"ressarcimentomonitores",
												"servicosmateriais",
												"servicosmateriaiscap",
												"servicosmateriaiscus",
												"escolaabertacap",
												"escolaabertacus",
												"kitCapital",
												"kitCusteio",
												"jovem1517",
												"jovem1517custeio",
												"jovem1517capital",
	 									   		"memvlrprimeiraparcela",												
												"totalCapital",
											    "totalCusteio",
												"totalGeral",
												"saldo",
												"peifCusteio",
												"peifCapital",
												"repasseCusteio",
												"repasseCapital",
												"totalRepasse"
							//,"total_fnde"
												
											  )	  
					);
	}
	else if($tipo == 'xls') {
		$agp = array(
					"agrupador" => array(),
					"agrupadoColuna" => array(
												"entcodent",
												"memclassificacaoescola",
	 									   		"htddata",
												"nomeDiretor",
												"telefoneDiretor",
												"emailDiretor",
												"nomeCoordenador",
												"telefoneCoordenador",
												"emailCoordenador",
	 									   		"ressarcimentomonitores",
												"servicosmateriais",
												"servicosmateriaiscap",
												"servicosmateriaiscus",
												"escolaabertacap",
												"escolaabertacus",
												"kitCapital",
												"kitCusteio",
												"jovem1517",
												"jovem1517custeio",
												"jovem1517capital",								
	 									   		"memvlrprimeiraparcela",												
												"totalCapital",
											    "totalCusteio",
												"totalGeral",
												"repasseCusteio",
												"repasseCapital",
												"saldo",
												"peifCusteio",
												"peifCapital",												
												"repasseCusteio",
												"repasseCapital",
												"totalRepasse"
							//,"total_fnde"
												
											  )	  
					);

		
	}
	
	array_push($agp['agrupador'], array("campo" => "regdescricao", "label" => "Região"));
	array_push($agp['agrupador'], array("campo" => "estdescricao", "label" => "Estado"));
	array_push($agp['agrupador'], array("campo" => "mundescricao", "label" => "Município"));
	array_push($agp['agrupador'], array("campo" => "tipo", "label" => "Tipo"));
	array_push($agp['agrupador'], array("campo" => "entnome", "label" => "Código - Escola"));
	
	return $agp;
}

// Função para montar as colunas do relatório
function monta_coluna($tipo) {
	
$ano = $_SESSION['exercicio'] ? $_SESSION['exercicio'] : $_REQUEST['ano'];
	
if($tipo == 'html') {
	
		$coluna    = array(
						array(
								"campo" => "memclassificacaoescola",
								"label" => "Classificação"
						),				
						array(
							  "campo" => "htddata",
					   		  "label" => "Data de Aprovação"
						),
						array(
							  "campo" => "nomeDiretor",
					   		  "label" => "Nome do Diretor"
						),
						array(
							  "campo" => "telefoneDiretor",
					   		  "label" => "Telefone do Diretor"
						),
						array(
							  "campo" => "emailDiretor",
					   		  "label" => "Email do Diretor",
							  "type"  => "string"
						),
						array(
							  "campo" => "nomeCoordenador",
					   		  "label" => "Nome do Coordenador"
						),
						array(
							  "campo" => "telefoneCoordenador",
					   		  "label" => "Telefone do Coordenador"
						),
						array(
							  "campo" => "emailCoordenador",
					   		  "label" => "Email do Coordenador",
							  "type"  => "string"
						)
				
				);
		
				array_push($coluna, array(
					  "campo" => "ressarcimentomonitores",
			   		  "label" => "Ressarc. Monitores (custeio)"
				));						
				array_push($coluna, array(
					  "campo" => "servicosmateriaiscap",
			   		  "label" => "Serviço/Mat. Cons. (capital)"
				));
				array_push($coluna, array(
					  "campo" => "servicosmateriaiscus",
			   		  "label" => "Serviço/Mat. Cons. (custeio)"
				));
				array_push($coluna, array(
					  "campo" => "kitCapital",
			   		  "label" => "Kits Materiais (capital)"
				));
				array_push($coluna, array(
					  "campo" => "kitCusteio",
			   		  "label" => "Kits Materiais (custeio)"
				));				
				
				if( (integer) $ano == 2013 ){
					
					array_push($coluna, array(
							"campo" => "jovem1517",
							"label" => "Jovens de 15 a 17 (Tutor)"
					));					
					array_push($coluna, array(
							"campo" => "jovem1517custeio",
							"label" => "Jovens de 15 a 17 (Custeio)"
					));
					array_push($coluna, array(
							"campo" => "jovem1517capital",
							"label" => "Jovens de 15 a 17 (Capital)"
					));			
					array_push($coluna, array(
						  "campo" => "escolaabertacap",
				   		  "label" => "Relação Escola-comunidade (capital)"
					));
					array_push($coluna, array(
						  "campo" => "escolaabertacus",
				   		  "label" => "Relação Escola-comunidade (custeio)"
					));
					
				}else{
					
					array_push($coluna, array(
						"campo" => "escolaabertacap",
						"label" => "Escola Aberta (capital)"
					));
					array_push($coluna, array(
						"campo" => "escolaabertacus",
						"label" => "Escola Aberta (custeio)"
					));
				}
				
				array_push($coluna, array(
					  "campo" => "totalCapital",
			   		  "label" => "Total(capital)"
				));
				array_push($coluna, array(
					  "campo" => "totalCusteio",
			   		  "label" => "Total(custeio)"
				));
				array_push($coluna, array(
					  "campo" => "totalGeral",
			   		  "label" => "Total Geral"
				));		

				if( (integer) $ano == 2013 ){					
				
					array_push($coluna, array(
							"campo" => "saldo",
							"label" => "Saldo"
					));
					array_push($coluna, array(
							"campo" => "peifCusteio",
							"label" => "PEIF (custeio)"
					));
					array_push($coluna, array(
							"campo" => "peifCapital",
							"label" => "PEIF (capital)"
					));						
					array_push($coluna, array(
							"campo" => "repasseCusteio",
							"label" => "Repasse (custeio)"
					));
					array_push($coluna, array(
							"campo" => "repasseCapital",
							"label" => "Repasse (capital)"
					));
					array_push($coluna, array(
							"campo" => "totalRepasse",
							"label" => "Total do Repasse"
					));
				}
				/*
				array_push($coluna, array(
					"campo" => "total_fnde",
					"label" => "Total do FNDE"
				));
				*/
				
					
}
else if($tipo == 'xls') {
	
		$coluna    = array(
					array(
						  "campo" => "entcodent",
				   		  "label" => "Código da Escola",
						  "type"  => "string"
					),
					array(
						"campo" => "memclassificacaoescola",
						"label" => "Classificação",
					),				
					array(
						  "campo" => "htddata",
				   		  "label" => "Data de Aprovação"
					),
					array(
						  "campo" => "nomeDiretor",
				   		  "label" => "Nome do Diretor"
					),
					array(
						  "campo" => "telefoneDiretor",
				   		  "label" => "Telefone do Diretor"
					),
					array(
						  "campo" => "emailDiretor",
				   		  "label" => "Email do Diretor",
						  "type"  => "string"
					),
					array(
						  "campo" => "nomeCoordenador",
				   		  "label" => "Nome do Coordenador"
					),
					array(
						  "campo" => "telefoneCoordenador",
				   		  "label" => "Telefone do Coordenador"
					),
					array(
						  "campo" => "emailCoordenador",
				   		  "label" => "Email do Coordenador",
						  "type"  => "string"
					),
					array(
						  "campo" => "ressarcimentomonitores",
				   		  "label" => "Ressarc. Monitores (custeio)"
					),
					array(
						  "campo" => "servicosmateriaiscap",
				   		  "label" => "Serviço/Mat. Cons. (Capital)"
					),
					array(
						  "campo" => "servicosmateriaiscus",
				   		  "label" => "Serviço/Mat. Cons. (Custeio)"
					),
					array(
						  "campo" => "kitCapital",
				   		  "label" => "Kits Materiais(capital)"
					),
					array(
						  "campo" => "kitCusteio",
				   		  "label" => "Kits Materiais(custeio)"
					),
				
					array(
							"campo" => "jovem1517",
							"label" => "Jovens de 15 a 17 (Tutor)"
					),

					array(
							"campo" => "jovem1517custeio",
							"label" => "Jovens de 15 a 17 (Custeio)"
					),
					array(
							"campo" => "jovem1517capital",
							"label" => "Jovens de 15 a 17 (Capital)"
					),
					
					
				
					array(
						  "campo" => "escolaabertacap",
				   		  "label" => "Relação Escola-comunidade (capital)"
					),
					array(
						  "campo" => "escolaabertacus",
				   		  "label" => "Relação Escola-comunidade (custeio)"
					),
					array(
						  "campo" => "totalCapital",
				   		  "label" => "Total(capital)"
					),
					array(
						  "campo" => "totalCusteio",
				   		  "label" => "Total(custeio)"
					),
					array(
						  "campo" => "totalGeral",
				   		  "label" => "Total Geral"
					),
				
					array(
							"campo" => "saldo",
							"label" => "Saldo"
					),
				
				array(
						"campo" => "peifCusteio",
						"label" => "PEIF (custeio)"
				),
				array(
						"campo" => "peifCapital",
						"label" => "PEIF (capital)"
				),
									
					array(
							"campo" => "repasseCusteio",
							"label" => "Repasse (custeio)"
					),
					array(
							"campo" => "repasseCapital",
							"label" => "Repasse (capital)"
					),
				array(
						"campo" => "totalRepasse",
						"label" => "Total do Repasse"
				)
				/*
				,array(
						"campo" => "total_fnde",
						"label" => "Total do FNDE"
				)
				*/
				
				);
		
}
	
return $coluna;
			  	
}

// FILTROS DO RELATÓRIO:
// Ano do Exercício
if(isset($_REQUEST["ano"]) && $_REQUEST["ano"] != "") {
	$ano = (integer)$_REQUEST["ano"];
}else{
	$ano = 	$_SESSION["exercicio"];
}

// Modalidade de Ensino
if(isset($_REQUEST["modalidade"]) && $_REQUEST["modalidade"] != "") {
	$mod = "AND mme.memmodalidadeensino = '".(string)$_REQUEST["modalidade"]."'";
	 
}else{
	$mod = "";
}

// Valor
if(isset($_REQUEST["valor"]) && $_REQUEST["valor"] != "") {
	$valor = str_replace(",", ".", str_replace(".", "", $_REQUEST["valor"]));
}

// Ano anterior
if(isset($_REQUEST["anoanterior"]) && $_REQUEST["anoanterior"] != "") {
	$anoAnterior = " INNER JOIN pdeescola.memaiseducacao mme2 ON mme2.entid = mme.entid AND mme2.memanoreferencia = ".($ano - 1)." AND mme2.memstatus = 'A' AND mme2.memmodalidadeensino = '".$modalidade."' ";
} else {
	$anoAnterior = "";
}

$where = array();

//Região
// if(isset($_REQUEST["regcod"]) && $_REQUEST["regcod"] != "") {
// 	array_push($where, "est.regcod = '".$_REQUEST['regcod']."'");
// }

// Estado
if(isset($_REQUEST["estuf"]) && $_REQUEST["estuf"] != "") {
	array_push($where, "mun.estuf = '".$_REQUEST['estuf']."'");
}

// Município
if(isset($_REQUEST["muncod"]) && $_REQUEST["muncod"] != "") {
	array_push($where, "mun.muncod = '".$_REQUEST['muncod']."'");
}

// Tipo
if(isset($_REQUEST["tpcid"]) && $_REQUEST["tpcid"] != "") {
	array_push($where, "ent.tpcid IN (".$_REQUEST["tpcid"].")");
}

//memclassificacaoescola
if(isset($_REQUEST["memclassificacaoescola"]) && $_REQUEST["memclassificacaoescola"] != "") {
	array_push($where, "mme.memclassificacaoescola IN ('".$_REQUEST["memclassificacaoescola"]."')");
}

// Situação
if(isset($_REQUEST["esdid"]) && $_REQUEST["esdid"] != "") {
	$arrEsdid = array();
	foreach ($_REQUEST["esdid"] as $esdid) {
		if( $esdid == 'null' ){
			array_push($where, "mme.docid is null");
		} else {
			array_push( $arrEsdid, $esdid );
		}
	}
	if( $arrEsdid ) array_push($where, "esd.esdid in (".implode(",",$arrEsdid).")");
	
} 

$flagAlerta2011 = false;

if( is_array($_REQUEST["entid"]) )
{
	if( $_REQUEST["entid"][0] != "" )
	{
		if( is_numeric($_REQUEST["entid"][0]) )
		{
			if($_REQUEST["entid_campo_excludente"] != "1")
				array_push($where, "ent.entid in (".implode(",", $_REQUEST["entid"]).")");
			else
				array_push($where, "ent.entid not in (".implode(",", $_REQUEST["entid"]).")");
		}
	}
}
else
{
	if( $_REQUEST["entid"] && is_numeric($_REQUEST["entid"]) )
	{
		array_push($where, "ent.entid = ".$_REQUEST["entid"]);
		
		$dadosEscola2010 = $db->carregar("SELECT * FROM pdeescola.memaiseducacao WHERE entid = ".$_REQUEST["entid"]." AND memstatus = 'A' AND memanoreferencia = 2010");
		
		if( $dadosEscola2010 )
		{
			$iniciouAnoAnterior = $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$dadosEscola2010[0]["memid"]." AND meacomecounoano = 't'");
						
			if( (integer)$iniciouAnoAnterior <= 0 && $dadosEscola2010[0]["memevlrpago"] )
			{
				$flagAlerta2011 = true;
			}
		}
	}
}

// PST
if(isset($_REQUEST["pst"]) && $_REQUEST["pst"] != "") {
	switch($_REQUEST["pst"]) {
		case 'aderiu':
			array_push($where, "mme.memadesaopst = 'S'");
			break;
		case 'naoaderiu':
			array_push($where, "mme.memadesaopst = 'N'");
			break;
		case 'naoinformado':
			array_push($where, "mme.memadesaopst is null");
			break;
	}
}

$where = (!empty($where) ? "WHERE ".implode(" AND ", $where) : "");

$sql = "SELECT
			ent.entid as entid,
			trim(ent.entcodent) AS entcodent,
			trim(ent.entcodent) || ' - ' || trim(ent.entnome) AS entnome,
			Case When mme.memclassificacaoescola = 'U' Then 'Urbana' Else 'Rural' end AS memclassificacaoescola,
			to_char(max(hid.htddata), 'DD/MM/YYYY') AS htddata,
			mme.memid,
			coalesce(mme.memvlrsaldodisponivel, 0) AS memvlrprimeiraparcela,
			mun.estuf,
			mun.muncod,
			mun.mundescricao,
			CASE ent.tpcid WHEN 1 THEN 'Escolas Estaduais' ELSE 'Escolas Municipais' END AS tipo,
			est.estdescricao,
			reg.regdescricao,
			mme.memsaldo
			--,trim(replace(replace(es1.vl_total,'.',''),',','.')) as total_fnde
		FROM
			entidade.entidade ent
		INNER JOIN
			pdeescola.memaiseducacao mme ON mme.entid = ent.entid AND mme.memanoreferencia = ".$ano." AND mme.memstatus = 'A' $mod
		".$anoAnterior."
				
		--INNER JOIN 
			--carga.me_escolas_educacao_integral es1 on es1.inep = ent.entcodent
				
		LEFT JOIN
			workflow.documento doc ON doc.docid = mme.docid
		LEFT JOIN
			workflow.historicodocumento hid ON hid.docid = doc.docid --AND hid.aedid = 68
		LEFT JOIN
			workflow.estadodocumento esd ON esd.esdid = doc.esdid
		LEFT JOIN 
			entidade.endereco en ON en.entid = ent.entid
		LEFT JOIN 
			territorios.municipio mun ON mun.muncod = en.muncod
		LEFT JOIN 
			territorios.estado est ON est.estuf = mun.estuf
		LEFT JOIN
			territorios.regiao reg ON reg.regcod = est.regcod
		".$where."
		GROUP BY
			ent.entid,
			ent.entcodent,
			ent.entnome,
			mme.memid,
			mme.memvlrsaldodisponivel,
			mme.memclassificacaoescola,
			mun.estuf,
			mun.muncod,
			mun.mundescricao,
			ent.tpcid,
			est.estdescricao,
			reg.regdescricao,
			est.regcod,
			mme.memsaldo
				
				--,es1.vl_total
		ORDER BY
			est.regcod, mun.estuf, mun.muncod, ent.tpcid";

// ver(1, $sql, d);
$entidades = $db->carregar($sql);

$arrResultado = array();

if($entidades) {
	
	echo "<script type=\"text/javascript\" src=\"../includes/funcoes.js\"></script>
		  <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\">
		  <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/listagem.css\">";
	
	$contResultado = 0;
	for($i=0; $i<count($entidades); $i++) {
		// RECUPERA OS DADOS DO DIRETOR DA ESCOLA
		$sql = "SELECT
					e.entnome as nome_diretor,
					'(' || e.entnumdddresidencial || ') ' || e.entnumresidencial as telefone_diretor,
					e.entemail as email_diretor
				FROM
					entidade.entidade e
				INNER JOIN
					entidade.funcaoentidade fe ON e.entid = fe.entid
				INNER JOIN 
					entidade.funentassoc fea on fea.fueid = fe.fueid
				LEFT JOIN 
					entidade.endereco en ON en.entid = e.entid
				LEFT JOIN
					territorios.municipio mun ON mun.muncod = en.muncod
				WHERE
					fea.entid = '".$entidades[$i]["entid"]."' AND
					fe.funid = ".FUN_DIRETOR_ME." AND 
					fe.fuestatus = 'A'";
		$diretorEscola = $db->carregar($sql);
		
		// RECUPERA OS DADOS DO COORDENADOR DA ESCOLA
		$sql = "SELECT
					e.entnome as nome_coordenador,
					'(' || e.entnumdddresidencial || ') ' || e.entnumresidencial as telefone_coordenador,
					e.entemail as email_coordenador
				FROM
					entidade.entidade e
				INNER JOIN
					entidade.funcaoentidade fe ON e.entid = fe.entid
				INNER JOIN 
					entidade.funentassoc fea on fea.fueid = fe.fueid
				LEFT JOIN 
					entidade.endereco en ON en.entid = e.entid
				LEFT JOIN
					territorios.municipio mun ON mun.muncod = en.muncod
				WHERE
					fea.entid = '".$entidades[$i]["entid"]."' AND
					fe.funid = ".FUN_COORDENADOR_ME." AND 
					fe.fuestatus = 'A'";
		$coordenadorEscola = $db->carregar($sql);
		
		$incluir = false;
		
		// Verifica o número de atividades escolhido
		if( !$_REQUEST["num_atividades_calculadas"] ) $_REQUEST["num_atividades_calculadas"] = 6;
			
		// Recupera o Saldo do Ano Anterior
		$saldoAnoAnterior = (float)$entidades[$i]["memvlrprimeiraparcela"];
		
		// Inicializa as variáveis p/ os cálculos
		$kitCusteio 			= 0;
		$kitCapital 			= 0;
		$ressarcimentoMonitores = 0;
		$valorLimite 			= 0;
		$servicosMateriais 		= 0;
		$escolaAbertaCapital	= 0;
		$escolaAbertaCusteio	= 0;
		$pagamentoProfessorPST	= 0;
		$calculoJovem15a17		= 0;
		$peifCusteio 			= 0;
		$peifCapital 			= 0;
		
		/*** INICIO - Regras para cálculo do pagamento de escolas 2010/2009 - INICIO ***/
		if((integer)$ano == 2010) {
			
			// verifica se a escola participou em 2009
			$dadosEscola2009 = $db->carregar("SELECT * FROM pdeescola.memaiseducacao WHERE entid = ".$entidades[$i]["entid"]." AND memstatus = 'A' AND memanoreferencia = 2009");
			
			// escola de 2010
			if(!$dadosEscola2009) {
				// Quantidade de meses
				$meses = 10;
				// Chama a função para o cálculo
				calculoKitsRessarcimento($entidades[$i]["memid"], $ano, $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
				$valorLimite = 999999;
				// Chama a função para cálculo de Serviços e Materiais
				calculoServicosMateriais($entidades[$i]["memid"], $ano, $servicosMateriais, $meses);
			}
			// participou em 2009
			else {
				// não foi paga em 2009
				//if(!$dadosEscola2009[0]["memvlrpago"]) {
				if( $dadosEscola2009[0]["mempagofnde"] == 'f' )
				{
					// Quantidade de meses
					$meses = 10;
					
					/*** Atividades de 2010 ***/
					// Chama a função para o cálculo
					calculoKitsRessarcimento($entidades[$i]["memid"], $ano, $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
					$valorLimite = 999999;
					// Chama a função para cálculo de Serviços e Materiais
					calculoServicosMateriais($entidades[$i]["memid"], $ano, $servicosMateriais, $meses);
				}
				// foi feito o pagamento em 2009
				else {
					$iniciouAnoAnterior = $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$dadosEscola2009[0]["memid"]." AND meacomecounoano = 't'");
					
					// Escola iniciou as atividades
					if((integer)$iniciouAnoAnterior > 0) {
						// Quantidade de meses
						$meses = 10;
						// Chama a função para o cálculo
						calculoKitsRessarcimento($entidades[$i]["memid"], $ano, $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
						// Chama a função para cálculo do valor limite
						//calculoValorLimite($entidades[$i]["memid"], $ano, $_REQUEST["num_atividades_calculadas"], &$valorLimite, $meses);
						$valorLimite = 999999;
						// Chama a função para cálculo de Serviços e Materiais
						calculoServicosMateriais($entidades[$i]["memid"], $ano, $servicosMateriais, $meses);
					}
					// Escola não iniciou as atividades
					else {
						// Quantidade de meses
						$meses = 4;
						/*** Atividades de 2009 ***/
						// Chama a função para o cálculo
						calculoKitsRessarcimento($dadosEscola2009[0]["memid"], 2009, $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses, true, true);
						// Chama a função para cálculo do valor limite
						calculoValorLimite($dadosEscola2009[0]["memid"], 2009, $_REQUEST["num_atividades_calculadas"], $valorLimite, $meses);
						// Chama a função para cálculo de Serviços e Materiais
						calculoServicosMateriais($dadosEscola2009[0]["memid"], 2009, $servicosMateriais, $meses);
						// Quantidade de meses
						$meses = 10;
						/*** Atividades de 2010 ***/
						// Chama a função para o cálculo
						calculoKitsRessarcimento($entidades[$i]["memid"], $ano, $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
						// Chama a função para cálculo do valor limite
						calculoValorLimite($entidades[$i]["memid"], $ano, $_REQUEST["num_atividades_calculadas"], $valorLimite, $meses);
					}
				}
			}
		}
		// Escolas que não são de 2010...
		else {
			
			$flag = true;
			$dadosEscolaAnoAnterior = $db->carregar("SELECT memid,mempagofnde FROM pdeescola.memaiseducacao WHERE entid = ".$entidades[$i]["entid"]." AND memstatus = 'A' AND memanoreferencia = ".((integer)$ano - 1));
			
			if($dadosEscolaAnoAnterior)
			{
				$temAtividades = $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$dadosEscolaAnoAnterior[0]['memid']."");
				
				if( $temAtividades > 0 )
				{
					if( $temAtividades != $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$dadosEscolaAnoAnterior[0]['memid']." AND meacomecounoano is null") )
					{
						$iniciouAnoAnterior = $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$dadosEscolaAnoAnterior[0]['memid']." AND meacomecounoano = 't'");
						
						if( (integer)$iniciouAnoAnterior == 0 && $dadosEscolaAnoAnterior[0]['mempagofnde'] == 't' )
						{
							// não paga nada
							$flag = false;
						}
					}
				}
			}
			
			if($flag)
			{
				
				if( (integer) $ano == 2013 ){
					// Quantidade de meses usada nos anos de 2008 e 2009
					$meses = 6;
				}else{
					// Quantidade de meses usada nos anos de 2008 e 2009
					$meses = 10;
				}
				
				// Chama a função para o cálculo
				calculoKitsRessarcimento($entidades[$i]["memid"], $ano, $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
							
				if( (integer) $ano == 2012 ) {
					if($pagamentoProfessorPST > 0){
						$ressarcimentoMonitores += $pagamentoProfessorPST;
						$pagamentoProfessorPST = 0;
					}
				}
				
				// Chama a função para cálculo de Serviços e Materiais
				calculoServicosMateriais($entidades[$i]["memid"], $ano, $servicosMateriais, $meses, $calculoServicosMateriaisCapital, $calculoServicosMateriaisCusteio);
			}
			
			$rsAtividades = $db->pegaLinha("SELECT mamescolaaberta, mempeif, memjovem1517 FROM pdeescola.memaiseducacao WHERE entid = ".$entidades[$i]["entid"]." AND memstatus = 'A' AND memanoreferencia = ".((integer)$ano));
			
			if($rsAtividades['mamescolaaberta'] == 't'){				
				calculoEscolaAberta($entidades[$i]["memid"], $ano, $escolaAbertaCapital, $escolaAbertaCusteio, $meses);
			}			
			if($rsAtividades['memjovem1517'] == 't'){				
				calculoJovem15a17($entidades[$i]["memid"], $ano, $meses, $calculoJovem15a17);
			}
			if($rsAtividades['mempeif'] == 't'){
				calculoPeif($entidades[$i]["memid"], $peifCusteio, $peifCapital, $ano);
			}
						
		}
		/*** FIM - Regras para cálculo do pagamento de escolas 2010/2009 - FIM ***/
		
		$valorKits = ($kitCusteio + $kitCapital);
		
		if( (integer) $ano == 2013 ) {
			$porcentagemCusteio = $calculoServicosMateriaisCusteio;
			$porcentagemCapital = $calculoServicosMateriaisCapital;
		}else{
			$porcentagemCusteio = ( $servicosMateriais * 0.8 );
			$porcentagemCapital = ( $servicosMateriais * 0.2 );
		}
		
		if( (integer) $ano == 2013 ){
		
			$totalCusteio = ($kitCusteio + $pagamentoProfessorPST + $ressarcimentoMonitores + $porcentagemCusteio + $escolaAbertaCusteio + ($calculoJovem15a17>0 ? 5000 : 0) + $calculoJovem15a17 );
			$totalCapital = ($kitCapital + $porcentagemCapital + $escolaAbertaCapital + ($calculoJovem15a17>0 ? 2000 : 0) );
			
			$totalGeral = $totalCusteio + $totalCapital;
		
		}else{
			
			// Calcula os Totais
			//$totalCusteio = ($kitCusteio + $ressarcimentoMonitores + $servicosMateriais + $pagamentoProfessorPST - $saldoAnoAnterior);
			//$totalCusteio = ($kitCusteio + $pagamentoProfessorPST + $ressarcimentoMonitores + $servicosMateriais - $saldoAnoAnterior);
			
			// aqui esta com problema
			//$totalCusteio = ($kitCusteio + $pagamentoProfessorPST + $ressarcimentoMonitores + $porcentagemCusteio + $escolaAbertaCusteio - $saldoAnoAnterior);
			$totalCusteio = $ressarcimentoMonitores + $porcentagemCusteio + $kitCusteio + $escolaAbertaCusteio;
			
			//ver($ressarcimentoMonitores, $porcentagemCusteio, $kitCusteio, $totalCusteio, d);
			
			//$totalCapital = $kitCapital;
			$totalCapital = ($kitCapital + $porcentagemCapital + $escolaAbertaCapital + $escolaAbertaCapital);
			
			//$totalGeral = ($valorKits + $ressarcimentoMonitores + $servicosMateriais + $pagamentoProfessorPST - $saldoAnoAnterior);
// 			$totalGeral = ($valorKits + $pagamentoProfessorPST + $ressarcimentoMonitores + $servicosMateriais + ($escolaAbertaCusteio+$escolaAbertaCapital) - $saldoAnoAnterior);
			
			$totalGeral = $totalCapital + $totalCusteio;
		}
		
		if(isset($valor) && $valor != "") {
		  if($_REQUEST["operador"] == "maior") {
			if((float)$totalGeral > (float)$valor) {
				$incluir = true;
			}
		  }
		  if($_REQUEST["operador"] == "menor") {
		  	if((float)$totalGeral < (float)$valor) {
		  		$incluir = true;
		  	}
		  }
		}
		else {
			$incluir = true;
		}
		
		if($totalGeral < 0) $totalGeral = 0;
		
		// novo cálculo definido pelo Leandro
		if($totalCusteio < 0) {
			if(abs($totalCusteio) > $totalCapital)
				$totalCusteio = (($totalCapital) * (-1));
		}
		
		$totalRepasse = $totalGeral-$entidades[$i]["memsaldo"];
		if($totalRepasse < 0) $totalRepasse = 0;
		
		$totalRepasse = $totalRepasse+$peifCapital+$peifCusteio;
		/*
		$entidades[$i]["total_fnde"] = trim($entidades[$i]["total_fnde"])=='-' ? '0' : $entidades[$i]["total_fnde"];
				
		if(trim($totalRepasse) == trim($entidades[$i]["total_fnde"])){
			$incluir = false;
		}
		*/
		if($incluir) {
// 			ver($totalRepasse,$entidades[$i]["total_fnde"], (trim($totalRepasse) == trim($entidades[$i]["total_fnde"])), d);
			if($_REQUEST["tipo"] == 'xls') {
				$arrResultado[$contResultado]["entcodent"]			= 	$entidades[$i]["entcodent"];
			}
			$arrResultado[$contResultado]["entnome"]				= 	$entidades[$i]["entnome"];
			$arrResultado[$contResultado]["htddata"]				= 	$entidades[$i]["htddata"];
			
			$arrResultado[$contResultado]["memclassificacaoescola"] = 	$entidades[$i]["memclassificacaoescola"];
			
			// Diretor
			$arrResultado[$contResultado]["nomeDiretor"]			= 	$diretorEscola[0]["nome_diretor"];
			$arrResultado[$contResultado]["telefoneDiretor"]		= 	$diretorEscola[0]["telefone_diretor"];
			$arrResultado[$contResultado]["emailDiretor"]			= 	$diretorEscola[0]["email_diretor"];
			
			// Coordenador
			$arrResultado[$contResultado]["nomeCoordenador"]		= 	$coordenadorEscola[0]["nome_coordenador"];
			$arrResultado[$contResultado]["telefoneCoordenador"]	= 	$coordenadorEscola[0]["telefone_coordenador"];
			$arrResultado[$contResultado]["emailCoordenador"]		= 	$coordenadorEscola[0]["email_coordenador"];
			
			$arrResultado[$contResultado]["pagamentoPST"]			= 	$pagamentoProfessorPST;
			$arrResultado[$contResultado]["memvlrprimeiraparcela"]	= 	$saldoAnoAnterior;
			$arrResultado[$contResultado]["ressarcimentomonitores"]	=	$ressarcimentoMonitores;
			$arrResultado[$contResultado]["servicosmateriais"]		=	$servicosMateriais;
			$arrResultado[$contResultado]["servicosmateriaiscap"]	=	$porcentagemCapital;
			$arrResultado[$contResultado]["servicosmateriaiscus"]	=	$porcentagemCusteio;
			$arrResultado[$contResultado]["escolaabertacap"]		=	$escolaAbertaCapital;
			$arrResultado[$contResultado]["escolaabertacus"]		=	$escolaAbertaCusteio;
			$arrResultado[$contResultado]["kitCapital"]				=	$kitCapital;
			$arrResultado[$contResultado]["kitCusteio"]				=	$kitCusteio;
			
			$arrResultado[$contResultado]["jovem1517"]				=	$calculoJovem15a17;
			
			$arrResultado[$contResultado]["jovem1517custeio"]		=	$calculoJovem15a17>0 ? 5000 : 0; 
			$arrResultado[$contResultado]["jovem1517capital"]		=	$calculoJovem15a17>0 ? 2000 : 0;
			
			$arrResultado[$contResultado]["totalCusteio"] 			= 	$totalCusteio;
			$arrResultado[$contResultado]["totalCapital"] 			= 	$totalCapital;
			$arrResultado[$contResultado]["totalGeral"] 			= 	$totalGeral;
			
			$arrResultado[$contResultado]["regdescricao"] 			= 	$entidades[$i]["regdescricao"];
			$arrResultado[$contResultado]["estdescricao"] 			= 	$entidades[$i]["estdescricao"];
			$arrResultado[$contResultado]["mundescricao"] 			= 	$entidades[$i]["mundescricao"];
			$arrResultado[$contResultado]["tipo"] 					= 	$entidades[$i]["tipo"];
			
			$arrResultado[$contResultado]["saldo"] 					= 	$entidades[$i]["memsaldo"];
			
			$arrResultado[$contResultado]["totalRepasse"] 			= 	$totalRepasse;
			
			$arrResultado[$contResultado]["repasseCusteio"] 		= 	$totalGeral>0 ? (($totalCusteio/$totalGeral)*$totalRepasse)+$peifCusteio : 0;
			$arrResultado[$contResultado]["repasseCapital"] 		= 	$totalGeral>0 ? (($totalCapital/$totalGeral)*$totalRepasse)+$peifCapital : 0;
			
			$arrResultado[$contResultado]["peifCusteio"]			=   $peifCusteio;
			$arrResultado[$contResultado]["peifCapital"]			=   $peifCapital;
			
			//$arrResultado[$contResultado]["total_fnde"]				=   $entidades[$i]["total_fnde"];
			
			
			
			$contResultado++; 
		}
		
	}
	
}else{
	echo "<script type=\"text/javascript\" src=\"../includes/funcoes.js\"></script>
		  <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\">
		  <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/listagem.css\">";
}

if($_REQUEST["tipo"] == 'html') {
	
	if( $flagAlerta2011 ){
		echo '<script>
				alert("A escola não receberá em 2011. A escola deverá executar o recurso referente ao plano de atendimento de 2010.");
			</script>';
	}
	
	$agrup = monta_agp('html');
	$col   = monta_coluna('html');

	$r = new montaRelatorio();
	$r->setAgrupador($agrup, $arrResultado); 
	$r->setColuna($col);
	$r->setBrasao(true);
	$r->setTotNivel(true);

	echo $r->getRelatorio();
}

if($_REQUEST["tipo"] == 'xls') {
	$agrup = monta_agp('xls');
	$col   = monta_coluna('xls');
	$r = new montaRelatorio();
	$r->setAgrupador($agrup, $arrResultado); 
	$r->setColuna($col);
	$r->setBrasao(true);
	$r->setTotNivel(true);
	
	ob_clean(); 
	$nomeDoArquivoXls = 'relatorio';
	echo $r->getRelatorioXls();
	die;
}

?>