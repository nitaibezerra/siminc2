<?php
if($_REQUEST['req']=='popupimpressao' && $_REQUEST['acao']!='I'){
	imprimirContrato($_SESSION['ctrid']);
	exit;
}
// verifica se foi solicitado para vincular o Processo
if($_REQUEST['requisicao'] == 'verificarProcesso'){
	if($_REQUEST['codProcesso']){
		global $db;

		$codProcesso = $_REQUEST['codProcesso'];
		$codProcesso = str_replace(array("/", "-", "."), "", $codProcesso);

		$count = strlen($codProcesso);
		if ($count == 17) {
			$pftNumeroProtocolo = substr($codProcesso, 0,11);
			$pftAno = substr($codProcesso, 11,-2);
		} else {
			$pftNumeroProtocolo = substr($codProcesso, 0,6);
			$pftAno = substr($codProcesso, 6,-2);
		}

		$prtDigito1 = (int)substr($codProcesso, -2, -1);
		$prtDigito2 = (int)substr($codProcesso, -1);

		$sql = "select prtid from protocolo.protocolo where  prtnumeroprotocolo = '".$pftNumeroProtocolo."' and prtano = '".$pftAno."' and prtdigitoverificador = ".$prtDigito1." and prtdigitoverificador2 = ".$prtDigito2."";
		$prtid = $db->pegaUm($sql);

		if ($prtid) {
			echo json_encode(array('prtid' => $prtid), true);
		} else {
			echo json_encode(array('erro' => true), false);
		}
		exit();
	}
}

if($_REQUEST['requisicao'] == 'vincularProcesso'){
	if($_REQUEST['prtid']){
		vincularTermo($_REQUEST['prtid']);
	}
}

function vincularTermo($prtid){
	if ($prtid) {
		global $db;
		$ctrid = ($_SESSION['ctrid'] ? $_SESSION['ctrid'] : $_REQUEST['ctrid']);
		$sqlUpdate = "update contratos.ctcontrato set prtid = ".$prtid." where ctrid = ".$ctrid."";

		$db->executar($sqlUpdate);
		if($db->commit()){
			$_SESSION['prtid'] = $prtid;
			if(!$_POST['action']) {
				die('1');
			}
		} else {
			if(!$_POST['action']) {
				die('0');
			}
		}
	}
}

if($_REQUEST['req'] == 'verificaModalidade'){
	$sql = "select modexigedsc from contratos.ctmodalidadecontrato where modid = {$_REQUEST['modid']}";
	$rs = $db->pegaUm($sql);
	echo $rs;
	exit;
}

include_once APPRAIZ . "includes/classes/dateTime.inc";

if($_REQUEST['acao'] == 'I'){
	unset($_SESSION['ctrid']);
	unset($_SESSION['prtid']);
}

if($_REQUEST['ctrid'] && $_REQUEST['acao'] == 'A'){
	$_SESSION['ctrid'] = $_REQUEST['ctrid'];
    $sql = "select * from contratos.historicogarantiacontrato where ctrid = {$_SESSION['ctrid']}";
    $rs = $db->carregar($sql);
    if (!$rs) {
        echo "<script>alert('Por favor cadastrar a Garantia do contrato')</script>";
    }
} elseif($_SESSION['ctrid'] && $_REQUEST['acao'] == 'A'){
	$_REQUEST['ctrid'] = $_SESSION['ctrid'];


}

if($_POST['action']){
	salvar();
}

if( $_REQUEST['ctrid'] && $_REQUEST['acao'] == 'A')
{ 
			
	$sql = "SELECT  
					ctr.ctrnum, 
		    		ctr.ctrano, 
		    		ctr.tpcid, 
		    		ctr.ctrproccontr, 
		    		ctr.ctrprocexecfin, 
		    		ctr.ctrprocexecctr, 
            		ctr.modid, 
            		ctr.ctrnummod, 
            		ctr.ctrobj, 
					ctr.retid,
            		ctr.ctrobs,
            		ctr.unaid, 
            		ctr.ctrnumcron, 
            		ctr.entidcontratada, 
            		ctr.ctrvlrinicial as ctrvlrinicial,					            		 
       				ctr.ctrobsvlrini, 
       				ctr.ctrvlrglobal as ctrvlrglobal,
			       	ctr.ctrvlrtotal as ctrvlrtotal,
       				ctr.ctrvlrmensal as ctrvlrmensal, 
       				ctr.ctrobsvlr, 
       				to_char(ctr.ctrdtiniciovig::date,'YYYY-MM-dd') AS ctrdtiniciovig,
       				to_char(ctr.ctrdtfimvig::date,'YYYY-MM-dd') AS ctrdtfimvig,       		
       				ctr.obvid,
       				ctr.sitid,
					ctr.entidcontratante ,
					ctr.ctrid,
					ctr.modexigedsc,
					ctr.ctrvlrreferencia,
					ctr.ctrvlrhomologado,
					ctr.ctrvlrrefhomolog,
					ctr.prtid,
					to_char(ctr.ctrdtassinatura::date,'YYYY-MM-dd') AS ctrdtassinatura,
					to_char(ctr.ctrdtpublicacao::date,'YYYY-MM-dd') AS ctrdtpublicacao,
					ctr.ctrcntcontabil,
					ctr.ctrjus,
					ctr.ctrprcjurctr,
					ctr.ctrprcjuradt,
					ctr.ctrparjuridico,
					ctr.ctrmaodeobra,
					ctr.ctrcontinuada,
					ctr.numcontinuada
					-- con.ungcod
  			FROM contratos.ctcontrato ctr 
			--LEFT JOIN contratos.contratante con ON con.ungid = ctr.ungid 
			WHERE
				ctr.ctrid = '".$_REQUEST['ctrid']."'
				";
	$rsDadosContrato = $db->carregar( $sql );

	
	if ($rsDadosContrato && $rsDadosContrato[0]['prtid']) {
		$_SESSION['prtid'] = $rsDadosContrato[0]['prtid'];
	}elseif (!$rsDadosContrato[0]['prtid']) {
		unset($_SESSION['prtid']);
	}	
		
	$_SESSION['ctrid'] = $_REQUEST['ctrid'];
}

function salvarSetorResponsavel($ctrid)
{
	global $db;
	$sql = "delete from contratos.contratosetorresponsavel where ctrid = $ctrid";
	$db->executar($sql);
	$db->commit();
	if($_POST['estid'][0]){
		foreach($_POST['estid'] as $estid)
		{
			$sqlI .= "insert into contratos.contratosetorresponsavel (ctrid,estid) values ($ctrid,$estid);";
		}
		$db->executar($sqlI);
		$db->commit();
	}
}

function salvarEmpenhoContrato($ctrid, $acao)
{
	global $db;
	
	// Obtem o cnpj
	$entidempresa = ($rsDadosContrato[0]["entidcontratada"] ? $rsDadosContrato[0]["entidcontratada"] : $_POST['entidempresa']);
	if( $entidempresa ){
		$sql = "select
					ent.entnumcpfcnpj as cnpj
				from entidade.entidade ent
					left join entidade.funcaoentidade fue on ent.entid = fue.entid
				where
					ent.entid = " . $entidempresa;
		$entcnpjempresa = $db->pegaUm($sql);
	}
	
	//verifica se soma dos valores do empenho é maior que o valor do contrato.
	$rsNuEmpenhos = $_POST['numempenho'];
	if($rsNuEmpenhos) {
		//$vlr_empenho = pegaValorPorEmpenhos ( $rsNuEmpenhos, $ctrid );
		if(!$vlr_empenho) $vlr_empenho = 0;
	}

	$ctrvlrinicial = str_replace(',','.',str_replace('.','',$_REQUEST['ctrvlrinicial']));
	if(!$ctrvlrinicial) $ctrvlrinicial = 0;
	
	if($acao == 'I'){
		if($vlr_empenho > $ctrvlrinicial){
			echo '<script>
					alert("A soma dos empenhos (R$ '.number_format($vlr_empenho, 2, ',', '.').') não pode ser maior que o valor total do contrato (R$ '.number_format($ctrvlrinicial, 2, ',', '.').')!");
					history.back();
				  </script>';
			die();
		}
		
	}
	else{
		
		$somaAditivos = $db->pegaUm("select sum(tdavlr) as total from contratos.cttermoaditivo 
									 where tdastatus = 'A'
									 and ctrid={$ctrid}");
		$vlTotal = ($ctrvlrinicial+$somaAditivos);
		if($vlr_empenho > $vlTotal){
			echo '<script>
					alert("A soma dos empenhos (R$ '.number_format($vlr_empenho, 2, ',', '.').') não pode ser maior que o valor total do contrato (R$ '.number_format($vlTotal, 2, ',', '.').')!");
					history.back();
				  </script>';
			die();
		}
		
	}
	
	$sql = "DELETE FROM contratos.empenhovinculocontrato WHERE ctrid = $ctrid";
	$db->executar($sql);
	$db->commit();
	
	if($_POST['numempenho'][0]){
		foreach($_POST['numempenho'] as $numempenho)
		{
			$sqlI .= "INSERT INTO contratos.empenhovinculocontrato (ctrid,epsid) VALUES ($ctrid,'$numempenho');";
		}
		$db->executar($sqlI);
		$db->commit();
	}
}

function verificaProcessoProtocolo($codProcesso) {
	global $db;
	
	$codProcesso = str_replace(array("/", "-", "."), "", $codProcesso);
	
	$count = strlen($codProcesso);
	if ($count == 17) {
		$pftNumeroProtocolo = substr($codProcesso, 0,11);
		$pftAno = substr($codProcesso, 11,-2);
	} else {
		$pftNumeroProtocolo = substr($codProcesso, 0,6);
		$pftAno = substr($codProcesso, 6,-2);
	}
	
	$prtDigito1 = (int)substr($codProcesso, -2, -1);
	$prtDigito2 = (int)substr($codProcesso, -1);
	
	return true;
	
	$sql = "select prtid from protocolo.protocolo where  prtnumeroprotocolo = '".$pftNumeroProtocolo."' and prtano = '".$pftAno."' and prtdigitoverificador = ".$prtDigito1." and prtdigitoverificador2 = ".$prtDigito2."";
	$prtid = $db->pegaUm($sql);
	
	return $prtid;
	
}

function salvar(){
	
	global $db; 
 	 
	$formataDatamI = formata_data_sql( $_REQUEST['ctrdtiniciovig'] );
	$formataDatamF = formata_data_sql( $_REQUEST['ctrdtfimvig'] ); 
	$formataDataAssinatura = formata_data_sql($_REQUEST['ctrdtassinatura']);
	$formataDataDOU = formata_data_sql($_REQUEST['ctrdtpublicacao']);
	$ctrvlrinicial = str_replace(',','.',str_replace('.','',$_REQUEST['ctrvlrinicial']));
	if(!$ctrvlrinicial) $ctrvlrinicial = 'NULL';
	else $ctrvlrinicial = "'".$ctrvlrinicial."'";
	
	$ctrvlrmensal = str_replace(',','.',str_replace('.','',$_REQUEST['ctrvlrmensal']));
	if(!$ctrvlrmensal) $ctrvlrmensal = 'NULL';
	else $ctrvlrmensal = "'".$ctrvlrmensal."'";
	
	$ctrvlrtotal = str_replace(',','.',str_replace('.','',$_REQUEST['ctrvlrtotal']));
	if(!$ctrvlrtotal) $ctrvlrtotal = 'NULL';
	else $ctrvlrtotal = "'".$ctrvlrtotal."'";
	
	$ctrvlrglobal = str_replace(',','.',str_replace('.','',$_REQUEST['ctrvlrglobal']));
	if(!$ctrvlrglobal) $ctrvlrglobal = 'NULL';
	else $ctrvlrglobal = "'".$ctrvlrglobal."'";
	
	
	$ctrvlrhomologado = str_replace(',','.',str_replace('.','',$_REQUEST['ctrvlrhomologado']));
	if(!$ctrvlrhomologado) $ctrvlrhomologado = 'NULL';
	else $ctrvlrhomologado = "'".$ctrvlrhomologado."'";
	
	$ctrvlrrefhomolog = str_replace(',','.',str_replace('.','',$_REQUEST['ctrvlrrefhomolog']));
	if(!$ctrvlrrefhomolog) $ctrvlrrefhomolog = 'NULL';
	else $ctrvlrrefhomolog = "'".$ctrvlrrefhomolog."'";
	
	$ctrvlrreferencia = str_replace(',','.',str_replace('.','',$_REQUEST['ctrvlrreferencia']));
	if(!$ctrvlrreferencia) $ctrvlrreferencia = 'NULL';
	else $ctrvlrreferencia = "'".$ctrvlrreferencia."'";
	
	//o número do processo do contrato passa a ser o prtid do protocolo somente
	//não se deve mais armazenar o número
	$ctrproccontr = $_POST['ctrproccontr'];
	if ($_SESSION['prtid']) {
		$_POST['ctrproccontr'] = '';
	}

	/*
	if(!$_REQUEST['ctrobj']){
		die("<script>
				alert('O campo Descrição do Objeto do Contrato é obrigatório');
				history.back();
		     </script>");
	}
	*/

    $sql = "select ctrnum from contratos.ctcontrato where ctrano = '{$_POST['ctrano']}' and ctrnum = {$_POST['ctrnum']} order by ctrnum DESC limit 10 ";

    $rs = $db->pegaUm($sql);

        if(!$_REQUEST['ctrid'] && $_REQUEST['acao'] == 'I'){

            $sql = "INSERT INTO
	    		contratos.ctcontrato
	    		(
					ctrnum,
					ctrano,
					tpcid,
	    			estid,
					ctrproccontr,
					ctrprocexecfin,
					ctrprocexecctr,
					modid,
					ctrnummod,
					ctrobj,
					ctrobs,
					unaid,
					ctrnumcron,
					entidcontratada,
					ctrvlrinicial,
					ctrobsvlrini,
					ctrvlrtotal,
					ctrvlrglobal,
					ctrvlrmensal,
					ctrobsvlr,
					ctrdtiniciovig,
					ctrdtfimvig,
					obvid,
					sitid,
	    			entidcontratante,
	    			modexigedsc,
	    			ctrvlrreferencia,
					ctrvlrhomologado,
					ctrvlrrefhomolog,
		    		ctrdtassinatura,
		    		ctrjus,
					ctrprcjurctr,
					ctrprcjuradt,
	    			prtid,
	    			ctrdtpublicacao,
	    			ctrcntcontabil,
	    			retid,
	    			ctrparjuridico,
	    			ctrmaodeobra,
	    			ctrcontinuada,
	    			garantia
	    						)
				VALUES
				(
					'".$_POST['ctrnum']."',
					'".$_POST['ctrano']."',
					".( $_POST['tpcid'] ? $_POST['tpcid'] : 'NULL' ).",
					".( $_POST['dmtid']? $_POST['dmtid'] : 'NULL' ).",
					'".$_POST['ctrproccontr']."',
					'".$_POST['ctrprocexecfin']."',
					'".$_POST['ctrprocexecctr']."',
					".( $_POST['modid'] ? $_POST['modid'] : 'NULL' ).",
					'".$_POST['ctrnummod']."',
					'".$_POST['ctrobj']."',
					'".$_POST['ctrobs']."',
					".( $_POST['unaid'] ? $_POST['unaid'] : 'NULL' ).",
					'".$_POST['ctrnumcron']."',
					".( $_POST['entidempresa'] ? $_POST['entidempresa'] : 'NULL' ).",
					".trim(str_replace('R$','',$ctrvlrinicial)).",
                                        '".$_POST['ctrobsvlrini']."',
					".trim(str_replace('R$','',$ctrvlrtotal)).",
                                        ".trim(str_replace('R$','',$ctrvlrglobal)).",
                                        ".trim(str_replace('R$','',$ctrvlrmensal)).",
                                        '".$_POST['ctrobsvlr']."',
					'".$formataDatamI."',
					'".$formataDatamF."',
					".( $_POST['obvid'] ? $_POST['obvid'] : 'NULL' ).",
					".( $_POST['sitid'] ? $_POST['sitid'] : 'NULL' ).",
					".( $_POST['entidcontratante'] ? $_POST['entidcontratante'] : 'NULL' ).",
					'".($_POST['modexigedsc'] ? $_POST['modexigedsc'] : '')."',
                                        ".trim(str_replace('R$','',$ctrvlrreferencia)).",     
                                        ".trim(str_replace('R$','',$ctrvlrhomologado)).",     
                                        ".trim(str_replace('R$','',$ctrvlrrefhomolog)).",     
					".($formataDataAssinatura?"'".$formataDataAssinatura."'": 'null').",
					'".$_POST['ctrjus']."',
					'".$_POST['ctrprcjurctr']."',
					'".$_POST['ctrprcjuradt']."',
					".( $_POST['prtid'] ? $_POST['prtid'] : 'NULL' ).",
					".($formataDataDOU?"'".$formataDataDOU."'": 'null').",
					'".$_POST['ctrcntcontabil']."',
					".( $_POST['retid'] ? $_POST['retid'] : 'NULL' ).",
					'".$_POST['ctrparjuridico']."',
					'".$_POST['maodeobra']."',
					'".$_POST['continuado']."',
					'".$_POST['garantia']."'
				)
	    		RETURNING ctrid";
            $ctrid = $db->pegaUm($sql);
            $_SESSION['ctrid'] = $ctrid;
            $db->commit();

        }else{
            $sql = "UPDATE contratos.ctcontrato
				SET
					ctrnum				= '".$_POST['ctrnum']."',
					ctrano 				= '".$_POST['ctrano']."',
					tpcid				= ".( $_POST['tpcid'] ? $_POST['tpcid'] : 'NULL' ).",
					ctrproccontr		= '".$_POST['ctrproccontr']."',
					ctrprocexecfin		= '".$_POST['ctrprocexecfin']."',
					ctrprocexecctr		= '".$_POST['ctrprocexecctr']."',
					estid				= ".( $_POST['dmtid'] ? $_POST['dmtid'] : 'NULL' ).",
					modid				= ".( $_POST['modid'] ? $_POST['modid'] : 'NULL' ).",
					ctrnummod			= '".$_POST['ctrnummod']."',
					ctrobj				= '".$_POST['ctrobj']."',
					ctrobs				= '".$_POST['ctrobs']."',
					unaid				= ".( $_POST['unaid'] ? $_POST['unaid'] : 'NULL' ).",
					ctrnumcron			= '".$_POST['ctrnumcron']."',
					entidcontratada		= ".( $_POST['entidempresa'] ? $_POST['entidempresa'] : 'NULL' ).",
					ctrvlrinicial		= ".trim(str_replace('R$','',$ctrvlrinicial)).",
					ctrobsvlrini		= '".$_POST['ctrobsvlrini']."',
					ctrvlrtotal		= ".trim(str_replace('R$','',$ctrvlrtotal)).",
					ctrvlrglobal		= ".trim(str_replace('R$','',$ctrvlrglobal)).",
					ctrvlrmensal		= ".trim(str_replace('R$','',$ctrvlrmensal)).", 
					ctrobsvlr		= '".$_POST['ctrobsvlr']."', 
					ctrdtiniciovig		= '".$formataDatamI."',
					ctrdtfimvig			= '".$formataDatamF."',
					obvid				= ".( $_POST['obvid'] ? $_POST['obvid'] : 'NULL' ).",
					entidcontratante	= ".( $_POST['entidcontratante'] ? $_POST['entidcontratante'] : 'NULL' ).",
					sitid				= ".( $_POST['sitid'] ? $_POST['sitid'] : 'NULL' ).",
					modexigedsc			= '".( $_POST['modexigedsc'] ? $_POST['modexigedsc'] : '' )."',
					ctrvlrreferencia	= ".trim(str_replace('R$','',$ctrvlrreferencia)).",
					ctrvlrhomologado	= ".trim(str_replace('R$','',$ctrvlrhomologado)).", 
					ctrvlrrefhomolog	= ".trim(str_replace('R$','',$ctrvlrrefhomolog)).", 
					ctrdtassinatura		=".($formataDataAssinatura?"'".$formataDataAssinatura."'": 'null').",
					ctrjus				='".$_POST['ctrjus']."',
					ctrprcjurctr		='".$_POST['ctrprcjurctr']."',
					ctrprcjuradt		='".$_POST['ctrprcjuradt']."',
					prtid				= ".( $_SESSION['prtid'] ? $_SESSION['prtid'] : 'NULL' ).",
					ctrdtpublicacao 	= ".( $formataDataDOU ? "'".$formataDataDOU."'" : 'NULL' ).",
					ctrcntcontabil		='".$_POST['ctrcntcontabil']."',
					retid				=".( $_POST['retid'] ? $_POST['retid'] : 'NULL' ).",
					ctrparjuridico      ='".$_POST['ctrparjuridico']."',
					ctrmaodeobra        ='".$_POST['maodeobra']."',
					ctrcontinuada       ='".$_POST['continuado']."',
					numcontinuada       ='".$_POST['ctrnumcontinuada']."',
					garantia            ='".$_POST['garantia']."'
				WHERE
					ctrid = ".$_REQUEST['ctrid'];

            $db->executar($sql);
            $db->commit();

            $ctrid = $_REQUEST['ctrid'];
        }

 	$erro = 0;
 	
	if ($ctrid){
		salvarEmpenhoContrato($ctrid, $_REQUEST['acao']);
		salvarSetorResponsavel($ctrid);
		
		//se ainda não vinculou o processo e existe um número de processo, deve-se vincular o processo ao protocolo
		if (!$_SESSION['prtid'] && $ctrproccontr) {
		
			$prtid = verificaProcessoProtocolo($ctrproccontr);
			//encontrou o num do processo
			if ($prtid) {
				vincularTermo($prtid);
				if (!$_SESSION['prtid']) {
					alert('Falha ao vincular o processo');
					$erro = 1;
				}
			}
		}
	}

	if ($erro == 0) {
		$_REQUEST['acao'] = "A";
		
		$db->sucesso('principal/cadContrato', "&ctrid=".$ctrid);
		die;
	}
} 

$acesso 		= verificaPermissaoTelaUsuario();
$desabilitado   = $acesso['desabilitado'];
$somenteLeitura = $acesso['leitura'];
$perfis 		= arrayPerfil();

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( $titulo_modulo, obrigatorio() . ' Indica campo obrigatório',false );

if($_SESSION['ctrid']){
	montaCabecalhoContrato( $_SESSION['ctrid'] );
}
?> 
<script src="../includes/prototype.js"></script>
<script src="../includes/calendario.js"></script> 

    <form name="formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <table class="tabela notprint" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td  class ="SubTituloDireita" align="right" width="25%">Tipo: </td> 
                <td> 
                 <?
                 $sql = "SELECT 
                            tpcid AS codigo, 
                            tpcdsc AS descricao
                        FROM
                            contratos.cttipocontrato order by codigo";
                 
                 $tpcid = ($rsDadosContrato[0]['tpcid'] ? $rsDadosContrato[0]['tpcid'] : $_POST['tpcid']);
                 
				 $db->monta_combo('tpcid', $sql, (($desabilitado)?'N':'S'), "Selecione...", '', '', '', '200', 'S', 'tpcid',false, $tpcid,'Tipo');
                 ?>
                </td>
            </tr>
            
            <tr>
                <td class ="SubTituloDireita" align="right">Número:</td>
                <td>
                 <?
                 $ctrnum = ($rsDadosContrato[0]['ctrnum'] ? $rsDadosContrato[0]['ctrnum'] : $_POST['ctrnum']);                               
                 ?>          
                 <?= campo_texto('ctrnum', 'S', $somenteLeitura, 'Número do Contrato', 5, 8, '########', '', 'right', '',  0, 'id="ctrnum" onblur="MouseBlur(this);"', '', $ctrnum); ?>
                    <input type="button" value="Gerar" style="cursor: pointer;" id="gerarNumContrato"/>
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right">Contrato continuado?</td>
                <td>
                    <?
                    $maoN = "";
                    $maoS = "";
                    if ($rsDadosContrato[0]["ctrcontinuada"] == 'S')
                        $maoS = "checked";
                    if ($rsDadosContrato[0]["ctrcontinuada"] == 'N')
                        $maoN = "checked";

                    ?>
                    <input type="radio" name="continuado" value="N" <? echo $maoN ?> /> Não
                    <input type="radio" name="continuado" value="S" <? echo $maoS ?> /> Sim
                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Número:
                    <?
                    $ctrnumcontinuada = ($rsDadosContrato[0]['numcontinuada'] ? $rsDadosContrato[0]['numcontinuada'] : $_POST['ctrnumcontinuada']);
                    ?>
                    <?= campo_texto('ctrnumcontinuada', 'S', $somenteLeitura, 'Número do Contrato', 5, 8, '########', '', 'right', '',  0, 'id="ctrnum" onblur="MouseBlur(this);"', '', $ctrnum); ?>
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right">Ano:</td>
                <td>

                    <?php
                    $entidempresa = ($rsDadosContrato[0]["entidcontratada"] ? $rsDadosContrato[0]["entidcontratada"] : $_POST['entidempresa']);
                    if( $entidempresa ){

                        $sql = "select
										ent.entnumcpfcnpj as cnpj
									from entidade.entidade ent
									left join entidade.funcaoentidade fue on ent.entid = fue.entid
									where
										ent.entid = " . $entidempresa;

                        $entcnpjempresa = $db->pegaUm($sql);
                    }

                    $somenteLeituraAno 	   = $somenteLeitura;
                    $somenteLeituraEmpresa = $somenteLeitura;

                    if( $rsDadosContrato[0]["ctrid"] && $entcnpjempresa && $somenteLeitura == 'S' ){
                        $sql = "SELECT
										COUNT(*) AS total
									FROM
										contratos.empenhovinculocontrato e
									INNER JOIN contratos.empenho_siafi s ON s.epsid = e.epsid
									WHERE
										e.ctrid = {$rsDadosContrato[0]["ctrid"]} AND
										co_favorecido='{$entcnpjempresa}'
									ORDER BY
										1";

                        $existeEmpenho = $db->pegaUm($sql);

                        $somenteLeituraAno 	   = ( $existeEmpenho ? 'N' : 'S');
                        $somenteLeituraEmpresa = ( $existeEmpenho ? 'N' : 'S');
                    }

                    $anoini = (int) 2012;
                    $anofim = (int) date("Y");

                    for($i=$anoini; $i<=$anofim; $i++){
                        $sqlano .= " select '$i' as codigo, '$i' as descricao ";
                        if($i != $anofim) $sqlano .= " union ";
                    }
                    $sqlano .= " order by 1 ";

                    $ctrano = ($rsDadosContrato[0]['ctrano'] ? $rsDadosContrato[0]['ctrano'] : ($_POST['ctrano'] ? $_POST['ctrano'] : date("Y")));

                    $db->monta_combo( 'ctrano', $sqlano, $somenteLeituraAno, '--Selecione o Ano--','','','','','S','ctrano', '', $ctrano, 'Ano' );
                    ?>
                </td>

            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right">Dedicação exclusiva de mão de obra:</td>
                <td>
                    <?
                        $maoN = "";
                        $maoS = "";
                        if ($rsDadosContrato[0]["ctrmaodeobra"] == 'S')
                            $maoS = "checked";
                        if ($rsDadosContrato[0]["ctrmaodeobra"] == 'N')
                            $maoN = "checked";

                    ?>
                    <input type="radio" name="maodeobra" value="N" <? echo $maoN ?> /> Não
                    <input type="radio" name="maodeobra" value="S" <? echo $maoS ?> /> Sim
                </td>

            </tr>

            <tr>
            	<td class="SubTituloDireita" align="right">Razão Social da Contratada: </td>
            	<td align="left">
	            	<?php
	            	
	            	$entidempresa = ($rsDadosContrato[0]["entidcontratada"] ? $rsDadosContrato[0]["entidcontratada"] : $_POST['entidempresa']);
	            	
	            	if($entidempresa){
	            		
	            	 	$sql = "select 	 
	                			ent.entnome,
								ent.entnumcpfcnpj as cnpj 
							from entidade.entidade ent
							left join entidade.funcaoentidade fue on ent.entid = fue.entid
							where 1=1
							--fue.funid = 46
							and ent.entid = ".$entidempresa;
	            	 	$arrEmpresaContratada = $db->pegaLinha($sql);
	            	 	$entnomeempresa = $arrEmpresaContratada['entnome'];
	            	 	$entcnpjempresa = $arrEmpresaContratada['cnpj'];
	            	 	 
	            	}
            		?>
			  		<span id="entnomeempresa"><?=$entnomeempresa;?></span>
			  		<input type="hidden" name="entidempresa" id="entidempresa" value="<?=$entidempresa;?>" title="Razão Social da Contratada">
			  		<input type="button" name="pesquisar_entidade" value="Pesquisar" style="cursor: pointer;" onclick="inserirEmpresa(document.getElementById('entidempresa').value, '<?php echo $somenteLeituraEmpresa ?>');" <?php if($somenteLeitura=="N") echo "disabled"; ?>> <?php echo obrigatorio()?>
            	</td>           	
            </tr>
            <tr>
            	<td class="SubTituloDireita" align="right">CNPJ da Contratada: </td>
            	<td align="left"><span id="entcnpjempresa"><?php echo $entcnpjempresa ? mascara_global($entcnpjempresa, "##.###.###/####-##") : "" ?></span></td>           	
            </tr>
            <tr id="ug">
                <td class="SubTituloDireita" valign="top">UG:</td>
                <td align="left">
                    <select id="ungcod" style="display: none"></select>
            </tr>

            <tr>
                <td class="SubTituloDireita" valign="top">Responsável pelo contrato:</td>
                <td align="left">
                    <input type="button" value="Adicionar" style="cursor: pointer;" id="btnCadResponsavel" onclick="inserirResponsavel();"/>
                </td>
            </tr>
             <tr id="tr_demandante" style="display:<?php echo $hspid == CONTRATANTE_EBSERH ? "" : "none"?>" >
            	<td class="SubTituloDireita" align="right" valign="top">Área Demandante: </td>
            	<td align="left">
            		<?php 
            		$sql = "select 
								estid as codigo, 
								estareasigla || ' - ' || estarea as descricao 
							from 
								corporativo.estrutura 
							where 
								estid in (3,4,5,6,7,8)
							and
								eststatus = 'A' ";
            		if($rsDadosContrato[0]["ctrid"]){
	            		$sqlCarregado = "select
											estid as dmtid
										from
											contratos.ctcontrato
										where
											ctrid = {$rsDadosContrato[0]["ctrid"]} ";
	            		$dmtid = $db->pegaUm($sqlCarregado);
	            		//dbg($dmtid,1);
	            	}
	            	$dmtid = ($dmtid ? $dmtid : $_POST['dmtid']);
	            	 
	            	$arrAtributos = false;
	            	$arrAtributos['name'] 			= "dmtid";
	            	$arrAtributos['id'] 			= "dmtid";
	            	$arrAtributos['title'] 			= "Demandante";
	            	$arrAtributos['sql'] 			= $sql;
	            	$arrAtributos['obrigatorio']	= false;
	            	$arrAtributos['habilitado'] 	= (($desabilitado)?false:true);
	            	$arrAtributos['size'] 			= "400";
	            	$arrAtributos['value'] 			= $dmtid;
	            	//$db->monta_combo($arrAtributos);	            	
            		?>
            	</td>           	
            </tr>
            <tr id="tr_diretoria_ebserh" style="display:<?php echo $hspid == CONTRATANTE_EBSERH ? "" : "none"?>" >
            	<td class="SubTituloDireita" align="right" valign="top">Interessado: </td>
            	<td align="left">
            		<?php 
            		$sql = "select 
								estid as codigo, 
								estareasigla || ' - ' || estarea as descricao 
							from 
								corporativo.estrutura 
							where 
								estid in (3,4,5,6,7,8)
							and
								eststatus = 'A' ";

            		if($rsDadosContrato[0]["ctrid"] && $hspid == CONTRATANTE_EBSERH) {
	            		$sqlCarregado = "select
											est.estid as codigo,
											estareasigla || ' - ' || estarea as descricao
										from
											corporativo.estrutura est
										inner join
												contratos.contratosetorresponsavel csr ON est.estid = csr.estid
										where
											ctrid = {$rsDadosContrato[0]["ctrid"]}
										and
											eststatus = 'A' ";

	            		$estid = $db->carregar($sqlCarregado);
	            	}
	            	
	            	//combo_popup( "estid", $sql, "Diretoria(s)", '400x400', null,array(), '', (($desabilitado)?'N':'S'), false,false, 10, 400 , null, null, false,null);
            		?>
            	</td>           	
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Arquivo Contrato: </td>
                <td> 
                <?php
				$sql = "SELECT 
	                        '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/cadContratoAnexo&acao=A&download=S&arqid=' || arq.arqid || '\';\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>'
	                     FROM
	                        ((public.arquivo arq INNER JOIN contratos.ctanexo aqb
	                        ON arq.arqid = aqb.arqid) INNER JOIN contratos.tipoanexo tarq
	                        ON tarq.tpaid = aqb.tpaid) INNER JOIN seguranca.usuario usu
	                        ON usu.usucpf = arq.usucpf
	                    WHERE
	                        arq.arqstatus = 'A'
	                    AND tarq.tpaid = ".ID_CONTRATO." and
	                    aqb.ctrid = {$_SESSION['ctrid']}
	                    LIMIT 1";

				if($_SESSION['ctrid']) {
                    $termo = $db->pegaUm( $sql );

                    if( $termo ){
                        echo $termo;
                    }else{
                        echo "Contrato não anexado";
                    }

				} else {
                    echo '<input type="file" name="anexarContrato">';
                }

				?> 
                </td> 
            </tr>	
			<tr id="tr_empenho_contrato" style="display:<?php echo $entcnpjempresa ? "" : "none"?>" >
				<?php
					if($entcnpjempresa){
						//FiltraEmpenhoPorCNPJ($entcnpjempresa,$rsDadosContrato[0]["ctrid"],$rsDadosContrato[0]["ungcod"]);
						FiltraEmpenhoPorCNPJ2($entcnpjempresa,$rsDadosContrato[0]["ctrid"],$rsDadosContrato[0]["hspid"],$rsDadosContrato[0]["ctrano"], $desabilitado);
					}
				?>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Conta Contábil:</td>
                <td>
                 <?
                 $ctrcntcontabil = ($rsDadosContrato[0]['ctrcntcontabil'] ? $rsDadosContrato[0]['ctrcntcontabil'] : $_POST['ctrcntcontabil']);                               
                 ?>          
                 <?= campo_texto('ctrcntcontabil', 'N', $somenteLeitura, 'Número do Contrato', 15, 10, '', '', '', '',  '', '', '', $ctrcntcontabil); ?>
                </td>
            </tr>
			<script	type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
				<script type="text/javascript">
					jQuery.noConflict();
					
					function confirmarVinculoProtocolo() {
						var codProcesso = document.getElementsByName("ctrproccontr")[0].value;
						if (codProcesso == '') {
							alert('Favor informe o número do processo que deseja vincular!');
						} else {
							jQuery.ajax({
								url:        'contratos.php?modulo=principal/cadContrato&acao=A&ctrid='+<?= $_SESSION['ctrid']; ?>,
								type:       'post',
								dataType:   'json',
								data:       'requisicao=verificarProcesso&codProcesso='+codProcesso,
								success: function(response){
									var prtid = response.prtid
									if (prtid) {
										if(confirm('Deseja realmente vincular esse Processo?')){
											jQuery.ajax({
												url:        'contratos.php?modulo=principal/cadContrato&acao=A&ctrid='+<?= $_SESSION['ctrid']; ?>,
												type:       'post',
												dataType:   'json',
												data:       'requisicao=vincularProcesso&prtid='+prtid,
												success	: function(e){
														if(e == '1'){
																alert('Operação realizada com sucesso.');
																window.location.reload();
														}else{
																alert('Falha ao vincular o processo');
														}
												}
											});
										}
									} else {
										alert('Número do processo não encontrado.');
									}
								}
							});
						}

					}
				</script>
            <tr>
                <td class ="SubTituloDireita" align="right" colspan="1">Data da Assinatura:</td>
                <td>
                <?php 
                $ctrdtassinatura = $rsDadosContrato[0]["ctrdtassinatura"];   
                ?>
                <?= campo_data2( 'ctrdtassinatura','S', (($desabilitado)?'N':'S'), 'Data da assinatura', 'S','','',$ctrdtassinatura ); ?>
                
                </td>
            </tr>  
            <tr>
                <td class ="SubTituloDireita" align="right" colspan="1">Data da publicação no DOU:</td>
                <td>
                <?php 
                $ctrdtpublicacao = $rsDadosContrato[0]["ctrdtpublicacao"];   
                ?>
                <?= campo_data2( 'ctrdtpublicacao','N', (($desabilitado)?'N':'S'), 'Data da publicação', 'S','','',$ctrdtpublicacao ); ?>
                
                </td>
            </tr>           
			<tr id="numeroProtocolo">            
            <?php if ($dado['prtnumeroprotocolo']) : ?>
				<td class="SubTituloDireita" align="right"><b> Número do Processo </b></td>
				<td>
					<?php 
						if (strlen($dado['prtnumeroprotocolo']) == 11)
							echo mascara_global($dado['prtnumeroprotocolo'].$dado['prtano'].$dado['prtdigitoverificador'].$dado['prtdigitoverificador2'],"#####.######/####-##");
						else
							echo mascara_global($dado['prtnumeroprotocolo'].$dado['prtano'].$dado['prtdigitoverificador'].$dado['prtdigitoverificador2'],"######/####-##"); 
					?>
				</td>
            <?php else : ?>
                <td class ="SubTituloDireita" align="right">Vincular Processo: </td> 
					<td><b> N°: </b>
		                 <?
						 $ctrproccontr = ($rsDadosContrato[0]['ctrproccontr'] ? $rsDadosContrato[0]['ctrproccontr'] : "");
		                 ?>
		            	 <?= campo_texto('ctrproccontr', 'S', $somenteLeitura, 'Número do Processo de Contratação', 30, 50, '#####.######/####-##', '', 'left', '',  0, 'id="ctrproccontr" onblur="MouseBlur(this);"' ); ?>
		            	 <?php if($_REQUEST['acao'] == 'A'): ?>
							<input type="button" value="Vincular" onclick="confirmarVinculoProtocolo()" style="cursor: pointer;"/>
						<?php  endif; ?>
					</td>
            <?php endif; ?>
			</tr>
							
            <tr>
                <td class ="SubTituloDireita" align="right">Modalidade:</td>
                <td>
              	 <?
				 $sql = "SELECT 
                            modid AS codigo, 
                            moddsc AS descricao
                        FROM
                            contratos.ctmodalidadecontrato order by codigo";
				 $modid = ($rsDadosContrato[0]['modid'] ? $rsDadosContrato[0]['modid'] : $_POST['modid']);
				 
				 $db->monta_combo('modid', $sql, (($desabilitado)?'N':'S'), "Selecione...", 'carregarDescricaoModalidade', '', '', '200', 'S', 'modid',false,$modid,'Modalidade');
            	 ?> 
 				</td>
			</tr>
			
			<tr id="tr_modexigedsc" style="display: none;">
                <td class ="SubTituloDireita" align="right">Complemento da Modalidade:</td>
                <td>
                	<?
	            	 $modexigedsc = ($rsDadosContrato[0]['modexigedsc'] ? $rsDadosContrato[0]['modexigedsc'] : $_POST['modexigedsc']);
	            	 
	            	?>
              		<?= campo_texto('modexigedsc', 'S', $somenteLeitura, 'Complemento da Modalidade', 45, 255, '', '', 'left', '',  0, 'id="modexigedsc" onblur="MouseBlur(this);"' ); ?>
 				</td>
			</tr>
			
            <tr>
                <td class ="SubTituloDireita" align="right">Número da Licitação: </td>
                <td>
            	 <?
            	 $ctrnummod = ($rsDadosContrato[0]['ctrnummod'] ? $rsDadosContrato[0]['ctrnummod'] : $_POST['ctrnummod']);
            	 ?>
            	 <?= campo_texto('ctrnummod', 'N', $somenteLeitura, 'Número Modalidade Contratação', 20, 50, '', '', 'left', '',  0, 'id="ctrnummod" onblur="MouseBlur(this);"' ); ?>
 				</td>
			</tr>
		           		                      
           	<tr>
                <td class ="SubTituloDireita" align="right">Processo Execução Financeira Nº: </td> 
                <td> 
                 <?
				 $ctrprocexecfin = ($rsDadosContrato[0]['ctrprocexecfin'] ? $rsDadosContrato[0]['ctrprocexecfin'] : $_POST['ctrprocexecfin']);
                 ?>
            	 <?= campo_texto('ctrprocexecfin', 'N', $somenteLeitura, 'Número do Processo de Execução Financeira', 30, 50, '', '', 'left', '',  0, 'id="ctrprocexecfin" onblur="MouseBlur(this);"' ); ?>
            	 
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right">Processo Execução do Contrato Nº:</td>
                <td>
            	 <?
            	 $ctrprocexecctr = ($rsDadosContrato[0]['ctrprocexecctr'] ? $rsDadosContrato[0]['ctrprocexecctr'] : $_POST['ctrprocexecctr']);
            	  ?>
            	  <?= campo_texto('ctrprocexecctr', 'N', $somenteLeitura, 'Número do Processo de Execução do Contrato', 80, 200, '', '', 'left', '',  0, 'id="ctrprocexecctr" onblur="MouseBlur(this);"' ); ?>            	 
            	         
 				</td>
			</tr>           
         
          	 
            <tr>
	      		<td class="subtitulodireita" valign="top"> 	
	      	 		Descrição do Objeto do Contrato:
	      	 	</td>
	      	 	<td align="left">
	      	 	<?php
				$ctrobj = ($rsDadosContrato[0]["ctrobj"] ? $rsDadosContrato[0]["ctrobj"] : $_POST['ctrobj']);
				
	      	 	?>
	      	 		<div style="float:left">
	      	 		<!--  
		      	 	<textarea name="ctrobj" title="Descrição do Objeto" rows="15" cols="80" class="text_editor_simple"><?= $ctrobj ?></textarea>
		      	 	-->
		      	 	<textarea name="ctrobj" title="Descrição do Objeto" rows="15" cols="80" <?=(($desabilitado)?'disabled':'')?>><?= $ctrobj ?></textarea>
		      	 	</div>
		      	 	<div style="float:left">
		      	 	&nbsp;<?php echo obrigatorio();?>
		      	 	</div>
		      	 </td>
	        </tr>
            <tr>
	      		<td class="subtitulodireita" valign="top"> 	
	      	 		Justificativa:
	      	 	</td>
	      	 	<td align="left">
	      	 	<?php
				$ctrjus = ($rsDadosContrato[0]["ctrjus"] ? $rsDadosContrato[0]["ctrjus"] : $_POST['ctrjus']);
	      	 	?>
	      	 		<div style="float:left">
		      	 	<textarea name="ctrjus" title="Justificativa do Contrato" rows="10" cols="80" <?=(($desabilitado)?'disabled':'')?>><?= $ctrjus ?></textarea>
		      	 	</div>
		      	 	<div style="float:left">
 	 		    	 	&nbsp;<?php echo obrigatorio();?>
		      	 	</div>
		      	 </td>
	        </tr>
            <tr>
	      		<td class="subtitulodireita" valign="top"> 	
	      	 		Observação:
	      	 	</td>
	      	 	<td align="left">
	      	 	<?php
				$ctrobs = ($rsDadosContrato[0]["ctrobs"] ? $rsDadosContrato[0]["ctrobs"] : $_POST['ctrobs']);
	      	 	?>
	      	 		<div style="float:left">
		      	 	<textarea name="ctrobs" title="Observação do Contrato" rows="10" cols="80" <?=(($desabilitado)?'disabled':'')?>><?= $ctrobs ?></textarea>
		      	 	</div>
		      	 	<div style="float:left">
 	 		    	 	&nbsp;<?php echo obrigatorio();?>
		      	 	</div>
		      	 </td>
	        </tr>
	        
	        
	        <tr>
                <td class ="SubTituloDireita" align="right" colspan="1">Valores de Referência:</td>
                <td>
                <?php 
                $_POST['ctrvlrreferencia'] = str_replace(',','.',str_replace('.','',$_REQUEST['ctrvlrreferencia']));
                $ctrvlrreferencia  = ($rsDadosContrato[0]["ctrvlrreferencia"] ? $rsDadosContrato[0]["ctrvlrreferencia"] : $_POST['ctrvlrreferencia']);
                if($ctrvlrreferencia) $ctrvlrreferencia = number_format($ctrvlrreferencia,2,",",".");
                ?>
                <?= campo_texto( 'ctrvlrreferencia', 'N', $somenteLeitura, '', 17, 15, '###.###.###,##', '', 'right', '', 0, '','', $ctrvlrreferencia); ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right" colspan="1">Valor Homologado Após Negociação:</td>
                <td>
                <?php 
                $_POST['ctrvlrhomologado'] = str_replace(',','.',str_replace('.','',$_REQUEST['ctrvlrhomologado']));
                $ctrvlrhomologado  = ($rsDadosContrato[0]["ctrvlrhomologado"] ? $rsDadosContrato[0]["ctrvlrhomologado"] : $_POST['ctrvlrhomologado']);
                if($ctrvlrhomologado) $ctrvlrhomologado = number_format($ctrvlrhomologado,2,",",".");
                ?>
                <?= campo_texto( 'ctrvlrhomologado', 'N', $somenteLeitura, '', 17, 15, '###.###.###,##', '', 'right', '', 0, '', '', $ctrvlrhomologado); ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right" colspan="1">Valor de Referência Apenas dos Itens Homologados:</td>
                <td>
                <?php
                $_POST['ctrvlrrefhomolog'] = str_replace(',','.',str_replace('.','',$_REQUEST['ctrvlrrefhomolog']));
                $ctrvlrrefhomolog  = ($rsDadosContrato[0]["ctrvlrrefhomolog"] ? $rsDadosContrato[0]["ctrvlrrefhomolog"] : $_POST['ctrvlrrefhomolog']);
                if($ctrvlrrefhomolog) $ctrvlrrefhomolog = number_format($ctrvlrrefhomolog,2,",",".");
                ?>
                <?= campo_texto( 'ctrvlrrefhomolog', 'N', $somenteLeitura, '', 17, 15, '###.###.###,##', '', 'right', '', 0, '','',$ctrvlrrefhomolog); ?>
                </td>
            </tr>
            
            <tr>
                <td class ="SubTituloDireita" align="right" colspan="1">Valor Inicial Contrato:</td>
                <td>
                <?php 
                $_POST['ctrvlrinicial'] = str_replace(',','.',str_replace('.','',$_REQUEST['ctrvlrinicial']));
                $ctrvlrinicial  = ($rsDadosContrato[0]["ctrvlrinicial"] ? $rsDadosContrato[0]["ctrvlrinicial"] : $_POST['ctrvlrinicial']);
                if($ctrvlrinicial) $ctrvlrinicial = number_format($ctrvlrinicial,2,",",".");
                ?>
                <?= campo_texto( 'ctrvlrinicial', 'N', $somenteLeitura, '', 17, 15, '###.###.###,##', '', 'right', '', 0, '','',$ctrvlrinicial); ?>
                </td>
            </tr>
			<tr>
				<td class="SubTituloDireita" valign="top">Alíquota de Retenção:</td>
				<td valign="top">
				<?php
				/*********
				 * Demonstrando um campo popup com dimensões da popup definidas, tanhamo do campo texto, filtro do popup e tipo de campo (texto ou combo)
				*
				* A função abaixo possui documentação explicando seus parâmetros.
				* 
				*********/
				if($rsDadosContrato[0]['retid']){
					$sql = "SELECT
								retid as codigo,
								(retcod||' - '||retres ||'-'||retalicota ) AS descricao
								FROM
								contratos.ctretencaocontrato
								WHERE
								retid={$rsDadosContrato[0]['retid']}";
					$retid = $db->pegaLinha($sql);
				}
					//dbg($retid,d);
				$sql = 	"SELECT
						  retid as codigo,
						(retcod||' - '||retres ||'-'||retalicota ) AS descricao 
						FROM
							contratos.ctretencaocontrato
	
						ORDER BY
							retres";
					
				$filtro = array(
								array("codigo"    => "retid",
										"descricao" => "Código da receita",
										"numeric" => "1"),
								array("codigo" 	  => "retres",
										"descricao" => "Resumo",
										"numeric" => "0")
								);
				
				campo_popup('retid',$sql,'Selecione uma Alíquota','','400x600','60', $filtro, 1);
						
				?>
				</td>
			</tr>
            <tr>
                <td class ="SubTituloDireita" align="right" colspan="1">Observação Valor Inicial:</td>
				<td>
					 <?php 
                	$ctrobsvlrini  = ($rsDadosContrato[0]["ctrobsvlrini"] ? $rsDadosContrato[0]["ctrobsvlrini"] : $_POST['ctrobsvlrini']);
                	?>
				
					<input type="radio" name="ctrobsvlrini" value="E" <?if($ctrobsvlrini == 'E') echo 'checked';?> <?=(($desabilitado)?'disabled':'')?>>Estimado 
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<input type="radio" name="ctrobsvlrini" value="F" <?if($ctrobsvlrini == 'F') echo 'checked';?> <?=(($desabilitado)?'disabled':'')?>>Fixo 
					
				</td>

            </tr>            
            <!-- 
            <tr>
                <td class ="SubTituloDireita" align="right" colspan="1">Valor Atual Mensal:</td>
                <td>
                           
                <?php 
                $ctrvlrmensal  = ($rsDadosContrato[0]["ctrvlrmensal"] ? $rsDadosContrato[0]["ctrvlrmensal"] : $_POST['ctrvlrmensal']);
                if($ctrvlrmensal) $ctrvlrmensal = number_format($ctrvlrmensal,2,",",".");
                ?>
                <?= campo_texto( 'ctrvlrmensal', 'N', $somenteLeitura, '', 17, 15, '###.###.###,##', '', 'right', '', 0, '','',$ctrvlrmensal); ?>
                
                <?//= campo_texto('ctrvlrmensal', 'S', $somenteLeitura, 'Valor Mensal do Contrato', 30, 200, '', '', 'left', '',  0, 'id="ctrvlrmensal" onblur="MouseBlur(this);"' ); ?>
                
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right" colspan="1">Valor Atual Anual:</td>
                <td>
                <?php 
                $ctrvlrtotal  = ($rsDadosContrato[0]["ctrvlrtotal"] ? $rsDadosContrato[0]["ctrvlrtotal"] : $_POST['ctrvlrtotal']);
                if($ctrvlrtotal) $ctrvlrtotal = number_format($ctrvlrtotal,2,",",".");
                ?>
                <?= campo_texto( 'ctrvlrtotal', 'N', $somenteLeitura, '', 17, 15, '###.###.###,##', '', 'right', '', 0, '','',$ctrvlrtotal); ?>
                
                <?//= campo_texto('ctrvlrtotal', 'S', $somenteLeitura, 'Valor Anual do Contrato', 30, 200, '', '', 'left', '',  0, 'id="ctrvlrtotal" onblur="MouseBlur(this);"' ); ?>
     
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right" colspan="1">Valor Atual Global:</td>
                <td>
                 <?php 
                $ctrvlrglobal  = ($rsDadosContrato[0]["ctrvlrglobal"] ? $rsDadosContrato[0]["ctrvlrglobal"] : $_POST['ctrvlrglobal']);
                if($ctrvlrglobal) $ctrvlrglobal = number_format($ctrvlrglobal,2,",",".");
                ?>
                <?= campo_texto( 'ctrvlrglobal', 'N', $somenteLeitura, '', 17, 15, '###.###.###,##', '', 'right', '', 0, '', '', $ctrvlrglobal); ?>
                
                <?//= campo_texto('ctrvlrglobal', 'S', $somenteLeitura, 'Valor Global do Contrato', 30, 200, '', '', 'left', '',  0, 'id="ctrvlrglobal" onblur="MouseBlur(this);"' ); ?>
                
                </td>
            </tr>
             
            <tr>
                <td class ="SubTituloDireita" align="right" colspan="1">Observação valor Atual:</td>
				<td>
					 <?php 
                	$ctrobsvlr  = ($rsDadosContrato[0]["ctrobsvlr"] ? $rsDadosContrato[0]["ctrobsvlr"] : $_POST['ctrobsvlr']);
                	?>
					<input type="radio" name="ctrobsvlr" value="E" <?if($ctrobsvlr == 'E') echo 'checked';?>>Estimado 
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<input type="radio" name="ctrobsvlr" value="F" <?if($ctrobsvlr == 'F') echo 'checked';?>>Fixo 
					
				</td>
            </tr>
            -->
            <tr>
                <td class ="SubTituloDireita" align="right" colspan="1">Previsão de início da Vigência:</td>
                <td>
                <?php 
                $ctrdtiniciovig = $rsDadosContrato[0]["ctrdtiniciovig"];   
                ?>
                <?= campo_data2( 'ctrdtiniciovig','S', (($desabilitado)?'N':'S'), 'Início da Vigência', 'S','','',$ctrdtiniciovig ); ?>
                
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right" colspan="1">Previsão de término da Vigência:</td>
                <td>
                <?php 
                $ctrdtfimvig = $rsDadosContrato[0]["ctrdtfimvig"];   
                ?>
                <?= campo_data2( 'ctrdtfimvig','S', (($desabilitado)?'N':'S'), 'Término da Vigência', 'S','','',$ctrdtfimvig ); ?>
                
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right" colspan="1">Possui garantia:</td>
                <td>
                    <input type="radio" name="garantia" value="N" <? echo $maoN ?> /> Não
                    <input type="radio" name="garantia" value="S" <? echo $maoS ?> /> Sim
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right" colspan="1">Observação Vigência:</td>
                <td>              
                <?
                 $sql = "SELECT 
                            obvid AS codigo, 
                            obvdsc AS descricao
                        FROM
                            contratos.ctobsvigencia
                            order by codigo";
				 $obvid = ($rsDadosContrato[0]['obvid'] ? $rsDadosContrato[0]['obvid'] : $_POST['obvid']);
				 
				 $db->monta_combo('obvid', $sql, (($desabilitado)?'N':'S'), "Selecione...", '', '', '', '400', 'S', 'obvid',false,$obvid,'Observação Vigência');
                 ?>
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right">Situação Vigência:</td>
                <td>
              	 <?
				 $sql = "SELECT 
                            sitid AS codigo, 
                            sitdsc AS descricao
                        FROM
                            contratos.ctsituacaocontrato
						where tpcstatus = 'A' order by codigo";
				
				 $sitid = ($rsDadosContrato[0]['sitid'] ? $rsDadosContrato[0]['sitid'] : $_POST['sitid']);
				 
				 $db->monta_combo('sitid', $sql, (($desabilitado)?'N':'S'), "Selecione...", '', '', '', '200', 'S', 'sitid',false,$sitid,'Situação do Contrato');
            	 ?> 
 				</td>
			</tr>
            <tr>
                <td class ="SubTituloDireita" align="right">N° página parecer jurídico sobre contrato:</td>
                <td>
                <?= 
                campo_texto( 'ctrprcjurctr', 'N', $somenteLeitura, '', 17, 15, '', '', '', '', 0, '', '', (($rsDadosContrato[0]["ctrprcjurctr"] ? $rsDadosContrato[0]["ctrprcjurctr"] : $_POST['ctrprcjurctr']))); ?>
                
 				</td>
			</tr>			
            <tr>
                <td class ="SubTituloDireita" align="right">Dados do Parecer Jurídico:</td>
                <td>
                <?=
                 campo_texto( 'ctrparjuridico', 'S', $somenteLeitura, '', 30, 30, '', '', '', '', 0, '', '', (($rsDadosContrato[0]["ctrparjuridico"] ? $rsDadosContrato[0]["ctrparjuridico"] : $_POST['ctrparjuridico']))); ?>
                
 				</td>
			</tr>			
           
            <tr>
            	<td  class ="SubTituloDireita" align="right"></td>
            	 <td>
            	   <input type="hidden" name="action" value="1" id="action">
            	   <input type="hidden" name="ctrid" value="<?=$_SESSION['ctrid']; ?>" id="ctrid">
            	   <input type="hidden" name="prtid" value="<?=$_SESSION['prtid']; ?>" id="prtid">
            	   <? if( !$desabilitado || array_intersect($acesso['pflcodComResponsabilidade'], array(PERFIL_TRIAGEM, PERFIL_GESTOR_FINANCEIRO_UNIDADE)) ) : ?>
            	   <input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="enviaForm();">
            	   <? 	if($_REQUEST['acao'] == 'A' && !$desabilitado){ //dbg($desabilitado);?>
            	   		<input type="button" value="Incluir Novo" onclick="window.location.href='contratos.php?modulo=principal/cadContrato&acao=I';" /><? } ?>
            	   <? 	endif; ?>  
            	   <? if($_REQUEST['acao'] == 'I'){ ?><input type="button" value="Voltar" onclick="window.location.href='contratos.php?modulo=principal/inicioContrato&acao=A';" /><? } ?>
            	   
            	   <? if($_REQUEST['acao'] == 'A'){ ?><input type="button" name="btnImprimir" value="Imprimir" id="Imprimir" onclick="popupimpressao();" align="right"><? } ?> 
            	  </td>
            </tr>
      </table>
      </form>     

      
 <!--  versao de impressão -->
 <?php 
 function imprimirContrato($ctrid){
?>		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/><?php

	//Chamada de programa
	//include  APPRAIZ."includes/cabecalho.inc";
     
	echo monta_cabecalho_relatorio( '100' );

	 global $db;
	 $sql = "SELECT
             ent.entnumcpfcnpj as cnpj,
             ctr.ctrdtpublicacao,
             ctr.ctrproccontr,
			 ent.entnome as contratada,
			 tpc.tpcdsc || ' Nº ' ||  ctr.ctrnum || ' / ' || ctr.ctrano as numcontrato,
			 mod.moddsc as moddsc,
			 ctr.ctrobj as ctrobj,
			 case
			 when ctrvlrtotal is not null then ctrvlrtotal
			 else ctrvlrinicial
			 end as valor_total,
 			( SELECT SUM(a.total) FROM (
				 SELECT
				 		obs.valor AS total
 					FROM
 						contratos.ctcontrato c
					 JOIN entidade.entidade e ON e.entid = c.entidcontratada
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
			 WHERE ctr.ctrid = $ctrid";

 		$dados = $db->pegaLinha($sql);
 		
 		$sql = "SELECT
 		ct.ctrproccontr as protocolo,
 		ct.ctrobj as objeto,
 		CASE WHEN ct.ctrvlrtotal IS NOT NULL THEN
 		ct.ctrvlrtotal
 		ELSE
 		(ct.ctrvlrinicial)+(SELECT
 		COALESCE(SUM(tdavlr),0) as total
 		FROM
 		contratos.cttermoaditivo adi
 		WHERE
 		adi.tdastatus = 'A' AND	adi.ctrid =ct.ctrid)
 		END AS valorcontrato,
 		nu_empenho,
 		ctrprcjurctr,
 		ctrprcjuradt,
 		ctrproccontr,
 		ctrjus,
 		ct.ctrdtassinatura,
 		ct.ctrdtiniciovig,
 		ct.ctrdtfimvig
 		FROM
 		contratos.ctcontrato ct
 		NATURAL JOIN contratos.cttipocontrato tp
 		LEFT JOIN contratos.empenhovinculocontrato ec ON ct.ctrid=ec.ctrid
 		LEFT JOIN contratos.empenho_siafi es ON es.epsid =ec.epsid
 		LEFT JOIN territorios.municipio mun ON mun.muncod = h.muncod
 		
 		 
 		WHERE ct.ctrid=$ctrid";

 		$dados = array_merge($dados,$db->pegaLinha($sql));

 		//dbg($dados, d);

 ?>
     <table class="tabela" border="1 #f5f5f5" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td class ="SubTituloDireita" align="right" style="width:20px;">Empenho: </td> 
                <td  style="width:50px;"> 
                 <?
                 

                 echo trim($dados['nu_empenho']," ");
                 ?>
                </td>
                <td  class ="SubTituloDireita" align="right">Contratante:</td><td><?php echo $dados['contratante']?></td>
           
             </tr>
            <tr>
    			 <td  class ="SubTituloDireita" align="right">Contrato Nº:</td><td>
                <?php echo $dados['numcontrato'] ?></td>
                <td class ="SubTituloDireita" align="right">Processo:</td>
                <td style="width:80px;">
					<?php echo $dados['ctrproccontr']?>
                </td>
             </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Data Assinatura:</td>
                <td>
          		<?php echo formata_data($dados['ctrdtassinatura']) ?>	
                </td>
                <td class ="SubTituloDireita" align="right">Data publicação DOU:</td>
                <td>
          		<?php echo formata_data($dados['ctrdtpublicacao']) ?>
                </td>
             </tr>
             <tr>
                <td class ="SubTituloDireita" align="right">Objeto:</td>
                <td colspan=3>
          		<?php echo $dados['ctrobj'] ?>	
                </td>
             </tr>
             <tr>
                <td class ="SubTituloDireita" align="right">Fundamento Legal:</td>
                <td colspan=3>Lei nº 8.666/93 e Lei nº 8.078/90</td>
             </tr> 
             <tr>
                <td class ="SubTituloDireita" align="right">Modalidade:</td>
                <td colspan=3><?php echo $dados['moddsc'] ?>	</td>
             </tr> 
             <tr>
                <td class ="SubTituloDireita" align="right" style="width:20%">Justificativa:</td>
                <td style="width:80%" colspan=3>
          		<?php echo $dados['ctrjus'] ?>	
                </td>
             </tr>  
              <tr>
                <td class ="SubTituloDireita" align="right" style="width:20%">Nome da Contratada:</td>
                <td style="width:80%" colspan=3>
          		<?php echo $dados['contratada'] ?>	
                </td>
             </tr>  
             <tr>
                <td class ="SubTituloDireita" align="right" style="width:20%">CNPJ:</td>
                <td style="width:80%" colspan=3>
          		<?php echo $dados['cnpj'] ? mascara_global($dados['cnpj'], "##.###.###/####-##") : "" ?></td>
             </tr> 
             <tr>
                <td class ="SubTituloDireita" align="right">Vigência do Contrato:</td>
                <td colspan=4>
          		<?php echo "De ".formata_data($dados['ctrdtiniciovig'])." à ".formata_data($dados['ctrdtfimvig']) ?>	
                </td>
             </tr> 


             <?php
             	$sql = "SELECT 				 
							tda.tdanum,
							to_char(tda.tdadata::date,'YYYY-MM-dd') AS tdadata, 
							tda.tdaobj, 
							tda.tdavlr  as tdavlr,	
							tda.tdainiciovig AS tdainiciovig,
							tda.tdafimvig AS tdafimvig,
							tda.tiaid
						FROM 
							contratos.cttermoaditivo AS tda 
						WHERE
						  tda.ctrid = $ctrid";
             	$rsDadosProcesso = $db->carregar( $sql );
             	if(is_array($rsDadosProcesso)){
					echo "<tr><td class =\"SubTituloDireita\" align=\"right\">Aditivos:</td><td colspan=4>";
					$cont=1;
             		foreach($rsDadosProcesso as $aditivo){
						echo $cont."º aditivo - De ".formata_data($aditivo['tdainiciovig'])." à ".formata_data($aditivo['tdafimvig'])."<br>";
						$cont++;
					}
					echo "</td></tr>";
             	}
			?>
			 <tr>
                <td class ="SubTituloDireita" align="right">Valor Total:</td>
                <td>
          		<?php echo number_format($dados['valor_total'], 2, ',', '.') ?>	
                </td>
                <td class ="SubTituloDireita" align="right">Conta Contábil:</td>
                <td>
          		<?php echo $dados['contacontabil'] ?>	
                </td>
             </tr>
			<?php 
             	$sql = "select
							'".(($desabilitado)?"":"<img src=\"../imagens/alterar.gif\" class=\"link\" onclick=\"editarFiscal(' || ent.entid || ')\" /> <img src=\"../imagens/excluir.gif\" class=\"link\" onclick=\"removerFiscal(' || fsc.fscid || ')\" />")."' as acao,
							entnome,
							entnumcpfcnpj
							from
								contratos.fiscalcontrato fsc
							inner join
								entidade.entidade ent ON ent.entid = fsc.entid
							where
								ctrid = $ctrid
							and
						fsc.fscstatus = 'A'";
             	$rsDadosProcesso = $db->carregar( $sql );
             	if(is_array($rsDadosProcesso)){
					echo "<tr><td class =\"SubTituloDireita\" align=\"right\">Fiscais:</td><td colspan=4>";
             		foreach($rsDadosProcesso as $fiscais){
						echo $fiscais['entnome']." - ".$fiscais['entnumcpfcnpj']."<br>";

					}
					echo "</td></tr>";
             	}else 
             		echo "<tr><td class =\"SubTituloDireita\" align=\"right\">Fiscais:</td><td colspan=4>Nenhum fiscal cadastrado.</td></tr>";
             
             ?>
            <tr>
                <td class ="SubTituloDireita" align="right">Gestores:</td>
                <td colspan=4></td>
            </tr>
              <tr>
                <td class ="SubTituloDireita" align="right" >Nº Pág. parecer jurídico</td>
                <td >Contrato:<?php echo $dados['ctrprcjurctr'] ?>	
                <td colspan=2>Primeiro termo aditivo:<?php echo $dados['ctrprcjuradt'] ?></td>
			 </tr>
			 <tr class="notprint"><td><input type="button" name="btnImprimir" value="Imprimir" id="Imprimir" onclick="window.print();" align="right"></td>
            </tr>
      </table>
<?php } ?>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript">

function removecabecalhoimpressao(){
	jQuery('.tabela').first().addClass('notprint');
	jQuery('.tabela:eq(1)').addClass('notprint');
}
function popupimpressao()
{
	var popUp = window.open('/contratos/contratos.php?modulo=principal/cadContrato&acao=A&req=popupimpressao', 'popupImpressao', 'height=650,width=900,scrollbars=yes,top=50,left=200');
	popUp.focus();
}

function enviaForm(){
 	var nomeform 		= 'formulario';
	var submeterForm 	= true;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();

	campos[0] 			= "tpcid";
	campos[1] 			= "ctrnum";
	campos[2]			= "ctrano"; 
	campos[3]			= "entidempresa"; 
	campos[4]			= "ctrproccontr";
	campos[5]			= "modid";
	campos[6]			= "ctrobj";
	campos[7]			= "ctrobs";	
	campos[8]			= "ctrnumcron";
	campos[9]			= "ctrdtiniciovig";		
	campos[10]			= "ctrdtfimvig";
	campos[11]			= "obvid";	
	campos[12]			= "sitid";	
	campos[13]			= "ctrdtassinatura";
    campos[14]          = "ctrmaodeobra";
	

	if(jQuery('#tr_modexigedsc').css('display') != 'none'){
		campos[13]			= "modexigedsc";
	}
						 
	tiposDeCampos[0] 	= "select";
	tiposDeCampos[1] 	= "numero"; 
	tiposDeCampos[2] 	= "select";
	tiposDeCampos[3] 	= "texto"; 
	tiposDeCampos[4] 	= "texto";
	tiposDeCampos[5] 	= "select"; 
	tiposDeCampos[6] 	= "select";
	tiposDeCampos[7] 	= "textarea";
	tiposDeCampos[8] 	= "textarea";	 
	tiposDeCampos[9] 	= "texto"; 
	tiposDeCampos[10] 	= "texto";
	tiposDeCampos[11] 	= "texto";
	tiposDeCampos[12] 	= "select";
	tiposDeCampos[13] 	= "select";
	tiposDeCampos[14] 	= "data";
    tiposDeCampos[15]   = "radio";

	if(jQuery('#tr_modexigedsc').css('display') != 'none'){
		tiposDeCampos[13] 	= "texto";
	}

	if(jQuery('#tr_diretoria_ebserh').css('display') != 'none'){
		selectAllOptions( formulario.estid );
	}
	if(jQuery('#tr_empenho_contrato').css('display') != 'none'){
		selectAllOptions( formulario.numempenho );
	}
	
	/*
	if( !processaObjTextarea() ){
		return false;
	}
	*/
	
	validaForm(nomeform, campos, tiposDeCampos, submeterForm );
}
/*
function adicionarItem(){
	window.open('contratos.php?modulo=principal/popupItem&acao=A','','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=1000,height=500');
}

function removeVinculoItem(coiid){
	if(confirm('Deseja desvincular este item?')){
		window.location.href = "/evento/contratos.php?modulo=principal/cadProcesso&acao=A&coiidExcluir="+coiid;
		return true;
	} else {
		return false;
	}
}
*/
function processaObjTextarea(){ 
	var elemento = document.getElementById('ctrobj');
	var msg = '';	

	alert(elemento.value);
	return false;
	
	if( elemento.value == '' ){
		msg = 'O campo Descrição do Objeto do Contrato é obrigatório';
	}

	if(msg != ''){
		alert(msg);
		return false;
	}
	
	return true;
} 

var caminho_atual = "contratos.php";

function inserirEmpresa(entidempresa, disabled){
	if (entidempresa){
			var urlDisabled = (disabled == 'N' ? '&disabled=true' : '');
		 
			return windowOpen( caminho_atual + '?modulo=principal/inserir_empresa&acao=A&busca=entnumcpfcnpj&entid=' + entidempresa + urlDisabled,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}else{
			return windowOpen( caminho_atual + '?modulo=principal/inserir_empresa&acao=A','blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}
}

function inserirContratante(entid){
	if (entid){ 
			return windowOpen( caminho_atual + '?modulo=principal/popupContratante&acao=A&tpeid=2&entid=' + entidempresa,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}else{
			return windowOpen( caminho_atual + '?modulo=principal/popupContratante&acao=A&tpeid=2','blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}
}

function exibeInteressado(hspid)
{

    jQuery.ajax({
        url		: 'contarUG.php',
        type	: 'post',
        data: { hspid: jQuery('#hspid').val() }
    })
        .done(function( msg ) {

            vetor = JSON.parse(msg);

            if (vetor.length > 1) {
                jQuery("#ungcod").show();
                jQuery("#ungcod").empty();
                for (x = 0; x < vetor.length; x++) {
                    jQuery("#ungcod").append('<option value='+vetor[x].ungcod+'>'+vetor[x].ungdsc+'</option>');
                }

            } else {
                jQuery("#ungcod").hide();
            }
        });

    if(hspid == "<?php echo CONTRATANTE_EBSERH ?>"){
		jQuery("#tr_diretoria_ebserh").show();
	}else{
		jQuery("#tr_diretoria_ebserh").hide();
	}
}

function carregarDescricaoModalidade(modid)
{
	jQuery.ajax({
		url		: '/contratos/contratos.php?modulo=principal/cadContrato&acao=A',
		type	: 'post',
		data	: 'req=verificaModalidade&modid='+modid,
		success	: function(e){
			if(e=='t'){					
				jQuery("#tr_modexigedsc").show();
				jQuery("[name=modexigedsc]").attr('disabled', false).removeClass('disabled').addClass('normal obrigatorio').removeAttr('readonly');
			}else{
				jQuery("#tr_modexigedsc").hide();
				jQuery("[name=modexigedsc]").attr('disabled', true).removeClass('normal obrigatorio').addClass('disabled').attr('readonly', true);
			}
		}
	});		
}

var caminho_atual = "contratos.php";
function inserirResponsavel(){
     return windowOpen( caminho_atual + '?modulo=principal/inserir_responsavel&acao=A&campo=1','blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

jQuery(document).ready(function(){

    jQuery('#gerarNumContrato').click(function() {
        if (jQuery('#hspid').val() != '') {
            jQuery.ajax({
                type: "POST",
                url: "gerarNumContrato.php",
                data: { hspid: jQuery('#hspid').val(), ctrano: jQuery('#ctrano').val() }
            })
                .done(function( msg ) {
                    if (msg != '')
                        jQuery("#ctrnum").attr('value', msg);
                    else
                        jQuery("#ctrnum").attr('value', '');
                });
        } else {
            alert("Por favor selecione um contratante");
        }
    });

    jQuery('#gerarNumContrato').click(function() {
            jQuery.ajax({
                type: "POST",
                url: "gerarNumContrato.php",
                data: { ctrano: jQuery('#ctrano').val() }
            })
                .done(function( msg ) {
                    if (msg != '')
                        jQuery("#ctrnum").attr('value', msg);
                    else
                        jQuery("#ctrnum").attr('value', '');
                });
    });
});

</script>
<?php if($modid): ?>
	<?php
	$sql = "select modexigedsc from contratos.ctmodalidadecontrato where modid = {$modid}";
	$rsMod = $db->pegaUm($sql);
	
	if($rsMod == 't'): 
	?>
		<script>
			jQuery(document).ready(function(){

                jQuery('#ctrnum').blur(function() {
                        alert("ok");
                });

				jQuery('#tr_modexigedsc').show();
				<?php 
				if ($desabilitado != true){
				?>
				jQuery('[name=modexigedsc]').attr('disabled', false).removeClass('disabled').addClass('normal obrigatorio').removeAttr('readonly');
				<?php 
				}
				?>
			});
		</script>
	<?php endif; ?>
<?php endif; ?>
