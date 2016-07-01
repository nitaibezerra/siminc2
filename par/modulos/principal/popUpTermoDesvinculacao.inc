<?php

// nome
// cpf
// crn
// crn regiao
// data desvinculacao 
// Motivo
// Entidade


// Salva o formulário

function getEmailRespNutri($inuid)
{
	
	global $db;
	
	$sql = "
		SELECT iu.itrid,
			CASE WHEN iu.itrid = 2 THEN
				iu.muncod
			WHEN iu.itrid = 1 THEN
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
			$esfera = 'Município';
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
			$esfera = 'Estado';
		}
		
		$resultEmail = $db->pegalinha($sqlEmail);
	
		$emailTo =  $resultEmail['email'];
		if( ! $emailTo )
		{
			return false;
		}
		else
		{
			return array(  
					'email' =>$emailTo,
					'esfera' => $esfera
			) ;
		}
}



if($_REQUEST['requisicao']== 'formulario_motivo')
{
	
	$vndatadesvinculacao	= $_POST['vndatadesvinculacao'];
	$vnid					= $_POST['vnid'];
	$vnmotivo				= utf8_decode($_POST['vnmotivo']);

	$data = $db->pegaUm("select vndatadesvinculacao from par.vinculacaonutricionista where vnid = {$vnid}");

	if(! $data)
	{
		$arrDt = explode("/", $vndatadesvinculacao);
		$dataDesvinculacao = $arrDt[2].'-'.$arrDt[1].'-'.$arrDt[0];
		$upData = ",vndatadesvinculacao = '{$dataDesvinculacao}'";
	}
	else
	{
		$upData = '';
	}

	$sqlVu = "
		UPDATE
			par.vinculacaonutricionista
		SET
			vnmotivo = '{$vnmotivo}'
		$upData
		WHERE
			vnid =	{$vnid}
		";
	$db->executar($sqlVu);

	if($db->commit())
	{
	echo "sucesso";
			die();
	}
	else
	{
	echo "erro";
	}

}



function mesTextual($mes){
	$meses	= array("01" => "Janeiro",
			"02" => "Fevereiro",
			"03" => "Março",
			"04" => "Abril",
			"05" => "Maio",
			"06" => "Junho",
			"07" => "Julho",
			"08" => "Agosto",
			"09" => "Setembro",
			"10" => "Outubro",
			"11" => "Novembro",
			"12" => "Dezembro",
			"1" => "Janeiro",
			"2" => "Fevereiro",
			"3" => "Março",
			"4" => "Abril",
			"5" => "Maio",
			"6" => "Junho",
			"7" => "Julho",
			"8" => "Agosto",
			"9" => "Setembro",);
	return $meses[$mes];
}

$vnid 					= $_REQUEST['vnid'];
$vndatadesvinculacao 	= $_REQUEST['vndatadesvinculacao'];
$dunid					= $_REQUEST['dunid'];

$sqlDadosDoc = "

	SELECT 
		vn.vnid,
		dn.entid,
		ent.entnome,
		vn.vnassinado,
		CASE WHEN inu.mun_estuf IS NULL THEN
			'Estado'
 		ELSE
			'Municipio'
		END
			as tipoEsfera,
		TO_CHAR( dn.dncpf::bigint, 'FM000\".\"000\".\"000\"-\"00') as dncpf,
		dn.dncrn,
		dn.dncrnuf,
		to_char(vn.vndatadesvinculacao,'DD/MM/YYYY') as vndatadesvinculacao,
		vn.vnmotivo,
		CASE WHEN inu.estuf IS NULL THEN
			m.mundescricao || '-' || inu.mun_estuf
		ELSE
			estdescricao
		END
		as entidade
	
	FROM
		par.vinculacaonutricionista  vn
	INNER JOIN par.instrumentounidade inu ON inu.inuid = vn.inuid
	INNER JOIN par.dadosnutricionista dn	ON dn.dncpf = vn.vncpf
	left JOIN entidade.entidade ent ON ent.entid = dn.entid
	LEFT JOIN territorios.municipio m ON m.muncod = inu.muncod
	LEFT JOIN territorios.estado e ON e.estuf = inu.estuf
	
	WHERE
		vnid = '$vnid'	
";


$resultTermo = $db->pegaLinha($sqlDadosDoc);


$entid 					= $resultTermo['entid'];
$entnome 				= $resultTermo['entnome'];
$cpf					= $resultTermo['dncpf'];
$dncrn 					= $resultTermo['dncrn'];
$dncrnuf 				= $resultTermo['dncrnuf'];
$tipoEsfera				= $resultTermo['tipoesfera'];
$vndatadesvinculacaobd 	= $resultTermo['vndatadesvinculacao'];
$vnmotivo				= $resultTermo['vnmotivo'];
$vnassinado				= $resultTermo['vnassinado'];
$entidade 				= $resultTermo['entidade'];


//if( ( $entnome == '' ) || ( $cpf == ''  ) || ( $dncrn == ''  ) || ( $dncrnuf == ''  ) || ( $vndatadesvinculacao == ''  ) || ( $entidade == ''  ) )



// Salva o formulário
if($_POST['formulario_assinatura'])
{
	
	$vnid		= $_POST['vnid'];
	$dadosNutri = $db->pegaLinha(
		"SELECT 
			inuid, entnome  from par.vinculacaonutricionista  vn
		INNER JOIN par.dadosnutricionista dn ON dn.dncpf = vn.vncpf
		INNER JOIN entidade.entidade 	 ent 	ON ent.entid = dn.entid
		WHERE 
			vnid = {$vnid}");
	
	$inuid 		= $dadosNutri['inuid'];
	$nomeNutri	= $dadosNutri['entnome'];
	
	$dunid					= $_POST['dunid'];
	
	$obDadosUnidade = new DadosUnidade($dunid);
	
	$obDadosUnidade->excluir($dunid);
		
	if($obDadosUnidade->commit()){
	
				
		$sqlVu = "
		UPDATE
			par.vinculacaonutricionista
		SET
			vnassinado 	= true,
			vnstatus 	= 'I',
			vndataassinatura = 'now()',
			usucpfalteracao = '{$_SESSION['usucpf']}'
		WHERE
			vnid = 	{$vnid}
		";
		
		$db->executar($sqlVu);
		
		$inuid = $db->pegaUm("select inuid from par.vinculacaonutricionista where vnid = {$vnid}");
		
		//@todo responsável email
		if($db->commit())
		{
			
			$arrEmail 	= getEmailRespNutri($inuid);
			if($arrEmail)
			{
				$email		= $arrEmail['email'];
				$esfera		= $arrEmail['esfera'];
				
				
				// . $dopTexto
				
				$strAssunto = "Aviso de desvinculação de nutricionista";
								
				$remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
			
				$strMensagem =
"
<pre align=\"center\" style=\"text-align: justify;\"  >
	Informamos que o(a) nutricionista  {$nomeNutri} assinou o Termo de Desvinculação do Programa Nacional de Alimentação Escolar nesta data.
Observe se o número de nutricionistas vinculados ao seu  {$esfera} está de acordo com os parâmetros numéricos definidos na Resolução CFN nº 465/2010.

http://simec.mec.gov.br

Atenciosamente,
Equipe SIMEC/PAR.
</pre>";			
				if( $_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost" )
				{		
					
				}
				elseif($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br")
				{
					$strEmailTo = array($_SESSION['email_sistema']);
					$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
					
				}
				else
				{
					$strEmailTo = $email;
					$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
					
				}
				
			}
			
			
			
			echo " <script>alert('Dados salvos com sucesso');
					window.parent.opener.location.reload()
				   </script>";
		}
		else
		{
		echo "<script>alert('Erro ao salvar os dados');</script>";
		}
		
		
	}
}

?>

<html>
<head>
<style>

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 100%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
</style>
  <script src="../library/jquery/jquery-1.11.1.min.js" type="text/javascript" charset="ISO-8895-1"></script>
    <script src="../library/jquery/jquery-ui-1.10.3/jquery-ui.min.js"></script>
    <script src="../library/chosen-1.0.0/chosen.jquery.js"></script>
    <script language="javascript">$1_11 = $.noConflict();</script>

</head>




<body marginwidth="0" marginheight="0" bgcolor="#ffffff" leftmargin="0" topmargin="0">

<?php 
monta_titulo( "Termo de Desvinculação", "Alimentação Escolar" );


$cpfNutricionista = $_SESSION['usucpf'];

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<!-- JAVASCRIPT -->
<script>

function assinarTermoDesvinculacao(vnid, data)
{
	if(data == 'ativo')
	{
	 	if( jQuery('#data_desvincula'+vnid).val() == '')
	 	{
			alert( "Preencha o campo \"Data de desvinculação\"");
	 	}
	 	else
	 	{
			
	 	}
	}
	else
	{
		alert(data);
	}	
}

function visualizaTermoDesvinculacao(vnid)
{
	alert('visualiza');
}

function selecionaEntidade(vnid)
{
		var selecionado = jQuery('input:radio[name=radiovnid]:checked').val();

		jQuery('form#formulario :radio').each(function() {

			idLoop = jQuery(this).val();
			if( idLoop == vnid )
			{
				jQuery('#div_termo'+vnid).show();
				jQuery('#div_data'+vnid).show();
			}
			else
			{	
				jQuery('#div_termo'+idLoop).hide();
				jQuery('#div_data'+idLoop).hide();
			}

		
		//alert();

	
	});
	//div_termo
	//div_data
	/*
	jQuery('#formulario_validado').show();
	jQuery('[name=cpf]').val(mascaraglobal('###.###.###-##',jQuery('[name=cpf]').val()));			

	jQuery('#formulario_validado').hide();
	*/
}


function salvarMotivo()
{
	
	if( jQuery('textarea#vnmotivo').val() != '')
	{

		vndatadesvinculacao = jQuery('#vndatadesvinculacao').val();
		vnid 				= jQuery('#vnid').val();
		vnmotivo			= jQuery('textarea#vnmotivo').val();
				
		jQuery.ajax({
	        type: 'post',
	        url:  'par.php?modulo=principal/popUpTermoDesvinculacao&acao=A',
	        data: 'requisicao=formulario_motivo&vnid=' + vnid + '&vndatadesvinculacao='+ vndatadesvinculacao + '&vnmotivo='+vnmotivo,
	        async: false,
	        success: function (res){
	
				if(res == 'sucesso')
				{
					jQuery('#span_parecer').html(jQuery('textarea#vnmotivo').val());
					jQuery('#span_parecer').show();
					jQuery('#div_termo').show();
			    	return true;
				}
				else
				{
					alert(res);
				}
					
			}
		});

		
	}
	else
	{
		alert(' O campo "Motivo da desvinculação" tem preenchimento obrigatório');
	}

}



function salvarAssinatura()
{

	//$entidade
	if (  ( confirm( "Assinando este termo você confirma a desvinculação com a entidade <?=$entidade?>?" ) )) 
	{
		jQuery('#formulario_assinatura').submit();
	}
}


jQuery(document).ready(function(){


	var snid = jQuery('#snidVal').val();
	if( snid == 1 )//Validou
	{
		jQuery('#formulario_validado').show();
		jQuery('[name=cpf]').val(mascaraglobal('###.###.###-##',jQuery('[name=cpf]').val()));			
	}
	else// Não validou
	{
		jQuery('#formulario_validado').hide();
		
	}

	
	jQuery('#endcep1').blur(function(){

		if( jQuery(this).val().length == 9 ){
	
			var endcep = jQuery(this).val();
			
			jQuery.ajax({
		        type: 'post',
		        url:  '/geral/consultadadosentidade.php',
		        data: 'requisicao=pegarenderecoPorCEP&endcep=' + endcep,
		        async: false,
		        success: function (res){
			        
		        	var dados = res.split("||");
		        	jQuery('#endlog1').val(dados[0]);
					jQuery('#endbai1').val(dados[1]);
					jQuery('#mundescricao1').val(dados[2]);
					jQuery('#estuf1').val(dados[3]);
					if(! dados[4])
					{
						alert('Cep inexistente. Insira um CEP válido');
						jQuery('#muncod1').val('');
					}
					jQuery('#muncod1').val(dados[4]);
				}
			});
		}
	});
});


</script>

<?php 

// /onclick="self.close();
// <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">

if($vndatadesvinculacaobd != '')
{
	$vndatadesvinculacao = $vndatadesvinculacaobd;
}

if( ( $entnome == '' ) || ( $cpf == ''  ) || ( $dncrn == ''  ) || ( $dncrnuf == ''  ) || ( $vndatadesvinculacao == ''  ) || ( $entidade == ''  ) )
{
	?>
		
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
			<tr>
				<td>
					<center> Para gerar o termo é necessário completar o cadastro na aba "Cadastro". </center>
				</td>
			</tr>
			<tr>
				<td>
					<center> <input type="button" value="Fechar" onclick="self.close();">  </center>
				</td>
			</tr>
		</table>
	<?php 
	
}
else
{
	if ( $vnassinado =='f' )
	{
		?>
			<form>
			<input type="hidden" id="vndatadesvinculacao" value="<?= $vndatadesvinculacao ?>"/>
			<input type="hidden" id="vnid" value="<?= $vnid ?>"/>
			
				<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
					<tr >
						<td class="SubTituloDireita"> Motivo da desvinculação:</td>
						<td>
							<textarea rows="8" id="vnmotivo" name="vnmotivo"  cols="60"><?= $vnmotivo ?></textarea>
						</td>
					</tr>
					<tr>
						<td colspan="2">
							<center> <input type="button" value="Gerar Termo" onclick="salvarMotivo();">  </center>
						</td>
					</tr>
				</table>
			</form>
			<?php
	}
	
		
		?>
					<form action="" method="post" name="formulario_assinatura" id="formulario_assinatura">
					<input type="hidden" name="formulario_assinatura" value="1"/>
					<input type="hidden" name="entidade" value="<?=$entidade?>"/>
					<input type="hidden" name="dunid"    value="<?=$dunid?>"/>
					<input type="hidden" name="vndatadesvinculacao" value="<?= $vndatadesvinculacao ?>"/>
					<input type="hidden" name="vnid" value="<?= $vnid ?>"/>
					
						<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
							<tr >
								<td align="center">

	<div  style='width: 700; 
	 		min-height:300;
	 		background-color: white;
	 		padding: 10px;
	 		<?php echo ( $vnmotivo =='' ) ? "display: none;" : "" ?>
	 		'
	 	  id="div_termo"
	  >

<center>
       <br>
<b style="font-size: 15"> DECLARAÇÃO DE DESVINCULAÇÃO </b>
</center>
<br><br><br>
<p style="font: 10pt Arial,verdana;  text-align: justify !important; " >
	&nbsp; &nbsp;&nbsp;&nbsp; Eu, <?=$entnome ?>,CPF <?=$cpf?>, CRN Nº <?=$dncrn?>, Região  "<?=$dncrnuf?>"  declaro que, a
partir da data de <?= $vndatadesvinculacao ?>, por motivo(s)  <span id="span_parecer" style="<?php echo ( $vnmotivo =='' ) ? "display: none;" : "" ?>" > <?= $vnmotivo ?></span>, me desligo das incumbências 
a mim atribuídas como nutricionista do Programa Nacional de Alimentação Escolar (PNAE)
no <?= $tipoEsfera?> de <?=$entidade?>.

<br><br>
					<?php if( $vnassinado =='f' )
					{  ?>
						Em, <?= Date('d') ?> de  <?= mesTextual(Date('m'))?> de <?= Date('Y') ?>
					<?php 
					}else{
						$result = $db->pegaLinha(
							" SELECT 
								ent.entnome,
								to_char( vn.vndataassinatura , 'DD') as dia,
								to_char( vn.vndataassinatura , 'MM') as mes,
								to_char( vn.vndataassinatura , 'YYYY') as ano,
								TO_CHAR( dn.dncpf::bigint, 'FM000\".\"000\".\"000\"-\"00') as dncpf
							FROM
								par.vinculacaonutricionista  vn
							INNER JOIN par.instrumentounidade inu ON inu.inuid = vn.inuid
							INNER JOIN par.dadosnutricionista dn	ON dn.dncpf = vn.vncpf
							left JOIN entidade.entidade ent ON ent.entid = dn.entid
							LEFT JOIN territorios.municipio m ON m.muncod = inu.muncod
							LEFT JOIN territorios.estado e ON e.estuf = inu.estuf
							WHERE
								vnid = {$vnid}
						");
						
					
					?>
						Em, <?= $result['dia'] ?> de  <?= mesTextual($result['mes'])?> de <?= $result['ano'] ?>
						<br>
						<br>
						<br>
						<br>
						<br>
						<br>
						
						<center>Validado por <?= $result['entnome'] ?>, CPF nº <?= $result['dncpf'] ?> em, <?= $result['dia'] ?> de  <?= mesTextual($result['mes'])?> de <?= $result['ano'] ?></center> 
					<?php
					}
					?>
</p>

								<?php if( $vnassinado =='f' )
								{  ?>
									<br>
									<br>
									<br>
									<br>
									<br>
									
									<center> <input type="button" value="Assinar" onclick="salvarAssinatura();">  </center>
								<?php 
								}		
								?>
</div>								
								</td>
								
							</tr>
							<tr>
								<td >
								<?php if( $vnassinado =='t' )
								{  ?>
									<center> <input type="button" value="Fechar" onclick="self.close();">  </center>

								<?php 
								}
?>
								</td>
							</tr>
						</table>
				</form>
					<?php
	
}
die();

$cpfNutricionista = $_SESSION['usucpf'];

?>

 
 
</div>
