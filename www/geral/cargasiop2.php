<?

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

ini_set( "memory_limit", "1024M" ); // ...
set_time_limit(0);

$db = new cls_banco();

$sql = "select login, 
	senha, 
	orgao, 
	a.acaid, 
	esfcod, 
	unicod, 
	funcod, 
	sfucod, 
	prgcod, 
	acacod, 
	loccod, 
	a.tipodetalhamento, 
	quantidadefisico, 
	( select valorfisico from carga.auxiliar2 z where z.acaid = a.acaid and z.tipodetalhamento = a.tipodetalhamento ) as valorfisico, 
	justificativa, 
	ano, 
	iducod, 
	idocod, 
	ndpcod, 
	foncod, 
	valor, 

	nrcod, 
	valor_receita 
 from
(

select 	
	login, 
	senha, 
	orgao, 
	acaid, 
	esfcod, 
	unicod, 
	funcod, 
	sfucod, 
	prgcod, 
	acacod, 
	loccod, 
	case when acacod in('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110') and substr(ndpcod, 1,2)<>'31' then '1' else tdecod end as tipodetalhamento, 
	quantidadefisico, 
	valor as valorfisico, 
	justificativa, 
	ano, 
	iducod, 
	idocod, 
	ndpcod, 
	foncod, 
	valor, 

	nrcod, 
	valor_receita 
	
 from 
( 	
select  'EYX' as login, 
	md5('COISUCA') as senha, 
	 '". CODIGO_ORGAO_SISTEMA. "' AS  orgao, 
	acao.acaid, 
	acao.esfcod, 
	acao.unicod, 
	acao.funcod, 
	acao.sfucod, 
	acao.prgcod, 
	acao.acacod, 
	acao.loccod, 
	acao.tdecod,
	Case trim(coalesce(acao.acadscprosof,''))
		when '' then '' else coalesce(acaqtdefisico,1)::character varying(10) end as quantidadefisico, 
	--acaqtdefisico as quantidadefisico, 
	acaqtdefinanceiro as valorfisico, 
	justificativa as justificativa, 
	'2010' as ano, 
	da.iducod, 
	idoc.idocod, 
	nd.ndpcod as ndpcod, 
	da.foncod, 
	coalesce ( SUM(da.dpavalor), 0 ) as valor, 

	'' as nrcod, 
	0 as valor_receita 
 from elabrev.despesaacao da 
 	inner join elabrev.ppaacao_orcamento acao on acao.acaid = da.acaid 
	inner join naturezadespesa nd ON nd.ndpid = da.ndpid 
	inner join idoc on idoc.idoid = da.idoid 
	inner join ( select ao.acaid, max(tpdid) as tpdid from elabrev.tipodetalhamentoacao a 
			inner join elabrev.ppaacao_orcamento ao ON ao.acaid = a.acaid 
			where ao.prgano ='2009' 
			group by ao.acaid ) tda ON tda.acaid = acao.acaid
	--left join naturezareceita nr ON nr.nrcid = da.nrcid
	where 1=1  and da.ppoid = 153 and
		nd.ndpcod in ( '33903017',
'33903047',
'33903504',
'33903618',
'33903654',
'33903655',
'33903908',
'33903911',
'33903926',
'33903927',
'33903928',
'33903930',
'33903931',
'33903957',
'33903995',
'33903997',
'44903017',
'44903047',
'44903504',
'44903618',
'44903654',
'44903655',
'44903957',
'44903995',
'44905235'
 )
 group by 
	acao.acaid,
	 acao.esfcod, 
	acao.unicod, 
	acao.funcod, 
	acao.sfucod, 	
	acao.prgcod, 
	acao.acacod, 
	acao.loccod, 
	tda.tpdid,
	acaqtdefisico , 
	acaqtdefinanceiro , 
	justificativa ,	
	nd.ndpcod, 
	
	da.iducod, 
	da.foncod, 
	idoc.idocod, 
	acao.acaqtdefisico, 
	acao.tdecod, 
	trim(coalesce(acao.acadscprosof,'')),
	acao.justificativa 

union all

select  'EYX' as login,
	md5('COISUCA') as senha,
	 '". CODIGO_ORGAO_SISTEMA. "' AS  orgao,
	acao.acaid,
	acao.esfcod, 
	acao.unicod, 
	acao.funcod, 
	acao.sfucod, 
	acao.prgcod, 
	acao.acacod, 
	acao.loccod, 
	acao.tdecod,
	Case trim(coalesce(acao.acadscprosof,''))
		when '' then '' else coalesce(acaqtdefisico,1)::character varying(10) end as quantidadefisico, 
	--acaqtdefisico as quantidadefisico, 
	acaqtdefinanceiro as valorfisico, 
	justificativa as justificativa, 
	'2010' as ano, 
	da.iducod, 
	idoc.idocod, 
	'' as ndpcod, 
	da.foncod, 
	0 as valor, 

	nr.nrccod, 
	coalesce ( SUM(da.dpavalor), 0 ) as valor_receita
 from elabrev.despesaacao da 
 	inner join elabrev.ppaacao_orcamento acao on acao.acaid = da.acaid 
	inner join naturezareceita nr ON nr.nrcid = da.nrcid
	inner join idoc on idoc.idoid = da.idoid 
	inner join ( select ao.acaid, max(tpdid) as tpdid from elabrev.tipodetalhamentoacao a 
			inner join elabrev.ppaacao_orcamento ao ON ao.acaid = a.acaid 
			where ao.prgano ='2009' 
			group by ao.acaid ) tda ON tda.acaid = acao.acaid
	where 1=1  and da.ppoid = 153 and
			nr.nrccod in ( '33903017',
'33903047',
'33903504',
'33903618',
'33903654',
'33903655',
'33903908',
'33903911',
'33903926',
'33903927',
'33903928',
'33903930',
'33903931',
'33903957',
'33903995',
'33903997',
'44903017',
'44903047',
'44903504',
'44903618',
'44903654',
'44903655',
'44903957',
'44903995',
'44905235'
 )
 group by 
	acao.acaid,
	 acao.esfcod, 
	acao.unicod, 
	acao.funcod, 
	acao.sfucod, 	
	acao.prgcod, 
	acao.acacod, 
	acao.loccod, 
	tda.tpdid,
	acaqtdefisico , 
	acaqtdefinanceiro , 
	justificativa ,	
	da.iducod, 
	da.foncod, 
	nr.nrccod,
	idoc.idocod, 
	acao.acaqtdefisico, 
	acao.tdecod, 
	trim(coalesce(acao.acadscprosof,'')),
	acao.justificativa 
) as foo 
where tdecod not in ( '2', '4', '7' ) -- and unicod <> '26291' and prgcod = '1374' and acacod = '6321'
group by 	login, 
	senha, 
	orgao, 
	acaid, 
	esfcod, 
	unicod, 
	funcod, 
	sfucod, 
	prgcod, 
	acacod, 
	loccod, 
	tipodetalhamento, 
	quantidadefisico, 
	justificativa, 
	ano, 
	iducod, 
	idocod, 
	ndpcod, 
	foncod, 
	valor, 
	nrcod, 
	valor_receita ,
	tdecod
order by  esfcod, 
	unicod, 
	funcod, 
	sfucod, 	
	prgcod, 
	acacod, 
	loccod, tipodetalhamento

) as a
group by login, 
	senha, 
	orgao, 
	a.acaid, 
	esfcod, 
	unicod, 
	funcod, 
	sfucod, 
	prgcod, 
	acacod, 
	loccod, 
	a.tipodetalhamento, 
	quantidadefisico, 
	justificativa, 
	ano, 
	iducod, 
	idocod, 
	ndpcod, 
	foncod, 
	valor, 

	nrcod, 
	valor_receita 
 order by a.acaid, tipodetalhamento ";

$dadosXML = $db->carregar($sql);

$xml   = '';
$xml  .= '<?xml version="1.0" encoding="ISO-8859-1"?>
			<integracao>
				<informacoesRegistro dataHoraRegistro="2008-07-13T18:54:37-05:00">
					<usuario>
						<login>'.$dadosXML[0]["login"].'</login>
						<senha>'.$dadosXML[0]["senha"].'</senha>
						<codigoOrgao>'.$dadosXML[0]["orgao"].'</codigoOrgao>
					</usuario>
				</informacoesRegistro>
				<propostas>';


$acaid = 0;
$tpecod = 0;

$financeiros 	= '';
$receitas		= '';
$cont = 0;


for($i=0; $i<count($dadosXML); $i++) { 
	
	if($dadosXML[$i]["acaid"] != $acaid ) {
		
			if($i > 0) { 
			$xml .= '<financeiros>'.$financeiros.'</financeiros>';
			
			if ( $cont > 0 ) {
					 $xml .= '<receitas>'.$receitas.'</receitas>';
					 $cont = 0;
			}

					 
				$xml .= '</proposta>'; 
			
			$financeiros 	= '';
			$receitas		= '';
		}
		
		
		$acaid = $dadosXML[$i]["acaid"];
		$tpecod = $dadosXML[$i]["tipodetalhamento"];
		
	
		$xml .= '<proposta>
					<esfera>'.$dadosXML[$i]["esfcod"].'</esfera>
					<unidade>'.$dadosXML[$i]["unicod"].'</unidade>
					<funcao>'.$dadosXML[$i]["funcod"].'</funcao>
					<subfuncao>'.$dadosXML[$i]["sfucod"].'</subfuncao>
					<programa>'.$dadosXML[$i]["prgcod"].'</programa>
					<acao>'.$dadosXML[$i]["acacod"].'</acao>
					<localizador>'.$dadosXML[$i]["loccod"].'</localizador>
					<tipoDetalhamento>'.$dadosXML[$i]["tipodetalhamento"].'</tipoDetalhamento>
					<quantidadeFisico>'.$dadosXML[$i]["quantidadefisico"].'</quantidadeFisico>
					<valorFisico>'.$dadosXML[$i]["valorfisico"].'</valorFisico>
					<justificativa>'.$dadosXML[$i]["justificativa"].'</justificativa>
					<exercicio>'.$dadosXML[$i]["ano"].'</exercicio>';
	}elseif($dadosXML[$i]["tipodetalhamento"] != $tpecod ) {
		
			if($i > 0) { 
			$xml .= '<financeiros>'.$financeiros.'</financeiros>';
			
			if ( $cont > 0 ) {
					 $xml .= '<receitas>'.$receitas.'</receitas>';
					 $cont = 0;
			}

					 
				$xml .= '</proposta>'; 
			
			$financeiros 	= '';
			$receitas		= '';
			}
		
		
		$acaid = $dadosXML[$i]["acaid"];
		$tpecod = $dadosXML[$i]["tipodetalhamento"];
		
	
		$xml .= '<proposta>
					<esfera>'.$dadosXML[$i]["esfcod"].'</esfera>
					<unidade>'.$dadosXML[$i]["unicod"].'</unidade>
					<funcao>'.$dadosXML[$i]["funcod"].'</funcao>
					<subfuncao>'.$dadosXML[$i]["sfucod"].'</subfuncao>
					<programa>'.$dadosXML[$i]["prgcod"].'</programa>
					<acao>'.$dadosXML[$i]["acacod"].'</acao>
					<localizador>'.$dadosXML[$i]["loccod"].'</localizador>
					<tipoDetalhamento>'.$dadosXML[$i]["tipodetalhamento"].'</tipoDetalhamento>
					<quantidadeFisico></quantidadeFisico>
					<valorFisico>'.$dadosXML[$i]["valorfisico"].'</valorFisico>
					<justificativa>'.$dadosXML[$i]["justificativa"].'</justificativa>
					<exercicio>'.$dadosXML[$i]["ano"].'</exercicio>';
	}
	
		
	$financeiros 	.= '<financeiro>
							<idUso>'.$dadosXML[$i]["iducod"].'</idUso>
							<idoc>'.$dadosXML[$i]["idocod"].'</idoc>
							<naturezaDespesa>'.$dadosXML[$i]["ndpcod"].'</naturezaDespesa>
							<fonte>'.$dadosXML[$i]["foncod"].'</fonte>
							<valor>'.$dadosXML[$i]["valor"].'</valor>
						</financeiro>';
	
	if((integer)$dadosXML[$i]["esfcod"] == 30) {
		$receitas		.= '<receita>
								<naturezaReceita>'.$dadosXML[$i]["nrcod"].'</naturezaReceita>
								<valor>'.$dadosXML[$i]["valor_receita"].'</valor>
							</receita>';
		$cont++;
	}
	
	if( $i == ( count($dadosXML) - 1 ) ) {
		$xml .= '<financeiros>'.$financeiros.'</financeiros>';
		
		if ( $cont > 0 ) {
					 $xml .= '<receitas>'.$receitas.'</receitas>';
					 $cont = 0;
					}				 

			$xml .= '</proposta>'; 
	}
}


$xml .= '</propostas>
	</integracao>';



header("Content-Type: text/xml");
echo $xml;
die;

?>




