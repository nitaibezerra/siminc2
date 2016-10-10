<?php
ini_set("memory_limit", "1024M");

$_REQUEST['baselogin'] = "simec_espelho_producao";

include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();


$sql = "select * from 
						( 
						select  'EYX' as login, 
							MD5('COISUCA') as senha, 
							 '". CODIGO_ORGAO_SISTEMA. "' AS  orgao, 
							acao.acaid, 
							acao.esfcod, 
							acao.unicod, 
							acao.funcod, 
							acao.sfucod, 
							acao.prgcod, 
							acao.acacod, 
							acao.loccod, 
							tda.tpdid as tipodetalhamento, 
							Case when tda.tpdid = 1 then coalesce(acaqtdefisico,1) else acaqtdefisico end as quantidadefisico, 
							Case when acaqtdefinanceiro is null then coalesce ( SUM(da.dpavalor), 0 ) else acaqtdefinanceiro end as valorfisico, 
							justificativa as justificativa, 
							'2011' as ano, 
							da.iducod, 
							idoc.idocod, 
							substr(nd.ndpcod,1,6)||'00' as ndpcod, 
							da.foncod, 
							coalesce ( SUM(da.dpavalor), 0 ) as valor, 
						
							'' as nrcod, 
							0 as valor_receita 
						 from elabrev.despesaacao da 
						 	inner join elabrev.ppaacao_orcamento acao on acao.acaid = da.acaid 
							inner join naturezadespesa nd ON nd.ndpid = da.ndpid 
							inner join idoc on idoc.idoid = da.idoid 
							inner join ( select ao.acaid, min(tpdid) as tpdid from elabrev.tipodetalhamentoacao a 
									inner join elabrev.ppaacao_orcamento ao ON ao.acaid = a.acaid 
									where ao.prgano ='2010' 
									group by ao.acaid ) tda ON tda.acaid = acao.acaid
							--left join naturezareceita nr ON nr.nrcid = da.nrcid
							where 1=1  and da.ppoid = 157 and acao.acacod not in ( '2004','2010','2011','2012','20CW' )
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
							substr(nd.ndpcod,1,6)||'00', 
							da.iducod, 
							da.foncod, 
							idoc.idocod, 
							acao.acaqtdefisico, 
							acao.tdecod, 
							acao.justificativa 
							
						union all
						
						select  'EYX' as login,
							MD5('COISUCA') as senha,
							 '". CODIGO_ORGAO_SISTEMA. "' AS  orgao,
							acao.acaid,
							acao.esfcod, 
							acao.unicod, 
							acao.funcod, 
							acao.sfucod, 
							acao.prgcod, 
							acao.acacod, 
							acao.loccod, 
							tda.tpdid as tipodetalhamento, 
							Case when tda.tpdid = 1 then coalesce(acaqtdefisico,1) else acaqtdefisico end as quantidadefisico, 
							Case when acaqtdefinanceiro is null then coalesce ( SUM(da.dpavalor), 0 ) else acaqtdefinanceiro end as valorfisico, 
							justificativa as justificativa, 
							'2011' as ano, 
						
							da.iducod, 
							idoc.idocod, 
							'' as ndpcod, 
							da.foncod, 
							0 as valor, 
						
							substr(nr.nrccod,1,6)||'00' as nrccod,
							coalesce ( SUM(da.dpavalor), 0 ) as valor_receita
						 from elabrev.despesaacao da 
						 	inner join elabrev.ppaacao_orcamento acao on acao.acaid = da.acaid 
							inner join naturezareceita nr ON nr.nrcid = da.nrcid 
							inner join idoc on idoc.idoid = da.idoid 
							inner join ( select ao.acaid, min(tpdid) as tpdid from elabrev.tipodetalhamentoacao a 
									inner join elabrev.ppaacao_orcamento ao ON ao.acaid = a.acaid 
									where ao.prgano ='2010' 
									group by ao.acaid ) tda ON tda.acaid = acao.acaid 
							where 1=1  and da.ppoid = 157 and acao.acacod not in ( '2004','2010','2011','2012','20CW' )
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
							substr(nr.nrccod,1,6)||'00',
							idoc.idocod, 
							acao.acaqtdefisico, 
							acao.tdecod, 
							acao.justificativa 
						) as foo 
						
						order by  esfcod, 
							unicod, 
							funcod, 
							sfucod, 	
							prgcod, 
							acacod, 
							loccod";


$arrDados = $db->carregar($sql);

if($arrDados[0]) {
	foreach($arrDados as $d) {
		if(!$dados['informacoes'])
			$dados['informacoes'] = array('login' => $d['login'], 
										  'senha' => $d['senha'],
										  'codigoOrgao' => $d['orgao'],
										  'dataHoraRegistro' => date("Y-m-d").'T18:54:37-05:00');
		if(!$dados['propostas'][$d['acaid']])
			$dados['propostas'][$d['acaid']] = 	array('esfera'      	 => $d['esfcod'],
													  'unidade'     	 => $d['unicod'],
													  'funcao'      	 => $d['funcod'],
													  'subfuncao'   	 => $d['sfucod'],
													  'programa'    	 => $d['prgcod'],
													  'acao'        	 => $d['acacod'],
													  'localizador' 	 => $d['loccod'],
													  'tipoDetalhamento' => $d['tipodetalhamento'],
													  'quantidadeFisico' => $d['quantidadefisico'],
													  'valorFisico' 	 => $d['valorfisico'],
													  'justificativa'    => $d['justificativa'],
													  'exercicio'		 => $d['ano']);

			$dados['propostas'][$d['acaid']]['financeiros'][] = array('idUso' 			=> $d['iducod'],
				  													  'idoc'  			=> $d['idocod'],
				  													  'naturezaDespesa' => $d['ndpcod'],
				  													  'fonte'           => $d['foncod'],
				  													  'valor'           => $d['valor']);
			
		 
	}
}

if($dados) {
	$xml .= "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>";
	$xml .= "<integracao>";
	$xml .= "<informacoesRegistro dataHoraRegistro=\"".$dados['informacoes']['dataHoraRegistro']."\">";
	$xml .= "<usuario>";
	$xml .= "<login>".$dados['informacoes']['login']."</login>";
	$xml .= "<senha>".$dados['informacoes']['senha']."</senha>";
	$xml .=	"<codigoOrgao>".$dados['informacoes']['codigoOrgao']."</codigoOrgao>";
	$xml .= "</usuario>";
	$xml .= "</informacoesRegistro>";
	
	if($dados['propostas']) {
		$xml .= "<propostas>";
		foreach($dados['propostas'] as $p) {
			$xml .=	"<proposta>";
			$xml .= "<esfera>".$p['esfera']."</esfera>";
			$xml .= "<unidade>".$p['unidade']."</unidade>";
			$xml .= "<funcao>".$p['funcao']."</funcao>";
			$xml .= "<subfuncao>".$p['subfuncao']."</subfuncao>";
			$xml .= "<programa>".$p['programa']."</programa>";
			$xml .= "<acao>".$p['acao']."</acao>";
			$xml .= "<localizador>".$p['localizador']."</localizador>";
			$xml .=	"<tipoDetalhamento>".$p['tipoDetalhamento']."</tipoDetalhamento>";
			$xml .= "<quantidadeFisico>".$p['quantidadeFisico']."</quantidadeFisico>";
			$xml .= "<valorFisico>".$p['valorFisico']."</valorFisico>";
			$xml .= "<justificativa>".$p['justificativa']."</justificativa>";
			$xml .= "<exercicio>".$p['exercicio']."</exercicio>";
			if($p['financeiros']) {
				$xml .= "<financeiros>";
				foreach($p['financeiros'] as $f) {
					$xml .= "<financeiro>";
					$xml .= "<idUso>".$f['idUso']."</idUso>";
					$xml .= "<idoc>".$f['idoc']."</idoc>";
					$xml .= "<naturezaDespesa>".$f['naturezaDespesa']."</naturezaDespesa>";
					$xml .= "<fonte>".$f['fonte']."</fonte>";
					$xml .= "<valor>".$f['valor']."</valor>";
					$xml .= "</financeiro>";
				}
				$xml .= "</financeiros>";
			}
			$xml .= "</proposta>";			
		}
		$xml .= "</propostas>";
	}
	
	$xml .= "</integracao>";
}

header ("content-type: text/xml");
echo $xml;


?>