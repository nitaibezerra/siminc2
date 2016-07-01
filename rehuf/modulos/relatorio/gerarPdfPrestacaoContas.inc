<?php

ini_set("memory_limit", "1024M");

//Chamando a classe de FPDF
//require('../../includes/fpdf/fpdf.inc');
require_once APPRAIZ . "includes/fpdf/fpdf.inc";
//require_once APPRAIZ . "adodb/adodb.inc.php";
//require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
//require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

if($_GET['prtid']){
$prtid = $_GET['prtid'];
}
if($_GET['pltid']){
$pltid = $_GET['pltid'];
}

if($_GET['entid']){
$entid = $_GET['entid'];
}else if($_GET['entungcod']){
	$entungcod = $_GET['entungcod'];
}

if($_GET['vpiid']){
$vpiid = $_GET['vpiid'];
}

	include_once APPRAIZ . "rehuf/classes/VinculoPlanoInterno.class.inc";
	include_once APPRAIZ . "rehuf/classes/PrestacaoContas.class.inc";
	include_once APPRAIZ . "includes/classes/modelo/entidade/Entidade.class.inc";
	include_once APPRAIZ . "includes/classes/Modelo.class.inc";
	
	$e = new Entidade();
	
if($entungcod){
	$entid = $e->buscarentidadeporentungcod($entungcod);
}

if($entid){
	
		$e->carregarPorId( $entid );
		extract($e->getDados());
		$arrPortariasentidade = $e->buscarUniversidadeByHospital($entid);
}

if($prtid || $pltid){
	$vpi = new VinculoPlanoInterno();
	extract($vpi->buscarplanointernoByPortariaORPlanoTrabalho($entungcod,$prtid,$pltid));
}

if($prcid){
	$prc = new PrestacaoContas();
	$arrRespostasQuestionario = $prc->buscarRespostaQuestionarioPorPrestacaoContas( $prcid );
}

if($vpiid){
	$arrVpqid = $vpi->buscarQuestionarioPlanoInterno($vpiid);
}


$PDF = new PDF_Table( 'P','cm','A4' ); //documento em formato de retrato, medido em cm e no tamanho A4
$PDF->SetMargins(1, 1, 1); // margem esquerda = 3 , superior = 3 e direita = 2.
$PDF->SetAuthor($GLOBALS['parametros_sistema_tela']['sigla']); //informando o autor do documento.
$PDF->SetTitle($GLOBALS['parametros_sistema_tela']['sigla']); //informando o título do documento.
$PDF->Footer();
$PDF->AddPage(); //adicionando um nova página


/*
 * CRIANDO CABEÇALHO DO PDF
 */
/*$PDF->Image("../imagens/brasao.JPG",1,0.8,1.2,1.2); //endereco da imagem,posicao X(horizontal),posicao Y(vertical), tamanho altura, tamanho largura
$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
$PDF->SetX(2.5);
$PDF->Cell(6,0,$GLOBALS['parametros_sistema_tela']['unidade_pai'],0,'P');
$PDF->SetX(2.5);
$PDF->Cell(6,0.5,'Palácio do Planalto',0,'P');
$PDF->SetX(2.5);
$PDF->Cell(6,1,'CEP:  - Brasília - DF - Brasil',0,'P');
*/

/*
 * FIM
 * CRIANDO CABEÇALHO DO PDF
 */


$PDF->SetY( 2.5 );
$PDF->SetX( 1 );
$PDF->SetFont( 'Arial', 'B', 8 ); 
$PDF ->SetFillColor( 234, 234, 234 ); 
$PDF->Cell( 19, 0.5, 'Prestação de Contas', 1, 0, 'C', 1 );
$PDF->ln();

$PDF->SetFont('Arial', '', 6);


// Parâmetros para a nova conexão com o banco do SIAFI
$servidor_bd = "";
$porta_bd    = "5432";
$nome_bd     = "";
$usuario_db  = "";
$senha_bd    = "";

$db2 = new cls_banco();

// Parâmetros da nova conexão com o banco do SIAFI para o componente 'combo_popup'.
$dados_conexao = array(
					'servidor_bd' => $servidor_bd,
					'porta_bd' => $porta_bd,
					'nome_bd' => $nome_bd,
					'usuario_db' => $usuario_db,
					'senha_bd' => $senha_bd
				);

if($docid){
	$esdid = pegarEstadoPrestacaoContas($docid);
	if( in_array($esdid, array(ESTADO_ENCERRADO))){
		$Enviado = 'S';
	}else{
		$Enviado = 'N';
	}
}



$arrCabecalho = array('Nº de Empenho','Natureza de Despesa Detalhada','CNPJ do Favorecido','Nome do Favorecido','Empenhos Emitidos R$','Empenhos Liquidados R$','Valores Pagos R$','Exerc.inscr.em RP n-proc');

if($prtnumero): 
	$PDF->MultiCell( 19, 0.5, "Portaria Nº {$prtnumero}, de ".exibeDataPorExtenso($prtdata)." - DOU {$prtnumerodou}, de ".formata_data($prtdtpublicacao).".", 0, 'J', 0);
endif;
if($plttitulo):
	$PDF->MultiCell( 19, 0.5, 'Plano Trabalho: ' . $plttitulo , 0, 'J', 0);
endif;

$PDF->MultiCell( 19, 0.5, 'Código Unidade Gestora: ' . $entungcod , 0, 'J', 0);
	$sql = "SELECT 
					ent.entid as codigo,
					upper(ena.entsig) as sigla, 
					ena.entnome as descricao 
				FROM 
					entidade.entidade ent
				LEFT JOIN 
					entidade.funcaoentidade fen ON fen.entid = ent.entid 
				INNER JOIN 
					entidade.funentassoc fue ON fue.fueid = fen.fueid
				LEFT JOIN entidade.entidade ena ON ena.entid = fue.entid
				LEFT JOIN 
					rehuf.estruturaunidade esu ON esu.entid = ent.entid
				WHERE 
					fen.funid IN ('16','93') 
				AND 
					(esu.esuindexibicao IS NULL OR esu.esuindexibicao = true)
				AND 
					ent.entid = {$entid}
				GROUP BY ena.entid, ena.entnome, ena.entsig,ent.entunicod,ent.entid
				ORDER BY ena.entsig";
			$ifes = $db->pegaLinha($sql);
			
$PDF->MultiCell( 19, 0.5, 'IFES: ' . $ifes['sigla']." - ".ucfirst($ifes['descricao']) , 0, 'J', 0);
$PDF->MultiCell( 19, 0.5, 'Plano Interno: ' . mb_strtoupper($plinumero) , 0, 'J', 0);

$PDF->ln();
$plinumero = mb_strtoupper($plinumero);
$PDF->MultiCell( 19, 0.5, 'Empenhos Custeio' , 1, 'C', 1);

$sql1 = "						SELECT
						 						conta_corrente AS cod_agrupador1, naturezadet AS cod_agrupador2, codigo_favorecido, it_no_credor,   
										       --'' AS dsc_agrupador1,naturezadet_desc AS dsc_agrupador2,
										       sum(valor1) AS coluna1,sum(valor2) AS coluna2,sum(valor3) AS coluna3,sum(valor4) AS coluna4
										FROM
										(SELECT 
										   sld.sldcontacorrente AS conta_corrente,sld.ctecod || '.' || sld.gndcod || '.' || sld.mapcod || '.' || sld.edpcod || '.' || substr(trim(sld.sldcontacorrente), 13, 2)::character varying(2) AS naturezadet,
										   sld.sldcontacorrente AS conta_corrente_desc,ndp.ndpdsc AS naturezadet_desc,
										   CASE WHEN sld.sldcontacontabil in ('292410101', '292410402', '292410403', '292410405') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('292410402','292410403') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor2,CASE WHEN sld.sldcontacontabil in ('292410403') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor3,CASE WHEN sld.sldcontacontabil in ('292410405') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor4
									       FROM
										   dw.saldo2012 sld
										   LEFT JOIN ( select distinct ctecod, gndcod, mapcod, edpcod, sbecod, ndpdsc from dw.naturezadespesa where sbecod <> '00' ) as ndp ON cast(ndp.ctecod AS text) = sld.ctecod AND cast(ndp.gndcod AS text) = sld.gndcod AND cast(ndp.mapcod AS text) = sld.mapcod AND cast(ndp.edpcod AS text) = sld.edpcod AND cast(ndp.sbecod AS text) = sld.sbecod
										   WHERE sld.ungcod in ('{$entungcod}') AND sld.plicod = '{$plinumero}' AND sld.sldcontacontabil in ( '292410101', '292410402', '292410403', '292410405', '292410402','292410403', '292410403', '292410405' )
										    UNION ALL	
										   SELECT 
										   sld.sldcontacorrente AS conta_corrente,null,
										   sld.sldcontacorrente AS conta_corrente_desc,null,
										   CASE WHEN sld.sldcontacontabil in ('292410101', '292410402', '292410403', '292410405') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('292410402','292410403') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor2,CASE WHEN sld.sldcontacontabil in ('292410403') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor3,CASE WHEN sld.sldcontacontabil in ('292410405') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor4 
									       FROM 
										   dw.saldo2012 sld
										   LEFT JOIN ( select distinct ctecod, gndcod, mapcod, edpcod, sbecod, ndpdsc from dw.naturezadespesa where sbecod <> '00' ) as ndp ON cast(ndp.ctecod AS text) = sld.ctecod AND cast(ndp.gndcod AS text) = sld.gndcod AND cast(ndp.mapcod AS text) = sld.mapcod AND cast(ndp.edpcod AS text) = sld.edpcod AND cast(ndp.sbecod AS text) = sld.sbecod
										   WHERE sld.ungcod in ('{$entungcod}') AND sld.plicod = '{$plinumero}' AND sld.sldcontacontabil in ('292410101', '292410402', '292410403', '292410405', '292410402','292410403', '292410403', '292410405')
									   ) as foo
									   inner join ( select numero_ne, codigo_favorecido, it_no_credor
												from siafi2012.ne ne 
												inner join dw.credor c ON c.it_co_credor = ne.codigo_favorecido 
												where codigo_ug_operador = '{$entungcod}' 
												and trim(plano_interno) = '{$plinumero}' 
												 ) ne ON
													numero_ne = '{$entungcod}'||(select orgcodgestao from dw.uguo where ungcod = '{$entungcod}' order by ugoid desc limit 1)||substr(foo.conta_corrente, 1,12)		
										WHERE
										naturezadet != '' and ( valor1 <> 0 OR valor2 <> 0 OR valor3 <> 0 OR valor4 <> 0 )and naturezadet ilike '3%'
										GROUP BY
										conta_corrente,codigo_favorecido, it_no_credor, naturezadet,
										conta_corrente_desc,naturezadet_desc
										ORDER BY
										conta_corrente,naturezadet,
										conta_corrente_desc,naturezadet_desc";
		       $arrDados = $db2->carregar($sql1);
		       //$arrCabecalho = array('Nº de Empenho','Natureza de Despesa Detalhada','CNPJ do Favorecido','Nome do Favorecido','Empenhos Emitidos R$','Empenhos Liquidados R$','Valores Pagos R$','Exerc.inscr.em RP n-proc');
		       if($arrDados){
					$n = 0;
					
					$PDF->SetFont('Arial', '', 5);
					$PDF->Cell( 2, 0.5, 'Nº de Empenho', 1, 0, 'C', 1 );
					$PDF->Cell( 1.5, 0.5, 'Nat.Desp.Det.', 1, 0, 'C', 1 );
					$PDF->Cell( 2, 0.5, 'CNPJ do Favorecido', 1, 0, 'C', 1 );
					$PDF->Cell( 7.5, 0.5, 'Nome do Favorecido', 1, 0, 'C', 1 );
					$PDF->Cell( 1.5, 0.5, 'Emp. Emitidos R$', 1, 0, 'C', 1 );
					$PDF->Cell( 1.5, 0.5, 'Emp. Liq. R$', 1, 0, 'C', 1 );
					$PDF->Cell( 1.5, 0.5, 'Pagos R$', 1, 0, 'C', 1 );
					$PDF->Cell( 1.5, 0.5, 'Ex.inscr.RP n-proc', 1, 0, 'C', 1 );
					$PDF->ln();
					
					foreach($arrDados as $dado)
					{
						$cor = ($i % 2) ? '255,255,255' : '245,245,235';	
					
						$arrLista[$n]['n_empenho'] = $dado['cod_agrupador1'];
						$arrLista[$n]['natureza_despesa_detalhada'] = MoedaToBd($dado['cod_agrupador2']);
						$arrLista[$n]['cnpj_favorecido'] = formatar_cnpj($dado['codigo_favorecido']);
						$arrLista[$n]['nome_favorecido'] = $dado['it_no_credor'];
						$arrLista[$n]['empenhos_emitidos'] = formata_valor($dado['coluna1'],'2',true);
						$arrLista[$n]['empenhos_liquidados'] = formata_valor($dado['coluna2'],'2',true);
						$arrLista[$n]['valores_pagos'] = formata_valor($dado['coluna3'],'2',true);
						$arrLista[$n]['n-proc'] = formata_valor($dado['coluna4'],'2',true);
						
						$PDF->SetFont('Arial', '', 5);
						$PDF->SetFillColor( $cor );
						$PDF->Cell( 2, 0.5, $arrLista[$n]['n_empenho'], 1, 0, 'J', 1 );
						$PDF->Cell( 1.5, 0.5, $arrLista[$n]['natureza_despesa_detalhada'], 1, 0, 'J', 1 );
						$PDF->Cell( 2, 0.5, $arrLista[$n]['cnpj_favorecido'], 1, 0, 'J', 1 );
						$PDF->Cell( 7.5, 0.5, $arrLista[$n]['nome_favorecido'], 1, 0, 'J', 1 );
						$PDF->Cell( 1.5, 0.5, $arrLista[$n]['empenhos_emitidos'], 1, 0, 'J', 1 );
						$PDF->Cell( 1.5, 0.5, $arrLista[$n]['empenhos_liquidados'], 1, 0, 'J', 1 );
						$PDF->Cell( 1.5, 0.5, $arrLista[$n]['valores_pagos'], 1, 0, 'J', 1 );
						$PDF->Cell( 1.5, 0.5, $arrLista[$n]['n-proc'], 1, 0, 'J', 1 );
						$PDF->ln();
						
						$valor_totalc1+=$dado['coluna1'];
						$valor_totalc2+=$dado['coluna2'];
						$valor_totalc3+=$dado['coluna3'];
						$valor_totalc4+=$dado['coluna4'];
						
						$n++;
					}
					$cor1 = ($i % 2) ? '245,245,245' : '235,235,225';
					$PDF->SetFillColor( $cor1 );
					$PDF->Cell( 2, 0.5, 'Totais:', 0, 0, 'C', 1 );
					$PDF->Cell( 1.5, 0.5, '', 0, 0, 'C', 1 );
					$PDF->Cell( 2, 0.5, '', 0, 0, 'C', 1 );
					$PDF->Cell( 7.5, 0.5, '', 0, 0, 'C', 1 );
					$PDF->Cell( 1.5, 0.5, formata_valor($valor_totalc1,'2',true), 0, 0, 'C', 1 );
					$PDF->Cell( 1.5, 0.5, formata_valor($valor_totalc2,'2',true), 0, 0, 'C', 1 );
					$PDF->Cell( 1.5, 0.5, formata_valor($valor_totalc3,'2',true), 0, 0, 'C', 1 );
					$PDF->Cell( 1.5, 0.5, formata_valor($valor_totalc4,'2',true), 0, 0, 'C', 1 );

					$PDF->ln();
		       }	
$PDF->ln();
$plinumero = mb_strtoupper($plinumero);
$PDF->MultiCell( 19, 0.5, 'Empenhos Capital' , 1, 'C', 1);
$sql2 = "						SELECT
						 						conta_corrente AS cod_agrupador1,naturezadet AS cod_agrupador2, codigo_favorecido, it_no_credor,  
										       --'' AS dsc_agrupador1,naturezadet_desc AS dsc_agrupador2,
										       sum(valor1) AS coluna1,sum(valor2) AS coluna2,sum(valor3) AS coluna3,sum(valor4) AS coluna4
										FROM
										(SELECT 
										   sld.sldcontacorrente AS conta_corrente,sld.ctecod || '.' || sld.gndcod || '.' || sld.mapcod || '.' || sld.edpcod || '.' || substr(trim(sld.sldcontacorrente), 13, 2)::character varying(2) AS naturezadet,
										   sld.sldcontacorrente AS conta_corrente_desc,ndp.ndpdsc AS naturezadet_desc,
										   CASE WHEN sld.sldcontacontabil in ('292410101', '292410402', '292410403', '292410405') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('292410402','292410403') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor2,CASE WHEN sld.sldcontacontabil in ('292410403') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor3,CASE WHEN sld.sldcontacontabil in ('292410405') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor4
									       FROM
										   dw.saldo2012 sld
										   LEFT JOIN ( select distinct ctecod, gndcod, mapcod, edpcod, sbecod, ndpdsc from dw.naturezadespesa where sbecod <> '00' ) as ndp ON cast(ndp.ctecod AS text) = sld.ctecod AND cast(ndp.gndcod AS text) = sld.gndcod AND cast(ndp.mapcod AS text) = sld.mapcod AND cast(ndp.edpcod AS text) = sld.edpcod AND cast(ndp.sbecod AS text) = sld.sbecod
										   WHERE sld.ungcod in ('{$entungcod}') AND sld.plicod = '{$plinumero}' AND sld.sldcontacontabil in ( '292410101', '292410402', '292410403', '292410405', '292410402','292410403', '292410403', '292410405' )
										    UNION ALL	
										   SELECT 
										   sld.sldcontacorrente AS conta_corrente,null,
										   sld.sldcontacorrente AS conta_corrente_desc,null,
										   CASE WHEN sld.sldcontacontabil in ('292410101', '292410402', '292410403', '292410405') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('292410402','292410403') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor2,CASE WHEN sld.sldcontacontabil in ('292410403') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor3,CASE WHEN sld.sldcontacontabil in ('292410405') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor4 
									       FROM 
										   dw.saldo2012 sld
										   LEFT JOIN ( select distinct ctecod, gndcod, mapcod, edpcod, sbecod, ndpdsc from dw.naturezadespesa where sbecod <> '00' ) as ndp ON cast(ndp.ctecod AS text) = sld.ctecod AND cast(ndp.gndcod AS text) = sld.gndcod AND cast(ndp.mapcod AS text) = sld.mapcod AND cast(ndp.edpcod AS text) = sld.edpcod AND cast(ndp.sbecod AS text) = sld.sbecod
										   WHERE sld.ungcod in ('{$entungcod}') AND sld.plicod = '{$plinumero}' AND sld.sldcontacontabil in ('292410101', '292410402', '292410403', '292410405', '292410402','292410403', '292410403', '292410405')
									   ) as foo
									   inner join ( select numero_ne, codigo_favorecido, it_no_credor
												from siafi2012.ne ne 
												inner join dw.credor c ON c.it_co_credor = ne.codigo_favorecido 
												where codigo_ug_operador = '{$entungcod}' 
												and trim(plano_interno) = '{$plinumero}' 
												 ) ne ON
													numero_ne = '{$entungcod}'||(select orgcodgestao from dw.uguo where ungcod = '{$entungcod}' order by ugoid desc limit 1)||substr(foo.conta_corrente, 1,12)		
										WHERE
										naturezadet != '' and ( valor1 <> 0 OR valor2 <> 0 OR valor3 <> 0 OR valor4 <> 0 )and naturezadet ilike '4%'
										GROUP BY
										conta_corrente,codigo_favorecido, it_no_credor, naturezadet,
										conta_corrente_desc,naturezadet_desc
										ORDER BY
										conta_corrente,naturezadet,
										conta_corrente_desc,naturezadet_desc";
		       $arrDados2 = $db2->carregar($sql2);
		       //$arrCabecalho = array('Nº de Empenho','Natureza de Despesa Detalhada','CNPJ do Favorecido','Nome do Favorecido','Empenhos Emitidos R$','Empenhos Liquidados R$','Valores Pagos R$','Exerc.inscr.em RP n-proc');
		       if($arrDados2){
					$n = 0;

					$PDF->SetFont('Arial', '', 5);
					$PDF->Cell( 2, 0.5, 'Nº de Empenho', 1, 0, 'C', 1 );
					$PDF->Cell( 1.5, 0.5, 'Nat.Desp.Det.', 1, 0, 'C', 1 );
					$PDF->Cell( 2, 0.5, 'CNPJ do Favorecido', 1, 0, 'C', 1 );
					$PDF->Cell( 7.5, 0.5, 'Nome do Favorecido', 1, 0, 'C', 1 );
					$PDF->Cell( 1.5, 0.5, 'Emp. Emitidos R$', 1, 0, 'C', 1 );
					$PDF->Cell( 1.5, 0.5, 'Emp. Liq. R$', 1, 0, 'C', 1 );
					$PDF->Cell( 1.5, 0.5, 'Pagos R$', 1, 0, 'C', 1 );
					$PDF->Cell( 1.5, 0.5, 'Ex.inscr.RP n-proc', 1, 0, 'C', 1 );
					$PDF->ln();
					
					foreach($arrDados2 as $dado)
					{
						$cor = ($i % 2) ? '255,255,255' : '245,245,235';	
					
						$arrLista2[$n]['n_empenho'] = $dado['cod_agrupador1'];
						$arrLista2[$n]['natureza_despesa_detalhada'] = MoedaToBd($dado['cod_agrupador2']);
						$arrLista2[$n]['cnpj_favorecido'] = formatar_cnpj($dado['codigo_favorecido']);
						$arrLista2[$n]['nome_favorecido'] = $dado['it_no_credor'];
						$arrLista2[$n]['empenhos_emitidos'] = formata_valor($dado['coluna1'],'2',true);
						$arrLista2[$n]['empenhos_liquidados'] = formata_valor($dado['coluna2'],'2',true);
						$arrLista2[$n]['valores_pagos'] = formata_valor($dado['coluna3'],'2',true);
						$arrLista2[$n]['n-proc'] = formata_valor($dado['coluna4'],'2',true);
						
						$PDF->SetFont('Arial', '', 5);
						$PDF->SetFillColor( $cor );
						$PDF->Cell( 2, 0.5, $arrLista2[$n]['n_empenho'], 1, 0, 'J', 1 );
						$PDF->Cell( 1.5, 0.5, $arrLista2[$n]['natureza_despesa_detalhada'], 1, 0, 'J', 1 );
						$PDF->Cell( 2, 0.5, $arrLista2[$n]['cnpj_favorecido'], 1, 0, 'J', 1 );
						$PDF->Cell( 7.5, 0.5, $arrLista2[$n]['nome_favorecido'], 1, 0, 'J', 1 );
						$PDF->Cell( 1.5, 0.5, $arrLista2[$n]['empenhos_emitidos'], 1, 0, 'J', 1 );
						$PDF->Cell( 1.5, 0.5, $arrLista2[$n]['empenhos_liquidados'], 1, 0, 'J', 1 );
						$PDF->Cell( 1.5, 0.5, $arrLista2[$n]['valores_pagos'], 1, 0, 'J', 1 );
						$PDF->Cell( 1.5, 0.5, $arrLista2[$n]['n-proc'], 1, 0, 'J', 1 );
						$PDF->ln();
						
						$n++;
						
						$valor_total1+=$dado['coluna1'];
						$valor_total2+=$dado['coluna2'];
						$valor_total3+=$dado['coluna3'];
						$valor_total4+=$dado['coluna4'];
					}
					$cor1 = ($i % 2) ? '245,245,245' : '235,235,225';
					$PDF->SetFillColor( $cor1 );
					$PDF->Cell( 2, 0.5, 'Totais:', 0, 0, 'C', 1 );
					$PDF->Cell( 1.5, 0.5, '', 0, 0, 'C', 1 );
					$PDF->Cell( 2, 0.5, '', 0, 0, 'C', 1 );
					$PDF->Cell( 7.5, 0.5, '', 0, 0, 'C', 1 );
					$PDF->Cell( 1.5, 0.5, formata_valor($valor_total1,'2',true), 0, 0, 'C', 1 );
					$PDF->Cell( 1.5, 0.5, formata_valor($valor_total2,'2',true), 0, 0, 'C', 1 );
					$PDF->Cell( 1.5, 0.5, formata_valor($valor_total3,'2',true), 0, 0, 'C', 1 );
					$PDF->Cell( 1.5, 0.5, formata_valor($valor_total4,'2',true), 0, 0, 'C', 1 );

					$PDF->ln();
		       }
$PDF->ln();


$sql = "SELECT
		     --  ungcod AS cod_agrupador1,natureza AS cod_agrupador2,
		      -- ungdsc AS dsc_agrupador1,natureza_desc AS dsc_agrupador2,
		       sum(valor1) AS provisaorecebida,sum(valor2) AS destaquerecebido,sum(valor3) AS provisaoconcedida,sum(valor4) AS destaqueconcedido,sum(valor5) AS creditodisponivel
		       FROM
		       (SELECT 
			  sld.ungcod,sld.ctecod || '.' || sld.gndcod || '.' || sld.mapcod || '.' || sld.edpcod AS natureza,
			  ung.ungdsc,ndp.ndpdsc AS natureza_desc,
			  CASE WHEN sld.sldcontacontabil in ('192220100') THEN 
	CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
	ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('192210101','192210102','192210201','192210202','192210300','192210901','192210909') THEN 
	CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
	ELSE 0 END AS valor2,CASE WHEN sld.sldcontacontabil in ('292220100','292220200','292220901','292220909') THEN 
	CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
	ELSE 0 END AS valor3,CASE WHEN sld.sldcontacontabil in ('292210101','292210201','292210300','292210901','292210909') THEN 
	CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
	ELSE 0 END AS valor4,CASE WHEN sld.sldcontacontabil in ('292110000') THEN 
	CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
	ELSE 0 END AS valor5
	      FROM
			  dw.saldo2012 sld
			  LEFT JOIN dw.ug ung ON ung.ungcod = sld.ungcod LEFT JOIN ( select distinct ctecod, gndcod, mapcod, edpcod, ndpdsc from dw.naturezadespesa where sbecod = '00' ) as ndp ON cast(ndp.ctecod AS text) = sld.ctecod AND cast(ndp.gndcod AS text) = sld.gndcod AND cast(ndp.mapcod AS text) = sld.mapcod AND cast(ndp.edpcod AS text) = sld.edpcod
			  WHERE sld.acacod in ('20RX') AND sld.sldcontacontabil in ('192220100','192210101','192210102','192210201','192210202','192210300','192210901','192210909','292220100','292220200','292220901','292220909','292210101','292210201','292210300','292210901','292210909','292110000') 
				      and sld.ungcod = '{$entungcod}'
	) as foo
		       WHERE
		       ( valor1 <> 0 OR valor2 <> 0 OR valor3 <> 0 OR valor4 <> 0 OR valor5 <> 0 ) and ungcod = '{$entungcod}'
		     --  GROUP BY
		     --  ungcod,natureza,
		     --  ungdsc,natureza_desc
		     --  ORDER BY
		     --  ungcod,natureza,
		     --  ungdsc,natureza_desc";
		     
$arrDados = $db2->Carregar($sql);
if($arrDados[0]):
	foreach($arrDados as $dado):
		$PDF->MultiCell( 9, 0.5, number_format($dado['provisaoconcedida'],2,',','.').$PDF->Cell( 10, 0.5, 'Crédito Descentralizado Provisão Concedida: ' , 1, 'L', 1) , 1, 'R', 1);	
		$PDF->MultiCell( 9, 0.5, number_format($dado['provisaorecebida'],2,',','.').$PDF->Cell( 10, 0.5, 'Crédito Descentralizado Provisão Recebida: ' , 1, 'L', 1) , 1, 'R', 1);	
		$PDF->MultiCell( 9, 0.5, number_format($dado['destaqueconcedido'],2,',','.').$PDF->Cell( 10, 0.5, 'Crédito Descentralizado Destaque Concedido: ' , 1, 'L', 1) , 1, 'R', 1);	
		$PDF->MultiCell( 9, 0.5, number_format($dado['destaquerecebido'],2,',','.').$PDF->Cell( 10, 0.5, 'Crédito Descentralizado Destaque Recebido: ' , 1, 'L', 1) , 1, 'R', 1);	
		$PDF->MultiCell( 9, 0.5, number_format($dado['creditodisponivel'],2,',','.').$PDF->Cell( 10, 0.5, 'Crédito Disponível: ' , 1, 'L', 1) , 1, 'R', 1);	
	endforeach;
endif;

$PDF->ln();

$PDF->MultiCell( 19, 0.5, 'Comentários ', 1, 'C', 1);
if($arrVpqid[0]){
	$sql = "SELECT 
					qp.perid,
					qp.pertitulo,
					qp.queid
				FROM 
					rehuf.vinculointernoquestionario vpiq
				inner join 
					questionario.pergunta qp ON vpiq.queid=qp.queid
				WHERE vpqid in(".implode(",",$arrVpqid).")
				ORDER BY qp.perid";
	$arrQuestionario = $db->carregar($sql);
	if($arrQuestionario):
 		$n = 0; 
		$numero = 1;
			foreach($arrQuestionario as $dado):
				$PDF->MultiCell( 19, 0.5, $numero.' - '.strip_tags($dado['pertitulo']) , 1, 'J', 0);
				$PDF->MultiCell( 19, 0.5, $arrRespostasQuestionario[$dado['queid']][$dado['perid']]['resdsc'] , 0, 'J', 0);
				$n++;
				$numero++;
				$PDF->ln();
			endforeach;
	endif;
}

	$sql = "select distinct
						segu.usucpf as cpf,
						segu.usunome as nome,
						prc.dataatualizacao as data
			from 
					seguranca.usuario segu
			inner join 
					rehuf.prestacaocontas prc on prc.usucpf = segu.usucpf
			inner join 
					rehuf.vinculoplanointerno vpi on vpi.vpiid = prc.vpiid
			where 
					vpi.vpiid = {$vpiid}
			and
					prc.prcug = '{$entungcod}'";
	$assinatura = $db->pegaLinha($sql);
if($assinatura){
	if( in_array($esdid, array(ESTADO_EM_ELABORACAO))){
		$PDF->MultiCell( 19, 0.5, 'Última Atualização: ' . $assinatura['nome'].' ' .formatar_cpf($assinatura['cpf']).' - '.strftime("%d/%m/%Y %H:%M:%S", strtotime($assinatura['data'])) , 1, 'C', 0);
	}else{
		$PDF->MultiCell( 19, 0.5, 'Enviado por: ' . $assinatura['nome'].' ' .formatar_cpf($assinatura['cpf']).' - '.strftime("%d/%m/%Y %H:%M:%S", strtotime($assinatura['data'])) , 1, 'C', 0);
	}
}
$PDF->Output();

?>
