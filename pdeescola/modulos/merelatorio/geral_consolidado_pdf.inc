<?php

ini_set( "memory_limit", "512M" );
set_time_limit(0);

//Chamando a classe de FPDF
require('../../includes/fpdf/fpdf.inc');

if( isset($_REQUEST["lista_escolas"]) )
{
	$listaEscolas = $db->carregarColuna("SELECT entid FROM pdeescola.meloteimpressao WHERE mlicodigolote in (".$_REQUEST["lista_escolas"].") ORDER BY mliid");
	$listaEscolas = " AND e.entid in (" . implode(',',$listaEscolas) .")";
}
else
{
	$listaEscolas = "";
}

// Recupera o perfil do usuário no Mais Educação.
$usuPerfil = arrayPerfil();

// Se for só visualização, recupera as entidades com estado 'relatório emitido', senão pega os 'finalizados'.
$esdid = ' in (39,40) ';

if(in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $usuPerfil) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil)) {
	if(isset($_REQUEST["muncod"]) && $_REQUEST["muncod"] != "") {
		$funid = 1;
		
		$whereEntExecutora = "en.muncod = '".$_REQUEST["muncod"]."'";
		
		$whereEntDirigente = "funprefeito.funid = 2 AND 
				              funprefeitura.funid = 1 AND 
				              mun.muncod ='".$_REQUEST["muncod"]."'";
		
		$whereEntidades = "m.muncod = '".$_REQUEST["muncod"]."' AND 
						   e.tpcid IN (3) AND 
						   est.esdid ".$esdid." 
						   ".$listaEscolas."";
	}
	if(isset($_REQUEST["estuf"]) && $_REQUEST["estuf"] != "") {
		$funid = 6;
		
		$whereEntExecutora = "en.estuf = '".$_REQUEST["estuf"]."'";
		
		$whereEntDirigente = "funprefeito.funid = 25 AND 
	            			  funprefeitura.funid = 6 AND 
			            	  mun.estuf ='".$_REQUEST["estuf"]."'";
		
		$whereEntidades = "m.estuf = '".$_REQUEST["estuf"]."' AND 
						   e.tpcid IN (1) AND 
						   est.esdid ".$esdid." 
						   ".$listaEscolas."";
	}
	
	$sql = "SELECT 
				ent.entnumcpfcnpj, 
				ent.entnome,
				mun.mundescricao,
				mun.estuf
			FROM 
				entidade.entidade ent
			INNER JOIN 
				entidade.endereco en ON en.entid = ent.entid
			INNER JOIN
				entidade.funcaoentidade fue ON fue.entid = ent.entid AND
											   fue.funid = ".$funid."
			INNER JOIN
				territorios.municipio mun ON mun.muncod = en.muncod
			WHERE
				".$whereEntExecutora;
	$entidadeExecutora = $db->carregar($sql);
	//dbg($entidadeExecutora, 1);
	
	$sql = "SELECT 
				entprefeito.entnumcpfcnpj,
				entprefeito.entnome
            FROM 
            	entidade.entidade entprefeito
           	INNER JOIN 
           		entidade.funcaoentidade funprefeito ON entprefeito.entid = funprefeito.entid
            INNER JOIN 
            	entidade.funentassoc feaprefeito ON feaprefeito.fueid = funprefeito.fueid
            INNER JOIN 
            	entidade.entidade entprefeitura ON entprefeitura.entid = feaprefeito.entid
            INNER JOIN 
            	entidade.funcaoentidade funprefeitura ON funprefeitura.entid = entprefeitura.entid
            INNER JOIN 
            	entidade.endereco entd ON entd.entid = entprefeitura.entid
            INNER JOIN 
            	territorios.municipio mun ON entd.muncod = mun.muncod
            WHERE 
            	".$whereEntDirigente;
	$entidadeDirigente = $db->carregar($sql);
	//dbg($entidadeDirigente, 1);
	
	$sql = "SELECT DISTINCT
				maedu.memid,
				e.entid,
			 	e.entcodent,
			 	e.entnome,
			 	maedu.memid,
			 	est.esddsc
			FROM
				entidade.entidade e
			INNER JOIN 
				entidade.endereco ee ON e.entid = ee.entid
			INNER JOIN 
				entidade.endereco endi ON endi.endid = ee.endid
			INNER JOIN 
				territorios.municipio m ON m.muncod = endi.muncod
			INNER JOIN 
				pdeescola.memaiseducacao maedu ON maedu.entid = e.entid 
										      AND maedu.memanoreferencia = " . $_SESSION["exercicio"] . "
										      AND maedu.memstatus = 'A'
			LEFT JOIN 
				workflow.documento d ON d.docid = maedu.docid
			LEFT JOIN 
				workflow.estadodocumento est ON est.esdid = d.esdid
			WHERE
				".$whereEntidades;
	$entidades = $db->carregar($sql);
}
else if(in_array(PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO, $usuPerfil)) {
	$sql = "SELECT * FROM pdeescola.usuarioresponsabilidade WHERE usucpf = '".$_SESSION["usucpf"]."' AND rpustatus = 'A' AND pflcod = ".PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO;
	$pflcod = $db->carregar($sql);

	if($listaEscolas == "") {
		$sql = "SELECT memid FROM pdeescola.meloteimpressao WHERE muncod = '".$pflcod[0]["muncod"]."'";
		$memids = $db->carregar($sql);
		
		if($memids) {
			$restricaoLote = array();
			for($i=0; $i<count($memids); $i++) {
				$restricaoLote[$i] = $memids[$i]["memid"];
			}
			$restricaoLote = " AND maedu.memid not in (".implode(',', $restricaoLote).")";
		} else {
			$restricaoLote = "";
		}
	} else {
		$restricaoLote = "";
	}

	$sql = "SELECT 
				ent.entnumcpfcnpj, 
				ent.entnome,
				mun.mundescricao,
				mun.estuf
			FROM 
				entidade.entidade ent
			INNER JOIN 
				entidade.endereco en ON en.entid = ent.entid
			INNER JOIN
				entidade.funcaoentidade fue ON fue.entid = ent.entid AND
											   fue.funid = 1
			INNER JOIN
				territorios.municipio mun ON mun.muncod = en.muncod
			WHERE
				en.muncod = '".$pflcod[0]["muncod"]."'";
	$entidadeExecutora = $db->carregar($sql);
	
	/*$sql = "SELECT 
				ent.entnumcpfcnpj, 
				ent.entnome
			FROM 
				entidade.entidade ent
			INNER JOIN 
				entidade.endereco en ON en.entid = ent.entid
			INNER JOIN
				entidade.funcaoentidade fue ON fue.entid = ent.entid AND
											   fue.funid = 2
			WHERE
				en.muncod = '".$pflcod[0]["muncod"]."'";*/
	$sql = "SELECT 
				entprefeito.entnumcpfcnpj,
				entprefeito.entnome
            FROM 
            	entidade.entidade entprefeito
           	INNER JOIN 
           		entidade.funcaoentidade funprefeito ON entprefeito.entid = funprefeito.entid
            INNER JOIN 
            	entidade.funentassoc feaprefeito ON feaprefeito.fueid = funprefeito.fueid
            INNER JOIN 
            	entidade.entidade entprefeitura ON entprefeitura.entid = feaprefeito.entid
            INNER JOIN 
            	entidade.funcaoentidade funprefeitura ON funprefeitura.entid = entprefeitura.entid
            INNER JOIN 
            	entidade.endereco entd ON entd.entid = entprefeitura.entid
            INNER JOIN 
            	territorios.municipio mun ON entd.muncod = mun.muncod
            WHERE 
            	funprefeito.funid = 2 AND 
            	funprefeitura.funid = 1 AND 
            	mun.muncod ='".$pflcod[0]["muncod"]."'";
	$entidadeDirigente = $db->carregar($sql);
	
	$sql = "SELECT DISTINCT
				maedu.memid,
				e.entid,
			 	e.entcodent,
			 	e.entnome,
			 	maedu.memid,
			 	est.esddsc
			FROM
				entidade.entidade e
			INNER JOIN 
				entidade.endereco ee ON e.entid = ee.entid
			INNER JOIN 
				entidade.endereco endi ON endi.endid = ee.endid
			INNER JOIN 
				territorios.municipio m ON m.muncod = endi.muncod
			INNER JOIN 
				pdeescola.memaiseducacao maedu ON maedu.entid = e.entid 
											  AND maedu.memanoreferencia = " . $_SESSION["exercicio"] . " 
											  AND maedu.memstatus = 'A'
			LEFT JOIN 
				workflow.documento d ON d.docid = maedu.docid
			LEFT JOIN 
				workflow.estadodocumento est ON est.esdid = d.esdid
			WHERE
				m.muncod = '".$pflcod[0]["muncod"]."' AND 
				e.tpcid IN (3) AND
				est.esdid ".$esdid."
				".$listaEscolas." 
				".$restricaoLote."";
	//dbg($sql,1);
	$entidades = $db->carregar($sql);
}
else if(in_array(PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, $usuPerfil)) {
	$sql = "SELECT * FROM pdeescola.usuarioresponsabilidade WHERE usucpf = '".$_SESSION["usucpf"]."' AND rpustatus = 'A' AND pflcod = ".PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO;
	$pflcod = $db->carregar($sql);
	
	if($listaEscolas == "") {
		$sql = "SELECT memid FROM pdeescola.meloteimpressao WHERE estuf = '".$pflcod[0]["estuf"]."'";
		$memids = $db->carregar($sql);
		
		if($memids) {
			$restricaoLote = array();
			for($i=0; $i<count($memids); $i++) {
				$restricaoLote[$i] = $memids[$i]["memid"];
			}
			$restricaoLote = " AND maedu.memid not in (".implode(',', $restricaoLote).")";
		} else {
			$restricaoLote = "";
		}
	} else {
		$restricaoLote = "";
	}
	
	$sql = "SELECT 
				ent.entnumcpfcnpj, 
				ent.entnome,
				mun.mundescricao,
				mun.estuf
			FROM 
				entidade.entidade ent
			INNER JOIN 
				entidade.endereco en ON en.entid = ent.entid
			INNER JOIN
				entidade.funcaoentidade fue ON fue.entid = ent.entid AND
											   fue.funid = 6
			INNER JOIN
				territorios.municipio mun ON mun.muncod = en.muncod
			WHERE
				en.estuf = '".$pflcod[0]["estuf"]."'";
	$entidadeExecutora = $db->carregar($sql);
	
	/*$sql = "SELECT 
				ent.entnumcpfcnpj, 
				ent.entnome 
			FROM 
				entidade.entidade ent
			INNER JOIN 
				entidade.endereco en ON en.entid = ent.entid
			INNER JOIN
				entidade.funcaoentidade fue ON fue.entid = ent.entid AND
											   fue.funid = 25
			WHERE
				en.estuf = '".$pflcod[0]["estuf"]."'";*/
	$sql = "SELECT 
				entprefeito.entnumcpfcnpj,
				entprefeito.entnome
            FROM 
            	entidade.entidade entprefeito
           	INNER JOIN 
           		entidade.funcaoentidade funprefeito ON entprefeito.entid = funprefeito.entid
            INNER JOIN 
            	entidade.funentassoc feaprefeito ON feaprefeito.fueid = funprefeito.fueid
            INNER JOIN 
            	entidade.entidade entprefeitura ON entprefeitura.entid = feaprefeito.entid
            INNER JOIN 
            	entidade.funcaoentidade funprefeitura ON funprefeitura.entid = entprefeitura.entid
            INNER JOIN 
            	entidade.endereco entd ON entd.entid = entprefeitura.entid
            INNER JOIN 
            	territorios.municipio mun ON entd.muncod = mun.muncod
            WHERE 
            	funprefeito.funid = 25 AND 
            	funprefeitura.funid = 6 AND 
            	mun.estuf ='".$pflcod[0]["estuf"]."'";
	$entidadeDirigente = $db->carregar($sql);
	
	$sql = "SELECT DISTINCT
				maedu.memid,
				e.entid,
			 	e.entcodent,
			 	e.entnome,
			 	maedu.memid,
			 	est.esddsc
			FROM
				entidade.entidade e
			INNER JOIN 
				entidade.endereco ee ON e.entid = ee.entid
			INNER JOIN 
				entidade.endereco endi ON endi.endid = ee.endid
			INNER JOIN 
				territorios.municipio m ON m.muncod = endi.muncod
			INNER JOIN 
				pdeescola.memaiseducacao maedu ON maedu.entid = e.entid 
											  AND maedu.memanoreferencia = " . $_SESSION["exercicio"] . "
											  AND maedu.memstatus = 'A'
			LEFT JOIN 
				workflow.documento d ON d.docid = maedu.docid
			LEFT JOIN 
				workflow.estadodocumento est ON est.esdid = d.esdid
			WHERE
				m.estuf = '".$pflcod[0]["estuf"]."' AND
				e.tpcid IN (1) AND
				est.esdid ".$esdid." 
				".$listaEscolas."
				".$restricaoLote."";
	$entidades = $db->carregar($sql);
}
else {
	echo "<script>
			alert('Não há nenhum Munícipio ou Estado associado ao seu perfil.');
			window.close();
		  </script>";
	exit;
}

// função recupera as informações referentes ao pagamento a ser realizado
function recuperaDadosPagamento($memid, $entid, $ano, &$kitCusteio, &$kitCapital, &$ressarcimentoMonitores, &$servicosMateriais, &$valorLimite, &$pagamentoProfessorPST) {
	global $db;
	
	$numAtividadesCalculadas = 6;
	
	/*** INICIO - Regras para cálculo do pagamento de escolas 2010/2009 - INICIO ***/
	if((integer)$ano == 2010) {
		
		// verifica se a escola participou em 2009
		$dadosEscola2009 = $db->carregar("SELECT * FROM pdeescola.memaiseducacao WHERE entid = ".$entid." AND memstatus = 'A' AND memanoreferencia = 2009");
		
		// escola de 2010
		if(!$dadosEscola2009) {
			// Quantidade de meses
			$meses = 10;
			// Chama a função para o cálculo
			calculoKitsRessarcimento($entid, $ano, $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
			// Chama a função para cálculo do valor limite
			//calculoValorLimite($entidades[$i]["memid"], $ano, $_REQUEST["num_atividades_calculadas"], &$valorLimite, $meses);
			$valorLimite = 999999;
			// Chama a função para cálculo de Serviços e Materiais
			calculoServicosMateriais($memid, $ano, $servicosMateriais, $meses);
		}
		// participou em 2009
		else {
			// não foi paga em 2009
			//if(!$dadosEscola2009[0]["memvlrpago"]) {
			if( $dadosEscola2009[0]["mempagofnde"] == 'f' )
			{
				// Quantidade de meses
				$meses = 10;
				
				/*** Atividades de 2009 ***/
				// Chama a função para o cálculo
				//calculoKitsRessarcimento($dadosEscola2009[0]["memid"], 2009, &$kitCusteio, &$kitCapital, &$ressarcimentoMonitores, $meses, true);
				// Chama a função para cálculo do valor limite
				//calculoValorLimite($dadosEscola2009[0]["memid"], 2009, $_REQUEST["num_atividades_calculadas"], &$valorLimite, $meses);
				// Chama a função para cálculo de Serviços e Materiais
				//calculoServicosMateriais($dadosEscola2009[0]["memid"], 2009, &$servicosMateriais, $meses);
				
				/*** Atividades de 2010 ***/
				// Chama a função para o cálculo
				calculoKitsRessarcimento($memid, $ano, $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
				// Chama a função para cálculo do valor limite
				//calculoValorLimite($entidades[$i]["memid"], $ano, $_REQUEST["num_atividades_calculadas"], &$valorLimite, $meses);
				$valorLimite = 999999;
				// Chama a função para cálculo de Serviços e Materiais
				calculoServicosMateriais($memid, $ano, $servicosMateriais, $meses);
			}
			// foi feito o pagamento em 2009
			else {
				$iniciouAnoAnterior = $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$dadosEscola2009[0]["memid"]." AND meacomecounoano = 't'");
				
				// Escola iniciou as atividades
				if((integer)$iniciouAnoAnterior > 0) {
					// Quantidade de meses
					$meses = 10;
					// Chama a função para o cálculo
					calculoKitsRessarcimento($memid, $ano, $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
					// Chama a função para cálculo do valor limite
					//calculoValorLimite($entidades[$i]["memid"], $ano, $_REQUEST["num_atividades_calculadas"], &$valorLimite, $meses);
					$valorLimite = 999999;
					// Chama a função para cálculo de Serviços e Materiais
					calculoServicosMateriais($memid, $ano, $servicosMateriais, $meses);
				}
				// Escola não iniciou as atividades
				else {
					// Quantidade de meses
					$meses = 4;
					/*** Atividades de 2009 ***/
					// Chama a função para o cálculo
					calculoKitsRessarcimento($memid, 2009, $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses, true, true);
					// Chama a função para cálculo do valor limite
					calculoValorLimite($memid, 2009, $numAtividadesCalculadas, $valorLimite, $meses);
					// Chama a função para cálculo de Serviços e Materiais
					calculoServicosMateriais($memid, 2009, $servicosMateriais, $meses);
					
					// Quantidade de meses
					$meses = 10;
					/*** Atividades de 2010 ***/
					// Chama a função para o cálculo
					calculoKitsRessarcimento($memid, $ano, $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
					// Chama a função para cálculo do valor limite
					calculoValorLimite($memid, $ano, $numAtividadesCalculadas, $valorLimite, $meses);
					// Chama a função para cálculo de Serviços e Materiais
					//calculoServicosMateriais($entidades[$i]["memid"], $ano, &$servicosMateriais, $meses);
				}
			}
		}
	}
	// Escolas que não são de 2010...
	else {
		$flag = true;
		$dadosEscolaAnoAnterior = $db->carregar("SELECT memid,mempagofnde FROM pdeescola.memaiseducacao WHERE entid = ".$entid." AND memstatus = 'A' AND memanoreferencia = ".((integer)$ano - 1));
		
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
			// Quantidade de meses usada nos anos de 2008 e 2009
			$meses = 10;
			
			// Chama a função para o cálculo
			calculoKitsRessarcimento($memid, $ano, $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
						
			// Chama a função para cálculo do valor limite
			//calculoValorLimite($entidades[$i]["memid"], $ano, $_REQUEST["num_atividades_calculadas"], &$valorLimite, $meses);
			
			// Chama a função para cálculo de Serviços e Materiais
			calculoServicosMateriais($memid, $ano, $servicosMateriais, $meses);
		}
	}
	/*** FIM - Regras para cálculo do pagamento de escolas 2010/2009 - FIM ***/
}

$PDF = new FPDF( 'P','cm','A4' ); //documento em formato de retrato, medido em cm e no tamanho A4
$PDF -> SetMargins(1, 1, 1); // margem esquerda = 3 , superior = 3 e direita = 2.
$PDF -> SetAuthor('SIMEC'); //informando o autor do documento.
$PDF -> SetTitle('SIMEC'); //informando o título do documento.
$PDF -> AddPage(); //adicionando um nova página

if($entidades) {
	/*
	 * CRIANDO CABEÇALHO DO PDF
	 */
	$PDF ->Image("../imagens/brasao.JPG",1,0.8,1.2,1.2); //endereco da imagem,posicao X(horizontal),posicao Y(vertical), tamanho altura, tamanho largura
	$PDF ->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
	$PDF->SetX(2.5);
	$PDF->Cell(6,0,'MEC - Ministério da Educação',0,'P');
	$PDF->SetX(2.5);
	$PDF->Cell(6,0.5,NOME_SISTEMA,0,'P');
	$PDF->SetX(2.5);
	$PDF->Cell(6,1,'PDDE EDUCAÇÃO INTEGRAL',0,'P');
	
	$PDF->SetX(15);
	$PDF->Cell(6,0,'Impresso por: '.$_SESSION['usunome'],0,'P');
	$PDF->SetX(15);
	$PDF->Cell(6,0.5,'Data/Hora da Impressão:'. date( 'd/m/Y - H:i:s' ), 0, 'P');
	/*
	 * FIM
	 * CRIANDO CABEÇALHO DO PDF
	 */
	$PDF->SetX(0);
	$PDF->SetY(2.5);
	$PDF->Cell(0,0.5,'PLANO DE ATENDIMENTO GERAL CONSOLIDADO - '.$_SESSION["exercicio"],1,1,'C');
	$PDF->SetFillColor(192,192,192);
	$PDF->Cell(0,0.5,'ENTIDADE EXECUTORA',1,1,'L',true);
	 
	$PDF->Cell(3,0.5,"CNPJ: ".(($entidadeExecutora[0]["entnumcpfcnpj"]) ? ($entidadeExecutora[0]["entnumcpfcnpj"]) : "NÃO CADASTRADO"),1,0,'L',false);
	$PDF->Cell(10,0.5,"NOME DA ENTIDADE: ".(($entidadeExecutora[0]["entnome"]) ? $entidadeExecutora[0]["entnome"] : "NÃO CADASTRADO"),1,0,'L',false);
	$PDF->Cell(5,0.5,"MUNICÍPIO: ".(($entidadeExecutora[0]["mundescricao"]) ? $entidadeExecutora[0]["mundescricao"] : "NÃO CADASTRADO"),1,0,'L',false);
	$PDF->Cell(0,0.5,"UF: ".(($entidadeExecutora[0]["estuf"]) ? $entidadeExecutora[0]["estuf"] : "NÃO CADASTRADO"),1,1,'L',false);
	
	$PDF->Cell(0,0.5,'DIRIGENTE DA ENTIDADE',1,1,'L',true);
	
	$PDF->Cell(3,0.5,"CNPJ: ".(($entidadeDirigente[0]["entnumcpfcnpj"]) ? $entidadeDirigente[0]["entnumcpfcnpj"] : "NÃO CADASTRADO"),1,0,'L',false);
	$PDF->Cell(0,0.5,"NOME DO DIRIGENTE: ".(($entidadeDirigente[0]["entnome"]) ? $entidadeDirigente[0]["entnome"] : "NÃO CADASTRADO"),1,1,'L',false);
	
	$PDF->Ln();
	
	$PDF->Cell(0,0.5,'LISTAGEM DAS ESCOLAS',1,1,'L',true);
	
	// totais
	$kitCusteioGeral 			 = 0;
	$kitCapitalGeral 			 = 0;
	$ressarcimentoMonitoresGeral = 0;
	$valorLimiteGeral			 = 0;
	$servicosMateriaisGeral		 = 0;
	$totalGeralGeral			 = 0;
	$qtdGeralAlunosGeral		 = 0;
	
	for($i=0; $i<count($entidades); $i++) {
		// variáveis para uso no pagamento de escolas
		$kitCusteio 			= 0;
		$kitCapital 			= 0;
		$ressarcimentoMonitores = 0;
		$valorLimite 			= 0;
		$servicosMateriais 		= 0;
		$pagamentoProfessorPST	= 0;
		
		// verifica se a escola participou do programa no ano passado
		$existeAnoAnterior = $db->pegaUm("SELECT memid FROM pdeescola.memaiseducacao WHERE entcodent = '".$entidades[$i]["entcodent"]."' AND memstatus = 'A' AND memanoreferencia = ".($_SESSION["exercicio"] - 1)."");
		
		$PDF ->SetFont('Arial', '', 4);
		
		$PDF->Cell(2,0.5,"REGISTRO: #".($i+1),1,0,'L',false);
		$PDF->Cell(3,0.5,"COD INEP: ".$entidades[$i]["entcodent"],1,0,'L',false);
		$PDF->Cell(11,0.5,"NOME DA ESCOLA: ".$entidades[$i]["entnome"],1,0,'L',false);
		$PDF->Cell(0,0.5,"ANO: ".$_SESSION["exercicio"]."".(($existeAnoAnterior) ? " (participou em ".($_SESSION["exercicio"] - 1).")" : ""),1,1,'L',false);
		
		$PDF->Cell(0,0.5,'ALUNADO PARTICIPANTE',1,1,'L',false);
		$PDF->Cell(1.5,0.5,"1° ano",1,0,'C',false);
		$PDF->Cell(1.5,0.5,"2° ano",1,0,'C',false);
		$PDF->Cell(1.5,0.5,"3° ano",1,0,'C',false);
		$PDF->Cell(1.5,0.5,"4° ano",1,0,'C',false);
		$PDF->Cell(1.5,0.5,"5° ano",1,0,'C',false);
		$PDF->Cell(1.5,0.5,"6° ano",1,0,'C',false);
		$PDF->Cell(1.5,0.5,"7° ano",1,0,'C',false);
		$PDF->Cell(1.5,0.5,"8° ano",1,0,'C',false);
		$PDF->Cell(1.5,0.5,"9° ano",1,0,'C',false);
		$PDF->Cell(1.5,0.5,"1° ano",1,0,'C',false);
		$PDF->Cell(1.5,0.5,"2° ano",1,0,'C',false);
		$PDF->Cell(1.5,0.5,"3° ano",1,0,'C',false);
		$PDF->Cell(0,0.5,"TOTAL",1,1,'C',false);
		
		$modalidadeEnsino = $db->pegaUm("SELECT memmodalidadeensino FROM pdeescola.memaiseducacao WHERE memid = ".$entidades[$i]["memid"]);
		
		$somaAlunado = 0;
		
		for($j=0; $j<12; $j++) {
			// Ensino Fundamental
			if($j>=0 && $j<=8) $inSerie = ($j + 1);
			// Ensino Médio
			if($j>=9 && $j<=11) $inSerie = ($j + 11);
			
			$sql = "SELECT
						map.mapquantidade as qtd
					FROM
						pdeescola.mealunoparticipante map
					INNER JOIN
						pdeescola.seriecicloescolar sce ON sce.sceid = map.sceid
									AND sce.sceid IN (".$inSerie.")
					WHERE
						map.memid = ".$entidades[$i]["memid"]." AND
						map.mapano = " . $_SESSION["exercicio"] . "
					ORDER BY
						map.sceid";
			$alunadoParticipante = $db->carregar($sql);
			
			if($modalidadeEnsino == "F") {
				if($j>=0 && $j<=8) {
					$PDF->Cell(1.5,0.5,$alunadoParticipante[0]["qtd"],1,0,'C',false);
					$somaAlunado += (integer)$alunadoParticipante[0]["qtd"];
				}
				else {
					$PDF->Cell(1.5,0.5,"-",1,0,'C',false);
				}
			}
			else if($modalidadeEnsino == "M") {
				if($j>=9 && $j<=11) {
					$PDF->Cell(1.5,0.5,$alunadoParticipante[0]["qtd"],1,0,'C',false);
					$somaAlunado += (integer)$alunadoParticipante[0]["qtd"];
				}
				else {
					$PDF->Cell(1.5,0.5,"-",1,0,'C',false);
				}
			}
			else {
				$PDF->Cell(1.5,0.5,$alunadoParticipante[0]["qtd"],1,0,'C',false);
				$somaAlunado += (integer)$alunadoParticipante[0]["qtd"];
			}
		}
		
		$PDF->Cell(0,0.5,$somaAlunado,1,1,'C',false);
		
		$PDF->Cell(6,0.5,"MACROCAMPO",1,0,'L',false);
		$PDF->Cell(6,0.5,"ATIVIDADE",1,0,'L',false);
		$PDF->Cell(3,0.5,"QTD DE ALUNOS POR ATIVIDADE",1,0,'C',false);
		$PDF->Cell(2,0.5,"VALOR KIT (capital)",1,0,'C',false);
		$PDF->Cell(0,0.5,"VALOR KIT (custeio)",1,1,'C',false);
		
		$sql = "SELECT
					mea.meaid,
					mtm.mtmdescricao,
					mta.mtadescricao,
					sum(mta.mtavlrcapital) as capital,
					sum(mta.mtavlrcusteio) as custeio
				FROM
					pdeescola.meatividade mea
				INNER JOIN
					pdeescola.metipoatividade mta ON mta.mtaid = mea.mtaid
								AND mta.mtasituacao = 'A'
				INNER JOIN
					pdeescola.metipomacrocampo mtm ON mtm.mtmid = mta.mtmid
								AND mtm.mtmsituacao = 'A'
				WHERE
					mea.memid = ".$entidades[$i]["memid"]." AND
					mea.meaano = " . $_SESSION["exercicio"] . "
				GROUP BY
					mea.meaid,
					mtm.mtmdescricao,
					mta.mtadescricao
				ORDER BY
					mea.meaid";
		$dadosAtividades = $db->carregar($sql);
		
		$qtdGeralAlunos = 0;
		$contAtividades = 0;
		$qtdAlunosPST	= 0;
		$kitCapitalPST	= 0;
		$kitCusteioPST	= 0;
		
		if($dadosAtividades) {
			
			for($k=0; $k<count($dadosAtividades); $k++) {
				
				if($modalidadeEnsino == "F")
					$inSeries = "1,2,3,4,5,6,7,8,9";
				else if($modalidadeEnsino == "M")
					$inSeries = "20,21,22";
				else
					$inSeries = "1,2,3,4,5,6,7,8,9,20,21,22";
					
				$sql = "SELECT
							map.mpaquantidade
						FROM
							pdeescola.mealunoparticipanteatividade map
						WHERE
							map.meaid = ".$dadosAtividades[$k]["meaid"]." AND 
							map.sceid IN (".$inSeries.")
						ORDER BY
							map.sceid";
				$numAlunosAtividade = $db->carregar($sql);
				
				$qtdAlunos = 0;
				for($z=0; $z<count($numAlunosAtividade); $z++) {
					$qtdAlunos += (integer)$numAlunosAtividade[$z]["mpaquantidade"];
				}
					
				if( strtoupper(trim($dadosAtividades[$k]["mtadescricao"])) != "PST" ) {
					$contAtividades++;
					$qtdGeralAlunos += (integer)$qtdAlunos;
					
					$PDF->Cell(6,0.5,$dadosAtividades[$k]["mtmdescricao"],1,0,'L',false);
					$PDF->Cell(6,0.5,$dadosAtividades[$k]["mtadescricao"],1,0,'L',false);
					$PDF->Cell(3,0.5,$qtdAlunos,1,0,'C',false);
					$PDF->Cell(2,0.5,(($dadosAtividades[$k]["capital"]) ? number_format($dadosAtividades[$k]["capital"],"2",",",".") : "0"),1,0,'C',false);
					$PDF->Cell(0,0.5,(($dadosAtividades[$k]["custeio"]) ? number_format($dadosAtividades[$k]["custeio"],"2",",",".") : "0"),1,1,'C',false);
					
				} else {
					$qtdAlunosPST 	= $qtdAlunos;
					$kitCapitalPST	= (($dadosAtividades[$k]["capital"]) ? number_format($dadosAtividades[$k]["capital"],"2",",",".") : "0");
					$kitCusteioPST	= (($dadosAtividades[$k]["custeio"]) ? number_format($dadosAtividades[$k]["custeio"],"2",",",".") : "0");
				}
			}
		}
		
		// recupera as informações sobre os kits/serviços e materiais/ressarcimento
		recuperaDadosPagamento($entidades[$i]["memid"], $entidades[$i]["entid"], $_SESSION["exercicio"], $kitCusteio, $kitCapital, $ressarcimentoMonitores, $servicosMateriais, $valorLimite, $pagamentoProfessorPST);
		
		// Finaliza os cálculos. Ressacimento não pode ser maior que o valor limite. 
		//$ressarcimentoMonitores = ($ressarcimentoMonitores > $valorLimite) ? $valorLimite : $ressarcimentoMonitores;
		$valorKits = ($kitCusteio + $kitCapital);
			
		/*** Regra da Clarissa (28/03/2011) ***/
		$porcentagemCusteio = ( $servicosMateriais * 0.8 );
		$porcentagemCapital = ( $servicosMateriais * 0.2 );
		
		// Calcula os Totais
		//$totalCusteio = ($kitCusteio + $pagamentoProfessorPST + $ressarcimentoMonitores + $servicosMateriais - $saldoAnoAnterior);
		$totalCusteio = ($kitCusteio + $pagamentoProfessorPST + $ressarcimentoMonitores + $porcentagemCusteio - $saldoAnoAnterior);
		
		//$totalCapital = $kitCapital;
		$totalCapital = ($kitCapital + $porcentagemCapital);
		
		// novo cálculo definido pelo Leandro
		if($totalCusteio < 0) {
			if(abs($totalCusteio) > $totalCapital)
				$totalCusteio = (($totalCapital) * (-1));
		}
		
		//$totalGeral = ($valorKits + $pagamentoProfessorPST + $ressarcimentoMonitores + $servicosMateriais - $saldoAnoAnterior);
		$totalGeral = ($valorKits + $pagamentoProfessorPST + $ressarcimentoMonitores + $servicosMateriais - $saldoAnoAnterior);
		
		if($totalGeral < 0) $totalGeral = 0;
		
		// soma os totais gerais
		$kitCusteioGeral 			 += $kitCusteio;
		$kitCapitalGeral 			 += $kitCapital;
		$ressarcimentoMonitoresGeral += $ressarcimentoMonitores;
		$servicosMateriaisGeral		 += $servicosMateriais;
		$totalGeralGeral			 += $totalGeral;
		$qtdGeralAlunosGeral		 += $qtdGeralAlunos;
		
		$PDF->Cell(12,0.5,"TOTAL DE ATIVIDADES: ".$contAtividades,1,0,'R',false);
		$PDF->Cell(3,0.5,$qtdGeralAlunos,1,0,'C',false);
		$PDF->Cell(2,0.5,number_format($kitCapital,"2",",","."),1,0,'C',false);
		$PDF->Cell(0,0.5,number_format($kitCusteio,"2",",","."),1,1,'C',false);
				
		if($qtdAlunosPST) {
			$PDF->Cell(12,0.5,"Pagamento Professor PST: ".number_format($pagamentoProfessorPST,"2",",","."),1,0,'C',false);
			$PDF->Cell(3,0.5,$qtdAlunosPST,1,0,'C',false);
			$PDF->Cell(2,0.5,number_format($kitCapitalPST,"2",",","."),1,0,'C',false);
			$PDF->Cell(0,0.5,number_format($kitCusteioPST,"2",",","."),1,1,'C',false);
			
			$PDF->Cell(12,0.5,"TOTAL GERAL ATIVIDADES: ".($contAtividades + 1),1,0,'R',false);
			$PDF->Cell(3,0.5,($qtdGeralAlunos + $qtdAlunosPST),1,0,'C',false);
			$PDF->Cell(2,0.5,number_format(($kitCapital+$kitCapitalPST),"2",",","."),1,0,'C',false);
			$PDF->Cell(0,0.5,number_format(($kitCusteio+$kitCusteioPST),"2",",","."),1,1,'C',false);
		}
		
		$PDF->Cell(0,0.5,"Ressarc. Monitores: ".number_format($ressarcimentoMonitores,"2",",",".")."      |      Serviços/Materiais: ".number_format($servicosMateriais,"2",",",".")."      |      Total Geral: ".number_format(($totalGeral),"2",",",".")."",1,1,'C',false);
		$PDF->Cell(0,0.5,'',1,1,'L',true);
	}
	
	$PDF->Ln();
	
	$PDF->Cell(0,0.5,'TOTAIS GERAIS',1,1,'L',true);
	$PDF->Cell(0,0.5,"N° de Escolas: ".count($entidades)."      |      Quantidade de Alunos: ".number_format($qtdGeralAlunosGeral,"0",",",".")."      |      Kits(custeio): ".number_format($kitCusteioGeral,"2",",",".")."      |      Kits(capital): ".number_format($kitCapitalGeral,"2",",",".")."      |      Ressarc. Monitores: ".number_format($ressarcimentoMonitoresGeral,"2",",",".")."      |      Serviços/Materiais: ".number_format($servicosMateriaisGeral,"2",",",".")."      |      Total Geral: ".number_format($totalGeralGeral,"2",",",".")."",1,1,'C',false);
	
	$PDF->Ln();
	
	$PDF->Cell(0,0.5,'AUTENTICAÇÃO',1,1,'L',true);
	$PDF->Cell(9.5,1," ",'LTR',0,'C',false);
	$PDF->Cell(9.5,1," ",'LTR',1,'C',false);
	$PDF->Cell(9.5,0.2,"__________________________________________________________________________________",'LR',0,'C',false);
	$PDF->Cell(9.5,0.2,"__________________________________________________________________________________",'LR',1,'C',false);
	$PDF->Cell(9.5,0.2,"LOCAL E DATA",'LBR',0,'C',false);
	$PDF->Cell(9.5,0.2,"DIRIGENTE DA EEx",'LBR',1,'C',false);
	
	
	$PDF->Cell(0,0.5,'ANÁLISE DA SECAD',1,1,'L',true);
	$PDF->Cell(9.5,1," ",'LTR',0,'C',false);
	$PDF->Cell(9.5,1," ",'LTR',1,'C',false);
	$PDF->Cell(9.5,0.2,"__________________________________________________________________________________",'LR',0,'C',false);
	$PDF->Cell(9.5,0.2,"__________________________________________________________________________________",'LR',1,'C',false);
	$PDF->Cell(9.5,0.2,"LOCAL E DATA",'LBR',0,'C',false);
	$PDF->Cell(9.5,0.2,"ANALISTA",'LBR',1,'C',false);
	
	$PDF->Cell(0,0.5,'APROVAÇÃO',1,1,'L',true);
	$PDF->Cell(9.5,1," ",'LTR',0,'C',false);
	$PDF->Cell(9.5,1," ",'LTR',1,'C',false);
	$PDF->Cell(9.5,0.2,"__________________________________________________________________________________",'LR',0,'C',false);
	$PDF->Cell(9.5,0.2,"__________________________________________________________________________________",'LR',1,'C',false);
	$PDF->Cell(9.5,0.2,"LOCAL E DATA",'LBR',0,'C',false);
	$PDF->Cell(9.5,0.2,"SECRETÁRIO DA SECAD",'LBR',1,'C',false);
}
else
{
	$PDF ->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
	$PDF->SetX(2.5);
	$PDF->Cell(0,0,'Não existem entidades com relatório consolidado anteriormente gerado.',0,1,'C');
}

$PDF->Output('geral_consolidado_pdf.pdf', 'D');

?>