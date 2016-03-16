<?php
//dbg($_REQUEST,1);
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
if ($_GET['download']  ){
	$obFile = new FilesSimec(NULL, NULL, "pdeescola");//passando nome do esquema para corrigir erro "arquivo não encontrado na pasta"
	$obFile->getDownloadArquivo( $_GET['arqid'] );
}

if($_REQUEST['requisicao']=='telaSubirArquivo') {
	$dadosarquivo = $db->pegaLinha("SELECT arqnome||'.'||arqextensao as nomearquivo, arqtamanho FROM public.arquivo WHERe arqid='".$_REQUEST['arqid']."'");
	die("<script>function limpaUpload(arqid){document.getElementById('arquivo_' + arqid).value = \"\";}</script><form method=post enctype=multipart/form-data id=formulario_ name=formulario><input type=hidden name=requisicao value=subirarquivo><input type=hidden name=_sisdiretorio value=pdeescola><table class=tabela><tr><td class=SubTituloDireita>Nome do arquivo:</td><td>".$dadosarquivo['nomearquivo']."</td></tr><tr><td class=SubTituloDireita>Tamanho (bytes):</td><td>".$dadosarquivo['arqtamanho']."</td></tr><tr><td class=SubTituloDireita>Selecione o Arquivo:</td><td><input type=file name=arquivo[".$_REQUEST['arqid']."] id=arquivo_".$_REQUEST['arqid']." > <img onclick=limpaUpload('".$_REQUEST['arqid']."') src=../imagens/excluir.gif /></td></tr><tr><td colspan=2 class=SubTituloCentro><input type=button value=Enviar onclick=document.getElementById('formulario_').submit();> <input type=button name=fechar value=Fechar onclick=closeMessage();></td></tr></table></form>");
}

if($_REQUEST['requisicao']=='subirarquivo') {
	
	if($_FILES['arquivo']) {
		
		include APPRAIZ ."includes/funcoes_public_arquivo.php";
		
		$resp = atualizarPublicArquivo();
		
		if($resp['TRUE']) $msg = 'Arquivo atualizado com sucesso';
		else {
			if($resp['FALSE']) {
				$msg .= 'Problemas encontrados:'.'\n';
				foreach($resp['FALSE'] as $k => $v) {
					$msg .= $v .'\n';
				}
			}
		}
		
		die("<script>
				alert('".$msg."');
				window.location = window.location;
			 </script>");
	}
	
}

//$entrada = "11.123.123.123,20";
function trataNumero($num)
{
	preg_match_all("([0-9/./,/null])", $num, $saida);
	return implode("",$saida[0]);	
}
//dbg(trataNumero($entrada));

# Verfica se o pdeid e intid estão na sessão
pde_verificaSessao();

$pdeid 	= $_SESSION["pdeid"];
$pseid = $_REQUEST['pseid'];

$perfis = arrayPerfil();
$perfilSalvar = ((in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $perfis)) || (in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL, $perfis)) || (in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfis)) || (in_array(PDEESC_PERFIL_MONITORAMENTO_ESTADUAL, $perfis)) || (in_array(PDEESC_PERFIL_MONITORAMENTO_MUNICIPAL, $perfis)));


if (!$pdeid){
	die('<script>
			alert(\'Falta parametros!\nTente novamente.\');
			history.go(-1);
		 </script>');
}

#Remove todo o registro
if($_POST['excluirRegistroMonitoramento'] && $_POST['moaid']){
	
	$campos	= array("moaid" => $_POST['moaid']);
	$file   = new FilesSimec("monitoramentoacao", $campos, "pdeescola");
	// Remove o registro do anexo antigo (pelo arqid) e o arquivo físico
	$file->setRemoveUpload( $removeAnexo['arqid'] );
	die;
}

#Remove o anexo, setando NULL
if($_POST['excluirAnexoMonitoramento'] && $_POST['moaid']){
	
	$sqlRemoveAnexo = "SELECT arqid FROM pdeescola.monitoramentoacao WHERE moaid = ".$_POST['moaid'];
	$arqid = $db->pegaUm($sqlRemoveAnexo);
	
	if($arqid){
		$sql = "UPDATE public.arquivo set arqstatus = 'I' WHERE arqid = ".$arqid;
		$executado = $db->executar($sql);
		if(!$executado){
			echo "erro";
		}
	}
	
	$sqlNullAnexo = "UPDATE 
					pdeescola.monitoramentoacao
				 SET
				 	arqid = NULL 
		         WHERE
		         	moaid = {$_POST['moaid']}";

	if($db->executar($sqlNullAnexo)){
		echo "ok";
		$db->commit();
		$file   = new FilesSimec("monitoramentoacao", $campos, "pdeescola");
		$file->excluiArquivoFisico( $arqid );
	}else{
		echo "erro";
	}
	die;
}

//trabalhando o array para jogar na function setUpload
if(is_array($_FILES["anexo"])){
	
	foreach($_FILES["anexo"] as $chave => $file){
		$n = 0;
		foreach($file as $k => $f){
			$arrFiles[$k][$chave] = $f;
			$n++;
		}
	}
	$_FILES = $arrFiles;
}

$sqlDpaid = "SELECT   distinct
				dpa.dpaid
			FROM
				pdeescola.planosuporteestrategico pseobj
			INNER JOIN 
				pdeescola.planosuporteestrategico pseest ON pseest.pseidpai = pseobj.pseid
			INNER JOIN 
				pdeescola.planosuporteestrategico psemet ON psemet.pseidpai = pseest.pseid
			INNER JOIN 
				pdeescola.planoacao pa ON pa.pseid = psemet.pseid
			INNER JOIN
				pdeescola.detalheplanoacao dpa ON dpa.plaid = pa.plaid
			INNER JOIN 
				pdeescola.fonterecurso as f on f.forid = dpa.forid --and dpa.forid = 5
 			WHERE
				pseobj.pdeid = {$_SESSION['pdeid']}
			GROUP BY
				dpa.dpaid
			ORDER BY
				dpa.dpaid";
//ver($sqlDpaid,d);

$arDpaid = $db->carregarColuna($sqlDpaid);

# Salvar ou alterar monitoramento
if ($_POST['acaom'] == 'Salvar' || $_POST['acaom'] == 'Alterar'){
	$tamanho = count($arDpaid);
	for($x=0;$x<$tamanho;$x++){    //criando um for para cada plano
		$dpaid = $arDpaid[$x];
		
  		$tsmid = $_POST['tsmid'][$dpaid] ? $_POST['tsmid'][$dpaid] : "null";
  		$moaresursorealizado = $_POST['moaresursorealizado'][$dpaid] ? str_replace(',','.',str_replace('.','',trataNumero($_POST['moaresursorealizado'][$dpaid]))) : "null";
  		$moaresursorealizado = is_numeric($moaresursorealizado) ? $moaresursorealizado : "null";
  		
			if($_FILES[$dpaid]["tmp_name"]){
				
				$sqlDelete = "delete from pdeescola.monitoramentoacao where dpaid = {$dpaid}";
				$db->executar($sqlDelete);
		  		
				$campos	= array("dpaid"	        		=> $dpaid,
								"moadata"				=> "now()",
								"tsmid"          	    => $tsmid,
								"moaresultadoalcancado" => "'".$_POST['moaresultadoalcancado'][$dpaid]."'",
								"moarecursorealizado"	=> $moaresursorealizado,
								"moajustificativa"		=> "'".$_POST['moajustificativa'][$dpaid]."'");
				
				$file = new FilesSimec("monitoramentoacao", $campos, "pdeescola");
				$arquivoSalvo = $file->setUpload(null,$dpaid);
	
			}else{
				
				$sql = "SELECT dpaid FROM pdeescola.monitoramentoacao WHERE dpaid = ".$dpaid;
				$testaDpaid = $db->pegaUm( $sql );
				
				if( !$testaDpaid ){				
					$sqlSalva = "INSERT INTO 
								      pdeescola.monitoramentoacao 
								    ( dpaid,
						              moadata,
						              tsmid,
						              moaresultadoalcancado,
						              moarecursorealizado,
						              moajustificativa
						            ) VALUES (
						            	'".$dpaid."',
						            	'now()', 
						            	".$tsmid.",
						            	'" . pg_escape_string( $_POST['moaresultadoalcancado'][$dpaid] ) . "',
						            	".$moaresursorealizado.",
						            	'" . pg_escape_string( $_POST['moajustificativa'][$dpaid] ) . "')";
					$arquivoSalvo = $db->executar($sqlSalva);
				}
				else{
					$sqlAltera = "UPDATE 
							pdeescola.monitoramentoacao
						 SET
						 	moadata				  = 'now()',
						 	tsmid				  = " . $tsmid .",
						 	moaresultadoalcancado = '" . pg_escape_string( $_POST['moaresultadoalcancado'][$dpaid] ) . "',
						 	moarecursorealizado	  = " .$moaresursorealizado . ",
						 	moajustificativa	  = '" . pg_escape_string( $_POST['moajustificativa'][$dpaid] ) . "'
				         WHERE
				         	dpaid = {$dpaid}";
					$arquivoSalvo = $db->executar($sqlAltera);
				} 
			}
	}

	$db->commit();

	if($arquivoSalvo){
		echo '<script type="text/javascript"> alert("Operação Realizada com Sucesso!\n Não esqueça de apresentar os resultados do monitoramento para o Conselho/ Colegiado Escolar, guardar uma cópia da ata e anexá-la em meio eletrônico aqui no SIMEC.");</script>';
		echo"<script>window.location.href = 'pdeescola.php?modulo=principal/monitoramento/monitoramentoPlanoAcao&acao=A';</script>";
		die;
	}
}

/*
if ($_POST['acaom'] == 'Alterar'){

	for($x=0;$x<count($arDpaid);$x++){ //varrendo para alterar plano por plano
		$dpaid = $arDpaid[$x];
		
		$tsmid = $_POST['tsmid'][$dpaid] ? $_POST['tsmid'][$dpaid] : "null";
  		$moaresursorealizado = $_POST['moaresursorealizado'][$dpaid] ? $_POST['moaresursorealizado'][$dpaid] : "null";
		
		if($_FILES[$dpaid]["tmp_name"]){
			
			$sqlDelete = "delete from pdeescola.monitoramentoacao where dpaid = {$dpaid}";
			$db->executar($sqlDelete);

			$campos	= array("dpaid"	        		=> $dpaid,
							"moadata"				=> "now()",
							"tsmid"          	    => $tsmid,
							"moaresultadoalcancado" => "'".$_POST['moaresultadoalcancado'][$dpaid]."'",
							"moarecursorealizado"	=> str_replace(',','.',str_replace('.','',trataNumero($moaresursorealizado))),
							"moajustificativa"		=> "'".$_POST['moajustificativa'][$dpaid]."'");

			$file = new FilesSimec("monitoramentoacao", $campos, "pdeescola");
			
			if($_POST['arqid'][$dpaid])
			{
				$file->setRemoveUpload( $_POST['arqid'][$dpaid] );// Remove o registro do anexo antigo (pelo arqid) e o arquivo físico
			}
				$file->setUpload(null,$dpaid);// faz o insert novamente dos dados e do anexo
				
		}else{//Update de arquivos sem anexo e já salvos anteriormente

			$sqlAltera = "UPDATE 
							pdeescola.monitoramentoacao
						 SET
						 	moadata				  = 'now()',
						 	tsmid				  = " . $tsmid .",
						 	moaresultadoalcancado = '" . pg_escape_string( $_POST['moaresultadoalcancado'][$dpaid] ) . "',
						 	moarecursorealizado	  = " . str_replace(',','.',str_replace('.','',trataNumero($moaresursorealizado))).",
						 	moajustificativa	  = '" . pg_escape_string( $_POST['moajustificativa'][$dpaid] ) . "'
				         WHERE
				         	dpaid = {$dpaid}";
			//dbg($sqlAltera,1);
			$arquivoSalvo = $db->executar($sqlAltera); 
	    }
	
	}
	    $db->commit();
	    
		if($arquivoSalvo){
			echo '<script type="text/javascript"> alert("Operação Realizada com Sucesso!\n Não esqueça de apresentar os resultados do monitoramento para o Conselho/ Colegiado Escolar, guardar uma cópia da ata e anexá-la em meio eletrônico aqui no SIMEC.");</script>';
			echo"<script>window.location.href = 'pdeescola.php?modulo=principal/monitoramento/monitoramentoPlanoAcao&acao=A';</script>";
			die;	
		}
}
*/
		
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
 

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
echo montarAbasArray(carregaAbas(), "/pdeescola/pdeescola.php?modulo=principal/monitoramento/monitoramentoPlanoAcao&acao=A");
monta_titulo('Plano de Ação ', '');
echo cabecalhoPDE();
?> 

<?php
	$sqlObj  = "SELECT * FROM pdeescola.planosuporteestrategico WHERE pdeid = '{$_SESSION['pdeid']}' AND pseidpai IS NULL ORDER BY pseid";
	$objetivo = $db->carregar( $sqlObj  );
	$objetivo = $objetivo ? $objetivo : array();
	
	if($objetivo){
		$boHabilitado = true;
		foreach( $objetivo as $arObjetivo ){
		
			$sqlEstr = "SELECT * FROM pdeescola.planosuporteestrategico    
			    		WHERE pseidpai = '{$arObjetivo['pseid']}'
						ORDER BY pseid ";
		                     
			$estrategia = $db->carregar( $sqlEstr );
			$estrategia = $estrategia ? $estrategia : array();
			
			foreach( $estrategia as $arEstrategia ){

				$sqlMeta = "SELECT * FROM pdeescola.planosuporteestrategico    
							WHERE pseidpai = '{$arEstrategia['pseid']}'
							ORDER BY pseid ";
            
				$meta = $db->carregar( $sqlMeta );
				$meta = $meta ? $meta : array();
				
				foreach( $meta as $arMeta ){
					
					$sqlPlanos = " SELECT
										pa.plaid,
										pa.pseid,
										ps.pseidpai, 
										pa.plaliderobjetivo,
							            pa.plaindicadormeta,
							            pa.plagerenteplanoacao,
							            to_char(pa.pladatainicio::date,'MM/YYYY') as pladatainicio,
							            tr.tirdescricao,
							            to_char(pa.pladatatermino::date,'MM/YYYY') as pladatatermino,
							            ps.psedescricao
							        FROM
							         	pdeescola.planoacao AS pa
							        INNER JOIN 
							         	pdeescola.planosuporteestrategico AS ps ON ps.pseid = pa.pseid
							        INNER JOIN
							         	pdeescola.tiporevisao AS tr ON pa.tirid = tr.tirid
							        WHERE 
									    --ps.pdeid = '$pdeid'
									    --and 
									    ps.pseid = '{$arMeta["pseid"]}'
									ORDER BY ps.pseid";

					$rsPlanosMetas = $db->pegaLinha($sqlPlanos);
	
					$msg = "Não informado";
						
					$plaid 				 = $rsPlanosMetas['plaid'];
					$pseid 				 = $rsPlanosMetas['pseid'];
					$plaliderobjetivo 	 = $rsPlanosMetas['plaliderobjetivo'] ? $rsPlanosMetas['plaliderobjetivo'] : $msg ;
					$plaindicadormeta 	 = $rsPlanosMetas['plaindicadormeta'] ? $rsPlanosMetas['plaindicadormeta'] : $msg ;
					$plagerenteplanoacao = $rsPlanosMetas['plagerenteplanoacao'] ? $rsPlanosMetas['plagerenteplanoacao'] : $msg;
					$pladatainicio		 = $rsPlanosMetas['pladatainicio'] ? $rsPlanosMetas['pladatainicio'] : $msg ;
					$pladatarevisao 	 = $rsPlanosMetas['tirdescricao'] ? $rsPlanosMetas['tirdescricao'] : $msg ;
					$pladatatermino 	 = $rsPlanosMetas['pladatatermino'] ? $rsPlanosMetas['pladatatermino'] : $msg ;					

					$_SESSION ['pdeescola']['plaid'] = $rsPlanosMetas['plaid']; 
	
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
jQuery.noConflict();
</script>


<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />

<script type="text/javascript">
				messageObj = new DHTML_modalMessage();	// We only create one object of this class
				messageObj.setShadowOffset(5);	// Large shadow
				
				function displayMessage(url) {
					messageObj.setSource(url);
					messageObj.setCssClassMessageBox(false);
					messageObj.setSize(690,400);
					messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
					messageObj.display();
				}
				function displayStaticMessage(messageContent,cssClass) {
					messageObj.setHtmlContent(messageContent);
					messageObj.setSize(600,150);
					messageObj.setCssClassMessageBox(cssClass);
					messageObj.setSource(false);	// no html source since we want to use a static message here.
					messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
					messageObj.display();
				}
				function closeMessage() {
					messageObj.close();	
				}
				function substituirArquivoCorrompido(arqid){
					displayMessage(window.location.href+'&requisicao=telaSubirArquivo&arqid=' + arqid,false);
				}
</script>

<!-- cabeçalho com detalhamento Plano de Acao -->
<table  align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
	<tr>
		<td class="subtitulodireita" width="20%"><b>Objetivo Estratégico:</b></td> 
		<td align="left"><?php echo $arObjetivo["psedescricao"]; ?></td>
    </tr>	
    <tr>
		<td class="subtitulodireita"><b>Líder do Objetivo:</b></td> 
		<td align="left"><?php echo $plaliderobjetivo; ?></td>
   	</tr>	
	<tr>
		<td class="subtitulodireita"><b>Estratégia:</b></td> 
		<td align="left"><?php echo $arEstrategia["psedescricao"]; ?> </td>
	</tr>	 
	<tr>
		<td class="subtitulodireita"><b>Meta:</b></td> 
		<td align="left"><?php echo $arMeta["psedescricao"]; ?> </td>
	</tr>	 
	<tr>
		<td class="subtitulodireita"><b>Indicador da Meta:</b> </td> 
		<td align="left"> <?php echo $plaindicadormeta; ?></td>
	</tr> 
	<tr>
		<td class="subtitulodireita"> <b>Gerente de Plano de Ação:</b> </td> 
		<td align="left"> <?php echo $plagerenteplanoacao; ?></td>
	</tr>	
    <tr>
		<td class="subtitulodireita"> <b>Início:</b> </td> 
		<td align="left"> <?php echo $pladatainicio; ?> </td>
    </tr>	
    <tr>
		<td class="subtitulodireita"> <b>Revisão:</b> </td> 
		<td align="left"> <?php echo $pladatarevisao; ?> </td>
    </tr>	
    <tr>
		<td class="subtitulodireita"> <b>Data Término:</b> </td> 
		<td align="left"> <?php echo $pladatatermino; ?></td>
	</tr>	
</table>
							  	
<!-- Inicio monitoramento -->
<?php

	$sqlDados = " SELECT DISTINCT 
					dp.dpadescricaoacao,
					dp.dpaid,
	                dparesponsavel,
	                dparesultadoesperado,
	                dpaindicador,
					dp.dpavalorcapital,
					dp.dpavalorcusteio,
	                tp.tprdescricao,
	                tp.tprid,
	                tc.tcaid,
	                tc.tcadescricao,
	              	ts.tsmid,
	             	ts.tsmdescricao,
	                ma.moaresultadoalcancado,
	                fr.fordescricao,
	                fr.forid,
	                ma.moarecursorealizado,
	                ma.moajustificativa,
	                ma.moaid,
			        ma.arqid,
			     	'<a id=\"link_' || ma.moaid || '\"; style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/monitoramento/monitoramentoPlanoAcao&acao=A&download=S&arqid=' || arq.arqid || '\';\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>' as anexo
              	FROM 
				    pdeescola.detalheplanoacao as dp
			  	INNER JOIN 
				    pdeescola.fonterecurso as fr on fr.forid = dp.forid --and dp.forid = 5
			  	LEFT JOIN 
				    pdeescola.tipocategoria as tc on tc.tcaid = dp.tcaid
			  	LEFT JOIN 
				    pdeescola.tipoprograma as tp on tp.tprid = dp.tprid
			    LEFT JOIN
			 		pdeescola.monitoramentoacao as ma on ma.dpaid = dp.dpaid 
                LEFT JOIN
			 	    pdeescola.tiposituacaomonitoramento as ts on ts.tsmid = ma.tsmid
			  	LEFT JOIN 
 			 	    public.arquivo as arq ON arq.arqid = ma.arqid
			  	WHERE 
				    plaid = '$plaid'
		    	ORDER BY
		    		dpaid;";

	//ver( $sqlDados );
    if($plaid != '' ){			
		$dados = $db->carregar( $sqlDados );
		$dados = ($dados) ? $dados : array();
		$_SESSION['pdescola']['dpaid'] = $dados[$k]['dpaid'];
	}
	
	/*
	$dadoNovo = $dados;
	$resultado = array();
	foreach( $dados as $v => $dado ){
		$sql = "SELECT
					ts.tsmid,
	                ts.tsmdescricao,
	                ma.moaresultadoalcancado,
	                ma.moarecursorealizado,
	                ma.moajustificativa,
	                ma.moaid,
					ma.arqid,
					'<a id=\"link_' || ma.moaid || '\"; style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/monitoramento/monitoramentoPlanoAcao&acao=A&download=S&arqid=' || arq.arqid || '\';\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>' as anexo
				FROM
					pdeescola.monitoramentoacao ma
				LEFT JOIN
			        pdeescola.tiposituacaomonitoramento as ts on ts.tsmid = ma.tsmid
			  	LEFT JOIN 
				    public.arquivo as arq ON arq.arqid = ma.arqid
				WHERE
					ma.dpaid = ".$dado['dpaid'];

		$dad = $db->pegaLinha( $sql );
		if(is_array($dad)){
			$resultado[] = array_merge( (array)$dadoNovo[$v], (array)$dad );
		}
	}	
	$dados = $resultado;
	*/
?>
<form name=formulario id=formulario method=post enctype="multipart/form-data">
		<?=subCabecalho(' Monitoramento das metas em Planos de Ação');?>

			<table width="95%" align="center" border="3" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
				<thead>
				<tr>
					<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						Nº
					</td> 
					<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						Descrição
					</td>
					<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						Situação
					</td>
					<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						Resultado Esperado
					</td>
					<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						Resultado Alcançado
					</td>
					<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						Recurso Previsto Capital
					</td>
					<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						Recurso Previsto Custeio
					</td>
					<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						Recurso Realizado (R$)
					</td>
					<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						Quem Financia
					</td>
					<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						Justificativa/Ocorrências 
					</td>
					<td class="anexoT" title="(notas fiscais, atas e outros registros)" align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						Anexos 
					</td>
				</tr> 
				</thead>
				
				<tbody>
					<?php
					if(count($dados) == 0){
						echo "<tr><td colspan='11' align='center'>
								<table width='95%' align='center' border='0' cellspacing='0' cellpadding='2' style='color:333333;' class='listagem'>
									<tr>
										<td style='color:red;' align='center';>
											Não foram encontrados Registros.
										</td>
									</tr>
							  	</table>
							  </td>
							  </tr>";
					}else{
						//loop preenche dados do Desdobramento das Metas em Planos de Ação
						for( $k = 0; $k < count( $dados ); $k++ ){
							
							if( $dados[$k]['moaid'] != '' ){
								$boHabilitado = false;
							} ?>
						
							<tr bgcolor="" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
								<td style="border-color: #c0c0c0; border-right: 1px solid #c0c0c0;" title="Número">				
									<b><?=$k+1 ?></b>
								</td> 
								
								<td style="border-color: #c0c0c0; border-right: 1px solid #c0c0c0;" title="Descrição">
									<?=$dados[$k]['dpadescricaoacao']?>
									<input type='hidden' id='dpaid' name='dpaid[]' value='<?=$dados[$k]['dpaid']?>'>
								</td>
								
								<td style="border-color: #c0c0c0; border-right: 1px solid #c0c0c0;" title="Situação">
								    <?$sqlSituacao = "SELECT 
								    						tsmid as codigo, 
								    						tsmdescricao as descricao 
								    				  FROM 
								    				  		pdeescola.tiposituacaomonitoramento 
								    				  ORDER BY descricao asc";
								    
							        $tsmid = $dados[$k]['tsmid'];//substituindo o value do componente, caso já tenha sido preenchido
									$db->monta_combo( 'tsmid['. $dados[$k]['dpaid'] .']', $sqlSituacao, 'S', 'Selecione...', '', '','','','','combo','',$tsmid );?>
								</td>
								
								<td style="border-color: #c0c0c0; border-right: 1px solid #c0c0c0;" title="Resultado Esperado">
									<?=$dados[$k]['dparesultadoesperado']?>
								</td>
								
								<td style="border-color: #c0c0c0; border-right: 1px solid #c0c0c0;" title="Resultado Alcançado">
								  	<?$moaresultadoalcancado =  stripslashes($dados[$k]['moaresultadoalcancado']);?>
								    <?=campo_textarea('moaresultadoalcancado['. $dados[$k]['dpaid'] .']', 'N', 'S', '', 20, 5,'','','','','','',$moaresultadoalcancado); ?>
								</td>
								
								<td style="border-color: #c0c0c0; border-right: 1px solid #c0c0c0;" title="Recurso Previsto Capital">
									<?if((($dados[$k]['dpavalorcapital'])!= '') || (($dados[$k]['dpavalorcapital'])!= NULL)){?>
										<?=$dados[$k]['dpavalorcapital']?>
							    	<?}else{?>
							    		<left> 0,00 </left>
							    	<?} ?>
								</td>
									
								<td style="border-color: #c0c0c0; border-right: 1px solid #c0c0c0;" title="Recurso Previsto Custeio">
									<?if((($dados[$k]['dpavalorcusteio'])!= '') || (($dados[$k]['dpavalorcusteio'])!= NULL)){?>
										<?=$dados[$k]['dpavalorcusteio']?>
							    	<?}else{?>
							    		<left> 0,00 </left>
							    	<?} ?>									
								</td>
								
								<td style="border-color: #c0c0c0; border-right: 1px solid #c0c0c0;" title="Recurso Realizado (R$)">
								  	<?if($dados[$k]['forid'] != 8){?>
							    		<?$moarecursorealizado = str_replace('.',',',$dados[$k]['moarecursorealizado']);?>				    
										<?=campo_texto( 'moaresursorealizado['. $dados[$k]['dpaid'] .']', 'N', 'S', '', 10, 20, '', '', 'left', '', 0, 'id="moarecursorealizado" onkeyup="this.value=mascaraglobal(\'##.###.###,##\',this.value);" onfocus="this.select();"','',$moarecursorealizado); ?>
									<?}else{?>
										<?=$dados[$k]['fordescricao']?>
										<?echo '<input type="hidden" name="moaresursorealizado['. $dados[$k]["dpaid"] .']" id="moarecursorealizado">';?>
									<?}?>
									<?echo '<input type="hidden" name="forid['. $dados[$k]["dpaid"] .']" value="'. $dados[$k]["forid"] .'">';?>
								</td>
								
								<td style="border-color: #c0c0c0; border-right: 1px solid #c0c0c0;" title="Quem Financia">
									<?=$dados[$k]['fordescricao']?>
								</td>
								
								<td style="border-color: #c0c0c0; border-right: 1px solid #c0c0c0;" title="Justificativa">
									<?$moajustificativa =  trim(stripslashes($dados[$k]['moajustificativa']));?>
									<?=campo_textarea('moajustificativa['. $dados[$k]['dpaid'] .']', 'N', 'S', '', 25, 5,'','','','','','',$moajustificativa);?>
								</td>
								
								<td style="border-color: #c0c0c0; border-right: 1px solid #c0c0c0;" title="Anexo">
									<?$anexo = $dados[$k]['anexo'];
										if($anexo){?>
											<?php if(!verificaExistenciaArquivo($dados[$k]['arqid'])): ?>
												<span style="background-color:red"><?=$anexo;?></span> <img border="0" title="Arquivo Corrompido" align="absmiddle" src="../imagens/restricao.png"> &nbsp;&nbsp;&nbsp;<a onclick="novoAnexo(this,'link_<?=$dados[$k]['moaid']?>', 'anexoArq_<?=$dados[$k]['moaid']?>','<?php echo $dados[$k]['moaid']; ?>');"></a>
												<img title="Alterar anexo" style="border: 0pt none ; cursor: pointer;" onclick="novoAnexo(this,'link_<?=$dados[$k]['moaid']?>', 'anexoArq_<?=$dados[$k]['moaid']?>','<?php echo $dados[$k]['moaid']; ?>');" id="img_alterar_<?php echo $dados[$k]['moaid']; ?>" src="../imagens/alterar.gif">
												<input type="button" value="Substituir" onclick="substituirArquivoCorrompido('<?php echo $dados[$k]['arqid'] ?>')" />
											<?php else: ?>
												<?=$anexo;?>&nbsp;&nbsp;&nbsp;<a onclick="novoAnexo(this,'link_<?=$dados[$k]['moaid']?>', 'anexoArq_<?=$dados[$k]['moaid']?>','<?php echo $dados[$k]['moaid']; ?>');"></a>
												<img title="Alterar anexo" style="border: 0pt none ; cursor: pointer;" onclick="novoAnexo(this,'link_<?=$dados[$k]['moaid']?>', 'anexoArq_<?=$dados[$k]['moaid']?>','<?php echo $dados[$k]['moaid']; ?>');" id="img_alterar_<?php echo $dados[$k]['moaid']; ?>" src="../imagens/alterar.gif">
											<?php endif; ?>
											<img title="Excluir anexo" style="border: 0pt none ; cursor: pointer;" id="img_excluir_<?php echo $dados[$k]['moaid']; ?>" onclick="excluirAnexoMonitoramento('<?php echo $dados[$k]['moaid']; ?>');" src="../imagens/excluir.gif">
											<!--<img src="/imagens/excluir.gif" border=0 title="Excluir" style="cursor:pointer;" onclick="Excluir('?modulo=inicio&acao=E&cmpid=' || cam.cmpid || '','Deseja realmente excluir este anexo?');\">-->
											<input type="hidden" name="arqid[]" value="<?=$dados[$k]['arqid']; ?>" />
											<input type="hidden" name="moaid[]" value="<?=$dados[$k]['moaid']; ?>" />
											<input id="anexoArq_<?php echo $dados[$k]['moaid']; ?>" style="display: none;" type="file" name="anexo[<?= $dados[$k]['dpaid']?>]" />
									<?	}else{?>
											<input type="hidden" name="moaid[]" value="<?php echo $dados[$k]['moaid']; ?>" />
											<input type="file" name="anexo[<?= $dados[$k]['dpaid']?>]" />
									<?	}
									?>
								</td>
							</tr>
				   <?php }//fim for{ $dados}
					}//fim count($dados)
			       ?>
				</tbody>
			</table>			
			<br>
		     <? }// fim foreach meta
	  		}//fim foreach estrategia
		}//fim foreach objetivo
	}else{//fim if($objetivo)
		die('<script>
			 alert(\'Não foram encontrados Registros.\');
			 history.go(-1);
	 		 </script>');
	}
?>
<? $sqlMoadata = $db->pegaUm("SELECT
								DISTINCT
					 			to_char( moadata ,'DD/MM/YYYY') as data
							  FROM
								pdeescola.planosuporteestrategico pseobj
							  INNER JOIN 
							  	pdeescola.planosuporteestrategico pseest ON pseest.pseidpai = pseobj.pseid
							  INNER JOIN 
							  	pdeescola.planosuporteestrategico psemet ON psemet.pseidpai = pseest.pseid
							  INNER JOIN 
							  	pdeescola.planoacao pa ON pa.pseid = psemet.pseid
							  INNER JOIN 
							  	pdeescola.detalheplanoacao dpa ON dpa.plaid = pa.plaid
							  INNER JOIN 
							  	pdeescola.monitoramentoacao ma ON ma.dpaid = dpa.dpaid
							  WHERE
								pseobj.pdeid = {$_SESSION['pdeid']}");?>	
								
	<div style="text-align: center; font-size:10px; background-color:#F8F8FF;">
		<?if (!$boHabilitado){?>
			Data da última atualização:&nbsp;&nbsp;<?=$sqlMoadata?>
		<?} ?>		
	</div>
	
	<br>
	<br>
	
	<div align="center">
		<? 
			/*
			if( $boHabilitado ){
				$value = 'Salvar';
			}else{
				$value = 'Alterar';
			}
			*/
		
			$value = 'Salvar';
			
			if($perfilSalvar){?>
				<input type="button" name="botao" value="<?=$value ?>" onclick="validaForm()">
				<input type="hidden" id="acaom" value="<?=$value ?>" name="acaom" >
	  <?php }else{?>
				<input type="button" disabled="disabled" name="botao" value="<?=$value ?>" onclick="validaForm()">
	  <?php } ?>
	</div>
</form>
        
<script><!--

	function salvaAnexo(campo){
		
		var anexo = $(campo);

		alert($(campo));
		alert(anexo.value);
			
	}
	
	function validaForm(){

		var arDpaid = eval('<?=simec_json_encode($arDpaid)?>');
		var campoVazio = false;	
		for (i=0; i <  arDpaid.length; i++){
			var dpaid = arDpaid[i];

			var situacao  = document.getElementsByName('tsmid[' + dpaid + ']')[0];
			var resultado = document.getElementsByName('moaresultadoalcancado[' + dpaid + ']')[0];
			var recurso   = document.getElementsByName('moaresursorealizado[' + dpaid + ']')[0];
			var justif    = document.getElementsByName('moajustificativa[' + dpaid + ']')[0];
			var forid     = document.getElementsByName('forid[' + dpaid + ']')[0];
//			var anexo     = document.getElementsByName('anexo[]');


			
			if (resultado.value.length > 100){
				alert('Você excedeu o nº máximo de 100 caracteres no campo Resultado Esperado!"');
				resultado.focus();
				return false;
			}
			
			if((situacao.value == '3' && recurso.value == '') && forid.value != '8'){
//				if (recurso.value == ''){
					alert('O campo "Recurso Realizado" é obrigatório!"');
					recurso.focus();
					return false;
//				}
			}

			if((situacao.value == '4') || (situacao.value == '5') ||(situacao.value == '6')){
				if (justif.value == ''){
					alert('O campo "justificativa" é obrigatório!"');
					justif.focus();
					return false;
				}
			}
			
			if (justif.value.length > 1000){
				alert('Você excedeu o nº máximo de 1000 caracteres no campo Justificativa!"');
				justif.focus();
				return false;
			}
			
			if((situacao.value == '') || (resultado.value == '') || (recurso.value == '') || (justif.value == '')) {
				campoVazio = true;				  
			}
		}

		if(campoVazio){
			//var resposta = confirm('O monitoramento não está concluido. Deseja continuar?');
		  	if( confirm('O monitoramento não está concluído. Tem certeza de que deseja finalizar?') ){
//		  		if( !confirm('Você poderá concluir o Monitoramento em outro momento.')){
//			  		return false;
//		  		} 
		  		alert('Você poderá concluir o Monitoramento em outro momento.');
		  		document.formulario.submit();
		  	}else{
			  	return false;
		  	}  		
		}else{
			document.formulario.submit();
		}
	}
	
	function novoAnexo( obj, link, file , moaid){  
		obj.style.display = 'none';
		document.getElementById( link ).style.display = 'none';
		document.getElementById( file ).style.display = '';
		if(moaid){
			excluirRegistroMonitoramento(moaid);
		}
	}

	function excluirRegistroMonitoramento(moaid){
		new Ajax.Request(
				'pdeescola.php?modulo=principal/monitoramento/monitoramentoPlanoAcao&acao=A',
				{
					method: 'post',
					parameters: '&excluirRegistroMonitoramento=true&moaid=' + moaid,
					onComplete: function(res){
						//window.location.reload();
					}
				});
	}

	function excluirAnexoMonitoramento(moaid){

		if(confirm('Deseja realmente excluir este anexo?')){
			
			new Ajax.Request(
					//passando parâmetros
					'pdeescola.php?modulo=principal/monitoramento/monitoramentoPlanoAcao&acao=A&',
					{
						method: 'post',
						parameters: 'excluirAnexoMonitoramento=true&moaid=' + moaid,
						onComplete: function(res){
						//tratando retorno - neste caso, necessário. 
						//Como a exclusão já aconteceu por trás do código, ocultando file de anexar e imagens para alterar imagem.
							if(res.responseText == "ok"){
								document.getElementById( 'anexoArq_' + moaid).style.display = '';
								document.getElementById( 'link_' + moaid).style.display = 'none';
								document.getElementById( 'img_alterar_' + moaid).style.display = 'none';
								document.getElementById( 'img_excluir_' + moaid).style.display = 'none';
							}else{
								alert('Não foi possível excluir o anexo!');
							}
							//document.getElementById('erro').innerHTML = res.responseText;//teste para exibir string na tela 1
						}
					});
		}		
	}
	

--></script>
<!--<div id="erro"></div>--><!-- teste para exibir string na tela -->