<?php

$downloadfile = "sidor.txt";

header("Content-Type: plain/text");
header("Content-disposition: attachment; filename=$downloadfile");
header("Content-Transfer-Encoding: binary");
header("Pragma: no-cache");
header("Expires: 0");

//include APPRAIZ."includes/cabecalho.inc";

//echo "<br>";
//$titulo_modulo='Créditos adicionais do SIDOR';
//monta_titulo($titulo_modulo,'');

//$arquivo = fopen(APPRAIZ . "elabrev/modulos/sistema/comunica/arquivo_sidor.txt", "w");

$sql = pg_query("SELECT

unidade, tipocredito, programa , 

acao , localizador , fonte , 

idoc , iduso , 
Case 
	when acao in ('0005', '0022', '002F', '00H2', '0C04', '20TP', '0110', '0181', '0486', '0515', '0625', '0716', '0920', '0969', '0E36', '2004', '2010', '2011', '2012',  '20CE', '20CW', '8744', '8790' ) THEN '1'
	when acao in ('2992', '4001', '4009', '4086', '6318', '6321') then
		Case substr(natureza, 2, 1) 
			when '1' THEN '1' else '2' end 
	when acao in ('12KU', '12KV' ) THEN '3'
	when acao in ('00G5', '00GP', '0283', '0284', '09HB', '00H7' ) THEN '0'
	ELSE '2'
end as irpcod,
sum( suplementacao ) as suplementacao, sum( cancelamento ) as cancelamento, 

natureza , esfera , funcao , subfuncao 

 

from

(

SELECT

a.unicod as unidade, tc.tcrcod as tipocredito, a.prgcod as programa, 

a.acacod as acao, a.loccod as localizador, d.foncod as fonte, 

i.idocod as idoc, d.iducod as iduso, a.irpcod as irpcod, d.dpavalor as suplementacao, 0 as cancelamento,

CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as natureza,

d.dpaid as sequencial, a.esfcod as esfera, a.funcod as funcao, a.sfucod as subfuncao

 

FROM elabrev.despesaacao d

INNER JOIN monitora.acao a on a.acaid = d.acaidloa                 

                                   and a.unicod like '26%'
                                   --and a.unicod in ( '26270' , '26247' , '26234' ) 

                                   and a.prgano = '2012'

                                   and a.acasnrap = 'f'

									and a.acacod not in ( '2004', '2010', '2011', '2012', '20CW', '20CE', '09HB', '0181' )

INNER JOIN elabrev.tipocredito tc on tc.tcrid = d.tcrid

                                               --and tc.tcrcod in ('100','102','103','107','110','111','112','120','150','151','152','200','500','600','800','910')
                                              	--and tc.tcrcod in ('100', '101', '102', '103', '107', '109', '110', '111', '112', '120', '150', '151', '152', '200', '500', '600', '800', '910')
                                              	--and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '151', '152', '200', '500', '600', '800', '910')
                                              	--and tc.tcrcod in ('102','152')
                                              	and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920')
                                              	--and tc.tcrcod in ('100', '110', '600', '103', '102', '152', '120', '156')

INNER JOIN public.idoc i on i.idoid = d.idoid

LEFT JOIN public.naturezadespesa n on n.ndpid = d.ndpid

LEFT JOIN public.naturezareceita nr on nr.nrcid = d.nrcid

WHERE 

d.dpavalor > 0 and d.mcrid = 28
--d.dpavalor > 0 and d.mcrid = 16
--and d.dpaid not in ( 328935, 328933, 328912, 328910, 376279, 376275, 376277, 376273, 398858, 398856, 398855, 398857 )
--and ( a.unicod <> '26214' or a.unicod = '26337' ) and
--( tc.tcrcod = '102' or tc.tcrcod = '152' ) 
--and d.dpaid in ( 478691 )
and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )


union all



SELECT

a.unicod as unidade, tc.tcrcod as tipocredito, a.prgcod as programa, 

a.acacod as acao, a.loccod as localizador, d.foncod as fonte, 

i.idocod as idoc, d.iducod as iduso, a.irpcod as irpcod, 0 as suplementacao, d.dpavalor as cancelamento,

CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as natureza,

d.dpaid as sequencial, a.esfcod as esfera, a.funcod as funcao, a.sfucod as subfuncao

 

FROM elabrev.despesaacao d

 

INNER JOIN monitora.acao a on a.acaid = d.acaidloa                 

                                   and a.unicod like '26%'
                                   --and a.unicod in ( '26270' , '26247' , '26234' ) 


                                   and a.prgano = '2012'

                                   and a.acasnrap = 'f'

				and a.acacod not in ( '2004', '2010', '2011', '2012', '20CW', '20CE', '09HB', '0181' )


INNER JOIN elabrev.tipocredito tc on tc.tcrid = d.tcrid                                        

                                              -- and tc.tcrcod in ('100','102','103','107','110','111','112','120','150','151','152','200','500','600','800','910')
                                              	--and tc.tcrcod in ('100', '101', '102', '103', '107', '109', '110', '111', '112', '120', '150', '151', '152', '200', '500', '600', '800', '910')
                                              	--and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '151', '152', '200', '500', '600', '800', '910')
                                              	--and tc.tcrcod in ('102','152')
                                              	and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920')
                                              	--and tc.tcrcod in ('100', '110', '600', '103', '102', '152', '120', '156')

INNER JOIN public.idoc i on i.idoid = d.idoid



LEFT JOIN public.naturezadespesa n on n.ndpid = d.ndpid

 

LEFT JOIN public.naturezareceita nr on nr.nrcid = d.nrcid


WHERE 

d.dpavalor < 0 and d.mcrid = 28
--d.dpavalor < 0 and d.mcrid = 16
--and d.dpaid not in ( 328935, 328933, 328912, 328910, 376279, 376275, 376277, 376273, 398858, 398856, 398855, 398857 )
--and d.dpaid in ( 478691 )
--and ( a.unicod <> '26214' or a.unicod = '26337' ) and
--( tc.tcrcod = '102' or tc.tcrcod = '152' ) 
and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
) as foo

group by  unidade, tipocredito, programa , 

acao , localizador , fonte , 

idoc , iduso , irpcod,

natureza , esfera , funcao , subfuncao

order by unidade, tipocredito 
");

//
$cont = 0;
$unidadeOrcamentaria = "";
$unidadeOrcamentariaFixa = "";
$tipoPedidoCredito = "";
$tipoPedidoCreditoFixo = "";

while(($dados = pg_fetch_array($sql)) != false) {
	//
	$cont++;
	$unidadeOrcamentaria = $dados['unidade'];
	$tipoPedidoCredito = $dados['tipocredito'];
	$dpaid = $dados['dpaid'];	
	
	if(($unidadeOrcamentaria == $unidadeOrcamentariaFixa) && ($cont > 1)) {
		if($tipoPedidoCredito != $tipoPedidoCreditoFixo) {
			// Recupera o nº do pedido de crédito
			$sql_pedido = "SELECT coalesce(max(pcrnumeropedidocredito),'0000') as pcrnumeropedidocredito
						   FROM elabrev.pedidocredito where pcrano = '". $_SESSION['exercicio'] ."' ";	
			$numPedidoCredito = $db->pegaUm($sql_pedido);
			$numPedidoCredito = sprintf("%04s", ++$numPedidoCredito);
				
			// Insere na tabela elabrev.pedidocredito
			$sql_inclusao = "INSERT INTO elabrev.pedidocredito(pcrnumeropedidocredito,usucpf,pcrano) VALUES('".$numPedidoCredito."','".$_SESSION['usucpf']."','".$_SESSION['exercicio']."')";
			$db->executar($sql_inclusao);
			$db->commit();
			
			// Recupera o 'pcrid' da tabela elabrev.pedidocredito
			$sql_recupera = "SELECT pcrid FROM elabrev.pedidocredito WHERE pcrnumeropedidocredito = '".$numPedidoCredito."' and pcrano = '".$_SESSION['exercicio']."' ";	
			$pcrid = $db->pegaUm($sql_recupera);
						
			$tipoPedidoCreditoFixo = $tipoPedidoCredito;
			$linha = "0001";
		}
	}
	else {
		// Recupera o nº do pedido de crédito
		$sql_pedido = "SELECT coalesce(max(pcrnumeropedidocredito),'0000') as pcrnumeropedidocredito
					   FROM elabrev.pedidocredito where pcrano = '". $_SESSION['exercicio'] ."' ";	
		$numPedidoCredito = $db->pegaUm($sql_pedido);
		$numPedidoCredito = sprintf("%04s", ++$numPedidoCredito);
			
		// Insere na tabela elabrev.pedidocredito
		$sql_inclusao = "INSERT INTO elabrev.pedidocredito(pcrnumeropedidocredito,usucpf,pcrano) VALUES('".$numPedidoCredito."','".$_SESSION['usucpf']."','".$_SESSION['exercicio']."')";
		$db->executar($sql_inclusao);
		$db->commit();
		
		// Recupera o 'pcrid' da tabela elabrev.pedidocredito
		$sql_recupera = "SELECT pcrid FROM elabrev.pedidocredito WHERE pcrnumeropedidocredito = '".$numPedidoCredito."' and pcrano = '".$_SESSION['exercicio']."' ";	
		$pcrid = $db->pegaUm($sql_recupera);
		
		$unidadeOrcamentariaFixa = $unidadeOrcamentaria;
		$tipoPedidoCreditoFixo = $tipoPedidoCredito;
		$linha = "0001";
	}
	
	$strLinha = "";
	if($cont > 1)
		$strLinha = "\r\n".$strLinha;

	/*** NUM PEDIDO DE CRÉDITO ***/
	$strLinha .= $numPedidoCredito;
	
	/*** NUM LINHA DO PEDIDO ***/
	$numLinhaPedido = $linha;
	$strLinha .= $numLinhaPedido;
	
	
	/*** USUÁRIO ***/
	$usuario = "NAJ";
	$strLinha .= $usuario;
		
	/*** ANO DO EXERCÍCIO ***/
	if($_SESSION["exercicio"] != "") {
		$anoExercicio = $_SESSION["exercicio"];		
	}
	else {
		$anoExercicio = "    ";		 
	}	
	$strLinha .= $anoExercicio;	
		
	/*** MOMENTO ***/	
	$momento = "20";
	$strLinha .= $momento;
		
	/*** TIPO DE CRÉDITO ***/
	if(($dados['tipocredito'] != "") && ($dados['tipocredito'] != null))
	{
		
		
		if( $dados['tipocredito'] == "102" || $dados['tipocredito'] == "103" || $dados['tipocredito'] == "160" || $dados['tipocredito'] == "600" )
		{
			$tipoCredito = (string) $dados['tipocredito'] . "a ";
		}
		else
		{
			$tipoCredito = (string) $dados['tipocredito'] . "  ";
		}
				
	} else {
		$tipoCredito = "     ";
	}
	$strLinha .= $tipoCredito;
	
	/*** ESFERA ***/
	if(($dados['esfera'] != "") && ($dados['esfera'] != null)) {
		$esfera = $dados['esfera'];		
	} else {
		$esfera = "  ";
	}
	$strLinha .= $esfera;
	
	/*** UNIDADE e ÓRGÃO ***/
	if(($dados['unidade'] != "") && ($dados['unidade'] != null)) {
		$orgao = substr($dados['unidade'], 0, 2);
		$unidade = substr($dados['unidade'], 2);
	}
	else {
		$orgao = "  ";
		$unidade = "   ";
	}
	$strLinha .= $orgao.$unidade;
	
	/*** FUNÇÃO ***/
	if(($dados['funcao'] != "") && ($dados['funcao'] != null)) {
		$funcao = $dados['funcao'];
	} else {
		$funcao = "  ";
	}
	$strLinha .= $funcao; 
	
	/*** SUBFUNÇÃO ***/
	if(($dados['subfuncao'] != "") && ($dados['subfuncao'] != null)) {
		$subfuncao = $dados['subfuncao'];
	} else {
		$subfuncao = "   ";
	}
	$strLinha .= $subfuncao; 
	
	/*** PROGRAMA ***/
	if(($dados['programa'] != "") && ($dados['programa'] != null)) {
		$programa = $dados['programa'];
	} else {
		$programa = "    ";
	}
	$strLinha .= $programa;
	
	/*** AÇÃO ***/
	if(($dados['acao'] != "") && ($dados['acao'] != null)) {
		$acao = $dados['acao'];
	} else {
		$acao = "    ";
	}
	$strLinha .= $acao;

	/*** TIPO DE INCLUSÃO DA AÇÃO
		1 - PLOA
		2 - Emenda
		4 - PPA
		5 - Crédito Especial
		6 - Crédito Extraordinário	***/
	$tipoinclusaoacao = "1";
	$strLinha .= $tipoinclusaoacao;

		
	/*** LOCALIZADOR ***/
	if(($dados['localizador'] != "") && ($dados['localizador'] != null)) {
		$localizador = $dados['localizador'];		
	} else {
		$localizador = "    ";
	}
	$strLinha .= $localizador;

	/*** TIPO DE INCLUSÃO DO LOCALIZADOR ***/
	$tipoinclusaoloc = "1";
	$strLinha .= $tipoinclusaoloc;

	/*** CÓDIGO BENS/SERVIÇOS ***/
	$codigoBensServicos = "    ";
	$strLinha .= $codigoBensServicos;
		
	/*** QTD. ACRÉSCIMO ***/
	$quantidadeAcrescimo = "         ";
	$strLinha .= $quantidadeAcrescimo;
	
	/*** QTD. REDUÇÃO ***/
	$quantidadeReducao = "         ";
	$strLinha .= $quantidadeReducao;
	
	/*** NATUREZA ***/
	if(($dados['natureza'] != "") && ($dados['natureza'] != null)) {
		$natureza = $dados['natureza'];		
	} else {
		$natureza = "        ";
	}
	$strLinha .= $natureza;
	
	/*** CÓD. RESULTADO PRIMÁRIO ***/
	if(($dados['irpcod'] != "") && ($dados['irpcod'] != null)) {
		$codResultadoPrimario = $dados['irpcod'];
	} else {
		$codResultadoPrimario = " ";
	}
	$strLinha .= $codResultadoPrimario;
	
	/*** IDENTIFICADOR DE USO ***/
	if(($dados['iduso'] != "") && ($dados['iduso'] != null)) {
		$iduso = $dados['iduso'];
	} else {
		$iduso = " ";
	}
	$strLinha .= $iduso;
		
	/*** GRUPO DA FONTE e FONTE ***/
	if(($dados['fonte'] != "") && ($dados['fonte'] != null)) {
		$grupoFonte = substr($dados['fonte'], 0, 1);
		$fonte = substr($dados['fonte'], 1);
	}
	else {
		$grupoFonte = " ";
		$fonte = "  ";
	}
	$strLinha .= $grupoFonte.$fonte;
	
	/*** IDENT. OPER. DE CRÉDITO ***/
	if(($dados['idoc'] != "") && ($dados['idoc'] != null)) {
		$idoc = $dados['idoc'];
	} else {
		$idoc = "    ";
	}
	$strLinha .= $idoc;
		
	/*** VALOR DE SUPLEMENTAÇÃO ***/
	if(($dados['suplementacao'] != "") && ($dados['suplementacao'] != null)) {
		$suplementacao = $dados['suplementacao'];
	} else {
		$suplementacao = "            ";
	}
	$strLinha .= sprintf("%012s", $suplementacao);
	
	/*** VALOR DE CANCELAMENTO ***/
	if(($dados['cancelamento'] != "") && ($dados['cancelamento'] != null)) {
		$cancelamento = abs($dados['cancelamento']);		
	} else {
		$cancelamento = "            ";
	}	
	$strLinha .= sprintf("%012s", $cancelamento);

	// Insere na tabela elabrev.itempedidocredito
	$sql_inclusao = "INSERT INTO elabrev.itempedidocredito (pcrid,ipcnumerolinhapedido,ipcusuario,
							ipcanoexercicio,ipcmomento,ipctipocredito,ipcesfera,ipcorgao,ipcunidade,
							ipcfuncao,ipcsubfuncao,ipcprograma,ipcacao,ipclocalizador,ipccodigobensservicos,
							ipcquantidadeacrescimo,ipcquantidadereducao,ipcnatureza,ipccodigoresultadoprimario,
							ipciduso,ipcgrupofonte,ipcfonte,ipcidoc,ipcvalorsuplementacao,ipcvalorcancelamento ) 
							VALUES (".$pcrid.",'".$numLinhaPedido."','".$usuario."','".$anoExercicio."',
							'".$momento."','".$tipoCredito."','".$esfera."','".$orgao."','".$unidade."',
							'".$funcao."','".$subfuncao."','".$programa."','".$acao."','".$localizador."',
							'".$codigoBensServicos."','".$quantidadeAcrescimo."','".$quantidadeReducao."',
							'".$natureza."','".$codResultadoPrimario."','".$iduso."','".$grupoFonte."',
							'".$fonte."','".$idoc."','".$suplementacao."','".$cancelamento."')";	
	$db->executar($sql_inclusao);
	$db->commit();
		
	// Recupera o 'ipcid' da tabela elabrev.itempedidocredito
	$sql_recupera = "SELECT max(ipcid) FROM elabrev.itempedidocredito";	
	$ipcid = $db->pegaUm($sql_recupera);

	/*** NÚMERO SEQUENCIAL ***/
	$numeroSequencial = $ipcid;	
	$strLinha .= sprintf("%06s", $numeroSequencial);
		
	$linha = sprintf("%04s", ++$linha);
	//$retorno = fwrite($arquivo, $strLinha);
	echo $strLinha;
}
exit;

/*if($retorno)
	$mensagem = "ESCRITA BEM SUCEDIDA DO ARQUIVO!";
else
	$mensagem = "ERRO NA ESCRITA DO ARQUIVO!";

fclose($arquivo);*/

?>