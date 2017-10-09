<?php 
include_once APPRAIZ . "includes/classes/dateTime.inc";
require_once APPRAIZ . "includes/Email.php";

function verificaEnquadramentoTributario($docid = null){

	global $db;

	$docid = $docid ? $docid : $_REQUEST['docid'];

	$sql = "SELECT ctrid FROM contratos.faturacontrato WHERE docid = ".$docid;
	$ctrid = $db->pegaUm($sql);
	$sql = "SELECT retid FROM contratos.ctcontrato WHERE ctrid = ".$ctrid;
	$retid = $db->pegaUm($sql);
    if(!$retid){
       return "Este Contrato está sem enquadramento tributário. Favor vincular na aba Dados do Contrato.";
    }else{
    	$sql = "SELECT retir,retcsll,retcofins,retpasep,retoutro FROM contratos.faturacontrato WHERE docid= ".$docid;
    	$dados = $db->pegaLinha($sql);
    	if($dados)
    		$soma = $dados['retir']+$dados['retcsll']+$dados['retcofins']+$dados['retpasep']+$dados['retoutro'];
    	if($soma)
    		return true;
    	else 
    		return "É necessário salvar antes de tramitar para Aprovação.";
  	
    }
}
// VERIFICA SE O VALOR DAS OBs É IGUAL AO VALOR A PARA AO FORNECEDOR
function verificaDiferencaOB($diferenca){
	$diferenca = number_format ( $diferenca, 2 );
	if($diferenca!=0.00){
		return "O valor a pagar ao Fornecedor deve ser igual à soma das Ordens Bancárias.";
	}else
		return true;
}

function pegaPerfil( $usucpf )
{
	global $db;
	$sql = "SELECT pu.pflcod
			FROM seguranca.perfil AS p 
			LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
			WHERE p.sisid = '{$_SESSION['sisid']}'
			AND 
			pu.usucpf = '$usucpf'";
	

	$pflcod = $db->pegaUm( $sql );
	return $pflcod;
}
function redirecionar( $modulo, $acao, $parametros = array() )
{
    $parametros = http_build_query( (array) $parametros, '', '&' );
    header( "Location: ?modulo=$modulo&acao=$acao&$parametros" );
    exit();
}

function headEvento($title, $gestor, $unidadeDemandante, $condicao, $adreferendum, $dtInclusao, $valores = true) {
    global $db;


    if ($_SESSION['eveid']) {
        
        $sql = "
            SELECT  urevalorrecurso, 
                    evecustoprevisto, 
                    urevalorsaldo 
            FROM contratos.unidaderecurso ur
            INNER JOIN contratos.evento ev ON ev.ureid = ur.ureid 
            WHERE ev.eveid = '" . $_SESSION['eveid'] . "'
        ";
        $arrFinanceiroEvento = $db->pegaLinha($sql);
        
        $saldos = '';
        
        if ($valores) {
            $saldos = "
                <td rowspan=5 width=15% valign=top><table class=listagem width=100%>
                    <tr>
                        <td class=\"SubTituloDireita\"><b>Saldo da Unidade:</b></td>
                        <td>" . number_format($arrFinanceiroEvento['urevalorsaldo'], 2, ",", ".") . "</td>
                    </tr>
                    <tr>
                        <td class=\"SubTituloDireita\"><b>Custo de Evento:</b></td>
                        <td>" . number_format($arrFinanceiroEvento['evecustoprevisto'], 2, ",", ".") . "</td>
                    </tr>
                </table>
                </td>
            ";
        }
    }

    $cab = "
        <table align=\"center\" class=\"Tabela\">
            <tbody>
                <tr>
                    <td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Nome do Evento:</td>
                    <td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$title}</td>
                    {$saldos}
                </tr>
                <tr>
                    <td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Fiscal do Evento:</td>
                    <td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$gestor}</td>
                </tr>
    ";

    if (( $condicao != 1) AND ( $condicao != '')) {
        $cab.="
            <tr>
                <td style=\"text-align: right;\" class=\"SubTituloEsquerda\" nowrap>Unidade Demandante:</td>
                <td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$unidadeDemandante}</td>
            </tr>
        ";
    }
    
    if ($adreferendum == 't') {
        $refer = "<img src=\"/imagens/check.jpg\" border=0\">";
    } else {
        $refer = " -- ";
    }
    $cab.="<tr>
						<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">AD Referendum:</td>
						<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$refer}</td>
					</tr>";
    $cab.="<tr>
						<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Data de Inclusão:</td>
						<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dtInclusao}</td>
					</tr>";
    $cab.="	 </tbody>
			</table>";
    echo $cab;
}

function headCompra( $copnumprocesso, $codataabertura, $codsc ){
	$cab = "<table align=\"center\" class=\"Tabela\">
			 <tbody>
				<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">N° do Processo</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$copnumprocesso}</td>
				</tr>
				<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Data de Abertura</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$codataabertura}</td>
				</tr>";

 
	$cab.="<tr>
						<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Tipo de Cotação</td>
						<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$codsc}</td>
					</tr>";
	 
	$cab.="	 </tbody>
			</table>";
	echo $cab; 
}

function montaCabecalhoProcesso($copid, $mostraLink = true){
	
	global $db;
	
	if($copid){
		$sql = "SELECT p.copnumprocesso, p.copdsc,
					  to_char(p.copdatalimite,'dd/mm/YYYY') as copdatalimite,
					  cocdsc  
				FROM contratos.coprocesso p
					left join contratos.cotipocotacao tc on p.cocid = tc.cocid
				WHERE p.copid = $copid";
	
		$dados = $db->pegaLinha($sql);
		$cab = '<table class="tabela" align="center" bgcolor="#f5f5f5" cellpadding="3" cellspacing="1">
	    	<tbody>
		    	<tr>
		        	<td class="SubTituloDireita" style="vertical-align: top; width: 25%;" align="right">Número do Processo:</td>
		        	<td>
		        	';
		if($mostraLink){
			$cab .= '<a href="contratos.php?modulo=principal/cadProcesso&acao=A">'. $dados['copnumprocesso'] .'</a>';
		} else {
			$cab .= $dados['copnumprocesso'];
		}
		$cab .= '</td>      
		    	</tr>
				<tr>
			        <td class="SubTituloDireita" style="vertical-align: top; width: 25%;" align="right">Tipo de Cotação:</td>
			        <td>'. $dados['cocdsc'] .'</td>      
			    </tr>
			    <tr>
			        <td class="SubTituloDireita" style="vertical-align: top; width: 25%;" align="right">Data limite para adesão:</td>
			        <td>'. $dados['copdatalimite'] .'</td>      
			    </tr>
	    	</tbody>
		</table>';
	} else {
		$cab = "";
	}
	echo $cab; 
	
}

function montaCabecalhoUnidade($usgid){
	
	global $db;
	
	if($usgid){
		$sql = "SELECT usgid, usgdsc, usgcod FROM contratos.uasg WHERE usgid = $usgid";
		$dados = $db->pegaLinha($sql);
		$cab = '<table class="tabela" align="center" bgcolor="#f5f5f5" cellpadding="3" cellspacing="1">
	    	<tbody>
		    	<tr>
		        	<td class="SubTituloDireita" style="vertical-align: top; width: 25%;" align="right">Unidade:</td>
		        	<td>'. $dados['usgcod'] .' - '.$dados['usgdsc'] .'</td>      
		    	</tr>
	    	</tbody>
		</table>';
		
	} else {
		$cab = "";
	}
	echo $cab; 
	
}

// Obtem-se o valor total dos empenhos subtraindo os empenhos de anulação/cancelamento.
function pegaValorTotalDosEmpenhos( $nuEmpenhos, $ctrid, $cod_favorecido ){

	global $db, $servidor_bd, $porta_bd, $nome_bd, $usuario_db, $senha_bd, $ini_array;
	
	$ungcod 	= $db->pegaUm( "SELECT 
									es.ungcod AS ungcod 
								FROM
									contratos.empenho_siafi es
								INNER JOIN
									contratos.empenhovinculocontrato evc ON evc.epsid = es.epsid
								WHERE
									evc.ctrid =  {$ctrid}
								ORDER BY
									es.epsid ASC" );
	if($nuEmpenhos) {
		
		$sql = "SELECT 
					( totais.total_positivo + COALESCE( totais.total_negativo, 0 )) AS total
				FROM	( SELECT 
					
					( 
						SELECT
							SUM(es.valor) 
						FROM
								contratos.empenho_siafi es
						INNER JOIN
								contratos.empenhovinculocontrato evc ON evc.epsid = es.epsid
						WHERE 
						evc.ctrid =  {$ctrid}
							AND es.valor > 0 
							AND es.nu_empenho IN ('".implode("','",$nuEmpenhos)."')
							AND es.co_favorecido = '{$cod_favorecido}' 
							AND es.ungcod = '{$ungcod}'
					) AS total_positivo,
			
					( 
						SELECT
							SUM(es.valor) 
						FROM
								contratos.empenho_siafi es
						INNER JOIN
								contratos.empenhovinculocontrato evc ON evc.epsid = es.epsid
						WHERE 
							evc.ctrid =  {$ctrid}
							AND es.valor < 0 
							AND es.nu_empenho IN ('".implode("','",$nuEmpenhos)."')
							AND es.co_favorecido = '{$cod_favorecido}' 
							AND es.ungcod = '{$ungcod}'
					) AS total_negativo
				) 
				AS totais";
		
		$total = $db->pegaUm($sql);
		
		/*					
		// Valores positivos.
		$sql_pos = "select sum(vlr_empenho) as vlr_empenho from (
					(
					SELECT
                		coalesce(sum(valor_transacao),0) as vlr_empenho
					FROM
						siafi2012.ne ne
					WHERE
						substr(numero_ne,12,12) in('".implode("','",$nuEmpenhos)."')
						AND codigo_favorecido = '{$cod_favorecido}'
						AND codigo_ug_operador = '{$ungcod}'
					) UNION ALL (
					SELECT
                		coalesce(sum(valor_transacao),0) as vlr_empenho
					FROM
						siafi2013.ne ne
					WHERE
						substr(numero_ne,12,12) in('".implode("','",$nuEmpenhos)."')
						AND codigo_favorecido = '{$cod_favorecido}' 
						AND codigo_ug_operador = '{$ungcod}' 
					) UNION ALL (
					select
                		coalesce(sum(valor_transacao),0) as vlr_empenho
					FROM
						siafi2014.ne ne
					WHERE
						substr(numero_ne,12,12) in('".implode("','",$nuEmpenhos)."')
						AND codigo_favorecido  = '{$cod_favorecido}' 
						AND codigo_ug_operador = '{$ungcod}' 
					)
					) foo";
		
		// Valores negativos.
		$sql_neg = "select sum(vlr_empenho) as vlr_empenho from (
					(
					SELECT
                		coalesce(sum(valor_transacao),0) as vlr_empenho
					FROM
						siafi2012.ne ne
					WHERE
						substr(numero_ne,12,12) in('".implode("','",$nuEmpenhos)."')
						AND codigo_favorecido = '{$cod_favorecido}'
						AND codigo_ug_operador = '{$ungcod}'
						AND tipo_ne = '0'
						AND esfera_orcamentaria = '0'
					) UNION ALL (
					SELECT
						coalesce(sum(valor_transacao),0) as vlr_empenho
						FROM
						siafi2013.ne ne
					WHERE
						substr(numero_ne,12,12) in('".implode("','",$nuEmpenhos)."')
						AND codigo_favorecido = '{$cod_favorecido}'
						AND codigo_ug_operador = '{$ungcod}'
						AND tipo_ne = '0'
						AND esfera_orcamentaria = '0'
					) UNION ALL (
					SELECT
						coalesce(sum(valor_transacao),0) as vlr_empenho
					FROM
						siafi2014.ne ne
					WHERE
						substr(numero_ne,12,12) in('".implode("','",$nuEmpenhos)."')
						AND codigo_favorecido  = '{$cod_favorecido}'
						AND codigo_ug_operador = '{$ungcod}'
						AND tipo_ne = '0'
						AND esfera_orcamentaria = '0'
						)
					) foo";
		
		$servidor_bd = '';
		$porta_bd = '5432';
		$nome_bd = '';
		$usuario_db = '';
		$senha_bd = '';
			
		$db2 = new cls_banco();
		$total =  $db2->pegaUm($sql_pos) - $db2->pegaUm($sql_neg) ;
		
		if ($_SESSION['baselogin']){
			$servidor_bd        = $ini_array['db']['servidor_bd_'.$_SESSION['baselogin']];
			$porta_bd           = $ini_array['db']['porta_bd_'.$_SESSION['baselogin']];
			$nome_bd            = $ini_array['db']['nome_bd_'.$_SESSION['baselogin']];
			$usuario_db         = $ini_array['db']['usuario_db_'.$_SESSION['baselogin']];
			$senha_bd           = $ini_array['db']['senha_bd_'.$_SESSION['baselogin']];
		} else {
			$servidor_bd        = $ini_array['db']['servidor_bd'];
			$porta_bd           = $ini_array['db']['porta_bd'];
			$nome_bd            = $ini_array['db']['nome_bd'];
			$usuario_db         = $ini_array['db']['usuario_db'];
			$senha_bd           = $ini_array['db']['senha_bd'];
		}
		
		$db = new cls_banco();
		*/
		
		return $total;
		
	} else{
		return '0';
	}
}

function verificaEmpenhoAnulacao( $numero_ne, $ctrid, $ano_ne, $cod_favorecido ){
	
	global $db, $servidor_bd, $porta_bd, $nome_bd, $usuario_db, $senha_bd, $ini_array;
	
	// Obtem o ungcod
	$ungcod = $db->pegaUm( "SELECT 
									es.ungcod AS ungcod
								FROM
									contratos.empenho_siafi es
								INNER JOIN
									contratos.empenhovinculocontrato evc ON evc.epsid = es.epsid
								WHERE
									evc.ctrid =  {$ctrid}
								ORDER BY
									es.epsid ASC" );
	if( $numero_ne ){
		// Se o valor for menor que zero, retorna true.
		$verifica = $db->pegaUm("SELECT
									COUNT(*)
								FROM
									contratos.empenho_siafi es
								INNER JOIN
									contratos.empenhovinculocontrato evc ON evc.epsid = es.epsid
								WHERE
									evc.ctrid =  {$ctrid}
									AND es.valor < 0
									AND es.nu_empenho = '{$numero_ne}'
									AND es.co_favorecido = '{$cod_favorecido}'
									AND es.ungcod = '{$ungcod}'");
		return ( $verifica > 0 ? true : false );
		
	/*	
	$sql = "SELECT
				COUNT(*)
			FROM
				siafi$ano_ne.ne ne
			WHERE
				substr(numero_ne,12,12) = '{$numero_ne}'
				AND codigo_favorecido 	= '{$cod_favorecido}'
				AND codigo_ug_operador 	= '{$ungcod}'
				AND tipo_ne = '0'
				AND esfera_orcamentaria = '0' ";
	
	// Abre conexão
	$servidor_bd = '';
	$porta_bd = '5432';
	$nome_bd = '';
	$usuario_db = '';
	$senha_bd = '';
	
	$db2 = new cls_banco();
	
	$verifica =  $db2->pegaUm($sql);
	
	if ($_SESSION['baselogin']){
		$servidor_bd        = $ini_array['db']['servidor_bd_'.$_SESSION['baselogin']];
		$porta_bd           = $ini_array['db']['porta_bd_'.$_SESSION['baselogin']];
		$nome_bd            = $ini_array['db']['nome_bd_'.$_SESSION['baselogin']];
		$usuario_db         = $ini_array['db']['usuario_db_'.$_SESSION['baselogin']];
		$senha_bd           = $ini_array['db']['senha_bd_'.$_SESSION['baselogin']];
	} else {
		$servidor_bd        = $ini_array['db']['servidor_bd'];
		$porta_bd           = $ini_array['db']['porta_bd'];
		$nome_bd            = $ini_array['db']['nome_bd'];
		$usuario_db         = $ini_array['db']['usuario_db'];
		$senha_bd           = $ini_array['db']['senha_bd'];
	}
	
	$db = new cls_banco();
	*/
	}
}
function pegaSaldoContrato($ctrid, $ftcid = null){

	global $db, $servidor_bd, $porta_bd, $nome_bd, $usuario_db, $senha_bd, $ini_array;

	if($ctrid){
	
		$sql = "SELECT
					CASE WHEN ctrvlrtotal IS NOT NULL
						THEN ctrvlrtotal
						ELSE ctrvlrinicial
					END AS valor_total,
					(SELECT
						SUM(obfvalor) AS total
						FROM 
							contratos.faturacontrato ftc
						JOIN contratos.ordembancariafatura obf ON ftc.ftcid = obf.ftcid
						WHERE 
							ftc.ftcstatus = 'A'
							" . ($ftcid ? " AND ftc.ftcid != {$ftcid}" : "") . "
							AND ftc.ctrid = ctr.ctrid) AS valor_executado,
					(SELECT
						SUM(tdavlr) AS total
						FROM 
							contratos.cttermoaditivo adi
						WHERE 
							adi.tdastatus = 'A'
							AND adi.ctrid = ctr.ctrid) AS valor_aditivo
					FROM contratos.ctcontrato ctr
					WHERE ctr.ctrid = $ctrid";
		$dados = $db->pegaLinha($sql);
		
		$saldo = (($dados['valor_total'] + $dados['valor_aditivo']) - $dados['valor_executado']);
		
		return $saldo;
	}
	
	return '0';
	
}

function montaCabecalhoContrato( $ctrid, $mostraLink = true ){
	
	global $db, $servidor_bd, $porta_bd, $nome_bd, $usuario_db, $senha_bd, $ini_array;
	
	if($ctrid){

		$rsContratosVinculados = $db->carregarColuna("SELECT epsid FROM contratos.empenhovinculocontrato WHERE ctrid={$ctrid}");
		
		// Obtem o nu_empenho de acordo com o contrato
 		$rsContratosVinculados 	= $db->carregar( "SELECT
													es.nu_empenho AS nu_empenho, es.co_favorecido AS codigo_favorecido
												FROM
													contratos.empenho_siafi es
												INNER JOIN
													contratos.empenhovinculocontrato evc ON evc.epsid = es.epsid
												WHERE
													evc.ctrid =  {$ctrid}
 												ORDER BY 
 												es.epsid ASC" );


		if($rsContratosVinculados) {
			$nu_empenho = array();
			foreach($rsContratosVinculados as $key => $contratos):
			   $nu_empenho[$key] = $contratos['nu_empenho'];
			endforeach;
			//$nu_empenho = "'".implode("','",$arrEmpenho)."'";
			$cod_favorecido	= $rsContratosVinculados[0]['codigo_favorecido'];
			$vlr_empenho = pegaValorTotalDosEmpenhos( $nu_empenho, $ctrid, $cod_favorecido );
		}
		
		$faturaContrato   = new FaturaContrato();
		$valorTotalFatura = $faturaContrato->pegaValorTotalPorCtrid($ctrid);

		$param['esdid']   	  = ESTADO_WK_FATURAMENTO_PAGO;
		$valorTotalFaturaPaga = $faturaContrato->pegaValorTotalPorCtrid($ctrid, $param);		
		
		$ordemBancaria 			 = new OrdemBancariaFatura();
		$valorTotalOrdemBancaria = $ordemBancaria->pegaValorTotalPorCtrid( $ctrid );
		
		$valorDeducaoLegal = ($valorTotalFaturaPaga - $valorTotalOrdemBancaria);
		
		$sql = "SELECT 
						ent.entnome as contratada,
						tpc.tpcdsc || ' Nº ' ||  ctr.ctrnum || ' / ' || ctr.ctrano as numcontrato, 
					   mod.moddsc as moddsc,
					   ctr.ctrobj as ctrobj,
					   case 
					   	when ctrvlrtotal is not null then ctrvlrtotal
					   	else ctrvlrinicial
					   end as valor_total,
					   (
						   SELECT 
						   	SUM(a.total)
						   FROM (
							   		SELECT
										obs.valor AS total
									FROM
										contratos.ctcontrato c
									JOIN entidade.entidade e ON e.entid = c.entidcontratada	
									JOIN contratos.hospital h ON h.hspid = c.hspid AND
												     h.hspstatus = 'A'
									JOIN contratos.hospitalug hu ON hu.hspid = h.hspid 
									JOIN contratos.empenhovinculocontrato ec ON ec.ctrid = c.ctrid			     
									JOIN contratos.empenho_siafi es ON es.epsid = ec.epsid AND
													   TRIM(es.ungcod) = TRIM(hu.ungcod) AND
													   TRIM(es.co_favorecido) = TRIM(e.entnumcpfcnpj)
									JOIN contratos.ob_siafi obs ON TRIM(obs.empenho) = TRIM(es.nu_empenho) AND
												       TRIM(obs.unidade) = TRIM(hu.ungcod) AND
												       TRIM(obs.it_co_credor) = TRIM(e.entnumcpfcnpj)
									WHERE
										c.ctrid = {$ctrid} AND
										obs.ob NOT IN (
														SELECT 
															obf.obfnumero 
														FROM 
															contratos.faturacontrato fc
														JOIN contratos.ordembancariafatura obf ON obf.ftcid = fc.ftcid
														WHERE 
															fc.ftcstatus = 'A' AND
															fc.ctrid = {$ctrid}
														)
							   	UNION ALL	
								   SELECT 
										SUM(obfvalor) AS total
									FROM contratos.faturacontrato ftc
									JOIN contratos.ordembancariafatura obf ON ftc.ftcid = obf.ftcid
									WHERE ftc.ftcstatus = 'A'
										  AND ftc.ctrid = ctr.ctrid
							) AS a						
						) AS valor_executado,
						( 
							SELECT SUM(ftcglosa) AS total FROM contratos.faturacontrato ftc WHERE ftc.ftcstatus = 'A'
										  AND ftc.ctrid = ctr.ctrid
						)as glosa,
						(select 
							sum(tdavlr) as total
						from contratos.cttermoaditivo adi
						where adi.tdastatus = 'A'
						and adi.ctrid = ctr.ctrid) as valor_aditivo,
						hspabrev || ' - '|| hspdsc as contratante
				FROM contratos.ctcontrato ctr
					left join contratos.ctmodalidadecontrato mod on ctr.modid = mod.modid
					left join contratos.cttipocontrato tpc on ctr.tpcid = tpc.tpcid
					left join entidade.entidade ent on ent.entid = ctr.entidcontratada 
					left join contratos.hospital h on h.hspid = ctr.hspid 
				WHERE ctr.ctrid = $ctrid";
		$dados = $db->pegaLinha($sql);
		
		$valorTotal = ($dados['valor_total']+$dados['valor_aditivo']);
		//$saldo = ($vlr_empenho-$dados['valor_executado']);
		$saldo = ($valorTotal-$dados['valor_executado']);
		
		$valorContratoPago 			= ($valorTotal - $valorTotalFaturaPaga);
		
		$valorContratoComprometido 	= ($valorTotal - $valorTotalFatura) + $dados['glosa'];
		
		$cab = '<table class="tabela" align="center" bgcolor="#f5f5f5" cellpadding="3" cellspacing="1" >
	    	<tbody>
		    	<tr>
		        	<td class="SubTituloDireita" style="vertical-align: top; width: 25%;" align="right"><b>Número:</b></td>
		        	<td>
		        	';
		if($mostraLink){
			$cab .= '<a href="contratos.php?modulo=principal/cadContrato&acao=A">'. $dados['numcontrato'] .'</a>';
		} else {
			$cab .= $dados['numcontrato'];
		}
		$cab .= '</td>      
		    	</tr>
				<tr>
			        <td class="SubTituloDireita" style="vertical-align: top; width: 25%;" align="right"><b>Unidade Contratante:</b></td>
			        <td>'. $dados['contratante'] .'</td>      
			    </tr>
			    <tr>
			        <td class="SubTituloDireita" style="vertical-align: top; width: 25%;" align="right"><b>Contratada:</b></td>
			        <td>'. $dados['contratada'] .'</td>      
			    </tr>    		
				<tr>
			        <td class="SubTituloDireita" style="vertical-align: top; width: 25%;" align="right"><b>Modalidade:</b></td>
			        <td>'. $dados['moddsc'] .'</td>      
			    </tr>
			    <tr>
			        <td class="SubTituloDireita" style="vertical-align: top; width: 25%;" align="right"><b>Objeto:</b></td>
			        <td>'. $dados['ctrobj'] .'</td>      
			    </tr>';
		
		$cab .= '<tr>
					<td class="SubTituloDireita" valign="top">Valores do contrato:</td>
					<td>
						<table class="listagem" align="center" cellpadding="3" cellspacing="1">
							<thead>
							<tr>
								<th align="center" valign="top">
									(A)<br>
									Valor do contrato
								</th>
								<th align="center" valign="top">
									(B)<br>
									Valor Empenhado
									<br>
									<a class="notprint" title="Visualizar dados do empenho" href="javascript:janela(\'?modulo=principal/popUpFatura&acao=A&requisicao=popUpEmpenho\', \'detalheempenho\', 500, 600);">			        					
						        		<img src="/imagens/icone_lupa.png" border="0">					
						        	</a>
								</th>
								<th align="center" valign="top">
									(C)<br>
									Valor das Notas Ficais Cadastradas
									<br>
									<a class="notprint" title="Visualizar as notas ficais cadastradas" href="javascript:janela(\'?modulo=principal/popUpFatura&acao=A&requisicao=popUpListaFatura\', \'detalhenf\', 500, 600);">			        					
						        		<img src="/imagens/icone_lupa.png" border="0">					
						        	</a>
								</th>
								<th align="center" valign="top">
									(D)<br>
									Glosa
									<br>

								</th>
								<th align="center" valign="top">
									(E)<br>
									Valor das Notas Ficais Pagas
									<br>
									<a class="notprint" title="Visualizar as notas ficais pagas" href="javascript:janela(\'?modulo=principal/popUpFatura&acao=A&requisicao=popUpListaFatura&nf=paga\', \'detalhenf\', 500, 600);">			        					
						        		<img src="/imagens/icone_lupa.png" border="0">					
						        	</a>
								</th>
								<th align="center" valign="top">
									(F)<br>
									Valor das Ordens Bancárias
									<br>
									<a class="notprint" title="Visualizar as ordens bancárias" href="javascript:janela(\'?modulo=principal/popUpFatura&acao=A&requisicao=popUpListaOB\', \'detalheob\', 500, 600);">			        					
						        		<img src="/imagens/icone_lupa.png" border="0">					
						        	</a>
								</th>

								<th align="center" valign="top">
									(G)<br>
									Saldo do Contrato<br>
									(G) = (A) - (C - D)
								</th>
							</thead>			
							</tr>
							
							<tr style="color:#0066CC;">
								<td align="right" id="vl_total_contrato">
									'. ( $valorTotal < 0 
											? '<font color=red>'.number_format($valorTotal, 2, ',', '.').'</font>' 
											: number_format($valorTotal, 2, ',', '.') ) .'
								</td>
								<td align="right">'.				
					        		( $vlr_empenho < 0 
					        			? '<font color=red>'.number_format($vlr_empenho, 2, ',', '.').'</font>' 
					        			: number_format((( $vlr_empenho) ? $vlr_empenho : "0"), 2, ',', '.') ) .'
								</td>
								<td align="right">
									'. number_format($valorTotalFatura, 2, ',', '.') .'
								</td>
								<td align="right">
									'. number_format($dados["glosa"], 2, ',', '.') .'
								</td>
								<td align="right">
									'. number_format($valorTotalFaturaPaga, 2, ',', '.') .'
								</td>
								<td align="right">
									'. number_format($valorTotalOrdemBancaria, 2, ',', '.') .'
								</td>
				<!--							
								<td align="right">
									'. ($valorDeducaoLegal < 0 
											? '<div style="color:red;">' . number_format($valorDeducaoLegal, 2, ',', '.') . '</div>' 
											: number_format($valorDeducaoLegal, 2, ',', '.')) .'
								</td>
								<td align="right">
									'. ($valorContratoPago < 0 
											? '<div style="color:red;">' . number_format($valorContratoPago, 2, ',', '.') . '</div>' 
											: number_format($valorContratoPago, 2, ',', '.')) .'
								</td>
				-->									
								<td align="right">
									'. ($valorContratoComprometido < 0 
											? '<div style="color:red;">' . number_format($valorContratoComprometido, 2, ',', '.') . '</div>' 
											: number_format($valorContratoComprometido, 2, ',', '.')) .'
								</td>
							</tr>
						</table>
					</td>
				 </tr>';
		
		
// 		if ( $dados['valor_total'] != $valorTotal ):
// 			$cab .='<tr>
// 				        <td class="SubTituloDireita" style="vertical-align: top; width: 25%;" align="right"><b><span title="Valor Inicial Contrato">Valor original</span>:</b></td>
// 				        <td>'. ( $dados['valor_total'] < 0 ? '<font color=red>'.number_format($dados['valor_total'], 2, ',', '.').'</font>' : number_format($dados['valor_total'], 2, ',', '.') ) .'</td>      
// 				    </tr>';
// 		endif;
		
		$cab .='	</tbody>
				</table>';
	} else {
		$cab = "";
	}
	echo $cab; 
	
}


/**
 * @author: Pedro Dantas
 * @date: 18/02/2009
 * @params: no
 * @returns: boolean 
 * @coments: verifica se os eventos já cadastrados deste usuario tem notas técnicaas
 * 			 caso ja tenha 2 eventos cadastrados sem nota técnica a função retorna 'false' 
 */
function verificaEventos( $ungcod ){
	global $db;
 
//	$sqlEvePassados = "select 
//						evedatafim , 
//						eveid,
//						evedatafim - integer '10' 
//						from contratos.evento 
//						where  
//						evedatafim < date(now()) - integer '10' and 
//						evedatafim - integer '10' < now() 
//						and evestatus = 'A'
//						and ungcod = '".$ungcod."'
//						order by evedatafim ";
	
	$sqlEvePassados = "
							select 
						 *,
						e.evedatafim - integer '10' 
						from contratos.evento e
						inner join workflow.documento as d on d.docid = e.docid
						inner join workflow.estadodocumento as es on es.esdid = d.esdid
						where  
						e.evedatafim < date(now()) - integer '10' and 
						e.evedatafim - integer '10' < now() 
						and e.evestatus = 'A'
						and ungcod = '".$ungcod."'
						and es.esdid <> ".CADASTRAMENTO_WF."
						order by e.evedatafim ";
	
	$arrEventos = $db->carregar( $sqlEvePassados );
 	$arrSemNota = array();
 	$arrSemAval = array(); 
 	$arrTemNota = array();
 	$rsSemNota  = array();
 	$pend = 0;
 	
	for( $i = 0; $i < count( $arrEventos ); $i++){ 
		if( $arrEventos[$i]['eveid']!= '' ){ 
			$sqlBuscaNota = "SELECT distinct tpaid FROM contratos.anexoevento where eveid =  ".$arrEventos[$i]['eveid']." and tpaid = 1 and axestatus = 'A'";
			$tpaid        = $db->pegaUm( $sqlBuscaNota );			
			if( !$tpaid ){
				array_push( $arrSemNota , 'sem_nota' ); 
			}
			$sqlBuscaAval = "select e.eveid from contratos.evento as e inner join contratos.avaliacaoevento as a on a.eveid = e.eveid where e.eveid = ".$arrEventos[$i]['eveid'];
			$aval = $db->pegaUm( $sqlBuscaAval );
			if( !$aval){
				array_push( $arrSemAval , 'sem_aval' ); 
			}
		}
	}	 
	//$numEventosSemNota = count( $arrSemNota ); 
	$numEventosSemNota = 0;
	$numEventosSemAval = count( $arrSemAval ); 
	return $numEventosSemNota.'_'.$numEventosSemAval; 
}

function existeAvaliacao(){
	global $db;
	$sql = "select e.eveid from contratos.evento as e inner join contratos.avaliacaoevento as a on a.eveid = e.eveid 
			where e.eveid = ".$_SESSION['eveid'];
	if( $_SESSION['eveid'] ){
		$evento = $db->pegaUm( $sql );
		
		if( $evento ) {
			return true;
		}else{
			return false;
		}	
	}else{
		return false;
	}
}

function getUnidadeByCpf( $eveid = false ){	
	global $db;	
	if( !$eveid ){
		$sql = "SELECT ungcod FROM seguranca.usuario WHERE usucpf = '{$_SESSION['usucpf']}'"; 
	}else{
		$sql = "SELECT ungcod FROM contratos.evento WHERE eveid = ".$_SESSION['eveid'];
	}	
	$cod = $db->pegaUm( $sql ); 
	return $cod;
}

function validaTramit()
{
	global $db; 
	
	$sql = " 
			SELECT DISTINCT
				e.eveid, 
				e.evetitulo, 
				ta.tpaid, 
				unr.urevalorrecurso, 
				e.evecustoprevisto
			FROM 
				contratos.evento AS e
			INNER JOIN 
				contratos.itemevento AS itm ON itm.eveid = e.eveid 
			INNER JOIN 
				contratos.anexoevento AS anx ON anx.eveid = e.eveid 
			INNER JOIN 
				contratos.tipoanexo AS ta ON ta.tpaid = anx.tpaid 
			INNER JOIN 
				contratos.unidaderecurso AS unr ON unr.ureid = e.ureid
			WHERE 
				--e.usucpf = '{$_SESSION['usucpf']}' 
				e.eveid = {$_SESSION['eveid']} 
			AND 
				e.evestatus ='A'
			AND 
				ta.tpaid = 2 
			AND 
				anx.axestatus = 'A'
			AND 
				e.evenumeropi IS NOT NULL
			AND 
				e.eveanopi IS NOT NULL
			AND 
				unr.urevalorrecurso IS NOT NULL";
	 
	//ver($sql);
	$rs = $db->pegaLinha( $sql );
	
	if( $rs ){
		if($rs['urevalorrecurso'] >= $rs['evecustoprevisto']){ 
			return true;
		}		 
	}
	return false;		
}
 
function pre( $var1, $die = false )
{
	if( $var1 != '' )
	{
		echo("<pre>");
			   	print_r( $var1 );
		echo("</pre>");
	}  
	if( $die == 1 )
		die();
}

/**
 * Recupera o docid vinculado ao evento
 * 
 * @author Felipe Tarchiani Cerávolo Chiavicatti
 * @param (int|null) $eveid Se for null, assumirá o valor da SESSION['eveid']
 * @return (int|null) docid
 */
function evtPegarDoc($eveid=null){
	global $db;
	
	$eveid = $eveid ? $eveid : $_SESSION['eveid'];
	
	$sql = "SELECT
				docid
			FROM 
				contratos.evento
			WHERE
				eveid = {$eveid}";
	
	return $db->pegaUm($sql);
}

/**
 * Inseri o evento no documento, fazendo com o mesmo entre do Workflow.
 * 
 * @author Felipe Tarchiani Cerávolo Chiavicatti
 * @param (int|null) $eveid Se for null, assumirá o valor da SESSION['eveid']
 * @return (int) docid
 */
function evtCriarDoc($eveid=null){
	global $db;
	
	$eveid = $eveid ? $eveid : $_SESSION['eveid'];
	
	if (!$eveid)
		return false;
		
	$docid = evtPegarDoc($eveid);
	
	if (!$docid){
		/*
		 * Pega tipo do documento "WORKFLOW"
		 */		
		$sql = "SELECT
					tpdid
				FROM
					workflow.tipodocumento
				WHERE
					tpdid =".WF_TPDID_EVENTOS;
		
		$tpdid = $db->pegaUm($sql);
		/*
		 * Pega nome do evento
		 */		
		$sql = "SELECT
					evetitulo
				FROM
					contratos.evento
				WHERE
					eveid ={$eveid}";
		
		$tit = $db->pegaUm($sql);
		
		$docdsc = "Cadastramento Evento - " . $tit;		
		/*
		 * cria documento WORKFLOW
		 */
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );
		/*
		 * Atualiza o $docid no evento
		 */
		$sql = "UPDATE contratos.evento SET 
					docid = '".$docid."' 
				WHERE
					eveid = {$eveid}";
		
		$db->executar( $sql );		
		$db->commit();		
	}
	
	return $docid;
}
function arrayPerfil(){
	global $db;
	
	$sql = sprintf("SELECT
					 pu.pflcod
					FROM
					 seguranca.perfilusuario pu
					 INNER JOIN seguranca.perfil p ON p.pflcod = pu.pflcod AND
					 	p.sisid = " . $_SESSION['sisid'] . "
					WHERE
					 pu.usucpf = '%s'
					ORDER BY
					 p.pflnivel",
				$_SESSION['usucpf']);
	
	return (array) $db->carregarColuna($sql,'pflcod');
}
 
function montaBradScrum( $arrLinks ){
	
	if( is_array( $arrLinks ) ) {

		echo("<table width=\"100%\" align=\"center\"class=\"tabela\">");
		echo("<tr>");
		echo("<td>");
			for($i = 0; $i<count( $arrLinks ); $i++) {
	
				$texto = $arrLinks[$i]['texto'];
				$link  = $arrLinks[$i]['link'];
				
				$content .= " <img align=\"absmiddle\" src=\"/imagens/arrow_h.png\" /> <a href=\"$link\"> $texto </a>";			
			}
			echo("<b>Você está em:</b> $content");
		echo("</td>");
		echo("</tr>");
		echo("</table>"); 
	} 
}

// enviar email para o gestor financeiro da UG, gestor da UG e fiscal especifico da aba designação fiscal
function enviarEmailPago($ctrid, $nf) {
    global $db;

    $sql = "select
                entnome,
                entnumcpfcnpj,
                entemail
            from
                contratos.fiscalcontrato fsc
            inner join
                entidade.entidade ent ON ent.entid = fsc.entid
            where
                ctrid = $ctrid
            and
                fsc.fscstatus = 'A'
            order by
                entnome";


    $dados = $db->carregar($sql);

    $arrRemetente['email'] = '';
    $arrRemetente['nome'] = "SIG - CONTRATOS";

    for ($x = 0; $x <= count($dados); $x++) {

        $assunto   = "[SIG] Módulo de Contratos";
        $mailBody = "    Prezado(a) {$dados[$x]['entnome']}
        A fatura (Nota Fiscal) Nº $nf está no estado: Pago.
        Para acessá-lo basta abrir o módulo 'Contratos' no endereço http://sig.ebserh.gov.br";

        $mail = new PHPMailer();
        $mail->IsSMTP();
        $mail->SMTPDebug  = 0;
        $mail->Debugoutput = 'html';
        $mail->Host       = "172.17.61.46";
        $mail->Port       = 25;
        $mail->SMTPAuth   = false;
        $mail->From =  '';
        $mail->FromName = "SIG - CONTRATOS";
        $mail->AddAddress($dados[$x]['entemail']);
        $mail->IsHTML(true);
        $mail->Subject  = $assunto; // Assunto da mensagem
        $mail->Body = html_entity_decode($mailBody); //Conteudo
        $mail->Send();

    }

    return true;
}

// mandar email para o gestor financeiro da unidade

function enviarEmailFinanceiro($ctrid, $nf) {

    global $db;

    $sql = "select
                entnome,
                entnumcpfcnpj,
                entemail
            from
                contratos.fiscalcontrato fsc
            inner join
                entidade.entidade ent ON ent.entid = fsc.entid
            where
                ctrid = $ctrid
            and
                fsc.fscstatus = 'A'
            order by
                entnome";


    $dados = $db->carregar($sql);

    $arrRemetente['email'] = '';
    $arrRemetente['nome'] = "SIG - CONTRATOS";

    for ($x = 0; $x <= count($dados); $x++) {

        $assunto   = "[SIG] Módulo de Contratos";
        $mailBody = "    Prezado(a) {$dados[$x]['entnome']}
        A fatura (Nota Fiscal) Nº $nf está no estado: Aguardando Pagamento e aguarda trâmite.
        Para acessá-lo basta abrir o módulo 'Contratos' no endereço http://sig.ebserh.gov.br";

        $mail = new PHPMailer();
        $mail->IsSMTP();
        $mail->SMTPDebug  = 0;
        $mail->Debugoutput = 'html';
        $mail->Host       = "172.17.61.46";
        $mail->Port       = 25;
        $mail->SMTPAuth   = false;
        $mail->From =  '';
        $mail->FromName = "SIG - CONTRATOS";
        $mail->AddAddress($dados[$x]['entemail']);
        $mail->IsHTML(true);
        $mail->Subject  = $assunto; // Assunto da mensagem
        $mail->Body = html_entity_decode($mailBody); //Conteudo
        $mail->Send();

    }

    return true;
}

// cadastramento para triagem: para o fiscal especifico da aba designação de fiscal e todos usuários com perfil triagem da UG do contrato
// pagamento para triagem: quando retorna do aguardando pagamento envia somente para o perfil triagem na UG
function enviarEmailTriagem($ctrid, $nf) {

    global $db;

    $sql = "select
                entnome,
                entnumcpfcnpj,
                entemail
            from
                contratos.fiscalcontrato fsc
            inner join
                entidade.entidade ent ON ent.entid = fsc.entid
            where
                ctrid = $ctrid
            and
                fsc.fscstatus = 'A'
            order by
                entnome";


    $dados = $db->carregar($sql);

    $arrRemetente['email'] = '';
    $arrRemetente['nome'] = "SIG - CONTRATOS";

    for ($x = 0; $x <= count($dados); $x++) {

        $assunto   = "[SIG] Módulo de Contratos";
        $mailBody = "    Prezado(a) {$dados[$x]['entnome']}
        A fatura (Nota Fiscal) Nº $nf está no estado: Em triagem, aguarda tramite.
        Para acessá-lo basta abrir o módulo 'Contratos' no endereço http://sig.ebserh.gov.br";

        $mail = new PHPMailer();
        $mail->IsSMTP();
        $mail->SMTPDebug  = 0;
        $mail->Debugoutput = 'html';
        $mail->Host       = "172.17.61.46";
        $mail->Port       = 25;
        $mail->SMTPAuth   = false;
        $mail->From =  '';
        $mail->FromName = "SIG - CONTRATOS";
        $mail->AddAddress($dados[$x]['entemail']);
        $mail->IsHTML(true);
        $mail->Subject  = $assunto; // Assunto da mensagem
        $mail->Body = html_entity_decode($mailBody); //Conteudo
        $mail->Send();

    }

    return true;
}


// quando retorna da traigem envia email somente para o fiscal especifico que está na aba designação fiscal
function enviarEmailCadastramento($ctrid, $nf) {

    global $db;

    $sql = "select
                entnome,
                entnumcpfcnpj,
                entemail
            from
                contratos.fiscalcontrato fsc
            inner join
                entidade.entidade ent ON ent.entid = fsc.entid
            where
                ctrid = $ctrid
            and
                fsc.fscstatus = 'A'
            order by
                entnome";


    $dados = $db->carregar($sql);

    $arrRemetente['email'] = '';
    $arrRemetente['nome'] = "SIG - CONTRATOS";

    for ($x = 0; $x <= count($dados); $x++) {

        $assunto   = "[SIG] Módulo de Contratos";
        $mailBody = "    Prezado(a) {$dados[$x]['entnome']}
        A fatura (Nota Fiscal) Nº $nf está no estado: Em Cadastramento.
        Para acessá-lo basta abrir o módulo 'Contratos' no endereço http://sig.ebserh.gov.br";

        $mail = new PHPMailer();
        $mail->IsSMTP();
        $mail->SMTPDebug  = 0;
        $mail->Debugoutput = 'html';
        $mail->Host       = "172.17.61.46";
        $mail->Port       = 25;
        $mail->SMTPAuth   = false;
        $mail->From =  '';
        $mail->FromName = "SIG - CONTRATOS";
        $mail->AddAddress($dados[$x]['entemail']);
        $mail->IsHTML(true);
        $mail->Subject  = $assunto; // Assunto da mensagem
        $mail->Body = html_entity_decode($mailBody); //Conteudo
        $mail->Send();

    }

    return true;
}

function enviarEmailConfirm(){
 
	global $db;
	
//	if(enviarEmailPorEstadoWorkflow()){
//		return true;
//	}else{
//		return false;
//	}
//	
//	exit();
	
	$sql = "SELECT 
				evetitulo,  
				to_char(evedatainicio::date,'DD/MM/YYYY') as evedatainicio, 
				to_char(evedatafim::date,'DD/MM/YYYY') as evedatafim,
				ureid,
				evecustoprevisto
			FROM 
				contratos.evento 
			WHERE 
				eveid = '{$_SESSION['eveid']}'";
	 
	$rs = $db->carregar( $sql );
 
	$arrEmails = array($_SESSION['email_sistema']);

 /*
	for ( $i=0;$i<count($arrEmails);$i++ ) {
		$arrDestinatarios[$i]['usunome']  = $arrEmails[$i];
		$arrDestinatarios[$i]['usuemail'] = $arrEmails[$i];
	}
 */
	$remetente = array('nome'=>REMETENTE_WORKFLOW_NOME, 'email'=>REMETENTE_WORKFLOW_EMAIL);
					  
	$assunto   = "[SIMEC] Novo Evento cadastrado no SIMEC - Módulo de Eventos";					
	$mailBody = '
	Prezados Senhores, <br> 
	<br>
	Informamos que o evento nº '.$_SESSION['eveid'].' - "'.$rs[0]['evetitulo'].'" a ser realizado no período de '.$rs[0]['evedatainicio'].' à '.$rs[0]['evedatafim'].',<br>
	foi cadastrado no SIMEC e enviado para análise e aprovação do comitê de eventos.<br>
	<br>
	<br>	
	<a href="http://simec.mec.gov.br">Clique Aqui para acessar o SIMEC.</a>
	<br>	
	<br>		
	Atenciosamente,<br>
	<br>
	<br>	
	'.$remetente['nome'].'<br>
	';
	
	atulaizarSaldoEnvio($rs);
 	
	if($_REQUEST['esdid'] == EM_ANALISE_COMITE_WF){
		
		if(verificaPrazoConformeComite()){
				
			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
			return true;
		 
		} else {
			
			 return false;
		}
		
	} else {
		
		enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		return true;
	}
	
	
//	if(verificaVoltaEstadoWorflow()){
//		
//		enviarEmailPorEstadoWorkflow();
//	}

}
/*
function eventoPosAcaoAssinadoSaa(){
	global $db;
	
	$sql = "SELECT 
				evetitulo,  
				to_char(evedatainicio::date,'DD/MM/YYYY') as evedatainicio, 
				to_char(evedatafim::date,'DD/MM/YYYY') as evedatafim,
				eveemail,
				evecustoprevisto
			FROM 
				contratos.evento 
			WHERE 
				eveid = '{$_SESSION['eveid']}'";
	 
	$rs = $db->carregar( $sql );
 
	$destinatario = $rs[0]['eveemail'];

	$remetente = array('nome'=>REMETENTE_WORKFLOW_NOME, 'email'=>REMETENTE_WORKFLOW_EMAIL);
					  
	$assunto   = "[SIMEC] Evento Assinado pelo SAA - Módulo de Eventos";					
	$mailBody = '
	Prezado(s) Senhor(es), <br> 
	<br>
	Informamos que o evento nº '.$_SESSION['eveid'].' - "'.$rs[0]['evetitulo'].'" a ser realizado no período de '.$rs[0]['evedatainicio'].' à '.$rs[0]['evedatafim'].',<br>
	foi Assinado pelo SAA. É necessário o preenchimento da ficha de avaliação.<br>
	<br>
	<br>	
	<a href="http://simec.mec.gov.br">Clique Aqui para acessar o SIMEC.</a>
	<br>	
	<br>		
	Atenciosamente,<br>
	<br>
	<br>	
	'.$remetente['nome'].'<br>
	';
	
	enviar_email($remetente, $destinatario, $assunto, $mailBody, $mailCopia );
	return true;
		
}
*/

function eventoPosAcaoGerarOS(){
	global $db;
	/*
	$sql = "SELECT 
				evetitulo,  
				to_char(evedatainicio::date,'DD/MM/YYYY') as evedatainicio, 
				to_char(evedatafim::date,'DD/MM/YYYY') as evedatafim,
				eveemail,
				evecustoprevisto
			FROM 
				contratos.evento 
			WHERE 
				eveid = '{$_SESSION['eveid']}'";
	 
	$rs = $db->carregar( $sql );
 
	$destinatario = $rs[0]['eveemail'];

	$remetente = array('nome'=>REMETENTE_WORKFLOW_NOME, 'email'=>REMETENTE_WORKFLOW_EMAIL);
					  
	$assunto   = "[SIMEC] Evento Assinado pelo SAA - Módulo de Eventos";					
	$mailBody = '
	Prezado(s) Senhor(es), <br> 
	<br>
	Informamos que o evento nº '.$_SESSION['eveid'].' - "'.$rs[0]['evetitulo'].'" a ser realizado no período de '.$rs[0]['evedatainicio'].' à '.$rs[0]['evedatafim'].',<br>
	é necessário o preenchimento da ficha de avaliação.<br>
	<br>
	<br>	
	<a href="http://simec.mec.gov.br">Clique Aqui para acessar o SIMEC.</a>
	<br>	
	<br>		
	Atenciosamente,<br>
	<br>
	<br>	
	'.$remetente['nome'].'<br>
	';
	
	enviar_email($remetente, $destinatario, $assunto, $mailBody, $mailCopia );
	return true;
	*/
	
	$eveid = $_SESSION['eveid'];
	
	if(!$eveid) return false;
	
	//gera numero O.S.
	$sql = "SELECT (COALESCE(max(gnosequencial),0)+1) as total FROM contratos.geranumeroos";
	$seqos = $db->pegaUm($sql);
	
	$sql = "INSERT INTO contratos.geranumeroos(gnosequencial) VALUES ($seqos)";
	$db->executar($sql);
	
	//busca dados pregao
	$sql = "SELECT precodpregao, precnpj, prerazaosocial, prenumcontrato FROM contratos.pregaoevento where prestatus='A' and CURRENT_DATE between preiniciovig and prefimvig limit 1";
	$dadosPregao = $db->pegaLinha($sql);

	//busca dados evento
	$sql = "SELECT evenumeroprocesso, evedatainicio, evedatafim, evecustoprevisto FROM contratos.evento where eveid=$eveid";
	$dadosEvento = $db->pegaLinha($sql);
	
	//insere O.S.
	$sql = "
            INSERT INTO 
                contratos.ordemservico(
	            eveid, 
	            osenumeroos, 
	            osedataemissaoos, 
	            osedatainiciofinal, 
	            osedatafimfinal, 
	            osecustofinal, 
	            oseobsos, 
	            osecnpj, 
	            oserazaosocial, 
	            oseproposta, 
	            osecodpregao, 
	            oseordenador, 
	            oseempenho, 
                    osenumcontrato,
	            oseststus                    
                )VALUES (
                    $eveid, 
                    $seqos, 
                    CURRENT_DATE, 
                    '".$dadosEvento['evedatainicio']."', 
                    '".$dadosEvento['evedatafim']."', 
                    null,  
                    null, 
                    '".$dadosPregao['precnpj']."', 
                    '".$dadosPregao['prerazaosocial']."', 
                    null, 
                    '".$dadosPregao['precodpregao']."', 
                    null, 
                    null, 
                    '".$dadosPregao['prenumcontrato']."',
                    'A'                    
                )
        ";
	$db->executar($sql);
	$db->commit();
	
	return true;
	
}

function eventoEnviarPagamento(){
	global $db;
	
	//verifica perfil DRP
	/*
	if( pegaPerfil($_SESSION['usucpf']) != EVENTO_PERFIL_DRP && pegaPerfil($_SESSION['usucpf']) != EVENTO_PERFIL_SUPER_USUARIO ){
		return 'É necessário possuir o perfil DRP!';
	}
	*/
	
	//verifica se preencheu a avaliação (itens radio)
	$verificaAvaliacao = true;
	
	$sql = "select aquid, aqudescricao from contratos.assuntoquestao order by aquid ";
	$rsAssunto = $db->carregar( $sql );
	for( $i = 0; $i< count($rsAssunto); $i++){
		$sql = "select q.qavid, q.qevdescricao, tq.tqadescricao 
				from contratos.questaoavaliacao as q 
				inner join contratos.tipoquestaoavaliacao as tq on q.tqaid = tq.tqaid 
				where q.qevstatus = 'A' 
				and q.aquid = '".$rsAssunto[$i]['aquid']."'";
        $rsQestaoAvaliacao = $db->carregar( $sql );
        for( $j = 0; $j < count( $rsQestaoAvaliacao ); $j++ ){
            	
           	$qavid = $rsQestaoAvaliacao[$j]['qavid'];
            $sql = "SELECT count(eavid) FROM contratos.avaliacaoevento WHERE eveid = ".$_SESSION['eveid']." AND qavid = $qavid";
			$tem = $db->pegaUm( $sql );
			if( $tem == 0 ){
				$verificaAvaliacao = false;		
			}
	   }
	}
	
	//verifica se preencheu a avaliação (itens textarea)
	$sqlQSub = "select * from contratos.questaosubjetivaevento";
	$rsQSub = $db->carregar( $sqlQSub );
	for( $s = 0; $s <count( $rsQSub ); $s++ ){
 		$sqlResp  = "SELECT r.rasresposta FROM contratos.avaliacaosubjetivaevento as a
					INNER JOIN contratos.respostaavaliacaosubjetivaeve as r ON a.rasid = r.rasid
					WHERE a.eveid = ".$_SESSION['eveid']." AND a.qusid = {$rsQSub[$s]['qusid']}";
		$rasresposta = $db->pegaUm( $sqlResp );
		if( !$rasresposta ){
			$verificaAvaliacao = false;		
		}
	}
	
	if($verificaAvaliacao == false){
		return 'É necessário preencher toda a ficha de avaliação!';
	}
	
	return true;
}


function eventoRegistrarPagamento(){
	global $db;
	
	if(!$_SESSION['eveid']) return 'Sessão expirou. Entre novamente no sistema.';	
	
 	$sql = "SELECT count(dpaid) FROM contratos.documentopagamento WHERE eveid = ".$_SESSION['eveid'];
	$tem = $db->pegaUm( $sql );
	if( $tem == 0 ){
		return false;		
	}
	
	return true;
}


function atulaizarSaldoEnvio($rs = null){
    global $db;

    if(!$rs){
        $sql = "
            SELECT  evetitulo,  
                    to_char(evedatainicio::date,'DD/MM/YYYY') as evedatainicio, 
                    to_char(evedatafim::date,'DD/MM/YYYY') as evedatafim,
                    ureid,
                    evecustoprevisto
            FROM contratos.evento 
            WHERE eveid = '{$_SESSION['eveid']}'
        ";
        $rs = $db->carregar( $sql );
    }

    $sql = "
        INSERT INTO contratos.unidadecontacorrente(
                ureidpai, 
                eveid, 
                uccdesclancamento, 
                uccvalorlancamento, 
                uccdatalancamento, 
                ucccpf
            )VALUES(
                {$rs[0]['ureid']}, 
                {$_SESSION['eveid']}, 
                '".addslashes( $rs[0]['evetitulo'] )."', 
                {$rs[0]['evecustoprevisto']}, 
                '".date('Y-m-d')."', 
                '{$_SESSION['usucpf']}');
    ";

    $sql .= "
        UPDATE contratos.unidaderecurso SET urevalorsaldo = urevalorsaldo-{$rs[0]['evecustoprevisto']} where ureid = {$rs[0]['ureid']};
    ";

    $db->executar($sql);
    if($db->commit()){
            return true;
    }
    return false;
}

function atualizarSaldoVoltarUnidade()
{
	global $db;
	
	if(!$rs){
		$sql = "SELECT 
					evetitulo,  
					to_char(evedatainicio::date,'DD/MM/YYYY') as evedatainicio, 
					to_char(evedatafim::date,'DD/MM/YYYY') as evedatafim,
					ureid,
					evecustoprevisto
				FROM 
					contratos.evento 
				WHERE 
					eveid = '{$_SESSION['eveid']}'";
	 
		$rs = $db->carregar( $sql );
	}
	
	$sql = "
        INSERT INTO contratos.unidadecontacorrente(
                ureidpai, 
                eveid, 
                uccdesclancamento, 
                uccvalorlancamento, 
                uccdatalancamento, 
                ucccpf
            )VALUES(
				{$rs[0]['ureid']}, 
                {$_SESSION['eveid']}, 
                'Estorno', 
                {$rs[0]['evecustoprevisto']}, 
                '".date('Y-m-d')."', 
                '{$_SESSION['usucpf']}');
    ";
	
	$sql .= " UPDATE contratos.unidaderecurso SET urevalorsaldo = urevalorsaldo+{$rs[0]['evecustoprevisto']} WHERE ureid = {$rs[0]['ureid']}; ";
	
	$db->executar($sql);
	if($db->commit()){
		return true;
	}
	return false;
}

function aprovarEvento(){
	global $db;
	if( $_SESSION['eveid'] != '' ){ 
		$id = $_SESSION['eveid'];
		$sql = "UPDATE contratos.evento SET sevid = 3 WHERE eveid = $id ";
		$up = $db->executar( $sql );
		$db->commit();
	}
}
 function listaSituacaoPorUF($id = "tabela_1",$sql,$titulo = null,$cabecalho = null,$sqlAgrupador = array(),$exibeSoma = "N",$link = array(),$arrOff = array()){
	 global $db;
	 $dados = $db->carregar($sql);
	 
	 $tabela = '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
	 
	 
	 if(!$dados){
	 	$tabela .= "<tr><td align=center ><span style=\"color:#990000\" >Não existem Registros.</span></td></tr></table>";
	 	echo $tabela; 
	 	return false;
	 }

	 $num_colunas = count($dados[0]);
	 $num_colunas = $num_colunas - (count($arrOff));
	 
	 if($titulo){
	 	$tabela .= "<tr bgcolor=#CCCCCC ><td colspan=\"$num_colunas\" align=center ><b>$titulo</b></td></tr>";
	 }
	 
	 if($cabecalho){
	 	$tabela .= "<tr bgcolor=#e9e9e9 onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='#e9e9e9'\" >";
		 $i = 0;
		 while($i < $num_colunas){
		 	$tabela .= "<td><b>".$cabecalho[$i]."</b></td>";
		 	$i++;
		 }
		 $tabela .= "</tr>";
	 }
	 $id_span = 1;
	 $i = 0;
	 foreach($dados as $d){
	 	($i % 2)? $cor = "#F7F7F7" : $cor = "#FCFCFC";
	 	
	 	$tabela .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";
	 	
	 	$sqlAg = $sqlAgrupador['sql'];
	 	
	 	if($sqlAgrupador['sql']){
	 		if($sqlAgrupador['agrupador'] && $d[$sqlAgrupador['agrupador']]){
	 			$sqlAg = str_replace("|agrupador|",$d[$sqlAgrupador['agrupador']],$sqlAg);
	 			$dadosAgrupados = $db->carregar($sqlAg);
	 		}else{
	 			$dadosAgrupados = "";
	 		}
	 		$listaAgrupada = '<table cellspacing="0" cellpadding="2" border="0" align="center" width="100%" class="listagem">';
	 		
	 		if(!$dadosAgrupados){
	 			$listaAgrupada .= "<tr><td><span style=\"color:#990000\" >Não existem registros.</span></td></tr>";
	 		}else{
	 	
	 			$xx = 0;
	 			foreach($dadosAgrupados as $dA){
	 				($xx % 2)? $cor = "#F7F7F7" : $cor = "#FCFCFC";
	 				$listaAgrupada .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";

	 				foreach($dA as $k => $dd){
	 					$kk[] = $k;
	 				}
	 				$ii = 0;			
	 				while($ii < count($dA)){
	 					
	 					if($sqlAgrupador['link']){
	 						if($sqlAgrupador['campo']){
	 							if(is_array($sqlAgrupador['campo'])){
	 								unset($arrCampos);
	 								foreach($sqlAgrupador['get'] as $cmp){
	 									$arrCampos[] = "{$cmp}={$dA[$cmp]}";
	 									$campos = implode("&",$arrCampos);
	 								}
	 							}else{
	 								$campos = "{$sqlAgrupador['get']}={{$dA[$kk[$sqlAgrupador['get']]]}}"; 
	 							}
	 						}

	 						$linkAg_a = "<a href=\"".$sqlAgrupador['link']."&".$campos."\" />";
	 						$linkAg_b = " </a>";
	 					}
	 					
	 					if($kk[$ii] == $kk[0]){
	 						$seta_filho = "<img src=\"../imagens/seta_filho.gif\" />";
	 					}else{
	 						$seta_filho = "";
	 					}

	 					if(!strstr($kk[$ii],"id") && !strstr($kk[$ii],"ordem") && !in_array($kk[$ii],$sqlAgrupador['arrOff'])){
	 					
	 						if(in_array($kk[$ii],$sqlAgrupador['exibeLink'])){
	 						
			 					if(is_numeric($dA[$kk[$ii]])){
							 		$campo = str_replace(",",".",number_format($dA[$kk[$ii]]));
							 		$listaAgrupada .= "<td align=\"right\"><span style=\"color:rgb(0, 102, 204);text-align:right\" >$seta_filho $linkAg_a $campo $linkAg_b</span></td>";
			 					}
							 	else{
							 		if( $dA[$kk[$ii]] == '' ){
							 			$dA[$kk[$ii]] = "sem estado cadastrado";
							 			$linkAg_a	  = "";
							 			$linkAg_b	  = "";
							 		}
							 		$listaAgrupada .= "<td>$seta_filho $linkAg_a {$dA[$kk[$ii]]} $linkAg_b</td>";
							 	}
	 						}
	 						else{
	 							if(is_numeric($dA[$kk[$ii]])){
							 		$campo = str_replace(",",".",number_format($dA[$kk[$ii]]));
							 		$listaAgrupada .= "<td align=\"right\" ><span style=\"color:rgb(0, 102, 204);text-align:right;width:100%\" >$seta_filho $campo</span></td>";
			 					}
							 	else{
							 		$listaAgrupada .= "<td>$seta_filho {$dA[$kk[$ii]]}</td>";
							 	}
	 						}						 	
	 					}
						$ii++;
	 					
	 				}
	 				$listaAgrupada .= "</tr>";
	 			$xx++;
	 			}
	 		}
	 		$listaAgrupada .= "</table>";
	 	}
	 	
	 	$keys = array_keys($d);
	 	$j = 0;
		while($j < $num_colunas){
			if($sqlAgrupador && $keys[$j] == $keys[0] && $dadosAgrupados){
				$img = "<img onclick=\"exibeAgrupador('{$id}_{$id_span}')\" style=\"cursor:pointer\" id=\"img_mais_{$id}_{$id_span}\" align=\"abdmiddle\" src=\"../imagens/mais.gif\" title=\"Abrir\" />
						<img onclick=\"escondeAgrupador('{$id}_{$id_span}')\" style=\"cursor:pointer;display:none\" id=\"img_menos_{$id}_{$id_span}\" align=\"abdmiddle\" src=\"../imagens/menos.gif\" title=\"Fechar\" /> ";
				$span = "<tr style=\"display:none\" bgcolor='#EEE9E9' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='#EEE9E9'\" id=\"tr_view_{$id}_{$id_span}\"><td colspan=\"$num_colunas\">$listaAgrupada</td></td></tr>";	
				$id_span ++; 
			}
			else{
				$img = "&nbsp;&nbsp;&nbsp;&nbsp;";
			}
						
			//Monta os links;
			if($link && $dadosAgrupados){
				$link_a = "<a href=\"{$link['link']}&{$link['get']}=".$d[$link['get']]."\" >";
				$link_b = "</a>";
			}else{
				$link_a = "";
				$link_b = "";
			}
			
			
			if(!strstr($keys[$j],"id") && !strstr($keys[$j],"ordem") && !in_array($keys[$j],$arrOff)){
				
				if(is_numeric($d[$keys[$j]])){
					$tabela .= "<td align=\"right\">";
				}else{
					$tabela .= "<td>";
				}
				
				if($link['campo'] == $keys[$j]){
					$tabela .= $img.$link_a;
				}else{
					$tabela .= $img;
				}
			 	if(is_numeric($d[$keys[$j]])){
			 		$campo = str_replace(",",".",number_format($d[$keys[$j]]));
			 		$tabela .= "<span style=\"color:rgb(0, 102, 204)\" >".$campo.$link_b."</span></td>";
			 	}else{
				 	if($link['campo'] == $keys[$j]){
						$tabela .= $d[$keys[$j]].$link_b."</td>";
					}else{
						$tabela .= $d[$keys[$j]]."</td>";
					}
			 		
			 	}

			}
		 	
		 	if(!strstr($keys[$j],"ordem") && is_numeric($d[$keys[$j]])  && !in_array($keys[$j],$arrOff)){
		 		$soma[$keys[$j]] += $d[$keys[$j]];
		 		$campo_soma[] = $keys[$j];
		 	}
		 	$j++;
		 	
		}
		
	 	$tabela .= "</tr>";
	 	$tabela .= $span;
	 	
	 	$i++;
	 }
	 
	 foreach($keys as $k => $k1){
	 	 if(strstr($k1,"id")){
	 	 	unset ($keys[$k]);
	 	 }
	 }
	 	 
	 //Exibe Soma
	 if($exibeSoma == "S"){
	 	$tabela .= "<tr bgcolor='DCDCDC' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='DCDCDC'\" >";
	 	$campo_soma = array_unique($campo_soma);
	 	foreach($keys as $k1 => $k){
	 		
	 		if(!in_array($k,$arrOff)){
	 		
		 		if(in_array($k,$campo_soma)){
		 			$tabela .= "<td align=\"right\" ><b>".str_replace(",",".",number_format($soma[$k]))."</b></td>";
		 		}elseif($k1 == 0){
		 			$tabela .= "<td><b>Total:</b></td>";
		 		}else{
		 			$tabela .= "<td></td>";
		 		}
	 		}
	 	}
	 	$tabela .= "</tr>";
	 }
	 
	 $tabela .= "</table>";
	 $tabela .="<script>
	 function exibeAgrupador(id){
	 	var img_mais = document.getElementById('img_mais_' +id);
	 	var img_menos = document.getElementById('img_menos_' +id);
	 	var tr_view = document.getElementById('tr_view_' +id);
	 	
	 	img_mais.style.display = 'none';
	 	img_menos.style.display = '';
	 	tr_view.style.display = '';
	 	
	 }
	 
	 function escondeAgrupador(id){
	 	var img_mais = document.getElementById('img_mais_' +id);
	 	var img_menos = document.getElementById('img_menos_' +id);
	 	var tr_view = document.getElementById('tr_view_' +id);
	 	
	 	img_mais.style.display = '';
	 	img_menos.style.display = 'none';
	 	tr_view.style.display = 'none';
	 	
	 }
	 
	 			</script>";
	 
	 echo $tabela;
}

/**
 * Listar as entidadades e seus itens.
 *
 * @author Juliano Meinen de Souza
 * @param (int,sql,string,array,array,string,array,array)
 * @return (string) lista de unidades
 */                       
/*Função para montar lista com Agrupador e Links*/

/*
 * PEDRO DANTAS, FAVOR NÃO APAGAR ESSA FUNÇÃO!
 * R) - JULIANO, NÃO CRIPTOGRAFA AS FUNCOES! 
 * 
 */

function listaUnidadesLink($id = "tabela_1",$sql,$titulo = null,$cabecalho = null,$sqlAgrupador = array(),$exibeSoma = "N",$link = array(),$arrOff = array()){
     global $db;
     $dados = $db->carregar($sql);
    
     $tabela = '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
    
    
     if(!$dados){
         $tabela .= "<tr><td align=center ><span style=\"color:#990000\" >Não existem Registros.</span></td></tr></table>";
         echo $tabela;
         return false;
     }

     $num_colunas = count($dados[0]);
     $num_colunas2 = count($dados[0]);
     $num_colunas = $num_colunas - (count($arrOff));
         
     foreach($dados[0] as $kkk => $ddd){
         if(strstr($kkk,"id") || strstr($kkk,"ordem")){
             $num_colunas2 --;
         }
     }
    
     $num_colunas2 = $num_colunas2 - (count($arrOff));
    
     if($titulo){
         $tabela .= "<tr bgcolor=#CCCCCC ><td colspan=\"$num_colunas2\" align=center ><b>$titulo</b></td></tr>";
     }
    
     if($cabecalho){
         $tabela .= "<tr bgcolor=#e9e9e9 onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='#e9e9e9'\" >";
         $i = 0;
         while($i < $num_colunas2){
             $tabela .= "<td style=\"text-align:center\" ><b>".$cabecalho[$i]."</b></td>";
             $i++;
         }
         $tabela .= "</tr>";
     }
     $id_span = 1;
     $i = 0;
     foreach($dados as $d){
         ($i % 2)? $cor = "#F7F7F7" : $cor = "#FCFCFC";
         
         $tabela .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";
         
         $sqlAg = $sqlAgrupador['sql'];
         
         if($sqlAgrupador['sql']){
             if($sqlAgrupador['agrupador'] && $d[$sqlAgrupador['agrupador']]){
                 $sqlAg = str_replace("|agrupador|",$d[$sqlAgrupador['agrupador']],$sqlAg);
                 $dadosAgrupados = $db->carregar($sqlAg);
             }else{
                 $dadosAgrupados = "";
             }
             $listaAgrupada = '<table cellspacing="0" cellpadding="2" border="0" align="center" width="100%" class="listagem">';
             
             if(!$dadosAgrupados){
                 $listaAgrupada .= "<tr><td><span style=\"color:#990000\" >Não existem registros.</span></td></tr>";
             }else{
                 
                 if(is_array($sqlAgrupador['cabecalho'])){
                     $listaAgrupada .= "<tr bgcolor=#e9e9e9 onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='#e9e9e9'\" >";
                     foreach($sqlAgrupador['cabecalho'] as $agCabecalho){
                         $listaAgrupada .= "<td style=\"text-align:center\" ><b>".$agCabecalho."</b></td>";
                     }
                 }
                 
                 $xx = 0;
                 foreach($dadosAgrupados as $dA){
                     ($xx % 2)? $cor = "#F7F7F7" : $cor = "#FCFCFC";
                     $listaAgrupada .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";

                     foreach($dA as $k => $dd){
                         $kk[] = $k;
                     }
                     $ii = 0;            
                     while($ii < count($dA)){
                         
                         if($sqlAgrupador['link']){
                             if($sqlAgrupador['campo']){
                                 if(is_array($sqlAgrupador['campo'])){
                                     unset($arrCampos);
                                     foreach($sqlAgrupador['get'] as $cmp){
                                         $arrCampos[] = "{$cmp}={$dA[$cmp]}";
                                         $campos = implode("&",$arrCampos);
                                     }
                                 }else{
                                     $campos = "{$sqlAgrupador['get']}={{$dA[$kk[$sqlAgrupador['get']]]}}";
                                 }
                             }

                             $linkAg_a = "<a href=\"".$sqlAgrupador['link']."&".$campos."\" />";
                             $linkAg_b = " </a>";
                         }
                         
                         if($kk[$ii] == $kk[0]){
                             $seta_filho = "<img src=\"../imagens/seta_filho.gif\" />";
                         }else{
                             $seta_filho = "";
                         }

                         if(!strstr($kk[$ii],"id") && !strstr($kk[$ii],"ordem") && !in_array($kk[$ii],$sqlAgrupador['arrOff'])){
                         
                             if(in_array($kk[$ii],$sqlAgrupador['exibeLink'])){
                             
                                 if(is_numeric($dA[$kk[$ii]])){
                                     $campo = str_replace(",",".",number_format($dA[$kk[$ii]]));
                                     $listaAgrupada .= "<td>$seta_filho $linkAg_a $campo $linkAg_b</td>";
                                 }
                                 else{
                                     $listaAgrupada .= "<td>$seta_filho $linkAg_a {$dA[$kk[$ii]]} $linkAg_b</td>";
                                 }
                             }
                             else{
                                 if(is_numeric($dA[$kk[$ii]])){
                                     $campo = str_replace(",",".",number_format($dA[$kk[$ii]]));
                                     $listaAgrupada .= "<td>$seta_filho $campo</td>";
                                 }
                                 else{
                                     $listaAgrupada .= "<td>$seta_filho {$dA[$kk[$ii]]}</td>";
                                 }
                             }
                             
                         }
                        $ii++;
                         
                     }
                     $listaAgrupada .= "</tr>";
                 $xx++;
                 }
             }
             $listaAgrupada .= "</table>";
         }
         
         $keys = array_keys($d);
         $j = 0;
        while($j < $num_colunas){
            if($sqlAgrupador && $keys[$j] == $keys[0] && $dadosAgrupados){
                $img = "<img onclick=\"exibeAgrupador('{$id}_{$id_span}')\" style=\"cursor:pointer;vertical-align: baseline;\" id=\"img_mais_{$id}_{$id_span}\" src=\"../imagens/mais.gif\" title=\"Abrir\" />
                        <img onclick=\"escondeAgrupador('{$id}_{$id_span}')\" style=\"cursor:pointer;display:none;vertical-align: baseline\" id=\"img_menos_{$id}_{$id_span}\" src=\"../imagens/menos.gif\" title=\"Fechar\" /> ";
                $span = "<tr style=\"display:none\" bgcolor='#EEE9E9' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='#EEE9E9'\" id=\"tr_view_{$id}_{$id_span}\"><td colspan=\"$num_colunas\">$listaAgrupada</td></td></tr>";    
                $id_span ++;
            }
            else{
                $img = "&nbsp;&nbsp;&nbsp;&nbsp;";
            }
                        
            //Monta os links;
            if($link && $dadosAgrupados){
                $link_a = "<a href=\"{$link['link']}&{$link['get']}=".$d[$link['get']]."\" >";
                $link_b = "</a>";
            }else{
                $link_a = "";
                $link_b = "";
            }
            
            
            if(!strstr($keys[$j],"id") && !strstr($keys[$j],"ordem") && !in_array($keys[$j],$arrOff)){
                $tabela .= "<td><center>";
                if($link['campo'] == $keys[$j]){
                    $tabela .= $img.$link_a;
                }else{
                    $tabela .= $img;
                }
                 if(is_numeric($d[$keys[$j]])){
                     $campo = str_replace(",",".",number_format($d[$keys[$j]]));
                     $tabela .= $campo.$link_b."</center></td>";
                 }else{
                     if($link['campo'] == $keys[$j]){
                        $tabela .= $d[$keys[$j]].$link_b."</center></td>";
                    }else{
                        $tabela .= $d[$keys[$j]]."</center></td>";
                    }
                     
                 }

            }
             
             if(!strstr($keys[$j],"ordem") && is_numeric($d[$keys[$j]])  && !in_array($keys[$j],$arrOff)){
                 $soma[$keys[$j]] += $d[$keys[$j]];
                 $campo_soma[] = $keys[$j];
             }
             $j++;
             
        }
        
         $tabela .= "</tr>";
         $tabela .= $span;
         
         $i++;
     }
    
     foreach($keys as $k => $k1){
          if(strstr($k1,"id")){
              unset ($keys[$k]);
          }
     }
         
     //Exibe Soma
     if($exibeSoma == "S"){
         $tabela .= "<tr bgcolor='DCDCDC' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='DCDCDC'\" >";
         $campo_soma = array_unique($campo_soma);
         foreach($keys as $k1 => $k){
             
             if(!in_array($k,$arrOff)){
             
                 if(in_array($k,$campo_soma)){
                     $tabela .= "<td><b>Total:</b> ".str_replace(",",".",number_format($soma[$k]))."</td>";
                 }else{
                     $tabela .= "<td></td>";
                 }
             }
         }
         $tabela .= "</tr>";
     }
    
     $tabela .= "</table>";
     $tabela .="<script>
     function exibeAgrupador(id){
         var img_mais = document.getElementById('img_mais_' +id);
         var img_menos = document.getElementById('img_menos_' +id);
         var tr_view = document.getElementById('tr_view_' +id);
         
         img_mais.style.display = 'none';
         img_menos.style.display = '';
         tr_view.style.display = '';
         
     }
    
     function escondeAgrupador(id){
         var img_mais = document.getElementById('img_mais_' +id);
         var img_menos = document.getElementById('img_menos_' +id);
         var tr_view = document.getElementById('tr_view_' +id);
         
         img_mais.style.display = '';
         img_menos.style.display = 'none';
         tr_view.style.display = 'none';
         
     }
    
                 </script>";
    
     echo $tabela;
} 
 
function evtPegarDocCompra($coaid=null){
	global $db;
	
	$coaid = $coaid ? $coaid : $_SESSION['coaid'];
	
	$sql = "SELECT
				docid
			FROM 
				contratos.coadesao
			WHERE
				coaid = {$coaid}";
	 
	return $db->pegaUm($sql);
}
 
function evtCriarDocCompra($coaid=null){
	global $db;
	
	$coaid = $coaid ? $coaid : $_SESSION['coaid'];
	
	if (!$coaid)
		return false;
		
	$docid = evtPegarDocCompra($coaid);
	 
	if (!$docid){
		/*
		 * Pega tipo do documento "WORKFLOW"
		 */		
		$sql = "SELECT
					tpdid
				FROM
					workflow.tipodocumento
				WHERE
					sisid =". $_SESSION['sisid'] ."
				AND
					tpdid = ". WF_TPDID_COMPRAS;
		
		$tpdid = $db->pegaUm($sql);
		/*
		 * Pega nome do evento
		 */		
		$sql = "SELECT
					c.copnumprocesso 
				FROM
					contratos.coadesao as ca
					inner join contratos.coprocesso as c on c.copid = ca.copid
				WHERE
					ca.coaid = $coaid";
		
		$tit = $db->pegaUm($sql);
		
		$docdsc = "Cadastramento Compras - " . $tit;		
		/*
		 * cria documento WORKFLOW
		 */
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );
		/*
		 * Atualiza o $docid no coadesao
		 */
		$sql = "UPDATE contratos.coadesao SET 
					docid = '".$docid."' 
				WHERE
					coaid = {$coaid}";
		
		$db->executar( $sql );		
		$db->commit();		
	}
	
	return $docid;
}

function montaAbasCompras( $linkAtual ){
	global $db;
	if( $linkAtual == '' ){
		return false;
	}
	$perfis = arrayPerfil();
	$res = array( 
 					0 => array ( "descricao" => "Processos",
						    "id" 		=> "4",
						    "link" 		=> "/evento/contratos.php?modulo=inicio&acao=C&submod=compra"
				  		  ),
				 	1 => array ( "descricao" => "Dados do Processo",
										    "id" 		=> "4",
										    "link" 		=> "/evento/contratos.php?modulo=principal/cadProcesso&acao=A"
								  		  )  );				
					 	if( $_SESSION['copid'] != '' ) {
							array_push($res,
											array ("descricao" => "Documentos Anexos",
													    "id"        => "3",
													    "link" 		=> "/evento/contratos.php?modulo=principal/cadCompraAnexo&acao=A"
													   )  );
						}					 			  
						array_push( $res, 
							 		array ("descricao" => "Registrar Demandas",
														    "id"		=> "2",
														    "link"		=> "/evento/contratos.php?modulo=principal/cadCompraInfra&acao=A"
												  		   )  );
				 		array_push( $res, 
					 		 		array ("descricao" => "Endereços de Entrega",
														    "id"		=> "1",
														    "link"		=> "/evento/contratos.php?modulo=principal/cadCompraEnd&acao=A"
												  		   )  );		
						array_push( $res,  	
							 		array ("descricao" => "Cadastrar Itens",
										    "id"		=> "1",
										    "link"		=> "/evento/contratos.php?modulo=principal/cadCompraItem&acao=A"
						  		   						   )  );
					 	   
	echo montarAbasArray($res, $_REQUEST['org'] ? false : $linkAtual);	
}

//function pegarEntidInstituicao($usucpf){
//	
//	global $db;
//	$sql = "select 
//			pflcod 
//	from 
//		contratos.usuarioresponsabilidade 
//	where 
//		usucpf = '$usucpf'";
//
//	$pflcod = $db->pegaUm($sql);
//	
//	$sql2 = "select
//			distinct 
//				ent.entid
//			from
//				contratos.usuarioresponsabilidade ur
//			inner join 
//				public.unidade p ON ur.unicod = p.unicod
//			inner join 
//				entidade.entidade ent ON ur.unicod = ent.entunicod
//			inner join 
//				entidade.entidadeendereco entEnd ON ent.entid = entEnd.entid
//			inner join 
//				entidade.endereco ende ON ende.endid = entEnd.endid
//			inner join
//				contratos.coenderecoentrega coend ON coend.entid = ent.entid
//			inner join
//				territorios.municipio mun on coend.muncod = mun.muncod
//			inner join
//				territorios.estado est on est.estuf = mun.estuf
//			where
//				ur.rpustatus = 'A' and
//				ur.usucpf = '$usucpf' and
//				ur.pflcod = $pflcod and
//				ur.prsano = '".$_SESSION['exercicio']."' and
//				coend.coendstatus = 'A'";
// 
//	return $db->pegaUm( $sql2 );
//}
 
function verificaAdesao(){
	global $db;
	
	//if( !pegaCoaid($_SESSION['copid'],$_SESSION['unidade'])){
		$sql ="
		insert into contratos.coadesao ( usucpf, docid, copid, usgid, coadatainclusao )
		values ( '".$_SESSION['usucpf']."', NULL, {$_SESSION['copid']}, {$_SESSION['unidade']}, 'now()' )
		returning coaid
		";
//ver($sql,d);
		$coaid = $db->pegaUm( $sql );
		$_SESSION['coaid'] = $coaid;
		evtCriarDocCompra();
		$db->commit();	
		return $coaid;
				
	//}
}

function gravaGestor($coaid, $usgid, $usucpfgestor){
	global $db;
	$sql = "update contratos.coadesao 
			set usucpfgestor = '{$usucpfgestor}' 
	 		where coaid = {$coaid} and usgid = {$usgid}";

	 $db->executar( $sql );		
	
	return true;
}

function pegaGestor($coaid, $usgid){
	global $db;
	$sql = "select usucpfgestor
		    from contratos.coadesao 
			where coaid = {$coaid} and usgid = {$usgid}";

	 $usucpfgestor = $db->pegaUm( $sql );		
	
	if( $usucpfgestor ){
		return $usucpfgestor;
	}else{
		return false;			
	}
	 
	
}


function pegaCoaid($copid, $usgid, $criaAdesao = true){
	global $db;
	$sql = "select coaid from contratos.coadesao where copid = {$copid} and usgid = {$usgid}";
	$coaid = $db->pegaUm( $sql );
	
	
	if( $coaid ){
		$_SESSION['coaid'] = $coaid;
		return $coaid;
	}else{
		if($criaAdesao){
			return verificaAdesao();			
		}
	}
	return false;
}

function validaTramiteCompras(){
	global $db; 
	
	if($_SESSION['copid'] && $_SESSION['unidade']) {
		$coaid = pegaCoaid($_SESSION['copid'], $_SESSION['unidade']);
		
		$sql = " SELECT DISTINCT 
							'<center><a href=\"javascript:carregaDetalheItemCadastrado('|| cd.cotid ||', ''cadastrado'', \''|| c.coidsc ||'\');\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\"> <a href=\"#\" onclick=\"javascript:excluirItemProcesso('|| cd.cotid ||', ''cadastrado'');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>'  as acao,                       
	                     	c.coidsc,
	            			c.coiqtde,
	            			c.coivlrreferenciamin,
	            			c.coivlrreferenciamax 
					FROM contratos.codemandaitem AS cd 
					INNER JOIN contratos.coitemprocesso AS ci ON ci.cotid = cd.cotid
					INNER JOIN contratos.coitem AS c ON c.coiid = ci.coiid
					WHERE cd.coaid = {$coaid}";
		$rsItens = $db->carregar( $sql );
		
		$sql = "select
					count(ee.coeid) as c
				FROM 
					contratos.coenderecoentrega  ee 
				INNER JOIN contratos.coadesao a on a.coaid = ee.coaid
				WHERE 
					a.copid = ". $_SESSION['copid'] ." 
				AND a.usgid = ". $_SESSION['unidade'];
		
		$rsEndereco = $db->pegaUm( $sql );
	}
	
	
	if($rsItens && $rsEndereco) {
		return true;
	} else {
		return false;
	}
}

function verificaUnidadesPermitidadas(){
	global $db;
	
	# Array de perfis que veem todas as unidades
	$arPerfisVerTodas = array(EVENTO_PERFIL_CGCC, 
						 	  EVENTO_PERFIL_CONSULTA,
						 	  EVENTO_PERFIL_PERFIL_EMPRESA,
						 	  EVENTO_PERFIL_SUPER_USUARIO						 	  
							  );
	# Array de perfis que so veem somente as unidades atribuidas						 	  
	$arPerfisUnidadesAtribuidas = array(EVENTO_PERFIL_ORDENADOR_DESPESA_COMPRAS,
										EVENTO_PERFIL_CONSULTA_COMPRAS,
							 	  		EVENTO_PERFIL_DEMANDANTE_COMPRAS);
							 	  
	# Array de perfis vinculado ao perfil do usuário
	$arPerfilVinculado = array();
	# Array de Unidade Visiveis para o perfil do usuário
	$arUnidadesVisiveis = array();
	$arUnidadesVisiveisTemp = array();

	# Recuperamos todos o perfis cadastrado para o usuário logado
	$arPerfis = arrayPerfil();
	foreach($arPerfis as $perfil){
		if(in_array($perfil,$arPerfisVerTodas)){
			return true;
		} elseif(in_array($perfil,$arPerfisUnidadesAtribuidas)){
			$arPerfilVinculado[] = $perfil;
		}
	}
	
	if(is_array($arPerfilVinculado)){
		$sql = "SELECT usgid FROM contratos.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod in (". implode(",", $arPerfilVinculado).")";
		$arUnidadesVisiveisTemp = $db->carregar($sql);
		
		if(is_array($arUnidadesVisiveisTemp)){
			extract($arUnidadesVisiveisTemp);
			foreach($arUnidadesVisiveisTemp as $unidadesVisiveis){
				$arUnidadesVisiveis[] = $unidadesVisiveis['usgid'];
			}
		} else {
			return "Não existe Unidades atribuidas ao perfil para este CPF: {$_SESSION['usucpf']}.";
		}
	}

	return $arUnidadesVisiveis;
}

function removerdeclaracao($dados = false) {
	global $db;
	
	if(!$dados)
		$dados['decid'] = $db->pegaUm("SELECT decid FROM contratos.declaracao WHERE copid='".$_SESSION['copid']."' AND usgid='".$_SESSION['unidade']."' AND decstatus='A'"); 
		
	$sql = "SELECT arqid FROM contratos.declaracao WHERE decid = '".$dados['decid']."'";
	$arqid = $db->pegaUm($sql);

	$sql = "DELETE FROM contratos.declaracao WHERE decid='".$dados['decid']."'";
	$db->executar($sql);
	//deletando pdf em public.arquivo
	if($arqid){
		$sql ="DELETE FROM public.arquivo WHERE arqid = '$arqid'";
		$db->executar($sql);
	}
	$db->commit();
	//deletando o arquivo pdf físico do servidor
	if($arqid){
		$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
		
		if(file_exists($caminho)){
			unlink($caminho);
		}
	}
	echo "<script>
			alert('Declaração removida com sucesso.');
			window.location = '?modulo=principal/formDeclaracao&acao=A';
		  </script>";
}

function verificaDeclaracao(){
	global $db;
	$sql = "SELECT 
				decid 
			FROM 
				contratos.declaracao dec 
			INNER JOIN contratos.coprocesso cop on dec.copid = cop.copid 
			WHERE 
				cop.copdatalimite >= CURRENT_DATE 
			AND dec.usgid = '".$_SESSION['unidade']."' 
			AND dec.copid = '".$_SESSION['copid']."' AND decstatus='A'";
	
	$decid = $db->pegaUm($sql);
	if(!$decid){
		return false;		
	} else {
		return true;	
	}
}

function pegaPerfilArray($cpf,$sisid){
	global $db;
	$sql = "select p.pflcod from seguranca.perfilusuario pu inner join seguranca.perfil p on pu.pflcod = p.pflcod where pu.usucpf = '$cpf' and p.pflstatus = 'A' and p.sisid = $sisid;";
	return $db->carregarColuna($sql);
}

function possuiPerfil( $pflcods ){
	
	global $db;
	
	if ($db->testa_superuser()) {
		return true;
	}else{
		if ( is_array( $pflcods ) ){
			$pflcods = array_map( "intval", $pflcods );
			$pflcods = array_unique( $pflcods );
		} else {
			$pflcods = array( (integer) $pflcods );
		} if ( count( $pflcods ) == 0 ) {
			return false;
		}
		$sql = "select
					count(*)
			from seguranca.perfilusuario
			where
				usucpf = '" . $_SESSION['usucpf'] . "' and
				pflcod in ( " . implode( ",", $pflcods ) . " ) ";
		return ($db->pegaUm( $sql ) > 0);
	}
}

function verificaEstadoDocumento( $docid ){
	global $db;
	$sql = "SELECT esdid FROM workflow.documento
			WHERE docid = {$docid}";
	$estado = $db->pegaUm( $sql );
	if( $estado )
		return $estado;	 
}

function permissaoAlterar($coaid){
	global $db;
	
	if($coaid){
		$docid = evtPegarDocCompra($coaid);
		if($docid){
			$estado = verificaEstadoDocumento( $docid );
		}
	}

	if ($db->testa_superuser() || possuiPerfil(EVENTO_PERFIL_ORDENADOR_DESPESA_COMPRAS) || possuiPerfil(EVENTO_PERFIL_DEMANDANTE_COMPRAS) ) {
		if($estado == EM_ANALISE_SAA_WF){
			return false;
		}
		return true;
	} else {
		if( $estado == AGUARDANDO_APROVACAO_CORD_WF ){
			return false;
		}
	}
	
	return false;
}

function verificaSessao($boVerificaCopid = false){
	
	if(!$_SESSION['unidade']){
		echo "<script>
				alert('Favor selecionar uma Unidade');
				window.location.href = 'contratos.php?modulo=principal/inicioCompraUnidade&acao=A';
			  </script>";
		die;
	}
	if($boVerificaCopid){
		if(!$_SESSION['copid'] && $_SESSION['unidade']){
			echo "<script>
					alert('Favor selecionar um Processo.');
					window.location.href = 'contratos.php?modulo=principal/listaProcessoUnidade&acao=A';			
				  </script>";
			die;
		}		
	}
	return false;
}

function verificaResponsabilidade($ctrid,$permissao,$excecao=array()){
	global $db;
	$superPerfis = array(PERFIL_ADMINISTRADOR,PERFIL_SUPER_USUARIO,PERFIL_CONSULTA_GERAL);
	$arIntersec = array_intersect($superPerfis, $permissao);
// 	dbg($arIntersec,d);
	if( empty($arIntersec)) {
		//dbg($arDif,d);
		$sql = "SELECT hspid FROM contratos.ctcontrato WHERE ctrid=".$ctrid." AND ctrstatus='A'";
		$hspidcontrato = $db->pegaUm($sql);
		$sql2 = "SELECT hspid FROM contratos.usuarioresponsabilidade WHERE usucpf='".$_SESSION["usucpf"]."' AND rpustatus='A'";
		$hspidusuario = $db->pegaUm($sql2);

		if($hspidcontrato!=$hspidusuario){
	 		echo '<script>
	 				alert(\'Seu usuário não possui permissão de acesso à este contrato.\nSelecione um Contrato.\');
	 				window.location.href = \'contratos.php?modulo=principal/inicioContrato&acao=A\';
	 			  </script>';
	 		exit;		
		}else{
			$sql = "SELECT count(gscid) as getor, count(fscid) as fiscal FROM entidade.entidade e LEFT JOIN contratos.gestorcontrato g ON e.entid=g.entid
				LEFT JOIN contratos.fiscalcontrato f ON f.entid=e.entid WHERE e.entnumcpfcnpj='".$_SESSION["usucpf"]."'";
			$gestorFiscal = $db->pegaLinha($sql);
			//dbg($gestorFiscal,d);
			if($gestorFiscal['gestor']==0 && $gestorFiscal['fiscal']==0 ){
				if(in_array(PERFIL_GESTOR_UNIDADE, $permissao))
					return true;
				else{
					if($arInt = array_intersect($permissao,$excecao))
						return true;
					else 
						return false;
				}
			}else 
				return true;
		}
	}else{ 
		return true;
	}	
}


function verificaPermissaoTelaUsuario(){
	global $db, $url;
	
	$desabilitado   		  = false;
	$somenteLeitura 		  = 'S';
	$arPflcodResponsabilidade = array();
	
	$arPerfil 	   = arrayPerfil();
	$arSuperPerfil = array( PERFIL_ADMINISTRADOR,
							PERFIL_SUPER_USUARIO);
	
	$arIntersect = array_intersect($arPerfil, $arSuperPerfil);
	
	if ( empty($arIntersect) && !empty($_SESSION['ctrid']) ){
		// 	if($_SESSION['ctrid']){
		$sql = "SELECT
					hspid
				FROM
					contratos.ctcontrato
				WHERE
					ctrstatus='A' AND
					ctrid= {$_SESSION['ctrid']}";
				$hspidContrato = $db->pegaUm( $sql );
			
				$sql = "SELECT
							hspid
						FROM
							contratos.usuarioresponsabilidade
						WHERE
							rpustatus = 'A' AND
							hspid IS NOT NULL AND
							usucpf = '" . $_SESSION["usucpf"] . "'";

		$arHspidResponsabilidade = $db->carregarColuna( $sql );

		if ( !in_array($hspidContrato, $arHspidResponsabilidade) && !in_array(PERFIL_CONSULTA_GERAL, $arPerfil) ){
			die('<script>
					alert(\'Seu usuário não possui permissão de acesso ao contrato!\nSelecione um Contrato.\');
	 				window.location.href = \'contratos.php?modulo=principal/inicioContrato&acao=A\';
	 			 </script>');
		}

		// Esse perfil não tem responsabilidade por UNIDADE, porém é obstrusivo, por isso está aqui e não junto aos "superperfis"
		if ( in_array(PERFIL_CONSULTA_GERAL, $arPerfil) ){
			$desabilitado   = true;
			$somenteLeitura = 'N';
		}
		
		// Pega os perfis que tem responsablidade na unidade gestora do contrato
		$sql = "SELECT
					pflcod
				FROM
					contratos.usuarioresponsabilidade
				WHERE
					rpustatus = 'A' AND
					hspid  = " . $hspidContrato . " AND
					usucpf = '" . $_SESSION["usucpf"] . "'";
				$arPflcodResponsabilidade = $db->carregarColuna( $sql );
	
		/**************
		// O ESCALONAMENTO DOS IFs DEVE PARTIR DO MAIS OBSTRUSIVO PARA O MENOS OBSTRUSIVO
		***************/

		if ( in_array(PERFIL_CONSULTA_UNIDADE, $arPflcodResponsabilidade) ){
			// Vê os contratos da unidade e NÃO edita nada
			$desabilitado   = true;
			$somenteLeitura = 'N';
		}

		// Validações de acesso do perfil FISCAL DO CONTRATO
		if ( in_array(PERFIL_FISCAL_CONTRATO, $arPflcodResponsabilidade) ){
			$sql = "SELECT
						COUNT(g.gscid) AS getor,
						COUNT(f.fscid) AS fiscal
					FROM
						entidade.entidade e
					LEFT JOIN contratos.gestorcontrato g ON g.entid = e.entid AND
															g.gscstatus = 'A' AND
															g.ctrid = '" . $_SESSION['ctrid'] . "'
					LEFT JOIN contratos.fiscalcontrato f ON f.entid = e.entid AND
															f.fscstatus = 'A' AND
															f.ctrid = '" . $_SESSION['ctrid'] . "'
					WHERE
						e.entnumcpfcnpj='".$_SESSION["usucpf"]."'";
	
			$gestorFiscal = $db->pegaLinha($sql);
	
			if( $gestorFiscal['gestor'] == 0 && $gestorFiscal['fiscal'] == 0 ){
				$desabilitado   = true;
				$somenteLeitura = 'N';
			}else{
				$desabilitado   = false;
				$somenteLeitura = 'S';
			}
		}

		if ( array_intersect($arPflcodResponsabilidade, array(PERFIL_TRIAGEM, PERFIL_GESTOR_FINANCEIRO_UNIDADE)) ){
			// 	Ver os contratos da unidade e só edita a aba de "Execução Finânceira" e em "Dados do contrato" pode alterar somente a "aliquota de Retenção"
			if ( strpos($url, 'principal/execucaoFinanceiraContratos') !== false || strpos($url, 'principal/addNotaContratos') !== false ){
				$desabilitado   = false;
				$somenteLeitura = 'S';
			}else{
				$desabilitado   = true;
				$somenteLeitura = 'N';
			}
		}

		if ( array_intersect($arPflcodResponsabilidade, array(PERFIL_EQUIPE_TECNICA_UNIDADE, PERFIL_GESTOR_UNIDADE, PFLCOD_ADMINISTRADOR_UNIDADE)) ){
			// vê os contratos da unidade e edita tudo
			$desabilitado   = false;
			$somenteLeitura = 'S';
		}

// 	}else{
// 		$desabilitado   = false;
// 		$somenteleitura = 's';
// 	}
	}else{
		// aqui liberação para perfis que tem acesso total
		$desabilitado   = false;
		$somenteLeitura = 'S';
	}
	
	return array('desabilitado' => $desabilitado, 'leitura' => $somenteLeitura, 'pflcodComResponsabilidade' => $arPflcodResponsabilidade);
}

function temPerfilEmpresa(){
	global $db;
	$perfis = arrayPerfil();
	if(in_array(PERFIL_EMPRESA, $perfis)){
		return true;
	}
	return false;
}

function verificaSessaoPagina(){
	
	if(!$_SESSION['ctrid']){
			echo "<script>
					alert('Sessão expirou. Favor selecionar o contrato novamente.');
					window.location.href = 'contratos.php?modulo=principal/inicioContrato&acao=A';			
				  </script>";
			die;
		}	
}

function enviarEmailPorEstadoWorkflow(){
 
	global $db, $docid;	
	
	$sql = "select 
				evetitulo, 
				to_char(evedatainicio::date,'DD/MM/YYYY') as evedatainicio, 
				to_char(evedatafim::date,'DD/MM/YYYY') as evedatafim				 
			from contratos.evento 
			where docid = {$_REQUEST['docid']}";
			
	$rs = $db->pegaLinha($sql);
	
	// Demandate
	$sql = "select usuemail from seguranca.usuario where usucpf = '{$_SESSION['usucpf']}'";
	$emailDemandate = $db->pegaUm($sql);

	$arrEmails 				= array();
	
	// Segue os arrays de emails 
	$arTodos 				= array($_SESSION['email_sistema']);
	
	$arEmpresa 				= array($_SESSION['email_sistema']);
	
	// Todos
	$esdidTodos 			= array(
								EM_ANALISE_COMITE_WF,
								APROVADO_PELO_COMITE_WF,
								PROJETO_FINALIZADO_WF
								);
	
	// Empresa
	$esdidEmpresa 			= array(
								APROVADO_PELO_COMITE_WF,
								ADEQUACAO_PROJETO_WF,
								PROJETO_FINALIZADO_WF,
								EMISSAO_EMPENHO_WF,
								PAGAMENTO_NF_WF								 
								);
	// Orçamento SPO					
	$esdidSPO    			= array(
								ELABORACAO_CDO_WF
								);

	// SPO (Subsecretário)
	$esdidSPOSubsecretario 	= array(
								EMISSAO_CDO_WF
								);
							
	// Área Demandante
	$esdidAreaDemandante 	= array(
								EMISSAO_CDO_WF,
								EMISSAO_EMPENHO_WF,
								ATESTO_NF_WF
								);
								
	// SAA
	$esdidSAA			 	= array(
								INSTRUCAO_PROCESSO_WF,
								EMISSAO_EMPENHO_WF,
								ATESTO_NF_WF,
								PAGAMENTO_NF_WF
								);
								
	// Comitê de Eventos
	$esdidComiteEventos	 	= array(								
								ATESTO_NF_WF,
								PAGAMENTO_NF_WF
								);
									
	// Adiciona Todos
	if(in_array($_REQUEST['esdid'], $esdidTodos))
		array_push($arrEmails, $arTodos);
								
	// Adiciona Empresas				
	if(in_array($_REQUEST['esdid'], $esdidEmpresa))				
		array_push($arrEmails, $arEmpresa);	
	
	// Adiciona SPO
	if(in_array($_REQUEST['esdid'], $esdidSPO))		
		array_push($arrEmails, $_SESSION['email_sistema']);

	// Adiciona SPO subsecretário
	if(in_array($_REQUEST['esdid'], $esdidSPOSubsecretario))		
		array_push($arrEmails, $_SESSION['email_sistema']);
	
	// Adiciona Área Demandate 
	if(in_array($_REQUEST['esdid'], $esdidAreaDemandante))		
		array_push($arrEmails, $_SESSION['email_sistema']);

	// Adiciona SAA
	if(in_array($_REQUEST['esdid'], $esdidSAA))		
		array_push($arrEmails, $_SESSION['email_sistema']);
	
	// Adiciona Comite de Eventos
	if(in_array($_REQUEST['esdid'], $esdidComiteEventos))		
		array_push($arrEmails, $_SESSION['email_sistema']);
	
	$remetente = array('nome'=>REMETENTE_WORKFLOW_NOME, 'email'=>REMETENTE_WORKFLOW_EMAIL);
					  
	$assunto   = "[SIMEC] Módulo de Eventos";
	
	// retirar quando validar essa funcao
	if($_REQUEST['esdid'] == EM_ANALISE_COMITE_WF){
		
		if(!verificaVoltaEstadoWorflow()){
			
			if(verificaPrazoConformeComite()){
				return true;
			} else {
				return false;
			}
		}
					
	} else {
		
		return true;
	}
	
	if($_REQUEST['esdid'] == EM_ANALISE_COMITE_WF){	
		$mailBody = '
		<b>Prezados Senhores,</b><br><br> 
		Informamos que o evento nº '.$_SESSION['eveid'].' - "'.$rs['evetitulo'].'" a ser realizado no período de '.$rs['evedatainicio'].' à '.$rs['evedatafim'].',<br>
		foi cadastrado no SIMEC e enviado para análise e aprovação do comitê de eventos.<br><br>
		<a href="http://simec.mec.gov.br/" title="Acessar o ". SIGLA_SISTEMA>Clique Aqui para acessar o SIMEC.</a><br><br> 
		Atenciosamente,<br><br>
		SIMEC - Módulo de Eventos<br>
		';
		
		if(!verificaVoltaEstadoWorflow()){
			
			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		}
		
	} else if($_REQUEST['esdid'] == APROVADO_PELO_COMITE_WF){
		
		$mailBody = '
		<b>Prezados Senhores,</b><br><br>
		Informamos que o evento nº '.$_SESSION['eveid'].' - "'.$rs['evetitulo'].'" a ser realizado no período de '.$rs['evedatainicio'].' à '.$rs['evedatafim'].',<br>
		foi cadastrado no SIMEC foi aprovado pelo comitê de eventos.<br><br>
		<a href="http://simec.mec.gov.br/" title="Acessar o ". SIGLA_SISTEMA>Clique Aqui para acessar o SIMEC.</a><br><br> 
		Atenciosamente,<br><br>
		SIMEC - Módulo de Eventos<br>
		';
		
		if(verificaVoltaEstadoWorflow()){
			
			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		}
		
	} else if($_REQUEST['esdid'] == ADEQUACAO_PROJETO_WF){
		
		$mailBody = '
		<b>Prezados Senhores,</b><br><br> 
		Informamos que o evento nº '.$_SESSION['eveid'].' - "'.$rs['evetitulo'].'" a ser realizado no período de '.$rs['evedatainicio'].' à '.$rs['evedatafim'].',<br>
		foi lançado de forma preliminar.<br><br>
		<a href="http://simec.mec.gov.br/" title="Acessar o ". SIGLA_SISTEMA>Clique Aqui para acessar o SIMEC.</a><br><br> 
		Atenciosamente,<br><br>
		SIMEC - Módulo de Eventos<br>
		';

		if(verificaVoltaEstadoWorflow()){
			
			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		}
		
	} else if($_REQUEST['esdid'] == PROJETO_FINALIZADO_WF){
		
		$mailBody = '
		<b>Prezados Senhores,</b><br><br> 
		Informamos que o evento nº '.$_SESSION['eveid'].' - "'.$rs['evetitulo'].'" a ser realizado no período de '.$rs['evedatainicio'].' à '.$rs['evedatafim'].',<br>
		foi  lançado de forma definitiva.<br><br>
		<a href="http://simec.mec.gov.br/" title="Acessar o ". SIGLA_SISTEMA>Clique Aqui para acessar o SIMEC.</a><br><br> 
		Atenciosamente,<br><br>
		SIMEC - Módulo de Eventos<br>
		';

		if(verificaVoltaEstadoWorflow()){
			
			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		}
		
	} else if($_REQUEST['esdid'] == ELABORACAO_CDO_WF){
		
		$mailBody = '
		<b>Prezados Senhores,</b><br><br> 
		Informamos que o evento nº '.$_SESSION['eveid'].' - "'.$rs['evetitulo'].'" a ser realizado no período de '.$rs['evedatainicio'].' à '.$rs['evedatafim'].',<br>
		foi cadastrado no SIMEC sendo necessária a preparação da  emissão da CDO.<br><br>
		<a href="http://simec.mec.gov.br/" title="Acessar o ". SIGLA_SISTEMA>Clique Aqui para acessar o SIMEC.</a><br><br> 
		Atenciosamente,<br><br>
		SIMEC - Módulo de Eventos<br>
		';

		if(verificaVoltaEstadoWorflow()){
			
			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		}
		
	} else if($_REQUEST['esdid'] == EMISSAO_CDO_WF){
		
		$mailBody = '
		<b>Prezados Senhores,</b><br><br> 
		Informamos que o evento nº '.$_SESSION['eveid'].' - "'.$rs['evetitulo'].'" a ser realizado no período de '.$rs['evedatainicio'].' à '.$rs['evedatafim'].',<br>
		está apto para emissão da CDO.<br><br>		
		<a href="http://simec.mec.gov.br/" title="Acessar o ". SIGLA_SISTEMA>Clique Aqui para acessar o SIMEC.</a><br><br> 
		Atenciosamente,<br><br>
		SIMEC - Módulo de Eventos<br>
		';	
		
		if(verificaVoltaEstadoWorflow()){
			
			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		}
		
	} else if($_REQUEST['esdid'] == INSTRUCAO_PROCESSO_WF){
		
		$mailBody = '
		<b>Prezados Senhores,</b><br><br> 
		Informamos que o evento nº '.$_SESSION['eveid'].' - "'.$rs['evetitulo'].'" a ser realizado no período de '.$rs['evedatainicio'].' à '.$rs['evedatafim'].',<br>
		está apto para impressão dos documentos relativos ao evento<br><br>
		<a href="http://simec.mec.gov.br/" title="Acessar o ". SIGLA_SISTEMA>Clique Aqui para acessar o SIMEC.</a><br><br> 
		Atenciosamente,<br><br>
		SIMEC - Módulo de Eventos<br>
		';

		if(verificaVoltaEstadoWorflow()){
			
			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		}
		
	} else if($_REQUEST['esdid'] == EMISSAO_EMPENHO_WF){
		
		$mailBody = '
		<b>Prezados Senhores,</b><br><br>  
		Informamos que o evento nº '.$_SESSION['eveid'].' - "'.$rs['evetitulo'].'" a ser realizado no período de '.$rs['evedatainicio'].' à '.$rs['evedatafim'].',<br>
		está apto para emissão da Nota de Empenho e Ordem de Serviço.<br><br>
		<a href="http://simec.mec.gov.br/" title="Acessar o ". SIGLA_SISTEMA>Clique Aqui para acessar o SIMEC.</a><br><br> 
		Atenciosamente,<br><br>
		SIMEC - Módulo de Eventos<br>
		';	
		
		if(verificaVoltaEstadoWorflow()){
			
			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		}
		
	} else if($_REQUEST['esdid'] == ATESTO_NF_WF){
		
		$mailBody = '
		<b>Prezados Senhores,</b><br><br> 
		Informamos que a Nota Fiscal relativa ao evento nº '.$_SESSION['eveid'].' - "'.$rs['evetitulo'].'" realizado no período de '.$rs['evedatainicio'].' à '.$rs['evedatafim'].',<br>
		foi realizado e a correspondente NF foi emitida para pagamento.<br><br>
		<a href="http://simec.mec.gov.br/" title="Acessar o ". SIGLA_SISTEMA>Clique Aqui para acessar o SIMEC.</a><br><br> 
		Atenciosamente,<br><br>
		SIMEC - Módulo de Eventos<br>
		';	
		
		if(verificaVoltaEstadoWorflow()){
			
			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		}
		
	} else if($_REQUEST['esdid'] == PAGAMENTO_NF_WF){
		
		$mailBody = '
		<b>Prezados Senhores,</b><br><br> 
		Informamos que o pagamento da Nota fiscal relativa ao evento nº '.$_SESSION['eveid'].' - "'.$rs['evetitulo'].'" realizado no período de '.$rs['evedatainicio'].' à '.$rs['evedatafim'].',<br>
		foi realizado.<br><br>
		<a href="http://simec.mec.gov.br/" title="Acessar o ". SIGLA_SISTEMA>Clique Aqui para acessar o SIMEC.</a><br><br> 
		Atenciosamente,<br><br>
		SIMEC - Módulo de Eventos<br>
		';	
		
		if(verificaVoltaEstadoWorflow()){
			
			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		}
		
	}

	return true;
}

function verificaPrazoConformeComite(){
	
	global $db;
	
	$sql = "SELECT 
				ev.evetitulo,  
				ev.ungcod,
				ev.tpeid,			 
				ev.evedatainicio,		 
				ev.evedatafim,			 
				ev.eveemail, 		 
				ev.evenumeropi, 		 
				ev.evenumeroprocesso,   
				ev.evecustoprevisto, 	 
				ev.evepublicoestimado,  
				ev.evequantidadedias,
				ev.muncod, 				 
				ev.estuf, 				 
				ev.sevid,
				ev.eveqtdpassagemaerea,
				u.ungdsc,
				us.usunome,
				ev.docid,
				ev.eveurgente,
				ev.rcoid,
				ev.endid,
				to_char(ev.evedatainclusao::date,'DD/MM/YYYY') AS evedatainclusao,
				ev.eveanopi		 
			FROM 
				contratos.evento AS ev
			LEFT JOIN contratos.tipoevento AS te 
				ON te.tpeid = ev.tpeid
			LEFT JOIN 
				public.unidadegestora AS u ON ev.ungcod = u.ungcod
			LEFT JOIN seguranca.usuario AS us ON us.usucpf = ev.usucpf 
			WHERE
				ev.eveid = '".$_SESSION['eveid']."'";
	
	$rsDadosEvento = $db->carregar( $sql );	
	
	if($_REQUEST['esdid'] == EM_ANALISE_COMITE_WF){
	
		$data = new Data();
		$retorno = $data->timeStampDeUmaData( date("d/m/Y") );
		$retorno1 = $data->timeStampDeUmaData($rsDadosEvento[0]['evedatainicio']);
		$segundos_diferenca = $retorno - $retorno1;
		$dias_diferenca = $segundos_diferenca / (60 * 60 * 24);
		$dias_diferenca = abs($dias_diferenca);
		$dias_diferenca = floor($dias_diferenca);
	 
		if( $rsDadosEvento[0]['evepublicoestimado'] <= 50 ){
			$diferenca_permitida = 30;
		}
		elseif( ( $rsDadosEvento[0]['evepublicoestimado'] > 50 ) AND ( $rsDadosEvento[0]['evepublicoestimado'] <= 250 ) ){
			$diferenca_permitida = 45;
		}
		elseif( ( $rsDadosEvento[0]['evepublicoestimado'] > 250 ) AND ( $rsDadosEvento[0]['evepublicoestimado'] <= 500 ) ){
			$diferenca_permitida = 60;
		}		
		elseif( $rsDadosEvento[0]['evepublicoestimado'] > 500 )	{
			$diferenca_permitida = 90;
		}  
	 
		if(  $dias_diferenca >= $diferenca_permitida ){
			$evedataurgente = "f";
			$eveurgente     = "f";
		} 
		else { 
			$evedataurgente = "t";
			$eveurgente     = "t";
		} 	
		if( $rsDadosEvento[0]['adreverendo'] ){
			$adreferendum = "t";
			$eveurgente   = "t";
		}else{
			$adreferendum = "f";
			$eveurgente   = "f";
		}
		
		#verificando se valerá a regra de AD-REFERENDUM para o perfil.
		$perfis = arrayPerfil();
		$boAdreferendum = true;
		if( !in_array( PERFIL_SUPER_USUARIO, $perfis) && !in_array(PERFIL_SAA, $perfis)){
			if( $adreferendum != $evedataurgente){
				$boAdreferendum = false;
			}	
		} 
		
		$arEvents =  explode( "_", verificaEventos( $rsDadosEvento[0]['ungcod'] ) );
		$numEventosSemNota = $arEvents[0];
		$numEventosSemAval = $arEvents[1];
			
	 	if( $numEventosSemNota < MAX_EVENTOS_SEM_NOTA || $numEventosSemAval < MAX_EVENTOS_SEM_NOTA ){
	 			 			
			if( !$boAdreferendum ){
				alert("A data de início do evento está fora do prazo, de acordo com as regras do comitê. Entre em contato com a SAA.");					
				echo "<script>window.close();</script>";				
				return false;				
			} else {							
				return true;
			}
			
		} else {
			
			echo '<script type="text/javascript"> 
		 			alert( "Relatórios Técnicos em Aberto, ou Avaliação de eventos não preenchida." ); 
			 		window.location.href = "?modulo=inicio&acao=C" 
			 	  </script>';
			
			return false;  
		}
	} else {
		
		return true;
	}
	
}

function eventoEnviaAnaliseComite()
{
	global $db;
	
	if(!$_SESSION['eveid'])
		return "Sessão expirou. Favor entrar novamente no sistema de evetos.";
	
	$sql = "SELECT count(axpid) FROM contratos.anexoevento where axestatus='A' and eveid = ".$_SESSION['eveid'];
	$verificaAnexo = $db->pegaUm($sql);
	if($verificaAnexo == 0)
		return "É necessário anexar um arquivo.";

	$sql = "SELECT count(ievid) FROM contratos.itemevento where ievstatus='A' and eveid = ".$_SESSION['eveid'];
	$verificaItemInfra = $db->pegaUm($sql);
	if($verificaItemInfra == 0)
		return "É necessário cadastrar pelo menos um item na aba Infraestrutura.";
		
	/*
	$sql = "SELECT evedatainicio, evepublicoestimado, evenumeropi, eveanopi FROM contratos.evento where eveid = ".$_SESSION['eveid'];
	$rsDadosEvento = $db->pegaLinha($sql);
	
	if(empty($rsDadosEvento['evenumeropi']) || empty($rsDadosEvento['eveanopi'])){
		return "É necessário informar o Ano do PI e o Nº PI na aba Estrutura Orçamentária.";
	}	
	*/
	
	return 'OK';	
		
}

function verificaDiasEnviarAnalise()
{
	global $db;
	
	$sql = "SELECT evedatainicio, evepublicoestimado, evenumeropi, eveanopi FROM contratos.evento where eveid = ".$_SESSION['eveid'];
	$rsDadosEvento = $db->pegaLinha($sql);
	
	if($rsDadosEvento){
		
		$data = new Data();
		$retorno = $data->timeStampDeUmaData( date("d/m/Y") );
		$retorno1 = $data->timeStampDeUmaData($rsDadosEvento['evedatainicio']);
		$segundos_diferenca = $retorno - $retorno1;
		$dias_diferenca = $segundos_diferenca / (60 * 60 * 24);
		$dias_diferenca = abs($dias_diferenca);
		$dias_diferenca = floor($dias_diferenca);
	 
		if( $rsDadosEvento['evepublicoestimado'] <= 50 ){
			$diferenca_permitida = 30;
		}
		elseif( ( $rsDadosEvento['evepublicoestimado'] > 50 ) AND ( $rsDadosEvento['evepublicoestimado'] <= 250 ) ){
			$diferenca_permitida = 45;
		}
		elseif( ( $rsDadosEvento['evepublicoestimado'] > 250 ) AND ( $rsDadosEvento['evepublicoestimado'] <= 500 ) ){
			$diferenca_permitida = 60;
		}		
		elseif( $rsDadosEvento['evepublicoestimado'] > 500 )	{
			$diferenca_permitida = 90;
		}  
		
		if(  $diferenca_permitida >= $dias_diferenca ){
			return false; // "A data de início do evento está fora do prazo, de acordo com as regras do comitê. A data de início deverá acontecer após $diferenca_permitida dias. Entre em contato com a SAA.";
		} 
	}
	return true;
}

function dPagamentoPermissaoEdicao(){
	global $db;
	
	if($_SESSION['eveid']){
		$sql = "
			Select dpaid From contratos.documentopagamento where eveid = '".$_SESSION['eveid']."'
		";
		$permite = $db->pegaLinha($sql);
		return $permite['dpaid'];
	}
}

function dPagamentoWorkFlow(){
	global $db;

	if($_SESSION['eveid']){
		$sql = "
		Select dpaid From contratos.documentopagamento where eveid = '".$_SESSION['eveid']."'
		";
		$permite = $db->pegaLinha($sql);
	}
	
	if( $permite['dpaid'] != ''){
		return true;
	}else{
		return false;
	}
}

function eventoPermissaoEdicao()
{
	global $db;

	if($_SESSION['eveid']){
			
		$sql = "select d.esdid
		from contratos.evento e
		inner join workflow.documento as d on d.docid = e.docid
		where e.eveid = ".$_SESSION['eveid'];
		$esdid = $db->pegaUm($sql);

		if($esdid != EM_CADASTRAMENTO_WF){
			return '<script>
			var obj = document.getElementsByTagName("input");
			var total = document.getElementsByTagName("input").length;
				
			for(i=0; i<total; i++){
			obj[i].disabled = true;
		}
			
		obj = document.getElementsByTagName("select");
		total = document.getElementsByTagName("select").length;
			
		for(i=0; i<total; i++){
		obj[i].disabled = true;
		}
		</script>';
		}
		else{
			return '';
		}

	}
	else{
		return '';
	}
}

function verificaVoltaEstadoWorflow(){
	
	global $docid, $db;
	
	$sql = "select * from workflow.historicodocumento where docid = {$docid} and aedid = {$_REQUEST['aedid']} order by hstid desc";
	$boVoltou = $db->pegaLinha($sql);
	
	if($boVoltou){		
		return true;
	} else {		
		return false;
	}
	
}

function verificarAnexoNF(){
	
	global $db;
	
	$sql = "select axpid from contratos.anexoevento aev
			inner join public.arquivo arq on aev.arqid = arq.arqid
			where arq.arqstatus = 'A' 
			and  aev.eveid = '{$_SESSION['eveid']}'";
			
	$anexo = $db->carregar($sql);
	
	if($anexo){
		
		return true;
			
	} else {
		
		return false;
	}
	 
}

function mostraAbaDocPagamento($eveid){
	global $db;
	
	$docid = evtCriarDoc($_SESSION['eveid']);
	$esdid = verificaEstadoDocumento($docid);
	
	if($esdid == AGUARDANDO_PAGAMENTO_EVENTO_WF || $esdid == PROJETO_FINALIZADO_WF){
		return true;
	}else{
		return false;
	}
	/*
	if($eveid){
		$sql = "select to_char(evedatafim, 'DD/MM/YYYY') as evedatafim  from contratos.evento where eveid = $eveid";
		$evedatafim = $db->pegaUm($sql);
		
		$dataAtual = date('d/m/Y'); 
		$obData = new Data();
		
		return true; //retirar esta linha antes de entrar pra produção.
		$retorno = $obData->diferencaEntreDatas(  $dataAtual, $evedatafim, 'maiorDataBolean', null, 'dd/mm/yyyy');
		
		if($retorno && possuiPerfil(EVENTO_PERFIL_SAA_FINANCEIRO)){
			return true;
		}
	}
	
	return false;
	*/	
}

function mostraAbaDocOS($eveid)
{
	global $db;
	
	if($eveid){
		$sql = "select oseid  from contratos.ordemservico where eveid = $eveid";
		$oseid = $db->pegaUm($sql);
		
		if($oseid){
			return true;
		}
	}
	
	return false;	
}

function carregaDados( $id ){
	global $db;
	
	$sql = "
            SELECT  precodpregao, 
                    predescpregao, 
                    TO_CHAR( preiniciovig, 'dd/mm/YYYY') as preiniciovig,
                    TO_CHAR( prefimvig, 'dd/mm/YYYY') as prefimvig,
                    trim(to_char(prevalorcontratado,'999g999g999g999d99')) as prevalorcontratado, 
                    trim(to_char(prevalorempenhado,'999g999g999g999d99')) as prevalorempenhado,
                    prenumprocesso,
                    precnpj,
                    prerazaosocial,
                    prenumcontrato
            FROM contratos.pregaoevento epe
            WHERE epe.preid = ".$id;
	$arrResp = $db->pegaLinha( $sql );
	
	echo $arrResp['precodpregao'] .'|'.$arrResp['predescpregao'] .'|'.$arrResp['preiniciovig'] .'|'.$arrResp['prefimvig'] .'|'.$arrResp['prevalorcontratado'] .'|'.$arrResp['prevalorempenhado'] .'|'.$arrResp['prenumprocesso'] .'|'.formatar_cpf_cnpj($arrResp['precnpj']). '|'.$arrResp['prerazaosocial']. '|' .$arrResp['prenumcontrato'] ;
}
/*

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
*/

function carregaDadosPregao( $id ){
	global $db;
	
	$sql = "SELECT 
					precodpregao, 
       				prenumprocesso,
       				prevalorcontratado
				FROM 
				    contratos.pregaoevento epe
 	 	 		WHERE
 	 	 			epe.preid = ".$id;
	return $arrResp = $db->pegaLinha( $sql );
}

function carregaDadosUnidade( $id ){
	global $db;
	
	$sql = "SELECT '<center>
					 <img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"alterar('||ur.ureid||')\" \" border=0 alt=\"Ir\" title=\"Alterar\">  ' || 
					 '<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluir('||ur.ureid||');\" border=0 alt=\"Ir\" title=\"Excluir\">
					</center>' as acao,
					'<a href=\"javascript:void(0);\" onclick=\"exibirExtrato(' || ur.ureid || ')\">' || ug.ungdsc || '</a>' as ungdsc,
					ureordenador,
					ureordenadorsub,
					ur.urevalorrecurso AS limite,
					coalesce(ur.urevalorsaldo,0) AS saldo
				FROM 
 	 	 			contratos.unidaderecurso ur 
 	 	 		INNER JOIN 
					 public.unidadegestora ug ON ug.ungcod = ur.ungcod
 	 	 		WHERE
 	 	 			preid = ".$id."
 	 	 		order by ug.ungdsc";
	return $sql;
}

function carregaDadosUnidadePorUreid( $id ){
	global $db;
	
	$sql = "SELECT 
					ug.ungcod,
					ur.ureordenador,
					ur.ureordenadorsub,
					ur.urevalorrecurso AS limite,
					ur.urevalorsaldo AS saldo
				FROM 
 	 	 			contratos.unidaderecurso ur 
 	 	 		INNER JOIN 
					 public.unidadegestora ug ON ug.ungcod = ur.ungcod
 	 	 		WHERE
 	 	 			ureid = ".$id;
	return $db->pegaLinha( $sql );
}

function cabecalhoContrato( $id ){
	
}

function retornaUngcods($perfils=Array()){
	
	global $db;
	
	$ungcods = Array();
	
	if( possuiPerfil( $perfils ) ){
		$sql = "SELECT DISTINCT
					uni.ungcod
				FROM 
					contratos.usuarioresponsabilidade ur 
				INNER JOIN public.unidadegestora uni ON
					uni.ungcod = ur.ungcod AND
					uni.ungcod = '%s' AND
					uni.ungstatus = 'A'
				INNER JOIN seguranca.perfil pfl ON
					pfl.pflcod = ur.pflcod AND
					pfl.pflcod = '" . $pflcod . "'
				where
					ur.rpustatus = 'A' and
					ur.usucpf <> '" . $_SESSION['usucpf'] . "'";
		$ungcods = $db->pegaColuna($sql);
	}
	return $ungcods;
}

//Workflow Solicitação de Ajuda de Custo (Diárias)
function wf_condicao_solicitacao(){
 	
//	global $db;
//	
//	if($db->testa_superuser()){
//		return true;
//	}else{
//		$ungcods = retornaUngcods(Array(EVENTO_PERFIL_SOLICITADOR));
//		
//		if(is_array($ungcods)){
//			$sql = "SELECT
//						ungcod
//					FROM
//						contratos.solicitacaodiaria
//					WHERE
//						ungcod in ('".implode('\',\'',$ungcods)."')
//						AND solid = ".$_SESSION['evento']['solid'];
//			$sol = $db->carregar($sql);
//			if(is_array($sol)){
//				return true;
//			}else{
//				return false;
//			}
//		}else{
			return true;
//		}
//	}
}

function wf_condicao_validacao(){
 	
	global $db;
	
//	if($db->testa_superuser()){
//		return true;
//	}else{
//		$ungcods = retornaUngcods(Array(EVENTO_PERFIL_VALIDADOR));
//		
//		if(is_array($ungcods)){
//			$sql = "SELECT
//						ungcod
//					FROM
//						contratos.solicitacaodiaria
//					WHERE
//						ungcod in ('".implode('\',\'',$ungcods)."')
//						AND solid = ".$_SESSION['evento']['solid'];
//			$sol = $db->carregar($sql);
//			if(is_array($sol)){
//				return true;
//			}else{
//				return false;
//			}
//		}else{
			return true;
//		}
//	}	
}

function wf_condicao_retorno_validacao(){
 	
	global $db;
	
//	if($db->testa_superuser()){
//		return true;
//	}else{
//		$ungcods = retornaUngcods(Array(EVENTO_PERFIL_VALIDADOR));
//		
//		if(is_array($ungcods)){
//			$sql = "SELECT
//						ungcod
//					FROM
//						contratos.solicitacaodiaria
//					WHERE
//						ungcod in ('".implode('\',\'',$ungcods)."')
//						AND solid = ".$_SESSION['evento']['solid'];
//			$sol = $db->carregar($sql);
//			if(is_array($sol)){
//				return true;
//			}else{
//				return false;
//			}
//		}else{
			return true;
//		}
//	}
}

function wf_condicao_autorizacao(){
 	
	global $db;
	
//	if($db->testa_superuser()){
//		return true;
//	}else{
//		$ungcods = retornaUngcods(Array(EVENTO_PERFIL_AGENTE_FINANCEIRO));
//		
//		if(is_array($ungcods)){
//			$sql = "SELECT
//						ungcod
//					FROM
//						contratos.solicitacaodiaria
//					WHERE
//						ungcod in ('".implode('\',\'',$ungcods)."')
//						AND solid = ".$_SESSION['evento']['solid'];
//			$sol = $db->carregar($sql);
//			if(is_array($sol)){
//				return true;
//			}else{
//				return false;
//			}
//		}else{
			return true;
//		}
//	}
}

function wf_condicao_retorno_autorizacao(){
 	
	global $db;
	
//	if($db->testa_superuser()){
//		return true;
//	}else{
//		$ungcods = retornaUngcods(Array(EVENTO_PERFIL_AGENTE_FINANCEIRO));
//		
//		if(is_array($ungcods)){
//			$sql = "SELECT
//						ungcod
//					FROM
//						contratos.solicitacaodiaria
//					WHERE
//						ungcod in ('".implode('\',\'',$ungcods)."')
//						AND solid = ".$_SESSION['evento']['solid'];
//			$sol = $db->carregar($sql);
//			if(is_array($sol)){
//				return true;
//			}else{
//				return false;
//			}
//		}else{
			return true;
//		}
//	}
}


function wf_condicao_pagamento(){
 	
	global $db;
	
	$sql = "SELECT
				solordembancaria
			FROM
				contratos.solicitacaodiaria
			WHERE
				solordembancaria not like 'NULL' AND
				solordembancaria is not null AND
				solid = ".$_SESSION['evento']['solid'];
	$sol = $db->carregar($sql);
	if(is_array($sol)){
		return true;
	}else{
		return "Solicitação sem ordem bancária informada.";
	}
//	
//	if($db->testa_superuser()){
//		return true;
//	}else{
//		$ungcods = retornaUngcods(Array(EVENTO_PERFIL_AGENTE_FINANCEIRO));
//		
//		if(is_array($ungcods)){
//			$sql = "SELECT
//						ungcod
//					FROM
//						contratos.solicitacaodiaria
//					WHERE
//						ungcod in ('".implode('\',\'',$ungcods)."')
//						AND solid = ".$_SESSION['evento']['solid'];
//			$sol = $db->carregar($sql);
//			if(is_array($sol)){
//				return true;
//			}else{
//				return false;
//			}
//		}else{
			return true;
//		}
//	}
}

function excluirSolicitacao( $solid ){
	global $db;
	
	$sql = "SELECT
				solcomplemento
			FROM
				contratos.solicitacaodiaria
			WHERE
				solid = ".$solid;
	$solidOriginal = $db->pegaUm($sql);
	
	if($solidOriginal){
		$sql = "UPDATE contratos.solicitacaodiaria SET
					solstatus = 'A'
				WHERE
					solid = ".$solidOriginal;
		$db->executar($sql);
	}
	
	$sql = "UPDATE contratos.solicitacaodiaria SET
				solstatus = 'I'
			WHERE
				solid = ".$solid;
	
	$db->executar($sql);
	$db->commit();
}

function enviarEmailSolicitacao( $dados ){

	global $db;

	$remetente = array('nome'=>REMETENTE_WORKFLOW_NOME, 'email'=>REMETENTE_WORKFLOW_EMAIL);
 	
	if($_SESSION['ambiente'] == 'Ambiente de Desenvolvimento'){

		$dados['to'] = $db->pegaUm('SELECT usuemail FROM seguranca.usuario WHERE usucpf = \''.$_SESSION['usucpf'].'\'');
		enviar_email($remetente, $dados['to'], $dados['assunto'], $dados['mailBody'] );
	} else {

		enviar_email($remetente, $dados['to'], $dados['assunto'], $dados['mailBody']  );
	}
}

function wf_pos_retorna_solicitacao( $solid = NULL ){
	
	global $db;
	
	if($solid){
		$sql = "SELECT 
					sol.solnome || replace(to_char(sol.usucpf::bigint, '000:000:000-00'), ':', '.') as nome,
					to_char(sol.soldatainclusao,'DD/MM/YYYY') as inclusao,
					usu.usuemail as to
				FROM 
					contratos.solicitacaodiaria sol
				INNER JOIN seguranca.usuario usu ON usu.usucpf = sol.solusucpf
				WHERE
					sol.solid = ".$solid;
		
		$dados = $db->pegaLinha($sql);
	}
	
	$dados['assunto']   = "[SIMEC] Solicitação retornada para 'Em solicitação' - Sistema de Solicitação de Diárias - Módulo Administrativo";					
	$dados['mailBody']  = '
	Prezados Senhores, <br> 
	<br>
	Informamos que a solicitação do Sr(a) "'.$dados['nome'].'" iniciada no dia '.$dados['inclusao'].',<br>
	retornou de \'Em Verificação\' para \'Em Solicitação\'.<br>
	<br>
	<br>	
	<a href="http://simec.mec.gov.br">Clique Aqui para acessar o SIMEC.</a>
	<br>	
	<br>		
	Atenciosamente,<br>
	<br>
	<br>	
	SIMEC<br>
	';
	
	if($dados['to']!=''){
		enviarEmailSolicitacao( $dados );
		return true;
	}else{
		return true;
	}
}

function wf_pos_retorna_verificacao( $solid = NULL ){

	global $db;
	
	if($solid){
		$sql = "SELECT 
					sol.solnome || replace(to_char(sol.usucpf::bigint, '000:000:000-00'), ':', '.') as nome,
					to_char(sol.soldatainclusao,'DD/MM/YYYY') as inclusao,
					usu.usuemail as to
				FROM 
					contratos.solicitacaodiaria sol
				INNER JOIN seguranca.usuario usu ON usu.usucpf = sol.solusucpf
				WHERE
					sol.solid = ".$solid;
		
		$dados = $db->pegaLinha($sql);
	}
	
	$dados['assunto']   = "[SIMEC] Solicitação retornada para 'Em verificação' - Sistema de Solicitação de Diárias - Módulo Administrativo";					
	$dados['mailBody']  = '
	Prezados Senhores, <br> 
	<br>
	Informamos que a solicitação do Sr(a) "'.$dados['nome'].'" iniciada no dia '.$dados['inclusao'].',<br>
	retornou de \'Em Autorização\' para \'Em Verificação\'.<br>
	<br>
	<br>	
	<a href="http://simec.mec.gov.br">Clique Aqui para acessar o SIMEC.</a>
	<br>	
	<br>		
	Atenciosamente,<br>
	<br>
	<br>	
	SIMEC<br>
	';
	
	if($dados['to']!=''){
		enviarEmailSolicitacao( $dados );
		return true;
	}else{
		return true;
	}
}

function wf_pos_retorna_autorizacao( $solid = NULL ){

	global $db;
	
	if($solid){
		$sql = "SELECT 
					sol.solnome || replace(to_char(sol.usucpf::bigint, '000:000:000-00'), ':', '.') as nome,
					to_char(sol.soldatainclusao,'DD/MM/YYYY') as inclusao,
					usu.usuemail as to
				FROM 
					contratos.solicitacaodiaria sol
				INNER JOIN seguranca.usuario usu ON usu.usucpf = sol.solusucpf
				WHERE
					sol.solid = ".$solid;
		
		$dados = $db->pegaLinha($sql);
	}
	
	$dados['assunto']   = "[SIMEC] Solicitação retornada para 'Em autorização' - Sistema de Solicitação de Diárias - Módulo Administrativo";					
	$dados['mailBody']  = '
	Prezados Senhores, <br> 
	<br>
	Informamos que a solicitação do Sr(a) "'.$dados['nome'].'" iniciada no dia '.$dados['inclusao'].',<br>
	retornou de \'Em Pagamento\' para \'Em Autorização\'.<br>
	<br>
	<br>	
	<a href="http://simec.mec.gov.br">Clique Aqui para acessar o SIMEC.</a>
	<br>	
	<br>		
	Atenciosamente,<br>
	<br>
	<br>	
	SIMEC<br>
	';
	
	if($dados['to']!=''){
		enviarEmailSolicitacao( $dados );
		return true;
	}else{
		return true;
	}
}


function wf_condicao_comite() {
	global $db;
	$existe_ar = $db->pegaUm("SELECT axpid FROM contratos.anexoevento WHERE eveid='".$_SESSION['eveid']."' AND axestatus='A'");
	$existe_iv = $db->pegaUm("SELECT ievid FROM contratos.itemevento  WHERE eveid='".$_SESSION['eveid']."' AND  ievstatus='A'");
	
	if($existe_iv && $existe_ar) return true;
	else return false;
}

function cancelarEvento()
{
	global $db;
	
	if($_SESSION['eveid']){
	
		$sql = "update contratos.evento set evestatus = 'I' where eveid = {$_SESSION['eveid']}";
		$db->executar($sql);
		
		if($db->commit()){
			return true;
		}	
	}
	return false;
}

function wf_verificaPrazoEnvioSecretaria()
{
	global $db;
	
	/*
	if(!eventoEnviaAnaliseComite()){
		return false;
	}
	*/
	
	/*
	if($_SESSION['eveid']){
		
		$sql = "select
					case when evepublicoestimado <= 50 and DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) < 30 then 'false'
					     when evepublicoestimado BETWEEN 51 AND 250 and DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) < 45 then 'false'
					     when evepublicoestimado BETWEEN 251 AND 500 and DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) < 60 then 'false'
					     when evepublicoestimado > 500 and DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) < 90 then 'false'
					else 'true' end as prazo
				from 
					contratos.evento 
				where 
					eveid = ".$_SESSION['eveid'];
		
		$rs = $db->pegaLinha($sql);
		
		if($rs['prazo'] == 'false'){
			return true;
		}
	}
	
	return 'O Evento deve estar fora do prazo para enviar para a Secretaria Executiva de Eventos';
	*/
	
	$msg = eventoEnviaAnaliseComite();
	
	if($msg != 'OK'){
		return $msg;
	}
	
	if($_SESSION['eveid']){
		
		//verifica prazo
		$sql = "select
					case when evepublicoestimado <= 50 and DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) < 30 then 'false'
					     when evepublicoestimado BETWEEN 51 AND 250 and DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) < 45 then 'false'
					     when evepublicoestimado BETWEEN 251 AND 500 and DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) < 60 then 'false'
					     when evepublicoestimado > 500 and DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) < 90 then 'false'
					else 'true' end as prazo,
					ungcod,
					evecustoprevisto
				from 
					contratos.evento 
				where 
					eveid = ".$_SESSION['eveid'];
		
		$rs = $db->pegaLinha($sql);
		
		if($rs['prazo'] == 'true'){
			return 'O Evento deve estar fora do prazo para enviar para a Secretaria Executiva de Eventos';
		}
		
		//verifica saldo no contrato
		$preid = $db->pegaUm("select preid from contratos.pregaoevento where prestatus = 'A'");
		
		if($preid && $rs['ungcod']){
			$urevalorsaldo = $db->pegaUm("select urevalorsaldo from contratos.unidaderecurso where preid = $preid and ungcod = '".$rs['ungcod']."'");

			if($rs['evecustoprevisto'] > $urevalorsaldo){
				return 'Saldo insuficiente para esta Unidade Gestora!';
			}
		}
		else{
			return 'Não existe contrato para esta Unidade Gestora!';
		}
		
		
		return true;
		
	}
	else{
		return 'Sessão expirou. Entre novamente no sistema.';
	}
	
	
}

function wf_verificaPrazoEnvioComite()
{
	global $db;
	
	/*
	if(!eventoEnviaAnaliseComite()){
		return false;
	}
	*/
	
	$msg = eventoEnviaAnaliseComite();
	
	if($msg != 'OK'){
		return $msg;
	}
	
	
	if($_SESSION['eveid']){
		
		//verifica prazo
		$sql = "select
					case when evepublicoestimado <= 50 and DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) < 30 then 'false'
					     when evepublicoestimado BETWEEN 51 AND 250 and DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) < 45 then 'false'
					     when evepublicoestimado BETWEEN 251 AND 500 and DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) < 60 then 'false'
					     when evepublicoestimado > 500 and DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) < 90 then 'false'
					else 'true' end as prazo,
					DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) as dias,
					ungcod,
					evecustoprevisto
				from 
					contratos.evento 
				where 
					eveid = ".$_SESSION['eveid'];
		
		$rs = $db->pegaLinha($sql);
		
		if(!$rs['prazo'] || $rs['prazo'] == 'false'){
			return 'O Evento deve estar dentro do prazo para enviar para análise do comitê';
		}
		
		//verifica saldo no contrato
		$preid = $db->pegaUm("select preid from contratos.pregaoevento where prestatus = 'A'");
		
		if($preid && $rs['ungcod']){
			$urevalorsaldo = $db->pegaUm("select urevalorsaldo from contratos.unidaderecurso where preid = $preid and ungcod = '".$rs['ungcod']."'");
			
			if($rs['evecustoprevisto'] > $urevalorsaldo){
				return 'Saldo insuficiente para esta Unidade Gestora!';
			}
		}
		else{
			return 'Não existe contrato para esta Unidade Gestora!';
		}
		
		
		return true;
		
	}
	else{
		return 'Sessão expirou. Entre novamente no sistema.';
	}
	
	
}

function wf_aprovaAdReferendum()
{
	global $db;
	
	if($_SESSION['eveid']){
		
		$sql = "update contratos.evento set eveurgente = 't' where eveid = ".$_SESSION['eveid'];
		$db->executar($sql);
		if($db->commit()){
			return true;
		}
	}
	return false;
}


function mascaraglobal2($value, $mask) {
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

#CADASTRO DE EMPENHO - FUNÇÃO PARA BUSCAR OS DADOS DO EMPENHO - DATA DE 09/04/2013
function carregarEmpenho($dados){
    global $db;
    
    extract($dados);
    
    if($emuid){
        $sql = "SELECT * FROM contratos.empenho_unidade WHERE emuid = ".$emuid;
        $rs = $db->pegaLinha($sql);
        echo $rs['emuid']."||".$rs['ungcod']."||".$rs['empnumero']."||".$rs['empdescricao']."||".$rs['empnumeropi']."||".$rs['empano'];
        exit;
    }
}

#CADASTRO DE EMPENHO - FUNÇÃO PARA CADASTRAR / ATUALIZAR EMPENHOS - DATA DE 09/04/2013
function salvarEmpenho($dados){
    global $db;

    extract($dados);
  
    if( $ungcod != '' && $emuid == '' ){
        $sql = "
            INSERT INTO contratos.empenho_unidade(
                    ungcod, 
                    empnumero, 
                    empdescricao, 
                    empnumeropi, 
                    empano, 
                    empstatus
                )VALUES (
                    '".$ungcod."', 
                    '".$empnumero."', 
                    '".addslashes( $empdescricao )."', 
                    '".$empnumeropi."',
                    '".$empano."', 'A'
                );
        ";
        $msg = "Dados Gravados com sucesso.";
    }elseif( $ungcod != '' && $emuid > 0 ){
        $sql = "
            UPDATE contratos.empenho_unidade
                SET empnumero       = '".$empnumero."', 
                    empdescricao    = '".addslashes( $empdescricao )."', 
                    empnumeropi     = '".$empnumeropi."', 
                    empano          = '".$empano."'
                WHERE emuid = ".$emuid." and ungcod = '".$ungcod."';
        ";
        $msg = "Dados Atualizados com sucesso.";
    }

    if( $db->executar($sql) ){
        $db->commit();
        die("
            <script>
                alert('".$msg."');
                window.location='contratos.php?modulo=principal/CadEmpenhos&acao=A&ungcod=".$ungcod."&form_pesquisa=empenho';
            </script>"
        );        
    }
}

#CADASTRO DE EMPENHO - FUNÇÃO PARA ATUALIZAR STATUS PARA "I" OS DADOS DO EMPENHOS - DATA DE 09/04/2013
function exclirEmpenho($dados){
    global $db;

    extract($dados);
  
    if($ungcod != '' && $emuid > 0){
        $sql = "
            UPDATE contratos.empenho_unidade
                SET empstatus       = 'I'
                WHERE emuid = ".$emuid." and ungcod = '".$ungcod."';
        ";
    }
             
    if( $db->executar($sql) ){
        $db->commit();
        die("
            <script>
                alert('Dados excluido com sucesso.');
                window.location='contratos.php?modulo=principal/CadEmpenhos&acao=A&ungcod=".$ungcod."&form_pesquisa=empenho';
            </script>"
        );        
    }
}

#ORDER DE SERVIÇO - FUNÇÃO PARA VALIDAÇÃO DA DATA, A DATA DA GERAÇÃO DE OS. NÃO PODE SER MAIOR QUE A DATA ATUAL- DATA DE 09/04/2013
function validaDataEvento(){
    global $db;
    
    $sql = "
        SELECT  ev.eveid,			 
                ev.evedatainicio,		 
                ev.evedatafim
        FROM contratos.evento AS ev
        WHERE ev.eveid = {$_SESSION['eveid']}
    ";
    $dados = $db->pegaLinha($sql);
    
    $dataHoje = strtotime("now");
    $dataInicio = strtotime($dados['evedatainicio']);

    if( $dataHoje < $dataInicio ){
        $msg = "ok";
    }else{
        $msg = "erro";
    }
    echo $msg;
}


#DOCUMENTO DE PAGAMENTO - VERIFICA SE FOI GERADO O DOCUMETO DE PAGAMENTO PARA HABILITAR O WORKFLOW
function validaDocPagamento(){
    global $db;
    
    $sql = "
        SELECT dpaid FROM contratos.documentopagamento WHERE dpavalor IS NOT NULL AND dpanumero IS NOT NULL AND eveid = {$_SESSION['eveid']};
    ";
    $dpaid = $db->pegaUm($sql);
    
    if( $dpaid > 0 ){
        return true;
    }else{
        return false;
    }
}

function condicaoGeraqrOS() {
	global $db;
	
	$eveid = $_SESSION['eveid'];
	$datahoje = date('Y-m-d');
	
	if(!$eveid) return false;
	
	$sql = "SELECT
				evedatainicio,
				evenumeroprocesso
			FROM
				contratos.evento
			WHERE
				eveid = {$eveid}";
	$dados = $db->pegaLinha($sql);
	
	if($dados['evedatainicio'] =='' || $dados['evenumeroprocesso']==''){
		return false;
	} elseif( strtotime($dados['evedatainicio']) < strtotime($datahoje)){
		return false;
	} else {
		return true;
	}
}


//O valor da soma das ordem bancárias deve ser igual ao da Nota Fiscal
function verificaOrdemBancaria($ftcid)
{
// 	global $db;
// 	$sql = "select ftcvalor from contratos.faturacontrato where ftcid = $ftcid";
// 	$ftcvalor = $db->pegaUm($sql);
// 	$sql = "select sum(obfvalor) from contratos.ordembancariafatura where ftcid = $ftcid";
// 	$obfvalor = $db->pegaUm($sql);
// 	if($ftcvalor == $obfvalor){
// 		return true;	
// 	}else{
// 		return "O valor da soma das OBs (".number_format($obfvalor,2,',','.').") deve ser igual ao valor do Pagamento (".number_format($ftcvalor,2,',','.').").";
// 	}
	
	return true;
}

function FiltraEmpenhoPorCNPJ($cnpj = null,$ctrid = null, $ungcod = null)
{
	global $db, $numempenho, $desabilitado, $servidor_bd, $porta_bd, $nome_bd, $usuario_db, $senha_bd;
		
	$servidor_bd = '';
	$porta_bd = '5432';
	$nome_bd = '';
	$usuario_db = '';
	$senha_bd = '';
	
	$db2 = new cls_banco(); 
	
	$servidor_bd_siafi = '';
	$porta_bd_siafi = '5432';
	$nome_bd_siafi = '';
	$usuario_db_siafi = '';
	$senha_bd_siafi = '';
	
	$conexao['servidor_bd'] = $servidor_bd_siafi;
	$conexao['porta_bd']    = $porta_bd_siafi;
	$conexao['nome_bd']     = $nome_bd_siafi;
	$conexao['usuario_db']  = $usuario_db_siafi;
	$conexao['senha_bd']    = $senha_bd_siafi;

	$cnpj = !$cnpj ? $_REQUEST['cnpj'] : $cnpj;
	$cnpj = str_replace(array(".","-","/"),array("","",""),$cnpj);
	
	?>
	<td class="SubTituloDireita" align="right">Empenho: </td>
	<td align="left">
	<?php 
		if($cnpj) $and = " and trim(codigo_favorecido) = '$cnpj' ";
		if($ungcod) $and .= " and trim(codigo_ug_operador) = '$ungcod' ";
		
		$sql = "(select 
                	substr(numero_ne,12,12) as codigo, 
                	substr(numero_ne,12,12) || ' - ' || observacao as descricao
				from 
					siafi2012.ne ne 
				where 1=1
				$and
				order by 
					1)
		union all
		(select 
                	substr(numero_ne,12,12) as codigo, 
                	substr(numero_ne,12,12) || ' - ' || observacao as descricao
				from 
					siafi2013.ne ne 
				where 1=1
				$and
				order by 
					1)
		union all
		(select 
                	substr(numero_ne,12,12) as codigo, 
                	substr(numero_ne,12,12) || ' - ' || observacao as descricao
				from 
					siafi2014.ne ne 
				where 1=1
				$and
				order by 
					1)";
		//dbg($sql,1);
		if($cnpj && $ungcod) $rsBdSiafi = $db2->carregar($sql);
		
		$rsBdSiafi = $rsBdSiafi ? $rsBdSiafi : array();
		
		if($ctrid) $rsContratosVinculados = $db->carregarColuna("select trim(numempenho) as numempenho from contratos.empenhovinculocontrato where ctrid not in ({$ctrid})");
		
		if($rsBdSiafi){
			foreach($rsBdSiafi as $k => $rs) {

				$rsBdSiafiNormalizado[$rs['codigo']] = $rs['descricao'];
				
				if(in_array(trim($rs['codigo']), $rsContratosVinculados)){
					$rsBdSiafi[$k]['disable'] = 'true';
					$rsBdSiafi[$k]['mesage'] = 'Este empenho está vinculado em outro contrato!';
				}
			}
		}
		
		if($ctrid){

			$sqlCarregado = "select
								numempenho as codigo,
								numempenho as descricao
							from
								contratos.empenhovinculocontrato
							where
								ctrid = $ctrid
							order by
								numempenho";
			
			$numempenho = $db->carregar($sqlCarregado);
			
			if($numempenho[0]) {
				foreach($numempenho as $i => $emp) {
					$numempenho[$i]['descricao'] = $rsBdSiafiNormalizado[$emp['codigo']];
				}
			}
		}
		$arrPesquisa[] = array("codigo"=>"numero_ne","descricao"=>"Empenho");
		$arrPesquisa[] = array("codigo"=>"observacao","descricao"=>"Descrição");
	    combo_popup( "numempenho", $rsBdSiafi, "Empenho(s)", '400x400', null,array(), '', (($desabilitado)?'N':'S'), false,false, 10, 400 , null, null, $conexao, $arrPesquisa);
        ?>
        </td> 
        <?php 
}

function FiltraEmpenhoPorCNPJ2($cnpj = null,$ctrid = null, $hspid = null, $ano = null, $desabilitado = false)
{
	global $db;
	
	$cnpj = !$cnpj ? $_REQUEST['cnpj'] : $cnpj;
	$cnpj = str_replace(array(".","-","/"),array("","",""),$cnpj);
	
	$hspid = trim($hspid);
	
	?>
	<td class="SubTituloDireita" alvalign="top">Empenho(s): </td>
	<td align="left">
	<?php 
		if($cnpj) 	$and = " and trim(co_favorecido) = '$cnpj' ";
		
		if($hspid){
			$sql = "SELECT
						ungcod
					FROM
						contratos.hospitalug
					WHERE
						hspid = {$hspid}";
			$arUngcod = $db->carregarColuna($sql);
			$arUngcod = (array) $arUngcod;
			
			$and .= " AND TRIM(ungcod) IN ('" . implode("', '", $arUngcod) . "') ";
		}
			
		if($ano) 	$and .= " and trim(ano) >= '$ano' ";
		if($ctrid) 	$whereEmpenho = " WHERE ctrid != {$ctrid}";
			
		$sql = "SELECT epsid as codigo,  
					   nu_empenho || ' - ' || observacao as descricao
				FROM contratos.empenho_siafi 
				WHERE 
					epsid NOT IN ( SELECT epsid FROM contratos.empenhovinculocontrato {$whereEmpenho} ) 
					$and
				ORDER BY 
					1";
		
// 		$rsBdSiafi = $db->carregar($sql);
// 		$rsBdSiafi = $rsBdSiafi ? $rsBdSiafi : array();
		
		if($ctrid){
			$sqlCarregado = "SELECT
								e.epsid as codigo,  
					   			s.nu_empenho || ' - ' || observacao as descricao
							FROM
								contratos.empenhovinculocontrato e
							INNER JOIN contratos.empenho_siafi s ON s.epsid = e.epsid
							WHERE
								e.ctrid = $ctrid AND co_favorecido='$cnpj'
							ORDER BY
								1";
// 			dbg($sqlCarregado, d);
			$numempenho = $db->carregar($sqlCarregado);
			$numempenho = $numempenho ? $numempenho : array();
		}
		
		$arrPesquisa[] = array("codigo"=>"nu_empenho","descricao"=>"Empenho");
		$arrPesquisa[] = array("codigo"=>"observacao","descricao"=>"Descrição");
	    combo_popup( "numempenho", $sql, "Empenho(s)", '400x500', null,array(), '', (($desabilitado)?'N':'S'), false,false, 10, 450 , null, null, $conexao, $arrPesquisa, $numempenho);
        ?>
        </td> 
        <?php 
}

function popUpEmpenho_old()
{
	global $db,$servidor_bd,$porta_bd,$nome_bd,$usuario_db,$senha_bd;
	
	$servidor_bd = '';
	$porta_bd = '5432';
	$nome_bd = '';
	$usuario_db = '';
	$senha_bd = '';
		
	$db2 = new cls_banco();
	
	extract($_REQUEST);
	
	monta_titulo("Dados do Empenho","&nbsp;");
	
	//Remover para passar os parâmetros.
	$empenho = $_REQUEST['numempenho']; //"2014NE800009";
	$cnpj = $_REQUEST['cnpj']; //"06064175000149";
	
/*	
	$sql = "(SELECT
conta_corrente AS empenho, it_no_credor, codigo_favorecido, naturezadet AS cod_agrupador2,
--'' AS dsc_agrupador1,naturezadet_desc AS dsc_agrupador2,
observacao, ptres, fonte_recurso as fonteempenho, data_transacao as dataempenho,
sum(valor1) AS coluna1--,sum(valor2) AS coluna2,sum(valor3) AS coluna3--,sum(valor4) AS coluna4
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
WHERE sld.ungcod in ('155007') AND sld.sldcontacontabil in ( '292410101', '292410402', '292410403', '292410405', '292410402','292410403', '292410403', '292410405' )
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
WHERE sld.ungcod in ('155007') AND sld.sldcontacontabil in ('292410101', '292410402', '292410403', '292410405', '292410402','292410403', '292410403', '292410405')
) as foo
inner join ( select numero_ne, codigo_favorecido, it_no_credor, observacao, ptres, fonte_recurso, data_transacao
from siafi2012.ne ne 
inner join dw.credor c ON c.it_co_credor = ne.codigo_favorecido 
where codigo_ug_operador = '155007' 
) ne ON
      numero_ne = '155007'||(select orgcodgestao from dw.uguo where ungcod = '155007' order by ugoid desc limit 1)||substr(foo.conta_corrente, 1,12)                           
WHERE
trim(substr(conta_corrente,1,12)) = '$empenho' and 
codigo_favorecido = '$cnpj' and naturezadet != '' and ( valor1 <> 0 OR valor2 <> 0 OR valor3 <> 0 OR valor4 <> 0 )
GROUP BY
conta_corrente,codigo_favorecido, it_no_credor, naturezadet,
conta_corrente_desc,naturezadet_desc, observacao, ptres, fonte_recurso, data_transacao
ORDER BY
conta_corrente,naturezadet,
conta_corrente_desc,naturezadet_desc
)
union all
(
SELECT
conta_corrente AS empenho, it_no_credor, codigo_favorecido, naturezadet AS cod_agrupador2,
--'' AS dsc_agrupador1,naturezadet_desc AS dsc_agrupador2,
observacao, ptres, fonte_recurso as fonteempenho, data_transacao as dataempenho,
sum(valor1) AS coluna1--,sum(valor2) AS coluna2,sum(valor3) AS coluna3--,sum(valor4) AS coluna4
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
dw.saldo2013 sld 
LEFT JOIN ( select distinct ctecod, gndcod, mapcod, edpcod, sbecod, ndpdsc from dw.naturezadespesa where sbecod <> '00' ) as ndp ON cast(ndp.ctecod AS text) = sld.ctecod AND cast(ndp.gndcod AS text) = sld.gndcod AND cast(ndp.mapcod AS text) = sld.mapcod AND cast(ndp.edpcod AS text) = sld.edpcod AND cast(ndp.sbecod AS text) = sld.sbecod
WHERE sld.ungcod in ('155007') AND sld.sldcontacontabil in ( '292410101', '292410402', '292410403', '292410405', '292410402','292410403', '292410403', '292410405' )
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
dw.saldo2013 sld
LEFT JOIN ( select distinct ctecod, gndcod, mapcod, edpcod, sbecod, ndpdsc from dw.naturezadespesa where sbecod <> '00' ) as ndp ON cast(ndp.ctecod AS text) = sld.ctecod AND cast(ndp.gndcod AS text) = sld.gndcod AND cast(ndp.mapcod AS text) = sld.mapcod AND cast(ndp.edpcod AS text) = sld.edpcod AND cast(ndp.sbecod AS text) = sld.sbecod
WHERE sld.ungcod in ('155007') AND sld.sldcontacontabil in ('292410101', '292410402', '292410403', '292410405', '292410402','292410403', '292410403', '292410405')
) as foo
inner join ( select numero_ne, codigo_favorecido, it_no_credor, observacao, ptres, fonte_recurso, data_transacao
from siafi2013.ne ne 
inner join dw.credor c ON c.it_co_credor = ne.codigo_favorecido 
where codigo_ug_operador = '155007' 
) ne ON
      numero_ne = '155007'||(select orgcodgestao from dw.uguo where ungcod = '155007' order by ugoid desc limit 1)||substr(foo.conta_corrente, 1,12)                           
WHERE
trim(substr(conta_corrente,1,12)) = '$empenho' and 
codigo_favorecido = '$cnpj' and naturezadet != '' and ( valor1 <> 0 OR valor2 <> 0 OR valor3 <> 0 OR valor4 <> 0 )
GROUP BY
conta_corrente,codigo_favorecido, it_no_credor, naturezadet,
conta_corrente_desc,naturezadet_desc, observacao, ptres, fonte_recurso, data_transacao
ORDER BY
conta_corrente,naturezadet,
conta_corrente_desc,naturezadet_desc
)
union all
(
SELECT
conta_corrente AS empenho, it_no_credor, codigo_favorecido, naturezadet AS cod_agrupador2,
--'' AS dsc_agrupador1,naturezadet_desc AS dsc_agrupador2,
observacao, ptres, fonte_recurso as fonteempenho, data_transacao as dataempenho,
sum(valor1) AS coluna1--,sum(valor2) AS coluna2,sum(valor3) AS coluna3--,sum(valor4) AS coluna4
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
dw.saldo2014 sld 
LEFT JOIN ( select distinct ctecod, gndcod, mapcod, edpcod, sbecod, ndpdsc from dw.naturezadespesa where sbecod <> '00' ) as ndp ON cast(ndp.ctecod AS text) = sld.ctecod AND cast(ndp.gndcod AS text) = sld.gndcod AND cast(ndp.mapcod AS text) = sld.mapcod AND cast(ndp.edpcod AS text) = sld.edpcod AND cast(ndp.sbecod AS text) = sld.sbecod
WHERE sld.ungcod in ('155007') AND sld.sldcontacontabil in ( '292410101', '292410402', '292410403', '292410405', '292410402','292410403', '292410403', '292410405' )
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
dw.saldo2014 sld
LEFT JOIN ( select distinct ctecod, gndcod, mapcod, edpcod, sbecod, ndpdsc from dw.naturezadespesa where sbecod <> '00' ) as ndp ON cast(ndp.ctecod AS text) = sld.ctecod AND cast(ndp.gndcod AS text) = sld.gndcod AND cast(ndp.mapcod AS text) = sld.mapcod AND cast(ndp.edpcod AS text) = sld.edpcod AND cast(ndp.sbecod AS text) = sld.sbecod
WHERE sld.ungcod in ('155007') AND sld.sldcontacontabil in ('292410101', '292410402', '292410403', '292410405', '292410402','292410403', '292410403', '292410405')
) as foo
inner join ( select numero_ne, codigo_favorecido, it_no_credor, observacao, ptres, fonte_recurso, data_transacao
from siafi2014.ne ne 
inner join dw.credor c ON c.it_co_credor = ne.codigo_favorecido 
where codigo_ug_operador = '155007' 
) ne ON
      numero_ne = '155007'||(select orgcodgestao from dw.uguo where ungcod = '155007' order by ugoid desc limit 1)||substr(foo.conta_corrente, 1,12)                           
WHERE
trim(substr(conta_corrente,1,12)) = '$empenho' and 
codigo_favorecido = '$cnpj' and naturezadet != '' and ( valor1 <> 0 OR valor2 <> 0 OR valor3 <> 0 OR valor4 <> 0 )
GROUP BY
conta_corrente,codigo_favorecido, it_no_credor, naturezadet,
conta_corrente_desc,naturezadet_desc, observacao, ptres, fonte_recurso, data_transacao
ORDER BY
conta_corrente,naturezadet,
conta_corrente_desc,naturezadet_desc
)";
*/

$sql = "select substr(numero_ne,12,12) as codigo, it_no_credor, codigo_favorecido, natureza_despesa, observacao, ptres, fonte_recurso, data_transacao, valor_transacao
from siafi2012.ne ne
inner join siafi2013.credor c ON c.it_co_credor = ne.codigo_favorecido
where substr(numero_ne,12,12) = '$empenho'
and trim(codigo_favorecido) = '$cnpj'
UNION ALL
select substr(numero_ne,12,12) as codigo, it_no_credor, codigo_favorecido, natureza_despesa, observacao, ptres, fonte_recurso, data_transacao, valor_transacao
from siafi2013.ne ne
inner join siafi2013.credor c ON c.it_co_credor = ne.codigo_favorecido
where substr(numero_ne,12,12) = '$empenho'
and trim(codigo_favorecido) = '$cnpj'
UNION ALL
select substr(numero_ne,12,12) as codigo, it_no_credor, codigo_favorecido, natureza_despesa, observacao, ptres, fonte_recurso, data_transacao, valor_transacao
from siafi2014.ne ne
inner join siafi2013.credor c ON c.it_co_credor = ne.codigo_favorecido
where substr(numero_ne,12,12) = '$empenho'
and trim(codigo_favorecido) = '$cnpj'
order by data_transacao desc limit 1";

	//dbg($sql,1);
	$arrDados = $db2->pegaLinha($sql);
	$arrDados = $arrDados ? $arrDados : array();
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<?php $arrLabel = array("Empenho","Nome da Contratante","CNPJ da Contratante","Natureza","Observação","PTRES","Fonte de Empenho","Data de Empenho","Valor Empenhado") ?>
	<?php $n=0;foreach($arrDados as $chave => $valor): ?>
		<?php 
		if($chave == "cod_agrupador2" && $valor){
			$ndpdsc = $db->pegaUm("select ndpdsc from naturezadespesa  where ndpcod = '".str_replace('.','',$valor)."'");
			$valor = $ndpdsc ? $valor.' - '.$ndpdsc : $valor;
		}
		if($chave == "ptres" && $valor){
			$sql = "SELECT DISTINCT
						ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as descricao
					FROM monitora.ptres p
					JOIN public.unidadegestora u
						ON u.unicod = p.unicod
					where ptres = '{$valor}'";
			$ptres = $db->pegaUm($sql);
			$valor = $ptres ? $ptres : $valor;
		}
		if($chave == "codigo_favorecido" && $valor){
			$valor = mascara_global($valor, "##.###.###/####-##");
		}
		if($chave == "dataempenho" && $valor){
			$valor = formata_data($valor);
		}
		if($chave == "coluna1" || $chave == "coluna2" || $chave == "coluna3"){
			$valor = number_format($valor, 2, ',', '.');
		}
		?>
		<tr>
	        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%"><?php echo $arrLabel[$n] ? $arrLabel[$n] : $chave?></td>
	        <td><?php echo $valor ?></td>
		</tr>
	<?php $n++;endforeach;?>
	</table>
	<?php
}


function popUpEmpenho()
{
	global $db;

	extract($_REQUEST);

	monta_titulo("Dados do Empenho","&nbsp;");

	//Remover para passar os parâmetros.
	$empenho 	= $_REQUEST['numempenho']; //"2014NE800009";
	$cnpj 		= $_REQUEST['cnpj']; //"06064175000149";
	$ctrid 		= $_SESSION['ctrid'] ? $_SESSION['ctrid'] : 0;
	
	if ( $empenho && $cnpj ){
		$sql = "SELECT nu_empenho as codigo, 
					   h.hspdsc, 
					   h.hspcnpj, 
					   natureza as natureza_despesa, 
				       observacao, 
				       ptres, 
				       fonte as fonte_recurso, 
				       dataempenho as dataempenho,
					   no_favorecido,				        
					   co_favorecido,
				       valor as coluna1
				FROM 
					contratos.empenho_siafi s
				INNER JOIN contratos.empenhovinculocontrato e on e.epsid = s.epsid
				INNER JOIN contratos.ctcontrato c ON c.ctrid = e.ctrid
				INNER JOIN contratos.hospital h ON h.hspid = c.hspid
				WHERE 
					nu_empenho = '$empenho' AND 
					TRIM(co_favorecido) = '$cnpj' AND 
					e.ctrid = ".$ctrid."
				ORDER BY s.dataempenho";
		
	}elseif ( $_GET['ctrid'] && $_GET['entidcontratada'] ){

		$ctrid 				= $_GET['ctrid'];
		$entidcontratada 	= $_GET['entidcontratada'];
	
		$sql = "SELECT
					nu_empenho AS codigo,
					h.hspdsc,
					h.hspcnpj,
					natureza AS natureza_despesa,
					observacao,
					ptres,
					fonte AS fonte_recurso,
					dataempenho AS dataempenho,
					no_favorecido,
					co_favorecido,
					valor AS coluna1
				FROM contratos.empenho_siafi s
				INNER JOIN contratos.empenhovinculocontrato e ON e.epsid = s.epsid
				INNER JOIN contratos.ctcontrato c ON c.ctrid = e.ctrid
				INNER JOIN contratos.hospital h ON h.hspid = c.hspid
				WHERE
					e.ctrid = {$ctrid} AND
					c.entidcontratada = {$entidcontratada}
					ORDER BY s.dataempenho";
		
	}elseif ( $ctrid  ){
		$sql = "SELECT 
					nu_empenho AS codigo,
            		h.hspdsc, 
					h.hspcnpj, 
					natureza AS natureza_despesa,
					observacao,
					ptres,
					fonte AS fonte_recurso,
					dataempenho AS dataempenho,
            		no_favorecido,
					co_favorecido,
					valor AS coluna1
				FROM contratos.empenho_siafi s
				INNER JOIN contratos.empenhovinculocontrato e ON e.epsid = s.epsid
				INNER JOIN contratos.ctcontrato c ON c.ctrid = e.ctrid
				INNER JOIN contratos.hospital h ON h.hspid = c.hspid
				WHERE 
					e.ctrid = ".$ctrid."
				ORDER BY s.dataempenho";
	}else{
		die('<script>alert(\'Faltam parâmetros para acessar a tela.\'); window.close();</script>');
	}

// 	dbg($sql,1);
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	
	$totRegistro = count( $arrDados );
	$i=1;
	$str = '';
	
	foreach ($arrDados as $arrDados):
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<?php 
		$arrLabel = array("Empenho","Nome da Contratante","CNPJ da Contratante","Natureza","Observação",
						  "PTRES","Fonte de Empenho","Data de Empenho", "Nome do Favorecido", "CNPJ do Favorecido", "Valor Empenhado");
		$n=0;
		
		foreach($arrDados as $chave => $valor):

			if( ($chave == "dataempenho" || $chave == "co_favorecido") && $valor){
				$numero_ne			= $arrDados['codigo'];
				$ano_ne				= substr( $numero_ne, 0, 4 );
				$cod_favorecido 	= $arrDados['co_favorecido'];
				$verifica 			= verificaEmpenhoAnulacao( $numero_ne, $ctrid, $ano_ne, $cod_favorecido );
			}
			
			if( ($chave == "hspcnpj" || $chave == "co_favorecido") && $valor){
				$valor 			= mascara_global($valor, "##.###.###/####-##");
			}

			if($chave == "cod_agrupador2" && $valor){
				$ndpdsc = $db->pegaUm("select ndpdsc from naturezadespesa  where ndpcod = '".str_replace('.','',$valor)."'");
				$valor = $ndpdsc ? $valor.' - '.$ndpdsc : $valor;
			}
				
			if($chave == "ptres" && $valor){
				$sql = "SELECT DISTINCT
							ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as descricao
						FROM monitora.ptres p
						JOIN public.unidadegestora u
							ON u.unicod = p.unicod
						where ptres = '{$valor}'";
				$ptres = $db->pegaUm($sql);
				$valor = $ptres ? $ptres : $valor;
			}
		
			if($chave == "dataempenho" && $valor){
				$valor 		= formata_data($valor);
			}
			
			if($chave == "coluna1" || $chave == "coluna2" || $chave == "coluna3"){

				if( $verifica == true || $verifica == 'NULL'){
					$texto = number_format($valor, 2, ',', '.');
					$valor = "<font style='color:red;'>" . $texto. "</font>";
				} else {
					$valor = number_format($valor, 2, ',', '.');
				}
				
/* 				if( $valor <= 0 ){
					$valor = "<font style='color:red;'>- " . number_format($valor, 2, ',', '.')."</font>";
				} else {
					$valor = number_format($valor, 2, ',', '.');
				} */
			}
		?>
			<tr>
		        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%"><?php echo $arrLabel[$n] ? $arrLabel[$n] : $chave?></td>
		        <td><?php echo $valor ?></td>
			</tr>
	<?php 
			$n++;
		endforeach;
		
		
		if ( $i < $totRegistro  ){
	?>
		<tr>
			<td colspan="2">&nbsp;</td>
		</tr>
	<?php		
		}
		$i++;
	?>
	</table>
	<?php
	endforeach;
}

function popUpOB_old()
{
	global $db,$servidor_bd,$porta_bd,$nome_bd,$usuario_db,$senha_bd;
	
	$servidor_bd = '';
	$porta_bd = '5432';
	$nome_bd = '';
	$usuario_db = '';
	$senha_bd = '';
		
	$db2 = new cls_banco();
	
	$numempenho = "";
	$ob = $_GET['ob']; //"158262264192012OB800014";
	
	monta_titulo("Dados da Ordem Bancária","&nbsp;");
	$sql = "select --distinct  
                ob.numero_ob as ob, 
                c.it_co_credor, c.it_no_credor, 
                to_char(ob.data_transacao, 'DD/MM/YYYY') || ' ' || to_char(ob.hora_transacao, 'HH24:MI') as datatransacao, 
                ob.observacao as obsob, 
                u.ungcod || ' - ' || u.ungdsc as unidade, 
                substr(ob.classificacao1_01,2,1) || '.' || substr(ob.classificacao1_01,3,1) || '.' || substr(ob.classificacao1_01,4,2) 
                || '.' || substr(ob.classificacao1_01,6,2) || '.' || substr(ob.classificacao1_01,8,2) as natureza, 
                c.it_co_credor ||' - '|| c.it_no_credor as favorecido, 
                ob.valor_transacao_01 as valorob 
                from siafi2012.ob ob 
                  inner join ( select c.it_co_credor, c2.it_no_credor 
                                               from (select c.it_co_credor, max(c.it_da_transacao) as it_da_transacao 
                                                               from siafi2012.credor c 
                                                                              where length(c.it_co_credor) = 14 and c.it_co_tipo_crdor = '1' 
                                                               group by c.it_co_credor) c 
                                               inner join siafi2012.credor c2 ON c2.it_co_credor = c.it_co_credor and c2.it_da_transacao = c.it_da_transacao ) c ON c.it_co_credor = ob.codigo_favorecido 
                  inner join dw.ug u ON u.ungcod = ob.codigo_ug_operador 
                                                                                              where 
                                                                                              --( substr(ob.classificacao1_01,4,2) = '40' or substr(ob.classificacao1_02,4,2) = '40' or substr(ob.classificacao1_03,4,2) = '40' ) and
                                                                                              ob.numero_ob = '$ob' --u.orgcod = '26443' 
order by ob.data_transacao
				";
	$arrDados = $db2->pegaLinha($sql);
	$arrDados = $arrDados ? $arrDados : array();?>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<?php $arrLabel = array("Número da OB","Nome da Contratante","CNPJ da Contratante","Data da Transação","Observação","Unidade","Natureza","Favorecido","Valor da OB") ?>
	<?php $n=0;foreach($arrDados as $chave => $valor): ?>
		<?php
		if($chave == "natureza" && $valor){
			$ndpdsc = $db->pegaUm("select ndpdsc from naturezadespesa  where ndpcod = '".str_replace('.','',$valor)."'");
			$valor = $ndpdsc ? $valor.' - '.$ndpdsc : $valor;
		} 
		if($chave == "it_co_credor" && $valor){
			$valor = mascara_global($valor, "##.###.###/####-##");
		}
		if($chave == "valorob"){
			$valor = number_format($valor, 2, ',', '.');
		}
		?>
		<tr>
	        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%"><?php echo $arrLabel[$n] ? $arrLabel[$n] : $chave?></td>
	        <td><?php echo $valor ?></td>
		</tr>
	<?php $n++;endforeach;?>
	</table>
	<?php
}

function popUpOB()
{
	global $db;

	$numempenho = trim($_GET['numempenho']);
	$ob 		= trim($_GET['ob']); //"158262264192012OB800014";

	if ( $numempenho || $ob ){
		$msg = "&nbsp;";
	}else{
		$msg = 'Todas as ordens bancárias';
	}
		
	monta_titulo("Dados da Ordem Bancária", $msg);
	
	if ( $numempenho && $ob ){
		$sql = "SELECT 
					os.empenho,
					os.ob, 
					h.hspdsc, 
					h.hspcnpj, 
					to_char(os.datatransacao, 'DD/MM/YYYY') || ' ' || to_char(os.datatransacao, 'HH24:MI') as datatransacao,
					os.obsob,
					os.unidade,
					os.natureza,
					os.it_no_credor,
					os.it_co_credor,
					os.valor as valorob
				FROM 
					contratos.ctcontrato c
					JOIN contratos.hospital h ON h.hspid = c.hspid
					JOIN contratos.faturacontrato fc ON fc.ctrid = c.ctrid AND
														fc.ftcstatus = 'A'
					JOIN contratos.ordembancariafatura of ON of.ftcid = fc.ftcid
					JOIN contratos.empenhovinculocontrato ec ON ec.epsid = of.epsid AND
																fc.ctrid = ec.ctrid										 
					JOIN contratos.empenho_siafi es ON es.epsid = ec.epsid AND
													   es.nu_empenho = '{$numempenho}'
					JOIN contratos.ob_siafi os ON os.ob = of.obfnumero AND
												  os.empenho = es.nu_empenho AND
												  os.ob = '{$ob}'
				WHERE 
					c.ctrid = {$_SESSION['ctrid']}";

		$arrDados = $db->carregar($sql);

		if(!$arrDados){
			$sql = "SELECT 
						es.nu_empenho AS empenho,
						ob, 
						h.hspdsc, 
						h.hspcnpj, 
						to_char(ob.datatransacao, 'DD/MM/YYYY') || ' ' || to_char(ob.datatransacao, 'HH24:MI') as datatransacao,
						obsob,
						es.ungcod as unidade,
						ob.natureza,
						entnome AS it_no_credor,
						entnumcpfcnpj AS it_co_credor,
						ob.valor as valorob
					FROM 
					contratos.ctcontrato c
					
					JOIN contratos.hospital h ON h.hspid = c.hspid
					JOIN entidade.entidade e ON e.entid = c.entidcontratada					
					JOIN contratos.empenhovinculocontrato ec ON ec.ctrid = c.ctrid
					JOIN contratos.empenho_siafi es ON es.epsid = ec.epsid AND
													   es.co_favorecido = e.entnumcpfcnpj
					JOIN contratos.ob_siafi ob ON ob.empenho = es.nu_empenho AND
												  ob.it_co_credor = e.entnumcpfcnpj	
					WHERE 
						c.ctrid = {$_SESSION['ctrid']} AND
						ob.ob = '{$ob}'";

			$arrDados = $db->carregar($sql);

		}
	}else{
		$sql = "	SELECT
						es.nu_empenho AS empenho,
						obfnumero as ob,
						obsob,
						hspdsc,
						hspcnpj, 
						to_char(obfdata, 'DD/MM/YYYY') || ' ' || to_char(obfdata, 'HH24:MI') as datatransacao,
						ob.obsob,
						of.ungcod as unidade,
						'' as natureza,
						'' AS it_no_credor,
						'' AS it_co_credor,
						obfvalor as valorob
					FROM
						contratos.ctcontrato c
					JOIN contratos.hospital h ON h.hspid = c.hspid
					JOIN contratos.faturacontrato fc ON fc.ctrid = c.ctrid AND
														fc.ftcstatus = 'A'
					JOIN contratos.ordembancariafatura of ON of.ftcid = fc.ftcid AND
															 of.obfsiafi = false
					JOIN contratos.empenhovinculocontrato ec ON ec.epsid = of.epsid AND
																fc.ctrid = ec.ctrid										 
					JOIN contratos.empenho_siafi es ON es.epsid = ec.epsid
					JOIN contratos.ob_siafi ob ON ob.ob = of.obfnumero
					WHERE
						fc.ftcstatus = 'A' AND
						c.ctrid = {$_SESSION['ctrid']}
				UNION ALL
					SELECT
						ob.empenho,
						ob,
						obsob,
						hspdsc, 
						hspcnpj, 
						to_char(datatransacao, 'DD/MM/YYYY') || ' ' || to_char(datatransacao, 'HH24:MI') as datatransacao,
						obsob,
						unidade,
						ob.natureza,
						it_no_credor,
						it_co_credor,
						ob.valor as valorob
					FROM
						contratos.ctcontrato c
					JOIN contratos.hospital h ON h.hspid = c.hspid
					JOIN contratos.faturacontrato fc ON fc.ctrid = c.ctrid AND
														fc.ftcstatus = 'A'
					JOIN contratos.ordembancariafatura of ON of.ftcid = fc.ftcid AND
															 of.obfsiafi = true
					JOIN contratos.empenhovinculocontrato ec ON ec.epsid = of.epsid AND
																fc.ctrid = ec.ctrid										 
					JOIN contratos.empenho_siafi es ON es.epsid = ec.epsid																
					JOIN contratos.ob_siafi ob ON ob.ob = of.obfnumero AND
								      			  ob.empenho = es.nu_empenho
					WHERE
						c.ctrid = {$_SESSION['ctrid']}";


		$arrDados = $db->carregar($sql);

	}

	$arrDados = $arrDados ? $arrDados : array();
    $c = 0;
	if ( count($arrDados) ):
		foreach( $arrDados as $arrDados ):
            $c++;
?>
			<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
<?php 
			$arrLabel = array("Empenho",
							  "Número da OB",
							  "Nome da Contratante",
							  "CNPJ da Contratante",
							  "Data da Transação",
							  "Observação",
							  "Unidade",
							  "Natureza",
							  "Nome do Favorecido",
							  "CNPJ do Favorecido",
							  "Valor da OB");
			$n=0;
			foreach($arrDados as $chave => $valor): 
				if($chave == "natureza" && $valor){
					$ndpdsc = $db->pegaUm("select ndpdsc from naturezadespesa  where ndpcod = '".str_replace('.','',$valor)."'");
					$valor = $ndpdsc ? $valor.' - '.$ndpdsc : $valor;
				} 
				if( ($chave == "hspcnpj" || $chave == "it_co_credor" )&& $valor){
					$valor = mascara_global($valor, "##.###.###/####-##");
				}
				if($chave == "valorob"){
					$valor = number_format($valor, 2, ',', '.');
				}
?>
				<tr>
			        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%"><?php echo $arrLabel[$n] ? $arrLabel[$n] : $chave?></td>
			        <td><?php echo $valor ?></td>
				</tr>
<?php 
				$n++;
			endforeach;
?>
			</table>
			<br>
<?php
		endforeach;
	else:
?>
		<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
					
			<tr> 
				<td>	
					<font style="color:#cc0000;">Não foram encontrados registros.</font>
				</td>
			</tr>
		</table>	
<?php		
	endif;
}

function popUpListaOB(){
	global $db;

	if ( $_GET['ctrid'] && $_GET['entidcontratada'] ){
		$ctrid 				= $_GET['ctrid'];
		$entidcontratada 	= $_GET['entidcontratada'];
	
		$sql = "SELECT
					c.ctrid
				FROM contratos.ctcontrato c
				WHERE
					c.ctrid = {$ctrid} AND
					c.entidcontratada = {$entidcontratada}";
		$ctrid = $db->pegaUm($sql);
	
		if ( empty($ctrid) ){
		die('<script>alert(\'Faltam parâmetros para acessar a tela.\'); window.close();</script>');
		}
	
		$_SESSION['ctrid'] = $ctrid;
	}
	
	$msg = 'Todas as ordens bancárias vinculadas ao contrato';
	monta_titulo("Dados da Ordem Bancária", $msg);
?>
<form name=formulario id=formulario method=post >
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	    <tr>
	        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Buscar:</td>
	        <td>
        		<?php
					$arrAtributos 					= false;
					$arrAtributos['name'] 			= "pesquisar";
					$arrAtributos['obrigatorio'] 	= false;
					$arrAtributos['habilitado'] 	= true;
					$arrAtributos['size'] 			= 60;
					$arrAtributos['maxsize'] 		= 60;
					$arrAtributos['align'] 			= "left";
					$arrAtributos['value'] 			= $_POST['pesquisar'];
					echo campo_texto($arrAtributos)
				?>
	        </td>      
	    </tr>
	    <tr style="background-color: #cccccc">
	        <td align='right' ></td>
	        <td>
	        	<input type="submit" name="btn_buscar" value="Buscar" >
			<?php 
        	if( $_POST['pesquisar'] ):
			?>
	        	<input type="button" name="btn_novo" value="Ver Todas" onclick="window.location=window.location" >
			<?php 
        	endif;
			?>
	        </td>
	    </tr> 
	</table>
</form>
<?php

	$whereOrSql1 = array();
	$whereOrSql2 = array();
	if($_POST['pesquisar']){
		$whereOrSql1[] = "fc.ftcnumero ilike ('%{$_POST['pesquisar']}%')";
		$whereOrSql1[] = "of.obfnumero ilike ('%{$_POST['pesquisar']}%')";
		$whereOrSql1[] = "es.nu_empenho ilike ('%{$_POST['pesquisar']}%')";
		$whereOrSql1[] = "to_char(obfdatatransacao,'DD/MM/YYYY') ilike ('%{$_POST['pesquisar']}%')";
		$whereOrSql1[] = "to_char(obfdatatransacao,'DD-MM-YYYY') ilike ('%{$_POST['pesquisar']}%')";
		$whereOrSql1[] = "removeacento(fc.ftcnumero) ilike removeacento(('%{$_POST['pesquisar']}%'))";
		$whereOrSql1[] = "obfvalor::text ilike ('%".str_replace(array(".",","),array("",""),$_POST['pesquisar'])."%')";
		
		
		$whereOrSql2[] = "ob.ob ilike ('%{$_POST['pesquisar']}%')";
		$whereOrSql2[] = "es.nu_empenho ilike ('%{$_POST['pesquisar']}%')";
		$whereOrSql2[] = "to_char(ob.datatransacao,'DD/MM/YYYY') ilike ('%{$_POST['pesquisar']}%')";
		$whereOrSql2[] = "to_char(ob.datatransacao,'DD-MM-YYYY') ilike ('%{$_POST['pesquisar']}%')";
		$whereOrSql2[] = "ob.valor::text ilike ('%".str_replace(array(".",","),array("",""),$_POST['pesquisar'])."%')";
		
	}


	$sqlLista = "SELECT 
					a.*
				 FROM
				 (
		        		(SELECT
		    				'<span onmouseover=\"return escape(\'Visualizar resumo da ordem bancária: ' || of.obfnumero || '\');\">
			    				 <img 
			    					border=\"0\" 
			    					src=\"/imagens/icone_lupa.png\" 
			    					onclick=\"location.href = \'?modulo=principal/popUpFatura&acao=A&requisicao=popUpOB&numempenho=' || es.nu_empenho || '&ob=' || of.obfnumero || '\';\"
			    					style=\"cursor:pointer;\">
		    				 </span>' AS acao,
		    				 
		    				 '<span
								style=\"cursor:pointer; color:#6388DD;\" 
								onclick=\"location.href = \'?modulo=principal/popUpFatura&acao=A&requisicao=popUpFatura&ftcid=' || fc.ftcid || '\';\"
								onmouseover=\"return escape(\'Visualizar resumo da fatura: ' || fc.ftcnumero || '\');\">
								' || fc.ftcnumero || '
							 </span>' AS ftcnumero, 
		    				 
							'<span
								style=\"cursor:pointer; color:#6388DD;\" 
								onclick=\"location.href = \'?modulo=principal/popUpFatura&acao=A&requisicao=popUpEmpenho&numempenho=' || es.nu_empenho || '&cnpj=' || es.co_favorecido || '\';\"
								onmouseover=\"return escape(\'Visualizar resumo do empenho: ' || es.nu_empenho || '\');\">
								' || es.nu_empenho || '
							 </span>' AS empenho,
							  
							'<span
								style=\"cursor:pointer; color:#6388DD;\" 
								onclick=\"location.href = \'?modulo=principal/popUpFatura&acao=A&requisicao=popUpOB&numempenho=' || es.nu_empenho || '&ob=' || of.obfnumero || '\';\"
								onmouseover=\"return escape(\'Visualizar resumo da ordem bancária: ' || of.obfnumero || '\');\">
								' || obfnumero || '
							 </span>' AS ob,
							 CASE 
							 	WHEN of.obfsiafi = true THEN 'Siafi' 
								ELSE 'Manual' 
							 END AS tiporegistro,
							 to_char(obfdatatransacao, 'DD/MM/YYYY') || ' ' || to_char(obfdatatransacao, 'HH24:MI') as datatransacao,
							 obfvalor as valorob
						FROM
							contratos.ctcontrato c
						JOIN contratos.hospital h ON h.hspid = c.hspid
						JOIN contratos.faturacontrato fc ON fc.ctrid = c.ctrid AND
															fc.ftcstatus = 'A'
						JOIN contratos.ordembancariafatura of ON of.ftcid = fc.ftcid -- AND of.obfsiafi = false
						JOIN contratos.empenhovinculocontrato ec ON ec.epsid = of.epsid AND
																	fc.ctrid = ec.ctrid										 
						JOIN contratos.empenho_siafi es ON es.epsid = ec.epsid																
						WHERE
							fc.ftcstatus = 'A' AND
							fc.ctrid = {$_SESSION['ctrid']}
							" . (count($whereOrSql1) ? " AND (" . implode(' OR ',$whereOrSql1).")" : "") . "
						ORDER BY
							obfdatatransacao, fc.ftcnumero
					)UNION ALL(
						SELECT
							'<span onmouseover=\"return escape(\'Visualizar resumo da ordem bancária: ' || ob.ob || '\');\">
			    				 <img 
			    					border=\"0\" 
			    					src=\"/imagens/icone_lupa.png\" 
			    					onclick=\"location.href = \'?modulo=principal/popUpFatura&acao=A&requisicao=popUpOB&numempenho=' || es.nu_empenho || '&ob=' || ob.ob || '\';\"
			    					style=\"cursor:pointer;\">
		    				 </span>' AS acao,
		    				 
							'<font style=\"color:red;\">Não informado</font>' AS ftcnumero, 
							 
							'<span
								style=\"cursor:pointer; color:#6388DD;\" 
								onclick=\"location.href = \'?modulo=principal/popUpFatura&acao=A&requisicao=popUpEmpenho&numempenho=' || es.nu_empenho || '&cnpj=' || es.co_favorecido || '\';\"
								onmouseover=\"return escape(\'Visualizar resumo do empenho: ' || es.nu_empenho || '\');\">
								' || es.nu_empenho || '
							 </span>' AS empenho,
							  
							'<span
								style=\"cursor:pointer; color:#6388DD;\" 
								onclick=\"location.href = \'?modulo=principal/popUpFatura&acao=A&requisicao=popUpOB&numempenho=' || es.nu_empenho || '&ob=' || ob.ob || '\';\"
								onmouseover=\"return escape(\'Visualizar resumo da ordem bancária: ' || ob.ob || '\');\">
								' || ob || '
							 </span>' AS ob, 
							'Siafi' AS tiporegistro,
							to_char(ob.datatransacao, 'DD/MM/YYYY') || ' ' || to_char(ob.datatransacao, 'HH24:MI') as datatransacao,
							ob.valor as valorob
						FROM
							contratos.ctcontrato c
						JOIN entidade.entidade e ON e.entid = c.entidcontratada	
						JOIN contratos.hospital h ON h.hspid = c.hspid
						JOIN contratos.empenhovinculocontrato ec ON ec.ctrid = c.ctrid										 
						JOIN contratos.empenho_siafi es ON es.epsid = ec.epsid AND es.co_favorecido = e.entnumcpfcnpj																
						JOIN contratos.ob_siafi ob ON ob.empenho = es.nu_empenho  AND  ob.it_co_credor = e.entnumcpfcnpj 
													 AND ob.ob NOT IN (
																	SELECT 
																		obf.obfnumero 
																	FROM 
																		contratos.faturacontrato fc
																	JOIN contratos.ordembancariafatura obf ON obf.ftcid = fc.ftcid
																	WHERE 
																		fc.ftcstatus = 'A' AND
																		fc.ctrid =  {$_SESSION['ctrid']}
																	)
						WHERE
							c.ctrstatus = 'A' AND
							c.ctrid = {$_SESSION['ctrid']}
							" . (count($whereOrSql2) ? " AND (" . implode(' OR ',$whereOrSql2).")" : "") . "
						ORDER BY
							ob.datatransacao)
				) a
			ORDER BY
		        datatransacao";
   //dbg($sqlLista);	
	$cabecalho = array("Ação", "Número da Nota Fiscal", "Número do Empenho", "Número da OB", "Vinculação", "Data da Transação", "Valor da OB");
	$db->monta_lista($sqlLista, $cabecalho, 50, 5, 'S', 'center', 'S', '', '', array('center', 'right', 'center', 'center', '', 'center'));
}

function popUpFatura()
{
	global $db;
	
	include_once APPRAIZ."includes/classes/Modelo.class.inc";
	include_once APPRAIZ."contratos/classes/FaturaContrato.class.inc";
	include_once APPRAIZ."contratos/classes/OrdemBancariaFatura.class.inc";
	include_once APPRAIZ."contratos/classes/AnexoFatura.class.inc";
		
	if ( $_GET['nf'] == 'paga' ){
		$msg = 'Todas as notas fiscais pagas'; 
	}elseif ( (!isset($_GET['ftcid']) || empty($_GET['ftcid']))){
		$msg = 'Todas as notas fiscais cadastradas'; 
	}elseif ( isset($_GET['ftcid']) && !is_numeric($_GET['ftcid']) ){
		$msg = 'Resumo das OBs que ainda não foram vinculadas a notas fiscais'; 
	}else{
		$msg = '&nbsp;'; 
	}
	
	monta_titulo("Dados do Pagamento", $msg);
	
	if ( is_numeric( $_GET['ftcid'] ) ){
		$faturaContrato = new FaturaContrato($_GET['ftcid']);
		$arDados 		= $faturaContrato->getDados();
		$arDados 		= array( $arDados );
	}elseif ( !empty($_GET['ftcid']) && !is_numeric( $_GET['ftcid'] ) ){
		$arDados = array(
						array(
							'ftcnumero' 	 => 'Não informado',
							'ftcdescricao' 	 => 'Não informado',
							'ftcdataemissao' => 'Não informado',
							'ftcvalor' 		 => 'Não informado',
							)
						);

	}elseif ( $_GET['nf'] == 'paga' ){
		$param 			= array();
		$param['esdid'] = ESTADO_WK_FATURAMENTO_PAGO;
		$faturaContrato = new FaturaContrato();
		$arDados 		= $faturaContrato->listaDadosPorCtrid( $_SESSION['ctrid'], $param );
	}else{
		$param 			= array();
		$faturaContrato = new FaturaContrato();
		$arDados 		= $faturaContrato->listaDadosPorCtrid( $_SESSION['ctrid'], $param );
	}
	//dbg($arDados, d);	
	if($arDados):
	
		$ob = new OrdemBancariaFatura();

		for($i=0; count($arDados) > $i; $i++):
			extract( $arDados[$i] );
			if ( $ftcid ){
				$ftcdataemissao = ($ftcdataemissao ? formata_data($ftcdataemissao) : "");
				$ftcvalor 		= (is_numeric($ftcvalor) ? number_format($ftcvalor, 2, ',', '.') : "");
				
				$arrOrdem = $ob->getOrdemBancaria($ftcid);
			}else{
				$arrOrdem = $ob->getOrdemBancariaSiafiPorCtrid( $_SESSION['ctrid'] );
			}
	?>
			<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
			
		<!-- 	    <tr> 
			        	<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Empenho:</td>
			        	<td><?php //echo $numempenho ?></td>
					</tr> -->
				<tr>
			        <td align='right' class="SubTituloDireita" width="20%">Número da Fatura:</td>
			        <td><?php echo $ftcnumero ?></td>
				</tr>
			    <tr>
			        <td align='right' class="SubTituloDireita" valign="top">Descrição:</td>
			        <td><?php echo $ftcdescricao ?></td>
				</tr>
			    <tr>
			        <td align='right' class="SubTituloDireita">Data de Emissão:</td>
			        <td><?php echo $ftcdataemissao ?></td>
				</tr>
			    <tr>
			        <td align='right' class="SubTituloDireita">Valor:</td>
			        <td><?php echo $ftcvalor ?></td>
				</tr>
			    <tr>
			        <td align='right' class="SubTituloDireita" valign="top">Ordem Bancária:</td>
			        <td>
			        	<?php if($arrOrdem): ?>
			        		<table class="listagem" cellspacing="1" cellpadding="3" style="width:90%" >
			        			<thead>
			        				<td align="center" ><b>Empenho</b></td>
			        				<td align="center" ><b>Número OB</b></td>
			        				<td align="center" ><b>Data da OB</b></td>
			        				<td align="center" ><b>Valor da OB</b></td>
			        			</thead>
				        	<?php foreach($arrOrdem as $ordem):?>
				        		<tr>
				        			<td>
					        			<a href="?modulo=principal/popUpFatura&acao=A&requisicao=popUpEmpenho&numempenho=<?php echo $ordem['nu_empenho'];?>&cnpj=<?php echo $ordem['cnpj'];?>">
				        				<?php 
				        					echo $ordem['nu_empenho'] 
				        				?>
					        			</a>
				        			</td>
				        			<td>
					        			<a href="?modulo=principal/popUpFatura&acao=A&requisicao=popUpOB&numempenho=<?php echo $ordem['nu_empenho'];?>&ob=<?php echo $ordem['obfnumero'];?>">
					        			<?php 
					        				echo $ordem['obfnumero'] 
					        			?>
					        			</a>
				        			</td>
				        			<td align="center">
				        				<?php echo ($ordem['obfdatatransacao'] ? formata_data( $ordem['obfdatatransacao'] ) : '-') ?>
				        			</td>
				        			<td align="right" style="color:#0066CC;">
			        				<?php 
			        					$soma_ob += $ordem['obfvalor']; 
			        					echo $ordem['obfvalor'] ? number_format($ordem['obfvalor'],2,',','.') : "" 
			        				?>
				        			</td>
				        		</tr>
				        	<?php endforeach;?>
				        		<tr>
				        			<td><b>Total</b></td>
				        			<td>&nbsp;</td>
				        			<td>&nbsp;</td>
				        			<td align="right" style="color:#0066CC;">
				        				<b><?php echo $soma_ob ? number_format($soma_ob,2,',','.') : "" ?></b>
				        			</td>
				        		</tr>
				        	</table>
				        <?php else:?>
				        	Não há ordens bancárias vinculadas à nota fiscal	
				        <?php endif;?>
			        </td>
				</tr>
<?php 
				if ( $ftcid ):
?>				
				<tr bgcolor="#DCDCDC">
					<td colspan="2"><center><b><span style="font-size:14px;">Lista de Anexos</span></b></center></td>
				</tr>
<?php 
				endif;
?>				
			</table>
			<?php
			if ( $ftcid ):
				$anexo = new AnexoFatura();
				$anexo->listaAnexo($ftcid,false);
				
				if ( count($arDados) > ($i + 1)  ){
					echo '<br/>';
					echo '<br/>';
				}
			endif;
		endfor;
	else:
?>	
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
			
	<tr> 
		<td>	
			<font style="color:#cc0000;">Não foram encontrados registros.</font>
		</td>
	</tr>
</table>	
<?php		
	endif;
}

function popUpListaFatura()
{
	global $db;
	
	include_once APPRAIZ."includes/classes/Modelo.class.inc";
	include_once APPRAIZ."contratos/classes/FaturaContrato.class.inc";
	include_once APPRAIZ."contratos/classes/OrdemBancariaFatura.class.inc";
	include_once APPRAIZ."contratos/classes/AnexoFatura.class.inc";
		
	if ( $_GET['ctrid'] && $_GET['entidcontratada'] ){
		$ctrid 				= $_GET['ctrid'];
		$entidcontratada 	= $_GET['entidcontratada'];
	
		$sql = "SELECT
					c.ctrid
				FROM contratos.ctcontrato c
				WHERE
					c.ctrid = {$ctrid} AND
					c.entidcontratada = {$entidcontratada}";
		$ctrid = $db->pegaUm($sql);	
		
		if ( empty($ctrid) ){
			die('<script>alert(\'Faltam parâmetros para acessar a tela.\'); window.close();</script>');
		}
		
		$_SESSION['ctrid'] = $ctrid;
	}
	
	if ( $_GET['nf'] == 'paga' ){
		$msg = 'Todas as notas fiscais pagas do contrato'; 
	}else{
		$msg = 'Todas as notas fiscais do contrato'; 
	}
	
	monta_titulo("Lista de Pagamentos", $msg);
	
	$faturaContrato = new FaturaContrato();
	if ( $_GET['nf'] == 'paga' ){
		$param 			= $_POST;
		$param['esdid'] = ESTADO_WK_FATURAMENTO_PAGO;
		
		$sqlLista 		= $faturaContrato->listaSqlResumoPorCtrid($_SESSION['ctrid'], $param );
	}else{
		$param 			= $_POST;
		
		$sqlLista 		= $faturaContrato->listaSqlResumoPorCtrid($_SESSION['ctrid'], $param );
	}
?>
<form name=formulario id=formulario method=post >
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	    <tr>
	        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Buscar:</td>
	        <td>
        		<?php
					$arrAtributos 					= false;
					$arrAtributos['name'] 			= "pesquisar";
					$arrAtributos['obrigatorio'] 	= false;
					$arrAtributos['habilitado'] 	= true;
					$arrAtributos['size'] 			= 60;
					$arrAtributos['maxsize'] 		= 60;
					$arrAtributos['align'] 			= "left";
					$arrAtributos['value'] 			= $_POST['pesquisar'];
					echo campo_texto($arrAtributos)
				?>
	        </td>      
	    </tr>
	    <tr style="background-color: #cccccc">
	        <td align='right' ></td>
	        <td>
	        	<input type="submit" name="btn_buscar" value="Buscar" >
			<?php 
        	if( $_POST['pesquisar'] ):
			?>
	        	<input type="button" name="btn_novo" value="Ver Todas" onclick="window.location=window.location" >
			<?php 
        	endif;
			?>
	        </td>
	    </tr> 
	</table>
</form>
<?php
	$cabecalho = array("Ação", "Número da NF", "Data de Emissão", "Valor da NF", "Situação");
	$db->monta_lista($sqlLista, $cabecalho, 15, 5, 'N', 'center', '', '', '', array('center', 'right', 'center'));
}

function listarFiscalContrato($ctrid = null)
{
	global $db, $desabilitado;
	$ctrid = $ctrid ? $ctrid : $_SESSION['ctrid'];
	$sql = "select
				'".(($desabilitado)?"":"<img src=\"../imagens/alterar.gif\" class=\"link\" onclick=\"editarFiscal(' || ent.entid || ')\" /> <img src=\"../imagens/excluir.gif\" class=\"link\" onclick=\"removerFiscal(' || fsc.fscid || ')\" />")."' as acao,
				entnome,
				entnumcpfcnpj
			from
				contratos.fiscalcontrato fsc
			inner join
				entidade.entidade ent ON ent.entid = fsc.entid
			where
				ctrid = {$_SESSION['ctrid']}
			and
				fsc.fscstatus = 'A'
			order by
				entnome";

	$arrCab = array("Ação","Nome","CPF");
	$db->monta_lista_simples($sql,$arrCab,1000,1000,"N");		
}

function listarGestorContrato($ctrid = null)
{
	global $db, $desabilitado;
	$ctrid = $ctrid ? $ctrid : $_SESSION['ctrid'];
	$sql = "select
				'".(($desabilitado)?"":"<img src=\"../imagens/alterar.gif\" class=\"link\" onclick=\"editarGestor(' || ent.entid || ')\" /> <img src=\"../imagens/excluir.gif\" class=\"link\" onclick=\"removerGestor(' || gsc.gscid || ')\" />")."' as acao,
				entnome,
				entnumcpfcnpj
			from
				contratos.gestorcontrato gsc
			inner join
				entidade.entidade ent ON ent.entid = gsc.entid
			where
				ctrid = $ctrid
			and
				gsc.gscstatus = 'A'
			order by
				entnome";
	$arrCab = array("Ação","Nome","CPF");
	$db->monta_lista_simples($sql,$arrCab,1000,1000,"N");		
}

function salvarFiscalContrato($entid)
{
	global $db;
	$ctrid = $_SESSION['ctrid'];
	$sql = "select count(*) from contratos.fiscalcontrato where ctrid = $ctrid and entid = $entid and fscstatus = 'A'";
	$existe = $db->pegaUm($sql);
	if($_GET['entidselecionado'] != $entid && $existe == 0){
		$sql = "insert into contratos.fiscalcontrato (ctrid,entid) values ($ctrid,$entid)";
		$db->executar($sql);
		$db->commit($sql);
	}
}

function salvarCadastroResponsavel($entid)
{
    global $db;
    $ctrid = $_SESSION['ctrid'];
    $ctrid = '1';
    $sql = "select count(*) from contratos.cadastroresponsavel where ctrid = $ctrid and entid = $entid and fscstatus = 'A'";
    $existe = $db->pegaUm($sql);
    if($_GET['entidselecionado'] != $entid && $existe == 0){
        $sql = "insert into contratos.cadastroresponsavel (ctrid,entid) values ($ctrid,$entid)";
        $db->executar($sql);
        $db->commit($sql);
    }
}

function salvarGestorContrato($entid)
{
	global $db;
	$ctrid = $_SESSION['ctrid'];
	$sql = "select count(*) from contratos.gestorcontrato where ctrid = $ctrid and entid = $entid and gscstatus = 'A'";
	$existe = $db->pegaUm($sql);
	if($_GET['entidselecionado'] != $entid && $existe == 0){
		$sql = "insert into contratos.gestorcontrato (ctrid,entid) values ($ctrid,$entid)";
		$db->executar($sql);
		$db->commit($sql);
	}
}

function removerFiscal()
{
	global $db;
	$fscid = $_POST['fscid'];
	if($fscid){
		$sql = "update contratos.fiscalcontrato set fscstatus = 'I'  where fscid = $fscid";
		$db->executar($sql);
		$db->commit($sql);
	}
	listarFiscalContrato();
}

function removerGestor()
{
	global $db;
	$gscid = $_POST['gscid'];
	if($gscid){
		$sql = "update contratos.gestorcontrato set gscstatus = 'I'  where gscid = $gscid";
		$db->executar($sql);
		$db->commit($sql);
	}
	listarGestorContrato();
}
function validarOrdemPagamento(){

	global $db;
	$ftcid = ($_REQUEST['ftcid'])? $_REQUEST['ftcid'] : $_GET['ftcid'];
	//dbg($ftcid, d);
	if ($ftcid){
		$sql = "SELECT count(*) FROM contratos.ordembancariafatura WHERE ftcid=".$ftcid;
		
		$buscaOB = $db->pegaUm($sql);
		dbg($buscaOB, d);
		if($buscaOB==0)
			return false;
		else 
			return true;
	}else
		return false;
}

function getSqlEntidadeHospital()
{
    $sql ="SELECT
                    ent.entid as codigo,
                    ent.entnome as descricao
            FROM
                    entidade.entidade ent
            INNER JOIN
                    entidade.organograma org on ent.entid = org.entid and orgstatus = 'A'
            INNER JOIN
                    entidade.entidade ent2 on ent2.entid = org.orgentiddono and orgstatus = 'A'
            WHERE
                    org.catid = ".CATEGORIA_FILIAL."
            OR
                    ent.entid = ".ENTID_EBSERH."
            ORDER BY descricao";
    return $sql;
}

?>
