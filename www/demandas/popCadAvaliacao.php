<?php


date_default_timezone_set ('America/Sao_Paulo');


// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );   
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';


// abre conexão com o servidor de banco de dados
$db = new cls_banco();





$dmdid = $_REQUEST['dmdid'];
$_SESSION['dmdid'] = $dmdid;

$codseg = $_REQUEST['codseg'];



if(!$dmdid){
	print "<script>
				alert('Acesso Negado. Acesse novamente o link para avaliar a demanda!');
				window.close(); 
		   </script>";
	exit;
}

if(!$codseg){
	print "<script>
				alert('Acesso Negado. Acesse novamente o link para avaliar a demanda!');
				window.close(); 
		   </script>";
	exit;
}
elseif($codseg != 'simecok'){
	
	$sql = "SELECT dmdcodseg FROM demandas.demanda where dmdid = $dmdid and dmdcodseg = '$codseg'";
	$dados = $db->PegaUm($sql);
	if(!$dados){
		print "<script>
					alert('Acesso Negado. Acesse novamente o link para avaliar a demanda!');
					window.close(); 
			   </script>";
		exit;		
	} 
}


//recupera o cpf do solicitante para gravar na auditoria
$sql = "SELECT usucpfdemandante FROM demandas.demanda where dmdid = {$dmdid}";
$usucpforigem = $db->PegaUm($sql);
$_SESSION['usucpforigem'] = $usucpforigem;
$_SESSION['usucpf'] = $usucpforigem;



function inserirAv(){
	global $db;
	
	$sql = " INSERT INTO demandas.avaliacaodemanda
			 (
			 	dmdid, avdprobres, avdtempo, avdtecnico, avdgeral, avsobs, avdstatus, avddata
			 ) VALUES (
			 	".$_SESSION['dmdid'].", 
			 	'".$_POST['avdprobres']."',
			 	'".$_POST['avdtempo']."',
			 	'".$_POST['avdtecnico']."',
			 	'".$_POST['avdgeral']."',
			 	'".$_POST['avsobs']."',
			 	'A',
			 	'".date('Y-m-d H:i:s')."'
			 );";
	$db->executar($sql, false);
	$db->commit();
}


/*
function alterarAv(){
	global $db;
	
	$sql = "UPDATE 
				demandas.avaliacaodemanda 
			SET 
				avdprobres = '".$_POST['avdprobres']."',
				avdtempo = '".$_POST['avdtempo']."',
				avdtecnico = '".$_POST['avdtecnico']."',
				avdgeral = '".$_POST['avdgeral']."',
				avsobs = '".$_POST['avsobs']."',
				avddata = '".date('Y-m-d H:i:s')."'
			WHERE 
				avdid = ".$_POST['avdid'];

	$db->executar($sql);					
	$db->commit();
}

*/




if($_POST){
	
	
	/*
	if(!$_POST['avdid']) {
		inserirAv();
	}
	else{
		alterarAv();
	}
	*/
	
	$sql = "select dmddatainclusao 
			from demandas.demanda 
			where dmdid = ".$_SESSION['dmdid']."
			and (dmddatainclusao + INTERVAL '30 DAYS') > now()";
	$dmddatainclusao = $db->pegaUm($sql);
	
	if(!$dmddatainclusao){

		print "<script> alert('Não é Possível avaliar, pois já se passaram mais de 30 dias do atendimento desta demanda!'); </script>";
		
	}
	else{
	
		inserirAv();
		
		
		//envia email para os gestores
		if($_POST['avdgeral'] == '1' || $_POST['avdgeral'] == '2'){
			
			if($_POST['avdgeral'] == '1') $flag = "RUIM";
			if($_POST['avdgeral'] == '2') $flag = "REGULAR";
			
			enviaEmailAvaliacaoRuim($_SESSION['dmdid'], $flag, $_POST['avsobs']);
			
			/*
			$assunto = "Demanda [{$_SESSION['dmdid']}] - Avaliada como {$flag} pelo solicitante";
			$conteudo = "A demanda <b>Nº {$_SESSION['dmdid']}</b> foi avaliada como {$flag}
					 <BR>
					 <b>Justificativa do usuário:</b> {$_POST['avsobs']}";
			*/
			
			
			/*
			$remetente = array('nome'=>REMETENTE_WORKFLOW_NOME, 'email'=>REMETENTE_WORKFLOW_EMAIL);
			$emailCopia = "";
			
			
			$sql = "SELECT
			 		 od.ordid as codorigem
					FROM
					 demandas.demanda d 
					 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
					 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
					WHERE
					 d.dmdid = {$_SESSION['dmdid']}";	
		
			$dado = (array) $db->pegaLinha($sql);
		
			//pega o email do gerente
			//origem diferente de sistema
			if($dado['codorigem'] != '1'){
				
				$sqlSuporteAtend = " UNION ALL
					 	             SELECT 'avaliacaosimec@mec.gov.br' ";
				//4=Logistica e 8=banco de dados
				if($dado['codorigem'] == '4' || $dado['codorigem'] == '8') $sqlSuporteAtend = "";
				
				$sqlx = "select distinct u2.usuemail from demandas.demanda d
						INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
						LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.ordid = ts.ordid AND
					 												   ur.rpustatus = 'A' AND
					 												   ur.pflcod = ".DEMANDA_PERFIL_ADMINISTRADOR."
					 	LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf		
					 	WHERE
					 		d.dmdid = {$_SESSION['dmdid']}
					 	$sqlSuporteAtend 	
					 	";
				$dadox = (array) $db->carregarColuna($sqlx);
				//$gerente = implode("; ", $dadox);
				
				//$emailCopia = "servicedesk@mec.gov.br";
			}
			else{ //origem = sistema
				$sqlx = "select distinct u2.usuemail from demandas.demanda d
						INNER JOIN demandas.sistemacelula sc ON sc.sidid = d.sidid	
						LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = sc.celid AND
					 												   ur.rpustatus = 'A' AND
					 												   ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO."
					 	LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf		
					 	WHERE
					 		d.dmdid = {$_SESSION['dmdid']}";
				$dadox = (array) $db->carregarColuna($sqlx);
				//$gerente = implode(";", $dadox);
				
				//$emailCopia = $gerente;
			}		
	
	
			foreach($dadox as $dadox2){
				$destinatario = $dadox2;
				if($destinatario){
					enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
				}	
			}
			*/
			
		}
		

		?>
		<script> 
			alert('Avaliação enviada com sucesso! \n\nObrigado(a) por avaliar o nosso atendimento. \nSua avaliação é muito importante para o sistema demandas.'); 
		</script>
		<?
		
	}
	
	unset($_POST);
	
	?>
	<script> 
		window.close(); 
		location.href='popCadAvaliacao.php?dmdid=<?=$dmdid?>&codseg=<?=$codseg?>';
	</script>
	<?
	//exit();
	
}


/*
// Carrega Avaliação, já salva.
$sql = "SELECT avdid, dmdid, avdprobres, avdtempo, avdtecnico, avdgeral, avsobs, avdstatus 
		FROM demandas.avaliacaodemanda where dmdid = {$dmdid}";
$dados = $db->carregar($sql);
if($dados) extract($dados[0]);
*/


print '<br>';

monta_titulo( 'Avaliação da Demanda - Cód. # '.$dmdid, '' );
?>
<html>
 <head>
  <script type="text/javascript" src="../includes/funcoes.js"></script>
  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
  <script type="text/javascript">
	function validaForm(){
	 	d = document;

		if(!d.formA.avdprobres[0].checked && !d.formA.avdprobres[1].checked){
			alert ('É necessário responder a questão de nº 1!');
			return false;
		}
		if(!d.formA.avdtempo[0].checked && !d.formA.avdtempo[1].checked && !d.formA.avdtempo[2].checked && !d.formA.avdtempo[3].checked){
			alert ('É necessário responder a questão de nº 2!');
			return false;
		}
		if(!d.formA.Truim.checked && !d.formA.Tregular.checked && !d.formA.Tbom.checked && !d.formA.Totimo.checked){
			alert ('É necessário responder a questão de nº 3!');
			return false;
		}
		if(!d.formA.Gruim.checked && !d.formA.Gregular.checked && !d.formA.Gbom.checked && !d.formA.Gotimo.checked){
			alert ('É necessário responder a questão de nº 4!');
			return false;
		}
		if((d.formA.Gruim.checked || d.formA.Gregular.checked) && d.formA.avsobs.value==''){
			alert ('É necessário justificar a questão de nº 4 no campo Observação');
			return false;
		}

		d.formA.btncad.disabled = true;
		return true;
		
		
	}	
  </script>
 </head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">


<?php 

$sql = "SELECT
			 dmdtitulo,
			 od.orddescricao ||' - '|| ts.tipnome AS origem,
			 CASE 
			  	WHEN u.usunome != '' THEN  upper(u.usunome)
			  	ELSE  upper(d.dmdnomedemandante)
			 END as solicitante			 
			FROM
			 demandas.demanda d
			 LEFT JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 LEFT JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
			WHERE
			 dmdid = {$dmdid}";
	$dados = $db->carregar($sql);
	extract($dados[0]);



	$cab = "<table align=\"center\" class=\"Tabela\" style='border-bottom:2px solid #000;'>
			 <tbody>
				<tr>
					<td width='30%'  style=\"text-align: right;\" class=\"SubTituloEsquerda\">Solicitante:</td>
					<td  style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$solicitante}</td>
				</tr>
			 	<tr>
					<td  style=\"text-align: right;\" class=\"SubTituloEsquerda\">Serviço Solicitado:</td>
					<td  style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dmdtitulo}</td>
				</tr>
				<tr>
					<td  style=\"text-align: right;\" class=\"SubTituloEsquerda\">Origem Demanda:</td>
					<td  style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$origem}</td>
				</tr>								 
			 </tbody>
			</table>";
	
	echo $cab;
?>

<form id="formA" name="formA" action="" method="post" onsubmit="return validaForm();">

<input type="hidden" name="avdid" value="<?=$avdid?>">
<input type="hidden" name="dmdid" value="<?=$dmdid?>">
<input type="hidden" name="codseg" value="<?=$codseg?>">

<table border=0 class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="30%" class="subtitulodireita">1. Sua solicitação foi atendida e seu problema foi resolvido?</td>
		<td >
			<input type="radio" name="avdprobres" value="S" <?if($avdprobres == 'S') echo 'checked';?>> Sim
			&nbsp;&nbsp;&nbsp;
			<input type="radio" name="avdprobres" value="N" <?if($avdprobres == 'N') echo 'checked';?>> Não
		</td>
	</tr>		
	<tr>
		<td class="subtitulodireita">2. O tempo que demorou para você receber o atendimento foi:</td>
		<td >
			<input type="radio" name="avdtempo" value="M" <?if($avdtempo == 'M') echo 'checked';?>> Muito Bom
			&nbsp;&nbsp;&nbsp;
			<input type="radio" name="avdtempo" value="B" <?if($avdtempo == 'B') echo 'checked';?>> Bom
			&nbsp;&nbsp;&nbsp;
			<input type="radio" name="avdtempo" value="A" <?if($avdtempo == 'A') echo 'checked';?>> Aceitável
			&nbsp;&nbsp;&nbsp;
			<input type="radio" name="avdtempo" value="T" <?if($avdtempo == 'T') echo 'checked';?>> Teria que ser mais rápido
		</td>
	</tr>		
	<tr>
		<td class="subtitulodireita">3. Sobre o técnico,<br> como foi o tratamento e atenção dispensados durante o atendimento,<br> sua postura, o nível do conhecimento sobre o assunto,<br> o interesse em resolver o problema?</td>
		<td >
			<?
			$opcoes = array
				(
					"Ruim" => array
					(
							"valor" => "1",
							"id"    => "Truim"	
					),
					"Regular" => array
					(
							"valor" => "2",
							"id"    => "Tregular"	
					),
					"Bom" => array
					(
							"valor" => "3",
							"id"    => "Tbom"	
					),
					"Ótimo" => array
					(
							"valor" => "4",
							"id"    => "Totimo"	
					)					
				);
			campo_radio( 'avdtecnico', $opcoes, 'h' );	
		?>	
		</td>
	</tr>		

	<tr>
		<td class="subtitulodireita">4. No geral o atendimento foi:</td>
		<td>
			<?
			$opcoes = array
				(
					"Ruim" => array
					(
							"valor" => "1",
							"id"    => "Gruim"	
					),
					"Regular" => array
					(
							"valor" => "2",
							"id"    => "Gregular"	
					),
					"Bom" => array
					(
							"valor" => "3",
							"id"    => "Gbom"	
					),
					"Ótimo" => array
					(
							"valor" => "4",
							"id"    => "Gotimo"	
					)					
				);
			campo_radio( 'avdgeral', $opcoes, 'h' );	
		?>	
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Observação:</td>
		<td >
			<?=campo_textarea('avsobs', 'N ', $habil, '', 80, 5, 4000); ?>
		</td>
	</tr>		
	<tr bgcolor="#C0C0C0">
		<td>&nbsp;</td>
		<td>
	    	<input type='submit' class='botao' value='Salvar' name='btncad' id='btncad' <?= $habil == 'N' ? 'disabled="disabled"' : ''?> />&nbsp;
	    	<input type='button' class='botao' value='Fechar' name='fechar' onclick='window.close();'> 	
		</td>			
	</tr>
	
	<tr>
		<td colspan="2" bgcolor="#f5f5f5" height="30" ><b>Suas avaliações:</b></td>
	</tr>
	
</table>

<?php
		$sql = "SELECT '<center>' || to_char(avddata::timestamp,'DD/MM/YYYY HH24:MI') || '</center>',  
					   (CASE avdprobres
						 	WHEN 'S' THEN '<center>SIM</center>'
						 	WHEN 'N' THEN '<center>NÃO</center>'
						END) AS avdprobres,
						(CASE avdtempo
						 	WHEN 'M' THEN '<center>MUITO BOM</center>'
						 	WHEN 'B' THEN '<center>BOM</center>'
						 	WHEN 'A' THEN '<center>ACEITÁVEL</center>'
						 	WHEN 'T' THEN '<center>TERIA QUE SER MAIS RÁPIDO</center>'
						END) AS avdtempo,
						(CASE avdtecnico
						 	WHEN '1' THEN '<center>RUIM</center>'
						 	WHEN '2' THEN '<center>REGULAR</center>'
						 	WHEN '3' THEN '<center>BOM</center>'
						 	WHEN '4' THEN '<center>ÓTIMO</center>'
						END) AS avdtecnico,
						(CASE avdgeral
						 	WHEN '1' THEN '<center>RUIM</center>'
						 	WHEN '2' THEN '<center>REGULAR</center>'
						 	WHEN '3' THEN '<center>BOM</center>'
						 	WHEN '4' THEN '<center>ÓTIMO</center>'
						END) AS avdgeral,
		 				avsobs 
				FROM demandas.avaliacaodemanda 
				where avdstatus='A' and dmdid = {$dmdid}
				order by avddata desc";
		
		$cabecalho = array( "Data Inclusão","Item 1" , "Item 2", "Item 3", "Item 4", "Observação");
		$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '', '');
		
		
		$sqlCount = "select count(1) from (" . $sql . ") rs";
		$totalRegistro = $db->pegaUm($sqlCount);
		
	?>


</form>
</body>

<script>
	var dmdid = "<?=$dmdid?>";
	var total = "<?=$totalRegistro?>";
	if(document.formA.codseg.value == 'simecok' && parseInt(total) > 0){
		parent.document.getElementById('div'+dmdid).innerHTML = "<font color=blue><b>SIM</b></font>";
	}
</script>
