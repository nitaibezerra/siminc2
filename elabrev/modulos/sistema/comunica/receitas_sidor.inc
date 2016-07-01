<?php

$downloadfile = "receitas_sidor.txt";

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

$sql = "SELECT

d.unicod as unidade, Case when tc.tcrcod in ('102', '103', '160') then tc.tcrcod||'a' else tc.tcrcod end as tipocredito, d.foncod as fonte, 

nr.nrccod as natureza, d.rcuid as sequencial,

sum(d.rcuacrescimo) as acrescimo, SUM(d.rcureducao) as reducao,

'10' as esfera


FROM elabrev.receitaunidade d


INNER JOIN elabrev.tipocredito tc on tc.tcrid = d.tcrid                                                                                       
                                  
				--and tc.tcrcod in ('100', '101', '102', '103', '107', '109', '110', '111', '112', '120', '150', '151', '152', '200', '500', '600', '800', '910')
                --and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '151', '152', '200', '500', '600', '800', '910')
                --and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '176', '196', '200', '500', '910', '920')
				and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920')
LEFT JOIN public.naturezareceita nr on nr.nrcid = d.nrcid 

WHERE 
 
--d.mcrid = 12
--d.mcrid = 13
--d.mcrid = 14
--d.mcrid = 15 and d.unicod <> '26214'
--d.mcrid = 16
--d.mcrid = 18 and rcuano = '". $_SESSION['exercicio'] ."'
--d.mcrid = 22 and rcuano = '". $_SESSION['exercicio'] ."'
d.mcrid = 25 and rcuano = '". $_SESSION['exercicio'] ."'
--d.mcrid = 19 and rcuano = '". $_SESSION['exercicio'] ."'
--d.mcrid = 20 and rcuano = '". $_SESSION['exercicio'] ."'

group by d.unicod , tc.tcrcod , d.foncod , 

nr.nrccod , d.rcuid 

order by unidade, tipocredito";

$dados = $db->carregar($sql);

$cont = 0;
$unidadeOrcamentaria = "";
$unidadeOrcamentariaFixa = "";
$tipoPedidoCredito = "";
$tipoPedidoCreditoFixo = "";

for($i=0; $i<count($dados); $i++) {
	$cont++;
	$unidadeOrcamentaria = $dados[$i]['unidade'];
	$tipoPedidoCredito = $dados[$i]['tipocredito'];
	
	if(($unidadeOrcamentaria == $unidadeOrcamentariaFixa) && ($cont > 1)) {
		if($tipoPedidoCredito != $tipoPedidoCreditoFixo) {
			// Recupera o nº de controle e o pcrid
			$sql_retorno = "select distinct 
								p.pcrid as pcrid, 
								substring(ip.ipcnumerodecontrole,5,4) as controle
							from 
								elabrev.pedidocredito p 
							inner join 
								elabrev.itempedidocredito ip ON ip.pcrid = p.pcrid
							where 
								--ip.ipcnumerodecontrole is not null and ip.ipcnumerodecontrole between '00001715' and '00001792' and
								--ip.ipcnumerodecontrole is not null and ip.ipcnumerodecontrole between '00001793' and '00001801' and 
								--ip.ipcnumerodecontrole is not null and ip.ipcnumerodecontrole between '00001802' and '00001805' and
								--ip.ipcnumerodecontrole is not null and ip.ipcnumerodecontrole between '00004091' and '00004151' and
								--ip.ipcnumerodecontrole is not null and ip.ipcnumerodecontrole between '00004998' and '00005037' and
								--ip.ipcnumerodecontrole is not null and ip.ipcnumerodecontrole between '00001448' and '00001489' AND 
								--ip.ipcnumerodecontrole is not null and ip.ipcnumerodecontrole between '00003580' and '00003638' AND 
								ip.ipcnumerodecontrole is not null and ip.ipcnumerodecontrole between '11046' and '11076' AND 
								--ip.ipcnumerodecontrole is not null and ip.ipcnumerodecontrole between '00001393' and '00001402' AND 
								--ip.ipcnumerodecontrole is not null and ip.ipcnumerodecontrole between '00001403' and '00001447' AND 
								
								p.pcrano = '". $_SESSION['exercicio'] ."' AND
								ip.ipcorgao || ip.ipcunidade = '".$dados[$i]['unidade']."' AND
								ip.ipctipocredito = '".$dados[$i]['tipocredito']."'";
			
			$retorno = $db->carregar($sql_retorno);
			
			$pcrid   = $retorno[0]["pcrid"];
			$numPedidoCredito = sprintf("%04s", $retorno[0]["controle"]);
			$tipoPedidoCreditoFixo = $tipoPedidoCredito;
			$linha = "0001";
		}
	}
	else {
		// Recupera o nº de controle e o pcrid
		$sql_retorno = "select distinct 
							p.pcrid as pcrid, 
							substring(ip.ipcnumerodecontrole,5,4) as controle
						from 
							elabrev.pedidocredito p 
						inner join 
							elabrev.itempedidocredito ip ON ip.pcrid = p.pcrid
						where 
							--ip.ipcnumerodecontrole is not null and ip.ipcnumerodecontrole between '00001715' and '00001792' and 
							--ip.ipcnumerodecontrole is not null and ip.ipcnumerodecontrole between '00001793' and '00001801' and 
							--ip.ipcnumerodecontrole is not null and ip.ipcnumerodecontrole between '00001802' and '00001805' and 
							--ip.ipcnumerodecontrole is not null and ip.ipcnumerodecontrole between '00004091' and '00004151' and 
							--ip.ipcnumerodecontrole is not null and ip.ipcnumerodecontrole between '00004998' and '00005037' and 
							--ip.ipcnumerodecontrole is not null and ip.ipcnumerodecontrole between '00001448' and '00001489' AND 
							--ip.ipcnumerodecontrole is not null and ip.ipcnumerodecontrole between '00003580' and '00003638' AND 
							ip.ipcnumerodecontrole is not null and ip.ipcnumerodecontrole between '11046' and '11076' AND 
							--ip.ipcnumerodecontrole is not null and ip.ipcnumerodecontrole between '00001393' and '00001402' AND 
							--ip.ipcnumerodecontrole is not null and ip.ipcnumerodecontrole between '00001403' and '00001447' AND 
							
							p.pcrano = '". $_SESSION['exercicio'] ."' AND
							ip.ipcorgao || ip.ipcunidade = '".$dados[$i]['unidade']."' AND
							ip.ipctipocredito = '".$dados[$i]['tipocredito']."'";

		$retorno = $db->carregar($sql_retorno);
		
		$pcrid = $retorno[0]["pcrid"];
		$numPedidoCredito = sprintf("%04s", $retorno[0]["controle"]);
		$unidadeOrcamentariaFixa = $unidadeOrcamentaria;
		$tipoPedidoCreditoFixo = $tipoPedidoCredito;
		$linha = "0001";
	}
	
	$strLinha = "";
	if($cont > 1)
		$strLinha = "\r\n".$strLinha;

	/*** NUM DE CONTROLE ***/
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
	$momento = "10";
	$strLinha .= $momento;
		
	/*** TIPO DE CRÉDITO ***/
	if(($dados[$i]['tipocredito'] != "") && ($dados[$i]['tipocredito'] != null)) {

		if( $dados[$i]['tipocredito'] == "102a" || $dados[$i]['tipocredito'] == "103a" || $dados[$i]['tipocredito'] == "160a" )
		{
			$tipoCredito = (string) $dados[$i]['tipocredito'] . " ";
		}
		else
		{
			$tipoCredito = (string) $dados[$i]['tipocredito'] . "  ";
		}
		
	} else {
		$tipoCredito = "   ";
	}
	$strLinha .= $tipoCredito;
	
	/*** ESFERA ***/
	if(($dados[$i]['esfera'] != "") && ($dados[$i]['esfera'] != null)) {
		$esfera = $dados[$i]['esfera'];		
	} else {
		$esfera = "  ";
	}
	$strLinha .= $esfera;
	
	/*** UNIDADE e ÓRGÃO ***/
	if(($dados[$i]['unidade'] != "") && ($dados[$i]['unidade'] != null)) {
		$orgao = substr($dados[$i]['unidade'], 0, 2);
		$unidade = substr($dados[$i]['unidade'], 2);
	}
	else {
		$orgao = "  ";
		$unidade = "   ";
	}
	$strLinha .= $orgao.$unidade;
	
	/*** NATUREZA ***/
	if(($dados[$i]['natureza'] != "") && ($dados[$i]['natureza'] != null)) {
		$natureza = $dados[$i]['natureza'];		
	} else {
		$natureza = "        ";
	}
	$strLinha .= $natureza;
	
	/*** GRUPO DA FONTE e FONTE ***/
	if(($dados[$i]['fonte'] != "") && ($dados[$i]['fonte'] != null)) {
		$grupoFonte = substr($dados[$i]['fonte'], 0, 1);
		$fonte = substr($dados[$i]['fonte'], 1);
	}
	else {
		$grupoFonte = " ";
		$fonte = "  ";
	}
	$strLinha .= $grupoFonte.$fonte;
	
	/*** VALOR DE ACRÉSCIMO ***/
	if(($dados[$i]['acrescimo'] != "") && ($dados[$i]['acrescimo'] != null)) {
		$acrescimo = $dados[$i]['acrescimo'];
	} else {
		$acrescimo = "            ";
	}
	$strLinha .= sprintf("%012s", $acrescimo);
	
	/*** VALOR DE REDUÇÃO ***/
	if(($dados[$i]['reducao'] != "") && ($dados[$i]['reducao'] != null)) {
		$reducao = abs($dados[$i]['reducao']);		
	} else {
		$reducao = "            ";
	}	
	$strLinha .= sprintf("%012s", $reducao);
	
	// Insere na tabela elabrev.itempedidoreceita
	if ( $pcrid ) {
		$sql_inclusao = "INSERT INTO elabrev.itempedidoreceita (pcrid,iprnumerolinhapedido,iprusuario,
								ipranoexercicio,iprmomento,iprtipocredito,ipresfera,iprorgao,iprunidade,
								iprnatureza,iprgrupofonte,iprfonte,iprvaloracrescimo,iprvalorreducao) 
								VALUES (".$pcrid.",'".$numLinhaPedido."','".$usuario."','".$anoExercicio."',
								'".$momento."','".$tipoCredito."','".$esfera."','".$orgao."','".$unidade."',
								'".$natureza."','".$grupoFonte."','".$fonte."','".$acrescimo."','".$reducao."') 
			 			 RETURNING iprid";
		$iprid = $db->pegaUm($sql_inclusao);
		$db->commit();
		
		/*** NÚMERO SEQUENCIAL ***/
		$numeroSequencial = $iprid ;
		$strLinha .= sprintf("%06s", $numeroSequencial);
			
		$linha = sprintf("%04s", ++$linha);
		//$retorno = fwrite($arquivo, $strLinha);
		echo $strLinha;
	
	}

}
exit;

/*if($retorno)
	$mensagem = "ESCRITA BEM SUCEDIDA DO ARQUIVO!";
else
	$mensagem = "ERRO NA ESCRITA DO ARQUIVO!";

fclose($arquivo);*/

?>