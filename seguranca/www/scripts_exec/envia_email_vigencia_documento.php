<?php

set_time_limit(0);

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

//$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento
 $_REQUEST['baselogin']  = "simec_desenvolvimento";//simec_desenvolvimento
 
// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
// require_once "../../global/config.inc";

require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";

include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/RegistroAtividade.class.inc";
include_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";


$db = new cls_banco ();

	$sqlUnidades = "select DISTINCT iu.inuid as inuid from par.documentopar dp
			inner join par.processopar prp on prp.prpid = dp.prpid
			inner join par.instrumentounidade iu on iu.inuid = prp.inuid
			WHERE
			dp.dopstatus = 'A' AND ( prp.prpfinalizado IS NULL OR prp.prpfinalizado = 'f')

	";
 
	$dadosUnidades = $db->carregar($sqlUnidades);
	$dadosUnidades = ($dadosUnidades) ? $dadosUnidades : Array();
	
	if(is_array($dadosUnidades) && (count($dadosUnidades) > 0))
	{
		//Foreach em todas as unidades
		foreach($dadosUnidades as $key => $value )
		{
			// Busca informações dos termos
			$sql = "SELECT 
						DISTINCT dopdatafimvigencia,
						dopnumerodocumento,
						dopid,
						'PAR' as tipo_doc 
					FROM par.documentopar dp
						inner join par.processopar prp on prp.prpid = dp.prpid
						inner join par.instrumentounidade iu on iu.inuid = prp.inuid
					WHERE
						dp.dopstatus = 'A' AND ( prp.prpfinalizado IS NULL OR prp.prpfinalizado = 'f') AND iu.inuid = {$value['inuid']}
			/* --termo de obras
					UNION ALL
					
					SELECT 
						DISTINCT dopdatafimvigencia,
						dopnumerodocumento,
						dopid,
						'OBRAS_PAR' as tipo_doc 
		
					from 
						par.documentopar dp
						inner join par.processoobraspar prp on prp.proid = dp.proid
					inner join par.instrumentounidade iu on iu.inuid = prp.inuid
					WHERE
						dp.dopstatus = 'A' AND ( prp.profinalizado IS NULL OR prp.profinalizado = 'f') AND iu.inuid = {$value['inuid']}
			*/
			";
			$dadosVigencia = $db->carregar($sql);
			$arrDP = array();
			
			
			require_once APPRAIZ .  'includes/classes/dateTime.inc';
			$objDataTime = new Data();
			
			// zera arrays
			$arrTermosVencendo = Array();
			$arrDoc 	 = Array();
			$arrVigencia = Array();
			//Caso tenha trago algum resultado para os termos
			if( is_array($dadosVigencia) )
			{
				
				foreach( $dadosVigencia as $dadoV )
				{
					// Trata o tipo da data do termo
					if( strlen($dadoV['dopdatafimvigencia']) == 7 )
					{ // XX/XXXX
						$mes = substr($dadoV['dopdatafimvigencia'], 0,2);
						$ano = substr($dadoV['dopdatafimvigencia'], 3,4);
						$data = $mes . "/" .  $ano;
							
							
							
					} else { // XX/XX/XXXX
						$mes = substr($dadoV['dopdatafimvigencia'], 3,2);
						$ano = substr($dadoV['dopdatafimvigencia'], 6,4);
						$data = $mes . "/" .  $ano;
			
							
					}
					// Busca o total de meses
					$qtdDiasMesVigencia = $objDataTime->getQuantidadeDiasMes($mes);
					// Valida a quantidade de dias que ainda faltam
					$diasRestantes = $objDataTime->quantidadeDeDiasEntreDuasDatas(date('d/m/Y'), $qtdDiasMesVigencia .'/'. $data ,'DD/MM/AAAA');
					
					// Verifica se existe algum vencendo daqui 15,30 ou 60 dias
					if($diasRestantes > 0 )
					{
						$numDoc = getNumDocVigencia($dadoV['dopid']);
							
						if( $diasRestantes == 15 )
						{
							
							$arrTermosVencendo[] = array(
									'numDoc' 	=> $numDoc,
									'vigencia'  => $dadoV['dopdatafimvigencia'],
									'dias'		=> 15,
									'dopid'		=> $dadoV['dopid'],
									'tipo_doc'	=> $dadoV['tipo_doc']
							);
						}
						elseif( $diasRestantes == 30 )
						{
							$arrTermosVencendo[] = array(
									'numDoc' 	=> $numDoc,
									'vigencia'  => $dadoV['dopdatafimvigencia'],
									'dias'		=> 30,
									'dopid'		=> $dadoV['dopid'],
									'tipo_doc'	=> $dadoV['tipo_doc']
							);
			
						}
						elseif( $diasRestantes == 60 )
						{
							$arrTermosVencendo[] = array(
									'numDoc' 	=> $numDoc,
									'vigencia'  => $dadoV['dopdatafimvigencia'],
									'dias'		=> 60,
									'dopid'		=> $dadoV['dopid'],
									'tipo_doc'	=> $dadoV['tipo_doc']
							);
			
						}
							
					}
			
				}
				
				if(count($arrTermosVencendo) > 0 )
				{
					
					// Verifica se existem termos vencendo
					foreach($arrTermosVencendo as $k => $v )
					{
						// Caso termo do par
						if($v['tipo_doc'] == 'PAR')
						{
							$arrDoc[] 	 = $v['numDoc'];
							$arrVigencia[] = $v['vigencia'];
						}
						else 
						{
							// Caso termo de obras (suspenso por enquanto)
							/*$arrDocObras[] 	 = $v['numDoc'];
							$arrVigenciaObras[] = $v['vigencia'];*/
						}
					
					}
					
					
					if((is_array($arrDoc)) && (is_array($arrVigencia)) )
					{
						if($v['tipo_doc'] == 'PAR')
						{
							$docs = implode(', ', $arrDoc);
							$vigencias = implode(', ', $arrVigencia);
						}
						elseif($v['tipo_doc'] == 'OBRAS_PAR')
						{
							// Caso termo de obras (suspenso por enquanto)
							/*
							$docsObras = implode(', ', $arrDocObras);
							$vigenciasObras = implode(', ', $arrVigenciaObras);*/
						}
					}
					

					$texto = "
					<pre><p style=\"text-align: justify;\">Prezados Senhores,
					Informamos que a(s) vigência(s) do (s) Termo (s) de Compromisso(s) nº (s) {$docs}
					firmado(s)  com esse Estado/Prefeitura e o Fundo Nacional de Desenvolvimento da Educação –FNDE, expirará(ão)
					em $vigencias.
					Esclarecemos que se a solicitação não for feita em tempo hábil, o(s) Termo(s) será (ão) automaticamente finalizado(s)
					e os recursos recebidos deverão ser devolvidos à conta do Tesouro Nacional. Ressaltamos, ainda, que o pedido deverá ser
					solicitado no próprio SIMEC, na aba “Execução e Acompanhamento”, e que não é possível a realização simultânea de pedidos
					de prorrogações de prazo e reprogramações de subações. Desta forma, caso o ente federativo esteja com pedido de reprogramação
					de subação em aberto, deverá efetuar o cancelamento no sistema para viabilizar o pedido de prorrogação.
					Caso não seja de interesse dessa prefeitura a prorrogação do (s) referido (s) termo (s), favor encaminhar e-mail para
					o par@fnde.gov.br com a justificativa para a não prorrogação.
					<br>
					Atenciosamente,
					Equipe do PAR
					</p></pre>
					";
					
					enviaEmailAvisoVigencia($texto, $value['inuid'], 'PAR' );
				}
			}

			$listaPac = getListaVigenciaPAC(277);
			if($listaPac)
			{
				enviaEmailAvisoVigencia($listaPac, $value['inuid'], 'OBRA_PAC' );
				
			}
			$listaObrasPar = getListaVigenciaObrasPar(277);
			if($listaObrasPar)
			{
				enviaEmailAvisoVigencia($listaObrasPar, $value['inuid'], 'OBRA_PAR' );
			}
		}	
	}
	die('chega!');
	
	
	
function enviaEmailAvisoVigencia($texto, $inuid , $tipo)
{
	 	
	if( !empty($inuid) ){
		$db = new cls_banco ();
		$sql = "SELECT iu.itrid,
					CASE WHEN iu.itrid = 2 THEN
						iu.muncod
					WHEN 
						iu.itrid = 1 THEN
					iu.estuf
				END as filtro
			FROM
				 par.instrumentounidade iu 
			WHERE
				inuid = {$inuid}
			";
		$result = $db->pegaLinha($sql);
		$itrid = $result['itrid'];
		$filtro = $result['filtro'];
		
				
		if( ($itrid == 2) && ($filtro)  )
		{
			$sqlEmail = "SELECT
			ent.entemail as email
				FROM
					par.entidade ent
				INNER JOIN par.entidade ent2 ON ent2.inuid = ent.inuid AND ent2.dutid = 6   AND ent2.entstatus = 'A'
				INNER JOIN territorios.municipio mun on mun.muncod = ent2.muncod
					WHERE
				ent.dutid =  7
					and ent.entstatus = 'A'
				AND
				mun.muncod in ( '{$filtro}' )
				";
		}
		else if( ($itrid == 1) && ($filtro))
		{
			$sqlEmail = "SELECT
						ent.entemail as email
					FROM
					par.entidade ent
						INNER JOIN par.entidade ent2 ON ent2.muncod = ent.muncod AND ent2.dutid = 9  AND ent2.entstatus = 'A'
						INNER JOIN territorios.estado est on est.estuf = ent2.estuf
						
					WHERE
					ent.entstatus='A'
					AND
					ent.dutid =  10
					AND
					ent2.estuf in ( '{$filtro}' )";
		}
		
		$resultEmail = $db->pegalinha($sqlEmail);
		
		$emailTo =  $resultEmail['email'];
		if( ! $emailTo )
		{
			return false;
		}
		
		$strMensagem = $texto;
		// . $dopTexto
		
		
		if($tipo == 'PAR')
		{
			$strAssunto = "Termo(s) de compromisso vencendo - Prorrogação";
		}
		elseif($tipo == 'OBRA_PAC')
		{
			$strAssunto = "Vencimento de vigência de Obras - PAC";
		}
		elseif($tipo == 'OBRA_PAR')
		{
			$strAssunto = "Vencimento de vigência de Obras - PAR";
		}
		
		
		$remetente = array("nome"=>SIGLA_SISTEMA, "email"=>"noreply@mec.gov.br");
		$strMensagem = html_entity_decode($strMensagem);
			
		if( $_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost" )
		{
			
			return false;
		} 
		elseif($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br")
		{
			$strEmailTo = array($_SESSION['email_sistema']);
			$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
			return $retorno;
		}
		else 
		{
			$strEmailTo = $emailTo;
			$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
			return $retorno;
		}
						
			
	} else {
			
		return false;
	}
	exit();
}
 
 
 
 function getNumDocVigencia( $dopid )
 {
 	$sql = "SELECT
 				CASE WHEN dp2.dopano::boolean THEN
 					dp.dopnumerodocumento::text || '/' || dp2.dopano::text
 			ELSE
 				dp.dopnumerodocumento::text
 			END
 				as ndocumento
 			FROM 
 				par.documentopar dp
 
 			LEFT JOIN par.documentopar dp2 ON dp2.dopid = dp.dopnumerodocumento
		 	WHERE
		 		dp.dopid = {$dopid}";
 	global $db;
 
 	$result = $db->carregar( $sql );
 	if(is_array($result) && count($result))
 	{
 
 	return $result[0]['ndocumento'];
	}
	else
 	{
 	return 'erro';
 	}
 
 
}
 
function getListaVigenciaObrasPar($inuid)
{
	global $db;
	
	
	
	
	$db = new cls_banco ();
	$sqlDadosUnidade = $db->pegaLinha("select estuf, muncod, mun_estuf from par.instrumentounidade where inuid = {$inuid}");
	if($sqlDadosUnidade)
	{
		if($sqlDadosUnidade['muncod']) 
		{
			$responsavel = "Prefeito(a)";
			$orgao	 = "Prefeitura Municipal";
			$local = "Município";
			$descLocal = $db->pegaUm("
					select mundescricao ||'-' || mun_estuf as descunidade from par.instrumentounidade  inu
					inner join territorios.municipio m ON inu.muncod = m.muncod
					where inu.inuid = {$inuid}");
		} 
		else
		{
			
			$responsavel = "Secretário(a) Estadual";
			$orgao = "Secretaria Estadual";
			$local = "Estado";
			$descLocal = $db->pegaUm("
					select estdescricao as descunidade from par.instrumentounidade  inu
					inner join territorios.estado m ON inu.estuf = m.estuf
					where inu.inuid = {$inuid}");
		}
	}
	
	$arrWhere = Array("inu.inuid = {$inuid}","pre.prestatus = 'A'","pre.preidpai IS NULL");

	// RECUPERA OBRAS DO PAR
	$sql = "
	    SELECT DISTINCT
			pre.preid as preid,
			pre.predescricao as nome_obra
		FROM
			obras.preobra pre
			LEFT  JOIN obras2.obras obr ON obr.obrid = pre.obrid AND obridpai IS NULL
			INNER JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
			INNER JOIN workflow.documento doc ON doc.docid = pre.docid
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			INNER JOIN par.subacaoobra sbo ON sbo.preid = pre.preid
	        INNER JOIN par.subacao sub on sub.sbaid = sbo.sbaid AND sub.sbastatus = 'A'
	        INNER JOIN par.acao aca ON aca.aciid = sub.aciid AND aca.acistatus = 'A'
	        INNER JOIN par.pontuacao pon ON pon.ptoid = aca.ptoid AND pon.ptostatus = 'A'
	        INNER JOIN par.instrumentounidade inu ON inu.inuid = pon.inuid AND ( pre.estuf = inu.estuf OR pre.muncod = inu.muncod )
		WHERE
			".implode(' AND ',$arrWhere)."
		ORDER BY
		    pre.predescricao
	";

	$arPreObrasSemCobertura = $db->carregar($sql);
	$arPreObrasSemCobertura = $arPreObrasSemCobertura ? $arPreObrasSemCobertura : array();


	if(is_array($arPreObrasSemCobertura)) {
		foreach($arPreObrasSemCobertura as $k => $preobra)
		{
				
			/* Trecho que preenche a coluna Fim da vigência */
			$sql = "
            SELECT
    			MIN(pag.pagdatapagamentosiafi) AS data_primeiro_pagamento,
    			MIN(pag.pagdatapagamentosiafi) + 720 as prazo
    		FROM
    			par.pagamentoobrapar po
    		INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
    		WHERE
    			po.preid = " . $preobra['preid'];

			$dataPrimeiroPagamento = $db->carregar($sql);
				
			if( $dataPrimeiroPagamento[0]['data_primeiro_pagamento'] ){
				$sql = "SELECT popdataprazoaprovado FROM obras.preobraprorrogacao WHERE popstatus = 'A' AND preid = ".$preobra['preid'];
				$prorrogado = $db->pegaUm($sql);
				if( $prorrogado )
				{
					$dataAtual = $prorrogado;
				} else {
					$dataAtual = $dataPrimeiroPagamento[0]['prazo'];
				}
					
			}
			if($dataAtual)
			{
				$mes = substr($dataAtual, 5,2);
				$ano = substr($dataAtual, 0,4);
				$dia = substr($dataAtual, 8,2);
				$data = $dia.'/'.$mes.'/'.$ano;
				$arPreObrasSemCobertura[$k]['fimvigencia'] = $data;
			}
		}
		
		$resultVigencias = verificaProximosVencimentosObrasEmail( $arPreObrasSemCobertura ) ;
		
		if($resultVigencias['resultado'])
		{
			
			$texto = "
			<p align='justify'>Prezados Senhores,<br><br>
			Informamos que consta no SIMEC para a {$orgao} de {$descLocal} a(s) obra(s), abaixo relacionada(s), que vencem nos próximos 90 dias e que ainda NÃO tiveram manifestação quanto à prorrogação da(s) mesma(s).
			
			{$resultVigencias['tabela']}
			<br> Caso o {$local} precise de mais prazo para conclusão da(s) obra(s) é necessário que o(a) {$responsavel} acesse o SIMEC com o seu CPF  e senha, clique no módulo PAR, selecione a visualização: Árvore, clique em
			\"Lista de Obras\".<br>
			Em seguida o Sistema apresentará a relação das obras com seus prazos de Fim de Vigência. O  {$responsavel} deve clicar em “Solicitar Prorrogação da Vigência”, inserir o prazo solicitado, a justificativa fundamentada para o pedido e clicar em salvar.<br><br>
			Antes de solicitar a prorrogação de vigência é imprescindível que o {$local} faça a atualização do cronograma de execução de todas as obras, bem como, os dados de execução das obras inseridos pelo(a) engenheiro(a) do município, pois utilizaremos como base para avaliação desses pedidos, e também para a liberação de recursos. <br>Portanto, para que possamos fazer uma análise coerente é indispensável que as obras estejam atualizadas no SIMEC no módulo de monitoramento de obras.
			<br>Para dúvidas, solicitamos  entrar em contato com a Equipe Técnica da COVEN - Coordenação de Convênio, por meio do E-mail: grupo.prorrogação@fnde.gov.br.
			Para os pedidos aprovados constará no SIMEC o novo prazo de fim da vigência das obras, bem como os pareceres de deferimento para visualização e impressão.
			<br>
			<span style='color:red'>Ressaltamos que não é preciso enviar Ofício ao FNDE.</span><br>
			<br>
			Atenciosamente,<br>
			Equipe do PAR</p>
				
			";
				
			return $texto;
		}
		else
		{
		return false;
		}

		$fimVigenciaObra = '';

	}
}
	
	function getListaVigenciaPAC($inuid)
	{
		$db = new cls_banco ();
		$sqlDadosUnidade = $db->pegaLinha("select estuf, muncod, mun_estuf from par.instrumentounidade where inuid = {$inuid}");
		if($sqlDadosUnidade)
		{
			if($sqlDadosUnidade['muncod']) {
				$muncodpar = " = '" . $sqlDadosUnidade['muncod'] . "'";
				$esfera = "M";
				$responsavel = "Prefeito(a)";
				$orgao	 = "Prefeitura Municipal";
				$local = "Município";
				$descLocal = $db->pegaUm("
					select mundescricao ||'-' || mun_estuf as descunidade from par.instrumentounidade  inu
					inner join territorios.municipio m ON inu.muncod = m.muncod
				 	where inu.inuid = {$inuid}");
				$ptoclassificacaoobra = array("'Q'", "'C'", "'P'");
				$filtroMuncodOREstuf = "pre.muncodpar  = '{$sqlDadosUnidade['muncod']}'";
			} else {
				$muncodpar = "IS NULL";
				$esfera = "E";
				$orgao	 = "Secretaria Estadual";
				$responsavel = "Secretário(a) Estadual";
				$local = "Estado";
				$descLocal = $db->pegaUm("
					select estdescricao as descunidade from par.instrumentounidade  inu
					inner join territorios.estado m ON inu.estuf = m.estuf
					where inu.inuid = {$inuid}");
				$ptoclassificacaoobra = array("'Q'", "'C'");
				$filtroMuncodOREstuf = "pre.estufpar = '{$sqlDadosUnidade['estuf']}'";
			}
		}	
		
		$sqlObrasPac = "
			SELECT
				pre.preid as preid,
				pre.predescricao as nome_obra,
				'PAC' as tipo_doc
			FROM
				obras.preobra pre
			LEFT  JOIN obras2.obras obr ON obr.obrid = pre.obrid AND obridpai IS NULL
			INNER JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
			INNER JOIN workflow.documento doc ON doc.docid = pre.docid
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			WHERE
				$filtroMuncodOREstuf
			AND pto.ptoesfera IN ('" . $esfera . "','T')
	        AND pre.preesfera = '" . $esfera . "'
	        AND pto.ptoclassificacaoobra IN (" . implode(',', $ptoclassificacaoobra) . ")
	        AND pre.presistema = '23'
	        AND pre.prestatus = 'A'
	        AND pre.tooid = 1
	        AND pre.preidpai IS NULL
		    ORDER BY
		        pre.preprioridade
		";
			$arPreObrasComCobertura = $db->carregar($sqlObrasPac);
			$arPreObrasComCobertura = ($arPreObrasComCobertura) ? $arPreObrasComCobertura : Array();
		
		if(count($arPreObrasComCobertura) > 0)
		{
			foreach($arPreObrasComCobertura as $k => $v)
			{
			
				$sql = "select
				MIN(pag.pagdatapagamentosiafi) as data_primeiro_pagamento,
							MIN(pag.pagdatapagamentosiafi) + 720 as prazo
						from
							par.pagamentoobra po
						inner join par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
						where
							po.preid = ".$v['preid'];
										$dataPrimeiroPagamento = $db->carregar($sql);
				
				if( $dataPrimeiroPagamento[0]['data_primeiro_pagamento'] )
				{
					$sql = "SELECT popdataprazoaprovado FROM obras.preobraprorrogacao WHERE popstatus = 'A' AND preid = ".$v['preid'];
					$prorrogado = $db->pegaUm($sql);
					if( $prorrogado )
					{
						$dataAtual = $prorrogado;
					} else {
						$dataAtual = $dataPrimeiroPagamento[0]['prazo'];
					}
				}
				if($dataAtual)
				{
					$mes = substr($dataAtual, 5,2);
					$ano = substr($dataAtual, 0,4);
					$dia = substr($dataAtual, 8,2);
					$data = $dia.'/'.$mes.'/'.$ano;
					$arPreObrasComCobertura[$k]['fimvigencia'] = $data;
				}
			
			}
				
		}
		
		$resultVigencias = verificaProximosVencimentosObrasEmail( $arPreObrasComCobertura ) ;
		
		if($resultVigencias['resultado'])
		{
			
			$texto = "
			<p align='justify'>Prezados Senhores,<br><br>
			
			Informamos que consta no SIMEC para a {$orgao} de {$descLocal} a(s) obra(s), abaixo relacionada(s), que vencem nos próximos 90 dias e que ainda NÃO tiveram manifestação quanto à prorrogação da(s) mesma(s).
				
			{$resultVigencias['tabela']}
			<br> Caso o {$local} precise de mais prazo para conclusão da(s) obra(s) é necessário que o(a) {$responsavel} acesse o SIMEC com o seu CPF  e senha, clique no módulo PAR, selecione a visualização: Árvore, clique em
			\"Lista de Obras\".<br>
			Em seguida o Sistema apresentará a relação das obras com seus prazos de Fim de Vigência. O  {$responsavel} deve clicar em “Solicitar Prorrogação da Vigência”, inserir o prazo solicitado, a justificativa fundamentada para o pedido e clicar em salvar.<br><br>
			Antes de solicitar a prorrogação de vigência é imprescindível que o {$local} faça a atualização do cronograma de execução de todas as obras, bem como, os dados de execução das obras inseridos pelo(a) engenheiro(a) do município, pois utilizaremos como base para avaliação desses pedidos, e também para a liberação de recursos. <br>Portanto, para que possamos fazer uma análise coerente é indispensável que as obras estejam atualizadas no SIMEC no módulo de monitoramento de obras.
			<br>Para dúvidas, solicitamos  entrar em contato com a Equipe Técnica da COVEN - Coordenação de Convênio, por meio do E-mail: grupo.prorrogação@fnde.gov.br.
			Para os pedidos aprovados constará no SIMEC o novo prazo de fim da vigência das obras, bem como os pareceres de deferimento para visualização e impressão.
			<br>
			<span style='color:red'>Ressaltamos que não é preciso enviar Ofício ao FNDE.</span><br>
			<br>
			Atenciosamente,<br>
			Equipe do PAR</p>
			";
				
			return $texto;
		}
		else
		{
			return false;
		}
		
	
	}	
function verificaProximosVencimentosObrasEmail( $arrayObras )
{
	$db = new cls_banco ();
	if( is_array($arrayObras) )
	{
		$objDataTime = new Data();
		foreach( $arrayObras as $dadoV )
		{
			if( strlen($dadoV['fimvigencia']) == 7 )
			{

				// XX/XXXX
				$mes = substr($dadoV['fimvigencia'], 0,2);
				$ano = substr($dadoV['fimvigencia'], 3,4);
				$data = $mes . "/" .  $ano;

				$qtdDiasMesVigencia = $objDataTime->getQuantidadeDiasMes($mes);

				$diasRestantes = $objDataTime->quantidadeDeDiasEntreDuasDatas(date('d/m/Y'), $qtdDiasMesVigencia .'/'. $data ,'DD/MM/AAAA');

			}
			else
			{
				// XX/XX/XXXX
				//$diasRestantes = $objDataTime->quantidadeDeDiasEntreDuasDatas(date('20/06/2016'), $dadoV['fimvigencia'] ,'DD/MM/AAAA');

				//
				$diasRestantes = $objDataTime->quantidadeDeDiasEntreDuasDatas(date('d/m/Y'), $dadoV['fimvigencia'] ,'DD/MM/AAAA');

			}

	
			if($diasRestantes > 0 )
			{
					
				if( $diasRestantes == 15 )
				{
					$arrObrasVencendo[] = array(
							'preid'  => $dadoV['preid'],
							'vigencia'  => $dadoV['fimvigencia'],
							'dias'		=> 15,
							'nome_obra'	=> $dadoV['nome_obra'],
							'tipo_doc'	=> $dadoV['tipo_doc']
				);
				}
				elseif( $diasRestantes == 30 ) 
				{
					$arrObrasVencendo[] = array(
							'preid'  => $dadoV['preid'],
							'vigencia'  => $dadoV['fimvigencia'],
							'dias'		=> 30,
							'nome_obra'	=> $dadoV['nome_obra'],
							'tipo_doc'	=> $dadoV['tipo_doc']
					);

				}
				elseif( $diasRestantes == 60 )
				{
					$arrObrasVencendo[] = array(
							'preid'  => $dadoV['preid'],
							'vigencia'  => $dadoV['fimvigencia'],
							'dias'		=> 60,
							'nome_obra'	=> $dadoV['nome_obra'],
							'tipo_doc'	=> $dadoV['tipo_doc']
					);
				}
				elseif( $diasRestantes == 90 )
				{
					$arrObrasVencendo[] = array(
							'preid'  => $dadoV['preid'],
							'vigencia'  => $dadoV['fimvigencia'],
							'dias'		=> 60,
							'nome_obra'	=> $dadoV['nome_obra'],
							'tipo_doc'	=> $dadoV['tipo_doc']
					);
				}
			}
		}
			
	}

	$arrObrasVencendo = (is_array($arrObrasVencendo)) ? $arrObrasVencendo : Array();
	
	$tabela = false;

	if(count($arrObrasVencendo) > 0)
	{

		$tabela = '
		<table>
			<thead>
					<tr bgcolor="#F2DCDB">
						<td>Nº Obra</td>
						<td>Nome da obra</td>
						<td>Vencimento</td>
					</tr>
			</thead>

			</tbody>

		';
		foreach($arrObrasVencendo as $k => $v )
		{
			$tabela .= "<tr>
							<td>
								{$v['preid']}
							</td>
							<td>
								{$v['nome_obra']}
							</td>
							<td>
								{$v['vigencia']}
							</td>
						</tr>";

			$arrObras[] 	= $v['nome_obra'];
			$arrVigencia[] 	= $v['vigencia'];

		}


		$tabela .= "
			</tbody>
		</table>";

		return array(
				'resultado' => true,
				'tabela' 	=> $tabela
			);
	}
	else
	{
		return array(
		'resultado' => false,
		'tabela' 	=> $tabela
		);
	}

}	
	

			?>