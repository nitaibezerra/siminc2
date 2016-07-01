<?php

/*
 * FUNÇÕES
 * 
 * 
 * */


function totalTermosSemcomposicao()
{
	global $db;
	$sql = "
		SELECT  count( distinct dp.dopid ) as total
		from 
			par.vm_documentopar_ativos dp 
		inner join par.processopar pp on pp.prpid = dp.prpid
		inner join par.processoparcomposicao pc on pc.prpid = pp.prpid and pc.ppcstatus = 'A'
		inner join par.subacaodetalhe sd on sd.sbdid = pc.sbdid
		inner join par.empenhosubacao es on es.sbaid = sd.sbaid and eobstatus = 'A'
		inner join par.empenho e on e.empsituacao = '2 - EFETIVADO' and e.empid = es.empid and empstatus = 'A'
		where pp.prpid in (select prpid from par.vm_documentopar_ativos where dopid not in (select dopid from par.termocomposicao   ) )  
		and 
		(select count(pcc.prpid)  from par.processoparcomposicao pcc where pcc.ppcstatus = 'A' and pcc.prpid = pc.prpid ) > 1
	";
	
	$result = $db->pegaLinha($sql);
	if(boolArrayAndCount($result))
	{
		return "<center>Total de Termos com mais de uma subação e sem composição: <span style=\"color:red\">{$result['total']}</span><center><br><br>";	
	}
	else
	{
		return "<center><span style=\"color:red\">Não foram encontrados registros de termos sem composição</span><center>";
	}
	
	
}

function buscaRegistrosSemSubacao(){
	
	global $db;
	$sql = "
		SELECT DISTINCT dp.dopid, sd.sbdid,  par.retornacodigosubacao(sd.sbaid) as localizacao
		FROM
			par.vm_documentopar_ativos dp 
		INNER JOIN par.processopar pp on pp.prpid = dp.prpid
		INNER JOIN par.processoparcomposicao pc on pc.prpid = pp.prpid and pc.ppcstatus = 'A'
		INNER JOIN par.subacaodetalhe sd on sd.sbdid = pc.sbdid
		INNER JOIN par.empenhosubacao es on es.sbaid = sd.sbaid and eobstatus = 'A'
		INNER JOIN par.empenho e on e.empsituacao = '2 - EFETIVADO' and e.empid = es.empid and empstatus = 'A'
		WHERE 
			pp.prpid in (select prpid from par.vm_documentopar_ativos where dopid not in (select dopid from par.termocomposicao   ) )  
		AND 
		(select count(pcc.prpid)  from par.processoparcomposicao pcc where pcc.ppcstatus = 'A' and pcc.prpid = pc.prpid ) > 1
		
		ORDER BY dp.dopid asc --desc -- asc primeiro so itens desc primeiro os que tem subacao
		
		LIMIT 300
		--OFFSET 15000
	
	";
	
	$result = $db->carregar($sql);
	
	return $result;
	
} 

function montaEstruturaParaBusca( $arrResultSemSubacao )
{
	
	foreach($arrResultSemSubacao as $k => $v)
	{
		$arrEstrutura[$v['dopid']][] = array
		(
			'localizacao'	=> $v['localizacao'],
			'dopid'			=> $v['dopid'],
			'sbdid'			=> $v['sbdid']
		);	
	}
	
	return $arrEstrutura;
}

function boolArrayAndCount( $arr )
{
	if( is_array($arr) && count(is_array($arr)) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

function getTemSubacao($doptexto)
{
	
}

function getItensSubacaoDetalhe($sbdid)
{
	global $db;
	
	$sql = "
		SELECT 
			picdescricao
		FROM 
			par.propostaitemcomposicao pi
		INNER JOIN par.propostasubacaoitem psi on psi.picid = pi.picid
		INNER JOIN par.subacao s on s.ppsid = psi.ppsid
		INNER JOIN par.subacaodetalhe sd on sd.sbaid = s.sbaid 
		INNER JOIN par.empenhosubacao es on es.sbaid = s.sbaid and sd.sbdano = es.eobano and eobstatus = 'A'
		INNER JOIN par.empenho e on e.empid = es.empid and e.empsituacao = '2 - EFETIVADO' and empstatus = 'A'
		WHERE
			sd.sbdid = {$sbdid}
	";
	
	$result = $db->carregar($sql);
	
	return $result;
}

function getDopTexto($dopid)
{
	global $db;
	
	if( $dopid ){
		
		$doptexto = pegaTermoCompromissoArquivo( $dopid, '' );
		
		return htmlspecialchars_decode($doptexto);	
	}
	else
	{
		return false;
	}
}

function changeHtmlEspecialParaTexto( $texto )
{
	$arrText = array(
		'á',
		'à',
		'í',
		'ô',
		'Á',
		'À',
		'Í',
		'Ô',
		'ã',
		'é',
		'ó',
		'ú',
		'Ã',
		'É',
		'Ó',
		'Ú',
		'â',
		'ê',
		'õ',
		'ç',
		'Â',
		'Ê',
		'Õ',
		'Ç',
		'–',
		'º',
		'ª',
		'“',
		'”',
		' ',
		'§',
		'°',
		'š',
		'‰',
		"'"
	
	
	);
	$arrCod = array(
		'&aacute;',
		'&agrave;',
		'&iacute;',
		'&ocirc;',
		'&Aacute;',
		'&Agrave;',
		'&Iacute;',
		'&Ocirc;',
		'&atilde;',
		'&eacute;',
		'&oacute;',
		'&uacute;',
		'&Atilde;',
		'&Eacute;',
		'&Oacute;',
		'&Uacute;',
		'&acirc;',
		'&ecirc;',
		'&otilde;',
		'&ccedil;',
		'&Acirc;',
		'&Ecirc;',
		'&Otilde;',
		'&Ccedil;',
		'&ndash',
		'&ordm;',
		'&ordf;',
		'&ldquo;',
		'&rdquo;',
		'&nbsp;',
		'&sect;',
		'&deg;',
		'&scaron;',
		'&permil;',
		'&#039;'
	
	);
	
	return str_replace($arrCod, $arrText, $texto);
	
}


function validaSubacoesDosTermos($arrTermoSubacao)
{
	// Contadores
	// Total
	$tot = 0;
	// por subacao
	$s = 0;
	// Por item
	$i = 0;
	// Não encontrado
	$null = 0;
	
	//Guarda os dopids que foram encontrados ou a localização ou o item
	$arrDopInclusos = array();
	// Guarda os dopids que não foram possível encontrar nada dentro do documento
	$arrDopNaoInlusos = array();
	// Guarda todos os dopids que entraram no loop
	$arrDopGeral = array();
	
	foreach($arrTermoSubacao as $k => $v )
	{
		define("LATIN1_UC_CHARS", "ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝ");
		define("LATIN1_LC_CHARS", "àáâãäåæçèéêëìíîïðñòóôõöøùúûüý");
		
		// Carrega array das subacoes que já foram incluídas nesta rodada
		$arrSbdInclusas = array();
		// Incrementa array
		$arrDopGeral[] = $k;
		// Conta como mais um
		$tot++;
		// Pega o texto do documento
		$textoDocumento = getDopTexto($k);
		// Transforma o texto retirando os códigos html e colocando tudo para caixa alta
//		$textoDocumento = strtoupper(changeHtmlEspecialParaTexto($textoDocumento));
		$textoDocumento = strtoupper(strtr(changeHtmlEspecialParaTexto($textoDocumento), LATIN1_LC_CHARS, LATIN1_UC_CHARS));
		
		// Trata os casos que tenham a subação e a localização no corpo do documento
		if (strpos($textoDocumento,'SUBAÇÃO') !== false)  
		{
		  // incrementa o número de documentos com subação
		  $s++;
		  	// Cicla as subações que o documento possui originalmente
			foreach ( $v as $ksbc => $subacao )
			{	
				// procura pela localização dentro do documento
				if ( strpos($textoDocumento,$subacao['localizacao']) !== false) 
				{
					// Incrementa os documentos que foram adicionados, caso ainda não tenha sido
					if( ! in_array($k, $arrDopInclusos ) )
					{
						$arrDopInclusos[] = $k;
					}
					// PEga o código da subação do ano
					$sbdidR = $subacao['sbdid'];
					
					// Incrementa o array do insert, caso ainda não tenha sido adicionado para esta subaçao
					if( ! in_array($sbdidR, $arrSbdInclusas ) )
					{
						// Adiciona ao array para ser inserido no banco de dados
						$arrSubacoesRecompor[$k][] = $subacao['sbdid'];
						// Incrementa as subações que foram adicionadas
						$arrSbdInclusas[] = $sbdidR;
					}
					
				}
			}
			
		}
		// Trata os casos que não possuem subação (localização) mais possuem o item que será utilizado pra achar o sbdid
		elseif (strpos($textoDocumento,'TIPO') !== false) 
		{
			print_r((string)$k . '  ' );
			// Incrementa o tipo de documentos com item
			$i++;
			
			// Cicla as subações que o documento possui originalmente
			foreach ( $v as $ksbc => $subacao )
			{
				// Carrega o valor do id da subação por ano
				$sbdid = $subacao['sbdid'];
				// Busca os items que pertencem a esta subação
				$arrItens = getItensSubacaoDetalhe($sbdid);
				// Caso exista
				if( boolArrayAndCount($arrItens)) 
				{
					// Cicla os items da subação para montar um array a ser comparado mais abaixo
					foreach ($arrItens as $kItem => $vItem)
					{
//						$subacao['itens'][]	=  strtoupper(ltrim(trim($vItem['picdescricao'])));
						$subacao['itens'][]	=  strtoupper(strtr(ltrim(trim($vItem['picdescricao'])), LATIN1_LC_CHARS, LATIN1_UC_CHARS));
					}
				}
				
				// Caso exista
				if( boolArrayAndCount($subacao['itens'] )) 
				{
					// Cicla os item afim de veriificar se as subações pertencentes aos mesmos exite ou não no documento
					foreach($subacao['itens'] as $ki => $descItem)
					{
						// Caso exista o item atual no documento:
						if (strpos($textoDocumento,$descItem) !== false) 
						{
							// Caso o dopid não tenha sido adicionado ao array, o adiciona
							if( ! in_array($k, $arrDopInclusos ) )
							{
								$arrDopInclusos[] = $k;
							}
							// Dop id
							$dopidR = $k;
							// Sbdid
							$sbdidR = $subacao['sbdid'];
							// Caso não tenha sido adicionada para o insert adiciona a subação do ano
							if( ! in_array($sbdidR, $arrSbdInclusas ) )
							{
								// Adiciona para o insert
								$arrSubacoesRecompor[$dopidR][] = $sbdidR;
								// Adiciona ao log de subações adicionadas
								$arrSbdInclusas[] = $sbdidR;
							}
							
						}
					}
				}
				// Por precaução caso não exista o icone mais exista a localização ele também adiciona				
				if ( strpos($textoDocumento,$subacao['localizacao']) !== false) 
				{
					// 	Dop id
					$dopidR = $k;
					// Sbdid
					$sbdidR = $subacao['sbdid'];
					if( ! in_array($sbdidR, $arrSbdInclusas ) )
					{
						// Adiciona para o insert
						$arrSubacoesRecompor[$dopidR][] = $sbdidR;
						// Adiciona ao log de subações adicionadas
						$arrSbdInclusas[] = $sbdidR;
					}
				}
				
			}
			
		}
		// Caso nenhum dos dois identificar individualmente
		else 
		{
			// Incrementa contador dizendo que está vazio
			$null++;
			//  Incrementa log dizendo que está vazio, caso o dop id ainda não tenha sido adicionado
			if( ! in_array($k, $arrDopNaoInlusos ) )
			{
				$arrDopNaoInlusos[] = $k;
			}
		}
	}
	
	
	// @todo Identifica os que tem localização ou tipo e não foram inseridos
	//print_r(array_diff($arrDopGeral, $arrDopInclusos));die('<< Os que existiam "TIPO" ou "SUBAÇÃO" ou não mais não foram adicionados');
	
	// Retorna dados dos contadores
	$numeros =  
  		"
  		<table>
	  		<tr>
				<td> Total: </td>
				<td> {$tot} </td>
			</tr>
			<tr>
				<td>Com Subacao: </td> 
				<td>{$s} </td>
			</tr>
			<tr>
				<td>Com Tipo:</td>
				<td>{$i}</td>
			</tr>
			<tr>
				<td>Sem nada</td>	 
				<td>{$null}</td>
			</tr>
		</table> 	  		
  		"
	;
	// Array de retorno
	$arrResult = array
	(
		'subacoesRecompor' 	=> $arrSubacoesRecompor,
		'numeros'			=> $numeros,
		'documentos_ok'		=> $arrDopInclusos,
		'documentos_fail'	=> $arrDopNaoInlusos,
		'dop_problemas'		=> array_diff($arrDopGeral, $arrDopInclusos)
	);
	
	return $arrResult;
		
}

function realizaCargaDocumentosSemSubacao( $arrRecompor )
{
	global $db;
	
    // ESTRUTURA DO $arrRecompor: 
    /*
		'subacoesRecompor' 	=> $arrSubacoesRecompor,
		'numeros'			=> $numeros,
		'documentos_ok'		=> $arrDopInclusos,
		'documentos_fail'	=> $arrDopNaoInlusos,
		'dop_problemas'		=> array_diff($arrDopGeral, $arrDopInclusos)
	*/
	
	
	// Carrega informações sobre a transação
	$arrSubacoesRecompor 	= $arrRecompor['subacoesRecompor'];
	$numerosDaOperacao		= $arrRecompor['numeros'];
	$comProblema		 	= (is_array($arrRecompor['dop_problemas'])) 	?  implode("<br>",$arrRecompor['dop_problemas'] ) 	: '<span style="color:red"> Sem registro </span>';
	$documentosOk	 	 	= (is_array($arrRecompor['documentos_ok'])) 	?  implode("<br>",$arrRecompor['documentos_ok'] )	: '<span style="color:red"> Sem registro </span>';
	$documentosFail			= (is_array($arrRecompor['documentos_fail']))	?  implode("<br>",$arrRecompor['documentos_fail']) 	: '<span style="color:red"> Sem registro </span>';
	
	
	// Contadores para montar a query
	$volta = 0;
	$voltaFinal = 0;
	
	// Caso exista subações para recompor
	if( boolArrayAndCount($arrSubacoesRecompor))
	{
		// Monta o insert
		$sqlInsert ="
			INSERT INTO par.termocomposicao
				(dopid, sbdid)
			VALUES
			
		";
		// Cicla os dopids e seus respectivos sbdids
		foreach ($arrSubacoesRecompor as $kDopid => $arrSbdids)
		{
			// Incrementa volta
			$volta++;
			
			// Caso não seja a ultima volta monta insert com vírgula
			if($volta < count($arrSubacoesRecompor))
			{
				// Monta parte da query
				foreach($arrSbdids as $Ksbdid => $vSbdid)
				{
					$sqlInsert .=
					"({$kDopid},{$vSbdid}),
				"; 	
				}
				
			}
			else
			{
				// Caso a ultima volta do loop principal 
				// Monta parte da query
				foreach($arrSbdids as $Ksbdid => $vSbdid)
				{
					// Incrementa a volta final
					$voltaFinal++;
					// Caso não seja o ultimo loop incrementa a query e coloca a virgula
					if($voltaFinal < count($arrSbdids))
					{
						$sqlInsert .=
						"({$kDopid},{$vSbdid}),
						"; 	
					}
					else
					{
						// Caso seja a ultima volta incrementa a query e não coloca a vírgula
						$sqlInsert .=
						"({$kDopid},{$vSbdid})
						";
					}
				
				}
			}
		}
	}
	else
	{
		// Se cair aqui significa que não foram retornados registros para serem corrigidos
		echo '<span style="color:red"> Nenhum documento sem subacao </span>';
	}
	//die($sqlInsert);
	
	$db->executar($sqlInsert);
	$db->commit();
	// @todo monta tabela com as informações sobre a operação:
	echo "
		<table class=\"tabela\" align=\"center\" border=\"1px;\" cellspacing=\"1\" cellpadding=\"3\">
			
			<tr>
				<td>Números da operação:</td>
				<td>{$numerosDaOperacao}</td>
			<tr>
			<tr>
				<td>Documentos(dopid) que tiveram problemas para serem inseridos:</td>
				<td>$comProblema</td>
			<tr>
			<tr>
				<td>Documentos(dopid) que forma modificados com sucesso :</td>
				<td>$documentosOk </td>
			<tr>
			<tr>
				<td>Documentos(dopid) que não foram modificados :</td>
				<td>$documentosFail </td>
			<tr>
		</table>
	";
	
}

/*
 * =========================================================================================================================
 * =========================================================================================================================
 * =========================================================================================================================
 * ========================================================================================================================= 
 * =====================================  * EXECUTA AS OPERAÇÕES * =========================================================
 * */

echo totalTermosSemcomposicao();

$arrResultSemSubacao = buscaRegistrosSemSubacao();

if( boolArrayAndCount( $arrResultSemSubacao ) )
{
	$arrTermosSemSubacao = montaEstruturaParaBusca($arrResultSemSubacao);
}
else 
{
	echo '<span style="color:red"> Nenhum registro </span>';
}

if( boolArrayAndCount( $arrTermosSemSubacao ) )
{
	$resultSubacoes = validaSubacoesDosTermos( $arrTermosSemSubacao ); 
}

if(boolArrayAndCount($resultSubacoes))
{
	realizaCargaDocumentosSemSubacao($resultSubacoes);
}

die();